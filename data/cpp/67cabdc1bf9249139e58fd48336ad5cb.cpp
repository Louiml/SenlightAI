Write a C++ function named `sumRangeWithStep` that takes three integer parameters `start`, `end`, and `step`, and returns the sum of all integers from `start` to `end` inclusive, incrementing by `step` each iteration. The function must handle cases where `start > end` by returning 0 (no values in the range), handle a zero or negative `step` by returning 0 (invalid step), and handle large ranges without overflow by using a 64-bit integer (`long long`) as the return type. The function should not print anything; it should only compute and return the sum. Assume all inputs are within the range of a 32-bit signed integer.

#include <cassert>

int main() {
    // Basic cases
    assert(sumRangeWithStep(50, 100, 1) == 3825); // sum 50..100
    assert(sumRangeWithStep(1, 10, 1) == 55);
    assert(sumRangeWithStep(1, 10, 2) == 25); // 1+3+5+7+9
    assert(sumRangeWithStep(1, 1, 1) == 1);
    assert(sumRangeWithStep(1, 1, 5) == 1);

    // Invalid cases
    assert(sumRangeWithStep(10, 1, 1) == 0); // start > end
    assert(sumRangeWithStep(1, 10, 0) == 0); // zero step
    assert(sumRangeWithStep(1, 10, -2) == 0); // negative step

    // Negative numbers
    assert(sumRangeWithStep(-5, 5, 2) == -5 + (-3) + (-1) + 1 + 3 + 5); // 0
    assert(sumRangeWithStep(-10, -1, 3) == -10 + (-7) + (-4) + (-1)); // -22

    // Large sum test (use long long)
    assert(sumRangeWithStep(0, 1000000, 1) == 500000500000LL);

    return 0;
}

#include <cstddef> // for std::size_t, not strictly needed but for clarity

// Sum all integers from 'start' to 'end' inclusive, stepping by 'step'.
// Returns 0 for invalid inputs (start > end or step <= 0).
// Uses long long to avoid overflow in the sum.
long long sumRangeWithStep(int start, int end, int step) {
    if (start > end || step <= 0) {
        return 0;
    }
    long long total = 0;
    for (int i = start; i <= end; i += step) {
        total += i;
    }
    return total;
}

// The solution iterates from `start` to `end` inclusive, adding each value to an accumulator. The main algorithm is straightforward: validate inputs first. If `start > end`, no numbers are in the range, so return 0. If `step <= 0`, the step is invalid (since we cannot increment or decrement properly), so return 0. Otherwise, use a `for` loop with `int i = start; i <= end; i += step`. However, careful: `i += step` can overflow if `i` is near `INT_MAX`, but since `end` is within `INT_MAX`, and we check `i <= end` before each addition, the loop terminates before overflow occurs (the last valid `i` is ≤ `end`). The accumulator is `long long` to avoid overflow when summing many large integers. Edge cases include: start equal to end (return that single value), step greater than the range (return start), and negative numbers (works naturally). Time complexity is O((end - start)/step + 1) which is linear in the number of terms, but could be large; space complexity is O(1).
