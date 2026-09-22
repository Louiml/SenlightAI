Create a C++ function named `calculateSalary` that takes a single floating-point argument representing an employee’s basic salary, and returns the total salary (basic + dearness allowance + house rent allowance) as a `double`. The function must apply the following tiered allowance rules: for basic ≤ 25,000, DA is 10% and HRA is 15% of basic; for basic ≤ 40,000 (but > 25,000), DA is 12% and HRA is 18% of basic; for basic > 40,000, DA is 15% and HRA is 20% of basic. The function must handle negative, zero, and very large inputs gracefully (no special behavior, just apply the same arithmetic). Use `const` where appropriate, return the computed total, and do not read from or write to any console inside the function.
The algorithm is straightforward conditional arithmetic. First, the function checks the value of the input `basic` against the two thresholds (25,000 and 40,000) in order. Since the conditions are mutually exclusive and the first `if` covers values ≤ 25,000, the `else if` naturally handles values from 25,000.01 to 40,000, and the final `else` handles anything above 40,000. For each branch, DA and HRA are computed as `(basic * percentage) / 100` using floating-point multiplication and division. The total salary is then `basic + DA + HRA`. Edge cases include: zero basic (both allowances zero, total zero), negative basic (percentages applied to negative numbers, producing negative DA/HRA — this is mathematically consistent, though nonsensical in real life; the function should not crash), and very large values (floating-point precision may introduce rounding, but the algorithm remains correct within `double` limits). Time complexity is O(1) because only a few arithmetic operations and comparisons are performed, regardless of input size. Space complexity is O(1) as only a few local variables are used.
#include <cstddef>  // Not strictly needed, but keep for clarity

// Calculate total salary based on basic pay and tiered DA/HRA percentages.
// Returns (basic + DA + HRA) as a double.
double calculateSalary(const double basic) {
    double da = 0.0;
    double hra = 0.0;

    if (basic <= 25000.0) {
        da = (basic * 10.0) / 100.0;
        hra = (basic * 15.0) / 100.0;
    } else if (basic <= 40000.0) {
        da = (basic * 12.0) / 100.0;
        hra = (basic * 18.0) / 100.0;
    } else {
        da = (basic * 15.0) / 100.0;
        hra = (basic * 20.0) / 100.0;
    }

    return basic + da + hra;
}
#include <cassert>
#include <cmath>

// Assume calculateSalary is defined as above.

int main() {
    // Exact thresholds
    assert(calculateSalary(25000.0) == 25000.0 + 2500.0 + 3750.0); // DA=10%, HRA=15%
    assert(calculateSalary(40000.0) == 40000.0 + 4800.0 + 7200.0); // DA=12%, HRA=18%
    
    // Interior values
    assert(calculateSalary(10000.0) == 10000.0 + 1000.0 + 1500.0);
    assert(calculateSalary(30000.0) == 30000.0 + 3600.0 + 5400.0);
    assert(calculateSalary(50000.0) == 50000.0 + 7500.0 + 10000.0);

    // Edge cases
    assert(calculateSalary(0.0) == 0.0);
    assert(calculateSalary(-5000.0) == -5000.0 + (-500.0) + (-750.0)); // negative, still consistent
    assert(std::fabs(calculateSalary(25001.0) - (25001.0 + 3000.12 + 4500.18)) < 0.001); // just above first threshold

    // Large value (no overflow likely for double)
    double big = 1e12;
    assert(std::fabs(calculateSalary(big) - (big + big*0.15 + big*0.20)) < 1e3); // DA=15%, HRA=20%
}
