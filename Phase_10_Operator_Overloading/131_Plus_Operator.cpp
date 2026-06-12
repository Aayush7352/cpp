/*
==================================================================
131. Plus Operator Overload (+)
==================================================================
Theory:
- Binary operator overloading using member/non-member functions
- Commutative property: a + b == b + a
- For commutative, implement as non-member friend so both operands
  get implicit conversion
- Return by value (new object), not reference
*/

#include <iostream>
#include <cassert>

class Vector2D {
private:
    double m_x, m_y;

public:
    Vector2D(double x = 0.0, double y = 0.0) noexcept
        : m_x(x), m_y(y) {}

    double getX() const noexcept { return m_x; }
    double getY() const noexcept { return m_y; }

    // Member operator+ (used when Vector2D is lhs)
    Vector2D operator+(const Vector2D& rhs) const noexcept {
        return Vector2D(m_x + rhs.m_x, m_y + rhs.m_y);
    }

    // Compound assignment (needed for consistent interface)
    Vector2D& operator+=(const Vector2D& rhs) noexcept {
        m_x += rhs.m_x;
        m_y += rhs.m_y;
        return *this;
    }

    void display() const {
        std::cout << "(" << m_x << ", " << m_y << ")\n";
    }
};

// Non-member operator+ for commutativity with different types
// e.g., 5.0 + vec
Vector2D operator+(double scalar, const Vector2D& v) noexcept {
    return Vector2D(scalar + v.getX(), scalar + v.getY());
}

Vector2D operator+(const Vector2D& v, double scalar) noexcept {
    return Vector2D(v.getX() + scalar, v.getY() + scalar);
}

int main() {
    std::cout << "=== Plus Operator Overload (+)\n\n";

    Vector2D a(3.0, 4.0);
    Vector2D b(1.5, 2.5);

    std::cout << "a = "; a.display();
    std::cout << "b = "; b.display();

    Vector2D c = a + b;
    std::cout << "a + b = "; c.display();

    // Commutative test: same result
    Vector2D d = b + a;
    std::cout << "b + a = "; d.display();
    assert(c.getX() == d.getX() && c.getY() == d.getY());
    std::cout << "Commutative property holds.\n\n";

    // Scalar + vector and vector + scalar
    std::cout << "a + 5.0 = " << (a + 5.0);
    std::cout << "\n7.5 + a = " << (7.5 + a);
    std::cout << "\n\nCompound += :\n";
    a += b;
    std::cout << "a += b => "; a.display();

    return 0;
}

/*
==================================================================
Sample I/O:
a = (3, 4)
b = (1.5, 2.5)
a + b = (4.5, 6.5)
b + a = (4.5, 6.5)
Commutative property holds.

a + 5.0 = (8, 9)
7.5 + a = (10.5, 11.5)

Compound += :
a += b => (4.5, 6.5)
==================================================================
Complexity Analysis:
- Time: O(1) per operation
- Space: O(1) — no dynamic allocation
==================================================================
Interview Questions:
1. Why can't operator+ be a member function for commutative types?
   -> If lhs is not the class type, member function won't work;
      non-member allows implicit conversion on both sides.

2. Should operator+ return by value or reference?
   -> By value. A new object is created; returning reference
      would dangle.

3. Relationship between + and += ?
   -> Typically implement += first (modifies this), then +
      as return Vector2D(*this) += rhs; (copy + add).
==================================================================
Common Mistakes:
- Returning reference to local object from operator+
- Forgetting const qualification for read-only operations
- Not providing non-member overloads for scalar types
- Ignoring the commutative property requirement
==================================================================
*/
