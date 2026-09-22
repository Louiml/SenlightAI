Write a C++ function `vector<int> validHomeworkKs(const vector<int>& scores)` that, given a non-empty sequence of `n` exam scores (where `n >= 3`), returns all integers `k` (with `1 <= k <= n-2`) such that after discarding the first `k` scores, then discarding the single minimum score among the remaining scores, the average of the remaining scores is equal to the maximum possible average achievable over all valid `k`. The function must return the qualifying `k` values in increasing order. Note: Each valid `k` removes exactly `k` elements from the beginning, then removes exactly one *minimum* element from the remaining (if multiple minima exist, only one copy is removed), and the average is computed over the leftover `n-k-1` scores as a double. The input list contains integers, possibly negative, and the result should list all `k` that achieve the maximum computed average.
The brute-force approach would test all `k` from 1 to `n-2`, each requiring finding the minimum of the suffix and summing it—this would be O(n²). We can do better with preprocessing.  
First, compute an array `suffixMin[i]` where `suffixMin[i]` is the minimum of scores from index `i` to the end. This is done in a single pass from right to left, O(n).  
Next, we need the sum of the suffix starting at index `k` (i.e., scores from index `k` to the end) efficiently. Instead of building a full prefix-sum array, notice that we can iterate `k` from 1 upward and maintain a running total. Start with `total = sum of all scores`. For `k=1`, the remaining scores are indices 1..n-1, so total becomes `total - scores[0]`. For `k=2`, subtract `scores[1]`, etc. This gives O(n) for all suffix sums.  
For each `k` (from 1 to n-2), the sum after removing the minimum from the suffix is `suffixSum - suffixMin[k]`, and the count is `n-k-1`. Compute average as `(double)(suffixSum - suffixMin[k]) / (n-k-1)`. Track the maximum average. Then after computing all averages, collect all `k` that achieve that maximum.  
Edge cases: The list has at least 3 elements (so that after removing `k` from the front and one minimum, at least one element remains). Negative scores are handled naturally. If multiple minima tie, we only remove one copy, which is exactly what `suffixMin[k]` gives—the minimum value itself. If a suffix has more than one element, the average is well-defined.  
Time complexity: O(n) for suffix minimum, O(n) for the loop over k, O(n) for collecting results. Space: O(n) for the suffixMin array and output vector.
#include <vector>
#include <algorithm>
#include <numeric>
#include <cstddef>

// Return all k (1..n-2) that yield the maximum possible average after
// discarding first k scores, then removing one minimum from the rest.
std::vector<int> validHomeworkKs(const std::vector<int>& scores) {
    const int n = static_cast<int>(scores.size());
    // Precompute suffix minimum: suffixMin[i] = min(scores[i..n-1])
    std::vector<int> suffixMin(n);
    int curMin = scores[n-1];
    for (int i = n-1; i >= 0; --i) {
        curMin = std::min(curMin, scores[i]);
        suffixMin[i] = curMin;
    }

    // Compute total sum of all scores for initial suffix sum
    int total = std::accumulate(scores.begin(), scores.end(), 0);
    std::vector<double> averages; // store average for each k (index 0 unused)
    averages.reserve(n-2);
    // For k=1, suffix is scores[1..n-1], so we subtract scores[0] first
    // We'll compute inside loop and update total accordingly.
    // Initialize total as if we already subtracted scores[0]? We'll handle in loop.
    // Easier: precompute suffixSum directly for each k via a suffix sum array.
    std::vector<int> suffixSum(n+1); // suffixSum[i] = sum scores[i..n-1]
    suffixSum[n] = 0;
    for (int i = n-1; i >= 0; --i) {
        suffixSum[i] = suffixSum[i+1] + scores[i];
    }

    double maxAvg = -1e18; // lower than any possible double
    std::vector<double> avgList(n); // use n for simplicity, index k
    for (int k = 1; k <= n-2; ++k) {
        int sumWithoutMin = suffixSum[k] - suffixMin[k];
        int count = n - k - 1;
        double avg = static_cast<double>(sumWithoutMin) / count;
        avgList[k] = avg;
        if (avg > maxAvg) maxAvg = avg;
    }

    // Collect all k that achieve maxAvg
    std::vector<int> result;
    for (int k = 1; k <= n-2; ++k) {
        // Use a tolerance for floating-point equality
        if (std::abs(avgList[k] - maxAvg) < 1e-9) {
            result.push_back(k);
        }
    }
    return result;
}
#include <cassert>
#include <cmath>
#include <vector>

// Assuming the solution function is declared above.
int main() {
    // Basic case: n=3, only k=1 possible
    {
        std::vector<int> scores = {10, 20, 30};
        std::vector<int> ks = validHomeworkKs(scores);
        assert(ks.size() == 1 && ks[0] == 1);
    }
    // Example with negative numbers
    {
        std::vector<int> scores = {-1, -2, -3, -4, -5};
        // k=1: remaining {-2,-3,-4,-5}, min=-5, sum=-9, avg=-2.25
        // k=2: remaining {-3,-4,-5}, min=-5, sum=-7, avg=-3.5
        // k=3: remaining {-4,-5}, min=-5, sum=-4, avg=-4.0
        // max avg = -2.25 for k=1
        std::vector<int> ks = validHomeworkKs(scores);
        assert(ks.size() == 1 && ks[0] == 1);
    }
    // Ties: two k give same max average
    {
        std::vector<int> scores = {5, 1, 1, 1, 5};
        // k=1: remaining {1,1,1,5}, min=1, sum=7, avg=7/3 ≈2.3333
        // k=2: remaining {1,1,5}, min=1, sum=6, avg=6/2=3.0
        // k=3: remaining {1,5}, min=1, sum=5, avg=5/1=5.0 -> but k=3 is allowed? n=5, k<=3, yes.
        // Actually check: k=3 removes {5,1,1}, remaining {1,5}, min=1, sum=5, avg=5.0
        // So max avg = 5.0 only for k=3.
        std::vector<int> ks = validHomeworkKs(scores);
        assert(ks.size() == 1 && ks[0] == 3);
    }
    // All identical: every k gives same average
    {
        std::vector<int> scores = {7, 7, 7, 7, 7};
        // For any k, after removing k copies and one 7, remaining sum = (n-k-1)*7, count = n-k-1, avg=7.
        // So k=1,2,3 all valid.
        std::vector<int> ks = validHomeworkKs(scores);
        assert(ks.size() == 3);
        assert(ks[0] == 1 && ks[1] == 2 && ks[2] == 3);
    }
    // Large n with a clear maximum
    {
        std::vector<int> scores = {100, 1, 2, 3, 4, 5, 6, 7};
        // Let's compute quickly:
        // n=8, k from 1 to 6
        // suffixMin: from index 1: min(1,2,3,4,5,6,7)=1; suffixMin[1]=1, suffixMin[2]=2, etc.
        // k=1: remaining {1,2,3,4,5,6,7}, min=1, sum=27, count=6, avg=4.5
        // k=2: remaining {2,3,4,5,6,7}, min=2, sum=26, count=5, avg=5.2
        // k=3: remaining {3,4,5,6,7}, min=3, sum=24, count=4, avg=6.0
        // k=4: remaining {4,5,6,7}, min=4, sum=21, count=3, avg=7.0
        // k=5: remaining {5,6,7}, min=5, sum=13, count=2, avg=6.5
        // k=6: remaining {6,7}, min=6, sum=7, count=1, avg=7.0
        // Max avg=7.0 occurs at k=4 and k=6
        std::vector<int> ks = validHomeworkKs(scores);
        assert(ks.size() == 2);
        assert(ks[0] == 4 && ks[1] == 6);
    }
    // Edge: n=3 only one k
    {
        std::vector<int> scores = {1, 2, 3};
        std::vector<int> ks = validHomeworkKs(scores);
        assert(ks.size() == 1 && ks[0] == 1);
    }
    return 0;
}
