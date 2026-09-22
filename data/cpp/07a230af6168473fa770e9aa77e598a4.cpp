// Write a C++ function named `productExceptSelf` that takes a vector of integers `nums` (with at least two elements) and returns a vector of integers where each element at index `i` is the product of all elements in `nums` except `nums[i]`. The function must not use division and must run in O(n) time. Additionally, implement it with O(1) extra space (the output vector does not count as extra space). Handle edge cases such as vectors containing zeros and negative numbers. The function should be `const`-correct and accept the input vector by `const` reference.
#include <cassert>
#include <vector>

// The solution function is assumed to be defined above.
// This test verifies correctness for multiple cases.
int main() {
    std::vector<int> nums1 = {1, 2, 3, 4};
    std::vector<int> expected1 = {24, 12, 8, 6};
    assert(productExceptSelf(nums1) == expected1);

    std::vector<int> nums2 = {2, 3, 4, 5};
    std::vector<int> expected2 = {60, 40, 30, 24};
    assert(productExceptSelf(nums2) == expected2);

    std::vector<int> nums3 = {0, 1, 2, 3};
    std::vector<int> expected3 = {6, 0, 0, 0};
    assert(productExceptSelf(nums3) == expected3);

    std::vector<int> nums4 = {0, 0, 5};
    std::vector<int> expected4 = {0, 0, 0};
    assert(productExceptSelf(nums4) == expected4);

    std::vector<int> nums5 = {-1, -2, -3};
    std::vector<int> expected5 = {6, 3, 2};
    assert(productExceptSelf(nums5) == expected5);

    std::vector<int> nums6 = {5, 5};
    std::vector<int> expected6 = {5, 5};
    assert(productExceptSelf(nums6) == expected6);

    std::vector<int> nums7 = {1, 2};
    std::vector<int> expected7 = {2, 1};
    assert(productExceptSelf(nums7) == expected7);

    std::vector<int> nums8 = {10, 3, 5, 2};
    std::vector<int> expected8 = {30, 100, 60, 150};
    assert(productExceptSelf(nums8) == expected8);

    return 0;
}
#include <vector>

// Returns a vector where output[i] = product of all nums[j] for j != i.
// Does not use division, runs in O(n), uses O(1) extra space (output vector excluded).
std::vector<int> productExceptSelf(const std::vector<int>& nums) {
    int len = static_cast<int>(nums.size());
    std::vector<int> result(len, 1);

    // Left pass: result[i] = product of nums[0] ... nums[i-1]
    for (int i = 1; i < len; ++i) {
        result[i] = result[i - 1] * nums[i - 1];
    }

    // Right pass: multiply by product of nums[i+1] ... nums[len-1]
    int rightProduct = 1;
    for (int i = len - 2; i >= 0; --i) {
        rightProduct *= nums[i + 1];
        result[i] *= rightProduct;
    }

    return result;
}
// The main algorithm uses two passes. In the first pass (left-to-right), we build a prefix product for each position: for each index `i`, we store the product of all elements to its left (i.e., `result[i] = product(nums[0] ... nums[i-1])`). This is done by initializing `result[0] = 1` and then for each subsequent index, multiplying the previous result by the previous element. In the second pass (right-to-left), we maintain a running product of all elements to the right of the current index (initially 1), and multiply it into `result[i]`. This gives the final product of all elements except the current one. Edge cases include arrays with zeros: if there are two or more zeros, most outputs become zero, but the algorithm still works correctly because multiplication by zero propagates correctly. Negative numbers are handled naturally. Time complexity is O(n) with two linear passes, and space complexity is O(1) extra (excluding the output vector).
