// Write a C++ function `maxScoreFromCards` that accepts a vector of non-negative integers representing points on cards arranged in a row and a positive integer `k`. You may only take cards from either the beginning or the end of the array in each of exactly `k` turns, and each turn removes that card permanently. Return the maximum total score achievable by picking exactly `k` cards under this rule. The input vector will contain at least `k` elements, and all values are non-negative. For example, given `[1,2,3,4,5,6,1]` and `k=3`, the optimal strategy is to take the two leftmost cards `1` and `2` and the rightmost card `1`, yielding `4`.
#include <cassert>
#include <vector>

// The solution function is assumed to be declared above.
int main() {
    // Basic examples
    assert(maxScoreFromCards({1, 2, 3, 4, 5, 6, 1}, 3) == 12);
    assert(maxScoreFromCards({9, 7, 7, 9, 7, 7, 9}, 7) == 55); // all cards
    assert(maxScoreFromCards({1, 1000, 1}, 1) == 1); // must pick from ends, both are 1
    assert(maxScoreFromCards({100, 40, 17, 9, 73, 75}, 3) == 248); // 100+75+73

    // Edge cases
    assert(maxScoreFromCards({5}, 1) == 5);
    assert(maxScoreFromCards({0, 0, 0}, 2) == 0);
    assert(maxScoreFromCards({1, 2, 3}, 3) == 6); // pick all
    assert(maxScoreFromCards({11, 49, 100}, 2) == 149); // pick 100 and 49 from right

    // k = 1: just pick the larger of the two ends
    assert(maxScoreFromCards({1, 2, 3, 4}, 1) == 4); // pick 4 from right
    assert(maxScoreFromCards({8, 1, 2, 3}, 1) == 8); // pick 8 from left

    // k = size - 1: all but one, must drop the smallest possible interior card
    // n=5, k=4 -> must drop one card; drop the smallest, which is 1
    assert(maxScoreFromCards({1, 5, 3, 4, 2}, 4) == 14); // 5+3+4+2 = 14

    return 0;
}
#include <vector>
#include <algorithm>

// Returns the maximum score obtainable by picking exactly k cards from either end of the row.
int maxScoreFromCards(const std::vector<int>& cardPoints, int k) {
    const int n = static_cast<int>(cardPoints.size());
    if (k <= 0) return 0;
    if (k >= n) {
        int total = 0;
        for (int points : cardPoints) total += points;
        return total;
    }

    // Initial score: take the first k cards from the left.
    int currentScore = 0;
    for (int i = 0; i < k; ++i) {
        currentScore += cardPoints[i];
    }
    int bestScore = currentScore;

    // Replace leftmost selected cards with cards from the right end.
    // We maintain a window of k cards: initially indices [0..k-1] and later mix.
    // We simulate moving from taking k left cards to taking k right cards.
    for (int i = 0; i < k; ++i) {
        // Remove the (k-1-i)th card from the left (index k-1-i) and add card from the right end (index n-1-i).
        currentScore -= cardPoints[k - 1 - i];
        currentScore += cardPoints[n - 1 - i];
        bestScore = std::max(bestScore, currentScore);
    }

    return bestScore;
}
// The key observation is that picking exactly `k` cards from the two ends is equivalent to taking some number of cards from the left prefix (say `i` cards) and the remaining `k - i` cards from the right suffix. Since the order of removal does not affect the sum, we can consider all possible splits: choose `i` from the left (where `i` ranges from 0 to `k`) and `k-i` from the right. Therefore, the maximum score is the maximum over all `i` of `sum(left[0..i-1]) + sum(right[n-(k-i)..n-1])`. A simple and efficient way to compute this is to first compute the sum of the first `k` cards (i.e., take all from the left), then iteratively replace one leftmost card from the current selection with the next card from the right end, updating the running sum and tracking the maximum. The algorithm runs in `O(k)` time because we only compute a prefix sum of the first `k` elements and then perform `k` constant-time updates. It uses `O(1)` extra space. Edge cases: if `k` equals the size of the array, the answer is simply the total sum of all cards; if `k=0` (although the task says positive `k`, but defensively handle it) the answer is 0. The code should also work with duplicate values and when cards are all zero.
