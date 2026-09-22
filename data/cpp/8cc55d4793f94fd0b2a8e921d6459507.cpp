Write a standalone C++ function named `computeImpulseFromRows` that simulates the core of a single Gauss-Seidel constraint-solving iteration for a set of one-dimensional impulse constraints. The function takes: (1) a `std::vector<double>` of current applied impulses `appliedImpulses` (size = number of rows), (2) a `std::vector<double>` of right-hand-side target values `rhs` (same size), (3) a `std::vector<double>` of inverse diagonal terms `jacDiagABInv` (same size), (4) a `std::vector<double>` of lower limits `lowerLimits` (same size), (5) a `std::vector<double>` of upper limits `upperLimits` (same size), (6) a `std::vector<double>` of constraint force mixing (CFM) coefficients `cfm` (same size), and (7) a 2D coupling matrix `coupling` of size `rows x rows` where `coupling[i][j]` represents the influence of the delta impulse at row `j` on row `i` (the diagonal is assumed to be 1.0, but the function must use `jacDiagABInv[i]` to scale the correction). The function performs one full sweep (rows 0 to n-1) of sequential updates: for each row `i`, compute the residual as `rhs[i] - cfm[i] * appliedImpulses[i]` minus the sum over all `j` of `coupling[i][j] * appliedImpulses[j]`, then compute a raw delta impulse as `residual * jacDiagABInv[i]`, add this to the current `appliedImpulses[i]`, and clamp the new value to the interval `[lowerLimits[i], upperLimits[i]]`. The coupling matrix must be passed as `std::vector<std::vector<double>>` and is assumed square with size equal to the number of rows. The function must not modify the input vectors (except to compute and return the new impulses as a fresh `std::vector<double>`). It must not assume any special structure like symmetry. Ensure the function is robust for empty input (return an empty vector).
#include <cassert>
#include <vector>

// The solution function is declared here.
std::vector<double> computeImpulseFromRows(
    const std::vector<double>& appliedImpulses,
    const std::vector<double>& rhs,
    const std::vector<double>& jacDiagABInv,
    const std::vector<double>& lowerLimits,
    const std::vector<double>& upperLimits,
    const std::vector<double>& cfm,
    const std::vector<std::vector<double>>& coupling);

int main() {
    // Test 1: Single row, no coupling, no CFM, free limits.
    {
        std::vector<double> impulses = {0.0};
        std::vector<double> rhs = {10.0};
        std::vector<double> invDiag = {2.0};
        std::vector<double> lower = {-100.0};
        std::vector<double> upper = {100.0};
        std::vector<double> cfm = {0.0};
        std::vector<std::vector<double>> coupling = {{1.0}};
        std::vector<double> result = computeImpulseFromRows(impulses, rhs, invDiag, lower, upper, cfm, coupling);
        // residual = 10 - 0 - (1*0) = 10, delta = 10*2 = 20, new = 20
        assert(result.size() == 1);
        assert(result[0] == 20.0);
    }

    // Test 2: Two rows, sequential update influence.
    {
        std::vector<double> impulses = {0.0, 0.0};
        std::vector<double> rhs = {10.0, 5.0};
        std::vector<double> invDiag = {1.0, 1.0};
        std::vector<double> lower = {-1000.0, -1000.0};
        std::vector<double> upper = {1000.0, 1000.0};
        std::vector<double> cfm = {0.0, 0.0};
        // Row0 depends on itself and row1 with weight 0.5, row1 depends on itself and row0 with weight 0.2.
        std::vector<std::vector<double>> coupling = {{1.0, 0.5}, {0.2, 1.0}};
        std::vector<double> result = computeImpulseFromRows(impulses, rhs, invDiag, lower, upper, cfm, coupling);
        // Row0: residual = 10 - 0 - (1*0 + 0.5*0) = 10, delta = 10, new0 = 10
        // Row1: residual = 5 - 0 - (0.2*10 + 1*0) = 3, delta = 3, new1 = 3
        assert(result.size() == 2);
        assert(result[0] == 10.0);
        assert(result[1] == 3.0);
    }

    // Test 3: Clamping to upper limit.
    {
        std::vector<double> impulses = {0.0};
        std::vector<double> rhs = {100.0};
        std::vector<double> invDiag = {1.0};
        std::vector<double> lower = {-50.0};
        std::vector<double> upper = {30.0};
        std::vector<double> cfm = {0.0};
        std::vector<std::vector<double>> coupling = {{1.0}};
        std::vector<double> result = computeImpulseFromRows(impulses, rhs, invDiag, lower, upper, cfm, coupling);
        assert(result[0] == 30.0);
    }

    // Test 4: CFM effect reduces the effective correction.
    {
        std::vector<double> impulses = {0.0};
        std::vector<double> rhs = {10.0};
        std::vector<double> invDiag = {1.0};
        std::vector<double> lower = {-100.0};
        std::vector<double> upper = {100.0};
        std::vector<double> cfm = {0.5};
        std::vector<std::vector<double>> coupling = {{1.0}};
        std::vector<double> result = computeImpulseFromRows(impulses, rhs, invDiag, lower, upper, cfm, coupling);
        // residual = 10 - 0.5*0 = 10, delta=10, new=10. (No change because cfm term is zero for zero impulse)
        // To see CFM effect, use a non-zero initial impulse.
        impulses[0] = 2.0;
        result = computeImpulseFromRows(impulses, rhs, invDiag, lower, upper, cfm, coupling);
        // residual = 10 - 0.5*2 - 1*2 = 10 -1 -2 = 7, delta=7, new=9
        assert(result[0] == 9.0);
    }

    // Test 5: Zero inverse diagonal leaves impulse unchanged.
    {
        std::vector<double> impulses = {7.0};
        std::vector<double> rhs = {100.0};
        std::vector<double> invDiag = {0.0};
        std::vector<double> lower = {-100.0};
        std::vector<double> upper = {100.0};
        std::vector<double> cfm = {0.0};
        std::vector<std::vector<double>> coupling = {{1.0}};
        std::vector<double> result = computeImpulseFromRows(impulses, rhs, invDiag, lower, upper, cfm, coupling);
        assert(result[0] == 7.0);
    }

    // Test 6: Empty input returns empty vector.
    {
        std::vector<double> result = computeImpulseFromRows({}, {}, {}, {}, {}, {}, {});
        assert(result.empty());
    }

    // Test 7: Larger coupling matrix with three rows, verify sequential effect.
    {
        std::vector<double> impulses = {0.0, 0.0, 0.0};
        std::vector<double> rhs = {1.0, 2.0, 3.0};
        std::vector<double> invDiag = {1.0, 1.0, 1.0};
        std::vector<double> lower = {-10.0, -10.0, -10.0};
        std::vector<double> upper = {10.0, 10.0, 10.0};
        std::vector<double> cfm = {0.0, 0.0, 0.0};
        std::vector<std::vector<double>> coupling = {
            {1.0, 0.0, 0.0},
            {0.0, 1.0, 0.0},
            {0.0, 0.0, 1.0}
        };
        std::vector<double> result = computeImpulseFromRows(impulses, rhs, invDiag, lower, upper, cfm, coupling);
        // Each row independent, so result equals rhs.
        assert(result[0] == 1.0);
        assert(result[1] == 2.0);
        assert(result[2] == 3.0);
    }

    // Test 8: Coupling where row0 influences row1.
    {
        std::vector<double> impulses = {0.0, 0.0};
        std::vector<double> rhs = {2.0, 0.0};
        std::vector<double> invDiag = {1.0, 1.0};
        std::vector<double> lower = {-100.0, -100.0};
        std::vector<double> upper = {100.0, 100.0};
        std::vector<double> cfm = {0.0, 0.0};
        // Row1 depends on row0 with weight 1.0, row0 independent.
        std::vector<std::vector<double>> coupling = {{1.0, 0.0}, {1.0, 1.0}};
        std::vector<double> result = computeImpulseFromRows(impulses, rhs, invDiag, lower, upper, cfm, coupling);
        // Row0: residual=2, delta=2, new0=2
        // Row1: residual = 0 - (1*2 + 1*0) = -2, delta=-2, new1=-2
        assert(result[0] == 2.0);
        assert(result[1] == -2.0);
    }

    // Test 9: Lower bound clamping.
    {
        std::vector<double> impulses = {0.0};
        std::vector<double> rhs = {-100.0};
        std::vector<double> invDiag = {1.0};
        std::vector<double> lower = {-10.0};
        std::vector<double> upper = {100.0};
        std::vector<double> cfm = {0.0};
        std::vector<std::vector<double>> coupling = {{1.0}};
        std::vector<double> result = computeImpulseFromRows(impulses, rhs, invDiag, lower, upper, cfm, coupling);
        assert(result[0] == -10.0);
    }

    // Test 10: Verify that the function does not modify the input vector.
    {
        std::vector<double> impulses = {1.0, 2.0};
        std::vector<double> original = impulses;
        std::vector<double> rhs = {5.0, 5.0};
        std::vector<double> invDiag = {1.0, 1.0};
        std::vector<double> lower = {-100.0, -100.0};
        std::vector<double> upper = {100.0, 100.0};
        std::vector<double> cfm = {0.0, 0.0};
        std::vector<std::vector<double>> coupling = {{1.0, 0.0}, {0.0, 1.0}};
        computeImpulseFromRows(impulses, rhs, invDiag, lower, upper, cfm, coupling);
        assert(impulses == original);
    }

    return 0;
}
#include <vector>
#include <algorithm>
#include <cstddef>

// Performs one Gauss-Seidel sweep over impulse constraints.
// Returns a new vector of impulses after applying the sequential updates.
std::vector<double> computeImpulseFromRows(
    const std::vector<double>& appliedImpulses,
    const std::vector<double>& rhs,
    const std::vector<double>& jacDiagABInv,
    const std::vector<double>& lowerLimits,
    const std::vector<double>& upperLimits,
    const std::vector<double>& cfm,
    const std::vector<std::vector<double>>& coupling)
{
    const std::size_t n = appliedImpulses.size();
    // Assume all input vectors are the same size as n.
    std::vector<double> newImpulses = appliedImpulses;

    for (std::size_t i = 0; i < n; ++i) {
        // Compute residual for row i using the latest impulses in newImpulses
        // (for j < i they are updated; for j > i they are still from previous iteration).
        double residual = rhs[i] - cfm[i] * newImpulses[i];
        for (std::size_t j = 0; j < n; ++j) {
            residual -= coupling[i][j] * newImpulses[j];
        }
        // The diagonal of coupling is supposed to be 1.0, so the raw correction is residual * invDiag.
        double deltaImpulse = residual * jacDiagABInv[i];
        double newValue = newImpulses[i] + deltaImpulse;
        // Clamp to limits.
        newValue = std::max(newValue, lowerLimits[i]);
        newValue = std::min(newValue, upperLimits[i]);
        newImpulses[i] = newValue;
    }

    return newImpulses;
}
// The algorithm is a direct serial Gauss-Seidel update, which is the core of the PGS (Projected Gauss-Seidel) solver used in constraint-based physics engines. For each constraint row `i` in order, we compute the current violation (residual) considering all impulses (including the already updated ones for indices `j < i`, and the old ones for `j > i`). The residual is the target `rhs[i]` minus the CFM damping term and minus the coupling-weighted sum of all impulses. Because the diagonal of the coupling matrix is assumed to be 1.0, the raw correction to the impulse is simply the residual multiplied by `jacDiagABInv[i]`. After adding this correction, we clamp to the lower/upper limits, which enforces inequality constraints (e.g., friction limits or joint limits). One important edge case: if `jacDiagABInv[i]` is zero (which could happen for a degenerate constraint), the update would keep the impulse unchanged; the code naturally handles this. Another edge case: the coupling matrix may contain non-zero off-diagonals even for rows that are not directly connected, but the algorithm still works because it processes all rows sequentially. The complexity is O(n^2) for the inner loop over all columns for each row, so total O(n^2) time and O(n) extra space (for the result vector) besides the input storage. The function is `const`-correct in the sense that it never modifies its inputs and returns a new vector.
