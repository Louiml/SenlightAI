/*
You are given the results of `k` gymnastics practice sessions, each listing all `n` cows in the order they finished (first place to last place). A pair of cows `(a, b)` is considered "consistent" if in every session, cow `a` placed before cow `b`. Write a C++ function `int countConsistentPairs(int k, int n, const std::vector<std::vector<int>>& sessions)` that returns the number of consistent pairs of cows. The cows are labeled with distinct integers from `1` to `n` (inclusive) in each session. The input follows the USACO "Gymnastics" problem format: the first session is given first, then the second, etc. There are no ties; each session is a permutation of `1..n`. The function must handle `k >= 1`, `n >= 2`. Return the count of unordered pairs? In the original USACO problem, the order matters (i.e., `(a,b)` where `a` finishes before `b` in all sessions). The count should be for such directed pairs, considering each ordered pair exactly once. For example, if `n=3` and in all sessions cow 1 is before cow 2, then pair `(1,2)` counts, but `(2,1)` does not.
*/
#include <vector>
#include <unordered_map>

// Count ordered pairs (a,b) such that a is before b in every session.
int countConsistentPairs(int k, int n, const std::vector<std::vector<int>>& sessions) {
    // Precompute position of each cow for each session.
    // rank[session][cow] = 0-based position in that session.
    std::vector<std::vector<int>> rank(k, std::vector<int>(n + 1)); // cows are 1..n
    for (int s = 0; s < k; ++s) {
        for (int pos = 0; pos < n; ++pos) {
            int cow = sessions[s][pos];
            rank[s][cow] = pos;
        }
    }

    int result = 0;
    // Check every ordered pair (a,b) with a != b.
    for (int a = 1; a <= n; ++a) {
        for (int b = 1; b <= n; ++b) {
            if (a == b) continue;
            bool consistent = true;
            for (int s = 0; s < k; ++s) {
                if (rank[s][a] > rank[s][b]) {
                    consistent = false;
                    break;
                }
            }
            if (consistent) ++result;
        }
    }
    return result;
}
#include <cassert>
#include <vector>

int main() {
    // Example from USACO gymnastics: k=3, n=4
    // Sessions: [4,1,2,3] [3,4,1,2] [2,3,4,1]
    // Consistent pairs: (1,2), (2,3), (3,4) -> 3
    std::vector<std::vector<int>> sessions1 = {{4,1,2,3}, {3,4,1,2}, {2,3,4,1}};
    assert(countConsistentPairs(3, 4, sessions1) == 3);

    // k=1, n=2: Any pair with first before second is consistent -> exactly (1,2) if session is [1,2]
    std::vector<std::vector<int>> sessions2 = {{1,2}};
    assert(countConsistentPairs(1, 2, sessions2) == 1);

    // k=2, n=2: sessions [[1,2],[2,1]] -> no consistent pair because order flips
    std::vector<std::vector<int>> sessions3 = {{1,2}, {2,1}};
    assert(countConsistentPairs(2, 2, sessions3) == 0);

    // k=2, n=3: sessions [[1,2,3],[1,3,2]] -> pairs: (1,2),(1,3) consistent; (2,3) not because in second session 3 before 2; total 2
    std::vector<std::vector<int>> sessions4 = {{1,2,3}, {1,3,2}};
    assert(countConsistentPairs(2, 3, sessions4) == 2);

    // k=3, n=3: all same order -> all ordered pairs (3*2=6) consistent
    std::vector<std::vector<int>> sessions5 = {{1,2,3}, {1,2,3}, {1,2,3}};
    assert(countConsistentPairs(3, 3, sessions5) == 6);

    // Large n=5, k=1: every ordered pair where first before second in that single session is consistent -> n*(n-1)/2 = 10
    std::vector<std::vector<int>> sessions6 = {{5,4,3,2,1}}; // descending order
    // All pairs (a,b) where a appears before b in descending list. Only pairs like (5,4),(5,3),...,(4,3),... -> exactly 10
    assert(countConsistentPairs(1, 5, sessions6) == 10);
}
// The main idea is to compare every ordered pair of cows `(a,b)` across all sessions. For each session, record the relative order: if cow `a` appears before cow `b` in that session, that session supports pair `(a,b)`. A pair is consistent if all `k` sessions support it. A naive approach would be to loop over all pairs and all sessions, giving O(k * n^2) time, which is acceptable for typical constraints (e.g., n up to 20, k up to 10). However, we can optimize by preprocessing each session into a rank array: for each cow, store its position (0-indexed). Then for a pair `(a,b)`, checking one session is O(1) by comparing ranks. So the algorithm is: for each session, build a `position` array where `position[cow]` = index in that session. Then iterate over all ordered pairs `(a,b)` with `a != b`, and for each pair, count how many sessions have `position[a] < position[b]`. If that count equals `k`, increment the result. Edge cases: when `n=2`, only two possible ordered pairs, check both. Time complexity: O(k * n) to build position arrays + O(n^2 * k) to evaluate pairs, but since n is small it's fine. Space complexity: O(k * n) for storing sessions, or O(n) per session if we process on the fly, but we store rank matrices for simplicity. We can also use a map similar to the snippet, but rank arrays are simpler and avoid hash overhead.
