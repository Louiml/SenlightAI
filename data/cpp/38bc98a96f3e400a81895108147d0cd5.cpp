/*
Write a C++ function `std::vector<std::pair<int,int>> segmentByVariance(const std::vector<int>& samples, int maxSegments)` that partitions a sequence of non-negative integers (representing pixel intensity or similar signal values) into at most `maxSegments` contiguous segments so that the sum over segments of the within-segment variance is minimized, where variance of a segment with values \(x_1,\dots,x_k\) and mean \(\mu\) is defined as \(\frac{1}{k}\sum_{i=1}^k (x_i - \mu)^2\). If `maxSegments` exceeds the number of samples, use exactly the number of samples as the limit. The function must return a list of the boundaries of the segments: for each segment, a pair `(startIndex, endIndex)` that is inclusive, where the first segment starts at index 0 and the last segment ends at index `samples.size()-1`. Segments must be non-empty and contiguous, and in the output, pairs must appear in increasing order of start index. If the input is empty, return an empty vector. If `maxSegments <= 0`, treat it as unlimited but still capped by the sample count. The implementation must avoid using floating-point arithmetic for variance comparison if possible, but if floating-point is used, it must be deterministic (i.e., same input always produces same output). Provide a self-contained free function named `segmentByVariance` and ensure it compiles with C++17.
*/
#include <vector>
#include <utility>
#include <cmath>
#include <limits>
#include <algorithm>

// Computes the variance of subarray samples[start..end] (inclusive) using prefix sums.
// Returns the variance as a long double for deterministic comparisons.
inline long double computeVariance(const std::vector<long long>& prefSum,
                                   const std::vector<long long>& prefSq,
                                   int start, int end) {
    long long n = end - start + 1;
    long long sum = prefSum[end + 1] - prefSum[start];
    long long sqSum = prefSq[end + 1] - prefSq[start];
    // variance = (sqSum/n) - (sum/n)^2 = (n*sqSum - sum^2) / n^2
    long double numerator = static_cast<long double>(n) * static_cast<long double>(sqSum)
                          - static_cast<long double>(sum) * static_cast<long double>(sum);
    long double denom = static_cast<long double>(n) * static_cast<long double>(n);
    return numerator / denom;
}

// Partitions samples into at most maxSegments contiguous segments, minimizing total variance.
// Returns a list of inclusive (start, end) index pairs for each segment.
std::vector<std::pair<int,int>> segmentByVariance(const std::vector<int>& samples, int maxSegments) {
    const int N = static_cast<int>(samples.size());
    if (N == 0) return {};

    // Effective number of segments: cap by sample count, and treat <=0 as unlimited.
    int K = (maxSegments <= 0) ? N : std::min(maxSegments, N);

    // Prefix sums (size N+1, prefSum[i] = sum of samples[0..i-1]).
    std::vector<long long> prefSum(N + 1, 0);
    std::vector<long long> prefSq(N + 1, 0);
    for (int i = 0; i < N; ++i) {
        prefSum[i + 1] = prefSum[i] + static_cast<long long>(samples[i]);
        long long v = static_cast<long long>(samples[i]);
        prefSq[i + 1] = prefSq[i] + v * v;
    }

    const long double INF = std::numeric_limits<long double>::infinity();

    // dp[seg][pos] = min total variance for first 'pos' samples (0..pos-1) using exactly 'seg' segments.
    std::vector<std::vector<long double>> dp(K + 1, std::vector<long double>(N + 1, INF));
    std::vector<std::vector<int>> prev(K + 1, std::vector<int>(N + 1, -1)); // prev[seg][pos] = best split point m

    dp[0][0] = 0.0L;
    for (int seg = 1; seg <= K; ++seg) {
        for (int pos = seg; pos <= N; ++pos) { // need at least 'seg' elements
            long double best = INF;
            int bestM = -1;
            // Try all possible start positions for the last segment
            for (int m = seg - 1; m < pos; ++m) {
                if (dp[seg - 1][m] >= INF) continue;
                long double cost = computeVariance(prefSum, prefSq, m, pos - 1);
                long double total = dp[seg - 1][m] + cost;
                if (total < best - 1e-12L) { // deterministic tie-breaking: strict <
                    best = total;
                    bestM = m;
                }
            }
            dp[seg][pos] = best;
            prev[seg][pos] = bestM;
        }
    }

    // Find the best number of segments: choose the smallest K' that achieves the minimum total variance.
    // Since using fewer segments may yield lower or equal variance? Actually more segments cannot increase the minimum.
    // The minimum over k of dp[k][N] is monotonic non-increasing with k, but we need to find the best k.
    long double bestTotal = INF;
    int bestSeg = 1;
    for (int seg = 1; seg <= K; ++seg) {
        if (dp[seg][N] < bestTotal - 1e-12L) {
            bestTotal = dp[seg][N];
            bestSeg = seg;
        }
    }

    // Reconstruct boundaries (inclusive start/end).
    std::vector<std::pair<int,int>> result;
    int pos = N;
    for (int seg = bestSeg; seg >= 1; --seg) {
        int m = prev[seg][pos]; // start index of the segment
        result.push_back({m, pos - 1});
        pos = m;
    }
    // The above pushes segments from last to first, so reverse.
    std::reverse(result.begin(), result.end());
    return result;
}
#include <cassert>
#include <vector>
#include <utility>
#include <cmath>

// Bring the solution function into scope; assume it is defined above.

int main() {
    // Empty input
    assert(segmentByVariance({}, 3).empty());

    // Single element: one segment (0,0)
    auto r1 = segmentByVariance({5}, 3);
    assert(r1.size() == 1);
    assert(r1[0] == std::make_pair(0, 0));

    // All equal values: variance zero, any segmentation is optimal; returns one segment (0,n-1)
    auto r2 = segmentByVariance({3,3,3,3}, 4);
    assert(r2.size() == 1);
    assert(r2[0] == std::make_pair(0, 3));

    // Two distinct values, maxSegments=1 -> one segment
    auto r3 = segmentByVariance({0,10}, 1);
    assert(r3.size() == 1);
    assert(r3[0] == std::make_pair(0, 1));

    // Two distinct values, maxSegments=2 -> split into two segments each of size 1 (variance zero each)
    auto r4 = segmentByVariance({0,10}, 2);
    assert(r4.size() == 2);
    assert(r4[0] == std::make_pair(0, 0));
    assert(r4[1] == std::make_pair(1, 1));

    // A more complex sequence: {1,2,3,10,11,12}
    // Best with 2 segments: split after index 2 (i.e., [0..2] and [3..5]) gives variance near 0.666 each.
    // Check that the split is at 2.
    auto r5 = segmentByVariance({1,2,3,10,11,12}, 2);
    assert(r5.size() == 2);
    assert(r5[0].first == 0 && r5[0].second == 2);
    assert(r5[1].first == 3 && r5[1].second == 5);

    // maxSegments > N: should cap to N and produce N single-element segments.
    auto r6 = segmentByVariance({7,8,9}, 10);
    assert(r6.size() == 3);
    assert(r6[0] == std::make_pair(0, 0));
    assert(r6[1] == std::make_pair(1, 1));
    assert(r6[2] == std::make_pair(2, 2));

    // maxSegments <= 0 treated as unlimited -> yields N segments for distinct values.
    auto r7 = segmentByVariance({4,5,6}, 0);
    assert(r7.size() == 3);
    assert(r7[0] == std::make_pair(0, 0));
    assert(r7[1] == std::make_pair(1, 1));
    assert(r7[2] == std::make_pair(2, 2));

    // Duplicate values: segmentation with 2 segments should split optimally.
    // {5,5,1} -> best split either after 0 or 1, both give variance 0 for one segment and 4/3 for other.
    // The DP will pick the first optimal by tie-breaking; either is acceptable, but we assert shape.
    auto r8 = segmentByVariance({5,5,1}, 2);
    assert(r8.size() == 2);
    // Both possibilities yield total variance 4/3; just check it's a valid partition.
    assert(r8[0].first == 0);
    assert(r8[1].second == 2);
    assert(r8[0].second + 1 == r8[1].first);

    return 0;
}
// This is a classic dynamic programming problem for optimal segmentation with additive cost. The key is to precompute segment costs efficiently. For any subarray from index \(i\) to \(j\), the variance of the values can be computed using the sum of values and sum of squares: \(\text{var}(i,j) = \frac{1}{n}\left(\text{sqsum}(i,j) - \frac{\text{sum}(i,j)^2}{n}\right)\) where \(n=j-i+1\). To avoid floating-point precision issues that could affect tie-breaking, we can scale the variance by \(n^2\) so that the cost becomes an integer: \(\text{scaledCost}(i,j) = n \cdot \text{sqsum} - \text{sum}^2\), which is always a non-negative integer. Minimizing the sum of these scaled costs is equivalent to minimizing the sum of variances for the same segmentation because scaling each segment cost by its length (which changes with segmentation) is not uniform. Actually, careful: the scaled cost differs from the variance by a factor of \(n^2\) divided by \(n\) = \(n\), so multiplying each segment's variance by its length gives \(n \cdot \text{var} = \text{sqsum} - \frac{\text{sum}^2}{n}\). That is not integer necessarily, but multiplying further by \(n\) gives \(n \cdot \text{sqsum} - \text{sum}^2\), which is integer and differs from variance by factor \(n^2\) per segment. Since each segment has its own \(n\), summing these scaled costs does not exactly correspond to summing variances (because the scaling factor varies). However, since the goal is to minimize the sum of variances, we can directly work with double precision variance but with careful comparison using an epsilon? Alternatively, we can note that the minimization of sum of variances is equivalent to minimizing the sum of `n * variance` because the total sum of `n * variance` equals sum of `(n_i * variance_i)` which is not a constant offset. Actually, the sum of `n_i * variance_i` is not a scaled version of sum of variance unless all segments have equal length. So we must compute the exact variance sum. To keep integer arithmetic, we can represent each variance as a rational number with denominator `n`; summing fractions requires careful handling, but we can compare sums of fractions with common denominators across different segmentations? That is complicated. Simpler: use `long double` for cost and avoid comparing non-exact values by using a tolerance? But the problem expects deterministic results and exact tie-breaking. Since the variance is a quadratic function over sums, for the dynamic programming recurrence `dp[k][i] = min_{j < i} (dp[k-1][j] + cost(j+1,i))`, the cost is monotonic and convex? Not necessarily, but typical inputs are small enough that we can use double. To ensure deterministic tie-breaking, we can compute cost as `long double` and compare with a small epsilon, but ties might be broken inconsistently. Instead, we can compute the scaled cost `cost_exact = n * sqsum - sum^2` which is an integer. Note that `sum of variances` = `sum ( (sqsum_i - sum_i^2/n_i) )`. To minimize that sum, we can equivalently minimize `sum (n_i * (sqsum_i - sum_i^2/n_i)) = sum(n_i * sqsum_i - sum_i^2)` because `n_i` multiplies each term. But that is not a constant addition across all segmentations; it's just a different objective. However, if we define `cost'(i,j) = n * sqsum - sum^2`, then the sum of these costs over segments is `sum(n_i * sqsum_i - sum_i^2)`. This is exactly `sum of (n_i * variance_i)`. Minimizing this is not the same as minimizing the sum of variances. But note that `sum of variances` = `sum( (sqsum_i - sum_i^2/n_i) )`. To minimize that, we can use DP with double. To guarantee deterministic results, we can avoid floating-point by using a common denominator for all segment costs? The total number of samples N, each segment cost has denominator `n_i`. Summing fractions with different denominators requires common denominator. We could compute the sum of variances as a rational number with denominator `N`? Because each segment cost can be represented as `(n_i * sqsum_i - sum_i^2) / n_i`. To compare two sums, we would need to compare `sum(A_i / n_i)` vs `sum(B_i / n_i)`. This is not straightforward with integers alone. Given typical constraints (small N), using `long double` is acceptable and deterministic if we avoid tie-breaking based on floating-point equality. But the problem statement asks for deterministic output for same input, which is satisfied if we use a stable algorithm with `long double` and no reliance on exact equality; comparisons with `<` are deterministic because `long double` arithmetic is deterministic. To avoid issues with `nan` or overflow, we can compute sums using `long double`. The main algorithm: Precompute prefix sums `prefSum[i] = sum_{0..i-1}` and `prefSq[i] = sum_{0..i-1} sq(value)`. Then `cost(i,j) = variance = ( (prefSq[j+1]-prefSq[i]) - ((prefSum[j+1]-prefSum[i])^2)/(j-i+1) ) / (j-i+1)`. Simpler: `cost(i,j) = ( (j-i+1) * (prefSq[j+1]-prefSq[i]) - (prefSum[j+1]-prefSum[i])^2 ) / ( (j-i+1)^2 )`? Let's derive: variance = (1/n) * (sqsum - sum^2/n) = (n*sqsum - sum^2) / n^2. So to minimize sum of variances, we can compute `dp[k][i] = min_{j} (dp[k-1][j] + (n*sqsum - sum^2)/n^2)`. Since n varies, we cannot combine denominators. We'll just use double. DP state: `dp[seg][pos]` = minimum total variance for first `pos` samples (indices 0..pos-1) partitioned into `seg` segments. Initialize `dp[0][0]=0`, others infinity. For seg>=1, `dp[seg][pos] = min_{m<pos} dp[seg-1][m] + variance(m, pos-1)`. Answer is `dp[k][N]` for any k <= maxSegments, take the minimum over k (since we can use up to maxSegments, not necessarily exactly). Then reconstruct the segmentation by storing the best `m` for each (seg,pos). Finally, output pairs of (start, end) inclusive. Edge cases: empty input -> empty vector. maxSegments==0 -> treat as unlimited, i.e., set limit = N. maxSegments > N -> limit = N. Negative values not allowed per problem statement but we can handle them (the variance formula works for any integers, but the function's spec says non-negative). Complexity: O(K * N^2) where K is the effective maxSegments (bounded by N), so O(N^3) worst-case. For N up to, say, 200, this is fine. Use `std::pair<int,int>` for boundaries.
