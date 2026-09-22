Given two integer vectors `nums1` and `nums2` of equal length `n` (n ≥ 2), you may perform an operation that swaps `nums1[i]` and `nums2[i]` for any index `i` (0 ≤ i < n). The goal is to make both `nums1[n-1]` (the last element of `nums1`) and `nums2[n-1]` (the last element of `nums2`) be the *maximum* values among all elements in their respective vectors, meaning that after any swaps, for every `i < n-1`, we require `nums1[i] ≤ nums1[n-1]` and `nums2[i] ≤ nums2[n-1]`. Write a C++ function `minSwapsToMaxEnd` that takes two `const std::vector<int>&` and returns the minimum number of swaps needed to achieve this condition, or `-1` if it is impossible. Note that you may also choose to swap the entire `nums1` and `nums2` contents if it helps (this counts as zero? — check: the original code swaps the vectors themselves, which is not counted as a swap operation but merely renames which vector is considered `nums1`; treat this as a free choice). The function must be `const`-correct and not modify the input vectors.

The key is that swapping the entire vectors (free) allows us to choose which original last element is assigned to which vector. Therefore we consider two possible orientations: (1) keep vectors as given, so the bounds are `a = nums1[n-1]` and `b = nums2[n-1]`; (2) swap the entire vectors, so the bounds become `a = nums2[n-1]` and `b = nums1[n-1]`. For each orientation, we iterate over the first `n-1` indices. At each index, we have two possible local actions: leave as is (requires `nums1[i] <= a` and `nums2[i] <= b`) or swap (requires `nums2[i] <= a` and `nums1[i] <= b`). If both are impossible, the orientation is infeasible. If exactly one is possible, that determines whether a swap is forced at that index. If both are possible, we can choose not to swap, costing zero. Thus for each orientation, the minimum number of swaps equals the count of indices where only the swap option is valid. We take the minimum over both orientations, or return `-1` if neither orientation is feasible. Edge cases include when an element exceeds both bounds, or when both elements at an index exceed one bound but not the other; these are handled by the conditions. The algorithm runs in `O(n)` time and uses `O(1)` extra space.

#include <vector>
#include <algorithm>

// Helper: given vectors and the target max values for each (a for nums1, b for nums2),
// return the minimum swaps needed, or -1 if impossible.
int countSwaps(const std::vector<int>& nums1, const std::vector<int>& nums2,
               int a, int b) {
    int n = static_cast<int>(nums1.size());
    int swaps = 0;

    for (int i = 0; i < n - 1; ++i) {
        bool noSwap = (nums1[i] <= a && nums2[i] <= b);
        bool doSwap = (nums2[i] <= a && nums1[i] <= b);

        if (!noSwap && !doSwap) {
            return -1;  // impossible at this index
        }
        if (!noSwap && doSwap) {
            ++swaps;    // forced to swap
        }
        // if both are possible, no swap needed
    }
    return swaps;
}

// Return the minimum swaps to make the last elements the maximum in each vector.
int minSwapsToMaxEnd(const std::vector<int>& nums1, const std::vector<int>& nums2) {
    int n = static_cast<int>(nums1.size());
    if (n == 0) return 0;

    // Orientation 1: keep vectors as given
    int a1 = nums1[n - 1];
    int b1 = nums2[n - 1];
    int res1 = countSwaps(nums1, nums2, a1, b1);

    // Orientation 2: swap entire vectors (free)
    int a2 = nums2[n - 1];
    int b2 = nums1[n - 1];
    int res2 = countSwaps(nums2, nums1, a2, b2);

    if (res1 == -1 && res2 == -1) return -1;
    if (res1 == -1) return res2;
    if (res2 == -1) return res1;
    return std::min(res1, res2);
}

#include <cassert>
#include <vector>

int main() {
    // Basic case requiring one swap
    assert(minSwapsToMaxEnd({3, 2}, {1, 5}) == 1);
    assert(minSwapsToMaxEnd({1, 5}, {3, 2}) == 1);

    // Already valid
    assert(minSwapsToMaxEnd({1, 2}, {3, 4}) == 0);
    assert(minSwapsToMaxEnd({2, 5}, {1, 3}) == 0); // after free whole swap, still 0

    // Impossible because an element exceeds both last values
    assert(minSwapsToMaxEnd({10, 1}, {2, 3}) == -1);

    // Both orientations require swaps, choose minimum
    assert(minSwapsToMaxEnd({2, 3, 5}, {4, 1, 6}) == 1);
    assert(minSwapsToMaxEnd({4, 1, 6}, {2, 3, 5}) == 1);

    // Larger case with forced swaps
    assert(minSwapsToMaxEnd({5, 3, 7}, {2, 8, 4}) == 2);

    // Equal last values, no swaps needed
    assert(minSwapsToMaxEnd({1, 2}, {3, 2}) == 0);

    // Impossible because both at an index exceed both bounds
    assert(minSwapsToMaxEnd({9, 1}, {8, 2}) == -1);

    return 0;
}
