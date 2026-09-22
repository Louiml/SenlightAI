// Write a C++ function `countQualifiedParticipants` that takes a vector of non-negative integers representing contestant scores (where a score of 0 means the contestant did not participate), an integer `n` (the number of contestants), and an integer `k` (the 0-based index of the target contestant). The function must return the number of contestants who both have a positive score and have a score greater than or equal to the score of the k-th contestant (using 0-based indexing). The function should work for any `n` from 1 to 100, any `k` in `[0, n-1]`, and handle cases where the target contestant has a score of 0 (in which case only other positive scorers with score ≥ 0 are counted, but since 0 is not > 0, effectively only positive scorers are counted). The function must not modify the input vector.
#include <cassert>
#include <vector>

// The solution function is declared above in the actual test, but for completeness:
// int countQualifiedParticipants(const std::vector<int>&, int, int);

int main() {
    // Basic case: k-th contestant has positive score, count ties and higher.
    assert(countQualifiedParticipants({5, 5, 3, 0, 4}, 5, 1) == 2); // scores >=5: idx0, idx1
    // k-th contestant has zero score: only positive scorers count.
    assert(countQualifiedParticipants({1, 0, 2, 0, 3}, 5, 1) == 3); // all positive: 1,2,3
    // All zeros => answer is 0.
    assert(countQualifiedParticipants({0, 0, 0}, 3, 2) == 0);
    // Single contestant, positive score => counts itself.
    assert(countQualifiedParticipants({7}, 1, 0) == 1);
    // Single contestant, zero score => 0.
    assert(countQualifiedParticipants({0}, 1, 0) == 0);
    // k-th has highest score, duplicates count.
    assert(countQualifiedParticipants({2, 4, 4, 4, 1}, 5, 2) == 3); // idx1,2,3
    // k-th has lowest positive score, all positive count.
    assert(countQualifiedParticipants({3, 1, 2, 0}, 4, 1) == 3); // 3,1,2
    // Large n (100) with all positive and k at 0, all count if all equal.
    std::vector<int> big(100, 10);
    assert(countQualifiedParticipants(big, 100, 0) == 100);
    // k at last index, scores decreasing.
    assert(countQualifiedParticipants({9, 8, 7}, 3, 2) == 1); // only 7
    return 0;
}
#include <vector>
#include <cstddef>

// Counts contestants with positive score >= the score of the k-th contestant.
// Parameters:
//   scores - vector of non-negative integers (0 means no participation)
//   n      - number of contestants (must equal scores.size())
//   k      - 0-based index of the target contestant (must be in [0, n-1])
// Returns: number of qualifying contestants.
int countQualifiedParticipants(const std::vector<int>& scores, int n, int k) {
    if (n <= 0 || k < 0 || k >= n || static_cast<size_t>(n) != scores.size()) {
        return 0; // Invalid input guard.
    }
    int threshold = scores[k];
    int count = 0;
    for (int i = 0; i < n; ++i) {
        if (scores[i] > 0 && scores[i] >= threshold) {
            ++count;
        }
    }
    return count;
}
// The problem is straightforward: iterate through all `n` contestants, and for each one check two conditions: (1) `a[i] > 0` (they actually participated and scored positively), and (2) `a[i] >= a[k]` (their score is at least as high as the k-th contestant's score). If both hold, increment a counter. Since the target contestant's own score might be 0, condition (1) ensures we never count a 0-score contestant, and condition (2) then works naturally: if `a[k]` is 0, then any positive score is ≥ 0, so all positive scorers count. If `a[k]` is positive, then only those with at least that score count (including the k-th contestant itself if its score is positive). Edge cases: if all scores are 0, the answer is 0; if the k-th contestant has the highest positive score, only contestants tied at that score count. Time complexity is O(n) because we scan the array once. Space complexity is O(1) auxiliary, since we only use a few integer variables.
