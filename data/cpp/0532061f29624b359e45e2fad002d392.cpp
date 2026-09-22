Write a C++ function `double aggregateScores(const std::vector<EvaluatorPtr>& evaluators, const std::vector<double>& weights, const std::vector<bool>& invertFlags, double inputScore)` that computes a weighted aggregation of scores, but instead of calling external evaluators, each "evaluator" is simulated by a simple scaling/reversal operation on the provided `inputScore`. The function must: (1) reject any call where the number of evaluators, weights, and invert flags are not all equal in size; (2) reject any call where an evaluator pointer is null or a weight is negative; (3) reject calls with an empty evaluator list; (4) compute the total score as the weighted average of each evaluator’s score, where each evaluator’s score is `inputScore` multiplied by a factor derived from its index (e.g., evaluator i produces `inputScore * (i+1)`), and if `invertFlags[i]` is true, the score is `1.0 - thatProduct`; (5) normalize each weight by the total weight before scaling; (6) return the final aggregated score. The evaluator is a simple struct holding a dummy pointer or a unique ID, but you may model it as a `std::shared_ptr<int>` for simplicity. The free function must be standalone, use `const` correctness, and include necessary headers.

#include <cassert>
#include <memory>
#include <vector>

// Function under test is declared above (or included from header).

int main() {
    using Eval = std::shared_ptr<int>;
    std::vector<Eval> evals;
    std::vector<double> w;
    std::vector<bool> inv;

    // Single evaluator, weight 1, no inversion, inputScore 0.5 -> score = 0.5*1 = 0.5
    evals = {std::make_shared<int>(1)};
    w = {1.0};
    inv = {false};
    assert(aggregateScores(evals, w, inv, 0.5) == 0.5);

    // Two evaluators: scores = 0.25*1=0.25 (weight 1), 0.25*2=0.5 (weight 1)
    // total_weight=2, normalized: 0.5*0.25 + 0.5*0.5 = 0.125+0.25=0.375
    evals = {std::make_shared<int>(1), std::make_shared<int>(2)};
    w = {1.0, 1.0};
    inv = {false, false};
    double result = aggregateScores(evals, w, inv, 0.25);
    assert(result > 0.374999 && result < 0.375001);

    // Test inversion: evaluator0: inputScore=0.8 -> 0.8, invert -> 0.2 (weight 2)
    // evaluator1: inputScore=0.8*2=1.6, no invert (weight 1) -> total_weight=3
    // normalized: (2/3)*0.2 + (1/3)*1.6 = 0.13333 + 0.53333 = 0.66666
    evals = {std::make_shared<int>(1), std::make_shared<int>(2)};
    w = {2.0, 1.0};
    inv = {true, false};
    result = aggregateScores(evals, w, inv, 0.8);
    assert(result > 0.66665 && result < 0.66667);

    // Empty evaluators should throw
    evals = {};
    w = {};
    inv = {};
    bool threw = false;
    try {
        aggregateScores(evals, w, inv, 1.0);
    } catch (const std::invalid_argument&) { threw = true; }
    assert(threw);

    // Null evaluator should throw
    evals = {nullptr};
    w = {1.0};
    inv = {false};
    threw = false;
    try {
        aggregateScores(evals, w, inv, 1.0);
    } catch (const std::invalid_argument&) { threw = true; }
    assert(threw);

    // Negative weight should throw
    evals = {std::make_shared<int>(1)};
    w = {-1.0};
    inv = {false};
    threw = false;
    try {
        aggregateScores(evals, w, inv, 1.0);
    } catch (const std::invalid_argument&) { threw = true; }
    assert(threw);

    // Mismatched sizes should throw
    evals = {std::make_shared<int>(1), std::make_shared<int>(2)};
    w = {1.0};
    inv = {false, false};
    threw = false;
    try {
        aggregateScores(evals, w, inv, 1.0);
    } catch (const std::invalid_argument&) { threw = true; }
    assert(threw);

    // All weights zero should throw
    evals = {std::make_shared<int>(1)};
    w = {0.0};
    inv = {false};
    threw = false;
    try {
        aggregateScores(evals, w, inv, 1.0);
    } catch (const std::invalid_argument&) { threw = true; }
    assert(threw);

    // Exact equality with simple numbers: one evaluator, weight 2, invert false
    evals = {std::make_shared<int>(1)};
    w = {2.0};
    inv = {false};
    result = aggregateScores(evals, w, inv, 0.3);
    assert(result == 0.3); // normalized weight = 2/2 = 1, score = 0.3

    // Inversion with single evaluator: inputScore=0.7 -> 1-0.7=0.3
    evals = {std::make_shared<int>(1)};
    w = {2.0};
    inv = {true};
    result = aggregateScores(evals, w, inv, 0.7);
    assert(result > 0.299999 && result < 0.300001);

    return 0;
}

#include <vector>
#include <memory>
#include <stdexcept>

// Aggregates scores from simulated evaluators. Each evaluator is represented by
// a shared_ptr<int> (non-null required). Weights must be non-negative.
// invertFlags[i] determines if evaluator i's score is inverted (1 - score).
double aggregateScores(
    const std::vector<std::shared_ptr<int>>& evaluators,
    const std::vector<double>& weights,
    const std::vector<bool>& invertFlags,
    double inputScore)
{
    const auto n = evaluators.size();
    if (weights.size() != n || invertFlags.size() != n) {
        throw std::invalid_argument("All input vectors must have equal size");
    }
    if (n == 0) {
        throw std::invalid_argument("No evaluators provided");
    }

    double total_weight = 0.0;
    for (size_t i = 0; i < n; ++i) {
        if (evaluators[i] == nullptr) {
            throw std::invalid_argument("Null evaluator not allowed");
        }
        if (weights[i] < 0.0) {
            throw std::invalid_argument("Weight must be >= 0.0");
        }
        total_weight += weights[i];
    }

    if (total_weight == 0.0) {
        throw std::invalid_argument("Total weight must be > 0.0");
    }

    double total_score = 0.0;
    for (size_t i = 0; i < n; ++i) {
        double score = inputScore * static_cast<double>(i + 1);
        if (invertFlags[i]) {
            score = 1.0 - score;
        }
        const double normalized_weight = weights[i] / total_weight;
        total_score += normalized_weight * score;
    }

    return total_score;
}

// The solution mirrors the given code snippet’s logic but abstracts away the evaluator calls. First, validate that the three input vectors have identical sizes and are non-empty; otherwise, throw `std::invalid_argument`. Then validate that every evaluator pointer is non-null and every weight is non-negative, throwing on violation. Compute `total_weight` by summing all weights; if total weight is zero, either handle by throwing or by treating all weights equally (but the problem statement does not specify; safest is to throw `std::invalid_argument` because division by zero occurs). For each index, compute `score = inputScore * (i+1)`; if invert is true, replace with `1.0 - score`. Multiply by normalized weight (`weight / total_weight`) and accumulate. Edge cases: negative inputScore is allowed, but scores may exceed 1.0 after multiplication, which is acceptable as no clamping is specified; however, inverted scores could become negative, which the algorithm handles naturally. Time complexity is O(n) for a single pass after validation, with O(n) auxiliary space for the vectors themselves but O(1) extra for computation. The function must be `const` in the sense that it does not modify the input vectors (passed by const reference).
