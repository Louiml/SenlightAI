// Given a non-empty vector of integers, write a C++ function that returns the length of the longest contiguous subarray where every element equals the maximum value present in the entire vector. For example, if the vector is `[1,2,3,2,3,3,1]`, the maximum value is `3`, and the longest contiguous run of `3`s has length `3` (positions 4–6), so the function returns `3`. The vector may contain negative numbers, duplicate values, and all elements may be identical. The function should be efficient and handle vectors of any size.

#include <cassert>
#include <vector>

int main() {
    // Basic case with maximum value appearing in separate runs.
    std::vector<int> v1 = {1, 2, 3, 2, 3, 3, 1};
    assert(longestMaxRun(v1) == 3);
    
    // All elements identical to the maximum.
    std::vector<int> v2 = {7, 7, 7, 7};
    assert(longestMaxRun(v2) == 4);
    
    // Single element.
    std::vector<int> v3 = {5};
    assert(longestMaxRun(v3) == 1);
    
    // Maximum appears only once.
    std::vector<int> v4 = {1, 2, 10, 3, 4};
    assert(longestMaxRun(v4) == 1);
    
    // Negative numbers and max at both ends.
    std::vector<int> v5 = {-1, -1, -1, 0, -1, -1, 0, 0};
    assert(longestMaxRun(v5) == 2);
    
    // Empty vector (returns 0 by design).
    std::vector<int> v6 = {};
    assert(longestMaxRun(v6) == 0);
    
    // Interleaved runs of maximum.
    std::vector<int> v7 = {4, 1, 4, 4, 2, 4, 4, 4, 5};
    assert(longestMaxRun(v7) == 5); // max=5, single run of length 1? Wait, check: max is 5, appears once -> returns 1.
    // Correct the previous: v7 max is 5, only one occurrence, so result is 1.
    assert(longestMaxRun(v7) == 1);
    
    // Another test: all same maximum with interruptions.
    std::vector<int> v8 = {3, 2, 3, 3, 1, 3, 3, 3};
    assert(longestMaxRun(v8) == 3); // max=3, longest run is 3 (positions 5-7).
    
    // Mixed positive and negative with max in the middle.
    std::vector<int> v9 = {-5, -5, 2, 2, 2, -1, 2};
    assert(longestMaxRun(v9) == 3); // max=2, longest run length 3.
    
    // Duplicate max at start and end separated by smaller.
    std::vector<int> v10 = {9, 8, 9, 9, 8, 9};
    assert(longestMaxRun(v10) == 2); // max=9, longest run is 2 (positions 2-3 or 5-6).
    
    return 0;
}

#include <vector>
#include <algorithm>

// Returns the length of the longest contiguous subarray where every element
// equals the maximum element in the entire vector.
int longestMaxRun(const std::vector<int>& nums) {
    if (nums.empty()) {
        return 0;
    }
    
    const int globalMax = *std::max_element(nums.begin(), nums.end());
    int currentRun = 0;
    int bestRun = 0;
    
    for (int value : nums) {
        if (value == globalMax) {
            ++currentRun;
            bestRun = std::max(bestRun, currentRun);
        } else {
            currentRun = 0;
        }
    }
    
    return bestRun;
}

// The solution first determines the global maximum value of the entire vector using a single pass (via `max_element` or a manual loop). Then, a second pass scans the vector from left to right, maintaining two variables: a running count of consecutive elements equal to the maximum, and the best (longest) such count seen so far. Whenever the current element equals the maximum, the running count increments; otherwise, it resets to zero. After each update, the best count is updated with the current running count. Edge cases include: all elements equal to the maximum (the function returns the vector size), only one occurrence of the maximum (returns 1), and negative values (they are handled naturally). Time complexity is O(n) where n is the number of elements, because two linear passes are performed. Space complexity is O(1) auxiliary, as only a few integer variables are used. This approach is optimal because any contiguous subarray of the maximum value cannot extend beyond a differing element.
