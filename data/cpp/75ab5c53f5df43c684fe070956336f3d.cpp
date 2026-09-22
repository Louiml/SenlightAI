Write a C++ function `productExceptSelf` that takes a non-empty vector of integers (which may include zeros, negative numbers, and duplicates) and returns a new vector of the same length where each element `result[i]` is the product of all elements in the input except the one at index `i`. The solution must run in linear time `O(n)` and use constant extra space `O(1)` (excluding the output vector). The function must handle cases with one or more zeros correctly (e.g., if there are two or more zeros, all outputs are zero; if exactly one zero, only that index is non-zero). Do not use division, and ensure the function is robust for large integer values (within the range of `int`).

#include <cassert>
#include <vector>

// Function declaration (prototype)
std::vector<int> productExceptSelf(const std::vector<int>& nums);

int main() {
    // Basic example from problem statement
    assert(productExceptSelf({1, 2, 3, 4}) == std::vector<int>({24, 12, 8, 6}));

    // Single element
    assert(productExceptSelf({5}) == std::vector<int>({1}));

    // Two elements
    assert(productExceptSelf({3, 7}) == std::vector<int>({7, 3}));

    // Contains one zero
    assert(productExceptSelf({1, 0, 3}) == std::vector<int>({0, 3, 0}));

    // Contains two zeros -> all results zero
    assert(productExceptSelf({0, 2, 0}) == std::vector<int>({0, 0, 0}));

    // Negative numbers
    assert(productExceptSelf({-1, -2, -3}) == std::vector<int>({6, -3, -2}));

    // All same values
    assert(productExceptSelf({2, 2, 2}) == std::vector<int>({4, 4, 4}));

    // Mixed positives, negatives, and zero
    assert(productExceptSelf({-1, 2, 0, 4}) == std::vector<int>({0, 0, -8, 0}));

    // Larger sequence
    assert(productExceptSelf({1, 2, 3, 4, 5}) == std::vector<int>({120, 60, 40, 30, 24}));

    // Duplicates including zero
    assert(productExceptSelf({0, 0, 0}) == std::vector<int>({0, 0, 0}));

    return 0;
}

#include <vector>
#include <cstddef>

// Compute product of all elements except self for each index.
// Time: O(n), Space: O(1) extra (output vector not counted).
std::vector<int> productExceptSelf(const std::vector<int>& nums) {
    std::size_t n = nums.size();
    std::vector<int> ans(n, 1);

    // First pass: prefix products from left to right.
    for (std::size_t i = 1; i < n; ++i) {
        ans[i] = ans[i - 1] * nums[i - 1];
    }

    // Second pass: multiply by suffix products from right to left.
    int suffix = 1;
    for (std::size_t i = n - 1; i-- > 0; ) { // careful unsigned underflow
        suffix *= nums[i + 1];
        ans[i] *= suffix;
    }

    return ans;
}
Note: The loop `for (std::size_t i = n - 1; i-- > 0; )` is safe because `i--` evaluates to the old value and then decrements; when `i` becomes 0, the condition `i-- > 0` is false (0 is not > 0), and the loop exits without entering with `i` underflowing. Alternatively, use a signed integer loop for clarity.

// The core idea is to build the result in two passes using prefix and suffix products without storing separate arrays. In the first pass, iterate from left to right, storing in `ans[i]` the product of all elements to the left of `i` (prefix product). Initially set `ans[0] = 1` (since there are no elements left of index 0). For each subsequent index `i`, multiply `ans[i]` by `ans[i-1] * nums[i-1]`. After this pass, `ans[i]` contains the product of all elements before `i`. In the second pass, iterate from right to left, maintaining a running suffix product (start with `suffix = 1`). For each index `i` from `n-2` down to 0, multiply `suffix` by `nums[i+1]` to get the product of all elements to the right of `i`, then multiply `ans[i]` by this suffix value. After both passes, `ans[i]` contains the product of all elements except `nums[i]`. Edge cases: if the input contains a zero, the prefix/suffix approach naturally handles it — if there are at least two zeros, every `ans[i]` will include at least one zero factor and thus be 0; if exactly one zero at index `z`, then for `i != z`, `ans[i]` will be 0, and for `i == z`, `ans[i]` will be the product of all non-zero elements. The algorithm uses `O(n)` time because it makes two linear passes, and `O(1)` extra space (only the output vector is allocated). For each element, the multiplication of potentially large integers stays within `int` as guaranteed by the problem statement.
