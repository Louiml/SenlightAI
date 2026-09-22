Write a C++ function `nextGreaterElements(const std::vector<int>& nums1, const std::vector<int>& nums2)` that, given two arrays `nums1` (a subset of `nums2`) and `nums2` (both without duplicates), returns a vector where for each element in `nums1`, you find the first greater number to its right in `nums2`. If no such greater number exists, use `-1`. The function must operate efficiently for large inputs, and should be `const`-correct, taking both inputs by `const` reference. Assume `nums1` contains only elements present in `nums2`. The function should not modify the input arrays.

The optimal solution uses a monotonic decreasing stack to preprocess `nums2` in a single right-to-left pass. For each element `x` as we iterate from right to left, we pop from the stack any values smaller than `x`, because those popped values can never be the "next greater" for any element to the left of `x` (since `x` is larger and closer). After popping, the top of the stack (if any) is the next greater element for `x`; otherwise, it’s `-1`. Store this mapping in an `unordered_map` from value to its next greater. Then, for each element in `nums1`, look it up in the map. Edge cases: the last element in `nums2` always has `-1`; if `nums2` is empty, the result is an empty vector; if `nums1` is empty, result is empty. Time complexity is O(|nums2| + |nums1|) on average, since each element is pushed and popped at most once. Space complexity is O(|nums2|) for the map and stack.

#include <vector>
#include <unordered_map>
#include <stack>

// For each element in nums1, returns the first greater element to its right in nums2,
// or -1 if none exists. nums1 must be a subset of nums2, and both arrays have no duplicates.
std::vector<int> nextGreaterElements(const std::vector<int>& nums1, const std::vector<int>& nums2) {
    std::unordered_map<int, int> nextGreater;  // value -> next greater in nums2
    std::stack<int> st;  // monotonic decreasing stack (values)

    // Traverse nums2 from right to left
    for (auto it = nums2.rbegin(); it != nums2.rend(); ++it) {
        int current = *it;
        // Pop all smaller elements, they cannot be next greater for anything left
        while (!st.empty() && st.top() < current) {
            st.pop();
        }
        // Top of stack (if any) is the next greater for current
        nextGreater[current] = st.empty() ? -1 : st.top();
        st.push(current);
    }

    // Build result for nums1
    std::vector<int> result;
    result.reserve(nums1.size());
    for (int value : nums1) {
        result.push_back(nextGreater[value]);
    }
    return result;
}

#include <cassert>
#include <vector>

// Assume nextGreaterElements function is already defined above.

int main() {
    // Example 1 from problem
    std::vector<int> nums1 = {4, 1, 2};
    std::vector<int> nums2 = {1, 3, 4, 2};
    std::vector<int> expected = {-1, 3, -1};
    assert(nextGreaterElements(nums1, nums2) == expected);

    // Example 2
    nums1 = {2, 4};
    nums2 = {1, 2, 3, 4};
    expected = {3, -1};
    assert(nextGreaterElements(nums1, nums2) == expected);

    // Single element in nums2
    nums1 = {5};
    nums2 = {5};
    expected = {-1};
    assert(nextGreaterElements(nums1, nums2) == expected);

    // All decreasing sequence in nums2
    nums1 = {9, 7, 5};
    nums2 = {9, 7, 5};
    expected = {-1, -1, -1};
    assert(nextGreaterElements(nums1, nums2) == expected);

    // Strictly increasing sequence
    nums1 = {1, 2, 3};
    nums2 = {1, 2, 3};
    expected = {2, 3, -1};
    assert(nextGreaterElements(nums1, nums2) == expected);

    // More complex case with interleaved values
    nums1 = {6, 10};
    nums2 = {10, 6, 8, 12};
    // For 6: next greater is 8; for 10: next greater is 12
    expected = {8, 12};
    assert(nextGreaterElements(nums1, nums2) == expected);

    // Empty nums1
    nums1 = {};
    nums2 = {1, 2, 3};
    expected = {};
    assert(nextGreaterElements(nums1, nums2) == expected);

    // Empty nums2 (but nums1 must also be empty per subset requirement)
    nums1 = {};
    nums2 = {};
    expected = {};
    assert(nextGreaterElements(nums1, nums2) == expected);

    return 0;
}
