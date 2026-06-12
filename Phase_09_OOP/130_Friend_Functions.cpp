/*
THEORY: Friend Functions and Classes

Friend: a function or class that has access to private/protected members.

Friend function: non-member function that can access private members.
  friend returnType functionName(params);

Friend class: all member functions of friend class can access private members.
  friend class ClassName;

Forward declaration: declare class/function before defining it.

friend vs member:
- Friend: non-member, not inherited, not transitive, not symmetric.
- Member: part of class, inherited, has this pointer.

Common uses:
- Operator overloading (<<, >>, + between different types).
- Iterator access to container internals.
- Testing / white-box access.

Caution: friends break encapsulation. Use sparingly.
*/

#include <iostream>
#include <string>

// Forward declaration
class Employee;
class Department;

// --- Friend function (non-member) ---
void displaySalary(const Employee& emp);

class Employee {
    std::string name_;
    double salary_;
    std::string ssn_;  // sensitive data

public:
    Employee(const std::string& name, double salary, const std::string& ssn)
        : name_{name}, salary_{salary}, ssn_{ssn} {}

    // Friend function declaration
    friend void displaySalary(const Employee& emp);

    // Friend class declaration
    friend class Department;

    // Friend function defined outside class (different from member)
    friend std::ostream& operator<<(std::ostream& os, const Employee& emp) {
        os << emp.name_ << " (ID: " << &emp << ")";
        return os;
    }

    // Member function: has this pointer
    std::string getPublicName() const { return name_; }
};

// Friend function definition
void displaySalary(const Employee& emp) {
    // Can access private members
    std::cout << emp.name_ << "'s salary: $" << emp.salary_ << '\n';
    // std::cout << "SSN: " << emp.ssn_ << '\n';  // would work too
}

// --- Friend class ---
class Department {
    std::string name_;
public:
    explicit Department(const std::string& name) : name_{name} {}

    void promote(Employee& emp, double raise) {
        // Can access Employee's private members
        emp.salary_ += raise;
        std::cout << emp.name_ << " promoted in " << name_
                  << ". New salary: $" << emp.salary_ << '\n';
    }

    void showSSN(const Employee& emp) {
        std::cout << "SSN accessed by " << name_ << ": " << emp.ssn_ << '\n';
    }
};

// --- Friend across namespaces / forward declaration ---
class Secret {
    int data_;
public:
    Secret(int d) : data_{d} {}
    friend void reveal(const Secret& s);
};

void reveal(const Secret& s) {
    std::cout << "Secret data: " << s.data_ << '\n';
}

// --- Friend member function (specific function, not whole class) ---
class Controller;  // forward declaration

class Machine {
    int state_;

public:
    Machine() : state_{0} {}

    // Only authorize() can access private members
    friend void Controller::authorize(Machine& m, int newState);
};

class Controller {
public:
    void authorize(Machine& m, int newState) {
        m.state_ = newState;     // access private member of Machine
        std::cout << "Machine state set to " << m.state_ << '\n';
    }
};

// --- Friend vs Member comparison ---
class Point {
    double x_, y_;
public:
    Point(double x, double y) : x_{x}, y_{y} {}

    // Member addition — p + q (p is this)
    Point operator+(const Point& other) const {
        return Point{x_ + other.x_, y_ + other.y_};
    }

    // Friend for different types — 3.0 + p
    friend Point operator+(double scalar, const Point& p) {
        return Point{scalar + p.x_, scalar + p.y_};
    }

    void display() const {
        std::cout << "(" << x_ << ", " << y_ << ")\n";
    }
};

int main() {
    // 1. Friend function
    std::cout << "=== Friend Function ===\n";
    Employee emp{"Alice", 85000, "123-45-6789"};
    displaySalary(emp);

    // 2. Friend class
    std::cout << "\n=== Friend Class ===\n";
    Department dept{"Engineering"};
    dept.promote(emp, 10000);
    dept.showSSN(emp);

    // 3. Friend with forward declaration
    std::cout << "\n=== Forward Declaration Friend ===\n";
    Secret s{42};
    reveal(s);

    // 4. Friend member function
    std::cout << "\n=== Friend Member Function ===\n";
    Machine m;
    Controller ctrl;
    ctrl.authorize(m, 5);

    // 5. Friend for operator overloading
    std::cout << "\n=== Friend vs Member ===\n";
    // cout << emp;  // friend operator<<
    std::cout << "Employee: " << emp << '\n';

    Point p1{1.0, 2.0};
    Point p2{3.0, 4.0};
    Point p3{p1 + p2};        // member operator+
    p3.display();

    Point p4 = 3.0 + p1;      // friend operator+ (asymmetric)
    p4.display();

    return 0;
}

/*
SAMPLE I/O:
=== Friend Function ===
Alice's salary: $85000

=== Friend Class ===
Alice promoted in Engineering. New salary: $95000
SSN accessed by Engineering: 123-45-6789

=== Forward Declaration Friend ===
Secret data: 42

=== Friend Member Function ===
Machine state set to 5

=== Friend vs Member ===
Employee: Alice (ID: 0x16f82b510)
(4, 6)
(4, 5)

COMPLEXITY:
- Friend access: O(1) — same as member access

INTERVIEW QUESTIONS:
1. Friend function vs member function?
   Friend: no this pointer, can be placed in any namespace, used for
   symmetric/asymmetric operators (like scalar + Point).
   Member: has this pointer, called on an object (p1 + p2).

2. Are friends inherited?
   No. Friend status is not transitive and not inherited.
   If Base has friend, Derived does NOT automatically have that friend access.

3. When would you use a friend function over a member?
   - Operator overloading where left operand isn't your class (cout <<, scalar +).
   - When you need access but the function shouldn't be a member (logically).
   - When the function needs access to multiple classes' private data.

COMMON MISTAKES:
- Using friends when public interface would suffice (breaks encapsulation)
- Expecting friend status to be inherited or transitive
- Forward declaring without defining before use (linker error)
- Making entire class friend when only one function needs access
- Using friend to access private data of a class that provides proper interface
- Confusing friend function definition inside class (it's still non-member)
- Not understanding that friendship is granted, not taken
*/
