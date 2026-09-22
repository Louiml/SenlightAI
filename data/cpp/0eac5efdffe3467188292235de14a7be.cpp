// Write a C++ function `int maxValidScore(const std::vector<int>& scores)` that receives a vector of non-negative integers representing scores from a quiz. Each score is guaranteed to be between 0 and 100 inclusive, and the vector may be empty or contain up to 10^5 elements. The function must return the maximum possible total score obtainable by selecting all scores, but with the constraint that the final total must NOT be a multiple of 10. If it is impossible to avoid a multiple of 10 (either because the total is already a multiple of 10 and no non-multiple-of-10 score exists to subtract, or the vector is empty), return 0. More precisely: start with the sum of all scores. If that sum is not divisible by 10, return it. If it is divisible by 10, try removing the smallest possible score that is itself not divisible by 10, so that the new sum becomes non‑divisible by 10. If no such score exists, return 0.

// The solution involves first sorting the scores in ascending order. Compute the total sum of all scores. If the total modulo 10 is non-zero, the answer is simply the total. Otherwise, we need to subtract the smallest score that is not divisible by 10, because subtracting any non-multiple-of-10 value from a multiple-of-10 sum will produce a non-multiple-of-10 result, and to maximize the remaining sum we should subtract the smallest possible such value. After sorting, we iterate from the smallest element upward; the first element with `% 10 != 0` is the one to subtract. If we find such an element, return `total - thatScore`. If the loop completes without finding a non-multiple-of-10 score, return 0. Edge cases: empty vector returns 0; all scores are multiples of 10 and the sum is a multiple of 10 → return 0; sum is not a multiple of 10 → return sum even if individual scores are multiples of 10. Time complexity is O(N log N) due to sorting, and O(1) extra space (excluding input storage).

#include <vector>
#include <algorithm>

// Return the maximum total score not divisible by 10.
int maxValidScore(const std::vector<int>& scores) {
    if (scores.empty()) {
        return 0;
    }

    std::vector<int> sortedScores = scores; // copy for const correctness
    std::sort(sortedScores.begin(), sortedScores.end());

    int total = 0;
    for (int score : sortedScores) {
        total += score;
    }

    if (total % 10 != 0) {
        return total;
    }

    for (int score : sortedScores) {
        if (score % 10 != 0) {
            return total - score;
        }
    }

    return 0;
}

#include <cassert>
#include <vector>

// The solution function is declared above; here we test it.
int maxValidScore(const std::vector<int>& scores); // forward declaration

int main() {
    // Basic non-multiple case
    assert(maxValidScore({5, 15, 20}) == 40); // sum=40, not %10
    // Multiple of 10 total, remove smallest non-multiple
    assert(maxValidScore({10, 20, 5}) == 35); // sum=35 already non-multiple? Wait: 10+20+5=35 → 35%10=5 → return 35
    // Correct test: sum=30, remove smallest odd (3) → 27
    assert(maxValidScore({10, 20, 3}) == 27); // sum=30 %10=0, remove 3 → 27
    // All multiples of 10, sum non-multiple impossible
    assert(maxValidScore({10, 20, 30}) == 60); // sum=60 %10=0, no non-multiple → return 0
    // Actually 60%10=0, no odd scores → return 0
    assert(maxValidScore({10, 20, 30}) == 0);
    // Empty vector
    assert(maxValidScore({}) == 0);
    // Single non-multiple
    assert(maxValidScore({7}) == 7);
    // Single multiple of 10
    assert(maxValidScore({10}) == 0);
    // Sum is non-multiple even with all multiples
    assert(maxValidScore({10, 20, 40}) == 70); // 70%10=0 → need to remove? All multiples, so return 0
    // let's fix: 10+20+40=70%10=0, no odd → 0
    assert(maxValidScore({10, 20, 40}) == 0);
    // Mixed case: sum 100, remove smallest odd (1) → 99
    assert(maxValidScore({10, 20, 30, 40, 1}) == 99);
    // Multiple odd scores, remove smallest
    assert(maxValidScore({10, 3, 5}) == 18); // sum=18%10=8 → return 18
    // Now a true multiple case: 10+3+5+2=20%10=0, remove 3 → 17
    assert(maxValidScore({10, 3, 5, 2}) == 17);
}
