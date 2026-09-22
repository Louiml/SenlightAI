// Write a C++ function that takes a vector of integers `nums` and an integer `target`, and returns a vector of all unique quadruplets `[nums[a], nums[b], nums[c], nums[d]]` such that `a < b < c < d` (indices, not values) and the sum of the four elements equals `target`. The solution must not contain duplicate quadruplets; two quadruplets are considered duplicates if they contain the same four values in any order. The function should handle input vectors containing duplicate values and negative numbers, and may receive an empty vector. The function signature should be `std::vector<std::vector<int>> fourSum(const std::vector<int>& nums, int target)`. The returned quadruplets should be sorted in ascending order (both the quadruplet elements and the list of quadruplets lexicographically).
#include <cassert>
#include <vector>
#include <iostream>

int main() {
    // Test 1: Basic case with repeated input values
    std::vector<int> nums1 = {1, 0, -1, 0, -2, 2};
    std::vector<std::vector<int>> result1 = fourSum(nums1, 0);
    std::vector<std::vector<int>> expected1 = {{-2, -1, 1, 2}, {-2, 0, 0, 2}, {-1, 0, 0, 1}};
    assert(result1 == expected1);

    // Test 2: Empty vector
    std::vector<int> nums2 = {};
    assert(fourSum(nums2, 0).empty());

    // Test 3: Vector with fewer than 4 elements
    std::vector<int> nums3 = {1, 2, 3};
    assert(fourSum(nums3, 6).empty());

    // Test 4: Negative target
    std::vector<int> nums4 = {2, 2, 2, 2, 2};
    std::vector<std::vector<int>> result4 = fourSum(nums4, 8);
    std::vector<std::vector<int>> expected4 = {{2, 2, 2, 2}};
    assert(result4 == expected4);

    // Test 5: Large numbers avoiding overflow
    std::vector<int> nums5 = {1000000000, 1000000000, 1000000000, 1000000000};
    std::vector<std::vector<int>> result5 = fourSum(nums5, -294967296);
    assert(result5.empty());

    // Test 6: All negative numbers
    std::vector<int> nums6 = {-3, -2, -1, 0, 1};
    std::vector<std::vector<int>> result6 = fourSum(nums6, -4);
    std::vector<std::vector<int>> expected6 = {{-3, -1, 0, 0}};
    assert(result6 == expected6);

    // Test 7: Duplicates in output should not appear
    std::vector<int> nums7 = {0, 0, 0, 0, 0};
    std::vector<std::vector<int>> result7 = fourSum(nums7, 0);
    std::vector<std::vector<int>> expected7 = {{0, 0, 0, 0}};
    assert(result7 == expected7);

    // Test 8: Mixed values requiring sorted output
    std::vector<int> nums8 = {4, 1, 2, -1, 1, -3};
    std::vector<std::vector<int>> result8 = fourSum(nums8, 1);
    std::vector<std::vector<int>> expected8 = {{-3, -1, 1, 4}, {-3, 1, 1, 2}};
    assert(result8 == expected8);

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
#include <vector>
#include <algorithm>
#include <cstdint>

std::vector<std::vector<int>> fourSum(const std::vector<int>& nums, int target) {
    std::vector<int> sorted_nums = nums;
    std::sort(sorted_nums.begin(), sorted_nums.end());
    
    std::vector<std::vector<int>> result;
    const int n = static_cast<int>(sorted_nums.size());
    
    for (int i = 0; i < n; ++i) {
        if (i > 0 && sorted_nums[i] == sorted_nums[i - 1]) {
            continue;
        }
        for (int j = i + 1; j < n; ++j) {
            if (j > i + 1 && sorted_nums[j] == sorted_nums[j - 1]) {
                continue;
            }
            
            int k = j + 1;
            int l = n - 1;
            
            while (k < l) {
                const int64_t a = static_cast<int64_t>(sorted_nums[i]);
                const int64_t b = static_cast<int64_t>(sorted_nums[j]);
                const int64_t c = static_cast<int64_t>(sorted_nums[k]);
                const int64_t d = static_cast<int64_t>(sorted_nums[l]);
                const int64_t sum = a + b + c + d;
                
                if (sum == static_cast<int64_t>(target)) {
                    result.push_back({sorted_nums[i], sorted_nums[j], sorted_nums[k], sorted_nums[l]});
                    ++k;
                    while (k < l && sorted_nums[k] == sorted_nums[k - 1]) {
                        ++k;
                    }
                    --l;
                    while (k < l && sorted_nums[l] == sorted_nums[l + 1]) {
                        --l;
                    }
                } else if (sum < static_cast<int64_t>(target)) {
                    ++k;
                    while (k < l && sorted_nums[k] == sorted_nums[k - 1]) {
                        ++k;
                    }
                } else {
                    --l;
                    while (k < l && sorted_nums[l] == sorted_nums[l + 1]) {
                        --l;
                    }
                }
            }
        }
    }
    
    return result;
}
// The algorithm begins by sorting the input array to enable a two-pointer approach and to simplify duplicate handling. The outer loop fixes the first element `i`, skipping any value equal to the previous `i` to avoid duplicate quadruplets. The second loop fixes the second element `j` starting from `i+1`, also skipping consecutive duplicates. With `i` and `j` fixed, the problem reduces to finding two numbers in the remaining subarray (indices `j+1` to `n-1`) that sum to `target - nums[i] - nums[j]`. Two pointers `k` (start) and `l` (end) are used: if the current four-sum equals the target, record the quadruplet, then move both pointers past duplicate values. If the sum is too small, increment `k`; otherwise decrement `l`. The comparison uses `long long` to avoid integer overflow when summing large values. Edge cases include empty input (returns empty vector), target sums requiring all negative numbers, and arrays with many duplicates—all handled by duplicate-skipping logic. Time complexity is \(O(n^3)\) due to two nested loops plus a linear two-pointer scan; space complexity is \(O(1)\) auxiliary (excluding the output storage).
