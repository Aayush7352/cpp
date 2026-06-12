/*
THEORY: Class and Object

Class: User-defined type that encapsulates data and functions.
Object: Instance of a class.

Access Specifiers:
- public: accessible from anywhere.
- protected: accessible from class and derived classes.
- private: accessible only within the class (default).

Member Functions: Functions defined inside class; operate on class data.
this pointer: Implicit pointer to the current object (available in non-static members).
*/

#include <iostream>
#include <string>

class Student {
    // Private by default
    std::string name_;
    int roll_;
    double marks_;

public:
    // Constructor
    Student(const std::string& name, int roll, double marks)
        : name_{name}, roll_{roll}, marks_{marks} {}

    // Member functions
    void display() const {
        // 'this' is a const pointer to the current object
        std::cout << "Roll: " << this->roll_
                  << " | Name: " << this->name_
                  << " | Marks: " << this->marks_ << '\n';
    }

    void updateMarks(double extra) {
        this->marks_ += extra;  // explicit use of this
    }

    // Getter (accessor)
    double getMarks() const { return marks_; }

    // Setter (mutator)
    void setName(const std::string& name) { name_ = name; }

    // Static member function (no this pointer)
    static std::string schoolName() { return "Greenwood High"; }

    // Const member function: promises not to modify the object
    bool isPassing(double passMark = 40.0) const {
        return marks_ >= passMark;
    }
};

int main() {
    // Creating objects
    Student s1{"Alice", 101, 85.5};
    Student s2{"Bob", 102, 38.0};

    // Accessing member functions
    s1.display();
    s2.display();

    std::cout << "\nAfter extra credit:\n";
    s1.updateMarks(5.0);
    s1.display();

    // Static member
    std::cout << "\nSchool: " << Student::schoolName() << '\n';

    // Const member
    std::cout << "Alice passing: " << (s1.isPassing() ? "Yes" : "No") << '\n';
    std::cout << "Bob passing: " << (s2.isPassing() ? "Yes" : "No") << '\n';

    // Pointer to object
    Student* ptr{&s1};
    ptr->display();

    // Reference to object
    Student& ref{s2};
    ref.setName("Robert");
    ref.display();

    // this pointer comparison
    std::cout << "\nAddress of s1: " << &s1 << ", this inside display: see output\n";

    return 0;
}

/*
SAMPLE I/O:
Roll: 101 | Name: Alice | Marks: 85.5
Roll: 102 | Name: Bob | Marks: 38

After extra credit:
Roll: 101 | Name: Alice | Marks: 90.5

School: Greenwood High
Alice passing: Yes
Bob passing: No
Roll: 101 | Name: Alice | Marks: 90.5
Roll: 102 | Name: Robert | Marks: 38

Address of s1: 0x16f82b510, this inside display: see output

COMPLEXITY:
- Constructor: O(1) or O(N) for string copy
- display / getters: O(1)

INTERVIEW QUESTIONS:
1. What is the 'this' pointer and when is it needed?
   Implicit pointer to the current object. Needed to resolve name conflicts
   (parameter vs member) or to return *this from chained functions.

2. What is the difference between a struct and a class in C++?
   Default access: struct is public, class is private. Both are otherwise identical.

3. Can 'this' be used in a static member function?
   No. Static member functions have no this pointer (they belong to the class type,
   not an instance).

COMMON MISTAKES:
- Forgetting semicolon after class definition
- Accessing private members from outside the class
- Modifying member variables in a const member function (compiler error)
- Using this-> unnecessarily (mostly style, but sometimes required)
- Confusing class declaration with object creation: Student s; vs Student s();
*/
