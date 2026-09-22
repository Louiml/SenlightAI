Write a standalone C++ function named `findTripletsWithZeroSum` that takes a `std::vector<int>` by value (so it can be freely sorted without affecting the caller) and returns a `std::vector<std::vector<int>>` containing all unique triplets `(a, b, c)` such that `a + b + c == 0`. Each triplet must be sorted in non‑decreasing order internally, and the overall list must contain no duplicate triplets (order of triplets in the output does not matter). The function should handle inputs with negative numbers, zeros, duplicates, and sizes from 0 upward. For an empty input or an input with fewer than 3 elements, return an empty vector. The solution must be efficient enough for vectors of size up to 3000, and you must not use any external libraries beyond the standard C++ ones.
#include <cassert>
#include <vector>
#include <algorithm>

// Free function (already declared above) – assume included here.

int main() {
    // Basic case from original snippet.
    std::vector<int> nums1 = {-1, 0, 1, 2, -1, -4};
    auto res1 = findTripletsWithZeroSum(nums1);
    assert(res1.size() == 2);
    // Verify each triplet sums to zero and contains no duplicate triplets.
    for (auto& t : res1) {
        assert(t[0] + t[1] + t[2] == 0);
        std::sort(t.begin(), t.end());
    }
    std::sort(res1.begin(), res1.end());
    std::vector<std::vector<int>> expected1 = {{-1, -1, 2}, {-1, 0, 1}};
    assert(res1 == expected1);

    // All zeros: one triplet.
    std::vector<int> nums2 = {0, 0, 0, 0};
    auto res2 = findTripletsWithZeroSum(nums2);
    assert(res2.size() == 1);
    assert(res2[0] == std::vector<int>({0, 0, 0}));

    // No triplets: all positive.
    std::vector<int> nums3 = {1, 2, 3, 4};
    assert(findTripletsWithZeroSum(nums3).empty());

    // Negative sum impossible.
    std::vector<int> nums4 = {-5, -4, -3, -2};
    assert(findTripletsWithZeroSum(nums4).empty());

    // Single triplet with duplicates.
    std::vector<int> nums5 = {-2, 1, 1, 1, 0};
    auto res5 = findTripletsWithZeroSum(nums5);
    assert(res5.size() == 1);
    assert(res5[0] == std::vector<int>({-2, 1, 1}));

    // Empty and small inputs.
    assert(findTripletsWithZeroSum({}).empty());
    assert(findTripletsWithZeroSum({0}).empty());
    assert(findTripletsWithZeroSum({0, 0}).empty());

    // Larger case with mixed values.
    std::vector<int> nums6 = {-3, -2, -1, 0, 1, 2, 3};
    auto res6 = findTripletsWithZeroSum(nums6);
    // Expected unique triplets: (-3,0,3), (-3,1,2), (-2,-1,3), (-2,0,2), (-1,0,1)
    assert(res6.size() == 5);

    // Check each sums to zero and no duplicates.
    std::vector<std::vector<int>> sorted_res6 = res6;
    for (auto& t : sorted_res6) {
        assert(t[0] + t[1] + t[2] == 0);
        std::sort(t.begin(), t.end());
    }
    std::sort(sorted_res6.begin(), sorted_res6.end());
    // Simple uniqueness check via set size.
    std::vector<std::vector<int>> expected6 = {{-3,0,3},{-3,1,2},{-2,-1,3},{-2,0,2},{-1,0,1}};
    assert(sorted_res6 == expected6);

    return 0;
}
#include <vector>
#include <algorithm>

// Return all unique triplets (a,b,c) from nums such that a+b+c == 0.
// Triplets are sorted internally; no duplicates in output.
std::vector<std::vector<int>> findTripletsWithZeroSum(std::vector<int> nums) {
    std::vector<std::vector<int>> result;
    const int n = static_cast<int>(nums.size());
    if (n < 3) return result;
    
    std::sort(nums.begin(), nums.end());
    
    for (int i = 0; i < n - 2; ++i) {
        // Skip duplicate first elements.
        if (i > 0 && nums[i] == nums[i - 1]) continue;
        // Since array is sorted, if smallest remaining sum > 0, no more triplets.
        if (nums[i] > 0) break;
        
        int left = i + 1;
        int right = n - 1;
        const int target = -nums[i];
        
        while (left < right) {
            const int current_sum = nums[left] + nums[right];
            if (current_sum == target) {
                result.push_back({nums[i], nums[left], nums[right]});
                ++left;
                --right;
                // Skip duplicates for left and right.
                while (left < right && nums[left] == nums[left - 1]) ++left;
                while (left < right && nums[right] == nums[right + 1]) --right;
            } else if (current_sum < target) {
                ++left;
            } else {
                --right;
            }
        }
    }
    return result;
}
// The canonical approach is to sort the input array first. Sorting allows us to use the two‑pointer technique: fix the first element `nums[i]` and then search for two other elements in the remaining subarray `[i+1, end]` that sum to `-nums[i]`. To avoid duplicates, we skip any `i` that repeats the previous value. Inside the two‑pointer loop, when a valid sum is found, we add the triplet and then skip over consecutive equal values on both the left and right pointers. Edge cases: if the fixed element is positive, we can break early because all later elements are larger and sums can only increase; if the array has fewer than 3 elements, return empty; handle cases with all zeros (one triplet `{0,0,0}`) and with repeated zeros/negatives. Time complexity is O(n²) because for each `i` we iterate the remaining part with two pointers. Sorting takes O(n log n). Space complexity is O(1) extra aside from the output storage (which can be O(n²) in the worst case if many triplets exist).
