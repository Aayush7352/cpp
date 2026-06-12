/*
==================================================================
132. Minus Operator Overload (-)
==================================================================
Theory:
- Binary operator- (subtraction) and unary operator- (negation)
- Unary negation: returns a new object with components negated
- Non-member friends needed when lhs is not the class itself
- Binary - can be implemented using compound -=
*/

#include <iostream>

class Vector2D {
private:
    double m_x, m_y;

public:
    Vector2D(double x = 0.0, double y = 0.0) noexcept
        : m_x(x), m_y(y) {}

    double getX() const noexcept { return m_x; }
    double getY() const noexcept { return m_y; }

    // Unary negation
    Vector2D operator-() const noexcept {
        return Vector2D(-m_x, -m_y);
    }

    // Binary subtraction
    Vector2D operator-(const Vector2D& rhs) const noexcept {
        return Vector2D(m_x - rhs.m_x, m_y - rhs.m_y);
    }

    Vector2D& operator-=(const Vector2D& rhs) noexcept {
        m_x -= rhs.m_x;
        m_y -= rhs.m_y;
        return *this;
    }

    void display() const {
        std::cout << "(" << m_x << ", " << m_y << ")\n";
    }
};

// Scalar subtraction
Vector2D operator-(const Vector2D& v, double s) noexcept {
    return Vector2D(v.getX() - s, v.getY() - s);
}
Vector2D operator-(double s, const Vector2D& v) noexcept {
    return Vector2D(s - v.getX(), s - v.getY());
}

int main() {
    std::cout << "=== Minus Operator Overload (-)\n\n";

    Vector2D a(10.0, 6.0);
    Vector2D b(3.0, 2.0);

    std::cout << "a = "; a.display();
    std::cout << "b = "; b.display();

    Vector2D c = a - b;
    std::cout << "a - b = "; c.display();

    // Unary negation
    Vector2D neg = -a;
    std::cout << "-a = "; neg.display();

    // Chaining
    Vector2D d = -a - b;
    std::cout << "-a - b = "; d.display();

    std::cout << "\na -= b => ";
    a -= b;
    a.display();

    return 0;
}

/*
==================================================================
Sample I/O:
a = (10, 6)
b = (3, 2)
a - b = (7, 4)
-a = (-10, -6)
-a - b = (-13, -8)

a -= b => (7, 4)
==================================================================
Complexity Analysis:
- Time: O(1) per operation
- Space: O(1)
==================================================================
Interview Questions:
1. Distinguish between unary and binary operator-.
   -> Unary (one operand) returns negated copy; binary (two)
      returns difference. Overloaded by parameter count.

2. Can operator- be a static function?
   -> Yes, if declared as a non-member friend. Static member
      cannot access *this.

3. Why is unary - typically const?
   -> It does not modify the original object; returns a new one.
==================================================================
Common Mistakes:
- Modifying *this in unary operator- (should return new object)
- Forgetting const correctness
- Not implementing - in terms of -= leading to code duplication
- Not handling mixed-type subtraction symmetrically
==================================================================
*/
