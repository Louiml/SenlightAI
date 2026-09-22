/*
You are given a sorted array of `n` distinct integers and `q` queries. For a segment of the array of length `L` starting at index `l` (0-based), define a **pivot count** as the number of ordered triples `(i, j, k)` such that `l ≤ i < j < k ≤ l+L-1` and `a[i] < a[j] < a[k]`. In a sorted array, this triple condition always holds for any `i<j<k`. Now consider all possible contiguous segments of any length `L` (from 1 to n) and any start index `l` such that the segment lies entirely within the array. For each query value `x`, you need to output how many distinct segments (by their `(l, L)` pair) have exactly `x` such triples. However, the actual counting is simplified: for a segment starting at index `l` and ending at index `r` (inclusive), the number of triples is simply `C(L, 3)` where `L = r-l+1`. But the problem in the snippet works differently: it uses a specific formula `tmp = i + (n-i-1) + i*(n-i-1)` for full-array splits, and then also considers gaps between consecutive elements. You need to implement a function that, given the sorted array and the queries, returns the frequency of each query value as described by the original snippet. Specifically, for each index `i` (0-based), compute `c1 = i + (n-i-1) + i*(n-i-1)`, and increment the frequency of that value by 1. For each adjacent pair `(i, i+1)`, compute `c2 = 1 + i + (n-i-2) + i*(n-i-2)`, and increment the frequency of that value by `a[i+1] - a[i] - 1`. Then answer each query by returning the frequency of that query value (if not present, return 0). Your function should take a vector of `long long` (the sorted array), a vector of `long long` (queries), and return a vector of `long long` with the answers in the same order. The function must be efficient for `n` and `q` up to `2e5`, and values in the array and queries fit in `long long`.
*/

#include <vector>
#include <unordered_map>

// Given a sorted array a and queries, return the frequency of each query value
// as computed by the described combinatorial formula.
std::vector<long long> countPivotFrequencies(const std::vector<long long>& a,
                                              const std::vector<long long>& queries) {
    long long n = static_cast<long long>(a.size());
    std::unordered_map<long long, long long> freq;

    // For each actual index i, compute the count of pairs (j,k) that
    // "pass through" i in the sense of the formula.
    for (long long i = 0; i < n; ++i) {
        long long tmp = i + (n - i - 1) + i * (n - i - 1);
        ++freq[tmp];
    }

    // For each adjacent gap, consider the missing integer values between a[i] and a[i+1].
    for (long long i = 0; i < n - 1; ++i) {
        long long tmp = 1LL + i + (n - i - 2) + i * (n - i - 2);
        long long gapCount = a[static_cast<size_t>(i + 1)] - a[static_cast<size_t>(i)] - 1;
        if (gapCount > 0) {
            freq[tmp] += gapCount;
        }
    }

    std::vector<long long> answers;
    answers.reserve(queries.size());
    for (const long long x : queries) {
        auto it = freq.find(x);
        answers.push_back(it == freq.end() ? 0LL : it->second);
    }
    return answers;
}

#include <cassert>
#include <vector>

// The function to test is declared above. We'll include the solution code by copy.
// (In a real exercise, the solution header would be included.)

int main() {
    // Test 1: n=1, single element, query 0 should have frequency 1.
    {
        std::vector<long long> a = {5};
        std::vector<long long> q = {0, 1};
        auto res = countPivotFrequencies(a, q);
        assert(res == std::vector<long long>({1, 0}));
    }

    // Test 2: n=2, consecutive elements, no gaps.
    {
        std::vector<long long> a = {1, 2};
        std::vector<long long> q = {0, 1, 2};
        // For i=0: tmp = 0 + 1 + 0*1 = 1 -> freq[1]=1
        // For i=1: tmp = 1 + 0 + 1*0 = 1 -> freq[1]=2
        // For gap i=0: tmp = 1 + 0 + 0*0 = 1, gapCount = 2-1-1=0 -> no addition
        // So answer for 1 is 2, others 0.
        auto res = countPivotFrequencies(a, q);
        assert(res == std::vector<long long>({0, 2, 0}));
    }

    // Test 3: n=3, consecutive, all gaps zero.
    {
        std::vector<long long> a = {10, 20, 30};
        std::vector<long long> q = {2, 4, 6};
        // Indices:
        // i=0: 0 + 2 + 0*2 = 2
        // i=1: 1 + 1 + 1*1 = 3
        // i=2: 2 + 0 + 2*0 = 2
        // Gaps: i=0: 1 + 0 + 1*0? wait formula: 1 + i + (n-i-2) + i*(n-i-2)
        // i=0: 1+0+1+0 = 2? Actually n=3, so (n-i-2)=1, so tmp=1+0+1+0=2, gapCount=10-10? No, a[1]-a[0]-1=9. So freq[2]+=9
        // i=1: 1+1+0+0=2, gapCount=30-20-1=9, so freq[2]+=9
        // Total freq[2]=2+9+9=20
        // freq[3]=1 from i=1.
        // So query 2 -> 20, 4->0, 6->0.
        auto res = countPivotFrequencies(a, q);
        assert(res == std::vector<long long>({20, 0, 0}));
    }

    // Test 4: n=3 with gaps, check a specific value.
    {
        std::vector<long long> a = {0, 5, 10};
        std::vector<long long> q = {2, 3, 5};
        // Indices:
        // i=0: 0+2+0=2
        // i=1: 1+1+1=3
        // i=2: 2+0+0=2
        // Gaps:
        // i=0: 1+0+1+0=2, gap=5-0-1=4 -> freq[2]+=4
        // i=1: 1+1+0+0=2, gap=10-5-1=4 -> freq[2]+=4
        // Final freq[2]=2+4+4=10, freq[3]=1
        // Queries: 2->10, 3->1, 5->0.
        auto res = countPivotFrequencies(a, q);
        assert(res == std::vector<long long>({10, 1, 0}));
    }

    // Test 5: Large n small test to check overflow and correctness.
    {
        std::vector<long long> a = {1, 2, 3, 4};
        std::vector<long long> q = {0, 1, 2, 3, 4, 5, 6, 7, 8};
        // n=4:
        // i=0: 0+3+0=3
        // i=1: 1+2+2=5
        // i=2: 2+1+2=5
        // i=3: 3+0+0=3
        // Gaps (all diff=1 so gapCount=0):
        // i=0: 1+0+2+0=3
        // i=1: 1+1+1+1=4
        // i=2: 1+2+0+0=3
        // So freq[3]= (i0,i3,gaps i0,i2) = 1+1+1+1=4
        // freq[5]=2
        // freq[4]=1
        // Queries: 0->0,1->0,2->0,3->4,4->1,5->2,6->0,7->0,8->0
        auto res = countPivotFrequencies(a, q);
        assert(res == std::vector<long long>({0,0,0,4,1,2,0,0,0}));
    }

    // Test 6: Query not present returns 0.
    {
        std::vector<long long> a = {1, 100};
        std::vector<long long> q = {99, 1000};
        auto res = countPivotFrequencies(a, q);
        assert(res == std::vector<long long>({0, 0}));
    }

    return 0;
}

// The core idea is to derive the number of ways a specific integer can appear as the middle element in a pair of indices that straddle a cut point. The snippet uses a combinatorial observation: For a given position `i` in the array, any pair `(j, k)` with `j ≤ i ≤ k` and `j < k` can be used to form a triple where `j` is the first and `k` is the last, and `i` is not necessarily the middle. But the actual formula `i + (n-i-1) + i*(n-i-1)` counts the number of unordered pairs `(j, k)` with `j < i < k`, plus pairs where one equals `i` and the other is any index not equal to `i`. Specifically, `i` choices for `j` from left side, `(n-i-1)` for `k` from right side, plus the cross products. This is essentially the number of triples where `i` is the median in some sense, but the snippet applies it to every index independently. Then for each gap between consecutive elements, it accounts for missing values: if `a[i+1] - a[i] > 1`, there are that many possible values that could fit between them, and each such value would be a distinct "virtual" index that wouldn't have a direct position, so it adds that many occurrences of the gap count. The algorithm is straightforward: iterate over indices, compute the two formulas, and accumulate into a map. Then for each query, simply look up the map (default 0). Complexity is `O(n + q)` time and `O(n + distinct counts)` space, with unordered_map for expected `O(1)` average. Edge cases: `n=1` has no gaps, only one index with `tmp = 0` (since `i=0`, `(n-i-1)=0`, `i*(n-i-1)=0`). For large `n`, the counts can exceed `2^31` but fit in `long long`. The queries might be enormous numbers not present in the map, return 0.
