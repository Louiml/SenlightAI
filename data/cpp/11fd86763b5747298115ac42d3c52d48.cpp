Write a C++ function `int bestCupCount(int hot, int cold, int target)` that determines which finite sequence of alternating pours of hot water at temperature `hot` and cold water at temperature `cold` (in degrees Celsius) yields an average temperature closest to a given target temperature `target`. The sequence always starts with a hot cup (1 cup of hot water) and then alternates: hot, cold, hot, cold, and so on. The total number of cups poured can be any positive odd integer (since we always start with hot and alternate, the total is always odd). Return the smallest number of cups (total count) whose average temperature is closest to `target`. If two different total cup counts give equally close averages, return the smaller count. Temperatures are integers, with `0 ≤ cold < target ≤ hot ≤ 10^9` (the target is strictly between cold and hot, but the function should also handle cases where target equals hot or cold gracefully). The function must handle large inputs efficiently without floating-point precision issues.

// The average temperature after pouring `2k+1` cups (k+1 hot, k cold) is `((k+1)*hot + k*cold) / (2k+1)`. The sequence of averages for increasing k starts near `hot` (for k=0, average = hot) and monotonically decreases toward `(hot+cold)/2` as k → ∞, but never actually reaches that limit because the number of hot cups always exceeds cold cups by exactly one. Therefore, for any target strictly between cold and hot, there are two possibilities: either the target is below the limit `(hot+cold)/2` (then the closest is always k=1, i.e., 3 cups, because the sequence can never go below the limit), or the target is above the limit. In the latter case, we can binary search or solve directly for the integer k that makes the average closest to target. Observing that the difference between consecutive averages is `(hot-cold) / ((2k+1)(2k+3))`, we can find the threshold where the average crosses the target. A direct formula: solve `((k+1)h + kc)/(2k+1) = t` for k, giving `k = (h - t) / (2t - h - c)` (the floor of this rational). Then check the two neighboring k values (floor and ceil) and also k=0 (1 cup) because the sequence starts at hot. Edge cases: if target == hot, return 1; if target == cold, no finite sequence can reach exactly cold (since average > cold always), so return the smallest count that gets closest—which is k=1 (3 cups) because that gives the lowest achievable average `(2h+c)/3` and is closest to cold. Also, if target ≤ (h+c)/2, the closest is always 3 cups because the limit is approached from above and the smallest k gives the highest average, but actually we must compare: the average for k=1 is `(2h+c)/3` which is less than or equal to? For target ≤ limit, the closest is indeed k=1 because all averages are > limit ≥ target, and the sequence decreases, so the smallest average (k=1) is closest. But careful: for k=0 (1 cup) average = h, which is far. So return 3. For general case where target > limit, we compute candidate k values using the formula: `k1 = floor((h - t) / (2t - h - c))` (but if denominator is zero or negative, handle). Since t > limit, denominator `2t - h - c` > 0. Let `k = (h - t) / (2t - h - c)` as an integer division? Better to compute using `long double` or `long long` to avoid overflow (values up to 1e9, squares up to 1e18, fits in 64-bit). Then check k, k+1, and also k=0. Compare absolute difference using cross-multiplication to avoid floating point: compare `abs(target*(2k+1) - ((k+1)h + kc)) * (2m+1)` vs similar for m. Time complexity O(1) per call, space O(1).

#include <cstdint>
#include <cstdlib>
#include <limits>

// Returns the smallest odd number of cups (1, 3, 5, ...) whose average
// temperature is closest to the target. The sequence starts with hot.
int bestCupCount(int hot, int cold, int target) {
    // If target equals hot, one cup gives exact match.
    if (target == hot) return 1;

    // For target at or below the asymptotic limit (hot+cold)/2,
    // the closest achievable average is from 3 cups (k=1), because
    // averages are always above the limit and decrease with more cups.
    // Use long long to avoid overflow.
    long long h = hot, c = cold, t = target;
    long long limit_num = h + c;  // numerator of (h+c)/2

    // Compare target <= (h+c)/2  <=>  2*target <= h+c
    if (2 * t <= h + c) {
        // If target is even lower than cold? But cold < target by spec,
        // but we still handle gracefully: the closest is 3 cups.
        return 3;
    }

    // Now target > (h+c)/2, so denominator 2t - h - c > 0.
    // Solve k = (h - t) / (2t - h - c) approximately.
    // Use long double for precision, then check floor and ceil.
    long double numerator = static_cast<long double>(h - t);
    long double denominator = static_cast<long double>(2 * t - h - c);
    long double k_exact = numerator / denominator;

    // Possible k values to check: floor(k_exact), floor(k_exact)+1, and 0.
    // k=0 corresponds to 1 cup (already handled but keep for safety).
    long long k_candidates[3] = {0, 
                                  static_cast<long long>(k_exact), 
                                  static_cast<long long>(k_exact) + 1};
    // Ensure k >= 0.
    for (int i = 1; i < 3; ++i) {
        if (k_candidates[i] < 0) k_candidates[i] = 0;
    }

    long long best_k = 0;
    long long best_diff_num = std::numeric_limits<long long>::max();
    // Compute best using cross-multiplication to avoid floating point.
    for (int idx = 0; idx < 3; ++idx) {
        long long k = k_candidates[idx];
        // Average numerator: (k+1)*h + k*c  over denominator (2k+1).
        long long avg_num = (k + 1) * h + k * c;
        long long avg_den = 2 * k + 1;
        // Difference = |target - avg_num/avg_den|  = |t*avg_den - avg_num| / avg_den.
        // To compare two diffs, compare |t*avg_den - avg_num| * other_den.
        long long diff_num = std::llabs(t * avg_den - avg_num);
        // Compare using cross multiplication with current best.
        // best_diff_num / best_den vs diff_num / avg_den.
        // We need best_den = 2*best_k+1.
        long long best_den = 2 * best_k + 1;
        // Compare: diff_num * best_den < best_diff_num * avg_den
        if (idx == 0 || 
            diff_num * best_den < best_diff_num * avg_den ||
            (diff_num * best_den == best_diff_num * avg_den && k < best_k)) {
            best_k = k;
            best_diff_num = diff_num;
        }
    }

    // Convert k to total cups: 2k+1.
    return static_cast<int>(2 * best_k + 1);
}

#include <cassert>
int main() {
    // Edge: target equals hot -> 1 cup.
    assert(bestCupCount(100, 0, 100) == 1);
    assert(bestCupCount(10, 4, 10) == 1);

    // Target below the mean (but not equal) -> 3 cups.
    assert(bestCupCount(100, 0, 49) == 3);
    assert(bestCupCount(10, 4, 6) == 3);   // mean=7, target=6 < 7 -> 3 cups avg 8 diff2, 5 cups avg 7.6 diff1.6? Wait 6<7, so closest should be 3? Actually 5 cups avg 7.6 diff1.6, 3 cups avg 8 diff2, so 5 cups is closer! But our logic says if target < mean (7), then all averages >7, the smallest average is at k=∞ limit=7, but finite k=2 gives 7.6, k=1 gives 8, so k=2 is closer. That contradicts my earlier claim! Let's re-evaluate: The average decreases with k, so larger k gives lower average, which is closer to the target below the mean. So my earlier claim was wrong! For target below mean, the closest is the largest possible k? But there is no largest. Actually since the sequence is unbounded in k, the average approaches the mean from above, so for any target strictly below the mean, the difference to target decreases with k and approaches a positive limit (mean - target) but never reaches 0. So the infimum is at k→∞, but no finite k achieves that. So the problem is ill-posed for target < mean! The original snippet handles this by printing "2" for target < mean, which is not odd. So I must reconsider the problem statement. The original snippet's logic: if t < (h+c)/2, they print 2. But that's not a valid cup count. Perhaps the intended problem is different: the sequence alternates hot and cold, but you can stop after any number of cups (not necessarily odd)? But the snippet uses odd counts only in the else branch. Actually the snippet's first check: if t==h print 1; else if (h+c)/2 == t print (h+c)/t (which is 2); else if t < (h+c)/2 print 2; else compute n. So they output 2 in many cases, which suggests the total number of cups is not necessarily odd? But the snippet's formula for n uses odd counts. It's inconsistent.

Given the complexity, I need to redefine the task to be well-posed. The cleanest is: The sequence can be any positive integer total cups, alternating starting with hot. Then the average after n cups is: if n is odd, n=2k+1 -> maybe different formula; if n is even, n=2k -> k hot and k cold, average = (h+c)/2 exactly for all even n. That makes sense: if you pour an even number of cups alternating, you get equal hot and cold, so average is exactly the mean. Therefore, if target equals the mean, the best is 2 cups (the smallest even number). If target is below the mean, then even counts give the mean (which is above target) and odd counts give averages above the mean (even higher), so the closest is the smallest even count, i.e., 2 cups? Actually mean is above target, but odd counts are even higher, so the closest is indeed the smallest even count, 2 cups, giving the mean. Similarly, if target is above the mean, even counts give the mean (below target) and odd counts give averages above mean, potentially closer. So the problem becomes well-defined: the total number of cups can be any positive integer, and you pour alternating starting with hot. Then for odd n, average > mean; for even n, average = mean exactly. Then the answer is straightforward: if target == hot, return 1; if target == mean, return 2; if target < mean, return 2 (because even counts give exact mean, which is the closest possible since you can't go below mean with odd counts); if target > mean, then you need to find the odd n that gives closest above, and compare with 2 (which gives mean). This is a clean problem.

Thus I will rewrite the task accordingly: The function should return the smallest number of cups (positive integer) in an alternating sequence starting with hot (so cup 1 is hot, cup 2 cold, etc.) whose average temperature is closest to target. The average after n cups is: if n even (n=2k), average = (h+c)/2; if n odd (n=2k+1), average = ((k+1)h + kc)/(2k+1) > (h+c)/2. Then the solution is simple: if target == hot -> 1; else if 2*target <= h+c -> 2 (because even counts give mean which is >= target, and odd counts give even higher, so mean is closest). Actually if target == mean, 2 is exact. If target < mean, 2 gives the closest (mean) because you cannot get below mean. So return 2 in all cases where target <= mean. If target > mean, then we need to compare 2 (mean) with the closest odd count. Since odd counts decrease toward mean but stay above, we find the k that gives the best average above target. Solve for k as before, check k, k+1, and compare with 2. Finally return the smallest count among those that tie.

This is well-posed. I'll present that as the task.
