/*
THEORY: Multilevel Inheritance

Multilevel inheritance: chain of inheritance (A -> B -> C).
- Each level inherits from the previous.
- Constructor order: most base first (A -> B -> C).
- Destructor order: most derived first (C -> B -> A).
- Each level can extend or override functionality.
- Base class initialization propagates up the chain
  (C must initialize B, B must initialize A).

Real-world analogy: Vehicle -> Car -> SportsCar.
*/

#include <iostream>
#include <string>

class Vehicle {
protected:
    std::string make_;
    std::string model_;
    int year_;

public:
    Vehicle(const std::string& make, const std::string& model, int year)
        : make_{make}, model_{model}, year_{year} {
        std::cout << "Vehicle ctor: " << make_ << " " << model_ << '\n';
    }

    virtual ~Vehicle() {
        std::cout << "Vehicle dtor\n";
    }

    virtual void start() const {
        std::cout << "Vehicle starting...\n";
    }

    virtual void display() const {
        std::cout << year_ << " " << make_ << " " << model_;
    }
};

class Car : public Vehicle {
protected:
    int doors_;

public:
    Car(const std::string& make, const std::string& model, int year, int doors)
        : Vehicle{make, model, year}
        , doors_{doors} {
        std::cout << "Car ctor: " << doors_ << " doors\n";
    }

    ~Car() override {
        std::cout << "Car dtor\n";
    }

    void start() const override {
        std::cout << "Car: Turn key, engine starts...\n";
    }

    void display() const override {
        Vehicle::display();
        std::cout << ", " << doors_ << " doors";
    }

    void honk() const {
        std::cout << "Beep beep!\n";
    }
};

class SportsCar : public Car {
    int topSpeed_;

public:
    SportsCar(const std::string& make, const std::string& model, int year,
              int doors, int topSpeed)
        : Car{make, model, year, doors}
        , topSpeed_{topSpeed} {
        std::cout << "SportsCar ctor: top speed " << topSpeed_ << " mph\n";
    }

    ~SportsCar() override {
        std::cout << "SportsCar dtor\n";
    }

    void start() const override {
        std::cout << "SportsCar: Push button, engine roars!\n";
    }

    void display() const override {
        Car::display();
        std::cout << ", top speed: " << topSpeed_ << " mph";
    }

    void activateTurbo() const {
        std::cout << "Turbo boost activated!\n";
    }
};

int main() {
    // 1. Multilevel construction
    std::cout << "=== Construction Order ===\n";
    SportsCar ferrari{"Ferrari", "SF90 Stradale", 2024, 2, 211};
    std::cout << '\n';

    // 2. Polymorphic behavior at each level
    std::cout << "=== Polymorphism ===\n";
    ferrari.display();
    std::cout << '\n';
    ferrari.start();
    ferrari.honk();
    ferrari.activateTurbo();

    // 3. Upcasting through chain
    std::cout << "\n=== Upcast Chain ===\n";
    Car* carPtr{&ferrari};
    carPtr->display();
    std::cout << '\n';
    carPtr->honk();

    Vehicle* vehPtr{&ferrari};
    vehPtr->display();          // displays all info (polymorphic)
    std::cout << '\n';

    // 4. Container of Vehicles
    std::cout << "\n=== Vehicle Collection ===\n";
    Vehicle* fleet[3];
    SportsCar sc{"Porsche", "911 Turbo", 2023, 2, 205};
    Car sedan{"Toyota", "Camry", 2022, 4};
    // SportsCar is-a Car is-a Vehicle

    fleet[0] = &ferrari;
    fleet[1] = &sc;
    fleet[2] = &sedan;

    for (auto* v : fleet) {
        v->start();
    }

    return 0;
}

/*
SAMPLE I/O:
=== Construction Order ===
Vehicle ctor: Ferrari SF90 Stradale
Car ctor: 2 doors
SportsCar ctor: top speed 211 mph

=== Polymorphism ===
2024 Ferrari SF90 Stradale, 2 doors, top speed: 211 mph
SportsCar: Push button, engine roars!
Beep beep!
Turbo boost activated!

=== Upcast Chain ===
2024 Ferrari SF90 Stradale, 2 doors, top speed: 211 mph
Beep beep!
2024 Ferrari SF90 Stradale, 2 doors, top speed: 211 mph

=== Vehicle Collection ===
SportsCar: Push button, engine roars!
SportsCar: Push button, engine roars!
Car: Turn key, engine starts...

(Desctructors run in reverse: SportsCar -> Car -> Vehicle for each)

COMPLEXITY:
- Constructor chain: O(D) where D = depth
- Virtual dispatch: O(1) per call

INTERVIEW QUESTIONS:
1. Difference between multilevel and multiple inheritance?
   Multilevel: linear chain (A -> B -> C). Single path of inheritance.
   Multiple: one class inherits from multiple bases directly (class C : A, B).

2. Can a derived class call base's base constructor directly?
   No. In C++ (unlike Java with super.super), a class can only directly
   initialize its direct base. C must call B's ctor, not A's.

3. How deep should inheritance hierarchies be?
   As shallow as possible. Deep hierarchies (more than 3-4 levels) are
   brittle — changes at the top affect everything below. Prefer composition.

COMMON MISTAKES:
- Skipping a level in initializer list (must initialize direct base)
- Forgetting to call base constructor for each level
- Duplicating code across levels (should be in the appropriate level)
- Making hierarchy too deep (hard to maintain, debug)
- Not using virtual destructors at every level
- Shadowing base members with derived members (name hiding)
*/
