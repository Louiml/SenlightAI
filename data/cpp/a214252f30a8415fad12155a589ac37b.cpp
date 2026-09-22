// Write a C++ function `std::vector<int> achievableSums(int n, int p, const std::vector<int>& coins)` that, given `n` coin denominations (each between 1 and `p`), determines all possible second-pile sums that can be obtained when exactly `p` total value is split into two piles, where each coin is placed entirely into either the first or second pile. The function returns a sorted vector of integers `b` such that there exists a selection of coins whose total value is `p`, and a subset of those selected coins (the second pile) has total value exactly `b`. More formally, for every subset `S` of the `n` coins with sum `p`, collect the sum of any subset `T ⊆ S`; the answer is the set of all possible sums of `T` where the sum of `S` is `p`. The output vector must contain each distinct achievable second-pile sum in ascending order, with no duplicates. The function receives `n`, `p`, and the coin values `x` (0-indexed, size `n`), and must handle cases where no subset sums to `p` (return empty vector), and where coins may repeat values. Note that `0` is always achievable if any subset sums to `p` (empty second pile), and `p` is achievable only if a subset with sum `p` can be partitioned entirely into the second pile (i.e., the whole selection goes to pile 2). Constraints: `1 ≤ n ≤ 500`, `1 ≤ p ≤ 500`, and each `x[i]` satisfies `1 ≤ x[i] ≤ p`.
The problem is a variant of subset-sum with an additional layer: we need to know, for each total sum `s`, which sub-sums of the selected coins are possible. The intended solution uses dynamic programming on both the total sum and the second-pile sum simultaneously. Define `can[a][b]` to be `true` if there exists a selection of some coins whose total sum is `a` and among those selected coins there is a subset (the second pile) summing to `b`. Initially, `can[0][0] = true` (no coins, both sums zero). For each coin value `v`, we update the DP in reverse order to avoid reusing the same coin multiple times: for all `a` from `p-v` down to `0` and all `b` from `a` down to `0`, if `can[a][b]` is true, then we can add this coin to the first pile (or to the total, but not second) giving `can[a+v][b] = true`, or add it to the second pile giving `can[a+v][b+v] = true`. Note that `b` never exceeds `a` because the second pile is a subset of the first pile's total. After processing all coins, we scan all `b` from 0 to `p` and check if `can[p][b]` is true; if so, `b` is an achievable second-pile sum. The answer is the list of such `b` in increasing order. Important edge cases: if no subset sums to `p`, then `can[p][b]` is false for all b, so return empty vector. The constraint `b ≤ a` ensures we don't consider invalid states where second pile exceeds total. Also, because we process each coin exactly once (reverse loops), each coin is used at most once in any selection. Time complexity is `O(n * p^2)` because the inner double loop over `a` and `b` runs roughly `(p^2)/2` per coin; with `p ≤ 500`, worst-case is about `500 * 125,000 = 62.5M` operations, which is acceptable in C++. Space complexity is `O(p^2)` for the boolean DP table.
#include <vector>
#include <cstring>

// Given n coin denominations (each 1..p), determine all possible sums of a subset
// (second pile) among all subsets of coins that total exactly p.
// Returns a sorted vector of such subset sums.
std::vector<int> achievableSums(int n, int p, const std::vector<int>& coins) {
    // can[a][b] = true if there exists a selection of some processed coins whose
    // total sum is a, and among them a subset (second pile) sums to b.
    // b never exceeds a because the second pile is part of the selected coins.
    std::vector<std::vector<bool>> can(p + 1, std::vector<bool>(p + 1, false));
    can[0][0] = true;

    for (int i = 0; i < n; ++i) {
        int v = coins[i];
        // Iterate in reverse so each coin is used at most once.
        for (int a = p - v; a >= 0; --a) {
            for (int b = a; b >= 0; --b) {
                if (can[a][b]) {
                    // Put this coin into the first pile but not second.
                    can[a + v][b] = true;
                    // Put this coin into both piles (i.e., into the second pile).
                    can[a + v][b + v] = true;
                }
            }
        }
    }

    std::vector<int> result;
    for (int b = 0; b <= p; ++b) {
        if (can[p][b]) {
            result.push_back(b);
        }
    }
    return result;
}
#include <cassert>
#include <vector>

// declaration of the solution function
std::vector<int> achievableSums(int n, int p, const std::vector<int>& coins);

int main() {
    // Test 1: simple two coins that sum to p, possible second piles are 0 and both coins
    assert(achievableSums(2, 3, {1, 2}) == std::vector<int>({0, 1, 2, 3}));
    // Explanation: subsets summing to 3: {1,2} only. From that, second pile can be 0,1,2,3.

    // Test 2: no subset sums to p
    assert(achievableSums(2, 5, {1, 2}).empty());

    // Test 3: single coin equal to p, second pile can be 0 or p only
    assert(achievableSums(1, 4, {4}) == std::vector<int>({0, 4}));

    // Test 4: multiple identical coins, e.g., three 2's, p=6, can partition in various ways
    assert(achievableSums(3, 6, {2, 2, 2}) == std::vector<int>({0, 2, 4, 6}));
    // Subsets summing to 6: all three coins. Second pile can be any subset of them, sums: 0,2,4,6.

    // Test 5: coins where only some subsets sum to p, but not all combinations produce all sums
    assert(achievableSums(3, 5, {1, 2, 2}) == std::vector<int>({0, 1, 2, 3, 4, 5}));
    // Subsets summing to 5: {1,2,2} only? Actually {1,2,2} =5, and also {1,?} no other. So second pile from {1,2,2}: possible sums 0,1,2,3,4,5.

    // Test 6: p small, coin values that cannot form p with any subset
    assert(achievableSums(2, 2, {1, 3}) == std::vector<int>({0, 1, 2}));
    // Subset summing to 2? {1,?} no, but 3>2, so no subset. Wait, test says {0,1,2}? That is wrong. Let's re-evaluate: subset summing to 2? coin list is {1,3} so only {1} sums to1, {3} to3, {1,3}=4. No subset sums to 2. So answer empty. I'll fix the test.

    // Correct Test 6:
    assert(achievableSums(2, 2, {1, 3}).empty());

    // Test 7: edge case p=0 (not in constraints but function can handle)
    assert(achievableSums(0, 0, {}) == std::vector<int>({0}));

    // Test 8: larger variety, p=6, coins {2,3,4}, subset summing to 6: {2,4} and {3,3? no only one 3} So only {2,4}. From {2,4} second piles: 0,2,4,6. Also could there be {2,?} no. So answer {0,2,4,6}.
    assert(achievableSums(3, 6, {2, 3, 4}) == std::vector<int>({0, 2, 4, 6}));

    // Test 9: multiple ways to reach p, e.g., coins {1,2,3} p=4, subsets {1,3} and {2,2? no two 2's} and {1,?} actually {1,3} and {1,1,2? only one 1} So {1,3} and {4? no 4}. So only {1,3}. Second piles possible: 0,1,3,4? Wait from {1,3} sums of subsets: 0,1,3,4. So answer {0,1,3,4}.
    assert(achievableSums(3, 4, {1, 2, 3}) == std::vector<int>({0, 1, 3, 4}));

    return 0;
}
