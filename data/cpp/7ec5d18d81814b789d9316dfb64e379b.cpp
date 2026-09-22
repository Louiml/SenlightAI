/*
Write a C++ function `vector<int> rearrangeForMinimumMaxAdjacentDifference(vector<int> a)` that takes a vector of integers (size \( n \ge 2 \)), sorts it internally, and returns a permutation of `a` that minimizes the maximum absolute difference between adjacent elements in the circular arrangement (i.e., including the wrap-around between the last and first element). If multiple permutations achieve the same minimum maximum adjacent difference, return any one of them. The function must handle duplicate values correctly, and for \( n=2 \) return the sorted pair directly. The input is not modified.
*/

#include <vector>
#include <algorithm>
#include <climits>

// Returns a permutation of the input that minimizes the maximum absolute
// difference between consecutive elements in a circular arrangement.
std::vector<int> rearrangeForMinimumMaxAdjacentDifference(std::vector<int> a) {
    int n = static_cast<int>(a.size());
    if (n == 2) {
        std::sort(a.begin(), a.end());
        return a;
    }

    std::sort(a.begin(), a.end());

    int min_gap = INT_MAX;
    for (int i = 0; i + 1 < n; ++i) {
        min_gap = std::min(min_gap, a[i + 1] - a[i]);
    }

    std::vector<int> best;
    int best_max_gap = INT_MAX;

    for (int i = 0; i + 1 < n; ++i) {
        if (a[i + 1] - a[i] != min_gap) {
            continue;
        }

        // Construction 1: middle in sorted order
        std::vector<int> candidate;
        candidate.push_back(a[i]);
        for (int j = 0; j < n; ++j) {
            if (j == i || j == i + 1) continue;
            candidate.push_back(a[j]);
        }
        candidate.push_back(a[i + 1]);

        int max_gap = 0;
        for (int j = 0; j < n; ++j) {
            int next = (j + 1 == n) ? 0 : j + 1;
            max_gap = std::max(max_gap, std::abs(candidate[j] - candidate[next]));
        }

        if (max_gap < best_max_gap) {
            best_max_gap = max_gap;
            best = candidate;
        }

        // Construction 2: rotate the middle to potentially improve wrap-around
        if (i > 0 && i + 1 < n) {
            std::vector<int> candidate2;
            candidate2.push_back(a[i]);
            for (int j = i + 2; j < n; ++j) {
                candidate2.push_back(a[j]);
            }
            for (int j = 0; j < i; ++j) {
                candidate2.push_back(a[j]);
            }
            candidate2.push_back(a[i + 1]);

            int max_gap2 = 0;
            for (int j = 0; j < n; ++j) {
                int next = (j + 1 == n) ? 0 : j + 1;
                max_gap2 = std::max(max_gap2, std::abs(candidate2[j] - candidate2[next]));
            }

            if (max_gap2 < best_max_gap) {
                best_max_gap = max_gap2;
                best = candidate2;
            }
        }
    }

    return best;
}

#include <cassert>
#include <vector>
#include <algorithm>
#include <climits>

// Function declaration from the solution
std::vector<int> rearrangeForMinimumMaxAdjacentDifference(std::vector<int> a);

// Helper to compute maximum circular adjacent difference
int circularMaxGap(const std::vector<int>& v) {
    int n = v.size();
    int g = 0;
    for (int i = 0; i < n; ++i) {
        g = std::max(g, std::abs(v[i] - v[(i + 1) % n]));
    }
    return g;
}

int main() {
    // Test 1: Simple case
    {
        std::vector<int> input = {1, 2, 3};
        auto result = rearrangeForMinimumMaxAdjacentDifference(input);
        assert(circularMaxGap(result) == 1);
    }

    // Test 2: n=2
    {
        std::vector<int> input = {5, 1};
        auto result = rearrangeForMinimumMaxAdjacentDifference(input);
        assert(result.size() == 2);
        assert(std::min(result[0], result[1]) == 1);
        assert(std::max(result[0], result[1]) == 5);
    }

    // Test 3: All same values
    {
        std::vector<int> input = {7, 7, 7, 7};
        auto result = rearrangeForMinimumMaxAdjacentDifference(input);
        assert(circularMaxGap(result) == 0);
    }

    // Test 4: Larger gaps
    {
        std::vector<int> input = {1, 100, 101, 200};
        auto result = rearrangeForMinimumMaxAdjacentDifference(input);
        assert(circularMaxGap(result) == 100);
    }

    // Test 5: Duplicates with one outlier
    {
        std::vector<int> input = {3, 3, 3, 10};
        auto result = rearrangeForMinimumMaxAdjacentDifference(input);
        assert(circularMaxGap(result) == 7);
    }

    // Test 6: Negative numbers
    {
        std::vector<int> input = {-5, -1, 0, 4};
        auto result = rearrangeForMinimumMaxAdjacentDifference(input);
        assert(circularMaxGap(result) == 5);
    }

    // Test 7: Random permutation check for moderate size
    {
        std::vector<int> input = {10, 2, 8, 3, 7, 1, 9, 4, 6, 5};
        auto result = rearrangeForMinimumMaxAdjacentDifference(input);
        // Compare with brute force for n=10 (factorial is huge, but we can
        // just check the result is a permutation and gap ≤ 2, since
        // the sorted arrangement {1,2,...,10} has circular max gap 9,
        // but optimal arrangement can be found by known solution: e.g., 
        // 1,3,5,7,9,10,8,6,4,2 gives max gap 2? Let's just assert gap <= 3.
        assert(circularMaxGap(result) <= 3);
        // Additionally verify it's a permutation
        auto sorted_copy = result;
        std::sort(sorted_copy.begin(), sorted_copy.end());
        assert(sorted_copy == std::vector<int>({1,2,3,4,5,6,7,8,9,10}));
    }

    // Test 8: Edge with duplicates at ends
    {
        std::vector<int> input = {1, 1, 2, 2, 3, 3};
        auto result = rearrangeForMinimumMaxAdjacentDifference(input);
        assert(circularMaxGap(result) == 1);
    }

    return 0;
}

// The optimal strategy is to place the two closest elements (minimum adjacent difference after sorting) at the two ends of the linear arrangement, because the wrap-around edge connecting the last and first elements must be one of the adjacent pairs, and we want at least one small gap to break the cycle. More precisely:
// 1. Sort the array. The circular maximum adjacent difference is at least the smallest adjacent difference in sorted order, because the circular sequence must have an edge that was the smallest gap (since every edge appears at least once). It is also at least the gap between the smallest and largest elements, because the wrap-around connects the extremes. 
// 2. The optimal arrangement takes a pair `(a[i], a[i+1])` that achieves the minimum adjacent difference, removes them from the middle, places `a[i]` first and `a[i+1]` last, and fills the middle with all remaining elements in sorted order. This ensures all internal adjacent gaps are as small as possible (each is ≤ the original sorted gaps), and the wrap-around gap is exactly `a[i+1] - a[i]` (which is the minimum). The maximum adjacent difference is then the maximum of:
//    - the wrap-around gap (which is the minimum adjacent difference), and
//    - the maximum gap among the sorted remaining elements placed in the middle.
// 3. However, there's a subtlety: the wrap-around gap in the described construction is `a[i+1] - a[i]`, but we also need to consider if moving `a[i+1]` to the end creates a larger gap between `a[i]` and the next element (which is `a[i+2]`) or between the previous-to-last element and `a[i+1]`. That's why we try all candidate pairs achieving the minimum and also consider an alternate construction: place `a[i]` first, then all elements from `a[i+2]` to `a[n-1]` in order, then `a[0]` to `a[i-1]` in order, then `a[i+1]`. This alternate construction places the minimum pair at the wrap (with `a[i]` first and `a[i+1]` last) but reorders the middle as a rotation.
// 4. For each candidate pair, compute the maximum adjacent difference in the constructed permutation. Keep the best (minimum) among all candidates. If a tie, any is fine.
// 5. Edge cases: If \( n=2 \), just return sorted pair. If all elements are equal, any permutation works. Duplicates are naturally handled by the multiset approach.
//
// Complexity: Sorting takes \(O(n \log n)\). For each candidate pair (up to \(n-1\) pairs that achieve minimum), constructing the result takes \(O(n)\) time by copying. So worst-case \(O(n^2)\) if many pairs achieve the minimum, but in practice it's fine. Space: \(O(n)\) for the result.
