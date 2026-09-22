Given a non-empty vector of positive integers representing the scores of sightseeing spots along a route, write a C++ function `maxScoreSightseeingPair` that returns the maximum possible score from choosing exactly two distinct spots i and j (with i < j), where the score of a pair is defined as `values[i] + values[j] + i - j`. The function must compute this in a single pass through the vector, without using nested loops or extra memory proportional to the input size.
#include <cassert>
#include <vector>

int maxScoreSightseeingPair(const std::vector<int>& values);

int main() {
    assert(maxScoreSightseeingPair({8, 1, 5, 2, 6}) == 11);
    assert(maxScoreSightseeingPair({1, 2}) == 2);
    assert(maxScoreSightseeingPair({1, 2, 3}) == 4);
    assert(maxScoreSightseeingPair({10, 4, 8, 3, 9}) == 16);
    assert(maxScoreSightseeingPair({1, 1, 1, 1}) == 2);
    assert(maxScoreSightseeingPair({5, 1, 1, 1, 5}) == 8);
    assert(maxScoreSightseeingPair({100, 1, 1, 1}) == 101);
    assert(maxScoreSightseeingPair({1, 100, 1}) == 101);
    assert(maxScoreSightseeingPair({2, 2, 2, 2, 2}) == 4);
    assert(maxScoreSightseeingPair({7, 8, 9, 10}) == 18);
    return 0;
}
#include <vector>
#include <algorithm>

// Returns the maximum score of a sightseeing pair (i < j) where score = values[i] + values[j] + i - j.
int maxScoreSightseeingPair(const std::vector<int>& values) {
    int maxScore = 0;
    int bestPrev = values[0]; // Best (values[i] + i) seen so far, before applying distance penalty for current j
    
    for (std::size_t j = 1; j < values.size(); ++j) {
        // Current pair score using best previous i with penalty already applied
        maxScore = std::max(maxScore, bestPrev + values[j] - static_cast<int>(j));
        // Update bestPrev for future positions: apply penalty for moving one step
        bestPrev = std::max(bestPrev, values[j] + static_cast<int>(j));
    }
    
    return maxScore;
}
// The key insight is to rewrite the pair score as `(values[i] + i) + (values[j] - j)`. As we iterate from left to right, for each current position `j`, we need the maximum possible value of `(values[i] + i)` among all previous indices `i < j`. We maintain a running maximum `bestPrev` that stores the best `(values[i] + i)` seen so far, adjusted for the distance penalty. For each element `value` at index `j`, the candidate score is `value + bestPrev` (where `bestPrev` already accounts for the `-j` by decrementing each step). Then we update `bestPrev = max(bestPrev, value) - 1` because moving to the next index increases the distance penalty by 1 for all previous spots. Initialization: since the pair must have `i < j`, we start with `bestPrev = values[0] - 1` implicitly through the loop logic, but the standard approach sets `bestPrev` to `values[0]` and starts iterating from the second element, or uses the given snippet's method. Edge cases: vector length at least 2 (task states choosing two distinct spots); all values are positive so no overflow concerns with typical int. Time complexity O(n), space complexity O(1).
