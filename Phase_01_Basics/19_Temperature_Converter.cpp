/*
 * ====================================================================
 * Program 19: Temperature Converter
 *
 * THEORY:
 * Conversion formulas:
 *   Celsius → Fahrenheit:  F = (C × 9/5) + 32
 *   Celsius → Kelvin:      K = C + 273.15
 *   Fahrenheit → Celsius:  C = (F − 32) × 5/9
 *   Fahrenheit → Kelvin:   K = (F − 32) × 5/9 + 273.15
 *   Kelvin → Celsius:      C = K − 273.15
 *   Kelvin → Fahrenheit:   F = (K − 273.15) × 9/5 + 32
 *
 * Absolute zero: 0 K = −273.15°C = −459.67°F
 *
 * FORMATTING:
 *   std::setprecision(n)   — decimal places
 *   std::fixed             — fixed-point notation
 *   std::showpoint         — always show decimal point
 *
 * SAMPLE INPUT/OUTPUT:
 *   Input:
 *     Enter temperature: 100
 *     Enter unit (C/F/K): C
 *   Output:
 *     100.00 °C = 212.00 °F = 373.15 K
 *
 * TIME COMPLEXITY:   O(1)
 * SPACE COMPLEXITY:  O(1)
 *
 * COMMON INTERVIEW QUESTIONS:
 *   Q: What is absolute zero in all three scales?
 *   A: 0 K, −273.15°C, −459.67°F
 *   Q: Why use constexpr for conversion functions?
 *   A: They can be computed at compile time if inputs are constant.
 *   Q: How does std::fixed affect setprecision?
 *   A: Without fixed, setprecision controls total significant digits;
 *      with fixed, it controls digits after the decimal point.
 *
 * COMMON MISTAKES:
 *   - Using integer division (9/5 = 1 instead of 1.8).
 *   - Forgetting the parentheses in F→C formula.
 *   - Not validating for physically impossible temperatures (< 0K).
 *   - Mixing up uppercase/lowercase unit input.
 * ====================================================================
 */

#include <iostream>
#include <iomanip>
#include <limits>
#include <cctype>  // std::toupper

constexpr double C_TO_F(double c)  { return c * 9.0 / 5.0 + 32.0; }
constexpr double C_TO_K(double c)  { return c + 273.15; }
constexpr double F_TO_C(double f)  { return (f - 32.0) * 5.0 / 9.0; }
constexpr double F_TO_K(double f)  { return (f - 32.0) * 5.0 / 9.0 + 273.15; }
constexpr double K_TO_C(double k)  { return k - 273.15; }
constexpr double K_TO_F(double k)  { return (k - 273.15) * 9.0 / 5.0 + 32.0; }

bool is_valid_kelvin(double k)
{
    return k >= 0.0;
}

int main()
{
    double temp{};
    char unit{};

    std::cout << "=== Temperature Converter ===\n";

    std::cout << "Enter temperature value: ";
    std::cin >> temp;
    if (std::cin.fail())
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid temperature value.\n";
        return 1;
    }

    std::cout << "Enter unit (C / F / K): ";
    std::cin >> unit;
    unit = static_cast<char>(std::toupper(static_cast<unsigned char>(unit)));

    std::cout << std::fixed << std::setprecision(2);

    switch (unit)
    {
    case 'C':
        if (!is_valid_kelvin(C_TO_K(temp)))
        {
            std::cout << "Error: Temperature below absolute zero!\n";
            return 1;
        }
        std::cout << temp << " °C = "
                  << C_TO_F(temp) << " °F = "
                  << C_TO_K(temp) << " K\n";
        break;

    case 'F':
        if (!is_valid_kelvin(F_TO_K(temp)))
        {
            std::cout << "Error: Temperature below absolute zero!\n";
            return 1;
        }
        std::cout << temp << " °F = "
                  << F_TO_C(temp) << " °C = "
                  << F_TO_K(temp) << " K\n";
        break;

    case 'K':
        if (!is_valid_kelvin(temp))
        {
            std::cout << "Error: Temperature below absolute zero (0 K)!\n";
            return 1;
        }
        std::cout << temp << " K = "
                  << K_TO_C(temp) << " °C = "
                  << K_TO_F(temp) << " °F\n";
        break;

    default:
        std::cout << "Invalid unit. Use C, F, or K.\n";
        return 1;
    }

    return 0;
}
