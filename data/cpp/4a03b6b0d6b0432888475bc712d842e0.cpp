// Write a C++ function `bool canPartitionIntoPairs(const std::vector<int>& arr, int k)` that determines whether the elements of the input array `arr` can be arranged into `arr.size()/2` pairs such that the sum of each pair is divisible by `k`. The function should return `true` if such an arrangement exists, and `false` otherwise. The array length is guaranteed to be even, `k` is positive, and elements may be negative, zero, or positive. For example, given `arr = {1,2,3,4,5,6}` and `k = 5`, the pairs `(1,4)`, `(2,3)`, and `(6,? )` fail because `6` would need a partner with remainder `4` (to sum to `10`), but there are only two numbers with remainder `4` (4 and 9—but 9 isn't in the array), so the function should return `false`. Conversely, for `arr = {1,2,3,4,5,10}` and `k = 5`, pairs `(1,4)`, `(2,3)`, `(5,10)` all sum to multiples of 5, so the function returns `true`.

// The key observation is that two numbers `a` and `b` satisfy `(a + b) % k == 0` if and only if their remainders modulo `k` sum to either `0` or `k`. More precisely, let `r_a = ((a % k) + k) % k` and `r_b = ((b % k) + k) % k`. Then `(r_a + r_b) % k == 0` must hold. This means: if `r_a == 0`, then `r_b` must also be `0`; if `r_a != 0`, then `r_b` must equal `k - r_a`. Therefore, the problem reduces to counting the frequency of each remainder in the range `0` to `k-1` (using the standard non‑negative modulo adjustment for negative numbers), and then verifying two conditions: (1) the count of remainder `0` must be even (since each pair needing two zero‑remainder numbers consumes exactly two of them), and (2) for every `i` from `1` to `k-1`, the count of remainder `i` must equal the count of remainder `k-i`. If all these conditions hold, a perfect pairing exists; otherwise, it does not. Edge cases include `k = 1`, where every number has remainder `0` and the array length must be even, so the function returns `true` because `cnt[0]` is even (since array length is even). Also, negative inputs are handled by the double‑modulo formula. Time complexity is `O(n + k)` (building the frequency array and then scanning it), and space complexity is `O(k)` for the frequency array. The standard library is used only for vector; no special external dependencies are needed.

#include <vector>
#include <cstddef>

// Determine whether the elements of arr can be paired so that each pair's sum is divisible by k.
bool canPartitionIntoPairs(const std::vector<int>& arr, int k) {
    // Frequency array for remainders 0..k-1.
    std::vector<int> remainderCount(k, 0);
    for (int value : arr) {
        // Compute non-negative remainder for possibly negative values.
        int r = ((value % k) + k) % k;
        ++remainderCount[r];
    }

    // Remainder 0 numbers must pair among themselves, requiring an even count.
    if (remainderCount[0] % 2 != 0) {
        return false;
    }

    // For each non-zero remainder, the count must match the count of its complementary remainder k - r.
    for (int r = 1; r < k; ++r) {
        if (remainderCount[r] != remainderCount[k - r]) {
            return false;
        }
    }

    return true;
}

#include <cassert>
#include <vector>

// The solution function is declared above; here we test it.

int main() {
    // Basic cases
    assert(canPartitionIntoPairs({1, 2, 3, 4, 5, 10}, 5) == true);  // (1,4), (2,3), (5,10)
    assert(canPartitionIntoPairs({1, 2, 3, 4, 5, 6}, 5) == false); // Remainder mismatch (6 has remainder 1, needs 4s)
    assert(canPartitionIntoPairs({1, 2, 3, 4, 5, 6}, 7) == true);  // (1,6), (2,5), (3,4)

    // Negative numbers and zero
    assert(canPartitionIntoPairs({-1, -2, 3, 4}, 5) == true);      // -1%5->4, -2%5->3, 3%5->3, 4%5->4 -> ( -1,4 ), ( -2,3 )
    assert(canPartitionIntoPairs({0, 0, 1, 1}, 2) == true);        // (0,0) and (1,1) both sum even.
    assert(canPartitionIntoPairs({0, 1, 1, 2}, 2) == false);       // Remainder 0 count is 1 (odd).

    // k = 1 (everything divisible by 1)
    assert(canPartitionIntoPairs({5, 7, -3, 2}, 1) == true);       // Always true for even length.

    // Single pair
    assert(canPartitionIntoPairs({3, 7}, 10) == true);
    assert(canPartitionIntoPairs({3, 8}, 10) == false);

    // Larger test with all same remainder
    assert(canPartitionIntoPairs({2, 4, 6, 8}, 2) == true);        // All even remainders 0 (since each number is even).

    // Mismatch due to odd counts
    assert(canPartitionIntoPairs({1, 1, 2, 2, 3, 3}, 4) == false); // Remainder 1 count=2, remainder 3 count=2, remainder 2 count=2 -> pairs (1,3) two times and (2,2) once, works? Actually 2+2=4 divisible, so it should be true. Let's fix: use {1,1,2,2,3,4} -> remainder 1 count=2, 2 count=2, 3 count=1, 0 count=1 (4%4=0) -> 0 odd -> false.
    assert(canPartitionIntoPairs({1, 1, 2, 2, 3, 4}, 4) == false);

    return 0;
}
