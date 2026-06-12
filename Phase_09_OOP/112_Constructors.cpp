/*
THEORY: Constructors

- Default constructor: no parameters; called when no args given.
- Parameterized constructor: takes arguments to initialize members.
- Initializer list: preferred way to initialize members (before body).
- explicit: prevents implicit conversion via constructor.
- default (C++11): request compiler-generated default constructor.
- delete (C++11): delete a constructor/function.

If no constructor is defined, compiler generates a default constructor
(unless any user-defined constructor exists — then default is not generated).
*/

#include <iostream>
#include <string>

class Person {
    std::string name_;
    int age_;
    mutable int cache_{};

public:
    // 1. Default constructor
    Person() : name_{"Unknown"}, age_{0} {
        std::cout << "Default ctor\n";
    }

    // 2. Parameterized constructor with initializer list
    Person(const std::string& name, int age)
        : name_{name}, age_{age} {
        std::cout << "Parameterized ctor for " << name_ << '\n';
    }

    // 3. explicit constructor — prevents implicit conversion
    explicit Person(int age) : name_{"AgeOnly"}, age_{age} {
        std::cout << "Explicit ctor with age " << age << '\n';
    }

    // 4. Delegating constructor (C++11)
    Person(const std::string& name) : Person{name, 0} {
        std::cout << "Delegating ctor\n";
    }

    // 5. defaulted constructors
    Person() = default;  // ERROR: can't redeclare — would conflict.
    // Only used when no other user-defined ctor exists.

    // 6. Deleted constructor
    Person(double) = delete;  // prevents Person(3.14)

    void display() const {
        std::cout << name_ << " (" << age_ << ")\n";
    }

    int getAge() const { return age_; }
};

class OnlyMove {
public:
    OnlyMove() = default;
    OnlyMove(const OnlyMove&) = delete;
    OnlyMove& operator=(const OnlyMove&) = delete;
    OnlyMove(OnlyMove&&) = default;
    OnlyMove& operator=(OnlyMove&&) = default;
};

int main() {
    // 1. Default constructor
    std::cout << "--- Default ---\n";
    Person p1;
    p1.display();

    // 2. Parameterized
    std::cout << "\n--- Parameterized ---\n";
    Person p2{"Alice", 25};
    p2.display();

    // 3. explicit prevents this:
    // Person p3 = 30;   // ERROR: constructor is explicit
    Person p3{30};        // OK: direct initialization
    p3.display();

    // 4. Delegating
    std::cout << "\n--- Delegating ---\n";
    Person p4{"Bob"};
    p4.display();

    // 5. Deleted
    // Person p5{3.14};   // ERROR: deleted

    // 6. explicit in context
    std::cout << "\n--- explicit in context ---\n";
    auto process = [](const Person& p) {
        std::cout << "Processing: ";
        p.display();
    };
    process(Person{40});   // OK: explicit conversion
    // process(40);        // ERROR: cannot convert implicitly

    // 7. Initializer list vs assignment
    std::cout << "\n--- init list performance ---\n";
    // Members initialized in initializer list (direct init) vs assignment in body:
    // Strings: init list calls string(const char*), assignment calls default then operator=
    // References and const members MUST be in initializer list.

    return 0;
}

/*
SAMPLE I/O:
--- Default ---
Default ctor
Unknown (0)

--- Parameterized ---
Parameterized ctor for Alice
Alice (25)

--- Explicit ---
Explicit ctor with age 30
AgeOnly (30)

--- Delegating ---
Parameterized ctor for Bob
Delegating ctor
Bob (0)

--- explicit in context ---
Processing: Explicit ctor for 40
AgeOnly (40)

COMPLEXITY:
- All constructors: O(1) or O(N) for string copy

INTERVIEW QUESTIONS:
1. When must you use initializer list (vs assignment in body)?
   For const members, reference members, base class constructors,
   and members without default constructors. Also more efficient
   (avoids default construction + assignment).

2. What does explicit do?
   Prevents implicit conversion sequences from other types.
   Recommended for single-argument constructors to avoid surprising conversions.

3. What is a delegating constructor?
   A constructor that calls another constructor of the same class.
   Reduces code duplication (C++11).

COMMON MISTAKES:
- Forgetting to initialize pointer members (leading to UB)
- Using assignment in body instead of initializer list for const/ref members (error)
- Not marking single-arg constructors as explicit (allows implicit conversions)
- Calling virtual functions from constructors (they don't dispatch to derived)
- Leaving members uninitialized (POD types have indeterminate values)
*/
