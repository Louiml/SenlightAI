You are given four integers: `a` (the time the train departs from station A), `b` (the time the train arrives at station B), `c` (the delay in minutes if the train is late, applying only when it arrives after `a` but not later than `b`), and `x` (the actual arrival time at station B). Write a C++ function named `probabilityOnTime` that takes these four integers as parameters (all non-negative, with `0 ≤ a < b` and `0 ≤ c ≤ b-a`) and returns a `double` representing the probability that the train arrives exactly on time or earlier, given that if `x` is in the interval `(a, b]`, the train is considered "on time" with probability `c / (b - a)`; if `x ≤ a`, the probability is exactly 1.0; and if `x > b`, the probability is exactly 0.0. The function should compute the result with high precision and return it as a `double`. Note that the probability is always between 0 and 1 inclusive, and the output should be accurate to at least 12 decimal places when printed.

// The problem is a straightforward piecewise linear mapping from input `x` to a probability value. The main algorithm is a simple conditional check: first, if `x` is less than or equal to `a`, return 1.0 (since the train is definitely on time when arrival is at or before the original departure time). Second, if `x` is between `a+1` and `b` inclusive (i.e., `x > a` and `x ≤ b`), the probability is `c / (b - a)`, which is a constant value independent of `x` within that interval. Third, if `x` exceeds `b`, return 0.0. The important edge case is when `a == b` (though the problem specifies `a < b`, we could still handle it defensively by returning 1.0 if `x ≤ a` and 0.0 otherwise, avoiding division by zero). Another edge case is that `c` can be zero, giving a probability of 0 in the middle interval. The computation of `c / (b - a)` should be done using floating-point division (cast to `double`) to avoid integer truncation. The time complexity is O(1) and space complexity is O(1), as only constant operations and a single double are involved.

#include <algorithm>  // for std::min, std::max (though not needed, kept for completeness)

// Compute the probability that the train is on time given arrival time x.
// Preconditions: 0 <= a < b, 0 <= c <= b - a, and x is non-negative.
double probabilityOnTime(int a, int b, int c, int x) {
    // If arrival is at or before the scheduled departure time: definitely on time.
    if (x <= a) {
        return 1.0;
    }
    // If arrival is between a+1 and b inclusive: probability is constant c/(b-a).
    if (x <= b) {
        // Ensure floating-point division to avoid integer truncation.
        return static_cast<double>(c) / static_cast<double>(b - a);
    }
    // If arrival is after b: definitely late.
    return 0.0;
}

#include <cassert>
#include <cmath>

int main() {
    // Basic case: a=1, b=5, c=2, x=3 (between a and b) -> 2/(5-1)=0.5
    assert(std::abs(probabilityOnTime(1, 5, 2, 3) - 0.5) < 1e-12);
    // x exactly at a -> 1.0
    assert(probabilityOnTime(1, 5, 2, 1) == 1.0);
    // x less than a -> 1.0
    assert(probabilityOnTime(1, 5, 2, 0) == 1.0);
    // x exactly at b -> 2/(5-1)=0.5
    assert(std::abs(probabilityOnTime(1, 5, 2, 5) - 0.5) < 1e-12);
    // x greater than b -> 0.0
    assert(probabilityOnTime(1, 5, 2, 6) == 0.0);
    // c = 0 -> middle interval probability is 0.0
    assert(probabilityOnTime(1, 5, 0, 3) == 0.0);
    // a=0, b=10, c=7, x between -> 0.7
    assert(std::abs(probabilityOnTime(0, 10, 7, 4) - 0.7) < 1e-12);
    // Large b-a to check precision
    double res = probabilityOnTime(2, 1000000, 999998, 500000);
    assert(std::abs(res - 999998.0 / 999998.0) < 1e-12);  // Actually c/(b-a) = 999998/999998 = 1.0, but note x is within (a,b] so yes.
    // Wait, 999998/999998 = 1.0, let's verify division: (b-a)=999998, c=999998 -> 1.0
    assert(std::abs(res - 1.0) < 1e-12);
    // Edge: x = a + 0? Already covered. Also test x = a+1 small.
    assert(std::abs(probabilityOnTime(3, 4, 1, 3) - 1.0) < 1e-12); // x<=a -> 1.0
    assert(std::abs(probabilityOnTime(3, 4, 1, 4) - 1.0) < 1e-12); // x==b -> 1/(4-3)=1.0
    assert(std::abs(probabilityOnTime(3, 4, 0, 4) - 0.0) < 1e-12); // c=0 at b -> 0/(1)=0.0
    return 0;
}
