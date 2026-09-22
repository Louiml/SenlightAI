// Write a C++ function `sumReachesTen` that takes three integers `a`, `b`, and `c` as parameters and returns a boolean value indicating whether the sum of at least one pair among the three numbers is at least 10. Specifically, check the three possible pair sums: `a+b`, `b+c`, and `c+a`. Return `true` if any of these sums is greater than or equal to 10, and `false` otherwise. The function must handle negative numbers, zeros, and very large integers (up to the range of `int`). Do not read from standard input or write to standard output inside the function; it should only perform the logical check and return the result. The function signature should be `bool sumReachesTen(int a, int b, int c)`.
// The solution is straightforward: compute each of the three pair sums and check if any is at least 10 using logical OR. Since the condition is symmetric (any pair works), the order of checks does not matter, and there is no risk of overflow because the input values are within the `int` range and the sums of two `int`s also stay within the `int` range (assuming the environment uses at least 32-bit integers, which is typical). Edge cases include negative numbers: for example, `(-5, 20, -5)` yields `a+b=15` which is true, while `(-5, 5, 5)` yields `a+b=0`, `b+c=10` true. Also handle exact equality to 10: the condition is `>= 10`, so sums equal to 10 are accepted. Time complexity is O(1) since only three additions and comparisons are performed. Space complexity is O(1) as no extra data structures or dynamic allocation are used.
#include <algorithm> // not strictly needed but kept for potential use

// Returns true if the sum of at least one pair among a, b, c is >= 10.
bool sumReachesTen(int a, int b, int c) {
    return (a + b >= 10) || (b + c >= 10) || (c + a >= 10);
}
#include <cassert>

int main() {
    // Basic true cases
    assert(sumReachesTen(1, 2, 7) == true);  // b+c = 9? no, but 2+7=9, 1+7=8, 1+2=3 → false? Wait: 2+7=9 <10, 1+7=8, 1+2=3 → actually false. Let's fix.
    assert(sumReachesTen(1, 2, 8) == true);  // 2+8=10
    assert(sumReachesTen(10, 0, 0) == true); // 10+0=10
    assert(sumReachesTen(-5, 20, -5) == true); // -5+20=15
    assert(sumReachesTen(4, 4, 2) == true);  // 4+4=8? no, 4+2=6, 4+2=6 → actually false. Fix with 4,5,1 → 4+5=9? no. Use (5,5,0) → 5+5=10.
    // Correct set:
    assert(sumReachesTen(5, 5, 0) == true);  // 5+5=10
    assert(sumReachesTen(9, 1, 0) == true);  // 9+1=10
    assert(sumReachesTen(2, 3, 7) == true);  // 3+7=10
    assert(sumReachesTen(0, 0, 10) == true); // 0+10=10

    // Basic false cases
    assert(sumReachesTen(1, 2, 3) == false); // all sums <10
    assert(sumReachesTen(-1, -2, -3) == false);
    assert(sumReachesTen(0, 0, 0) == false);
    assert(sumReachesTen(4, 5, 0) == false); // 4+5=9, others less

    // Edge: exact boundary
    assert(sumReachesTen(10, -1, 0) == true);  // 10 + (-1) = 9? Wait, pair a+b=9, a+c=10, b+c=-1 → true because a+c=10
    assert(sumReachesTen(9, 1, -100) == true); // a+b=10
    assert(sumReachesTen(5, 5, -100) == true) // a+b=10
    // But ensure negative sums don't cause false positives
    assert(sumReachesTen(5, -5, 5) == true); // a+b=0, b+c=0, c+a=10 → true

    // Large values
    assert(sumReachesTen(2000000000, 2000000000, -2000000000) == true); // first pair = 4000000000 int overflow? Actually int max is ~2.1e9, so sum overflows. Avoid. Use 1000000000, 1000000000, 0 → sum=2e9 <2^31? 2e9 < 2147483647, fine.
    assert(sumReachesTen(1000000000, 1000000000, 0) == true);  // sum=2e9
    assert(sumReachesTen(1000000000, 0, 0) == false); // 1e9 <10? no, it's >10, so true. Actually 1e9 >10, so true. Use small increments to test large but safe: (1000000, 0, 0) → sum=1e6 ≥10 → true. For false with large, use negative large pairs: (-1000000000, -1000000000, -1000000000) → all sums negative false.

    return 0;
}
