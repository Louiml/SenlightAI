/*
Write a C++ function `classifyNumber(ll N)` that takes a positive integer and returns a string `"Deficient"`, `"Perfect"`, or `"Abundant"` based on the sum of its proper divisors (all positive divisors excluding the number itself). For a number `N`, if the sum of proper divisors is less than `N`, the number is *deficient*; if greater, it is *abundant*; if equal, it is *perfect*. The special case `N = 1` must be treated as deficient (since its only proper divisor sum is 0). The function should handle `N` up to `10^18` efficiently.
*/
#include <string>
#include <cmath>

using int64 = long long int;

// Classify a positive integer as Deficient, Perfect, or Abundant
// based on the sum of its proper divisors (excluding itself).
std::string classifyNumber(int64 N) {
    if (N == 1) {
        return "Deficient";
    }

    int64 sum = 1;  // 1 is always a proper divisor for N > 1
    int64 limit = static_cast<int64>(std::sqrt(static_cast<double>(N)));

    for (int64 d = 2; d <= limit; ++d) {
        if (N % d == 0) {
            sum += d;
            int64 other = N / d;
            if (other != d) {
                sum += other;
            }
        }
    }

    if (sum < N) {
        return "Deficient";
    } else if (sum > N) {
        return "Abundant";
    } else {
        return "Perfect";
    }
}
#include <cassert>
#include <string>

using int64 = long long int;
std::string classifyNumber(int64 N);

int main() {
    assert(classifyNumber(1) == "Deficient");
    assert(classifyNumber(2) == "Deficient");
    assert(classifyNumber(6) == "Perfect");
    assert(classifyNumber(28) == "Perfect");
    assert(classifyNumber(12) == "Abundant");
    assert(classifyNumber(24) == "Abundant");
    assert(classifyNumber(100) == "Abundant");
    assert(classifyNumber(1000000000000000000LL) == "Abundant");
    assert(classifyNumber(999999999999999989LL) == "Deficient");
    return 0;
}
// The core algorithm computes the sum of all proper divisors of `N` by iterating from `2` to `sqrt(N)`. For each divisor `d` found, add `d` to the sum, and if `N/d` is different from `d`, also add `N/d`. After the loop, add `1` to the sum (since `1` is a proper divisor for any `N > 1`). For `N = 1`, the sum of proper divisors is `0` (not `1`), so handle that case by returning `"Deficient"` directly. After computing the sum, compare it with `N`: if sum `< N` → *Deficient*, if sum `> N` → *Abundant*, otherwise *Perfect*. Edge cases: `N = 2` → sum = 1, deficient; `N = 6` → sum = 1+2+3 = 6, perfect; `N = 12` → sum = 1+2+3+4+6 = 16, abundant. Time complexity is `O(sqrt(N))`, and space complexity is `O(1)`.
