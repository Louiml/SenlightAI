// Write a C++ function `bool canRepresentAsSumOf2020s(int n)` that determines whether a given positive integer `n` can be expressed as the sum of one or more terms, each of which is either 2020 or 2021. (This is equivalent to: can we write `n = 2020 * x + 2021 * y` with integers `x >= 0`, `y >= 0`, and `x + y >= 1`? Note the original snippet checks a specific pattern, but your task is to test the general representability.) Return `true` if such a representation exists, and `false` otherwise. The input is guaranteed to be between 1 and \(10^9\) inclusive. Your function should handle edge cases such as small numbers that cannot be represented, numbers exactly equal to 2020 or 2021, and large numbers that are combinations of many 2020s and 2021s.
#include <cassert>

bool canRepresentAsSumOf2020s(int n); // forward declaration

int main() {
    assert(canRepresentAsSumOf2020s(2020) == true);
    assert(canRepresentAsSumOf2020s(2021) == true);
    assert(canRepresentAsSumOf2020s(2019) == false);
    assert(canRepresentAsSumOf2020s(4040) == true);
    assert(canRepresentAsSumOf2020s(4041) == true);
    assert(canRepresentAsSumOf2020s(4042) == true);
    assert(canRepresentAsSumOf2020s(4043) == false);
    assert(canRepresentAsSumOf2020s(1000000000) == true); // large number
    assert(canRepresentAsSumOf2020s(1) == false);
    assert(canRepresentAsSumOf2020s(6060) == true); // 3*2020
    return 0;
}
#include <cstdint>

// Determines if n can be expressed as a sum of 2020's and 2021's.
// Returns true if such a sum exists, false otherwise.
bool canRepresentAsSumOf2020s(int n) {
    if (n < 2020) {
        return false;
    }
    int terms = n / 2020;          // Maximum possible number of terms (all 2020's)
    int remainder = n % 2020;      // Extra amount to distribute as 1's over terms
    return remainder <= terms;
}
// The key observation is that since 2020 and 2021 are consecutive integers, any sum of them can be written as `2020 * k + t`, where `k` is the total number of terms, and `t` is the number of 2021’s among those `k` terms (so `0 <= t <= k`). Therefore, the condition is: for a given `n`, if we set `k = n / 2020` (integer division), we need that `n % 2020` is at most `k`, because the remainder `n % 2020` must be distributed as extra 1’s over the 2020’s, and each of the `k` terms can contribute at most one extra 1 (turning a 2020 into a 2021). If `k == 0` (i.e., `n < 2020`), then no representation exists because the smallest allowed term is 2020. Also note that if `k >= 1` and remainder `r` satisfies `r <= k`, then we can form `n` as `(k - r)` copies of 2020 and `r` copies of 2021. Edge cases: `n = 2020` → `k = 1, r = 0` → true; `n = 2021` → `k = 1, r = 1` → true; `n = 2019` → `k = 0` → false; `n = 4040` → `k = 2, r = 0` → true; `n = 4041` → `k = 2, r = 1` → true; `n = 4042` → `k = 2, r = 2` → true (2021+2021); `n = 4043` → `k = 2, r = 3` > 2 → false. The algorithm is a single division and modulus, so time complexity is \(O(1)\) and space complexity is \(O(1)\).
