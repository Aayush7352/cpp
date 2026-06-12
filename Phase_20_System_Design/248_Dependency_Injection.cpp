/*
THEORY: Dependency Injection
- Design pattern that passes dependencies to objects rather than creating them internally
- Improves testability, flexibility, and separation of concerns
- Types: Constructor injection, Setter injection, Interface injection
- DI Container: manages dependency resolution and lifetimes
- Reduces coupling and supports the Dependency Inversion Principle

Complexity: O(n) container resolution
Interview Questions:
1. DI vs Service Locator trade-offs?
2. How to handle circular dependencies?
3. What are the different lifetimes? (transient, scoped, singleton)
Common Mistakes:
- Over-using DI for simple objects
- Constructor explosion (too many parameters)
- Service Locator anti-pattern (hiding dependencies)
*/

#include <iostream>
#include <memory>
#include <unordered_map>
#include <functional>
#include <typeindex>
#include <any>

// Interfaces
class ILogger {
public:
    virtual ~ILogger() = default;
    virtual void log(const std::string& msg) = 0;
};

class IDatabase {
public:
    virtual ~IDatabase() = default;
    virtual void save(const std::string& data) = 0;
};

// Implementations
class ConsoleLogger : public ILogger {
public:
    void log(const std::string& msg) override {
        std::cout << "[LOG] " << msg << "\n";
    }
};

class FileLogger : public ILogger {
    void log(const std::string& msg) override {
        std::cout << "[FILE] " << msg << "\n";
    }
};

class MySQLDatabase : public IDatabase {
    std::shared_ptr<ILogger> logger_;
public:
    explicit MySQLDatabase(std::shared_ptr<ILogger> logger) : logger_(std::move(logger)) {
        logger_->log("MySQL Database created");
    }
    void save(const std::string& data) override {
        logger_->log("Saving: " + data);
        std::cout << "[DB] Saved to MySQL: " << data << "\n";
    }
};

// Simple DI Container
class DIContainer {
    std::unordered_map<std::type_index, std::function<std::any()>> factories_;
    std::unordered_map<std::type_index, std::any> singletons_;

public:
    template<typename T, typename... Deps>
    void registerSingleton() {
        factories_[std::type_index(typeid(T))] = [this]() -> std::any {
            return resolve<T, Deps...>();
        };
    }

    template<typename T>
    void registerInstance(std::shared_ptr<T> instance) {
        singletons_[std::type_index(typeid(T))] = instance;
    }

    template<typename T, typename Impl, typename... Deps>
    void registerType() {
        factories_[std::type_index(typeid(T))] = [this]() -> std::any {
            return resolve<Impl, Deps...>();
        };
    }

    template<typename T>
    std::shared_ptr<T> resolve() {
        auto idx = std::type_index(typeid(T));
        if (auto it = singletons_.find(idx); it != singletons_.end()) {
            return std::any_cast<std::shared_ptr<T>>(it->second);
        }
        if (auto it = factories_.find(idx); it != factories_.end()) {
            auto obj = it->second();
            auto ptr = std::any_cast<std::shared_ptr<T>>(obj);
            singletons_[idx] = ptr; // singleton by default
            return ptr;
        }
        throw std::runtime_error("No registration for type");
    }

private:
    template<typename T, typename Dep, typename... Rest>
    std::shared_ptr<T> resolve() {
        auto dep = resolve<Dep>();
        return resolve<T, Rest...>(dep);
    }

    template<typename T>
    std::shared_ptr<T> resolve() {
        return std::make_shared<T>();
    }

    template<typename T, typename... Args>
    std::shared_ptr<T> resolve(std::shared_ptr<Args>... args) {
        return std::make_shared<T>(std::move(args)...);
    }
};

class UserService {
    std::shared_ptr<IDatabase> db_;
    std::shared_ptr<ILogger> logger_;
public:
    UserService(std::shared_ptr<IDatabase> db, std::shared_ptr<ILogger> logger)
        : db_(std::move(db)), logger_(std::move(logger)) {}

    void createUser(const std::string& name) {
        logger_->log("Creating user: " + name);
        db_->save("User: " + name);
    }
};

int main() {
    DIContainer container;
    container.registerInstance<ILogger>(std::make_shared<ConsoleLogger>());
    container.registerType<IDatabase, MySQLDatabase, ILogger>();
    container.registerType<UserService, UserService, IDatabase, ILogger>();

    try {
        auto service = container.resolve<UserService>();
        service->createUser("Alice");
        service->createUser("Bob");
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }
    return 0;
}

/*
SAMPLE OUTPUT:
[LOG] MySQL Database created
[LOG] Creating user: Alice
[DB] Saved to MySQL: User: Alice
[LOG] Creating user: Bob
[DB] Saved to MySQL: User: Bob
*/
