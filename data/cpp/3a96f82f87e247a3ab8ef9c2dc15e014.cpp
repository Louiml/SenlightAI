// Given two arrays `A` (size `n`) and `B` (size `m`), with all elements positive integers, you must write a C++ function that finds up to two pairs `(i, j)` – where `i` is an index in `A` and `j` is an index in `B` – such that the absolute value of the expression `sum(A) - sum(B) + sum_of_selected_A_elements - sum_of_selected_B_elements` is minimized. More precisely: start with the initial difference `D = sum(A) - sum(B)`. You may choose at most one index `i` from `A` (optional) and at most one index `j` from `B` (optional), and you must ensure that if you pick both, they are not paired in the same original `(i,j)` combination. The goal is to make the final difference as close to zero as possible. If picking zero pairs already gives the best possible (i.e., `|D|` is the minimum), output `0` pairs. Otherwise, output the minimum absolute difference and the chosen pairs (1 or 2 pairs) in the format: first the minimum possible absolute difference, then the number of pairs, then each pair as `i j` (1‑based indices). If there are multiple ways to achieve the same minimum, any valid solution is acceptable. The function should return a `std::vector<std::pair<int,int>>` representing the chosen pairs (empty if zero pairs), and the caller will separately print the minimum absolute difference and the number of pairs. The function must handle all edge cases correctly, including empty arrays? For this task, assume `n,m >= 1`. The arrays may contain large numbers up to 10^9, and sizes up to 2000, so a solution that is quadratic in `n*m` is acceptable. Write the solution as a free function `std::vector<std::pair<int,int>> minimizeDifference(const std::vector<int>& A, const std::vector<int>& B, long long& minAbsDiff)` that sets `minAbsDiff` and returns the chosen pairs.
The problem reduces to: we have all possible differences `d = -2*(A[i] - B[j]) = 2*(B[j]-A[i])` for each pair `(i,j)`. If we choose exactly one pair `(i,j)`, the new difference is `D + d`. If we choose two different pairs `(i1,j1)` and `(i2,j2)` that do not share the same `i` and do not share the same `j` (i.e., the two selected indices from `A` are distinct and from `B` are distinct), then the new difference is `D + d1 + d2`. Choosing zero pairs gives `D`. Thus we want to find the combination (0,1, or 2 pairs) that minimizes `|D + sum(selected d)|`. Since `d` values are symmetric around 0 (each `d` has its negative counterpart for some other pair? Not exactly, but we can sort all possible `d`). The classical approach: precompute all `diff1` = `diff2` = list of `2*(B[j]-A[i])` with their corresponding `(i,j)` indices. Sort one copy ascending, the other descending. For each `i` (pivot) in the ascending list, we want to find a `j` in the descending list such that the sum `diff1[i] + diff2[j]` is as close to `-D` as possible, while ensuring that the pair from `diff1` and the pair from `diff2` are either the same? Actually we must avoid using the exact same `(i,j)` pair twice. The condition in the reference code is: if `diff1[p1].first` (which is the `A` index) equals `diff2[p2].first` OR the `B` index equals, then skip. That prevents using two pairs that share an `A` index or share a `B` index, because that would effectively be adding twice the same `A[i]` or same `B[j]` – which is not allowed because we can select each array element at most once. Also, if we pick two pairs, we must not pick the same individual pair (which would be sharing both indices). The algorithm sorts `diff1` ascending and `diff2` descending. For each `p1` in ascending, we advance `p2` in descending while the absolute value of `D + diff1[p1].first + diff2[p2+1].first` is smaller than or equal to the current `p2`. This finds the best `p2` for each `p1` using the two‑pointer technique, because as `p1` increases, the optimal `p2` moves monotonically in one direction (due to convexity of the absolute value). Then we check the best candidate for each `p1` (after skipping invalid ones), track the global minimum. We also compare with `|D|` (zero pairs). Finally, we output the pairs. The implementation uses arrays of size `n*m+1` (the extra slot is a sentinel). Time complexity is `O(n*m log(n*m))` due to sorting, and memory is `O(n*m)`. Edge cases: if `n=m=1`, then `A[0]-B[0]` is the only pair, but we cannot pick two distinct pairs, and picking one pair may be worse than zero; the algorithm handles that because the two‑pointer will skip the only pair when the same indices appear. If all pairs share the same `A` index or same `B` index, the only possible valid selections are zero or one pair. The algorithm must correctly output zero pairs when `|D|` is the minimum. The reference solution uses a helper `as_hard_as_fft` to print the result, but our function must just return the chosen pairs and store the minimum in a reference.
#include <vector>
#include <algorithm>
#include <limits>

using ll = long long;

// Given arrays A (size n) and B (size m), find up to two pairs (i,j) (1-based indices,
// i from A, j from B) such that the absolute value of
// sum(A) - sum(B) + sum(selected A) - sum(selected B) is minimized.
// If no pair is needed, returns an empty vector and sets minAbsDiff to |sum(A)-sum(B)|.
// If one or two pairs are needed, returns them in the order they should be printed.
std::vector<std::pair<int,int>> minimizeDifference(
        const std::vector<int>& A,
        const std::vector<int>& B,
        ll& minAbsDiff) {
    const int n = static_cast<int>(A.size());
    const int m = static_cast<int>(B.size());
    const ll INF = std::numeric_limits<ll>::max();

    ll initialDiff = 0;
    for (int x : A) initialDiff += x;
    for (int y : B) initialDiff -= y;

    // Build list of all d = 2*(B[j] - A[i]) with pair (i+1, j+1)
    std::vector<std::pair<ll, std::pair<int,int>>> dList;
    dList.reserve(static_cast<size_t>(n) * m);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            ll d = 2LL * (static_cast<ll>(B[j]) - A[i]);
            dList.emplace_back(d, std::make_pair(i + 1, j + 1));
        }
    }

    // Sort ascending copy and descending copy.
    // We will use two pointers to find best combination of two distinct pairs.
    std::vector<std::pair<ll, std::pair<int,int>>> asc = dList;
    std::vector<std::pair<ll, std::pair<int,int>>> desc = dList;
    std::sort(asc.begin(), asc.end());
    std::sort(desc.begin(), desc.end(), [](const auto& a, const auto& b) {
        return a.first > b.first;
    });

    ll best = INF;
    std::vector<std::pair<int,int>> bestPairs;
    int p2 = 0;
    const int total = static_cast<int>(asc.size());

    for (int p1 = 0; p1 < total; ++p1) {
        // Advance p2 while the next value improves (or ties) the absolute value
        while (p2 + 1 < total) {
            ll cur = initialDiff + asc[p1].first + desc[p2].first;
            ll nxt = initialDiff + asc[p1].first + desc[p2 + 1].first;
            if (std::abs(cur) >= std::abs(nxt)) {
                ++p2;
            } else {
                break;
            }
        }

        // Check if the two pairs are valid (no shared index from A and no shared index from B)
        if (asc[p1].second.first != desc[p2].second.first &&
            asc[p1].second.second != desc[p2].second.second) {
            ll cand = initialDiff + asc[p1].first + desc[p2].first;
            if (std::abs(cand) < best) {
                best = std::abs(cand);
                bestPairs = {asc[p1].second, desc[p2].second};
            }
        }

        // Also consider using only one pair from asc[p1] (if it beats best)
        ll oneCand = initialDiff + asc[p1].first;
        if (std::abs(oneCand) < best) {
            best = std::abs(oneCand);
            bestPairs = {asc[p1].second};
        }
    }

    // Compare with zero pairs
    if (std::abs(initialDiff) < best) {
        best = std::abs(initialDiff);
        bestPairs.clear();
    }

    minAbsDiff = best;
    return bestPairs;
}
#include <cassert>
#include <vector>
#include <cmath>

// The solution function is assumed to be defined above.

int main() {
    // Test 1: Simple case where one pair helps
    {
        std::vector<int> A = {1};
        std::vector<int> B = {2};
        ll best = 0;
        auto pairs = minimizeDifference(A, B, best);
        // initialDiff = 1-2 = -1. d = 2*(2-1)=2 -> new diff = 1. |1| >= |-1|, so zero pairs best.
        assert(best == 1);
        assert(pairs.empty());
    }

    // Test 2: One pair improves
    {
        std::vector<int> A = {10};
        std::vector<int> B = {1};
        ll best = 0;
        auto pairs = minimizeDifference(A, B, best);
        // initialDiff = 9, d = 2*(1-10) = -18 -> new diff = -9. | -9 | = 9 < 9? Actually |9|=9, so tie, zero pairs acceptable.
        // Our function returns zero pairs because it checks < (strict), not <=. So best 9 with empty.
        assert(best == 9);
        assert(pairs.empty());
    }

    // Test 3: Two pairs
    {
        std::vector<int> A = {1, 100};
        std::vector<int> B = {50, 50};
        ll best = 0;
        auto pairs = minimizeDifference(A, B, best);
        // initialDiff = 101-100 = 1. Possible d's: for (1,50): 98; (1,50): 98; (100,50): -100; (100,50): -100.
        // Two pairs: (1,50)+(100,50) both with j=1? They share j? Actually pairs (1,1) and (2,1) share j=1, invalid.
        // (1,1)+(2,2) share no index? (1,1) and (2,2) valid. d1=98, d2=-100 -> sum=-2, new diff=-1, |−1|=1. Same as zero.
        // So best=1. Could be empty or one pair? One pair (100,50) gives diff=1-100=-99, |−99|>1. So zero pairs best.
        // Our test just expects best == 1 and pairs empty.
        assert(best == 1);
        assert(pairs.empty());
    }

    // Test 4: Multiple pairs available
    {
        std::vector<int> A = {5, 6};
        std::vector<int> B = {10, 11};
        ll best = 0;
        auto pairs = minimizeDifference(A, B, best);
        // initialDiff = 11 - 21 = -10.
        // d values: (5,10)=10, (5,11)=12, (6,10)=8, (6,11)=10.
        // Zero pair: |−10|=10.
        // One pair: (6,10) gives diff = -10+8=-2 -> |−2|=2, better.
        assert(best == 2);
        assert(pairs.size() == 1);
        assert(pairs[0] == std::make_pair(2, 1)); // 6 (1-based index 2) and 10 (index 1)
    }

    // Test 5: Best with two pairs
    {
        std::vector<int> A = {1, 2};
        std::vector<int> B = {3, 4};
        ll best = 0;
        auto pairs = minimizeDifference(A, B, best);
        // initialDiff = 3 - 7 = -4.
        // d's: (1,3)=4, (1,4)=6, (2,3)=2, (2,4)=4.
        // Zero: |−4| = 4.
        // One pair: (2,3) gives -4+2=-2 -> |−2|=2.
        // Two pairs: (1,3)+(2,4) = 4+4=8 -> diff=4, not good. (1,4)+(2,3)=6+2=8 -> diff=4.
        // So best is 2 with one pair.
        assert(best == 2);
        assert(pairs.size() == 1);
        assert(pairs[0] == std::make_pair(2, 1));
    }

    // Test 6: Larger numbers
    {
        std::vector<int> A = {1000000, 1};
        std::vector<int> B = {500000, 500000};
        ll best = 0;
        auto pairs = minimizeDifference(A, B, best);
        // initialDiff = 1000001 - 1000000 = 1.
        // d's: (1000000,500000)=-1000000, (1000000,500000)=-1000000, (1,500000)=999998, (1,500000)=999998.
        // One pair (1000000,500000) gives diff = 1 - 1000000 = -999999, large.
        // One pair (1,500000) gives diff=999999, large.
        // Two pairs (1000000,1?) Actually (1-based) (1,1)+(2,2) -> -1000000+999998=-2, diff=-1, |−1|=1 (same as zero).
        // So best=1, empty.
        assert(best == 1);
        assert(pairs.empty());
    }

    // Test 7: When only two pairs can achieve zero
    {
        std::vector<int> A = {10, 20};
        std::vector<int> B = {30, 40};
        ll best = 0;
        auto pairs = minimizeDifference(A, B, best);
        // initialDiff = 30 - 70 = -40.
        // d's: (10,30)=40, (10,40)=60, (20,30)=20, (20,40)=40.
        // Zero: |−40|=40.
        // One pair: (10,30) gives 0 exactly! diff=-40+40=0. So best=0.
        assert(best == 0);
        assert(pairs.size() == 1);
        assert(pairs[0] == std::make_pair(1, 1)); // (10,30)
    }

    // Test 8: Two pairs needed to get below single pair
    {
        std::vector<int> A = {1, 2, 3};
        std::vector<int> B = {10, 20};
        ll best = 0;
        auto pairs = minimizeDifference(A, B, best);
        // initialDiff = 6 - 30 = -24.
        // d's: (1,10)=18, (1,20)=38, (2,10)=16, (2,20)=36, (3,10)=14, (3,20)=34.
        // One pair: (3,10) gives -24+14=-10 -> |−10|=10.
        // Two pairs: (3,10)+(1,20) = 14+38=52 -> diff=28, too big.
        // (2,10)+(1,20)=16+38=54, etc. So best=10 with one pair.
        assert(best == 10);
        assert(pairs.size() == 1);
    }

    // Test 9: Verify that two pairs sharing an index are not allowed
    {
        std::vector<int> A = {1, 2};
        std::vector<int> B = {100, 101};
        ll best = 0;
        auto pairs = minimizeDifference(A, B, best);
        // initialDiff = 3 - 201 = -198.
        // d's: (1,100)=198, (1,101)=200, (2,100)=196, (2,101)=198.
        // One pair (1,100) gives diff=0. So best=0.
        assert(best == 0);
        assert(pairs.size() == 1);
        assert(pairs[0] == std::make_pair(1, 1));
    }

    return 0;
}
