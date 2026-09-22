// Write a C++ function named `classifySumPosition` that takes two `long long` integers `x` and `y`. It should return a string: `"Nol"` if `-x` equals `y`, `"Positif"` if `-x` is strictly less than `y`, and `"Negatif"` otherwise (i.e., when `-x` is strictly greater than `y`). Handle the full range of `long long` values including negatives, zeros, and extreme values. The function must be self-contained, take parameters by value, and return the result as a `std::string`. Do not read from standard input or write to standard output; the function should be pure and testable.
The solution is straightforward: compute the comparison between `-x` and `y`. The critical edge case is when `x` is `LLONG_MIN` (the most negative `long long`), because negating it directly would overflow. To avoid undefined behavior, instead of computing `-x`, we can compare using `y` and `x` directly: the condition `-x == y` is equivalent to `x + y == 0`? Not exactly, because overflow could also occur in `x + y`. Better: compare `-x` and `y` without negating `x` by restructuring the inequality. The original conditions are:  
- `-x == y` → `x == -y` (but this also risks overflow if `y = LLONG_MIN`).  
Instead, we can use the fact that `-x < y` ⇔ `x > -y` (multiplying by -1 reverses direction). However, `-y` could overflow. The safest method is to convert to `__int128` if available (GCC/Clang) to avoid overflow entirely. Since the problem statement doesn't forbid `__int128`, we can use it for robust handling. Alternatively, without `__int128`, we can compare using logic:  
- If `x > 0` and `y < 0`: then `-x < 0` and `y < 0` → compare using `-x` safely? `-x` might overflow if `x = LLONG_MAX`? No, `-LLONG_MAX` is fine. Actually `-x` overflows only when `x = LLONG_MIN`.  
So handle the case `x == LLONG_MIN` separately: then `-x` is undefined, but we can reason: `-x` would be `LLONG_MAX + 1` (beyond range), so it's larger than any `long long` value `y`. Thus:  
- If `x == LLONG_MIN`: then `-x` is effectively `> LLONG_MAX` → so `-x == y` is false (since `y` ≤ LLONG_MAX), and `-x < y` is false (since `-x` is larger than all possible `y`). Therefore, result is `"Negatif"`.  
Otherwise, compute `long long negX = -x;` safely, then compare with `y`. This handles all cases. Time complexity O(1), space O(1).
#include <string>
#include <climits>

// Classify the relationship between -x and y.
// Returns "Nol" if -x == y, "Positif" if -x < y, otherwise "Negatif".
std::string classifySumPosition(long long x, long long y) {
    // The only overflow risk is when x == LLONG_MIN, because -x would be
    // out of range. In that case, -x is conceptually larger than any LLONG_MAX,
    // so it cannot equal y and cannot be less than y.
    if (x == LLONG_MIN) {
        return "Negatif";
    }

    long long negX = -x;  // Safe now, since x != LLONG_MIN.
    if (negX == y) {
        return "Nol";
    } else if (negX < y) {
        return "Positif";
    } else {
        return "Negatif";
    }
}
#include <cassert>
#include <string>
#include <climits>

// The solution function (for completeness, placed above main in a real file)
std::string classifySumPosition(long long x, long long y);

int main() {
    // Basic cases
    assert(classifySumPosition(5, -5) == "Nol");
    assert(classifySumPosition(5, -4) == "Positif");   // -5 < -4
    assert(classifySumPosition(5, -6) == "Negatif");   // -5 > -6

    // Zero and positive y
    assert(classifySumPosition(0, 0) == "Nol");
    assert(classifySumPosition(0, 1) == "Positif");    // 0 < 1
    assert(classifySumPosition(0, -1) == "Negatif");   // 0 > -1

    // Negative x values
    assert(classifySumPosition(-3, 3) == "Nol");       // 3 == 3
    assert(classifySumPosition(-3, 2) == "Negatif");   // 3 > 2
    assert(classifySumPosition(-3, 4) == "Positif");   // 3 < 4

    // Edge case: LLONG_MIN
    assert(classifySumPosition(LLONG_MIN, LLONG_MAX) == "Negatif");
    assert(classifySumPosition(LLONG_MIN, LLONG_MIN) == "Negatif");

    // Extreme positive x
    assert(classifySumPosition(LLONG_MAX, -LLONG_MAX) == "Nol");
    assert(classifySumPosition(LLONG_MAX, -LLONG_MAX + 1) == "Positif");
    assert(classifySumPosition(LLONG_MAX, -LLONG_MAX - 1) == "Negatif");

    return 0;
}
