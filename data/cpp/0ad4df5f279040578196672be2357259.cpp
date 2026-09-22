/*
Write a standalone C++ function `solveTriDiagonalSystems(const std::vector<float>& lower, const std::vector<float>& diagonal, const std::vector<float>& upper, const std::vector<float>& rhs, int batchCount, int systemSize)` that solves `batchCount` independent tridiagonal linear systems, where each system has `systemSize` unknowns. The input vectors are interleaved: for each system `b` (0 to batchCount-1) and each row `i` (0 to systemSize-1), the lower diagonal value is `lower[b * systemSize + i]`, the diagonal is `diagonal[b * systemSize + i]`, the upper is `upper[b * systemSize + i]`, and the right-hand side is `rhs[b * systemSize + i]` (with `lower[0]` and `upper[systemSize-1]` unused). The function must return a `std::vector<float>` of size `batchCount * systemSize` containing the solution values interleaved in the same layout (solution for system `b`, unknown `i` at index `b * systemSize + i`). Use Gaussian elimination without pivoting (Thomas algorithm) applied independently to each system. The function must handle `systemSize >= 1`, `batchCount >= 1`, and use `const` references for inputs. You may assume the systems are nonsingular and no division by zero occurs. Do not use any external library; only standard C++.
*/
#include <vector>
#include <cassert>

// Solve batchCount independent tridiagonal systems of size systemSize.
// Input arrays are interleaved: index = batch * systemSize + row.
// lower[0] and upper[systemSize-1] are ignored.
// Returns solution interleaved similarly.
std::vector<float> solveTriDiagonalSystems(
    const std::vector<float>& lower,
    const std::vector<float>& diagonal,
    const std::vector<float>& upper,
    const std::vector<float>& rhs,
    int batchCount,
    int systemSize) {

    assert(batchCount >= 1);
    assert(systemSize >= 1);
    assert(static_cast<int>(lower.size()) == batchCount * systemSize);
    assert(static_cast<int>(diagonal.size()) == batchCount * systemSize);
    assert(static_cast<int>(upper.size()) == batchCount * systemSize);
    assert(static_cast<int>(rhs.size()) == batchCount * systemSize);

    std::vector<float> solution(batchCount * systemSize);

    for (int b = 0; b < batchCount; ++b) {
        const int offset = b * systemSize;

        // Local copies for modification during elimination
        std::vector<float> diag(diagonal.begin() + offset, diagonal.begin() + offset + systemSize);
        std::vector<float> up(upper.begin() + offset, upper.begin() + offset + systemSize);
        std::vector<float> r(rhs.begin() + offset, rhs.begin() + offset + systemSize);

        // Forward elimination (Thomas algorithm)
        for (int i = 1; i < systemSize; ++i) {
            const float w = lower[offset + i] / diag[i - 1];
            diag[i] -= w * up[i - 1];
            r[i] -= w * r[i - 1];
        }

        // Back substitution
        solution[offset + systemSize - 1] = r[systemSize - 1] / diag[systemSize - 1];
        for (int i = systemSize - 2; i >= 0; --i) {
            solution[offset + i] = (r[i] - up[i] * solution[offset + i + 1]) / diag[i];
        }
    }

    return solution;
}
#include <cassert>
#include <vector>
#include <cmath>

int main() {
    // System 1: [2 1 0; 1 2 1; 0 1 2] * x = [4; 6; 4] -> x = [1; 2; 1]
    // System 2: [4 1 0; 1 4 1; 0 1 4] * x = [5; 6; 5] -> x = [1; 1; 1]
    std::vector<float> lower = {0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f};
    std::vector<float> diag  = {2.0f, 2.0f, 2.0f, 4.0f, 4.0f, 4.0f};
    std::vector<float> upper = {1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f};
    std::vector<float> rhs   = {4.0f, 6.0f, 4.0f, 5.0f, 6.0f, 5.0f};

    auto sol = solveTriDiagonalSystems(lower, diag, upper, rhs, 2, 3);
    assert(sol.size() == 6);
    assert(fabs(sol[0] - 1.0f) < 1e-5f);
    assert(fabs(sol[1] - 2.0f) < 1e-5f);
    assert(fabs(sol[2] - 1.0f) < 1e-5f);
    assert(fabs(sol[3] - 1.0f) < 1e-5f);
    assert(fabs(sol[4] - 1.0f) < 1e-5f);
    assert(fabs(sol[5] - 1.0f) < 1e-5f);

    // Single system of size 1: [5] * x = [10] -> x = [2]
    std::vector<float> l1 = {0.0f};
    std::vector<float> d1 = {5.0f};
    std::vector<float> u1 = {0.0f};
    std::vector<float> r1 = {10.0f};
    auto sol1 = solveTriDiagonalSystems(l1, d1, u1, r1, 1, 1);
    assert(sol1.size() == 1);
    assert(fabs(sol1[0] - 2.0f) < 1e-6f);

    // One system of size 4 with known solution [1, 2, 3, 4]
    // Matrix: [2 1 0 0; 1 2 1 0; 0 1 2 1; 0 0 1 2]
    // RHS computed as A * x = [4; 8; 12; 11]
    std::vector<float> l2 = {0.0f, 1.0f, 1.0f, 1.0f};
    std::vector<float> d2 = {2.0f, 2.0f, 2.0f, 2.0f};
    std::vector<float> u2 = {1.0f, 1.0f, 1.0f, 0.0f};
    std::vector<float> r2 = {4.0f, 8.0f, 12.0f, 11.0f};
    auto sol2 = solveTriDiagonalSystems(l2, d2, u2, r2, 1, 4);
    for (int i = 0; i < 4; ++i) {
        assert(fabs(sol2[i] - (i + 1)) < 1e-5f);
    }

    // Test with negative numbers and slightly larger batch
    // System 1: [3 -1 0; -1 3 -1; 0 -1 3] * x = [1; 2; 3] -> by inspection
    // Let's just test determinism with 3 batches of 2 systems
    std::vector<float> l3 = {0.0f, 0.5f, 0.0f, 0.5f, 0.0f, 0.5f};
    std::vector<float> d3 = {2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f};
    std::vector<float> u3 = {0.5f, 0.0f, 0.5f, 0.0f, 0.5f, 0.0f};
    std::vector<float> r3 = {3.0f, 2.0f, 3.0f, 2.0f, 3.0f, 2.0f};
    auto sol3 = solveTriDiagonalSystems(l3, d3, u3, r3, 3, 2);
    // For each system: 2*x1 + 0.5*x2 = 3, 0.5*x1 + 2*x2 = 2 => solve manually
    // x1 = (3*2 - 0.5*2) / (4 - 0.25) = (6 - 1)/3.75 = 5/3.75 ≈ 1.3333
    // x2 = (3 - 2*x1)/0.5? Actually better: solve linear system
    // Check solution approximates 1.3333 and 0.6667
    assert(fabs(sol3[0] - 1.3333f) < 1e-3f);
    assert(fabs(sol3[1] - 0.6667f) < 1e-3f);
    assert(fabs(sol3[2] - sol3[0]) < 1e-6f);
    assert(fabs(sol3[3] - sol3[1]) < 1e-6f);
    assert(fabs(sol3[4] - sol3[0]) < 1e-6f);
    assert(fabs(sol3[5] - sol3[1]) < 1e-6f);
}
// The problem requires solving multiple independent tridiagonal systems. The natural algorithm is the Thomas algorithm, which is a specialized form of Gaussian elimination for tridiagonal matrices, running in O(systemSize) time per system. For each system `b`, we copy the diagonal, upper, and right-hand side arrays into local mutable vectors (since the algorithm modifies them), while the lower diagonal can be used directly. Then we perform forward elimination: for `i` from 1 to systemSize-1, compute `w = lower[i] / diag[i-1]`, update `diag[i] -= w * upper[i-1]`, update `rhs[i] -= w * rhs[i-1]`. Back substitution: set `x[systemSize-1] = rhs[systemSize-1] / diag[systemSize-1]`, then for `i` from systemSize-2 down to 0, `x[i] = (rhs[i] - upper[i] * x[i+1]) / diag[i]`. Edge case: when `systemSize == 1`, simply compute `rhs[0] / diag[0]`. Time complexity is O(batchCount * systemSize) and auxiliary space is O(systemSize) per system if we allocate temporary arrays, or O(batchCount * systemSize) if we copy all at once. To keep the solution simple and safe, we allocate temporary vectors of size `systemSize` inside the loop for each system.
