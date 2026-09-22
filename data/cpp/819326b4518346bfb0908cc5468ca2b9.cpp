// Given a positive integer `n` (1 ≤ n ≤ 10^9), write a C++ function that returns a `std::string` containing two integers `a` and `b` such that `a + b = n`, `1 ≤ a ≤ b`, and the difference `b - a` is as small as possible. If multiple such pairs exist, choose the one with the smallest `a`. The output string must have the format `"a b"` with a single space, and both numbers must be represented as plain integers without leading zeros. For example, for `n = 10`, the valid pairs are (1,9), (2,8), (3,7), (4,6), (5,5); the one with minimal difference is (5,5). For `n = 9`, valid pairs include (1,8), (2,7), (3,6), (4,5); minimal difference is 1, and among those, the smallest `a` is 4, so the output is `"4 5"`. The function must handle large `n` efficiently and must not rely on any input/output operations inside the function.
#include <cassert>
#include <string>

// The solution function (declared here for completeness, but in the test we include the actual implementation above)
std::string minimalPair(long long n);

int main() {
    assert(minimalPair(2) == "1 1");
    assert(minimalPair(3) == "1 2");
    assert(minimalPair(4) == "2 2");
    assert(minimalPair(5) == "2 3");
    assert(minimalPair(10) == "5 5");
    assert(minimalPair(9) == "4 5");
    assert(minimalPair(1) == "0 1"); // fallback for edge case, but task requires n >= 2
    assert(minimalPair(1000000000LL) == "500000000 500000000");
    assert(minimalPair(999999999LL) == "499999999 500000000");
    assert(minimalPair(7) == "3 4");
    return 0;
}
#include <string>

// Return a string "a b" where a + b = n, 1 ≤ a ≤ b, and b - a is minimal.
// If multiple pairs have the same minimal difference, choose the smallest a.
std::string minimalPair(long long n) {
    // For n >= 2, a = n / 2 (floor division) and b = n - a.
    // This maximizes a under the constraint a <= n/2, minimizing difference.
    long long a = n / 2;
    long long b = n - a;
    return std::to_string(a) + " " + std::to_string(b);
}
// The goal is to split `n` into two positive integers `a` and `b` with `a ≤ b` and `a + b = n` such that the difference `b - a` is minimized. Since `b = n - a`, the difference is `(n - a) - a = n - 2a`. To minimize this difference (which is non-negative because `a ≤ n/2`), we need to maximize `a` under the constraint `a ≤ n/2`. Thus the optimal value of `a` is `floor(n/2)`. Then `b = n - a` = `ceil(n/2)`. If `n` is even, then `a = b = n/2`, and the difference is 0. If `n` is odd, then `a = (n-1)/2` and `b = (n+1)/2`, giving difference 1. This pair automatically satisfies `a ≤ b` and `a ≥ 1` for all `n ≥ 2`. For `n = 1`, the only valid pair is (1,0) but since `b` must be positive, such `n` is invalid according to constraints (but if it were given, the function can still return "1 0" as a fallback, though the task limits to `n ≥ 2`). Edge cases: `n = 2` gives "1 1", `n = 3` gives "1 2", `n = 4` gives "2 2". The algorithm is O(1) time and O(1) space (excluding the returned string). No loops or recursion needed. The solution simply computes `a = n/2` (integer division) and `b = n - a`, then concatenates them as strings with a space.
