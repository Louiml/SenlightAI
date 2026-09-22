// Write a C++ function named `longestConsecutiveSequence` that takes a `const std::vector<int>&` and returns an `int` representing the length of the longest consecutive elements sequence (i.e., numbers that appear in the input and are consecutive integers, such as 3,4,5) that can be formed from the numbers in the array. The function must handle duplicate values (they only count once), an empty input (return 0), and negative numbers. The algorithm must run in O(n) average time and O(n) space, and must not sort the input. Use an unordered set to achieve efficiency.

The solution uses a hash set to store all unique numbers from the input. For each number in the set, we check if it is the start of a consecutive sequence by verifying that `num - 1` is not present. If it is not present, then `num` is the smallest element of a consecutive run, and we expand by checking `num + 1`, `num + 2`, etc., counting how many consecutive numbers exist. We update the answer with the maximum run length found. This avoids O(n²) work because each element is visited at most twice: once when it is a start candidate, and once when it is counted in a run (since runs are only processed from their start). Edge cases include an empty array (return 0) and arrays with all identical values (return 1). Duplicates are naturally handled by the set. Time complexity is O(n) average due to hash set operations, and space complexity is O(n) for the set.

#include <vector>
#include <unordered_set>
#include <algorithm>

// Returns the length of the longest consecutive elements sequence
// from the given vector of integers. Uses an unordered set for O(n) average time.
int longestConsecutiveSequence(const std::vector<int>& nums) {
    // Handle empty input immediately.
    if (nums.empty()) {
        return 0;
    }

    // Build a hash set of unique numbers.
    std::unordered_set<int> numSet(nums.begin(), nums.end());

    int maxLength = 1;

    // Iterate over each unique number.
    for (int num : numSet) {
        // Only process numbers that are the start of a consecutive sequence.
        // If num-1 exists, num is not a start; skip to avoid duplicate work.
        if (numSet.find(num - 1) != numSet.end()) {
            continue;
        }

        // Count how many consecutive numbers follow num.
        int currentNum = num + 1;
        int currentLength = 1;

        while (numSet.find(currentNum) != numSet.end()) {
            ++currentNum;
            ++currentLength;
        }

        // Update the maximum length found.
        maxLength = std::max(maxLength, currentLength);
    }

    return maxLength;
}

#include <cassert>
#include <vector>

int main() {
    // Test 1: Basic sequence.
    std::vector<int> nums1 = {100, 4, 200, 1, 3, 2};
    assert(longestConsecutiveSequence(nums1) == 4); // 1,2,3,4

    // Test 2: Empty input.
    std::vector<int> nums2 = {};
    assert(longestConsecutiveSequence(nums2) == 0);

    // Test 3: All duplicates.
    std::vector<int> nums3 = {5, 5, 5, 5};
    assert(longestConsecutiveSequence(nums3) == 1);

    // Test 4: Negative numbers.
    std::vector<int> nums4 = {-3, -2, -1, 0, 1, 2};
    assert(longestConsecutiveSequence(nums4) == 6);

    // Test 5: Single element.
    std::vector<int> nums5 = {42};
    assert(longestConsecutiveSequence(nums5) == 1);

    // Test 6: No consecutive numbers.
    std::vector<int> nums6 = {1, 3, 5, 7};
    assert(longestConsecutiveSequence(nums6) == 1);

    // Test 7: Interleaved duplicates and multiple runs.
    std::vector<int> nums7 = {1, 2, 2, 3, 10, 11, 12, 13, 14, 15};
    assert(longestConsecutiveSequence(nums7) == 6); // 10-15

    // Test 8: Large sequence overlapping edges.
    std::vector<int> nums8 = {0, -1, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    assert(longestConsecutiveSequence(nums8) == 11); // -1 to 9

    return 0;
}
