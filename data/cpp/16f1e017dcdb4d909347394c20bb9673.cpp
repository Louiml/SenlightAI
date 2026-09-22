// Implement a C++ function that simulates the core of a nonlinear conjugate gradient (NLCG) iterative solver for a system of scalar constraint residuals. Given vectors of residual changes `delta` (from the current iteration) and previous search directions `p`, a previous squared residual norm `prevDeltaSqr`, and a scalar threshold, the function must compute the Polak–Ribière–Polyak (PRP) style `beta` coefficient (with restart: if `beta > 1`, reset all search directions to zero) and return the updated search direction vector `p_new[i] = beta * p[i] + delta[i]` for each element. The function should also return the new squared residual norm (sum of squares of `delta`). Handle the edge case where `prevDeltaSqr == 0` by treating `beta` as 2 (as in the original code), and ensure no out-of-bounds access for empty inputs. The function must be `const`-correct, take vectors by `const&` for read-only inputs, and return the new direction vector by value. Demonstrate the function works for a simple 2-element system, including the restart case and the zero-previous-norm case, using `assert` statements in the test.
// The solution mirrors the NLCG update rule shown in the snippet: after a solver iteration, `delta` contains the residual change (impulse change), and `p` is the previous search direction. The PRP formula computes `beta = (newSqr / prevSqr)` when `prevSqr > 0`; otherwise, the code uses `beta = 2` (a fallback that typically forces a restart). If `beta > 1`, the direction is reset to zero (i.e., the next search direction equals `delta` only). Otherwise, the new direction is `beta * p + delta`. The function must also return the new squared norm so the caller can store it for the next iteration. Edge cases: empty vectors return empty result with `newSqr = 0`; a zero `prevSqr` yields `beta = 2` (which triggers a restart, so `p_new = delta`); if `delta` is non-zero but `prevSqr > 0` and `beta <= 1`, the linear combination applies. Complexity: O(n) time and O(1) auxiliary space besides the returned vector.
#include <vector>
#include <cmath>

/**
 * @brief Compute PRP-style NLCG search direction update.
 * 
 * Given the residual change vector `delta` from the current iteration,
 * the previous search direction vector `p`, the previous squared residual
 * norm `prevDeltaSqr`, compute the new search direction following the
 * Polak–Ribi`ere–Polyak rule with a restart if beta > 1.
 * 
 * The function returns a pair: {newDirection, newSquaredNorm}.
 * 
 * @param delta Residual changes (const reference, read-only).
 * @param p     Previous search directions (const reference, read-only).
 * @param prevDeltaSqr Squared norm of the previous residual delta.
 * @return std::pair<std::vector<double>, double> containing the updated
 *         direction and the current squared norm (sum of squares of delta).
 */
std::pair<std::vector<double>, double> nncgUpdate(
    const std::vector<double>& delta,
    const std::vector<double>& p,
    double prevDeltaSqr
) {
    // Compute current squared norm of delta
    double deltaSqr = 0.0;
    for (double d : delta) {
        deltaSqr += d * d;
    }

    // Determine beta using PRP formula with fallback to 2 when prevDeltaSqr == 0
    double beta = (prevDeltaSqr > 0.0) ? (deltaSqr / prevDeltaSqr) : 2.0;

    std::vector<double> pNew;
    if (beta > 1.0) {
        // Restart: set all directions to zero, so pNew[i] = delta[i]
        pNew = delta; // copy
    } else {
        pNew.resize(delta.size());
        for (size_t i = 0; i < delta.size(); ++i) {
            pNew[i] = beta * p[i] + delta[i];
        }
    }

    return {pNew, deltaSqr};
}
#include <cassert>
#include <cmath>
#include <vector>

// The function is assumed to be included from the solution above.

int main() {
    // Test 1: Basic linear combination with beta=0.5
    {
        std::vector<double> delta = {1.0, 2.0};
        std::vector<double> p = {0.5, -0.5};
        double prevSqr = 4.0; // deltaSqr = 1+4 = 5, beta = 5/4 = 1.25 > 1 -> restart
        auto [pNew, newSqr] = nncgUpdate(delta, p, prevSqr);
        assert(std::fabs(newSqr - 5.0) < 1e-12);
        assert(pNew.size() == 2);
        assert(std::fabs(pNew[0] - 1.0) < 1e-12);
        assert(std::fabs(pNew[1] - 2.0) < 1e-12);
    }

    // Test 2: beta <= 1 applies linear combination
    {
        std::vector<double> delta = {1.0, 0.0};
        std::vector<double> p = {1.0, 0.0};
        double prevSqr = 2.0; // deltaSqr = 1, beta = 0.5
        auto [pNew, newSqr] = nncgUpdate(delta, p, prevSqr);
        assert(std::fabs(newSqr - 1.0) < 1e-12);
        assert(std::fabs(pNew[0] - (0.5*1.0 + 1.0)) < 1e-12);
        assert(std::fabs(pNew[1] - (0.5*0.0 + 0.0)) < 1e-12);
    }

    // Test 3: zero previous norm -> beta = 2 -> restart (pNew = delta)
    {
        std::vector<double> delta = {3.0, -1.0};
        std::vector<double> p = {100.0, 200.0};
        auto [pNew, newSqr] = nncgUpdate(delta, p, 0.0);
        assert(std::fabs(newSqr - 10.0) < 1e-12);
        assert(std::fabs(pNew[0] - 3.0) < 1e-12);
        assert(std::fabs(pNew[1] - (-1.0)) < 1e-12);
    }

    // Test 4: empty vectors
    {
        std::vector<double> delta;
        std::vector<double> p;
        auto [pNew, newSqr] = nncgUpdate(delta, p, 1.0);
        assert(pNew.empty());
        assert(newSqr == 0.0);
    }

    // Test 5: beta exactly 1 is still a valid update
    {
        std::vector<double> delta = {2.0, 0.0};
        std::vector<double> p = {1.0, 0.0};
        double prevSqr = 4.0; // deltaSqr = 4, beta = 1
        auto [pNew, newSqr] = nncgUpdate(delta, p, prevSqr);
        assert(std::fabs(newSqr - 4.0) < 1e-12);
        assert(std::fabs(pNew[0] - (1.0*1.0 + 2.0)) < 1e-12);
        assert(std::fabs(pNew[1] - 0.0) < 1e-12);
    }

    return 0;
}
