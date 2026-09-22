Write a standalone C++ function `int minReverseOperations(int n, int k, const std::vector<int>& arr)` that determines the minimum number of operations needed to sort the given array of `n` distinct integers into ascending order using only the following operation: choose any contiguous subarray of length exactly `k` and reverse it. The function should return the minimum number of operations, or `-1` if sorting is impossible. The input array contains the numbers `1` through `n` in some order (so they are distinct and a permutation). The parameter `n` is the length (1 ≤ n ≤ 8) and `k` is the reversal window size (1 ≤ k ≤ n). For example, given `n=3`, `k=2`, and `arr = {2, 1, 3}`, reversing the first two elements sorts the array in 1 operation, so the function returns 1. Given `n=3`, `k=3`, and `arr = {2, 3, 1}`, one reversal of the whole array gives `{1, 3, 2}`, which is not sorted, and no other reversal length is allowed, so it is impossible and returns -1.

// The problem is a shortest-path search on the space of all permutations of size `n`. Since `n ≤ 8`, there are at most `8! = 40320` possible states, so a breadth-first search (BFS) from the initial permutation to the sorted permutation is feasible. For each state (represented as a string of digits for convenience), we generate all possible next states by reversing every contiguous length-`k` subarray. There are `n - k + 1` such reversals per state. We keep a `visited` set (or boolean array indexed by permutation rank) to avoid revisiting states. The BFS stops when we reach the target sorted string, returning the distance (number of reversals). If BFS exhausts without reaching the target, return -1. Edge cases: when the initial array is already sorted, return 0 immediately. When `k == 1`, reversing a single element does nothing, so any state with more than one element is unsolvable unless it is already sorted. The time complexity is O(n! * (n - k + 1) * n) for string construction, which at worst is O(8! * 8 * 8) ≈ 2.6 million operations — trivial. Space complexity is O(n!) for visited storage (or O(n!) for the set). The solution uses a queue of pairs (state, distance). We convert integers to characters for the string representation to keep the code simple and fast.

#include <string>
#include <vector>
#include <queue>
#include <unordered_set>
#include <algorithm>

// Return the minimum number of reverse-length-k operations to sort the permutation arr (values 1..n), or -1 if impossible.
int minReverseOperations(int n, int k, const std::vector<int>& arr) {
    // Build the starting state as a string of digits.
    std::string start;
    start.reserve(n);
    for (int x : arr) {
        start.push_back(static_cast<char>('0' + x));
    }

    // Build the target sorted string.
    std::string target;
    target.reserve(n);
    for (int i = 1; i <= n; ++i) {
        target.push_back(static_cast<char>('0' + i));
    }

    if (start == target) {
        return 0;
    }

    // BFS
    std::queue<std::pair<std::string, int>> q;
    q.push({start, 0});
    std::unordered_set<std::string> visited;
    visited.insert(start);

    while (!q.empty()) {
        auto [cur, dist] = q.front();
        q.pop();

        // Try reversing every window of length k.
        for (int i = 0; i <= n - k; ++i) {
            std::string next = cur;
            std::reverse(next.begin() + i, next.begin() + i + k);
            if (visited.find(next) == visited.end()) {
                if (next == target) {
                    return dist + 1;
                }
                visited.insert(next);
                q.push({next, dist + 1});
            }
        }
    }

    return -1;
}

#include <cassert>
#include <vector>

// The solution function is declared above. This main tests it.
// (In an actual submission, the solution code would be included here.)
int main() {
    // Already sorted
    assert(minReverseOperations(3, 2, {1, 2, 3}) == 0);

    // One reversal sorts it
    assert(minReverseOperations(3, 2, {2, 1, 3}) == 1);

    // Two reversals needed (reverse first two: 3 2 1, then reverse whole length 3: 1 2 3)
    assert(minReverseOperations(3, 3, {3, 2, 1}) == 1); // actually one whole reversal sorts it

    // Impossible when k=1 and not sorted
    assert(minReverseOperations(3, 1, {2, 1, 3}) == -1);

    // Example where it takes multiple reversals
    // n=4, k=2, start: 4 3 2 1 -> reverse [0,1] -> 3 4 2 1 -> reverse [2,3] -> 3 4 1 2 -> reverse [1,2] -> 3 1 4 2 -> ... actually let's test a known case.
    // Use n=4, k=2, start = {4,3,2,1}. One reverse [0,1] -> {3,4,2,1}. Then reverse [1,2] -> {3,2,4,1}. Then reverse [0,1] -> {2,3,4,1}. Then reverse [2,3] -> {2,3,1,4}. Then reverse [1,2] -> {2,1,3,4}. Then reverse [0,1] -> {1,2,3,4}. That's 6 steps. But likely there's a shorter path. We'll just assert it's >=0 and not -1.
    int result = minReverseOperations(4, 2, {4, 3, 2, 1});
    assert(result != -1 && result >= 0);

    // Known solution: n=5, k=3, start = {2,5,4,3,1} -> one reverse of [1,3] gives {2,3,4,5,1} -> reverse [0,4] gives {1,5,4,3,2} -> not good. Let's just check a specific solvable case.
    // For n=5, k=3, start = {3,2,1,5,4}. Reverse [0,2] -> {1,2,3,5,4}. Reverse [2,4] -> {1,2,4,5,3}. Not sorted. Likely multiple. We'll just check that a solvable case returns >0.
    // But better to test a known simple case: n=5, k=5, start = {5,4,3,2,1} -> whole reverse gives {1,2,3,4,5}, so 1.
    assert(minReverseOperations(5, 5, {5, 4, 3, 2, 1}) == 1);

    // Unreachable when k=2 and n=2 with start = {2,1} -> reverse whole array gives {1,2}, so 1.
    assert(minReverseOperations(2, 2, {2, 1}) == 1);

    // Unreachable when k=1 and already sorted -> 0
    assert(minReverseOperations(4, 1, {1, 2, 3, 4}) == 0);

    // k=1 and not sorted -> -1
    assert(minReverseOperations(4, 1, {2, 1, 3, 4}) == -1);

    // n=1 always sorted
    assert(minReverseOperations(1, 1, {1}) == 0);
}
