/*
THEORY: Object Pool
- Reuses objects to avoid expensive construction/destruction
- Acquire/Release pattern with automatic reset
- Useful for database connections, thread pools, network connections
- Object reset ensures clean state on reuse
- Configurable initial size and growth policy

Complexity: O(1) acquire/release
Interview Questions:
1. When to use Object Pool vs regular allocation?
2. How to handle pool exhaustion? (block/throw/create new)
3. How to validate objects before reuse? (health check callback)
Common Mistakes:
- Not resetting object state properly
- Memory/resource leaks from not returning objects
- Thread safety issues with shared pool
*/

#include <iostream>
#include <vector>
#include <memory>
#include <queue>
#include <mutex>
#include <functional>
#include <stdexcept>

template<typename T>
class ObjectPool {
    std::queue<std::unique_ptr<T>> pool_;
    std::function<void(T&)> resetter_;
    size_t initialSize_;
    mutable std::mutex mutex_;

public:
    ObjectPool(size_t initialSize, std::function<void(T&)> resetter = nullptr)
        : resetter_(resetter), initialSize_(initialSize) {
        for (size_t i = 0; i < initialSize; ++i) {
            pool_.push(std::make_unique<T>());
        }
    }

    class PooledObject {
        std::unique_ptr<T, std::function<void(T*)>> ptr_;
    public:
        PooledObject(T* obj, std::function<void(T*)> deleter)
            : ptr_(obj, deleter) {}
        T* operator->() { return ptr_.get(); }
        T& operator*() { return *ptr_; }
    };

    PooledObject acquire() {
        std::lock_guard lock(mutex_);
        if (pool_.empty()) {
            return PooledObject(new T(), [this](T* obj) {
                if (resetter_) resetter_(*obj);
                std::lock_guard l(mutex_);
                pool_.push(std::unique_ptr<T>(obj));
            });
        }
        auto obj = std::move(pool_.front());
        pool_.pop();
        return PooledObject(obj.release(), [this](T* obj) {
            if (resetter_) resetter_(*obj);
            std::lock_guard l(mutex_);
            pool_.push(std::unique_ptr<T>(obj));
        });
    }

    size_t available() const {
        std::lock_guard lock(mutex_);
        return pool_.size();
    }
};

struct Connection {
    int id{0};
    bool connected{false};
    
    void connect() { connected = true; std::cout << "Connected #" << id << "\n"; }
    void disconnect() { connected = false; std::cout << "Disconnected #" << id << "\n"; }
    void query(const std::string& q) {
        if (!connected) throw std::runtime_error("Not connected");
        std::cout << "Query on #" << id << ": " << q << "\n";
    }
};

int main() {
    ObjectPool<Connection> pool(3, [](Connection& c) {
        c.disconnect();
        c.id = 0;
    });

    std::cout << "Pool with " << pool.available() << " connections\n";

    {
        auto conn1 = pool.acquire();
        conn1->id = 1;
        conn1->connect();
        conn1->query("SELECT * FROM users");
        
        auto conn2 = pool.acquire();
        conn2->id = 2;
        conn2->connect();
        conn2->query("INSERT INTO logs VALUES('test')");
        
        std::cout << "Available: " << pool.available() << "\n";
    }
    
    std::cout << "Back in pool: " << pool.available() << "\n";
    return 0;
}

/*
SAMPLE OUTPUT:
Pool with 3 connections
Connected #1
Query on #1: SELECT * FROM users
Connected #2
Query on #2: INSERT INTO logs VALUES('test')
Available: 1
Disconnected #2
Disconnected #1
Back in pool: 3
*/
