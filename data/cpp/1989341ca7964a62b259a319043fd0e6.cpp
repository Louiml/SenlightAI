// Given an array of integers, write a C++ function `threeSumUnique` that returns a vector of vectors containing all unique triplets (i.e., combinations of three distinct indices) whose elements sum to zero. The input may contain duplicate values and may be unsorted. The output must not contain duplicate triplets (i.e., two triplets with the same three numbers in any order are considered identical and must appear only once). The order of triplets in the output does not matter. If no such triplets exist, return an empty vector.
The naive approach from the snippet has two critical flaws: (1) it has an infinite loop (`i++` inside the inner loop instead of `j++`), and (2) it returns duplicate triplets and includes triplets with repeated values if the input contains duplicates, but it also ignores the requirement of uniqueness. The correct approach: sort the input array first, then iterate with a fixed first element `i`. For each `i`, use two pointers (`left = i+1`, `right = n-1`) to find pairs that sum to `-nums[i]`. To avoid duplicates, skip the same value for `i` after the first use, and after finding a valid pair, move both pointers past any consecutive duplicate values. Edge cases: arrays with fewer than 3 elements return empty; all zeros return one triplet `{0,0,0}`; handling negative and positive numbers is natural after sorting. The sorting ensures that triplets are generated in a canonical order, and duplicate skipping ensures uniqueness. Time complexity is O(n^2) due to sorting O(n log n) plus two-pointer scan O(n) for each `i`. Space complexity is O(1) auxiliary (ignoring output storage and sorting overhead).
#include <vector>
#include <algorithm>

// Return all unique triplets (distinct indices) that sum to zero.
std::vector<std::vector<int>> threeSumUnique(std::vector<int> nums) {
    std::vector<std::vector<int>> result;
    int n = nums.size();
    if (n < 3) return result;

    std::sort(nums.begin(), nums.end());

    for (int i = 0; i < n - 2; ++i) {
        // Skip duplicate first elements.
        if (i > 0 && nums[i] == nums[i - 1]) continue;
        int target = -nums[i];
        int left = i + 1;
        int right = n - 1;

        while (left < right) {
            int sum = nums[left] + nums[right];
            if (sum == target) {
                result.push_back({nums[i], nums[left], nums[right]});
                // Skip duplicates for left and right.
                while (left < right && nums[left] == nums[left + 1]) ++left;
                while (left < right && nums[right] == nums[right - 1]) --right;
                ++left;
                --right;
            } else if (sum < target) {
                ++left;
            } else {
                --right;
            }
        }
    }
    return result;
}
#include <cassert>
#include <vector>
#include <algorithm>

// (Function definition from Solution goes here, or include it above)

int main() {
    // Example 1: basic case
    std::vector<int> nums1 = {-1, 0, 1, 2, -1, -4};
    std::vector<std::vector<int>> res1 = threeSumUnique(nums1);
    assert(res1.size() == 2);
    assert(std::find(res1.begin(), res1.end(), std::vector<int>{-1, -1, 2}) != res1.end());
    assert(std::find(res1.begin(), res1.end(), std::vector<int>{-1, 0, 1}) != res1.end());

    // Example 2: all zeros
    std::vector<int> nums2 = {0, 0, 0};
    std::vector<std::vector<int>> res2 = threeSumUnique(nums2);
    assert(res2.size() == 1);
    assert(res2[0] == std::vector<int>({0, 0, 0}));

    // Example 3: no triplets
    std::vector<int> nums3 = {1, 2, -2, -1};
    assert(threeSumUnique(nums3).empty());

    // Example 4: fewer than 3 elements
    std::vector<int> nums4 = {1, -1};
    assert(threeSumUnique(nums4).empty());

    // Example 5: duplicate values that only form one unique triplet
    std::vector<int> nums5 = {0, 0, 0, 0};
    std::vector<std::vector<int>> res5 = threeSumUnique(nums5);
    assert(res5.size() == 1);
    assert(res5[0] == std::vector<int>({0, 0, 0}));

    // Example 6: negative and positive with exact zero
    std::vector<int> nums6 = {-2, 0, 1, 1, 2};
    std::vector<std::vector<int>> res6 = threeSumUnique(nums6);
    assert(res6.size() == 2);
    assert(std::find(res6.begin(), res6.end(), std::vector<int>{-2, 0, 2}) != res6.end());
    assert(std::find(res6.begin(), res6.end(), std::vector<int>{-2, 1, 1}) != res6.end());

    // Example 7: no valid because all positive
    std::vector<int> nums7 = {1, 2, 3};
    assert(threeSumUnique(nums7).empty());

    // Example 8: mixed duplicates with multiple valid triplets
    std::vector<int> nums8 = {-1, -1, 2, 2, 0, 1, -4};
    std::vector<std::vector<int>> res8 = threeSumUnique(nums8);
    assert(res8.size() == 2);
    assert(std::find(res8.begin(), res8.end(), std::vector<int>{-1, -1, 2}) != res8.end());
    assert(std::find(res8.begin(), res8.end(), std::vector<int>{-1, 0, 1}) != res8.end());

    return 0;
}
