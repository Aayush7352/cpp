/*
 * ====================================================================
 * Program 20: Grade Calculator
 *
 * THEORY:
 * Computes final grade as a weighted average of multiple components:
 *   Grade = Σ (score_i × weight_i) / Σ weight_i
 *
 * Converts percentage to letter grade:
 *   90-100  → A
 *   80-89   → B
 *   70-79   → C
 *   60-69   → D
 *   0-59    → F
 *
 * Includes input validation:
 *   - Each component score: 0–100
 *   - Each weight: non-negative
 *   - Total weight should be reasonable (sum weights before dividing)
 *
 * SAMPLE INPUT/OUTPUT:
 *   Input:
 *     Enter number of components: 3
 *     Component 1 — score: 85, weight: 40
 *     Component 2 — score: 90, weight: 30
 *     Component 3 — score: 78, weight: 30
 *   Output:
 *     Weighted average: 84.40%
 *     Letter grade: B
 *
 * TIME COMPLEXITY:   O(n) — n = number of components
 * SPACE COMPLEXITY:  O(1)
 *
 * COMMON INTERVIEW QUESTIONS:
 *   Q: Why use double for weighted average?
 *   A: To avoid integer division truncation in the final result.
 *   Q: How do you handle invalid input robustly?
 *   A: Check cin.fail(), clear the error flag, ignore the bad input,
 *      and prompt again.
 *   Q: switch vs if-else for grade boundaries?
 *   A: if-else is better here since grades are ranges, not discrete
 *      values.
 *
 * COMMON MISTAKES:
 *   - Integer division giving 0 for weighted fraction.
 *   - Not validating scores (must be 0–100).
 *   - Using switch for range-based conditions.
 *   - Off-by-one in grade boundaries (is 90 an A or B?).
 * ====================================================================
 */

#include <iostream>
#include <iomanip>
#include <limits>
#include <vector>

double get_double(const std::string& prompt, double min_val, double max_val)
{
    double val{};
    while (true)
    {
        std::cout << prompt;
        std::cin >> val;

        if (std::cin.fail() || val < min_val || val > max_val)
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Enter a value between "
                      << min_val << " and " << max_val << ".\n";
        }
        else
        {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return val;
        }
    }
}

char percentage_to_grade(double pct)
{
    if (pct >= 90.0) return 'A';
    if (pct >= 80.0) return 'B';
    if (pct >= 70.0) return 'C';
    if (pct >= 60.0) return 'D';
    return 'F';
}

int main()
{
    std::cout << "=== Grade Calculator ===\n\n";

    int num_components = static_cast<int>(
        get_double("Number of graded components: ", 1, 100));

    double total_score{};
    double total_weight{};

    for (int i = 1; i <= num_components; ++i)
    {
        std::cout << "\n--- Component " << i << " ---\n";

        double score  = get_double("  Score (0–100):  ", 0.0, 100.0);
        double weight = get_double("  Weight (≥ 0):   ", 0.0, 1e9);

        total_score  += score * weight;
        total_weight += weight;
    }

    if (total_weight == 0.0)
    {
        std::cout << "\nError: Total weight is zero. Cannot compute grade.\n";
        return 1;
    }

    double average = total_score / total_weight;
    char grade = percentage_to_grade(average);

    std::cout << "\n=== Final Result ===\n";
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Weighted average: " << average << "%\n";
    std::cout << "Letter grade:     " << grade << '\n';

    return 0;
}
