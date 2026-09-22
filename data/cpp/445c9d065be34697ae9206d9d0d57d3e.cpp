Write a C++ function `std::vector<int> assignDepths(const std::vector<int>& values)` that, given a non-empty vector of distinct integers, returns a vector of the same length where each position `i` holds the depth of `values[i]` in the Cartesian tree built from the input array. In a Cartesian tree, the root is the maximum element of the entire array, and its depth is 0. Recursively, for any subarray `[l, r]`, the maximum element in that subarray becomes the root of the corresponding subtree, with depth equal to one more than its parent's depth. The function should process the input recursively without modifying it, and must work for arrays of any size up to at least 100. You must not use global variables; the recursive helper should accept the array, indices `l`, `r`, and the current depth, and fill an output vector passed by reference. The input values are guaranteed to be distinct, but may be negative or non-negative. The function should return the depth vector, with each element corresponding to the original index in the input.
The problem is solved by recursively building a Cartesian tree implicitly. The core idea: for any contiguous segment `[l, r]`, the element with maximum value is the root of the subtree representing that segment. Its depth is given by the parameter `currentDepth` passed from the caller (root call uses depth 0). Then, the left subarray `[l, idx-1]` and right subarray `[idx+1, r]` are processed recursively with depth `currentDepth+1`. Since input values are distinct, the maximum index is unique. The base case is when `l > r`, do nothing. We need to find the maximum element in the segment by a linear scan. Each recursive call handles one element, so there are exactly `n` calls, each performing a scan over its segment. The total work is `O(n^2)` in the worst case (e.g., a sorted array where each segment shrinks by 1) but on average `O(n log n)` for random data. Space complexity is `O(n)` for the output vector plus `O(n)` recursion depth in the worst case (e.g., a skewed tree), so overall `O(n)` auxiliary space. Edge cases: single element (returns `[0]`), all elements equal? But distinct guarantee avoids that. Negative values are handled by comparing integers. The recursion must correctly handle empty segments. The constant `Maxn=105` from the snippet is not needed, but the function should work for any size.
#include <vector>
#include <algorithm>

// Helper function: recursively fill depths for segment [l, r].
// values: input array (distinct integers)
// l, r: current segment bounds (inclusive)
// currentDepth: depth of the root of this segment
// depths: output vector to be filled (passed by reference)
void buildDepths(const std::vector<int>& values, int l, int r, int currentDepth, std::vector<int>& depths) {
    if (l > r) return;

    // Find index of maximum element in [l, r]
    int maxIdx = l;
    for (int i = l + 1; i <= r; ++i) {
        if (values[i] > values[maxIdx]) {
            maxIdx = i;
        }
    }

    // Assign depth for this root
    depths[maxIdx] = currentDepth;

    // Recursively process left and right subarrays
    buildDepths(values, l, maxIdx - 1, currentDepth + 1, depths);
    buildDepths(values, maxIdx + 1, r, currentDepth + 1, depths);
}

// Public function: returns a vector of depths for each element in 'values'.
// The Cartesian tree root (global maximum) has depth 0.
std::vector<int> assignDepths(const std::vector<int>& values) {
    int n = static_cast<int>(values.size());
    std::vector<int> depths(n, 0);
    if (n == 0) return depths; // handle empty input defensively
    buildDepths(values, 0, n - 1, 0, depths);
    return depths;
}
#include <cassert>
#include <vector>

// Include the solution function here (or link accordingly)
// For completeness, the function definitions are assumed to be above.

int main() {
    // Basic case: single element
    std::vector<int> v1 = {42};
    assert(assignDepths(v1) == std::vector<int>({0}));

    // Sorted increasing: root at last element, depths 0..n-1
    std::vector<int> v2 = {1, 2, 3, 4};
    std::vector<int> res2 = assignDepths(v2);
    assert(res2 == std::vector<int>({3, 2, 1, 0}));

    // Sorted decreasing: root at first element
    std::vector<int> v3 = {10, 9, 8, 7};
    std::vector<int> res3 = assignDepths(v3);
    assert(res3 == std::vector<int>({0, 1, 2, 3}));

    // Random distinct values, root at 5 (index 2)
    std::vector<int> v4 = {3, 1, 5, 2, 4};
    std::vector<int> res4 = assignDepths(v4);
    // Expected depths: root (5) depth 0. Left subarray [3,1] max=3 depth1, then 1 depth2. Right [2,4] max=4 depth1, then 2 depth2.
    assert(res4 == std::vector<int>({1, 2, 0, 2, 1}));

    // Negative values
    std::vector<int> v5 = {-5, -1, -10};
    std::vector<int> res5 = assignDepths(v5);
    // Root -1 at index 1 depth0, then -5 depth1, -10 depth1
    assert(res5 == std::vector<int>({1, 0, 1}));

    // Larger test with known structure: array where max is middle
    std::vector<int> v6 = {1, 3, 2, 5, 4, 6, 0};
    // root 6 at index5 depth0. Left [1,3,2,5,4] max=5 at idx3 depth1. Right [0] depth1.
    // For left: max 5 idx3 depth1. Left of that [1,3,2] max=3 idx1 depth2. Right [4] depth2.
    // For [1,3,2]: left [1] depth3, right [2] depth3. So depths:
    // idx0: depth3, idx1: depth2, idx2: depth3, idx3: depth1, idx4: depth2, idx5: depth0, idx6: depth1
    std::vector<int> res6 = assignDepths(v6);
    assert(res6 == std::vector<int>({3, 2, 3, 1, 2, 0, 1}));

    // Test that original vector is not modified
    std::vector<int> original = {2, 1, 3};
    std::vector<int> copy = original;
    std::vector<int> res7 = assignDepths(original);
    assert(original == copy);
    assert(res7 == std::vector<int>({1, 2, 0}));

    return 0;
}
