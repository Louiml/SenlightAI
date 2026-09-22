Write a C++ function `countActiveSessions` that takes an integer `n` representing the number of timestamped messages, an integer `c` representing a session timeout threshold, and a vector of `n` strictly non-decreasing integer timestamps (each message's arrival time). The function must simulate a session-based system where a new session starts whenever the gap between two consecutive messages exceeds `c` (i.e., when `arr[i] - arr[i-1] > c`). The session that contains the last message (the most recent one) continues until the end. Determine and return the total number of messages that belong to that final (most recent) session. In other words, scan the timestamps from the end backward, counting how many consecutive messages have time differences of at most `c`, and stop counting as soon as a gap greater than `c` is encountered. The function should handle edge cases where `n` is 0 (return 0) or `n` is 1 (return 1), and where all gaps are within the threshold (return `n`). Assume timestamps are given in non-decreasing order (duplicates allowed, gaps of 0 are valid). Provide a self-contained implementation with proper `const` correctness and no global state.

#include <cassert>
#include <vector>

// Declare the function under test (assume it's in the same translation unit)
int countActiveSessions(int n, int c, const std::vector<int>& arr);

int main() {
    // Basic case: all gaps within threshold
    assert(countActiveSessions(5, 10, {1, 3, 6, 10, 15}) == 5);
    
    // One gap exceeds threshold near the end
    assert(countActiveSessions(5, 4, {1, 2, 3, 10, 11}) == 2); // messages 10,11
    
    // Gap exceeds threshold at the very end
    assert(countActiveSessions(4, 2, {1, 2, 3, 20}) == 1); // only last message
    
    // Single element
    assert(countActiveSessions(1, 100, {5}) == 1);
    
    // Empty vector
    assert(countActiveSessions(0, 5, {}) == 0);
    
    // Duplicate timestamps (gaps of zero are allowed)
    assert(countActiveSessions(4, 3, {7, 7, 7, 8}) == 4);
    
    // Large gap at the start of the backwards scan
    assert(countActiveSessions(6, 2, {1, 100, 101, 102, 103, 104}) == 5); // 100..104
    
    // All gaps exceed threshold (only last message active)
    assert(countActiveSessions(3, 1, {1, 5, 9}) == 1);
    
    // Mixed: gap > c at index 2, then all within after that
    assert(countActiveSessions(5, 5, {1, 2, 10, 11, 12}) == 3); // 10,11,12
    
    // Large n, all gaps zero
    std::vector<int> big(1000, 3);
    assert(countActiveSessions(1000, 0, big) == 1000);
}

#include <vector>

// Counts messages in the final session, given timestamps and a timeout threshold.
// Precondition: arr is non-decreasing. n is the number of elements.
int countActiveSessions(int n, int c, const std::vector<int>& arr) {
    if (n <= 0) return 0;          // No messages
    if (n == 1) return 1;          // Single message forms its own session

    int count = 1;                 // Last message always in final session
    for (int i = n - 1; i > 0; --i) {
        if (arr[i] - arr[i-1] <= c) {
            ++count;               // Previous message still in same session
        } else {
            break;                 // Gap too large: start of a new session
        }
    }
    return count;
}

// The core idea is a backward linear scan. Since the timestamps are non-decreasing, the difference between consecutive elements is non-negative. Starting from the last message (index `n-1`), we initialize a counter `count = 1` (because the last message itself is always part of the final session). Then we iterate from `i = n-1` down to `1`. At each step, we check if `arr[i] - arr[i-1] <= c`. If true, it means the previous message belongs to the same session as the current one, so we increment the counter and continue. If false (gap > c), the previous message starts a new session, and since we are only interested in the final session, we break out of the loop. The result is the counter value. Edge cases: if `n == 0`, there are no messages, so the function must return 0 — handle this with an early check. If `n == 1`, the single message is its own session, so return 1. If all gaps are ≤ c, the loop never breaks, and the counter becomes `n`. The time complexity is O(n) in the worst case (when all gaps are small, we scan the entire array) and O(1) auxiliary space. No sorting or extra data structures are needed.
