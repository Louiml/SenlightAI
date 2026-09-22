Write a C++ function `double getSelectionScore(int nTried, int nNew, int nUnkBias)` that computes a combined selection score used to decide whether to prefer trying a node from the "tried" table or the "new" table. The function should accept the number of tried entries `nTried`, the number of new entries `nNew`, and an integer bias `nUnkBias` (assumed to be between 0 and 100 inclusive). It should return the score for the tried table, which is defined as `sqrt(nTried) * (100.0 - nUnkBias)`, and also provide a way to retrieve the corresponding new-table score via an output parameter `double* newScore` (non-null). Both scores must be non-negative. Handle edge cases where either count is zero, where the bias is 0 or 100, and where counts are large. Use the standard library's `std::sqrt` for the square root and ensure the function is `const`-correct (though it has no state) and does not modify its inputs. The function should be self-contained with no external dependencies beyond `<cmath>`.
#include <cassert>
#include <cmath>

int main() {
    double newScore = -1.0;
    
    // Basic case with equal counts and 50 bias.
    double tried = getSelectionScore(100, 100, 50, &newScore);
    assert(std::abs(tried - 500.0) < 1e-9);
    assert(std::abs(newScore - 500.0) < 1e-9);
    
    // Bias 0: tried score dominates, new score zero.
    newScore = -1.0;
    tried = getSelectionScore(16, 9, 0, &newScore);
    assert(std::abs(tried - 400.0) < 1e-9);
    assert(std::abs(newScore) < 1e-9);
    
    // Bias 100: new score dominates, tried score zero.
    newScore = -1.0;
    tried = getSelectionScore(16, 9, 100, &newScore);
    assert(std::abs(tried) < 1e-9);
    assert(std::abs(newScore - 900.0) < 1e-9);
    
    // Edge case: both counts zero.
    newScore = -1.0;
    tried = getSelectionScore(0, 0, 50, &newScore);
    assert(std::abs(tried) < 1e-9);
    assert(std::abs(newScore) < 1e-9);
    
    // Edge case: one count zero, bias 25.
    newScore = -1.0;
    tried = getSelectionScore(0, 4, 25, &newScore);
    assert(std::abs(tried) < 1e-9);
    assert(std::abs(newScore - 100.0) < 1e-9);
    
    // Edge case: negative inputs are clamped.
    newScore = -1.0;
    tried = getSelectionScore(-5, 4, 25, &newScore);
    assert(std::abs(tried) < 1e-9);
    assert(std::abs(newScore - 100.0) < 1e-9);
    
    // Edge case: bias out of range clamps.
    newScore = -1.0;
    tried = getSelectionScore(9, 16, 150, &newScore);
    assert(std::abs(tried) < 1e-9);
    assert(std::abs(newScore - 1600.0) < 1e-9);
    
    return 0;
}
#include <cmath>
#include <cassert>

/**
 * Compute the selection scores for tried and new node tables.
 * 
 * @param nTried    Number of entries in the tried table (non-negative).
 * @param nNew      Number of entries in the new table (non-negative).
 * @param nUnkBias  Bias between 0 and 100 (inclusive). 
 * @param newScore  Output parameter that receives the new-table score.
 * @return The tried-table score.
 */
double getSelectionScore(int nTried, int nNew, int nUnkBias, double* newScore) {
    // Clamp bias to valid range to avoid negative scores.
    if (nUnkBias < 0) nUnkBias = 0;
    if (nUnkBias > 100) nUnkBias = 100;
    
    // Clamp counts to non-negative.
    if (nTried < 0) nTried = 0;
    if (nNew < 0) nNew = 0;
    
    double triedScore = std::sqrt(static_cast<double>(nTried)) * (100.0 - nUnkBias);
    double newScoreValue = std::sqrt(static_cast<double>(nNew)) * nUnkBias;
    
    assert(newScore != nullptr);
    *newScore = newScoreValue;
    
    return triedScore;
}
// The task is straightforward: we need to compute two values derived from the given parameters. The main algorithm involves applying the formula for the tried score and the new score exactly as specified. We must be careful to handle the edge case of negative counts (which we can clamp to zero to avoid `sqrt` of a negative number, though the problem implies valid non-negative inputs). The bias `nUnkBias` is used directly in both formulas; when it is 0, the tried score becomes `sqrt(nTried) * 100` and the new score becomes 0; when it is 100, the tried score becomes 0 and the new score becomes `sqrt(nNew) * 100`. For zero counts, `sqrt(0)` is 0, which is fine. Time complexity is O(1) and space complexity is O(1). Edge cases: if `nTried` or `nNew` are negative, we treat them as zero; if `nUnkBias` is outside [0,100], we clamp it to that range to avoid negative scores or undefined behavior. The function writes the new score to the provided output pointer; we must check for null pointer and handle gracefully by doing nothing or asserting non-null (the task specifies it is non-null, so we can assert). The solution uses `std::sqrt` from `<cmath>`. The function is a free function, not a class method, so no `const` qualifier is needed on the function itself, but the parameters are passed by value to avoid modification.
