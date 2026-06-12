/*
THEORY: Abstract Classes — Interface Design, Contracts, Usage Patterns

Abstract Class patterns:
1. Interface class: all pure virtual, no data members.
2. Abstract base with partial implementation: some defaults, some pure virtuals.
3. Template method pattern: base defines skeleton, derived fills in details.

Interface contracts: ABC defines the "what" (interface), derived implements the "how".

ABC usage patterns:
- Strategy: different algorithms through same interface.
- Observer: notification interface.
- Factory: create objects through abstract factory interface.
- Polymorphic value types (type erasure via abstract base).

NVI (Non-Virtual Interface) pattern: public non-virtual calls private virtual.
*/

#include <iostream>
#include <memory>
#include <string>
#include <vector>

// --- 1. Interface class (all pure virtual, no data) ---
class ISerializable {
public:
    virtual ~ISerializable() = default;
    virtual std::string serialize() const = 0;
    virtual bool deserialize(const std::string& data) = 0;
};

// --- 2. Abstract base with partial implementation ---
class Logger {
public:
    virtual ~Logger() = default;

    // Template method pattern: public non-virtual
    void log(const std::string& message) {
        std::string formatted = format(message);
        write(formatted);
    }

protected:
    // Pure virtual — derived must implement
    virtual void write(const std::string& formatted) = 0;

private:
    // Common functionality — shared across all loggers
    std::string format(const std::string& msg) {
        auto now = std::time(nullptr);
        std::string timestamp = std::ctime(&now);
        timestamp.pop_back();  // remove newline
        return "[" + timestamp + "] " + msg;
    }
};

class ConsoleLogger : public Logger {
protected:
    void write(const std::string& formatted) override {
        std::cout << formatted << '\n';
    }
};

class FileLogger : public Logger {
    std::string path_;
public:
    explicit FileLogger(const std::string& path) : path_{path} {}
protected:
    void write(const std::string& formatted) override {
        std::cout << "[File " << path_ << "] " << formatted << '\n';
    }
};

// --- 3. Factory method pattern ---
class IVehicle {
public:
    virtual ~IVehicle() = default;
    virtual void drive() const = 0;
    virtual int wheels() const = 0;
};

class Car : public IVehicle {
public:
    void drive() const override { std::cout << "Driving car (4 wheels)\n"; }
    int wheels() const override { return 4; }
};

class Motorcycle : public IVehicle {
public:
    void drive() const override { std::cout << "Riding motorcycle (2 wheels)\n"; }
    int wheels() const override { return 2; }
};

class VehicleFactory {
public:
    enum Type { CarType, MotorcycleType };
    static std::unique_ptr<IVehicle> create(Type t) {
        switch (t) {
            case CarType: return std::make_unique<Car>();
            case MotorcycleType: return std::make_unique<Motorcycle>();
        }
        return nullptr;
    }
};

// --- 4. Contract enforcement via ABC ---
class IContract {
public:
    virtual ~IContract() = default;
    virtual void execute() = 0;

    // Contract: call execute before calling verify
    void run() {
        execute();
        if (!verified_) {
            std::cout << "Warning: derived did not call verify()\n";
        }
    }

protected:
    bool verified_{false};
    void verify() { verified_ = true; }
};

class GoodImplementation : public IContract {
    void execute() override {
        std::cout << "Good: execute called\n";
        verify();  // follows contract
    }
};

class BadImplementation : public IContract {
    void execute() override {
        std::cout << "Bad: execute called without verify\n";
        // forgot to call verify()
    }
};

int main() {
    // 1. Interface
    std::cout << "=== ISerializable Interface ===\n";
    struct Document : ISerializable {
        std::string content;
        std::string serialize() const override { return content; }
        bool deserialize(const std::string& data) override {
            content = data;
            return true;
        }
    };
    Document doc{"Hello, World!"};
    std::cout << "Serialized: " << doc.serialize() << '\n';

    // 2. Template method / partial impl
    std::cout << "\n=== Logger (Template Method) ===\n";
    ConsoleLogger clog;
    FileLogger flog{"/var/log/app.log"};
    clog.log("Console message");
    flog.log("File message");

    // 3. Factory pattern
    std::cout << "\n=== Factory Pattern ===\n";
    auto vehicle = VehicleFactory::create(VehicleFactory::CarType);
    vehicle->drive();
    vehicle = VehicleFactory::create(VehicleFactory::MotorcycleType);
    vehicle->drive();

    // 4. Contract enforcement
    std::cout << "\n=== Contract Enforcement ===\n";
    GoodImplementation good;
    good.run();
    BadImplementation bad;
    bad.run();

    return 0;
}

/*
SAMPLE I/O:
=== ISerializable Interface ===
Serialized: Hello, World!

=== Logger (Template Method) ===
[Mon Jun  8 14:30:45 2026] Console message
[File /var/log/app.log] [Mon Jun  8 14:30:45 2026] File message

=== Factory Pattern ===
Driving car (4 wheels)
Riding motorcycle (2 wheels)

=== Contract Enforcement ===
Good: execute called
Bad: execute called without verify
Warning: derived did not call verify()

COMPLEXITY:
- Virtual dispatch: O(1) per call
- Factory creation: O(1)

INTERVIEW QUESTIONS:
1. Interface class vs abstract base class — difference?
   Interface: all methods pure virtual, no data members (like Java interface).
   Abstract base: some pure virtual, some implementation, may have data.
   C++ has no language distinction — it's a design choice.

2. What is the Non-Virtual Interface (NVI) pattern?
   Public non-virtual methods that call private/protected virtual methods.
   Base controls the "how" (pre/post conditions, logging, etc.) while derived
   provides the "what" (specialized behavior).

3. Can an abstract class have a constructor?
   Yes. It's called during derived class construction. Useful for initializing
   data members common to all derived classes.

COMMON MISTAKES:
- Making everything pure virtual when some default behavior is shared
- Not using override specifier (accidental hiding)
- Forgetting virtual destructor (UB)
- Making base class too concrete (derived classes can't customize)
- Making base class too abstract (every derived duplicates common code)
- Violating Liskov Substitution Principle (derived doesn't fulfill base contract)
- Deep abstract hierarchies (hard to navigate, maintain)
*/
