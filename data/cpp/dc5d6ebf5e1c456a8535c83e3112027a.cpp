Write a standalone C++ function `findBestAndWorst` that takes a vector of candidate score pairs, where each pair consists of a model score and a BLEU score, and returns a struct containing the indices (or `size_t` values) of the candidate that maximizes `model_score + bleu_score` (the "best" candidate) and the candidate that minimizes `model_score + bleu_score` (the "worst" candidate), assuming all scores are finite and the vector is non-empty. In case of ties, return the smallest index for both best and worst. The function should handle negative and zero scores correctly, and should not modify the input. For example, given `{{1.0, 2.0}, {3.0, -1.0}, {0.5, 0.5}}`, the best is index 0 (sum=3.0) and the worst is index 1 (sum=2.0).
// The solution is straightforward: iterate through the vector once, computing the sum `model + bleu` for each candidate. Keep track of the maximum sum and its index, and the minimum sum and its index. Initialize both trackers with the first candidate's sum and index. For each subsequent candidate, update the best if the new sum is strictly greater than the current best (to preserve smallest index on ties), and update the worst if the new sum is strictly less than the current worst (to preserve smallest index on ties). Edge cases: an empty vector is not allowed per spec; ties are resolved by keeping the earlier index due to using strict comparisons. Scores can be negative, so initialize with the first element. Time complexity is O(n), space complexity is O(1) aside from the returned struct.
#include <vector>
#include <cstddef>

struct BestWorst {
    size_t best_index;
    size_t worst_index;
};

// Returns indices of candidate with max (model + bleu) and min (model + bleu).
// Assumes non-empty vector and all scores finite.
BestWorst findBestAndWorst(const std::vector<std::pair<double, double>>& candidates) {
    double best_sum = candidates[0].first + candidates[0].second;
    double worst_sum = best_sum;
    size_t best_index = 0;
    size_t worst_index = 0;

    for (size_t i = 1; i < candidates.size(); ++i) {
        double sum = candidates[i].first + candidates[i].second;
        if (sum > best_sum) {
            best_sum = sum;
            best_index = i;
        }
        if (sum < worst_sum) {
            worst_sum = sum;
            worst_index = i;
        }
    }

    return {best_index, worst_index};
}
#include <cassert>
#include <vector>
#include <utility>

int main() {
    using Candidate = std::pair<double, double>;
    
    // Basic test
    std::vector<Candidate> v1 = {{1.0, 2.0}, {3.0, -1.0}, {0.5, 0.5}};
    auto r1 = findBestAndWorst(v1);
    assert(r1.best_index == 0);
    assert(r1.worst_index == 1);

    // All equal sums -> smallest index for both
    std::vector<Candidate> v2 = {{1.0, 1.0}, {2.0, 0.0}, {-1.0, 3.0}};
    auto r2 = findBestAndWorst(v2);
    assert(r2.best_index == 0);
    assert(r2.worst_index == 0);

    // Negative scores
    std::vector<Candidate> v3 = {{-5.0, -2.0}, {-1.0, 1.0}, {-3.0, -3.0}};
    auto r3 = findBestAndWorst(v3);
    assert(r3.best_index == 1); // sum 0.0
    assert(r3.worst_index == 0); // sum -7.0

    // Single element
    std::vector<Candidate> v4 = {{3.0, 4.0}};
    auto r4 = findBestAndWorst(v4);
    assert(r4.best_index == 0);
    assert(r4.worst_index == 0);

    // Ties in best with distinct sums
    std::vector<Candidate> v5 = {{2.0, 2.0}, {1.0, 3.0}, {4.0, 0.0}}; // sums: 4,4,4
    auto r5 = findBestAndWorst(v5);
    assert(r5.best_index == 0);
    assert(r5.worst_index == 0);

    // Ties in worst with distinct sums
    std::vector<Candidate> v6 = {{0.0, 0.0}, {1.0, -1.0}, {-2.0, 2.0}}; // sums: 0,0,0
    auto r6 = findBestAndWorst(v6);
    assert(r6.worst_index == 0);
    assert(r6.best_index == 0);

    // All negative with different sums
    std::vector<Candidate> v7 = {{-10.0, -1.0}, {-2.0, -3.0}, {-5.0, -5.0}};
    auto r7 = findBestAndWorst(v7);
    assert(r7.best_index == 1); // sum -5
    assert(r7.worst_index == 0); // sum -11

    return 0;
}
