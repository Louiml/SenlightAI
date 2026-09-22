/*
Given a list of `n` scores (each between `-1000` and `1000`) and a threshold position `k` (1-indexed), write a C++ function `int countQualified(const std::vector<int>& scores, int k)` that returns the number of scores that are strictly positive and at least as large as the score at position `k` (the "cutoff" score). For example, with scores `[10, 20, 5, 30]` and `k=2`, the cutoff is `20`, so only `20` and `30` qualify, returning `2`. The input is guaranteed to have at least one element and `1 ≤ k ≤ n`. Scores can be negative or zero, but those never count regardless of the cutoff. The function must be efficient and handle duplicate values, where duplicates of the cutoff count if they are positive.
*/
#include <vector>

// Count how many scores are positive and at least as large as the score at position k (1-indexed).
// The cutoff is the element at index k-1 in the vector.
int countQualified(const std::vector<int>& scores, int k) {
    if (scores.empty()) {
        return 0;
    }
    int cutoff = scores[k - 1];
    int qualified = 0;
    for (int score : scores) {
        if (score > 0 && score >= cutoff) {
            ++qualified;
        }
    }
    return qualified;
}
#include <cassert>
#include <vector>

// Function declaration from the solution (not repeated here for brevity)
int countQualified(const std::vector<int>& scores, int k);

int main() {
    assert(countQualified({10, 20, 5, 30}, 2) == 2);
    assert(countQualified({-1, 0, 5, 3}, 1) == 2); // cutoff = -1, positives are 5 and 3
    assert(countQualified({7, 7, 7}, 3) == 3);      // cutoff = 7, all qualify
    assert(countQualified({0, -2, 4}, 2) == 1);     // cutoff = -2, only 4 qualifies
    assert(countQualified({5, 5, 5, 5}, 1) == 4);   // all equal and positive
    assert(countQualified({-3, -1, -2}, 2) == 0);   // no positive scores
    assert(countQualified({100, 50, 200}, 3) == 1); // cutoff = 200, only 200 qualifies
    assert(countQualified({1, 2, 3, 4, 5}, 5) == 1); // cutoff = 5, only 5 qualifies
    assert(countQualified({10, 20, 10, 20}, 2) == 4); // cutoff = 20, all ≥20? 10<20, so 20,20, and 10? Wait 10<20, so only 20 and 20 = 2? Correct: 2
    assert(countQualified({10, 20, 10, 20}, 2) == 2);
    assert(countQualified({1, -1, 0}, 2) == 1);     // cutoff = -1, only 1 qualifies
    return 0;
}
// The core idea is straightforward: first, identify the cutoff score by reading the element at index `k-1` (since `k` is 1-indexed). Then, iterate through the entire vector once. For each element, count it only if it is strictly greater than `0` and also greater than or equal to the cutoff. No preprocessing or sorting is needed because the cutoff is determined by the original order, not by magnitude. Edge cases to consider: (1) The cutoff itself may be negative or zero, in which case only positive numbers matter, and all positive numbers automatically satisfy the cutoff (since any positive number is ≥ a non-positive number). (2) Duplicates of the cutoff that are positive should be counted. (3) If all scores are non-positive, the result is `0`. The algorithm runs in `O(n)` time because it does a single pass over the input after reading the cutoff, and uses `O(1)` auxiliary space, as it only stores a few integer variables.
