Write a C++ function `calculateAdmissionFee(int age, int baseFee)` that, given a visitor's age and the standard admission fee for an attraction, returns the admission fee they should pay according to the following rules: adults (age 13 and older) pay the full base fee; children (ages 6 through 12 inclusive) pay half the base fee (integer division, so any fractional part is discarded); and children under 6 years old enter for free (fee 0). The function should accept the age as an `int` and base fee as a non-negative `int`, and return an `int` result. Assume age is non-negative and baseFee is non-negative. Your solution must not use any loops, recursion, or external libraries beyond the standard ones.
#include <cassert>

int main() {
    // Adults (13+)
    assert(calculateAdmissionFee(13, 20) == 20);
    assert(calculateAdmissionFee(30, 50) == 50);
    assert(calculateAdmissionFee(100, 7) == 7);

    // Children aged 6-12: half fee with integer truncation
    assert(calculateAdmissionFee(6, 20) == 10);
    assert(calculateAdmissionFee(12, 20) == 10);
    assert(calculateAdmissionFee(9, 5) == 2); // 5/2 = 2.5 -> truncates to 2
    assert(calculateAdmissionFee(6, 1) == 0); // 1/2 = 0

    // Under 6: free
    assert(calculateAdmissionFee(0, 100) == 0);
    assert(calculateAdmissionFee(5, 100) == 0);

    // Edge: age exactly 6 and 13 boundaries already covered; zero fee
    assert(calculateAdmissionFee(0, 0) == 0);
    return 0;
}
#include <algorithm> // for std::max, though not strictly needed here

// Return admission fee based on age and base fee.
// Full fee for age >= 13, half fee (integer division) for 6-12, free for under 6.
int calculateAdmissionFee(int age, int baseFee) {
    if (age >= 13) {
        return baseFee;
    } else if (age >= 6) { // age is between 6 and 12 inclusive
        return baseFee / 2;
    } else { // age < 6
        return 0;
    }
}
// The solution is a direct translation of the conditional pricing structure into a decision tree. We check three mutually exclusive age ranges in order: if `age >= 13`, return `baseFee`; else if `age >= 6` (which also guarantees `age <= 12` since the first condition failed), return `baseFee / 2` (using integer division, which automatically truncates toward zero, giving the “half” price for odd amounts); else (meaning `age < 6`), return `0`. Edge cases include boundary ages exactly 6, 12, and 13 — the conditions use inclusive comparisons to correctly classify them. Also, if `baseFee` is odd (e.g., 5), integer division gives 2, which matches the expected truncation. Time complexity is O(1) with constant operations, and space complexity is O(1) — no extra data structures are needed.
