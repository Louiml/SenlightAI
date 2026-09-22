/*
Write a C++ function `int findMissingNumber(const std::vector<int>& nums)` that takes a vector containing `n` distinct integers in the range `[0, n]`, exactly one number from that range is missing, and returns that missing number. The input vector is guaranteed to contain exactly `n` elements (where `n` is the size of the vector), all distinct, and each element is in `[0, n]`. The function must work for any `n ≥ 0` (including an empty vector, in which case `0` is missing). Implement the solution using bitwise XOR only, without sorting, using extra arrays, or using arithmetic sum formulas (though those approaches are valid, this task specifically tests the XOR technique).
*/
#include <vector>

// Return the missing number from the range [0, n] given a vector of size n
// containing all other distinct numbers from that range.
int findMissingNumber(const std::vector<int>& nums) {
    int n = static_cast<int>(nums.size());
    int ans = n; // Start with n, which is part of the expected set
    for (int i = 0; i < n; ++i) {
        ans ^= (i ^ nums[i]); // XOR with expected i and actual nums[i]
    }
    return ans;
}
#include <cassert>
#include <vector>

// (The solution function definition goes here or is included above)

int main() {
    // Basic case: missing 2 from [0,1,2,3,4] where n=4, vector has 0,1,3,4
    assert(findMissingNumber({0, 1, 3, 4}) == 2);

    // Missing 0
    assert(findMissingNumber({1, 2, 3}) == 0);

    // Missing the largest (n)
    assert(findMissingNumber({0, 1, 2, 3}) == 4);

    // Single element: missing 1 when vector is {0}
    assert(findMissingNumber({0}) == 1);

    // Single element: missing 0 when vector is {1} (n=1, range [0,1])
    assert(findMissingNumber({1}) == 0);

    // Empty vector: n=0, range [0,0], missing 0
    assert(findMissingNumber({}) == 0);

    // Larger case: n=5, missing 4, vector has 0,1,2,3,5
    assert(findMissingNumber({0, 1, 2, 3, 5}) == 4);

    // Larger case: missing middle value, n=6, missing 2, vector shuffled
    assert(findMissingNumber({6, 0, 5, 1, 3, 4}) == 2);

    // Missing 1, n=5, vector has 0,2,3,4,5
    assert(findMissingNumber({0, 2, 3, 4, 5}) == 1);

    // Missing 0, larger shuffled case
    assert(findMissingNumber({5, 4, 3, 2, 1}) == 0);

    return 0;
}
// The key insight is that XORing a number with itself yields 0, and XORing with 0 leaves the number unchanged. Since we know the complete set of numbers from `0` to `n` should all appear exactly once, we can XOR all of them together along with all the values present in the input vector. Every number that appears in both sets will cancel out (XOR twice = 0), leaving only the missing number. We start by setting `ans = n` (thus including `n` in the XOR set), then loop `i` from `0` to `n-1`, and for each `i` we XOR `ans` with both `i` and `nums[i]`. This effectively XORs all numbers from `0` to `n` (through the `i` values and the initial `n`) and all numbers present in `nums`. Because every present number appears twice (once from `i` pattern, once from `nums[i]`), they cancel, leaving the missing number. Edge cases: when `n = 0`, the loop runs zero times and `ans` starts as `0`, returning `0`, which is correct because the only missing number from `[0,0]` is `0`. The algorithm runs in O(n) time and uses O(1) auxiliary space. No overflow concerns because XOR operates on bits and the numbers are within `int` range.
