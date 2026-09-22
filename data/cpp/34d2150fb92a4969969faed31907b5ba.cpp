Write a C++ function `constructDistancedSequence(int n)` that, for a given positive integer `n`, returns a vector of size `2*n - 1` containing each number from `1` to `n` such that: the number `1` appears exactly once; every number `k` (where `2 ≤ k ≤ n`) appears exactly twice, and the two occurrences of `k` are exactly `k` positions apart in the sequence (i.e., if the first occurrence is at index `i`, the second is at index `i + k`). The resulting sequence must be the lexicographically largest such sequence. For example, for `n = 3`, the valid sequence `[3,1,2,3,2]` is the answer (since `2` appears at indices 2 and 4 with distance 2, and `3` at indices 0 and 3 with distance 3). If multiple valid sequences exist, return the one that is lexicographically largest (compared element-wise). Assume `n` is between 1 and 20 inclusive. The function should return a `std::vector<int>`.

// The problem is a classic backtracking problem. We need to construct a sequence of length `L = 2*n - 1`. We fill the positions from left to right, always trying the largest possible number first to guarantee lexicographic maximality. The algorithm uses a recursive helper that processes an index `i`:
// - If `i` equals `L`, we have filled all positions — success.
// - If `result[i]` is already filled (not -1), skip to the next index.
// - Otherwise, try numbers from `n` down to `1`. For each unused number:
//   - Mark it as used and place it at `result[i]`.
//   - If the number is `1`, it can be placed only once; then recurse on `i+1`.
//   - If the number is `k > 1`, we need to place its second occurrence at `j = i + k`. This is valid only if `j < L` and `result[j] == -1`. If valid, place `k` at `j`, recurse on `i+1`, and if recursion fails, undo both placements.
//   - If recursion succeeds, return true; otherwise, undo the current placement and try the next number.
// The base case is when all positions are filled. Since we always try numbers in descending order, the first successful complete sequence is lexicographically largest. Edge cases: `n = 1` returns `[1]`; for even and odd `n`, the pattern always works because the length `2n-1` guarantees enough space for placing pairs. The backtracking prunes invalid placements early. Time complexity is exponential in the worst case, but for `n ≤ 20`, the search space is heavily pruned and works within reasonable time. Space complexity is `O(n)` for the result and used arrays, plus recursion depth of `O(n)`.

#include <vector>
#include <functional>

// Builds a vector of size 2*n-1 where each k in [2..n] appears twice with distance k,
// and 1 appears once, producing the lexicographically largest such sequence.
std::vector<int> constructDistancedSequence(int n) {
    const int length = 2 * n - 1;
    std::vector<int> result(length, -1);
    std::vector<bool> used(n + 1, false);

    // Backtracking helper; returns true if a complete valid sequence is built.
    std::function<bool(int)> solve = [&](int i) -> bool {
        if (i >= length) {
            return true; // All positions filled
        }
        if (result[i] != -1) {
            return solve(i + 1); // Position already occupied
        }
        // Try largest numbers first for lexicographic maximality
        for (int num = n; num >= 1; --num) {
            if (used[num]) continue;

            used[num] = true;
            result[i] = num;

            if (num == 1) {
                // Number 1 appears only once, recurse to next index
                if (solve(i + 1)) {
                    return true;
                }
            } else {
                int j = i + num; // second occurrence position
                if (j < length && result[j] == -1) {
                    result[j] = num;
                    if (solve(i + 1)) {
                        return true;
                    }
                    result[j] = -1; // undo second placement
                }
            }
            // Undo current placement and try next number
            used[num] = false;
            result[i] = -1;
        }
        return false; // No valid number found, backtrack
    };

    solve(0);
    return result;
}

#include <cassert>
#include <vector>

// The solution function is assumed to be declared above.
int main() {
    // Basic examples
    std::vector<int> res1 = constructDistancedSequence(1);
    assert(res1 == std::vector<int>({1}));

    std::vector<int> res2 = constructDistancedSequence(2);
    assert(res2 == std::vector<int>({2, 1, 2})); // distance 2 for 2

    std::vector<int> res3 = constructDistancedSequence(3);
    assert(res3 == std::vector<int>({3, 1, 2, 3, 2}));

    // Lexicographically largest for n=4
    std::vector<int> res4 = constructDistancedSequence(4);
    assert(res4 == std::vector<int>({4, 2, 3, 2, 4, 3, 1}));

    // Check n=5: known largest is {5,3,1,4,3,5,2,4,2}
    std::vector<int> res5 = constructDistancedSequence(5);
    assert(res5 == std::vector<int>({5, 3, 1, 4, 3, 5, 2, 4, 2}));

    // Verify correctness for n=6 by checking properties
    std::vector<int> res6 = constructDistancedSequence(6);
    assert(res6.size() == 11);
    // Check for each k, distance property holds
    for (int k = 2; k <= 6; ++k) {
        int first = -1, second = -1;
        for (int i = 0; i < (int)res6.size(); ++i) {
            if (res6[i] == k) {
                if (first == -1) first = i;
                else second = i;
            }
        }
        assert(second - first == k);
        assert(first != -1 && second != -1);
    }
    // Check 1 appears exactly once
    int count1 = 0;
    for (int v : res6) if (v == 1) ++count1;
    assert(count1 == 1);

    // Ensure lexicographically largest for n=7 is as expected
    std::vector<int> res7 = constructDistancedSequence(7);
    assert(res7 == std::vector<int>({7, 5, 3, 6, 4, 3, 5, 7, 4, 6, 2, 1, 2}));

    return 0;
}
