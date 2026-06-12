/*
==================================================================
134. Stream Operators (<< and >>)
==================================================================
Theory:
- operator<< and operator>> must be non-member functions
  (ostream/istream is left operand, cannot modify standard streams)
- Typically declared as friend for private member access
- Return reference to the stream for chaining (cout << a << b)
- Format flags, width, precision, fill can be customized
*/

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <stdexcept>

class Date {
private:
    int m_day, m_month, m_year;

public:
    Date(int d = 1, int m = 1, int y = 2000)
        : m_day(d), m_month(m), m_year(y) {
        if (d < 1 || d > 31 || m < 1 || m > 12)
            throw std::invalid_argument("Invalid date");
    }

    // Getters needed if not friend (but we use friend below)
    int day()   const noexcept { return m_day; }
    int month() const noexcept { return m_month; }
    int year()  const noexcept { return m_year; }

    friend std::ostream& operator<<(std::ostream& os, const Date& dt);
    friend std::istream& operator>>(std::istream& is, Date& dt);
};

// Output: formatted as YYYY-MM-DD
std::ostream& operator<<(std::ostream& os, const Date& dt) {
    os << std::setfill('0')
       << std::setw(4) << dt.m_year << '-'
       << std::setw(2) << dt.m_month << '-'
       << std::setw(2) << dt.m_day;
    return os;
}

// Input: reads three integers separated by whitespace or delimiters
std::istream& operator>>(std::istream& is, Date& dt) {
    int d, m, y;
    char c1, c2;

    // Attempt to parse YYYY-MM-DD or YYYY/MM/DD
    if (is >> y >> c1 >> m >> c2 >> d) {
        if ((c1 == '-' || c1 == '/') && (c2 == '-' || c2 == '/')) {
            if (d >= 1 && d <= 31 && m >= 1 && m <= 12) {
                dt.m_day = d;
                dt.m_month = m;
                dt.m_year = y;
            } else {
                is.setstate(std::ios::failbit);
            }
        } else {
            is.setstate(std::ios::failbit);
        }
    }
    return is;
}

int main() {
    std::cout << "=== Stream Operators (<< and >>)\n\n";

    Date today(11, 6, 2026);
    std::cout << "Today: " << today << "\n\n";

    // Chaining
    Date d1(1, 1, 2020), d2(25, 12, 2025);
    std::cout << "Holidays: " << d1 << " to " << d2 << "\n\n";

    // Input parsing
    std::cout << "Enter a date (YYYY-MM-DD): ";
    Date input;
    if (std::cin >> input) {
        std::cout << "You entered: " << input << "\n";
    } else {
        std::cout << "Invalid input.\n";
        std::cin.clear();
    }

    // Formatted output with different styles
    std::cout << "\nCustom formatting:\n";
    std::cout << "Default:  " << today << "\n";
    std::cout << "Full:     " << today.day() << '/' 
              << today.month() << '/' << today.year() << "\n";

    return 0;
}

/*
==================================================================
Sample I/O:
Today: 2026-06-11

Holidays: 2020-01-01 to 2025-12-25

Enter a date (YYYY-MM-DD): 2024-07-04
You entered: 2024-07-04

Custom formatting:
Default:  2026-06-11
Full:     11/6/2026
==================================================================
Complexity:
- Time: O(1) for single date formatting/parsing
- Space: O(1)
==================================================================
Interview Questions:
1. Why must stream operators be non-member functions?
   -> Left operand is std::ostream/istream; member function
      would require modifying std::ostream class.

2. How to detect input failures?
   -> Check stream state: is.good(), is.fail(). On failure
      setstate(failbit); caller tests if (cin >> x).

3. How to support chaining (cout << a << b)?
   -> Return ostream& from operator<<.
==================================================================
Common Mistakes:
- Declaring stream operators as member functions
- Not returning reference to the stream (breaks chaining)
- Forgetting to handle input errors (set failbit)
- Using std::endl inside operator<< (surprising behavior)
- Not resetting format flags after custom formatting
==================================================================
*/
