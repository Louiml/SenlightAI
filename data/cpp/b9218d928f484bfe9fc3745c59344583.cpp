/*
Given a list of `n` positive integers and `q` queries, for each query with a value `x`, compute the sum of all prefix elements whose corresponding maximum-so-far is less than or equal to `x`. More precisely, after building a prefix-sum array and a prefix-maximum array (where `pref[i] = sum of first i elements` and `maxi[i] = maximum among the first i elements`), for each query `x`, find the largest index `k` such that `maxi[k] <= x`, and output `pref[k]`. If no such index exists, output `0`. You are to implement a function `vector<long long> processQueries(const vector<long long>& a, const vector<long long>& queries)` that takes the array `a` (1-indexed conceptually, but you can use 0-indexed internally) and the list of queries, and returns a vector of answers for each query. The constraints: `1 ≤ n, q ≤ 10^5`, each element and each query is within `1` to `10^9`. The function must be efficient enough for large inputs.
*/
#include <vector>
#include <algorithm>

// Process queries on prefix sums based on prefix maximum condition.
// a: array of positive integers (0-indexed internally).
// queries: list of x values.
// Returns a vector where the i-th element is the answer for queries[i].
std::vector<long long> processQueries(const std::vector<long long>& a, const std::vector<long long>& queries) {
    int n = static_cast<int>(a.size());
    
    // Prefix sum and prefix maximum, 1-indexed with index 0 as sentinel.
    std::vector<long long> pref(n + 1, 0);
    std::vector<long long> maxi(n + 1, 0);
    for (int i = 1; i <= n; ++i) {
        pref[i] = pref[i - 1] + a[i - 1];
        maxi[i] = std::max(maxi[i - 1], a[i - 1]);
    }
    
    std::vector<long long> answers;
    answers.reserve(queries.size());
    for (long long x : queries) {
        // Find first index where maxi > x.
        int pos = static_cast<int>(std::upper_bound(maxi.begin(), maxi.end(), x) - maxi.begin());
        int k = pos - 1; // largest index with maxi <= x.
        long long ans = (k >= 1) ? pref[k] : 0;
        answers.push_back(ans);
    }
    return answers;
}
#include <cassert>
#include <vector>

// Declaration of the solution function (assumed to be defined above).
std::vector<long long> processQueries(const std::vector<long long>& a, const std::vector<long long>& queries);

int main() {
    // Test 1: Basic example
    {
        std::vector<long long> a = {5, 3, 8, 2, 9};
        std::vector<long long> queries = {4, 5, 8, 10, 1};
        std::vector<long long> expected = {0, 5, 16, 27, 0};
        assert(processQueries(a, queries) == expected);
    }
    // Test 2: All elements equal
    {
        std::vector<long long> a = {7, 7, 7};
        std::vector<long long> queries = {6, 7, 8};
        std::vector<long long> expected = {0, 21, 21};
        assert(processQueries(a, queries) == expected);
    }
    // Test 3: Single element
    {
        std::vector<long long> a = {10};
        std::vector<long long> queries = {9, 10, 11};
        std::vector<long long> expected = {0, 10, 10};
        assert(processQueries(a, queries) == expected);
    }
    // Test 4: Decreasing sequence
    {
        std::vector<long long> a = {10, 9, 8, 7};
        std::vector<long long> queries = {7, 8, 9, 10};
        std::vector<long long> expected = {34, 34, 34, 34}; // maxi is constant 10, only query >=10 works? Actually maxi[1]=10, so all <=7? Let's recompute: maxi = [0,10,10,10,10]. For x=7, upper_bound returns 1 (since maxi[1]=10 >7), k=0 => 0. For x=8, same. For x=9, same. For x=10, upper_bound returns 5, k=4 => pref[4]=34. So expected is {0,0,0,34}. Correct.
        std::vector<long long> correct_expected = {0,0,0,34};
        assert(processQueries(a, queries) == correct_expected);
    }
    // Test 5: Large values, no queries threshold
    {
        std::vector<long long> a = {1000000000, 1, 1000000000};
        std::vector<long long> queries = {999999999, 1000000000};
        std::vector<long long> expected = {1000000000, 2000000001}; // For x=999999999: upper_bound gives index1 (maxi[1]=1e9>), k=0 =>0? Actually a[0]=1e9, maxi[1]=1e9, so upper_bound(1e9-1) returns 1 => k=0 =>0. For x=1e9: upper_bound returns 4 (since maxi[3]=1e9, not >, wait upper_bound returns first >1e9, none, so n+1=4), k=3 => pref[3]=2e9+1. So expected {0, 2000000001}.
        std::vector<long long> correct_expected = {0, 2000000001LL};
        assert(processQueries(a, queries) == correct_expected);
    }
    // Test 6: Empty queries
    {
        std::vector<long long> a = {1,2,3};
        std::vector<long long> queries = {};
        assert(processQueries(a, queries).empty());
    }
    // Test 7: All queries very small
    {
        std::vector<long long> a = {2,4,6,8};
        std::vector<long long> queries = {1,2,3};
        std::vector<long long> expected = {0,2,2};
        assert(processQueries(a, queries) == expected);
    }
    return 0;
}
// The core idea is to preprocess the array to answer all queries in `O((n + q) log n)` time. We build two arrays of length `n`: `pref[i]` stores the sum of the first `i` elements (1-indexed) and `maxi[i]` stores the maximum among the first `i` elements. Notice that `maxi` is a non-decreasing sequence because adding a new element can only increase (or keep) the running maximum. For a query `x`, we need the largest index `k` (1-based) such that `maxi[k] <= x`. Since `maxi` is sorted non-decreasingly, we can use binary search (`upper_bound`) to find the first element strictly greater than `x`, then take that index minus 1. If the found index `k >= 1`, then the answer is `pref[k]`, otherwise `0`. Edge cases: if all `maxi` elements are greater than `x`, `upper_bound` returns `1` (since `maxi[0]` is 0 and the array is 1-indexed conceptually, but we must carefully handle indices in implementation—we can use a sentinel `maxi[0]=0` and `pref[0]=0`). Then `k = index - 1` could be `0`, giving `pref[0]=0`. Also, if `x` is very large, `upper_bound` returns `n+1`, so `k = n`, giving the total sum. Time complexity: `O(n)` for prefix arrays and `O(q log n)` for binary searches per query, total `O((n+q) log n)` (though since `n` is 1e5, `log n` ~ 17). Space complexity: `O(n)` for the arrays plus `O(q)` for the output.
