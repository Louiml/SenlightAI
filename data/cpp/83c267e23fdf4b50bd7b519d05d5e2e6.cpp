Given a sequence of \(n\) integers and an integer \(K\) (with \(2 \le K \le n\)), write a C++ function `int longestRepeatedSubstringLength(const std::vector<int>& a, int K)` that returns the length of the longest substring that appears at least \(K\) times in the sequence. A substring is a contiguous block of elements; occurrences may overlap. For example, in `[2,3,2,3,2,3,2,3,2]` with \(K=4\), the substring `[2,3]` appears 4 times, and no longer substring appears 4 times, so the answer is 2. If no substring appears at least \(K\) times, return 0.
This is a classic problem solved using suffix arrays and the LCP (Longest Common Prefix) array.  
- Build a **suffix array** `sa` for the sequence `a` (1-indexed for convenience).  
- Compute the **rank** array and the **height** array, where `height[i]` = LCP of suffixes `sa[i-1]` and `sa[i]` (for `i>1`), and `height[1]=0`.  
- A substring of length `L` appears at least `K` times if and only if there exists a contiguous block of at least `K-1` consecutive `height` values all `>= L`. This is because suffixes sharing a prefix of length `L` form a contiguous block in the suffix array; if that block has size `>= K`, then the substring appears that many times.  
- Use **binary search** on the answer `L` from 1 to n. For a given `L`, scan the height array, count how many consecutive heights are `>= L`. If at any point the count `>= K-1`, then `L` works.  
- **Edge cases**: If `K == 1`, every element is a substring of length 1, so the answer would be `n`, but the problem says `K >= 2`, so we can ignore. If no substring repeats `K` times, the binary search will end at `0` (we can initialize `l=0`). Overlapping occurrences are handled naturally because we count suffixes sharing a prefix regardless of overlap.

**Complexities**:  
- Building suffix array takes \(O(n \log n)\) time using a radix sort (counting sort) per doubling step.  
- Computing height takes \(O(n)\).  
- Binary search with a linear check takes \(O(n \log n)\).  
Total: \(O(n \log n)\) time, \(O(n)\) auxiliary space.
#include <vector>
#include <algorithm>
#include <cassert>

// Suffix array and LCP computation for a vector of integers (values may be up to 1e6).
// Returns the length of the longest substring that appears at least K times.
int longestRepeatedSubstringLength(const std::vector<int>& a, int K) {
    int n = (int)a.size();
    if (n == 0 || K > n) return 0;
    if (K == 1) return n; // every single element appears n times, but problem says K>=2; still handle defensively.

    // We use 1-indexed arrays for convenience.
    std::vector<int> rank(n + 1), tmp(n + 1), sa(n + 1), height(n + 1);
    const int MAX_VAL = 1000000; // max possible value in input (from problem constraints)
    int m = MAX_VAL;

    // Initial rank: value of each element. tmp: index order.
    for (int i = 1; i <= n; ++i) {
        rank[i] = a[i - 1];
        tmp[i] = i;
    }

    // Counting sort helper (uses global rank and tmp).
    auto countingSort = [&]() {
        std::vector<int> book(m + 1, 0);
        for (int i = 1; i <= n; ++i) book[rank[i]]++;
        for (int i = 1; i <= m; ++i) book[i] += book[i - 1];
        for (int i = n; i >= 1; --i) sa[book[rank[tmp[i]]]--] = tmp[i];
    };

    // First sort.
    countingSort();

    // Doubling.
    for (int k = 1, cnt = 1; cnt < n; k <<= 1, m = cnt) {
        cnt = 0;
        for (int i = 1; i <= k; ++i) tmp[++cnt] = n - k + i;
        for (int i = 1; i <= n; ++i) if (sa[i] > k) tmp[++cnt] = sa[i] - k;
        countingSort();
        std::swap(rank, tmp);
        rank[sa[1]] = cnt = 1;
        for (int i = 2; i <= n; ++i) {
            rank[sa[i]] = (tmp[sa[i]] == tmp[sa[i - 1]] && tmp[sa[i] + k] == tmp[sa[i - 1] + k]) ? cnt : ++cnt;
        }
    }

    // Compute LCP (height array). height[i] = LCP of sa[i-1] and sa[i] for i>1, height[1]=0.
    int k = 0;
    for (int i = 1; i <= n; ++i) {
        if (rank[i] == 1) { height[rank[i]] = 0; continue; }
        int j = sa[rank[i] - 1];
        while (i + k <= n && j + k <= n && a[i - 1 + k] == a[j - 1 + k]) ++k;
        height[rank[i]] = k;
        if (k) --k;
    }

    // Check if there exists a substring of length L that appears at least K times.
    auto check = [&](int L) {
        if (L == 0) return true;
        int consecutive = 0;
        for (int i = 2; i <= n; ++i) {
            if (height[i] >= L) {
                consecutive++;
                if (consecutive >= K - 1) return true;
            } else {
                consecutive = 0;
            }
        }
        return false;
    };

    // Binary search the answer.
    int lo = 0, hi = n;
    while (lo < hi) {
        int mid = (lo + hi + 1) / 2;
        if (check(mid)) lo = mid;
        else hi = mid - 1;
    }
    return lo;
}
#include <cassert>
#include <vector>

int longestRepeatedSubstringLength(const std::vector<int>& a, int K); // from solution

int main() {
    // Example from problem: array of 2,3 repeated, K=4 => answer 2
    std::vector<int> a1 = {2,3,2,3,2,3,2,3,2};
    assert(longestRepeatedSubstringLength(a1, 4) == 2);

    // Second example: 1 2 3 4 4 3 1 2, K=2 => "1 2" appears twice, length 2
    std::vector<int> a2 = {1,2,3,4,4,3,1,2};
    assert(longestRepeatedSubstringLength(a2, 2) == 2);

    // All same, K=3 => length 6 (whole array can repeat? but K=3, substring length 6 appears once, actually length 1 appears 8 times, length 2 appears 7, ... length 6 appears 3 times? Let's check: vector of 8 zeros, K=3, longest is 6 because "0"*6 appears at positions 0,1,2)
    std::vector<int> a3(8, 0);
    assert(longestRepeatedSubstringLength(a3, 3) == 6);

    // No repetition: distinct values, K=2 => no substring of length >=1 repeats? Actually length 1 appears once each, so answer 0
    std::vector<int> a4 = {1,2,3,4,5,6,7};
    assert(longestRepeatedSubstringLength(a4, 2) == 0);

    // Single element: length 1, K=2 not possible because K<=n, but just test K=1 returns n (defensive)
    std::vector<int> a5 = {42};
    assert(longestRepeatedSubstringLength(a5, 1) == 1);

    // Overlapping: [1,1,1,1] K=3 => substring [1,1] appears 3 times (positions 0-1,1-2,2-3), length 2; longer length 3 appears twice, so answer 2
    std::vector<int> a6 = {1,1,1,1};
    assert(longestRepeatedSubstringLength(a6, 3) == 2);

    // Edge: K equals n, array all same length 5 => whole array appears once, but any substring of length 1 appears 5 times, so answer 1? Actually length 4 appears 2 times? Wait: [5,5,5,5,5], K=5, longest substring appearing 5 times is length 1 (each single element appears 5 times); length 2 appears 4 times, so answer 1.
    std::vector<int> a7(5, 7);
    assert(longestRepeatedSubstringLength(a7, 5) == 1);

    return 0;
}
