Write a C++ function named `countRecentRequests` that simulates a `RecentCounter` class. The function should take a vector of integer timestamps (in milliseconds, strictly increasing) and return a vector of integers where the i-th element is the number of requests that occurred within the last 3000 milliseconds up to and including the i-th timestamp. In other words, for each timestamp `t` in the input, count how many timestamps `x` (including `t` itself) satisfy `t - 3000 <= x <= t`. The first timestamp is guaranteed to be at least 1, and all timestamps are between 1 and 10^9 inclusive. The input vector will contain between 1 and 10000 elements. The function must be efficient enough to handle up to 10000 calls.

The core problem is maintaining a sliding window of recent timestamps. Since the input timestamps are strictly increasing, we can use a queue to store timestamps in chronological order. For each new timestamp `t`, we remove from the front of the queue any timestamps that are older than `t - 3000` (i.e., where `front < t - 3000`). After removing expired timestamps, we push `t` into the queue. The current number of valid requests is simply the size of the queue. This works because the queue always contains exactly the timestamps in the valid window `[t - 3000, t]`. Since each timestamp is pushed once and popped at most once, the total time complexity is O(n) for n timestamps. The space complexity is O(n) in the worst case when all timestamps are within a 3000 ms window. Edge cases: when the first timestamp arrives, the queue is empty, so we push it and return 1. When timestamps are equal (though the problem states strictly increasing, but we handle gracefully), the condition `front < t - 3000` correctly keeps all timestamps that are exactly `t - 3000` or greater. We must use a deque or queue, and the logic is identical to the provided snippet.

#include <vector>
#include <queue>

// Count the number of recent requests within a 3000 ms sliding window.
// Input timestamps are strictly increasing.
std::vector<int> countRecentRequests(const std::vector<int>& timestamps) {
    std::vector<int> result;
    std::queue<int> recent;

    for (int t : timestamps) {
        while (!recent.empty() && recent.front() < t - 3000) {
            recent.pop();
        }
        recent.push(t);
        result.push_back(static_cast<int>(recent.size()));
    }

    return result;
}

#include <cassert>
#include <vector>

// Assume the solution is included above.

int main() {
    // Example from the prompt
    std::vector<int> input1 = {1, 100, 3001, 3002};
    std::vector<int> expected1 = {1, 2, 3, 3};
    assert(countRecentRequests(input1) == expected1);

    // Single request
    assert(countRecentRequests({5}) == std::vector<int>{1});

    // All requests within 3000 ms of each other
    std::vector<int> input2 = {100, 200, 300, 400};
    std::vector<int> expected2 = {1, 2, 3, 4};
    assert(countRecentRequests(input2) == expected2);

    // Requests spread far apart
    std::vector<int> input3 = {1, 4000, 8000, 12000};
    std::vector<int> expected3 = {1, 1, 1, 1};
    assert(countRecentRequests(input3) == expected3);

    // Boundary: exactly 3000 difference keeps both
    std::vector<int> input4 = {1000, 4000};
    std::vector<int> expected4 = {1, 2};
    assert(countRecentRequests(input4) == expected4);

    // Boundary: 3001 difference drops the older one
    std::vector<int> input5 = {1000, 4001};
    std::vector<int> expected5 = {1, 1};
    assert(countRecentRequests(input5) == expected5);

    // Large timestamps and many calls (simulate a small case for testing)
    std::vector<int> input6 = {1000000000 - 4000, 1000000000 - 2000, 1000000000};
    std::vector<int> expected6 = {1, 2, 3};
    assert(countRecentRequests(input6) == expected6);

    // Multiple requests with some dropping off over time
    std::vector<int> input7 = {10, 20, 30, 40, 50, 3060, 3070, 11000};
    std::vector<int> expected7 = {1, 2, 3, 4, 5, 1, 2, 1};
    assert(countRecentRequests(input7) == expected7);

    // Duplicate timestamps (though problem says strictly increasing, test robustness)
    std::vector<int> input8 = {100, 100, 100};
    std::vector<int> expected8 = {1, 2, 3};
    assert(countRecentRequests(input8) == expected8);

    return 0;
}
