// Write a C++ function `largestSpecialNumber` that, given an unsigned 64-bit integer `N`, returns the largest "special" number not exceeding `N`. A special number is defined as a non-negative integer whose decimal representation consists only of repeated copies of a single digit (e.g., 0, 1, 2, ..., 9, 11, 22, 33, ..., 99, 111, 222, ..., 1111, 2222, ...). Note that 0 is special, and numbers like 10, 21, or 123 are not special. The input `N` may be up to 100,000,000,000,000,000 (10^17). If `N` is less than 0 (not possible with unsigned), handle gracefully anyway. Return the largest special number ≤ N. For example, if N = 120, the largest special number is 99; for N = 3, it's 3; for N = 0, it's 0; for N = 10, it's 9. Ensure the function is `const`-correct and efficient.
// The key observation is that all special numbers can be generated systematically: for each digit `d` from 0 to 9, and for each length `len` from 1 upward, the number consisting of `len` copies of digit `d` (like `d*111...1`) is special. The largest possible length is 17 because N can be as large as 10^17 (which is 100,000,000,000,000,000, a 18-digit number, but we only need to consider numbers up to N, and any number with more than 17 digits will exceed the maximum possible N, so we cap the length to 17 to avoid overflow; actually 10^17 is 1 followed by 17 zeros, which is 18 digits, but the largest special number with 17 digits is 999...9 (17 times) which is less than 10^17, so length 17 is sufficient). We can generate all such numbers up to a safe bound (e.g., `ULLONG_MAX` or just 10^18) and store them in a sorted set. Then, for a query N, we find the greatest element less than or equal to N using `lower_bound` or `upper_bound` with decrement. The set size is at most 10 * 17 = 170 numbers, so it's tiny. Alternatively, we can avoid storing all and directly search, but generating all is simpler and O(1) per query. Edge cases: N=0 returns 0; N less than the smallest positive special number (which is 1) but N is non-negative, so N=0 is handled; ensure no overflow when building numbers like 99999999999999999 (17 times) which fits in `unsigned long long` (max ~1.8e19). Time complexity: preprocessing O(170) to generate, each query O(log 170) = O(1). Space O(170). The solution function can generate the set once using a static local set, or we can generate on the fly per call, but since it's a standalone function, generating static is fine.
#include <set>
#include <cstdint>

using ULL = unsigned long long;

// Return the largest special number (repeated single digit) not exceeding N.
ULL largestSpecialNumber(ULL N) {
    static const std::set<ULL> special = []() {
        std::set<ULL> s;
        // For each digit 0..9
        for (ULL d = 0; d <= 9; ++d) {
            ULL cur = d; // handle single-digit numbers (including 0)
            s.insert(cur);
            // Build longer numbers of repeated digit d
            for (int len = 1; len <= 17; ++len) {
                // Prevent overflow: if cur > (max - digit)/10, stop
                if (cur > (ULLONG_MAX - d) / 10) break;
                cur = cur * 10 + d;
                if (cur <= 100000000000000000ULL) { // safe bound
                    s.insert(cur);
                }
            }
        }
        return s;
    }();
    
    // Find the largest special number <= N
    auto it = special.upper_bound(N);
    if (it == special.begin()) {
        // N is smaller than the smallest special number (0), but N is unsigned, so only N=0 possible
        return 0;
    }
    --it;
    return *it;
}
#include <cassert>

int main() {
    assert(largestSpecialNumber(0) == 0);
    assert(largestSpecialNumber(1) == 1);
    assert(largestSpecialNumber(9) == 9);
    assert(largestSpecialNumber(10) == 9);
    assert(largestSpecialNumber(11) == 11);
    assert(largestSpecialNumber(99) == 99);
    assert(largestSpecialNumber(100) == 99);
    assert(largestSpecialNumber(120) == 99);
    assert(largestSpecialNumber(111) == 111);
    assert(largestSpecialNumber(2222) == 2222);
    assert(largestSpecialNumber(2223) == 2222);
    assert(largestSpecialNumber(100000000000000000ULL) == 99999999999999999ULL);
    assert(largestSpecialNumber(123456789) == 99999999);
    return 0;
}
