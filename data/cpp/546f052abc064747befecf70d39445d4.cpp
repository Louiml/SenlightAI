// Write a C++ function named `canBothReachTargets` that takes four integers: `s1`, `s2`, `a1`, and `a2`. It returns `true` if it is possible for a team to have at least `s1` points in match 1 and at least `s2` points in match 2, given that the actual scores are `a1` and `a2` respectively, and the team’s final score is the sum of the two match scores. The condition is that the team’s total score must be at least the sum of the target scores (`s1 + s2`) AND at least one of the following must hold: either the team scores at least `s1` in match 1 and at least `s2` in match 2, OR the team scores at least `s2` in match 1 and at least `s1` in match 2 (i.e., the match order doesn’t matter, just that both targets are met in some permutation of the matches). However, the function should also return `true` if the actual scores already meet the targets in either order (i.e., `a1 >= s1 && a2 >= s2` or `a1 >= s2 && a2 >= s1`), even if the total sum condition is trivially satisfied. If none of these conditions hold, return `false`. The function must handle negative numbers gracefully; targets and scores can be any integers.

// The problem is essentially deciding whether the given actual scores (`a1`, `a2`) are sufficient to satisfy two target thresholds (`s1`, `s2`) when the match scores can be permuted. The key observation is that we only care about comparing the sorted order of the targets and the sorted order of the actual scores. The simplest correct approach is: sort the target scores and sort the actual scores, then check if after sorting, the actual scores are both greater than or equal to the corresponding sorted targets. That is, after sorting, we need `min(a1,a2) >= min(s1,s2)` and `max(a1,a2) >= max(s1,s2)`. This condition is equivalent to the two possible orderings being satisfied: either `a1>=s1 && a2>=s2` or `a1>=s2 && a2>=s1`. Since the problem statement explicitly says that either ordering is acceptable, this sorted comparison covers all cases. There is no need to check the sum condition because if both sorted comparisons hold, the sum condition automatically holds (since each actual score is at least the corresponding target). Conversely, if the sum condition holds but neither ordering matches, it’s impossible; but the sorted comparison will catch that. Edge cases include equal values, negative numbers, and zero – sorting and comparing still works correctly. Time complexity is O(1) since we only do a few comparisons and possibly a swap; space complexity is O(1). We do not need any loops or extra data structures.

#include <algorithm>

/**
 * Determines if a team’s actual scores satisfy two target thresholds in either match order.
 *
 * @param s1 Target score for match 1.
 * @param s2 Target score for match 2.
 * @param a1 Actual score for match 1.
 * @param a2 Actual score for match 2.
 * @return true if the actual scores meet the targets in either order, false otherwise.
 */
bool canBothReachTargets(int s1, int s2, int a1, int a2) {
    // Sort targets and actual scores in non-decreasing order.
    if (s1 > s2) std::swap(s1, s2);
    if (a1 > a2) std::swap(a1, a2);
    // After sorting, the smaller actual must meet the smaller target,
    // and the larger actual must meet the larger target.
    return a1 >= s1 && a2 >= s2;
}

#include <cassert>

bool canBothReachTargets(int, int, int, int); // declaration

int main() {
    // Simple positive case: both actual scores exceed targets.
    assert(canBothReachTargets(5, 3, 6, 4) == true);
    // Exact match.
    assert(canBothReachTargets(5, 3, 5, 3) == true);
    // Swap order: targets (3,5), actual (5,3) still works.
    assert(canBothReachTargets(3, 5, 5, 3) == true);
    // One target not met in either order.
    assert(canBothReachTargets(5, 5, 4, 6) == false);
    // Negative numbers: targets negative, actual positive.
    assert(canBothReachTargets(-2, -5, 0, 1) == true);
    // Zero targets.
    assert(canBothReachTargets(0, 0, -1, 2) == false);
    // One target met but the other not.
    assert(canBothReachTargets(10, 1, 9, 2) == false);
    // Large numbers.
    assert(canBothReachTargets(1000000000, 1, 1000000001, 1) == true);
    // Both actual scores less than both targets.
    assert(canBothReachTargets(5, 6, 4, 5) == false);
    return 0;
}
