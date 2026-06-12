/*
THEORY: Copy Assignment

Copy Assignment Operator: MyClass& operator=(const MyClass& other);
- Called when an already-initialized object is assigned from another object.
- Must handle: self-assignment, resource release, deep copy.

Copy-and-Swap Idiom:
1. Create a temporary copy of 'other' (using copy constructor).
2. Swap the temporary's resources with *this.
3. Temporary's destructor releases old resources.
Advantages: strong exception guarantee, no self-assignment check needed,
automatic reuse of copy constructor logic.

Self-assignment guard: if (this != &other) — needed for manual implementations.
Without it, you may delete your own resources before copying them.
*/

#include <iostream>
#include <cstring>
#include <algorithm>

class String {
    char* data_;
    size_t size_;

public:
    // Constructor
    explicit String(const char* str = "")
        : size_{std::strlen(str)}, data_{new char[size_ + 1]} {
        std::strcpy(data_, str);
        std::cout << "Ctor: \"" << str << "\"\n";
    }

    // Copy constructor
    String(const String& other)
        : size_{other.size_}, data_{new char[size_ + 1]} {
        std::strcpy(data_, other.data_);
        std::cout << "Copy ctor\n";
    }

    // Destructor
    ~String() {
        std::cout << "Dtor";
        if (data_) std::cout << " (\"" << data_ << "\")";
        std::cout << '\n';
        delete[] data_;
    }

    // --- Copy-and-Swap Idiom ---
    friend void swap(String& a, String& b) noexcept {
        using std::swap;
        swap(a.data_, b.data_);
        swap(a.size_, b.size_);
    }

    // Copy assignment using copy-and-swap
    String& operator=(const String& other) {
        std::cout << "Copy assign (copy-and-swap)\n";
        String temp{other};     // 1. copy
        swap(*this, temp);      // 2. swap (noexcept)
        return *this;           // 3. temp released
    }

    const char* c_str() const { return data_; }
};

// --- Traditional self-assignment guard (without copy-and-swap) ---
class TraditionalString {
    char* data_;
    size_t size_;

public:
    TraditionalString(const char* str) : size_{std::strlen(str)}, data_{new char[size_ + 1]} {
        std::strcpy(data_, str);
    }

    ~TraditionalString() { delete[] data_; }

    TraditionalString& operator=(const TraditionalString& other) {
        std::cout << "Traditional copy assign\n";
        if (this != &other) {
            delete[] data_;                          // release old
            size_ = other.size_;
            data_ = new char[size_ + 1];
            std::strcpy(data_, other.data_);         // copy
        }
        return *this;
    }

    const char* c_str() const { return data_; }
};

int main() {
    // 1. Copy assignment — copy-and-swap
    std::cout << "=== Copy-and-Swap ===\n";
    String s1{"Hello"};
    String s2{"World"};
    s2 = s1;                           // copy assignment
    std::cout << "s1: " << s1.c_str() << ", s2: " << s2.c_str() << '\n';

    // 2. Self-assignment (safe with copy-and-swap)
    std::cout << "\n=== Self-assignment ===\n";
    String s3{"Self"};
    s3 = s3;                           // no-op (safe)
    std::cout << "s3 after self-assign: " << s3.c_str() << '\n';

    // 3. Chain assignment
    std::cout << "\n=== Chain assignment ===\n";
    String a{"A"}, b{"B"}, c{"C"};
    a = b = c;                         // b = c (copy), then a = b (copy)
    std::cout << "a: " << a.c_str() << ", b: " << b.c_str()
              << ", c: " << c.c_str() << '\n';

    // 4. Traditional with self-assignment guard
    std::cout << "\n=== Traditional assignment ===\n";
    TraditionalString t1{"Traditional"};
    TraditionalString t2{"Style"};
    t2 = t1;
    std::cout << "t1: " << t1.c_str() << ", t2: " << t2.c_str() << '\n';

    // 5. Exception safety demo
    std::cout << "\n=== Exception safety ===\n";
    struct Bomb {
        bool shouldThrow;
        Bomb() : shouldThrow{false} {}
        Bomb(const Bomb& other) : shouldThrow{other.shouldThrow} {
            if (shouldThrow) throw std::runtime_error("Boom!");
        }
    };

    struct Container {
        Bomb b;
        String s;
        Container& operator=(const Container& other) {
            // copy-and-swap provides strong guarantee
            Container temp{other};
            swap(*this, temp);
            return *this;
        }
        friend void swap(Container& a, Container& b) noexcept {
            using std::swap;
            swap(a.b, b.b);
            swap(a.s, b.s);
        }
    };

    return 0;
}

/*
SAMPLE I/O:
=== Copy-and-Swap ===
Ctor: "Hello"
Ctor: "World"
Copy assign (copy-and-swap)
Copy ctor
Dtor ("Hello")
s1: Hello, s2: Hello
Dtor ("Hello")
Dtor ("Hello")

=== Self-assignment ===
Ctor: "Self"
Copy assign (copy-and-swap)
Copy ctor
Dtor ("Self")
s3 after self-assign: Self
Dtor ("Self")

=== Chain assignment ===
Ctor: "A"
Ctor: "B"
Ctor: "C"
Copy assign (copy-and-swap)
Copy ctor
Dtor ("B")
Copy assign (copy-and-swap)
Copy ctor
Dtor ("A")
a: C, b: C, c: C
Dtor ("C")
Dtor ("C")
Dtor ("C")

COMPLEXITY:
- Copy assignment: O(N) for deep copy + O(1) swap
- Self-assignment check: O(1)

INTERVIEW QUESTIONS:
1. What is the copy-and-swap idiom?
   Implement assignment by creating a copy and then swapping. Provides
   strong exception guarantee, handles self-assignment automatically,
   and reuses copy constructor logic.

2. Why is self-assignment important?
   Without the guard (this != &other), the implementation might release
   resources that are needed for copying. Copy-and-swap inherently handles it.

3. What exception guarantees does copy-and-swap provide?
   Strong guarantee: either assignment succeeds fully, or the object is
   unchanged. No partial modification (unlike manual implementations).

COMMON MISTAKES:
- Forgetting self-assignment check in manual copy assignment
- Not returning *this by reference (return type should be T&)
- Not using const reference parameter
- Throwing after deleting old resources (bad_alloc leaves object in invalid state)
- Writing swap in terms of copy assignment (infinite recursion)
*/
