// You are given an integer `n` (the number of elements in a hidden array, where all values are unknown but distinct and we can only learn information via queries) and a list of `k` non-empty subsets of indices (1-indexed). You can ask an interactive query: given a set of indices, the judge returns the maximum value among those indices. Write a self-contained C++ function `solveQuery(int n, vector<vector<int>> subsets)` that, without knowing the array values, computes for each subset the maximum value of that subset. The function may ask up to `n + k` queries (each query is made by a provided callback `int ask(const vector<int>& indices)` that returns the maximum among those indices). The function must return a vector of `k` integers, where the i-th value is the maximum of the i-th subset. You may assume that all queries are valid (non-empty index sets, indices in [1,n]), and that you can call `ask` as many times as needed but must not exceed `n + k` calls in the worst case.
// The core idea is: first, find the overall maximum of the whole array by asking `ask` on all indices [1..n]. This gives the global maximum `mx`. Then, determine the index of that maximum via binary search on the index range: keep an interval `[lo, hi]` where the maximum is known to be at some index in that interval. Repeatedly ask the left half `[lo, mid]`; if the result equals `mx`, then the maximum is in that half, so set `hi = mid`, else set `lo = mid+1`. This takes about `ceil(log2(n))` queries, but we can save queries: actually we only need to find the index of the maximum once. After we know that index `pos`, for each subset:
// - If the subset does not contain `pos`, then its maximum is `mx`.
// - If it does contain `pos`, then we ask for the maximum among all indices except those in the subset; that value is the second-highest in the whole array among indices outside the subset. But careful: since the global maximum is in the subset, the maximum of the subset is `mx` itself. Wait! The problem is to return the maximum of each subset. Since `mx` is the global maximum, any subset containing `pos` has maximum = `mx`. So actually we don't need to ask extra queries for subsets containing `pos`. That would simplify: answer `mx` for all subsets that contain `pos`, and `mx` for those that don't? No, that's wrong because a subset not containing `pos` might have a maximum less than `mx` but we don't know it. But we do know that the global maximum is `mx`, and if a subset does not contain `pos`, its maximum could be anything less than `mx`. We need to compute it. So the strategy: after finding `pos`, for each subset that does not contain `pos`, we ask a query on that subset directly (since it doesn't include the global max, the returned value is that subset's maximum). For subsets that contain `pos`, the maximum is `mx`. However, that would be up to `k` queries plus `log n` for binary search, which is within `n + k`. But the original snippet actually asks for the complement for subsets containing `pos` to cleverly get the maximum of that subset? Let's re-read: The original snippet's logic: after finding `bot` (index of maximum), for each subset, if it doesn't have `bot`, set answer = `mx` (correct? Actually the maximum of a subset not containing the global max is not necessarily `mx`! That seems wrong. Let me check the original: It does `if(!has)ans[i]=mx;` – That would be incorrect unless the subsets are partitions of the whole array? But the original problem might be from Codeforces "Guess the Maximum" where subsets are all distinct and the goal is to find the maximum of each subset? Wait the original snippet is from a problem where you have a hidden array, and you need to find the maximum of each of k subsets. But the trick is: if a subset doesn't contain the global maximum, its maximum is not necessarily the global maximum. However, the original code as shown does exactly that, which would be wrong in general. Maybe the original problem is different: perhaps the array values are such that the maximum of any subset containing the global max is the global max, and for those not containing it, the maximum is actually the global maximum because the array is "unimodal" or something? That doesn't make sense. Let me instead design a correct standalone task based on the structure but with a sound algorithm. I will create a task where the goal is to compute the maximum of each subset using queries, but the correct approach is: find global max index via binary search, then for each subset:
//
// - If it contains the global max index, answer is `mx`.
// - Else, ask the subset directly to get its max (since it doesn't contain the global max, the query returns that subset's max, which is valid). This uses at most `k` queries plus `log n` queries. But careful: if a subset has size 1, asking it returns that single value, which is its max. This is fine.
//
// But the original snippet used complement queries to save the queries for subsets containing the global max? Actually it asked complement for those containing the global max, to get the max of that subset? Let's see: If a subset contains the global max, then its maximum is `mx` directly, no need to ask. The original code asks for the complement (all indices not in the subset) and sets ans[i] to that result. That would give the maximum outside the subset, not inside. That seems off. Perhaps the original problem is to find the second maximum? Or maybe it's to find the maximum of each subset but the subsets are constructed such that for those containing the global max, the answer is the maximum outside the subset? That doesn't align. To avoid confusion, I'll design a clean standalone task: Given n and k subsets, you can query any set of indices and get the maximum among them. Compute the maximum for each subset using at most n + k queries. The correct algorithm: find global max index via binary search (log n queries), then for each subset, if it contains global max, answer is mx; else query that subset (1 query) to get its max. That uses log n + (number of subsets not containing global max) queries, which is ≤ log n + k ≤ n + k for n≥1. So it's valid. I'll write a function that does exactly that. The function accepts a callback `ask` that takes a vector of indices and returns the maximum. I'll provide a reference solution with proper const correctness and comments.
//
// Time complexity: O(n + k * (average subset size)) for constructing queries and O(log n) queries. Space O(n + k) for storing subsets and answers.
#include <vector>
#include <algorithm>
#include <functional>
#include <cassert>

/**
 * Given n (number of hidden array elements) and a list of k subsets (each as a vector of 1-indexed elements),
 * and a query function ask(indices) that returns the maximum among the given indices (assuming non-empty),
 * return a vector of k integers where the i-th value is the maximum of the i-th subset.
 * The function may call ask at most n + k times.
 */
std::vector<int> solveQuery(int n, const std::vector<std::vector<int>>& subsets,
                             const std::function<int(const std::vector<int>&)>& ask) {
    int k = (int)subsets.size();

    // 1. Find the global maximum by querying all indices.
    std::vector<int> all(n);
    for (int i = 0; i < n; ++i) all[i] = i + 1;
    int globalMax = ask(all);

    // 2. Find the index of the global maximum via binary search on the index range.
    int lo = 1, hi = n;
    while (lo < hi) {
        int mid = (lo + hi) / 2;
        std::vector<int> left;
        for (int i = lo; i <= mid; ++i) left.push_back(i);
        if (ask(left) == globalMax) {
            hi = mid;
        } else {
            lo = mid + 1;
        }
    }
    int maxIndex = lo; // 1-indexed position of the global maximum

    // 3. For each subset:
    std::vector<int> answer(k);
    for (int i = 0; i < k; ++i) {
        bool containsMax = false;
        for (int idx : subsets[i]) {
            if (idx == maxIndex) { containsMax = true; break; }
        }
        if (containsMax) {
            answer[i] = globalMax;
        } else {
            // Query the subset directly to get its maximum.
            answer[i] = ask(subsets[i]);
        }
    }
    return answer;
}
#include <cassert>
#include <vector>
#include <functional>
#include <algorithm>

// Include the solution function here for testing.

int main() {
    // Hidden array: [5, 1, 9, 3, 7]
    std::vector<int> hidden = {5, 1, 9, 3, 7};
    int n = (int)hidden.size();

    auto ask = [&](const std::vector<int>& indices) -> int {
        int best = hidden[indices[0] - 1];
        for (int idx : indices) best = std::max(best, hidden[idx - 1]);
        return best;
    };

    // Test 1: subsets containing the global max index (3) and not.
    std::vector<std::vector<int>> subsets1 = {{1,2}, {2,3,4}, {1,3,5}, {1,4,5}};
    std::vector<int> expected1 = {5, 9, 9, 7}; // {1,2}=max(5,1)=5; {2,3,4}=max(1,9,3)=9; {1,3,5}=max(5,9,7)=9; {1,4,5}=max(5,3,7)=7
    assert(solveQuery(n, subsets1, ask) == expected1);

    // Test 2: single-element subsets
    std::vector<std::vector<int>> subsets2 = {{1}, {2}, {3}, {4}, {5}};
    std::vector<int> expected2 = {5, 1, 9, 3, 7};
    assert(solveQuery(n, subsets2, ask) == expected2);

    // Test 3: all subsets contain the global max
    std::vector<std::vector<int>> subsets3 = {{1,3}, {3,5}, {2,3,4}};
    std::vector<int> expected3 = {9, 9, 9};
    assert(solveQuery(n, subsets3, ask) == expected3);

    // Test 4: empty case? k=0, should return empty vector
    std::vector<std::vector<int>> subsets4 = {};
    std::vector<int> expected4 = {};
    assert(solveQuery(n, subsets4, ask) == expected4);

    // Test 5: a single subset that is the whole array
    std::vector<std::vector<int>> subsets5 = {{1,2,3,4,5}};
    std::vector<int> expected5 = {9};
    assert(solveQuery(n, subsets5, ask) == expected5);

    // Test 6: n=1, only one element
    std::vector<int> hidden1 = {42};
    auto ask1 = [&](const std::vector<int>& idx) { return hidden1[idx[0]-1]; };
    std::vector<std::vector<int>> subsets6 = {{1}};
    std::vector<int> expected6 = {42};
    assert(solveQuery(1, subsets6, ask1) == expected6);

    // Test 7: binary search correctness with maximum at leftmost index
    std::vector<int> hidden2 = {8, 1, 2, 3};
    auto ask2 = [&](const std::vector<int>& idx) {
        int best = hidden2[idx[0]-1];
        for (int i : idx) best = std::max(best, hidden2[i-1]);
        return best;
    };
    std::vector<std::vector<int>> subsets7 = {{2,3}, {1,4}, {1,2,3}};
    std::vector<int> expected7 = {3, 8, 8};
    assert(solveQuery(4, subsets7, ask2) == expected7);

    // Test 8: binary search correctness with maximum at rightmost index
    std::vector<int> hidden3 = {1, 2, 3, 9};
    auto ask3 = [&](const std::vector<int>& idx) {
        int best = hidden3[idx[0]-1];
        for (int i : idx) best = std::max(best, hidden3[i-1]);
        return best;
    };
    std::vector<std::vector<int>> subsets8 = {{1}, {4}, {1,2,3}};
    std::vector<int> expected8 = {1, 9, 3};
    assert(solveQuery(4, subsets8, ask3) == expected8);

    return 0;
}
