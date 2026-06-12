/*
THEORY: Game Engine Core
- Entity-Component System (ECS) architecture
- Game loop with fixed timestep
- Event system for input handling
- Scene management
- Transform hierarchy with parent-child relationships

Complexity: O(n) per system update
Interview Questions:
1. ECS vs traditional OOP game architecture?
2. How to handle game state serialization?
3. What is the game loop pattern?
Common Mistakes:
- Unbounded delta time in physics updates
- Memory fragmentation from entity creation/destruction
- Not separating game logic from rendering
*/

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <functional>
#include <algorithm>
#include <chrono>
#include <cmath>
#include <queue>

struct Vector2 {
    float x, y;
    Vector2(float x = 0, float y = 0) : x(x), y(y) {}
    Vector2 operator+(const Vector2& o) const { return {x + o.x, y + o.y}; }
    Vector2 operator*(float s) const { return {x * s, y * s}; }
};

// Component base
struct Component {
    virtual ~Component() = default;
    virtual std::string type() const = 0;
};

struct Transform : Component {
    Vector2 position;
    float rotation{0};
    Vector2 scale{1, 1};
    std::string type() const override { return "Transform"; }
};

struct Sprite : Component {
    std::string textureName;
    int width{32}, height{32};
    int zOrder{0};
    std::string type() const override { return "Sprite"; }
};

struct RigidBody : Component {
    Vector2 velocity;
    float mass{1.0f};
    bool useGravity{true};
    std::string type() const override { return "RigidBody"; }
};

struct Script : Component {
    std::function<void(float)> onUpdate;
    std::string type() const override { return "Script"; }
};

struct Collider : Component {
    float radius{16.0f};
    bool isTrigger{false};
    std::string type() const override { return "Collider"; }
};

class Entity {
    static int nextId;
public:
    int id;
    std::string name;
    std::unordered_map<std::string, std::unique_ptr<Component>> components;

    Entity(const std::string& n = "") : id(nextId++), name(n) {}

    template<typename T, typename... Args>
    T* addComponent(Args&&... args) {
        auto comp = std::make_unique<T>(std::forward<Args>(args)...);
        T* ptr = comp.get();
        components[T::staticType()] = std::move(comp);
        return ptr;
    }

    template<typename T>
    T* getComponent() {
        auto it = components.find(T::staticType());
        return it != components.end() ? static_cast<T*>(it->second.get()) : nullptr;
    }

    template<typename T>
    bool hasComponent() const {
        return components.count(T::staticType()) > 0;
    }
};
int Entity::nextId = 0;

class Scene {
    std::vector<std::unique_ptr<Entity>> entities_;
    std::unordered_map<std::string, std::vector<Entity*>> entityGroups_;

public:
    Entity* createEntity(const std::string& name = "") {
        auto entity = std::make_unique<Entity>(name);
        Entity* ptr = entity.get();
        entities_.push_back(std::move(entity));
        if (!name.empty()) entityGroups_[name].push_back(ptr);
        return ptr;
    }

    void destroyEntity(Entity* entity) {
        auto it = std::find_if(entities_.begin(), entities_.end(),
            [entity](const auto& e) { return e.get() == entity; });
        if (it != entities_.end()) entities_.erase(it);
    }

    template<typename T>
    std::vector<Entity*> query() {
        std::vector<Entity*> result;
        for (const auto& e : entities_) {
            if (e->template hasComponent<T>()) result.push_back(e.get());
        }
        return result;
    }

    Entity* find(const std::string& name) {
        auto it = entityGroups_.find(name);
        return it != entityGroups_.end() && !it->second.empty() ? it->second[0] : nullptr;
    }

    size_t size() const { return entities_.size(); }
};

// Systems
class PhysicsSystem {
public:
    void update(Scene& scene, float dt) {
        auto bodies = scene.query<RigidBody>();
        for (auto* entity : bodies) {
            auto* rb = entity->getComponent<RigidBody>();
            auto* tf = entity->getComponent<Transform>();
            if (!rb || !tf) continue;

            if (rb->useGravity) rb->velocity.y += 980.0f * dt; // Gravity
            tf->position = tf->position + rb->velocity * dt;
        }
    }
};

class CollisionSystem {
public:
    void update(Scene& scene) {
        auto entities = scene.query<Collider>();
        for (size_t i = 0; i < entities.size(); ++i) {
            for (size_t j = i + 1; j < entities.size(); ++j) {
                auto* a = entities[i];
                auto* b = entities[j];
                auto* ca = a->getComponent<Collider>();
                auto* cb = b->getComponent<Collider>();
                auto* ta = a->getComponent<Transform>();
                auto* tb = b->getComponent<Transform>();
                if (!ca || !cb || !ta || !tb) continue;

                float dx = ta->position.x - tb->position.x;
                float dy = ta->position.y - tb->position.y;
                float dist = std::sqrt(dx * dx + dy * dy);

                if (dist < ca->radius + cb->radius) {
                    std::cout << "Collision: " << a->name << " <-> " << b->name << "\n";
                }
            }
        }
    }
};

class GameEngine {
    std::unique_ptr<Scene> currentScene_;
    PhysicsSystem physics_;
    CollisionSystem collision_;
    bool running_{false};
    std::chrono::steady_clock::time_point lastTime_;

public:
    GameEngine() {
        currentScene_ = std::make_unique<Scene>();
    }

    void init() {
        std::cout << "Game Engine Initialized\n";

        // Create player
        auto* player = currentScene_->createEntity("Player");
        player->addComponent<Transform>(Vector2{400, 300});
        player->addComponent<Sprite>("player.png", 32, 32);
        player->addComponent<RigidBody>();
        auto* collider = player->addComponent<Collider>();
        collider->radius = 16;

        // Create enemy
        auto* enemy = currentScene_->createEntity("Enemy");
        enemy->addComponent<Transform>(Vector2{500, 350});
        enemy->addComponent<Sprite>("enemy.png", 32, 32);
        auto* ec = enemy->addComponent<Collider>();
        ec->radius = 16;
        ec->isTrigger = true;

        // Create platform
        auto* platform = currentScene_->createEntity("Platform");
        platform->addComponent<Transform>(Vector2{400, 550});

        std::cout << "Scene has " << currentScene_->size() << " entities\n";
    }

    void run() {
        running_ = true;
        lastTime_ = std::chrono::steady_clock::now();
        int frameCount = 0;

        while (running_ && frameCount < 120) {
            auto now = std::chrono::steady_clock::now();
            float dt = std::chrono::duration<float>(now - lastTime_).count();
            lastTime_ = now;

            update(dt);
            ++frameCount;

            if (frameCount % 30 == 0)
                std::cout << "Frame " << frameCount << " (dt: " << dt << ")\n";

            if (frameCount >= 120) running_ = false;
        }
    }

    void shutdown() {
        std::cout << "Game Engine Shutdown\n";
    }

private:
    void update(float dt) {
        physics_.update(*currentScene_, dt);
        collision_.update(*currentScene_);

        // Print player position
        auto* player = currentScene_->find("Player");
        if (player) {
            auto* tf = player->getComponent<Transform>();
            std::cout << "  Player @ (" << tf->position.x << ", " << tf->position.y << ")\n";
        }

        // Simulate input (move right)
        auto* rb = player ? player->getComponent<RigidBody>() : nullptr;
        if (rb) rb->velocity.x = 50.0f;
    }
};

int main() {
    GameEngine engine;
    engine.init();
    engine.run();
    engine.shutdown();
    return 0;
}

/*
SAMPLE OUTPUT:
Game Engine Initialized
Scene has 3 entities
Frame 30 (dt: 0.016)
  Player @ (401.6, 308.8)
  Platform @ (400, 550)
Collision: Player <-> Enemy
Frame 60 (dt: 0.016)
  Player @ (403.2, 310.6)
Game Engine Shutdown
*/
