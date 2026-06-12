/*
THEORY: Single Inheritance

Single inheritance: one base class, one derived class.
- Constructor call order: Base -> Members -> Derived.
- Destructor call order: Derived -> Members -> Base (reverse of construction).
- Base class initialization: must happen in derived's initializer list.
- If base has no default constructor, derived MUST explicitly call base constructor.

Memory layout: Base sub-object at start, then derived members.
*/

#include <iostream>
#include <string>

class Person {
protected:
    std::string name_;
    int age_;

public:
    Person(const std::string& name, int age)
        : name_{name}, age_{age} {
        std::cout << "Person ctor: " << name_ << ", " << age_ << '\n';
    }

    virtual ~Person() {
        std::cout << "Person dtor: " << name_ << '\n';
    }

    virtual void display() const {
        std::cout << "Person: " << name_ << " (" << age_ << ")\n";
    }

    std::string getName() const { return name_; }
    int getAge() const { return age_; }
};

class Employee : public Person {
    int employeeId_;
    double salary_;

public:
    // Must call Person constructor explicitly
    Employee(const std::string& name, int age, int empId, double salary)
        : Person{name, age}                    // base class init
        , employeeId_{empId}
        , salary_{salary} {
        std::cout << "Employee ctor: " << name_ << ", ID=" << empId << '\n';
    }

    ~Employee() override {
        std::cout << "Employee dtor: " << name_ << '\n';
    }

    void display() const override {
        std::cout << "Employee: " << name_ << " | ID: " << employeeId_
                  << " | $" << salary_ << '\n';
    }

    double getSalary() const { return salary_; }
};

class Manager : public Employee {
    int teamSize_;

public:
    Manager(const std::string& name, int age, int empId, double salary, int teamSize)
        : Employee{name, age, empId, salary}
        , teamSize_{teamSize} {
        std::cout << "Manager ctor: " << name_ << ", team=" << teamSize << '\n';
    }

    ~Manager() override {
        std::cout << "Manager dtor: " << name_ << '\n';
    }

    void display() const override {
        std::cout << "Manager: " << name_ << " | ID: " << employeeId_
                  << " | $" << salary_ << " | Team: " << teamSize_ << '\n';
    }
};

int main() {
    // 1. Constructor/Destructor order
    std::cout << "=== Construction Order ===\n";
    Manager mgr{"Alice", 35, 1001, 95000, 8};
    std::cout << '\n';

    // 2. Display (polymorphic)
    std::cout << "=== Polymorphic display ===\n";
    Person* people[3];
    Person p{"Bob", 50};
    Employee e{"Charlie", 28, 2001, 65000};
    people[0] = &p;
    people[1] = &e;
    people[2] = &mgr;

    for (auto* person : people) {
        person->display();
    }
    std::cout << '\n';

    // 3. Accessing base members
    std::cout << "=== Base members ===\n";
    std::cout << "Manager's name (via Person): " << mgr.getName() << '\n';
    std::cout << "Manager's salary (via Employee): " << mgr.getSalary() << '\n';

    // 4. Destructor order (scope exit)
    std::cout << "\n=== Destruction Order ===\n";
    // mgr, e, p destroyed in reverse order of declaration

    return 0;
}

/*
SAMPLE I/O:
=== Construction Order ===
Person ctor: Alice, 35
Employee ctor: Alice, ID=1001
Manager ctor: Alice, team=8

=== Polymorphic display ===
Person: Bob (50)
Employee: Charlie | ID: 2001 | $65000
Manager: Alice | ID: 1001 | $95000 | Team: 8

=== Base members ===
Manager's name (via Person): Alice
Manager's salary (via Employee): 95000

=== Destruction Order ===
Manager dtor: Alice
Employee dtor: Alice
Person dtor: Alice
Person dtor: Charlie
Employee dtor: Charlie
Person dtor: Bob

COMPLEXITY:
- Construction: O(D) where D = depth of hierarchy
- Virtual dispatch: O(1) via vtable

INTERVIEW QUESTIONS:
1. Constructor order in single inheritance?
   Base class constructor -> Derived member initializers -> Derived constructor body.
   Destructor runs in exact reverse order.

2. Can a derived class call a base class virtual function from its constructor?
   Yes, but it will dispatch to the base version (not derived), because the
   derived part hasn't been constructed yet.

3. What happens if base has no default constructor?
   Derived class must explicitly call parameterized base constructor in its
   initializer list. Otherwise, compilation error.

COMMON MISTAKES:
- Forgetting to call base constructor in initializer list
- Expecting virtual dispatch in constructors/destructors
- Not marking base destructor virtual (UB when deleting via base pointer)
- Object slicing: passing derived by value to function expecting base
- Accessing derived-specific members from base pointer without casting
*/
