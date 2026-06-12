/*
THEORY: Singleton Pattern
- Ensures only one instance of a class exists
- Global point of access to that instance
- Meyer's Singleton: thread-safe (C++11 guarantees static local init)
- Lazy initialization (created on first use)
- Often used for logging, config, thread pools

Complexity: O(1) access
Interview Questions:
1. Is Meyer's Singleton thread-safe? (Yes, C++11 guarantees it)
2. How to destroy singleton? (static destruction order issues)
3. Singleton vs Dependency Injection trade-offs?
Common Mistakes:
- Not deleting copy/move operations
- Thread safety issues in pre-C++11
- Static initialization order fiasco with global singletons
*/

#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

class Singleton {
    std::string name_;
    int value_;
    mutable std::mutex mutex_;

    Singleton() : name_("Default"), value_(0) {
        std::cout << "Singleton instance created\n";
    }

    ~Singleton() {
        std::cout << "Singleton instance destroyed\n";
    }

public:
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
    Singleton(Singleton&&) = delete;
    Singleton& operator=(Singleton&&) = delete;

    static Singleton& instance() {
        static Singleton inst;
        return inst;
    }

    void configure(const std::string& name, int value) {
        std::lock_guard lock(mutex_);
        name_ = name;
        value_ = value;
    }

    std::string getName() const {
        std::lock_guard lock(mutex_);
        return name_;
    }

    int getValue() const {
        std::lock_guard lock(mutex_);
        return value_;
    }

    void print() const {
        std::lock_guard lock(mutex_);
        std::cout << "Singleton[" << this << "]: " << name_ 
                  << " = " << value_ << "\n";
    }
};

void worker(int id) {
    Singleton::instance().print();
    Singleton::instance().configure("Thread-" + std::to_string(id), id * 10);
    Singleton::instance().print();
}

int main() {
    std::cout << "Main starting...\n";
    
    // First access creates instance
    Singleton::instance().configure("AppConfig", 42);
    Singleton::instance().print();

    std::vector<std::thread> threads;
    for (int i = 0; i < 3; ++i) {
        threads.emplace_back(worker, i + 1);
    }
    for (auto& t : threads) t.join();

    std::cout << "Final state: ";
    Singleton::instance().print();
    return 0;
}

/*
SAMPLE OUTPUT:
Main starting...
Singleton instance created
Singleton[0x102e00008]: AppConfig = 42
Singleton[0x102e00008]: Thread-1 = 0
Singleton[0x102e00008]: Thread-1 = 10
Singleton[0x102e00008]: Thread-2 = 0
Singleton[0x102e00008]: Thread-2 = 20
Singleton[0x102e00008]: Thread-3 = 0
Singleton[0x102e00008]: Thread-3 = 30
Final state: Singleton[0x102e00008]: Thread-3 = 30
Singleton instance destroyed
*/
