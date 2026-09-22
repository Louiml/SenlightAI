Write a C++ function that takes three integers `a`, `b`, and `c` representing the coefficients of a quadratic equation `ax² + bx + c = 0`, and returns a `std::pair<int, int>` containing the two real roots in ascending order. The function must handle the case where the discriminant is negative by returning `{0, 0}` (a sentinel indicating no real roots), handle the case where the discriminant is zero (returning the repeated root correctly), and correctly compute the roots using the standard quadratic formula. The input coefficients can be any valid integers, but you may assume that `a` is non-zero (to avoid division by zero) and that all intermediate calculations fit within a `double`. The function must be `const`-correct and free of side effects.
#include <cassert>
#include <utility>

// Assume the solution function is declared above.

int main() {
    // x^2 - 3x + 2 = 0 -> roots 1, 2
    auto roots1 = solveQuadratic(1, -3, 2);
    assert(roots1.first == 1 && roots1.second == 2);

    // x^2 - 2x + 1 = 0 -> repeated root 1
    auto roots2 = solveQuadratic(1, -2, 1);
    assert(roots2.first == 1 && roots2.second == 1);

    // x^2 + 2x + 5 = 0 -> no real roots -> {0,0}
    auto roots3 = solveQuadratic(1, 2, 5);
    assert(roots3.first == 0 && roots3.second == 0);

    // 2x^2 - 4x - 6 = 0 -> roots -1, 3 (sorted)
    auto roots4 = solveQuadratic(2, -4, -6);
    assert(roots4.first == -1 && roots4.second == 3);

    // x^2 - 1 = 0 -> roots -1, 1
    auto roots5 = solveQuadratic(1, 0, -1);
    assert(roots5.first == -1 && roots5.second == 1);

    // -x^2 + 5x - 6 = 0 -> roots 2, 3 (sorted, negative a)
    auto roots6 = solveQuadratic(-1, 5, -6);
    assert(roots6.first == 2 && roots6.second == 3);
}
#include <cmath>
#include <utility>
#include <algorithm>

// Solve ax^2 + bx + c = 0 for real integer roots.
// Returns the two roots in ascending order, or {0,0} if no real roots exist.
// Assumes a != 0 and that roots are integers (discriminant is a perfect square).
std::pair<int, int> solveQuadratic(int a, int b, int c) {
    const long long discriminant = static_cast<long long>(b) * b - 4LL * a * c;
    if (discriminant < 0) {
        return {0, 0};
    }
    
    const double sqrtD = std::sqrt(static_cast<double>(discriminant));
    const double denom = 2.0 * a;
    
    const int r1 = static_cast<int>(std::lround((-b + sqrtD) / denom));
    const int r2 = static_cast<int>(std::lround((-b - sqrtD) / denom));
    
    if (r1 <= r2) {
        return {r1, r2};
    }
    return {r2, r1};
}
// The solution uses the quadratic formula: `r1, r2 = (-b ± sqrt(b² - 4ac)) / (2a)`. The key steps are:  
// 1. Compute the discriminant `D = b*b - 4*a*c`.  
// 2. If `D < 0`, there are no real roots, so return `{0, 0}`.  
// 3. Otherwise, compute the roots as `double` values using `std::sqrt` and `2.0 * a` in the denominator (important to avoid integer division).  
// 4. Round each root to the nearest integer using `std::lround` (or manually, since roots are integers if the input is integer and discriminant is a perfect square). Then sort the two integers so that the smaller is first.  
// 5. Edge cases: `D == 0` gives both roots equal; the sorting still works. The sentinel `{0,0}` is ambiguous if the actual roots are `0` and `0`, but since the task specifies this as the sentinel for "no real roots", accept that limitation.  
// Time complexity is O(1) and space complexity is O(1), as only a fixed number of scalar operations are performed.
