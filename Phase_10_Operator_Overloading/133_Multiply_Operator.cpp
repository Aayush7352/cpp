/*
==================================================================
133. Multiply Operator Overload (*)
==================================================================
Theory:
- Scalar multiplication: vec * scalar or scalar * vec
- Matrix multiplication: 2D dot product variant
- Different semantics based on operand types
- Return by value for new objects
*/

#include <iostream>
#include <vector>
#include <cassert>

// --- 2D Vector with Scalar Multiplication ---
class Vector2D {
private:
    double m_x, m_y;

public:
    Vector2D(double x = 0.0, double y = 0.0) noexcept
        : m_x(x), m_y(y) {}

    double getX() const noexcept { return m_x; }
    double getY() const noexcept { return m_y; }

    Vector2D operator*(double scalar) const noexcept {
        return Vector2D(m_x * scalar, m_y * scalar);
    }

    Vector2D& operator*=(double scalar) noexcept {
        m_x *= scalar;
        m_y *= scalar;
        return *this;
    }

    void display() const {
        std::cout << "(" << m_x << ", " << m_y << ")\n";
    }
};

Vector2D operator*(double scalar, const Vector2D& v) noexcept {
    return Vector2D(scalar * v.getX(), scalar * v.getY());
}

// --- Simple 2x2 Matrix ---
class Matrix2x2 {
private:
    double m[2][2];

public:
    Matrix2x2(double a11 = 0, double a12 = 0,
              double a21 = 0, double a22 = 0) noexcept {
        m[0][0] = a11; m[0][1] = a12;
        m[1][0] = a21; m[1][1] = a22;
    }

    // Matrix-matrix multiplication
    Matrix2x2 operator*(const Matrix2x2& rhs) const noexcept {
        return Matrix2x2(
            m[0][0] * rhs.m[0][0] + m[0][1] * rhs.m[1][0],
            m[0][0] * rhs.m[0][1] + m[0][1] * rhs.m[1][1],
            m[1][0] * rhs.m[0][0] + m[1][1] * rhs.m[1][0],
            m[1][0] * rhs.m[0][1] + m[1][1] * rhs.m[1][1]
        );
    }

    // Matrix-vector multiplication
    Vector2D operator*(const Vector2D& v) const noexcept {
        double x = m[0][0] * v.getX() + m[0][1] * v.getY();
        double y = m[1][0] * v.getX() + m[1][1] * v.getY();
        return Vector2D(x, y);
    }

    void display() const {
        std::cout << "[" << m[0][0] << " " << m[0][1] << "]\n"
                  << "[" << m[1][0] << " " << m[1][1] << "]\n";
    }
};

int main() {
    std::cout << "=== Multiply Operator Overload (*)\n\n";

    // --- Scalar ---
    Vector2D v(3.0, 4.0);
    std::cout << "v = "; v.display();
    std::cout << "v * 2.0 = " << (v * 2.0);
    std::cout << "3.0 * v = " << (3.0 * v);
    std::cout << "\n";

    // --- Matrix ---
    Matrix2x2 A(1, 2, 3, 4);
    Matrix2x2 B(5, 6, 7, 8);

    std::cout << "Matrix A:\n"; A.display();
    std::cout << "Matrix B:\n"; B.display();

    Matrix2x2 C = A * B;
    std::cout << "A * B:\n"; C.display();

    Vector2D result = A * v;
    std::cout << "A * v = "; result.display();

    return 0;
}

/*
==================================================================
Sample I/O:
v = (3, 4)
v * 2.0 = (6, 8)
3.0 * v = (9, 12)

Matrix A:
[1 2]
[3 4]
Matrix B:
[5 6]
[7 8]
A * B:
[19 22]
[43 50]
A * v = (11, 25)
==================================================================
Complexity:
- Scalar*Vector: O(1)
- Matrix*Matrix (2x2): O(1) — 8 mul + 4 add
- Matrix*Vector: O(n^2) for n×n
==================================================================
Interview Questions:
1. Why provide both member and non-member operator*?
   -> Non-member allows scalar * vector; member only vector * scalar

2. Can we use the same operator* for dot product and element-wise?
   -> Overload resolution works by parameter types; different
      semantics need distinct operator* overloads.

3. Performance of matrix multiplication?
   -> Naive O(n^3); Strassen O(n^2.81); for small sizes
      O(n^3) is fine due to cache behavior.
==================================================================
Common Mistakes:
- Forgetting commutativity for scalar multiplication
- Not implementing scalar * vector as non-member
- Matrix multiplication in wrong order (A*B ≠ B*A, not commutative)
- Returning reference to local in operator*
==================================================================
*/
