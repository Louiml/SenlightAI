// You are given a TV series with `N` episodes, numbered from 1 to `N` in broadcast order. Each episode is identified by a positive integer (its chapter number, possibly repeated across different episodes). Define a "continuous binge" as a contiguous sequence of episodes (one or more) in which no chapter number appears more than once — that is, the sequence has all distinct chapter numbers. Write a C++ function `int maxDistinctBingeLength(int n, const std::vector<int>& chapters)` that takes the total number of episodes `n` and a vector `chapters` (where `chapters[i]` is the chapter number of episode `i+1`) and returns the length of the longest continuous binge (i.e., the maximum size of a contiguous subarray with all distinct elements). The input may contain any positive integers, including duplicates scattered anywhere. The function must handle up to 10^5 episodes efficiently.

// The problem is a classic "longest subarray with all distinct elements" sliding window problem. We maintain two pointers: `left` (start of current window) and `right` (end, iterating through episodes). We use a hash map (`std::unordered_map<int,int>`) to store the most recent position (1-indexed) where each chapter number was seen. For each new episode at position `i` (0-indexed in input, but we can think 1-indexed), if the chapter number was previously seen at a position `prevPos` that is inside the current window (i.e., `prevPos >= left`), then we must shrink the window by moving `left` to `prevPos + 1` to maintain distinctness. Otherwise, we just extend the window and update the current length. At every step, we update the maximum length seen. This is a standard O(n) time, O(n) space solution. Important edge cases: empty vector (though `n` is guaranteed positive, still handle gracefully), single episode, all distinct episodes, and all identical episodes — in the last case the longest binge is 1. The hash map stores each chapter's latest position, so duplicate checks are constant-time average.

#include <vector>
#include <unordered_map>
#include <algorithm>

// Computes the length of the longest contiguous subarray with all distinct elements.
// `chapters[i]` is the chapter number of episode i+1; n = chapters.size().
int maxDistinctBingeLength(int n, const std::vector<int>& chapters) {
    if (n == 0) return 0;
    
    std::unordered_map<int, int> lastSeen; // chapter -> most recent 1-based position
    int left = 1; // current window start (1-based)
    int maxLength = 0;
    
    for (int i = 1; i <= n; ++i) {
        int ch = chapters[i - 1];
        auto it = lastSeen.find(ch);
        if (it != lastSeen.end() && it->second >= left) {
            // Duplicate found inside current window; move left past the previous occurrence
            left = it->second + 1;
        }
        lastSeen[ch] = i;
        int currentLength = i - left + 1;
        maxLength = std::max(maxLength, currentLength);
    }
    return maxLength;
}

#include <cassert>
#include <vector>

// (The solution function is assumed to be defined above; include the necessary header in the test file.)

int main() {
    // Basic cases
    assert(maxDistinctBingeLength(1, std::vector<int>{5}) == 1);
    assert(maxDistinctBingeLength(4, std::vector<int>{1,2,3,4}) == 4);
    assert(maxDistinctBingeLength(4, std::vector<int>{1,1,1,1}) == 1);
    assert(maxDistinctBingeLength(5, std::vector<int>{1,2,1,3,4}) == 3); // longest: [2,1,3] or [1,3,4]
    assert(maxDistinctBingeLength(6, std::vector<int>{3,1,2,3,4,5}) == 5); // [1,2,3,4,5]
    
    // Repeated patterns and edge positions
    assert(maxDistinctBingeLength(5, std::vector<int>{1,2,3,2,1}) == 3); // [1,2,3] or [3,2,1]
    assert(maxDistinctBingeLength(0, std::vector<int>{}) == 0);
    
    // Larger mixed test
    std::vector<int> test = {10, 20, 10, 30, 40, 50, 30, 60};
    assert(maxDistinctBingeLength(8, test) == 5); // [10,30,40,50,60] or [20,10,30,40,50] etc.
    
    // All distinct in large size
    std::vector<int> big(100000);
    for (int i = 0; i < 100000; ++i) big[i] = i + 1;
    assert(maxDistinctBingeLength(100000, big) == 100000);
    
    return 0;
}
