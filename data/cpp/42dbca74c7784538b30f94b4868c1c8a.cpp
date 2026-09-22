// Given a sequence of non-negative integers representing the heights of consecutive mountains, write a C++ function `int longestNonIncreasingRun(const std::vector<int>& h)` that returns the length (number of edges) of the longest contiguous segment where each height is greater than or equal to the next height. If the entire sequence has length less than 2 or has no such segment longer than 0, return 0. For example, for heights `[1, 3, 2, 2, 1, 4, 5, 0]`, the longest run is from indices 1 through 4 (`3 ≥ 2 ≥ 2 ≥ 1`), which has 3 edges, so the answer is 3. The input vector is non-empty and contains only non-negative integers.

#include <cassert>
#include <vector>

int longestNonIncreasingRun(const std::vector<int>& h);

int main() {
    assert(longestNonIncreasingRun({1, 3, 2, 2, 1, 4, 5, 0}) == 3);
    assert(longestNonIncreasingRun({5, 4, 3, 2, 1}) == 4);
    assert(longestNonIncreasingRun({1, 2, 3, 4}) == 0);
    assert(longestNonIncreasingRun({1}) == 0);
    assert(longestNonIncreasingRun({7, 7, 7}) == 2);
    assert(longestNonIncreasingRun({2, 1, 3, 2, 1, 0, 9, 8}) == 3);
    assert(longestNonIncreasingRun({10, 9, 8, 7, 6, 5, 4, 3, 2, 1}) == 9);
    assert(longestNonIncreasingRun({0, 0, 1, 1, 0}) == 1);
    return 0;
}

#include <vector>
#include <algorithm>

// Returns the length of the longest contiguous non-increasing run (number of edges).
int longestNonIncreasingRun(const std::vector<int>& h) {
    if (h.size() < 2) {
        return 0;
    }

    int ans = 0;
    int temp = 0;

    for (size_t i = 0; i + 1 < h.size(); ++i) {
        if (h[i] >= h[i + 1]) {
            ++temp;
        } else {
            ans = std::max(ans, temp);
            temp = 0;
        }
    }

    // Handle the case where the last run extends to the end.
    ans = std::max(ans, temp);

    return ans;
}

// The problem is a straightforward single-pass linear scan. Initialize a counter `temp` to 0 and a result variable `ans` to 0. Iterate through the vector from index 0 to `n-2` (since we compare adjacent pairs). For each pair `(h[i], h[i+1])`, if `h[i] >= h[i+1]`, increment `temp`; otherwise, the current run ends, so update `ans` with the maximum of `ans` and `temp`, then reset `temp` to 0. After the loop, there is one edge case: if the last run extends to the end (i.e., `h[n-2] >= h[n-1]`), then `temp` was never flushed inside the loop, so we must update `ans` again after the loop. Alternatively, you can avoid the edge case by appending a sentinel value like `-1` (since heights are non-negative) to the vector copy and iterating to the end. Time complexity is O(n), where n is the number of heights, and space complexity is O(1) beyond the input vector.
