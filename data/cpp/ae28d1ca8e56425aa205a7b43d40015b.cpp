Write a C++ function `int longestConsecutiveSequence(const std::vector<int>& nums)` that takes a vector of integers (which may contain duplicates and may be unsorted) and returns the length of the longest consecutive elements sequence. A consecutive sequence is defined as a set of numbers where each number is exactly 1 greater than the previous number (e.g., {1, 2, 3, 4}). The solution must run in O(n) time on average, so sorting is not allowed. You should implement this using an unordered set to store the numbers and then iterate through each number, checking if it is the start of a sequence (i.e., `num - 1` is not in the set). If it is the start, expand forward to find the full length of the consecutive sequence and track the maximum length found. Duplicate numbers in the input should be ignored, and the function should return 0 for an empty input.

The main algorithm uses an unordered_set to store all unique numbers from the input, which gives average O(1) insertion and lookup. We then iterate over each number in the input (or more efficiently, over the set itself, but iterating over the input vector is fine since duplicates are handled by the set). For each number `num`, we check if `num - 1` exists in the set; if not, then `num` is the start of a consecutive sequence. We then count upward from `num` while `num + 1` exists in the set, incrementing a counter. We update the maximum sequence length accordingly. This avoids processing numbers that are not sequence starts, ensuring that each number is visited at most twice (once for the start check and once as part of a forward expansion). Edge cases include empty input (return 0), all duplicates (return 1 for a single unique number), and negative numbers. Time complexity is O(n) on average for insertion and lookups, and O(n) total across all expansions, yielding O(n) average and O(n²) worst-case (though practically O(n)). Space complexity is O(n) for the set.

#include <vector>
#include <unordered_set>

// Return the length of the longest consecutive elements sequence.
int longestConsecutiveSequence(const std::vector<int>& nums) {
    // Handle empty input.
    if (nums.empty()) {
        return 0;
    }

    // Store all unique numbers for O(1) average lookup.
    std::unordered_set<int> numSet(nums.begin(), nums.end());

    int maxLength = 0;

    // Iterate over the set to avoid processing duplicates multiple times.
    for (const int num : numSet) {
        // Only start a sequence if num - 1 is not in the set (i.e., num is the start).
        if (numSet.find(num - 1) == numSet.end()) {
            int current = num;
            int length = 1;

            // Expand forward while consecutive numbers exist.
            while (numSet.find(current + 1) != numSet.end()) {
                ++current;
                ++length;
            }

            // Update maximum length.
            if (length > maxLength) {
                maxLength = length;
            }
        }
    }

    return maxLength;
}

#include <cassert>
#include <vector>

int main() {
    // Basic cases
    assert(longestConsecutiveSequence({1, 2, 3, 4}) == 4);
    assert(longestConsecutiveSequence({100, 4, 200, 1, 3, 2}) == 4);
    assert(longestConsecutiveSequence({0, 3, 7, 2, 5, 8, 4, 6, 0, 1}) == 9);

    // Duplicates and unsorted
    assert(longestConsecutiveSequence({5, 5, 5, 5}) == 1);
    assert(longestConsecutiveSequence({3, 1, 2, 2, 1, 3, 4}) == 4);

    // Negative numbers and zero
    assert(longestConsecutiveSequence({-2, -1, 0, 1, 2}) == 5);
    assert(longestConsecutiveSequence({-5, -3, -4, -2}) == 4);

    // Edge cases
    assert(longestConsecutiveSequence({}) == 0);
    assert(longestConsecutiveSequence({7}) == 1);
    assert(longestConsecutiveSequence({0, -1, 1}) == 3);

    // Non-consecutive mixed with consecutive
    assert(longestConsecutiveSequence({10, 11, 12, 20, 21, 22, 23}) == 4);
    assert(longestConsecutiveSequence({1, 2, 3, 100, 101, 102, 103, 104}) == 5);

    return 0;
}
