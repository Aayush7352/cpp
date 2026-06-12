/*
==================================================================
136. Custom Math Class — Complex Number
==================================================================
Theory:
- Complex number: a + bi where i^2 = -1
- Full operator overloading: +, -, *, /, ==, !=, <<, >>, unary -
- Math functions: conj, norm, abs, arg
- C++20 <=> and three-way comparison
- Friends used for non-member operators that need private access
*/

#include <iostream>
#include <cmath>
#include <compare>
#include <sstream>
#include <stdexcept>

class Complex {
private:
    double m_re, m_im;

public:
    Complex(double re = 0.0, double im = 0.0) noexcept
        : m_re(re), m_im(im) {}

    // Getters
    double real() const noexcept { return m_re; }
    double imag() const noexcept { return m_im; }

    // --- Arithmetic operators ---
    Complex operator+(const Complex& rhs) const noexcept {
        return Complex(m_re + rhs.m_re, m_im + rhs.m_im);
    }
    Complex operator-(const Complex& rhs) const noexcept {
        return Complex(m_re - rhs.m_re, m_im - rhs.m_im);
    }
    Complex operator-() const noexcept { // unary
        return Complex(-m_re, -m_im);
    }
    Complex operator*(const Complex& rhs) const noexcept {
        // (a+bi)(c+di) = (ac-bd) + (ad+bc)i
        return Complex(m_re * rhs.m_re - m_im * rhs.m_im,
                       m_re * rhs.m_im + m_im * rhs.m_re);
    }
    Complex operator/(const Complex& rhs) const {
        // (a+bi)/(c+di) = (a+bi)(c-di)/(c^2+d^2)
        double denom = rhs.m_re * rhs.m_re + rhs.m_im * rhs.m_im;
        if (denom == 0)
            throw std::domain_error("Division by zero complex");
        return Complex((m_re * rhs.m_re + m_im * rhs.m_im) / denom,
                       (m_im * rhs.m_re - m_re * rhs.m_im) / denom);
    }

    // --- Compound assignment ---
    Complex& operator+=(const Complex& rhs) noexcept {
        m_re += rhs.m_re; m_im += rhs.m_im; return *this;
    }
    Complex& operator-=(const Complex& rhs) noexcept {
        m_re -= rhs.m_re; m_im -= rhs.m_im; return *this;
    }
    Complex& operator*=(const Complex& rhs) noexcept {
        *this = *this * rhs; return *this;
    }
    Complex& operator/=(const Complex& rhs) {
        *this = *this / rhs; return *this;
    }

    // --- Comparison (C++20) ---
    auto operator<=>(const Complex& rhs) const {
        // Compare by magnitude first, then real
        double mag1 = norm();
        double mag2 = rhs.norm();
        if (mag1 != mag2) return mag1 <=> mag2;
        return m_re <=> rhs.m_re;
    }
    bool operator==(const Complex& rhs) const {
        return m_re == rhs.m_re && m_im == rhs.m_im;
    }

    // --- Math functions ---
    double norm()  const noexcept { return m_re * m_re + m_im * m_im; }
    double abs()   const noexcept { return std::sqrt(norm()); }
    double arg()   const noexcept { return std::atan2(m_im, m_re); }
    Complex conj() const noexcept { return Complex(m_re, -m_im); }

    // --- Stream ---
    friend std::ostream& operator<<(std::ostream& os, const Complex& c);
    friend std::istream& operator>>(std::istream& is, Complex& c);
};

std::ostream& operator<<(std::ostream& os, const Complex& c) {
    os << c.m_re;
    if (c.m_im >= 0) os << "+" << c.m_im << "i";
    else             os << c.m_im << "i";
    return os;
}

std::istream& operator>>(std::istream& is, Complex& c) {
    double re, im;
    char plus, i;
    if (is >> re >> plus >> im >> i) {
        if (plus != '+' && plus != '-') {
            is.setstate(std::ios::failbit);
            return is;
        }
        if (plus == '-') im = -im;
        if (i != 'i') {
            is.setstate(std::ios::failbit);
            return is;
        }
        c.m_re = re;
        c.m_im = im;
    }
    return is;
}

// --- Standalone math ---
Complex polar(double r, double theta) noexcept {
    return Complex(r * std::cos(theta), r * std::sin(theta));
}

int main() {
    std::cout << "=== Custom Complex Number Class\n\n";

    Complex a(3.0, 4.0);
    Complex b(1.0, -2.0);

    std::cout << "a = " << a << "\n";
    std::cout << "b = " << b << "\n\n";

    std::cout << "a + b = " << (a + b) << "\n";
    std::cout << "a - b = " << (a - b) << "\n";
    std::cout << "a * b = " << (a * b) << "\n";
    std::cout << "a / b = " << (a / b) << "\n";
    std::cout << "-a    = " << (-a) << "\n\n";

    std::cout << "|a|   = " << a.abs() << "\n";
    std::cout << "arg a = " << a.arg() << " rad\n";
    std::cout << "conj a= " << a.conj() << "\n\n";

    std::cout << "a == Complex(3,4) : " << (a == Complex(3, 4)) << "\n";
    std::cout << "a != b            : " << (a != b) << "\n";
    std::cout << "a > b             : " << (a > b) << "\n";

    std::cout << "\nEnter complex (a+bi): ";
    Complex input;
    if (std::cin >> input)
        std::cout << "You entered: " << input << "\n";

    return 0;
}

/*
==================================================================
Sample I/O:
a = 3+4i
b = 1-2i

a + b = 4+2i
a - b = 2+6i
a * b = 11-2i
a / b = -1+2i
-a    = -3-4i

|a|   = 5
arg a = 0.927295 rad
conj a= 3-4i

a == Complex(3,4) : 1
a != b            : 1
a > b             : 1

Enter complex (a+bi): 2+3i
You entered: 2+3i
==================================================================
Complexity:
- Each operation: O(1)
- Math functions (abs, arg): O(1) via std::sqrt, std::atan2
==================================================================
Interview Questions:
1. Why can't we define operator+ as a friend and member both?
   -> Overload resolution would be ambiguous. Choose one.

2. How to make complex division exception-safe?
   -> Check denom != 0 before division; throw domain_error.

3. Difference between norm and abs?
   -> norm = re^2 + im^2 (squared magnitude, no sqrt);
      abs = sqrt(norm).
==================================================================
Common Mistakes:
- Incorrect multiplication formula (FOIL error)
- Division without checking denominator for zero
- Using int for component type (precision loss)
- Not marking operations noexcept where appropriate
- failing to handle operator>> input validation correctly
==================================================================
*/
