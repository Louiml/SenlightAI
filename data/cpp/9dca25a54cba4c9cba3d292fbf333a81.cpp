Write a C++ function `findTriplets` that accepts a vector of integers `nums`, its size `n`, and a target sum `K`, and returns a vector of all unique triplets `(nums[i], nums[j], nums[k])` with `i < j < k` such that `nums[i] + nums[j] + nums[k] == K`. The input vector is not necessarily sorted and may contain duplicates (including negative and zero values). The output must not contain duplicate triplets, even if the same values appear at different indices. The order of the triplets in the output is not specified, but within each triplet the values must appear in non-decreasing order. If no such triplet exists, return an empty vector. You may modify the input vector (e.g., by sorting), but the function must be self-contained and work for any valid vector size (including size less than 3, where the answer is empty).

#include <cassert>
#include <vector>
#include <algorithm>

// The solution function must be declared above this test.

int main() {
    // Basic case with unique triplets.
    std::vector<int> nums1 = {1, 2, 3, 4, 5};
    auto res1 = findTriplets(nums1, nums1.size(), 9);
    assert(res1.size() == 2);
    // Expected triplets: (1,3,5) and (2,3,4) after sorting values within each.
    // Verify by checking exact contents after sorting the vector of triplets.
    std::sort(res1.begin(), res1.end());
    assert(res1[0] == std::vector<int>({1, 3, 5}));
    assert(res1[1] == std::vector<int>({2, 3, 4}));

    // Duplicate values in input: must produce only unique triplets.
    std::vector<int> nums2 = {1, 1, 2, 2, 3};
    auto res2 = findTriplets(nums2, nums2.size(), 4);
    assert(res2.size() == 1);
    assert(res2[0] == std::vector<int>({1, 1, 2}));

    // Negative numbers and zero.
    std::vector<int> nums3 = {-1, 0, 1, 2, -1, -4};
    auto res3 = findTriplets(nums3, nums3.size(), 0);
    assert(res3.size() == 2);
    std::sort(res3.begin(), res3.end());
    assert(res3[0] == std::vector<int>({-1, -1, 2}));
    assert(res3[1] == std::vector<int>({-1, 0, 1}));

    // No valid triplets.
    std::vector<int> nums4 = {1, 2, 3};
    auto res4 = findTriplets(nums4, nums4.size(), 10);
    assert(res4.empty());

    // Fewer than 3 elements.
    std::vector<int> nums5 = {1, 2};
    assert(findTriplets(nums5, nums5.size(), 3).empty());

    // All same values: only one unique triplet if it exists.
    std::vector<int> nums6 = {5, 5, 5, 5};
    auto res6 = findTriplets(nums6, nums6.size(), 15);
    assert(res6.size() == 1);
    assert(res6[0] == std::vector<int>({5, 5, 5}));

    // Large number of same values and target that doesn't match.
    std::vector<int> nums7 = {7, 7, 7, 7, 7};
    assert(findTriplets(nums7, nums7.size(), 20).empty());

    // Duplicate triplets from different positions but same values must be merged.
    std::vector<int> nums8 = {0, 0, 0, 0};
    auto res8 = findTriplets(nums8, nums8.size(), 0);
    assert(res8.size() == 1);
    assert(res8[0] == std::vector<int>({0, 0, 0}));
}

#include <vector>
#include <algorithm>

// Return all unique triplets (with indices i<j<k) that sum to K.
// The returned triplets are sorted in non-decreasing order within each triplet.
std::vector<std::vector<int>> findTriplets(std::vector<int> nums, int n, int K) {
    std::vector<std::vector<int>> ans;
    if (n < 3) return ans;

    std::sort(nums.begin(), nums.end());

    for (int i = 0; i < n - 2; ++i) {
        // Skip duplicate values for the first element.
        if (i > 0 && nums[i] == nums[i - 1]) continue;

        int j = i + 1;
        int k = n - 1;
        while (j < k) {
            int sum = nums[i] + nums[j] + nums[k];
            if (sum > K) {
                --k;
            } else if (sum < K) {
                ++j;
            } else {
                ans.push_back({nums[i], nums[j], nums[k]});
                ++j;
                --k;
                // Skip duplicates for the second element.
                while (j < k && nums[j] == nums[j - 1]) ++j;
                // Skip duplicates for the third element.
                while (j < k && nums[k] == nums[k + 1]) --k;
            }
        }
    }
    return ans;
}

// The solution uses the classic two-pointer technique after sorting the array. First, sort `nums` in non-decreasing order. Then iterate with index `i` from 0 to `n-3`. For each `i`, skip duplicate values of `nums[i]` to avoid generating identical triplets from different starting positions (e.g., if `nums = [1,1,2]` and `K=4`, only the triplet `(1,1,2)` should be produced, not two duplicates). For a fixed `i`, set `j = i+1` and `k = n-1`. While `j < k`, compute `sum = nums[i] + nums[j] + nums[k]`.  
// - If `sum > K`, decrement `k` to reduce the sum.  
// - If `sum < K`, increment `j` to increase the sum.  
// - If `sum == K`, record the triplet `{nums[i], nums[j], nums[k]}` (which is already sorted). Then increment `j` and decrement `k` to continue searching. After moving, skip any duplicate values of `nums[j]` (by advancing `j` while `j < k` and `nums[j] == nums[j-1]`) and skip duplicate values of `nums[k]` (by decrementing `k` while `j < k` and `nums[k] == nums[k+1]`).  
// Edge cases:  
// - If `n < 3`, return empty vector immediately.  
// - Duplicates in the array are handled by skipping at the outer loop and after each found triplet.  
// - Negative numbers and zeros naturally work with the numeric comparisons.  
// - If the array contains many duplicates of the same value, the skipping logic ensures each unique combination is only output once.  
// Time complexity: Sorting takes `O(n log n)`. The two-pointer loop runs in `O(n^2)` in the worst case because for each `i` we scan at most `n` pairs. Total time is `O(n^2)` (dominated by the nested loops). Space complexity is `O(n)` for the output vector (which can store up to `O(n^2)` triplets in theory, but at most `n` triplets if all values are distinct? Actually there can be `O(n^2)` triplets? No, for a given sum condition, the number of unique triplets is `O(n^2)` in the worst case, but the algorithm uses only `O(1)` auxiliary space beyond the output, since we only use a few integer variables). Typically we say extra space `O(1)` excluding output.
