// Write a standalone C++ function named `normDispOrUnbalance` that simulates the core convergence-testing logic from the provided `NormDispOrUnbalance::test()` method, but as a pure function operating on a sequence of solution vectors. The function should take: a vector of `std::vector<double>` representing the displacement increment vectors (`deltaX`) at each iteration, a vector of `std::vector<double>` representing the corresponding residual/unbalance vectors (`deltaR`), a double `tolDisp`, a double `tolUnbalance`, an integer `maxNumIter`, and an integer `maxIncr` (where `maxIncr < 0` means unlimited). Using the p-norm with p=2 (Euclidean norm) for each vector, the function must replicate the convergence test: return the current iteration number (1-indexed) if either norm is <= its tolerance, return `-2` if the iteration count exceeds `maxNumIter` or the number of "increment" violations exceeds `maxIncr` (unless `maxIncr` is negative, in which case only the iteration limit applies), and return `-1` if convergence is not yet achieved but limits are not exceeded. An "increment" violation is recorded when, starting from iteration 2, both the current displacement norm AND the current residual norm are greater than their respective norms from the previous iteration. The function must assume the two input vectors have equal length and that each inner vector is non-empty; iterate up to the minimum of `maxNumIter` and the input size, and treat reaching the end of input without convergence as a failure (return `-2`).

#include <cassert>
#include <vector>

int main() {
    using Vec = std::vector<double>;
    using Mat = std::vector<Vec>;

    // Case 1: Converges on first iteration (displacement norm small).
    {
        Mat dx = {{0.001, 0.0}};
        Mat dr = {{5.0, 5.0}};
        assert(normDispOrUnbalance(dx, dr, 0.01, 0.1, 10, 3) == 1);
    }

    // Case 2: Converges on second iteration (residual norm small after first).
    {
        Mat dx = {{1.0, 1.0}, {0.5, 0.5}};
        Mat dr = {{10.0, 10.0}, {0.05, 0.0}};
        assert(normDispOrUnbalance(dx, dr, 0.01, 0.1, 10, 3) == 2);
    }

    // Case 3: Converges on third iteration after no increment violations.
    {
        Mat dx = {{1.0, 0.0}, {0.5, 0.0}, {0.01, 0.0}};
        Mat dr = {{10.0, 0.0}, {5.0, 0.0}, {0.5, 0.0}};
        assert(normDispOrUnbalance(dx, dr, 0.02, 0.1, 10, 3) == 3);
    }

    // Case 4: Fails due to exceeding maxIncr (increments get larger).
    {
        Mat dx = {{1.0, 0.0}, {2.0, 0.0}, {3.0, 0.0}};
        Mat dr = {{1.0, 0.0}, {2.0, 0.0}, {3.0, 0.0}};
        assert(normDispOrUnbalance(dx, dr, 0.1, 0.1, 10, 1) == -2);
    }

    // Case 5: Fails due to exceeding maxNumIter even though maxIncr is negative (unlimited).
    {
        Mat dx = {{1.0, 0.0}, {2.0, 0.0}, {3.0, 0.0}, {4.0, 0.0}};
        Mat dr = {{1.0, 0.0}, {2.0, 0.0}, {3.0, 0.0}, {4.0, 0.0}};
        assert(normDispOrUnbalance(dx, dr, 0.1, 0.1, 3, -1) == -2);
    }

    // Case 6: Returns -1 (not yet converged but within limits) after two iterations.
    {
        Mat dx = {{1.0, 0.0}, {0.5, 0.0}};
        Mat dr = {{1.0, 0.0}, {0.5, 0.0}};
        assert(normDispOrUnbalance(dx, dr, 0.01, 0.01, 5, 3) == -1);
    }

    // Case 7: Empty input fails.
    {
        Mat dx;
        Mat dr;
        assert(normDispOrUnbalance(dx, dr, 0.1, 0.1, 10, 3) == -2);
    }

    // Case 8: Mismatched sizes fail.
    {
        Mat dx = {{1.0}};
        Mat dr = {{1.0}, {2.0}};
        assert(normDispOrUnbalance(dx, dr, 0.1, 0.1, 10, 3) == -2);
    }

    // Case 9: maxNumIter zero or negative fails immediately.
    {
        Mat dx = {{1.0}};
        Mat dr = {{1.0}};
        assert(normDispOrUnbalance(dx, dr, 0.1, 0.1, 0, 3) == -2);
        assert(normDispOrUnbalance(dx, dr, 0.1, 0.1, -1, 3) == -2);
    }

    // Case 10: One iteration with both norms above tolerance and maxNumIter=1 fails.
    {
        Mat dx = {{1.0, 1.0}};
        Mat dr = {{5.0, 5.0}};
        assert(normDispOrUnbalance(dx, dr, 0.01, 0.01, 1, 3) == -2);
    }
}

#include <vector>
#include <cmath>
#include <numeric>

// Compute the Euclidean norm (p=2) of a vector.
double euclideanNorm(const std::vector<double>& vec) {
    double sum = 0.0;
    for (double val : vec) {
        sum += val * val;
    }
    return std::sqrt(sum);
}

// Simulate the NormDispOrUnbalance convergence test.
// Returns: current iteration (1-based) on convergence, -1 if not yet converged but within limits,
//          -2 if failure (limits exceeded or empty/invalid inputs).
int normDispOrUnbalance(
    const std::vector<std::vector<double>>& deltaX,
    const std::vector<std::vector<double>>& deltaR,
    double tolDisp,
    double tolUnbalance,
    int maxNumIter,
    int maxIncr)
{
    // Validate inputs: equal non-empty sizes and positive maxNumIter.
    if (deltaX.empty() || deltaR.empty() || deltaX.size() != deltaR.size() || maxNumIter <= 0) {
        return -2;
    }

    int iterations = static_cast<int>(deltaX.size());
    int limit = (maxNumIter < iterations) ? maxNumIter : iterations;

    int currentIter = 1;
    int numIncr = 0;
    double prevNormX = 0.0;
    double prevNormB = 0.0;

    for (int i = 0; i < limit; ++i) {
        double normX = euclideanNorm(deltaX[i]);
        double normB = euclideanNorm(deltaR[i]);

        // Check for increment violation (starting from second iteration).
        if (i > 0 && normX > prevNormX && normB > prevNormB) {
            numIncr++;
        }

        prevNormX = normX;
        prevNormB = normB;

        // Convergence check.
        if (normX <= tolDisp || normB <= tolUnbalance) {
            return currentIter;
        }

        // Failure check: increment count exceeds maxIncr (if maxIncr non-negative) or iteration limit.
        if ((maxIncr >= 0 && numIncr > maxIncr) || currentIter >= maxNumIter) {
            return -2;
        }

        currentIter++;
    }

    // If we exited the loop without returning (e.g., reached end of input), treat as failure.
    return -2;
}

// The solution mirrors the logic from the original `test()` method but simplified to a pure function. The main steps: (1) Verify inputs—if either vector is empty or lengths differ, return a sentinel (e.g., `-2`). (2) Initialize `currentIter = 1`, `numIncr = 0`, and track the previous iteration's displacement norm `prevNormX` and residual norm `prevNormB` (initialize to `0.0`). (3) At each iteration `i` (0-indexed from 0 to min(maxNumIter, input size) - 1): compute `normX = EuclideanNorm(deltaX[i])` and `normB = EuclideanNorm(deltaR[i])`. (4) If `i > 0` (i.e., after the first iteration) and both `normX > prevNormX` and `normB > prevNormB`, increment `numIncr`. (5) Update `prevNormX` and `prevNormB` to current norms. (6) Check convergence: if `normX <= tolDisp || normB <= tolUnbalance`, return `currentIter`. (7) Check failure: if `(maxIncr >= 0 && numIncr > maxIncr)` OR `currentIter >= maxNumIter`, return `-2`. (8) Otherwise, increment `currentIter` and continue. After the loop, if no convergence, return `-2`. Edge cases: `maxIncr` negative invalidates the increment-limit check; if `maxNumIter` is 0 or negative, the loop doesn't execute and the function should fail immediately. The Euclidean norm uses `std::sqrt(std::inner_product(...))`. Time complexity is O(n * m) where n is number of iterations processed (up to min(maxNumIter, input size)) and m is the vector length; space complexity is O(1) auxiliary.
