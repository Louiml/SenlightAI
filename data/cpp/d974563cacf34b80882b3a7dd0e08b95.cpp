/*
You are given an integer `x` and an array of `n` positive integers representing available "tokens". You need to partition the tokens into the minimum number of groups, where each group must have a total value exactly equal to `x` (a token can be used in at most one group). Since tokens are indivisible, some tokens may be left unused or combined with others; the goal is to maximize the number of complete groups of sum `x` that can be formed from the tokens, and then output the total number of groups you can form (each complete group counts as 1, and if a token is leftover because no group can be completed, it is simply ignored). More formally: given `n` positive integers `a_i` and a target `x`, choose a collection of disjoint subsets of the given integers such that each subset sums to exactly `x`; maximize the number of such subsets, and return that maximum number. Note that you may leave any elements unused, and each element can be used at most once. Write a C++ function `int maxCompleteGroups(const std::vector<int>& tokens, int target)` that returns the maximum number of disjoint subsets each summing to `target`.
*/
#include <vector>
#include <queue>
#include <algorithm>
#include <functional>

// Returns the maximum number of disjoint subsets each summing to target.
int maxCompleteGroups(const std::vector<int>& tokens, int target) {
    std::vector<int> sorted = tokens;
    // Sort descending; tokens larger than target are useless.
    std::sort(sorted.begin(), sorted.end(), std::greater<int>());

    int complete = 0;
    // Max-heap of deficits (target - current partial sum) for incomplete groups.
    std::priority_queue<int> deficits;

    for (int token : sorted) {
        if (token > target) {
            // Cannot be used in any valid group.
            continue;
        }
        if (token == target) {
            // Forms a group by itself.
            ++complete;
            continue;
        }
        if (deficits.empty() || deficits.top() < token) {
            // Start a new group with this token.
            deficits.push(target - token);
        } else {
            // Fill the group with the largest current deficit.
            int deficit = deficits.top();
            deficits.pop();
            int remaining = deficit - token;
            if (remaining == 0) {
                ++complete;  // Group completed.
            } else {
                deficits.push(remaining);
            }
        }
    }
    return complete;
}
#include <cassert>
#include <vector>

// Assume maxCompleteGroups is defined above.

int main() {
    // Single token equal to target.
    assert(maxCompleteGroups({5}, 5) == 1);
    // Single token smaller than target.
    assert(maxCompleteGroups({3}, 5) == 0);
    // Single token larger than target.
    assert(maxCompleteGroups({7}, 5) == 0);
    // Simple two tokens summing to target.
    assert(maxCompleteGroups({2, 3}, 5) == 1);
    // Multiple groups possible.
    assert(maxCompleteGroups({1, 1, 1, 2, 2}, 3) == 2); // {1,2} and {1,2}
    // Tokens with duplicates and leftover.
    assert(maxCompleteGroups({1, 1, 1, 1, 4}, 5) == 1); // {1,4}, others unused
    // All tokens sum to multiple groups.
    assert(maxCompleteGroups({2, 2, 2, 2, 3, 3}, 4) == 2); // {2,2} and {2,2}
    // Targets with larger tokens that cannot be used.
    assert(maxCompleteGroups({10, 2, 2, 2}, 4) == 1); // only one {2,2}
    // Empty input.
    assert(maxCompleteGroups({}, 5) == 0);
    // Mixed large and small.
    assert(maxCompleteGroups({8, 1, 1, 1, 2, 2, 3}, 5) == 2); // {3,2} and {1,1,1? no, 1+1+1=3 not 5, so {2,3} and? actually {2,2,1}=5 and {1,? no} – let's check: tokens: 8(skip),1,1,1,2,2,3. Greedy: sort desc: 8,3,2,2,1,1,1. 8 skip. 3 -> start {2} deficit. 2 -> fill deficit -> complete {3,2} (ans=1). next 2 -> start deficit 2. next 1 -> deficit 2 >=1, pop -> remaining 1 push. next 1 -> pop 1 -> remaining 0 complete (ans=2). next 1 -> start deficit 4. end. ans=2.
    assert(maxCompleteGroups({8, 1, 1, 1, 2, 2, 3}, 5) == 2);
    // Test with a case where greedy works: many ones and one target.
    assert(maxCompleteGroups({1, 1, 1, 1, 1, 5}, 5) == 2); // {5} and {1+1+1+1+1}
    return 0;
}
// The solution uses a greedy approach with a max-heap. First, sort the tokens in descending order. Then iterate through them. If a token equals `target`, it forms a complete group immediately (increment answer). Otherwise, we maintain a max-heap storing "deficits" (target minus the current partial sum of tokens already chosen for some incomplete group). For each token smaller than `target`:
// - If the heap is empty or the largest deficit is strictly smaller than the current token, then starting a new group with this token is beneficial: push (target - token) into the heap.
// - Else, the largest deficit is at least as large as the current token, meaning we can use this token to fill the group that currently requires the most value (the largest remaining need). Pop that deficit, subtract the token from it: if the resulting deficit is zero, we completed a group (increment answer); otherwise push the new positive deficit back.
//
// This greedy works because using the current largest token to reduce the largest current deficit minimizes waste and maximizes completed groups. Edge cases: tokens equal to `target` are handled directly; tokens larger than `target` can never be part of a group, so they are ignored (but the function should handle them gracefully, e.g., skip them). If a token is larger than target, it cannot be used, so we skip it entirely. Time complexity is O(n log n) for sorting plus O(n log n) for heap operations = O(n log n). Space complexity is O(n) for the heap.
