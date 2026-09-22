// Write a C++ function named `longestConsecutiveSequence` that takes a non-empty `std::vector<int>` by const reference and returns the length (as an `int`) of the longest consecutive elements sequence in the array. A consecutive sequence is defined as a set of numbers where each element is exactly one greater than the previous (e.g., `{100, 4, 200, 1, 3, 2}` has longest sequence `1, 2, 3, 4` of length 4). The input may contain duplicate values, negative numbers, and numbers in any order. The function must handle edge cases like a single element, all duplicates, and sequences interleaved with non-consecutive numbers. The solution must run in `O(n)` average time using a hash set, not by sorting.

The algorithm uses an `unordered_set<int>` to store all unique numbers from the input vector, which provides average `O(1)` insertion and lookup. Then, for each number in the original array (or equivalently, any number still present in the set), we attempt to build a streak around it. To avoid redundant work and prevent `O(n^2)` behavior, we erase each visited number from the set immediately when it is processed. For a given starting number `num`, we extend leftward by checking `num-1`, `num-2`, etc., and rightward by checking `num+1`, `num+2`, etc., incrementing the streak count for each found neighbor and erasing those found numbers from the set. This ensures each number is processed at most once across all iterations, giving an amortized `O(n)` time complexity and `O(n)` space for the hash set. Edge cases: empty input (though specified non-empty, still safe), single element returns 1, all duplicates returns 1, and negative numbers work naturally with decrements/increments. The use of `erase` while iterating is safe because we never modify the vector itself and only modify the set. For `n` elements, time is `O(n)` average (worst-case `O(n^2)` only with pathological hash collisions, but the standard library's hash is robust) and space is `O(n)` for the set, not counting the input vector.

#include <vector>
#include <unordered_set>
#include <algorithm>

// Returns the length of the longest consecutive elements sequence in nums.
// Uses a hash set to achieve O(n) average time.
int longestConsecutiveSequence(const std::vector<int>& nums) {
    if (nums.empty()) return 0;
    
    std::unordered_set<int> num_set(nums.begin(), nums.end());
    int longestStreak = 0;
    
    for (int num : nums) {
        // If num already processed as part of a previous streak, skip.
        if (num_set.find(num) == num_set.end()) continue;
        
        int streak = 1;
        num_set.erase(num);
        
        // Expand leftward (smaller numbers).
        int smaller = num - 1;
        while (num_set.find(smaller) != num_set.end()) {
            num_set.erase(smaller);
            ++streak;
            --smaller;
        }
        
        // Expand rightward (larger numbers).
        int larger = num + 1;
        while (num_set.find(larger) != num_set.end()) {
            num_set.erase(larger);
            ++streak;
            ++larger;
        }
        
        longestStreak = std::max(longestStreak, streak);
    }
    
    return longestStreak;
}

#include <cassert>
#include <vector>

int main() {
    // Basic case from problem description.
    assert(longestConsecutiveSequence({100, 4, 200, 1, 3, 2}) == 4);
    
    // Single element.
    assert(longestConsecutiveSequence({5}) == 1);
    
    // All duplicates.
    assert(longestConsecutiveSequence({7, 7, 7, 7}) == 1);
    
    // Negative numbers and mixed order.
    assert(longestConsecutiveSequence({-5, -3, -4, -2, -1, 0, 1, 2}) == 8);
    
    // Non-consecutive isolated groups.
    assert(longestConsecutiveSequence({10, 20, 30, 40}) == 1);
    
    // Duplicates within a consecutive run.
    assert(longestConsecutiveSequence({1, 2, 2, 3, 3, 3, 4}) == 4);
    
    // Large gap and zero.
    assert(longestConsecutiveSequence({0, 1, 2, 100, 101}) == 3);
    
    // Two adjacent sequences, pick longest.
    assert(longestConsecutiveSequence({9, 1, 2, 3, 4, 5, 6, 8}) == 6);
    
    // Very large single streak.
    std::vector<int> big;
    for (int i = -100000; i <= 100000; ++i) big.push_back(i);
    assert(longestConsecutiveSequence(big) == 200001);
    
    return 0;
}
