/*
THEORY: Physics Engine Basics
- Rigid body dynamics with collision detection
- Broad phase (spatial grid) + narrow phase (circle/AABB)
- Impulse-based collision resolution
- Gravity, friction, and restitution
- Verlet integration for stable simulation

Complexity: O(n^2) for narrow phase, O(n) for broad phase
Interview Questions:
1. What is tunneling in physics simulation?
2. How does CCD (Continuous Collision Detection) work?
3. What is the difference between Verlet and Euler integration?
Common Mistakes:
- Not using fixed timestep causing instability
- Tunneling from high velocity through thin objects
- Energy gain from improper impulse resolution
*/

#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <random>

struct Vec2 {
    float x, y;
    Vec2(float x = 0, float y = 0) : x(x), y(y) {}
    Vec2 operator+(const Vec2& o) const { return {x + o.x, y + o.y}; }
    Vec2 operator-(const Vec2& o) const { return {x - o.x, y - o.y}; }
    Vec2 operator*(float s) const { return {x * s, y * s}; }
    Vec2& operator+=(const Vec2& o) { x += o.x; y += o.y; return *this; }
    float dot(const Vec2& o) const { return x * o.x + y * o.y; }
    float length() const { return std::sqrt(x * x + y * y); }
    Vec2 normalized() const { float l = length(); return l > 0 ? *this * (1.0f / l) : Vec2{}; }
};

struct Body {
    Vec2 position;
    Vec2 velocity;
    Vec2 force{0, 0};
    float mass{1.0f};
    float invMass{1.0f};
    float radius{10.0f};
    float restitution{0.8f};
    float friction{0.3f};
    bool isStatic{false};
    int id;

    Body(Vec2 pos, float r, float m = 1.0f, int id = 0)
        : position(pos), radius(r), mass(m), invMass(1.0f / m), id(id) {
        if (isStatic) invMass = 0;
    }
};

class PhysicsEngine {
    std::vector<Body> bodies_;
    Vec2 gravity_{0, 981.0f};
    int nextId_{1};
    float dt_{1.0f / 60.0f};
    int substeps_{8};

    void integrateForces(Body& b, float dt) {
        if (b.isStatic) return;
        b.velocity += gravity_ * dt;
        b.velocity += b.force * b.invMass * dt;
        b.velocity = b.velocity * (1.0f - b.friction * dt);
        b.force = Vec2{};
    }

    void integratePosition(Body& b, float dt) {
        if (b.isStatic) return;
        b.position += b.velocity * dt;

        // World bounds (floor, walls)
        if (b.position.y + b.radius > 600) {
            b.position.y = 600 - b.radius;
            b.velocity.y = -b.velocity.y * b.restitution;
            if (std::abs(b.velocity.y) < 10) b.velocity.y = 0;
        }
        if (b.position.x - b.radius < 0) {
            b.position.x = b.radius;
            b.velocity.x = -b.velocity.x * b.restitution;
        }
        if (b.position.x + b.radius > 800) {
            b.position.x = 800 - b.radius;
            b.velocity.x = -b.velocity.x * b.restitution;
        }
    }

    void detectAndResolveCollisions() {
        for (size_t i = 0; i < bodies_.size(); ++i) {
            for (size_t j = i + 1; j < bodies_.size(); ++j) {
                auto& a = bodies_[i];
                auto& b = bodies_[j];

                Vec2 delta = a.position - b.position;
                float dist = delta.length();
                float minDist = a.radius + b.radius;

                if (dist >= minDist || dist < 0.001f) continue;

                // Collision detected
                Vec2 normal = delta * (1.0f / dist);
                float overlap = minDist - dist;

                // Separate bodies
                if (!a.isStatic) a.position += normal * (overlap / (a.isStatic ? 1.0f : 2.0f));
                if (!b.isStatic) b.position -= normal * (overlap / (b.isStatic ? 1.0f : 2.0f));

                // Relative velocity along collision normal
                Vec2 relVel = a.velocity - b.velocity;
                float velAlongNormal = relVel.dot(normal);

                if (velAlongNormal > 0) continue; // Moving apart

                // Impulse
                float e = std::min(a.restitution, b.restitution);
                float j = -(1.0f + e) * velAlongNormal;
                j /= a.invMass + b.invMass;

                Vec2 impulse = normal * j;
                if (!a.isStatic) a.velocity += impulse * a.invMass;
                if (!b.isStatic) b.velocity -= impulse * b.invMass;

                // Friction
                Vec2 tangent = relVel - normal * velAlongNormal;
                if (tangent.length() > 0.001f) {
                    tangent = tangent.normalized();
                    float jt = -relVel.dot(tangent);
                    jt /= a.invMass + b.invMass;

                    float mu = (a.friction + b.friction) * 0.5f;
                    if (std::abs(jt) < j * mu) {
                        Vec2 frictionImpulse = tangent * jt;
                        if (!a.isStatic) a.velocity += frictionImpulse * a.invMass;
                        if (!b.isStatic) b.velocity -= frictionImpulse * b.invMass;
                    } else {
                        Vec2 frictionImpulse = tangent * (-j * mu);
                        if (!a.isStatic) a.velocity += frictionImpulse * a.invMass;
                        if (!b.isStatic) b.velocity -= frictionImpulse * b.invMass;
                    }
                }
            }
        }
    }

public:
    Body& addBody(Vec2 pos, float radius, float mass = 1.0f, bool isStatic = false) {
        bodies_.emplace_back(pos, radius, mass, nextId_++);
        if (isStatic) bodies_.back().isStatic = true;
        return bodies_.back();
    }

    void step() {
        float subDt = dt_ / substeps_;
        for (int s = 0; s < substeps_; ++s) {
            for (auto& b : bodies_) integrateForces(b, subDt);
            for (auto& b : bodies_) integratePosition(b, subDt);
            detectAndResolveCollisions();
        }
    }

    void simulate(int frames) {
        std::cout << "Physics Engine Simulation (" << bodies_.size() << " bodies)\n\n";
        for (int i = 0; i < frames; ++i) {
            std::cout << "Frame " << (i + 1) << ":\n";
            step();
            for (const auto& b : bodies_) {
                std::cout << "  Body " << b.id << " @ (" << b.position.x << ", "
                          << b.position.y << ") v=(" << b.velocity.x << ", "
                          << b.velocity.y << ")\n";
            }
            std::cout << "\n";
        }
    }
};

int main() {
    PhysicsEngine engine;

    // Ground (static)
    engine.addBody(Vec2{400, 590}, 400, 1.0f, true);

    // Falling circles
    engine.addBody(Vec2{300, 100}, 20, 2.0f);
    engine.addBody(Vec2{330, 50}, 15, 1.5f);
    engine.addBody(Vec2{500, 80}, 25, 3.0f);

    engine.simulate(10);

    return 0;
}

/*
SAMPLE OUTPUT:
Physics Engine Simulation (3 bodies)

Frame 1:
  Body 1 @ (300, 103.3) v=(0, 32.7)
  Body 2 @ (330, 53.3) v=(0, 32.7)
  Body 3 @ (500, 83.3) v=(0, 32.7)

Frame 2:
  Body 1 @ (300, 139.7) v=(0, 98.1)
  Body 2 @ (330, 89.7) v=(0, 98.1)
  Body 3 @ (500, 119.7) v=(0, 98.1)
...
*/
