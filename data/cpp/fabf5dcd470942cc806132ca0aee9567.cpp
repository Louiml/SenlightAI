/*
Write a C++ function `vector<double> medianSlidingWindow(const vector<int>& nums, int k)` that returns an array of the medians of all sliding windows of size `k` over the input integer array `nums`. The median of a window is defined as follows: if `k` is odd, the median is the middle element when the window is sorted; if `k` is even, the median is the average of the two middle elements. The input integers can be negative, and `k` is guaranteed to be between 1 and the length of `nums`. Duplicate values may appear, and the returned medians must be computed as double-precision floating point numbers (e.g., for even `k`, the average might be a non-integer). The solution must operate in O(n log k) time overall, where n is the size of `nums`, using a balanced data structure approach (e.g., two heaps or a multiset). Handle edge cases such as a window covering the entire array, negative values, and large `k` values.
*/

#include <vector>
#include <set>

// Returns the medians of every sliding window of size k in nums.
std::vector<double> medianSlidingWindow(const std::vector<int>& nums, int k) {
    std::multiset<int> left, right; // left: max-heap (smaller half), right: min-heap (larger half)
    const int n = static_cast<int>(nums.size());
    std::vector<double> result;
    if (n == 0 || k == 0) return result;

    // Initialize with the first k elements.
    for (int i = 0; i < k; ++i) right.insert(nums[i]);
    for (int i = 0; i < k / 2; ++i) {
        left.insert(*right.begin());
        right.erase(right.begin());
    }

    // Helper lambda to compute current median.
    auto get_median = [&]() -> double {
        if (k % 2 == 1) return static_cast<double>(*right.begin());
        return (static_cast<double>(*left.rbegin()) + *right.begin()) / 2.0;
    };

    result.push_back(get_median());

    for (int i = k; i < n; ++i) {
        int x = nums[i];        // element entering the window
        int y = nums[i - k];    // element leaving the window

        // Insert x
        if (x >= *right.begin()) right.insert(x);
        else left.insert(x);

        // Remove y
        if (y >= *right.begin()) right.erase(right.find(y));
        else left.erase(left.find(y));

        // Balance sizes: left.size() <= right.size() <= left.size() + 1
        while (left.size() > right.size()) {
            right.insert(*left.rbegin());
            left.erase(left.find(*left.rbegin()));
        }
        while (right.size() > left.size() + 1) {
            left.insert(*right.begin());
            right.erase(right.begin());
        }

        result.push_back(get_median());
    }

    return result;
}

#include <cassert>
#include <cmath>
#include <vector>

// Function declaration from the solution (assume it's included above).
std::vector<double> medianSlidingWindow(const std::vector<int>& nums, int k);

int main() {
    // Test 1: Given example.
    std::vector<int> nums1 = {1, 3, -1, -3, 5, 3, 6, 7};
    std::vector<double> res1 = medianSlidingWindow(nums1, 3);
    std::vector<double> expected1 = {1.0, -1.0, -1.0, 3.0, 5.0, 6.0};
    assert(res1.size() == expected1.size());
    for (size_t i = 0; i < expected1.size(); ++i) assert(std::fabs(res1[i] - expected1[i]) < 1e-9);

    // Test 2: Window size 1.
    std::vector<int> nums2 = {5, -2, 10};
    std::vector<double> res2 = medianSlidingWindow(nums2, 1);
    assert(res2.size() == 3);
    assert(res2[0] == 5.0 && res2[1] == -2.0 && res2[2] == 10.0);

    // Test 3: Window covers entire array, even k.
    std::vector<int> nums3 = {2, 4, 1, 3};
    std::vector<double> res3 = medianSlidingWindow(nums3, 4);
    assert(res3.size() == 1);
    assert(std::fabs(res3[0] - 2.5) < 1e-9);

    // Test 4: Duplicate values.
    std::vector<int> nums4 = {1, 1, 1, 1};
    std::vector<double> res4 = medianSlidingWindow(nums4, 2);
    std::vector<double> expected4 = {1.0, 1.0, 1.0};
    assert(res4.size() == expected4.size());
    for (size_t i = 0; i < expected4.size(); ++i) assert(res4[i] == expected4[i]);

    // Test 5: Negative numbers with odd k.
    std::vector<int> nums5 = {-1, -2, -3, -4, -5};
    std::vector<double> res5 = medianSlidingWindow(nums5, 3);
    std::vector<double> expected5 = {-2.0, -3.0, -4.0};
    for (size_t i = 0; i < expected5.size(); ++i) assert(res5[i] == expected5[i]);

    // Test 6: Single element window from a single-element array.
    std::vector<int> nums6 = {42};
    std::vector<double> res6 = medianSlidingWindow(nums6, 1);
    assert(res6.size() == 1 && res6[0] == 42.0);

    // Test 7: Large window with even k and negative values.
    std::vector<int> nums7 = {0, -5, 10, -3, 7};
    std::vector<double> res7 = medianSlidingWindow(nums7, 4);
    assert(res7.size() == 2);
    assert(std::fabs(res7[0] - (-1.5)) < 1e-9); // window {0,-5,10,-3} sorted: -5,-3,0,10 => avg(-3,0) = -1.5
    assert(std::fabs(res7[1] - 2.0) < 1e-9);    // window {-5,10,-3,7} sorted: -5,-3,7,10 => avg(-3,7) = 2.0

    return 0;
}

// The core idea is to maintain two multisets (or heaps) that split the current window into a "left" half (containing the smaller elements, conceptually a max-heap) and a "right" half (containing the larger elements, conceptually a min-heap). We enforce an invariant: the right half always contains either the same number of elements as the left half (if `k` is even) or exactly one more element (if `k` is odd). This ensures that the median is either the smallest element of the right half (odd `k`) or the average of the largest element of the left half and the smallest element of the right half (even `k`).
//
// Initialization: Insert the first `k` elements into the right multiset, then move the smallest `k/2` elements to the left multiset, so the invariant holds. For each subsequent step, add a new element `x` (at index `i`) and remove the element `y` (at index `i-k`) that slides out of the window. When inserting `x`, compare it to the current minimum of the right multiset: if `x >= *right.begin()`, insert into right; otherwise insert into left. When removing `y`, compare similarly: if `y >= *right.begin()`, erase from right (using a find iterator to avoid deleting duplicates); otherwise erase from left. After each insertion/removal, rebalance: if left has more elements than right, move the largest element of left to right; if right has more than left+1, move the smallest element of right to left. Then compute the median for the current window and append it to the result.
//
// Edge cases: `k=1` (each median is the element itself), `k` equal to array length (only one window), duplicate values (must use `find` when erasing to remove only one occurrence), and negative numbers (handled naturally by comparison). Time complexity is O(n log k) because each insertion and deletion in a multiset is O(log k), and each element is inserted and deleted exactly once. Space complexity is O(k) for the two multisets, plus O(n) for the output vector.
