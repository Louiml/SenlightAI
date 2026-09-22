// Write a C++ function named `alternatingSum` that takes a single positive integer `n` and returns the result of the alternating sum: `1 - 2 + 3 - 4 + 5 - ... ± n`, where odd numbers are added and even numbers are subtracted. The function must compute the result in constant time using a mathematical formula (not a loop), and it must correctly handle the special case of `n = 0` by returning `0`. The function signature is `int alternatingSum(int n)`. You may assume the input is a non-negative integer, but your implementation should still produce correct results for `n = 0` and `n = 1`.
// The alternating sum pattern `1 - 2 + 3 - 4 + ... ± n` can be expressed as `(sum of odd numbers) - (sum of even numbers)`. However, a simpler observation: for any even `n`, the sum is exactly `-n/2` because pairs like `(1-2) + (3-4) + ... + ((n-1)-n)` each equal `-1`, and there are `n/2` such pairs, giving `-n/2`. For odd `n`, the sum is `(-n/2)` plus `n` (the last odd term added), which simplifies to `-n/2 + n`, or equivalently `n - n/2` using integer division. For `n = 0`, the formula `-0/2 = 0` works. For `n = 1`, `-1/2 + 1 = 0 + 1 = 1`. Thus the code checks parity: if even, return `-n/2`; if odd, return `n - n/2` (or `-n/2 + n`). Time complexity is O(1), space complexity is O(1). Edge cases: `n=0` returns 0; small values like `n=1` and `n=2` should match brute-force sums.
// Compute alternating sum: 1 - 2 + 3 - 4 + ... ± n in O(1) time.
int alternatingSum(int n) {
    if (n % 2 == 0) {
        // For even n: pairs (1-2), (3-4), ... each sum to -1, total = -n/2
        return -n / 2;
    } else {
        // For odd n: even result plus last odd term n, i.e., -n/2 + n
        return n - n / 2;
    }
}
#include <cassert>

int main() {
    // Basic cases
    assert(alternatingSum(0) == 0);
    assert(alternatingSum(1) == 1);
    assert(alternatingSum(2) == -1);
    assert(alternatingSum(3) == 2);
    assert(alternatingSum(4) == -2);
    // Larger values
    assert(alternatingSum(5) == 3);
    assert(alternatingSum(10) == -5);
    assert(alternatingSum(11) == 6);
    // Very large even and odd
    assert(alternatingSum(1000000) == -500000);
    assert(alternatingSum(999999) == 500000);
}
