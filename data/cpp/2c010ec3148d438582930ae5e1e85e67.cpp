Write a C++ function `bool isAlwaysPlayable(int n, int k, const std::vector<std::pair<int,int>>& intervals)` that determines whether a given number `k` appears in every position necessary for a game described by `n` intervals. Specifically, the game is playable if and only if `k` appears as the left endpoint of at least one interval **and** as the right endpoint of at least one interval. The input consists of an integer `n` (number of intervals), an integer `k` (the critical value), and a vector of `n` pairs `(l, r)` representing intervals. Return `true` if both conditions hold, otherwise `false`. The function must not rely on global variables, use only standard library facilities, and be safe for all integer values (including negatives).

// The core observation is that we only need to check two independent facts across all intervals: does any interval have `l == k`, and does any interval have `r == k`? We can iterate through the vector once, maintaining two boolean flags (or counters, but booleans suffice). If both flags become true by the end, return `true`; otherwise return `false`. Edge cases include: `n` can be zero (then both flags are false, so result is false), intervals can have `l > r` or `l == r` (no special handling needed), and `k` may not appear at all. The algorithm runs in `O(n)` time and uses `O(1)` auxiliary space (excluding the input vector). No sorting or preprocessing is needed, and integer overflow is not a concern since we only compare values.

#include <vector>
#include <utility>

// Determine if a game with given intervals and critical value k is playable.
// Playable iff k appears as a left endpoint in some interval and as a right endpoint in some interval.
bool isAlwaysPlayable(int n, int k, const std::vector<std::pair<int,int>>& intervals) {
    bool hasAsLeft = false;
    bool hasAsRight = false;
    
    for (int i = 0; i < n; ++i) {
        if (intervals[i].first == k) {
            hasAsLeft = true;
        }
        if (intervals[i].second == k) {
            hasAsRight = true;
        }
        // Early exit if both found
        if (hasAsLeft && hasAsRight) {
            return true;
        }
    }
    return hasAsLeft && hasAsRight;
}

#include <cassert>
#include <vector>
#include <utility>

int main() {
    // Test 1: Basic case where k appears on both sides
    assert(isAlwaysPlayable(3, 5, {{5, 1}, {2, 5}, {3, 4}}) == true);
    // Test 2: k appears only as left endpoint
    assert(isAlwaysPlayable(2, 5, {{5, 1}, {2, 3}}) == false);
    // Test 3: k appears only as right endpoint
    assert(isAlwaysPlayable(2, 5, {{1, 5}, {4, 6}}) == false);
    // Test 4: k does not appear at all
    assert(isAlwaysPlayable(2, 7, {{1, 2}, {3, 4}}) == false);
    // Test 5: n=0 (empty intervals)
    assert(isAlwaysPlayable(0, 5, {}) == false);
    // Test 6: k appears in the same interval as both endpoints
    assert(isAlwaysPlayable(1, 9, {{9, 9}}) == true);
    // Test 7: negative values and k negative
    assert(isAlwaysPlayable(3, -3, {{-3, 1}, {2, -3}, {0, 0}}) == true);
    // Test 8: Many intervals but only one side matches
    assert(isAlwaysPlayable(100, 42, std::vector<std::pair<int,int>>(100, {42, 1})) == false);
    // Test 9: First interval matches both, early exit works
    assert(isAlwaysPlayable(5, 10, {{10, 10}, {1, 2}, {3, 4}, {5, 6}, {7, 8}}) == true);
    // Test 10: k appears but not at all in one side, large n
    assert(isAlwaysPlayable(500, 3, [](){
        std::vector<std::pair<int,int>> intervals;
        for (int i = 0; i < 250; ++i) intervals.push_back({3, i});
        for (int i = 0; i < 250; ++i) intervals.push_back({i, i + 100});
        return intervals;
    }()) == false);  // only left side has 3
    return 0;
}
