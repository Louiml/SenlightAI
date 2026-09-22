// Write a C++ function `computeTotalSalary(int basicSalary, int cityTier)` that calculates and returns the total monthly salary of an employee based on the given basic salary and a city tier (1, 2, or 3). The total salary is computed as: basic salary + dearness allowance (44% of basic) + travel allowance (8% of basic) + house rent allowance, where the HRA percentage depends on the tier: 24% for tier 1, 16% for tier 2, and 12% for tier 3. If the tier is not 1, 2, or 3, the function should return `-1` to indicate an invalid tier. The function must use exact floating-point arithmetic for the percentage calculations and return the result as an integer (truncating any fractional part, consistent with the original snippet's behavior). Handle only positive integer basic salaries and all integer tier values.

// The solution implements a straightforward selection structure: check the city tier to determine the HRA percentage, compute the three allowances using floating-point multiplication (since percentages like 0.44, 0.08, 0.24 are not exactly representable in binary floating-point, but the original snippet uses `double` implicit conversions), sum them with the basic salary, and return the truncated integer result. The main edge case is an invalid tier: the original snippet prints "Invalid tier" and leaves `h` uninitialized, leading to undefined behavior; our specification fixes this by returning `-1`. Another edge case is when the basic salary is 0 or extremely small; the function still works correctly. Time complexity is O(1) because only a fixed number of arithmetic operations and comparisons are performed. Space complexity is O(1) as no additional data structures are used. For very large basic salaries near the maximum `int`, the multiplication by 0.44 and 0.08 could overflow `int` if direct integer arithmetic were used, but using `double` for the calculations and then truncating avoids overflow (provided the product fits in `double`'s range, which is far larger). The final truncation to `int` may overflow if the total exceeds `INT_MAX`, but that is outside the expected input range.

#include <cmath>  // for std::trunc, though static_cast<int> suffices

// Compute total salary from basic and city tier (1,2,3).
// Returns -1 if tier is invalid.
int computeTotalSalary(int basicSalary, int cityTier) {
    // Determine HRA percentage based on tier.
    double hraRate = 0.0;
    if (cityTier == 1) {
        hraRate = 0.24;
    } else if (cityTier == 2) {
        hraRate = 0.16;
    } else if (cityTier == 3) {
        hraRate = 0.12;
    } else {
        return -1;  // Invalid tier.
    }

    // Compute allowances as doubles to avoid integer truncation during multiplication.
    const double dearnessAllowance = basicSalary * 0.44;
    const double travelAllowance = basicSalary * 0.08;
    const double houseRentAllowance = basicSalary * hraRate;

    // Total salary: basic + all allowances.
    const double total = basicSalary + dearnessAllowance + travelAllowance + houseRentAllowance;

    // Truncate fractional part (original snippet prints with %d, so truncation).
    return static_cast<int>(total);
}

#include <cassert>

// Forward declaration of the solution function.
int computeTotalSalary(int, int);

int main() {
    // Test valid tiers with basic salary 10000.
    assert(computeTotalSalary(10000, 1) == 17600);  // 10000 + 4400 + 800 + 2400
    assert(computeTotalSalary(10000, 2) == 16800);  // 10000 + 4400 + 800 + 1600
    assert(computeTotalSalary(10000, 3) == 16400);  // 10000 + 4400 + 800 + 1200

    // Test with zero basic salary.
    assert(computeTotalSalary(0, 1) == 0);

    // Test with a non-multiple-of-100 salary to check truncation.
    // 1234 * 0.44 = 542.96 -> truncates to 542
    // 1234 * 0.08 = 98.72 -> truncates to 98
    // 1234 * 0.24 = 296.16 -> truncates to 296
    // But note: the sum is computed as double, then truncated once.
    // 1234 + 542.96 + 98.72 + 296.16 = 2171.84 -> truncates to 2171
    assert(computeTotalSalary(1234, 1) == 2171);

    // Test invalid tier.
    assert(computeTotalSalary(5000, 4) == -1);
    assert(computeTotalSalary(5000, 0) == -1);
    assert(computeTotalSalary(5000, -1) == -1);

    // Test large salary within int range (no overflow in double).
    assert(computeTotalSalary(1000000, 2) == 1680000); // 1000000 + 440000 + 80000 + 160000

    return 0;
}
