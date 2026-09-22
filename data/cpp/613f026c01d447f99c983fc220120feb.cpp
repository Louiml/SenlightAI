Write a C++ function `getNthUglyNumber(int n)` that returns the n-th ugly number as an `unsigned long long`. Ugly numbers are positive integers whose only prime factors are 2, 3, or 5. By convention, 1 is considered ugly. The function should handle `n` from 1 to at least 1500. The input `n` is guaranteed to be positive. The function must be efficient enough for up to 1500 queries, but for a single call it simply needs to return the correct value. Edge cases: if `n == 1`, return 1. The result may exceed 32-bit range, so use a 64-bit unsigned type. Do not produce any output or read input inside the function; just compute and return the result.

// The classic approach to generate ugly numbers in sorted order uses a dynamic programming (or merge-like) method. Maintain an array `ugly[0..n-1]` where `ugly[0] = 1`. Use three indices `i2`, `i3`, `i5` that point to the next ugly number to multiply by 2, 3, and 5 respectively. At each step, compute the next candidate as `min(ugly[i2]*2, ugly[i3]*3, ugly[i5]*5)`. Add this candidate to the array. Then increment each index whose multiplication result equals the chosen candidate (this handles duplicates like 6 = 2*3 and 3*2). Continue until `n` numbers are generated. Since the sequence is sorted and each ugly number is generated exactly once, this works correctly. Edge cases: `n=1` returns 1; when duplicates occur, multiple indices are incremented to avoid reusing the same product. Time complexity: O(n) because each index moves at most n steps. Space complexity: O(n) for the array. The result may be large; for n=1500 it fits in `unsigned long long` (the 1500th ugly number is within 64-bit range). Use `std::min` with initializer list or separate min comparisons. Ensure no overflow by using `unsigned long long` throughout and multiplying by small constants (2,3,5) — safe because each ugly number is far below the maximum 64-bit value.

#include <vector>
#include <algorithm>

// Return the n-th ugly number (1-indexed), where ugly numbers are positive integers
// with prime factors only 2, 3, or 5, and 1 is considered ugly.
unsigned long long getNthUglyNumber(int n) {
    std::vector<unsigned long long> ugly(n);
    ugly[0] = 1ULL;

    unsigned long long next_multiple_of_2 = 2ULL;
    unsigned long long next_multiple_of_3 = 3ULL;
    unsigned long long next_multiple_of_5 = 5ULL;

    int index2 = 0;
    int index3 = 0;
    int index5 = 0;

    for (int i = 1; i < n; ++i) {
        unsigned long long next_ugly = std::min({next_multiple_of_2,
                                                 next_multiple_of_3,
                                                 next_multiple_of_5});
        ugly[i] = next_ugly;

        if (next_ugly == next_multiple_of_2) {
            ++index2;
            next_multiple_of_2 = ugly[index2] * 2ULL;
        }
        if (next_ugly == next_multiple_of_3) {
            ++index3;
            next_multiple_of_3 = ugly[index3] * 3ULL;
        }
        if (next_ugly == next_multiple_of_5) {
            ++index5;
            next_multiple_of_5 = ugly[index5] * 5ULL;
        }
    }

    return ugly[n - 1];
}

#include <cassert>

int main() {
    assert(getNthUglyNumber(1) == 1ULL);
    assert(getNthUglyNumber(2) == 2ULL);
    assert(getNthUglyNumber(3) == 3ULL);
    assert(getNthUglyNumber(4) == 4ULL);
    assert(getNthUglyNumber(5) == 5ULL);
    assert(getNthUglyNumber(6) == 6ULL);
    assert(getNthUglyNumber(7) == 8ULL);
    assert(getNthUglyNumber(10) == 12ULL);
    assert(getNthUglyNumber(11) == 15ULL);
    assert(getNthUglyNumber(20) == 36ULL);
    return 0;
}
