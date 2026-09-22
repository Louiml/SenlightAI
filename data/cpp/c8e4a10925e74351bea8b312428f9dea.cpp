// Write a C++ function `vector<long long> minimumDeletionsPerPrefix(const vector<long long>& values, long long budget)` that, given a sequence of non-negative integers and a positive total budget, returns for each prefix `i` (0-indexed) the minimum number of elements that must be **removed** from that prefix so that the sum of the remaining elements in the prefix does not exceed the budget. When considering prefix `i`, you may delete any subset of the first `i+1` elements; the goal is to maximize the number of kept elements (i.e., minimize deletions) subject to the sum of kept elements ≤ `budget`. If it is impossible to keep any element (e.g., the first element alone exceeds the budget), then for that prefix you must delete all elements in it, i.e., the answer is `i+1`. The deletion choices for different prefixes are independent; you only report the count for each prefix separately. The input values are guaranteed to be between 0 and 100 (inclusive) for all elements, and the budget is positive and can be up to 10^9. The function should return a vector of the same length as the input, with the i-th element being the minimum deletions for prefix `i`.

// For each prefix, we need to remove the fewest elements such that the sum of the remaining elements is ≤ budget. Since all values are in [0,100], we can use a frequency array (or map) of counts for values 0..100. Process the prefix from left to right, maintaining a running total `sum` of all elements in the current prefix and a frequency array `freq[v]` counting how many times value `v` appears in the prefix. For each new element `x`, we add it to `sum` and increment `freq[x]`. Then, to determine the maximum number of kept elements, we greedily keep the smallest values first: iterate values from 0 to 100, and for each value, we can keep as many as possible from `freq[v]` without exceeding the budget, subtracting `v * kept` from the current remaining budget. The number of deletions is `prefix_length - kept_total`. This works because keeping the smallest values minimizes the sum for a given count, thus maximizing the count within the budget. Edge cases: if an element is 0, it always can be kept and does not consume budget; if the first element already exceeds budget, the greedy will keep zero elements, and deletions equals prefix length. Time complexity: O(n * 100) = O(n) since 100 is constant, and space O(100) for the frequency array. The budget may be large, but sums are bounded by n*100, so use 64-bit integers.

#include <vector>
#include <cstdint>

// Given a sequence of non-negative integers (each 0..100) and a positive budget,
// return for each prefix the minimum number of deletions needed so that the
// sum of the remaining elements in that prefix is <= budget.
std::vector<long long> minimumDeletionsPerPrefix(const std::vector<long long>& values, long long budget) {
    int n = (int)values.size();
    std::vector<long long> result(n);
    std::vector<long long> freq(101, 0);
    long long totalSum = 0;

    for (int i = 0; i < n; ++i) {
        long long x = values[i];
        totalSum += x;
        freq[x]++;

        // Greedily keep the smallest values first to maximize count kept.
        long long remainingBudget = budget;
        long long kept = 0;
        for (int v = 0; v <= 100; ++v) {
            if (freq[v] == 0) continue;
            long long costPerItem = v;
            if (costPerItem == 0) {
                kept += freq[v]; // zero-cost items always kept
                continue;
            }
            long long maxCanKeep = remainingBudget / costPerItem;
            long long keepNow = std::min(freq[v], maxCanKeep);
            kept += keepNow;
            remainingBudget -= keepNow * costPerItem;
            if (keepNow < freq[v]) break; // cannot afford more of this value
        }

        result[i] = (long long)(i + 1) - kept;
    }

    return result;
}

#include <cassert>
#include <vector>

int main() {
    // First element already exceeds budget -> delete all for first prefix.
    {
        std::vector<long long> v = {150, 5, 1};
        // prefix0: sum=150, budget=10 -> keep none, delete 1
        // prefix1: elements {150,5}, budget=10 -> keep only 5, delete 1
        // prefix2: {150,5,1} -> keep 5 and 1 (sum=6) -> delete 1
        auto res = minimumDeletionsPerPrefix(v, 10);
        assert(res.size() == 3);
        assert(res[0] == 1);
        assert(res[1] == 1);
        assert(res[2] == 1);
    }

    // All zeros -> always keep all.
    {
        std::vector<long long> v = {0, 0, 0, 0};
        auto res = minimumDeletionsPerPrefix(v, 5);
        assert(res == std::vector<long long>({0, 0, 0, 0}));
    }

    // Negative not allowed, but test with positive small values and large budget.
    {
        std::vector<long long> v = {1, 2, 3, 4, 5};
        auto res = minimumDeletionsPerPrefix(v, 15);
        // All prefixes sum <= 15? prefix sums: 1,3,6,10,15 all <=15 -> no deletions.
        assert(res == std::vector<long long>({0,0,0,0,0}));
    }

    // Tight budget: some deletions needed.
    {
        std::vector<long long> v = {5, 4, 3, 2, 1};
        // budget=7
        // prefix0: {5} -> keep 5 -> delete 0
        // prefix1: {5,4} sum=9>7, keep 4 or 5? keep 4 (sum=4) or 5 (sum=5) -> both ok, but we keep smallest first: keep 4 and 5? 4+5=9>7, so keep only 4 -> keep 1, delete 1.
        // prefix2: {5,4,3} sum=12, keep 3+4=7 -> keep 2, delete 1.
        // prefix3: {5,4,3,2} sum=14, keep 2+3+? 2+3=5, +4=9>7, so keep 2+3=5, keep 2, delete 2.
        // prefix4: {5,4,3,2,1} sum=15, keep 1+2+3=6? highest count with sum<=7: keep 1,2,3,4? sum=10>7, so keep 1,2,3 (sum=6) -> keep 3, delete 2.
        auto res = minimumDeletionsPerPrefix(v, 7);
        assert(res == std::vector<long long>({0, 1, 1, 2, 2}));
    }

    // Large budget -> no deletions ever.
    {
        std::vector<long long> v = {100, 100, 100, 100};
        auto res = minimumDeletionsPerPrefix(v, 1000);
        assert(res == std::vector<long long>({0,0,0,0}));
    }

    // Single element exactly budget -> keep it.
    {
        std::vector<long long> v = {42};
        auto res = minimumDeletionsPerPrefix(v, 42);
        assert(res == std::vector<long long>({0}));
    }

    // Single element just above budget -> delete it.
    {
        std::vector<long long> v = {43};
        auto res = minimumDeletionsPerPrefix(v, 42);
        assert(res == std::vector<long long>({1}));
    }

    return 0;
}
