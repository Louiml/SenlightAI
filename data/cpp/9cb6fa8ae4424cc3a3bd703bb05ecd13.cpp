Write a C++ function named `computeFinalStatus` that simulates the core decision logic of the provided Albany/Piro analysis program, but in a standalone, simplified form. The function should take three parameters: (1) an integer `failedComparisons` representing the number of failed regression comparisons (initially accumulated by some analysis process), (2) an integer `mpiRank` (the rank of the current process in an MPI-style environment, where rank 0 is the "master"), and (3) a boolean `solverSucceeded` indicating whether the underlying solver/analysis step completed without throwing an exception. The function must return the final exit status as an integer, following these exact rules: If `solverSucceeded` is `false`, add `10000` to the current `failedComparisons` and return that. Otherwise, if `mpiRank` is greater than `0` (non-master processes), return `0` (regardless of `failedComparisons`). If `mpiRank` is `0`, return `failedComparisons` as-is. The function must handle edge cases such as negative input values for `failedComparisons` (treat any negative value as if it were `0` before applying the rules), and it must use `const` correctly for read-only parameters. The function should not print anything, not throw exceptions, and not depend on any external libraries beyond the C++ standard library.

// The solution requires a straightforward conditional logic chain that mirrors the behavior of the original code snippet's final status calculation. The main algorithm is as follows:  
// 1. First, clamp `failedComparisons` to be at least 0 (if it's negative, set it to 0).  
// 2. Check the `solverSucceeded` flag. If it is `false`, return `failedComparisons + 10000`.  
// 3. If `solverSucceeded` is `true`, check `mpiRank`. If `mpiRank > 0`, return `0`.  
// 4. If `mpiRank == 0` (or less, though normally only 0 or positive), return the clamped `failedComparisons`.  
//
// This mirrors the original code where `status` accumulates failures, then if an exception occurs (`success == false`), `status += 10000`, and non-zero ranks reset status to 0 for regression checks. The edge cases are: negative input values for `failedComparisons` (clamp to 0, as negative failures make no sense), and the precedence of the exception check over the rank check (if an exception occurred, even rank 0 gets the +10000 penalty). Time complexity is O(1) since it's just a few comparisons and arithmetic operations. Space complexity is O(1) as well, using only a few local variables.

#include <algorithm>

// Compute the final program exit status based on failed comparisons,
// MPI rank, and solver success. Negative failedComparisons are treated as 0.
int computeFinalStatus(int failedComparisons, int mpiRank, bool solverSucceeded) {
    // Clamp negative values to 0, as negative failure counts are invalid.
    int status = std::max(failedComparisons, 0);

    // If the solver step failed (e.g., an exception was caught),
    // add a large penalty for the failure.
    if (!solverSucceeded) {
        status += 10000;
        return status;
    }

    // On non-master MPI ranks (rank > 0), regression comparisons are skipped,
    // so the status is reset to 0.
    if (mpiRank > 0) {
        return 0;
    }

    // On rank 0 (or negative ranks), return the accumulated failures.
    return status;
}

#include <cassert>

int main() {
    // Basic positive case: rank 0, no failures, solver succeeded.
    assert(computeFinalStatus(0, 0, true) == 0);
    // Rank 0, some failures, solver succeeded.
    assert(computeFinalStatus(5, 0, true) == 5);
    // Non-master rank, failures present, solver succeeded -> reset to 0.
    assert(computeFinalStatus(7, 1, true) == 0);
    assert(computeFinalStatus(3, 2, true) == 0);
    // Solver failed, rank 0, no failures -> penalty added.
    assert(computeFinalStatus(0, 0, false) == 10000);
    // Solver failed, non-master rank -> penalty added before rank check.
    assert(computeFinalStatus(2, 1, false) == 10002);
    // Negative failedComparisons are clamped to 0.
    assert(computeFinalStatus(-10, 0, true) == 0);
    assert(computeFinalStatus(-1, 0, false) == 10000);
    // Edge case: negative rank (unusual) treated like master.
    assert(computeFinalStatus(4, -1, true) == 4);
    // Edge case: large failures plus penalty.
    assert(computeFinalStatus(12345, 0, false) == 22345);
    // Edge case: large failures on non-master rank with solver fail.
    assert(computeFinalStatus(999, 3, false) == 10999);
}
