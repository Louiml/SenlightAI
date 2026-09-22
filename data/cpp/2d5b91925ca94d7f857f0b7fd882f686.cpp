// Implement a C++ function `longestRun` that takes a `const std::vector<int>&` and returns the length of the longest consecutive subsequence where each element is equal to its predecessor plus 1 (i.e., a run of consecutive integers). The input vector may contain duplicate values, and the vector may be empty. The run must be contiguous in the original order, but duplicates can appear within a run. For example, in `{1,2,3,3,4,5}`, the longest run is `1,2,3,3,4,5` (length 6), not just `1,2,3` or `3,4,5`. In `{5,6,7,10,11,12,13}`, the longest run is `10,11,12,13` (length 4). If the vector is empty, return 0. The function must run in O(n) time and use O(1) auxiliary space.
// We solve this with a single pass through the vector, maintaining a current run length and a maximum run length. For each element after the first, check if it equals the previous element plus 1. If yes, increment the current run length; otherwise, reset it to 1 (since the element itself starts a new run). After each update, update the maximum. Edge cases: empty vector returns 0; a single element returns 1; duplicate consecutive equal values break the run because they are not equal to prev+1. Note that floats or negative numbers work fine as long as they are integers. Time complexity is O(n) because we iterate once. Space complexity is O(1) because we only store a few integer variables.
#include <vector>
#include <algorithm>

// Returns the length of the longest contiguous run where each element
// is equal to the previous element plus 1.
int longestRun(const std::vector<int>& nums) {
    if (nums.empty()) {
        return 0;
    }

    int maxRun = 1;
    int currentRun = 1;

    for (std::size_t i = 1; i < nums.size(); ++i) {
        if (nums[i] == nums[i - 1] + 1) {
            ++currentRun;
        } else {
            currentRun = 1;
        }
        maxRun = std::max(maxRun, currentRun);
    }

    return maxRun;
}
#include <cassert>
#include <vector>

int longestRun(const std::vector<int>& nums);

int main() {
    assert(longestRun({}) == 0);
    assert(longestRun({5}) == 1);
    assert(longestRun({1,2,3,3,4,5}) == 6);
    assert(longestRun({5,6,7,10,11,12,13}) == 4);
    assert(longestRun({1,1,1}) == 1);
    assert(longestRun({1,2,2,3,4,5}) == 4);
    assert(longestRun({-3,-2,-1,0,1}) == 5);
    assert(longestRun({10,20,30}) == 1);
    assert(longestRun({1,2,1,2,3}) == 3);
}
