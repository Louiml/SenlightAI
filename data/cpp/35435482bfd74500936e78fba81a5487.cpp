Write a C++ function that, given a non-empty vector of integers, returns the length of the longest strictly increasing subsequence (LIS). A subsequence is obtained by deleting zero or more elements from the original sequence without changing the order of the remaining elements. Strictly increasing means each element must be greater than the previous one in the subsequence. The function must handle vectors of any size, including size 1 (return 1) and size 0 (return 0, though the input is specified as non-empty, handle it gracefully). The algorithm must run in O(n log n) time using binary search and tail replacement, not the O(n²) dynamic programming approach.
The optimal solution uses a patience-sorting‑like approach. Maintain a vector `tails` where `tails[k]` is the smallest possible tail value of an increasing subsequence of length `k+1` (0-indexed). Initially `tails` is empty. For each number `x` in the input, find the first position `pos` in `tails` where `tails[pos] >= x` using lower_bound (or upper_bound if dealing with strictly increasing, but lower_bound works here). If `pos` equals the current size of `tails`, append `x` (extending the longest subsequence). Otherwise, replace `tails[pos]` with `x` (updating an existing subsequence tail to a smaller value). After processing all elements, the size of `tails` is the LIS length. Edge cases: empty input returns 0; single element returns 1; duplicate values do not extend the subsequence because we use lower_bound and replace, so strictly increasing is enforced. Time complexity is O(n log n) due to binary search per element; auxiliary space is O(n) for `tails`.
#include <algorithm>
#include <vector>

// Return the length of the longest strictly increasing subsequence.
// Uses binary search and tail replacement (O(n log n)).
int longestIncreasingSubsequence(const std::vector<int>& nums) {
    std::vector<int> tails;  // tails[k] = smallest tail of a subsequence of length k+1

    for (int x : nums) {
        // Find first tail >= x (since strictly increasing, we replace/append at this point)
        auto it = std::lower_bound(tails.begin(), tails.end(), x);
        if (it == tails.end()) {
            tails.push_back(x);  // extend the longest subsequence
        } else {
            *it = x;  // replace to keep tails as small as possible
        }
    }
    return static_cast<int>(tails.size());
}
#include <cassert>
#include <vector>

int main() {
    // Compare with known LIS lengths
    std::vector<int> v1 = {10, 9, 2, 5, 3, 7, 101, 18};
    assert(longestIncreasingSubsequence(v1) == 4);  // [2,3,7,101] or [2,5,7,18]

    std::vector<int> v2 = {0, 1, 0, 3, 2, 3};
    assert(longestIncreasingSubsequence(v2) == 4);  // [0,1,2,3]

    std::vector<int> v3 = {7, 7, 7, 7};
    assert(longestIncreasingSubsequence(v3) == 1);  // strict, only one of them

    std::vector<int> v4 = {5};
    assert(longestIncreasingSubsequence(v4) == 1);

    std::vector<int> v5 = {};  // empty (though spec says non-empty, test robustness)
    assert(longestIncreasingSubsequence(v5) == 0);

    std::vector<int> v6 = {3, 1, 4, 1, 5, 9, 2, 6, 5};
    assert(longestIncreasingSubsequence(v6) == 4);  // [1,2,5,9] or [1,2,6,5? no, [1,2,5,9] works]

    std::vector<int> v7 = {2, 1, 2, 3, 0, 4};
    assert(longestIncreasingSubsequence(v7) == 4);  // [1,2,3,4]

    std::vector<int> v8 = {-5, -1, -3, 0, 2};
    assert(longestIncreasingSubsequence(v8) == 4);  // [-5,-1,0,2] or [-5,-3,0,2]

    std::vector<int> v9 = {1, 2, 3, 4, 5};
    assert(longestIncreasingSubsequence(v9) == 5);

    std::vector<int> v10 = {5, 4, 3, 2, 1};
    assert(longestIncreasingSubsequence(v10) == 1);
}
