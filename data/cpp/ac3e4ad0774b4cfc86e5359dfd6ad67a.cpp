/*
You are given an array of `n` positive integers and `q` queries. For each query, you are given a value `x`; you must output the sum of the smallest prefix of the array (from index 0 to some index `i`) such that every element in that prefix is **less than or equal to** `x`. More formally, if we let `max_prefix[i]` be the maximum value among `a[0]...a[i]`, then the **valid prefix** for a query `x` is the longest prefix whose `max_prefix[i] ≤ x`. If no such prefix exists (i.e., even `a[0] > x`), output `0`. Return the sum of all elements in that valid prefix. Implement a function that takes the array `a`, and a vector of queries, and returns a `vector<long long>` where each element is the answer for the corresponding query. The array and queries are all positive integers within `long long` range.
*/

#include <vector>
#include <algorithm>

// Precompute prefix maxima and prefix sums, then answer queries via binary search.
std::vector<long long> prefixQueryAnswers(const std::vector<long long>& a,
                                          const std::vector<long long>& queries) {
    int n = static_cast<int>(a.size());
    std::vector<long long> prefMax(n);
    std::vector<long long> prefSum(n);

    // Build prefix arrays.
    long long currentMax = a[0];
    long long currentSum = 0;
    for (int i = 0; i < n; ++i) {
        currentMax = std::max(currentMax, a[i]);
        currentSum += a[i];
        prefMax[i] = currentMax;
        prefSum[i] = currentSum;
    }

    std::vector<long long> results;
    results.reserve(queries.size());

    for (long long x : queries) {
        // First index where prefMax > x. All before that are ≤ x.
        auto it = std::upper_bound(prefMax.begin(), prefMax.end(), x);
        int idx = static_cast<int>(it - prefMax.begin());
        if (idx == 0) {
            results.push_back(0);
        } else {
            results.push_back(prefSum[idx - 1]);
        }
    }
    return results;
}

#include <cassert>
#include <vector>

// Include the solution function here (or link to it).

int main() {
    // Basic test from the original problem style.
    std::vector<long long> a1 = {1, 2, 3, 4, 9};
    std::vector<long long> q1 = {1, 3, 4, 9, 0, 10};
    std::vector<long long> r1 = prefixQueryAnswers(a1, q1);
    assert(r1 == (std::vector<long long>{1, 6, 6, 19, 0, 19}));

    // Single element array.
    std::vector<long long> a2 = {5};
    std::vector<long long> q2 = {4, 5, 6};
    std::vector<long long> r2 = prefixQueryAnswers(a2, q2);
    assert(r2 == (std::vector<long long>{0, 5, 5}));

    // All elements equal.
    std::vector<long long> a3 = {7, 7, 7, 7};
    std::vector<long long> q3 = {6, 7, 8};
    std::vector<long long> r3 = prefixQueryAnswers(a3, q3);
    assert(r3 == (std::vector<long long>{0, 28, 28}));

    // Non‑increasing array (max stays same).
    std::vector<long long> a4 = {10, 9, 8, 7};
    std::vector<long long> q4 = {7, 8, 9, 10, 11};
    std::vector<long long> r4 = prefixQueryAnswers(a4, q4);
    assert(r4 == (std::vector<long long>{0, 0, 0, 10, 34}));

    // Mixed values with duplicates.
    std::vector<long long> a5 = {3, 1, 4, 1, 5};
    std::vector<long long> q5 = {2, 3, 4, 5};
    std::vector<long long> r5 = prefixQueryAnswers(a5, q5);
    // prefMax: 3,3,4,4,5; prefSum: 3,4,8,9,14
    assert(r5 == (std::vector<long long>{0, 3, 8, 14}));

    // Large values to check overflow (long long).
    std::vector<long long> a6 = {1000000000LL, 1000000000LL};
    std::vector<long long> q6 = {999999999LL, 1000000000LL, 1000000001LL};
    std::vector<long long> r6 = prefixQueryAnswers(a6, q6);
    assert(r6 == (std::vector<long long>{0, 2000000000LL, 2000000000LL}));

    // Empty array? Not allowed per spec, but handle gracefully.
    std::vector<long long> a7 = {2};
    std::vector<long long> q7 = {1};
    std::vector<long long> r7 = prefixQueryAnswers(a7, q7);
    assert(r7 == (std::vector<long long>{0}));

    return 0;
}

// The key observation is that as we extend the prefix, the maximum value is non‑decreasing. Therefore, for a fixed `x`, the condition `max_prefix[i] ≤ x` is true for a contiguous range of indices starting from 0. The longest such prefix ends at the last index where `max_prefix[i] ≤ x`. To answer many queries efficiently, we precompute:
// - `prefMax[i]`: the maximum of `a[0]...a[i]` for each `i`.
// - `prefSum[i]`: the sum of `a[0]...a[i]` for each `i`.
//
// Since `prefMax` is monotonic non‑decreasing, for each query `x` we can binary‑search (using `upper_bound`) to find the first index `idx` where `prefMax[idx] > x`. All indices `0 ... idx-1` satisfy the condition, so the answer is `prefSum[idx-1]` (if `idx == 0`, answer is `0`). Time complexity: O(n + q log n). Space: O(n) for the two auxiliary arrays. Important edge cases: empty prefix (when `a[0] > x`), queries equal to a maximum value (use `upper_bound`, not `lower_bound`), and large sums that exceed 32‑bit integers (use `long long`).
