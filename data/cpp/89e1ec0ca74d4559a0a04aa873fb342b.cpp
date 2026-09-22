// Write a C++ function that implements the core of the Weak-Zero SIV (Single Index Variable) dependence test, which is a simplified version of the technique used in LLVM's dependence analysis. Given two integer expressions represented as simple linear forms (a constant plus a coefficient times an induction variable, or just a constant), along with the loop's upper bound (trip count), determine whether the two array subscript expressions can ever be equal for some valid loop iteration value in the range `[0, UpperBound]`. The function should return `true` if a dependence is provably impossible (i.e., the two expressions can never be equal for any valid iteration), and `false` if a dependence is possible (including when we cannot prove independence). The two expressions are provided as follows: each expression is a `LinearExpr` structure containing a constant term and a coefficient for the induction variable. The first expression corresponds to `SrcConst + SrcCoeff * i` and the second to `DstConst + DstCoeff * i`, where `i` ranges from `0` to `UpperBound` inclusive. The function should handle the four cases: both expressions have non-zero coefficients, only the first has a coefficient, only the second has a coefficient, or both are loop-invariant constants (ZIV case). Use signed 64-bit integers (`int64_t`) for all arithmetic with proper overflow checking (you may assume inputs are within `int64_t` range and that intermediate products fit within `int64_t`). The function signature is: `bool weakZeroSIVIndependent(int64_t SrcConst, int64_t SrcCoeff, int64_t DstConst, int64_t DstCoeff, int64_t UpperBound);`
#include <cassert>

// Declaration of the function under test (as defined above).
bool weakZeroSIVIndependent(int64_t SrcConst, int64_t SrcCoeff,
                            int64_t DstConst, int64_t DstCoeff,
                            int64_t UpperBound);

int main() {
    // ZIV: both loop-invariant, equal constants -> dependence (false)
    assert(weakZeroSIVIndependent(5, 0, 5, 0, 10) == false);
    // ZIV: different constants -> independent (true)
    assert(weakZeroSIVIndependent(5, 0, 6, 0, 10) == true);

    // One coefficient non-zero: SrcCoeff = 2, DstCoeff = 0
    // 3 + 2*i == 9  => i = 3, which is in [0, 5] -> dependence (false)
    assert(weakZeroSIVIndependent(3, 2, 9, 0, 5) == false);
    // i = 3 is out of range [0, 2] -> independent (true)
    assert(weakZeroSIVIndependent(3, 2, 9, 0, 2) == true);
    // Not exact division: 3 + 2*i == 8 => i = 2.5, not integer -> independent (true)
    assert(weakZeroSIVIndependent(3, 2, 8, 0, 10) == true);

    // Both non-zero: SrcCoeff = 2, DstCoeff = 1
    // 1 + 2*i == 5 + 1*i => i = 4, in [0, 5] -> dependence (false)
    assert(weakZeroSIVIndependent(1, 2, 5, 1, 5) == false);
    // i = 4 out of range [0, 3] -> independent (true)
    assert(weakZeroSIVIndependent(1, 2, 5, 1, 3) == true);
    // Parallel lines: SrcCoeff = DstCoeff but constants differ -> independent (true)
    assert(weakZeroSIVIndependent(1, 2, 5, 2, 10) == true);
    // Identical lines: same coefficient and constant -> dependence (false)
    assert(weakZeroSIVIndependent(4, 3, 4, 3, 7) == false);

    // Negative coefficients: SrcCoeff = -2, DstCoeff = 0
    // 10 - 2*i == 2 => i = 4, in [0, 5] -> dependence (false)
    assert(weakZeroSIVIndependent(10, -2, 2, 0, 5) == false);
    // i = 4 out of range [0, 3] -> independent (true)
    assert(weakZeroSIVIndependent(10, -2, 2, 0, 3) == true);
    // Negative solution: 1 - 2*i == 5 => i = -2, out of range -> independent (true)
    assert(weakZeroSIVIndependent(1, -2, 5, 0, 10) == true);

    // Both negative coefficients: SrcCoeff = -3, DstCoeff = -1
    // 8 - 3*i == 4 - 1*i => -2*i = -4 => i = 2, in [0, 5] -> dependence (false)
    assert(weakZeroSIVIndependent(8, -3, 4, -1, 5) == false);

    // Zero upper bound (single iteration): only i=0 possible
    // 2 + 0*i == 2 + 0*i -> dependence (false)
    assert(weakZeroSIVIndependent(2, 0, 2, 0, 0) == false);
    // 2 + 0*i == 3 + 0*i -> independent (true)
    assert(weakZeroSIVIndependent(2, 0, 3, 0, 0) == true);
    // 1 + 2*i == 3 + 2*i at i=0 => 1 != 3 -> independent (true)
    assert(weakZeroSIVIndependent(1, 2, 3, 2, 0) == true);

    // Negative upper bound: no iterations -> independent (true)
    assert(weakZeroSIVIndependent(1, 2, 5, 1, -1) == true);

    return 0;
}
#include <cstdint>

// Linear expression: constant + coefficient * i, where i is the loop induction variable.
struct LinearExpr {
    int64_t constant;
    int64_t coefficient;
};

// Return true if no dependence is possible between the two linear expressions
// for any integer i in [0, UpperBound]. Return false if a dependence is possible.
bool weakZeroSIVIndependent(int64_t SrcConst, int64_t SrcCoeff,
                            int64_t DstConst, int64_t DstCoeff,
                            int64_t UpperBound) {
    // If the loop has no valid iterations, no dependence.
    if (UpperBound < 0)
        return true;

    // Solve SrcConst + SrcCoeff*i == DstConst + DstCoeff*i
    // Rearranged: (SrcCoeff - DstCoeff)*i == DstConst - SrcConst
    int64_t coeffDiff = SrcCoeff - DstCoeff;
    int64_t constDiff = DstConst - SrcConst;

    // Case 1: Both expressions are loop-invariant (ZIV).
    if (coeffDiff == 0) {
        // If constants equal, they are equal for all iterations (dependence exists).
        if (constDiff == 0)
            return false; // dependence exists
        return true;      // never equal, independent
    }

    // For non-zero coeffDiff, check for an integer solution i.
    // i = constDiff / coeffDiff, must be exact and within [0, UpperBound].
    // Check exact divisibility (works for negative numbers in C++: remainder == 0).
    if (constDiff % coeffDiff != 0)
        return true; // no integer solution, independent

    int64_t i = constDiff / coeffDiff;

    // The iteration i must be in the valid range [0, UpperBound].
    if (i < 0 || i > UpperBound)
        return true; // solution outside loop bounds, independent

    // If we reach here, there's a specific iteration where the expressions are equal.
    return false; // dependence possible
}
// The solution is based on solving the linear Diophantine-like equation `SrcConst + SrcCoeff * i = DstConst + DstCoeff * i` for integer `i` in the range `[0, UpperBound]`. First, rearrange to `(SrcCoeff - DstCoeff) * i = DstConst - SrcConst`, i.e., `CoeffDiff * i = ConstDiff`. We need to check if there exists an integer `i` satisfying this and within bounds. The approach handles cases systematically:
// - **Both coefficients zero (ZIV)**: The expressions are loop-invariant constants. If `SrcConst == DstConst`, a dependence exists for all iterations (return false); otherwise, no dependence (return true).
// - **Only one expression has a non-zero coefficient**: For example, `SrcCoeff != 0` and `DstCoeff == 0` (or vice versa). This is the classic Weak-Zero SIV. The solution `i = (DstConst - SrcConst) / SrcCoeff` must be an integer (division with no remainder) and within `[0, UpperBound]`. If the division is not exact or the result is out of bounds, no dependence; otherwise, dependence exists. Handle the sign of the coefficient correctly—if the coefficient is negative, we can multiply both numerator and denominator by -1 to keep the coefficient positive, or use signed division carefully (floor/ceiling division with truncation toward zero might be incorrect; use exact check with remainder).
// - **Both coefficients non-zero**: Compute `CoeffDiff = SrcCoeff - DstCoeff` and `ConstDiff = DstConst - SrcConst`. If `CoeffDiff == 0`, then the equations are parallel; if `ConstDiff == 0`, they are identical (dependence for all `i`); otherwise, no dependence. If `CoeffDiff != 0`, solve `i = ConstDiff / CoeffDiff`. The value must be an integer (remainder zero) and within `[0, UpperBound]`. The division must use exact integer arithmetic; check `ConstDiff % CoeffDiff == 0` before dividing. Handle negative coefficients by using signed modulo and division correctly—in C++, the modulo for negative numbers can be negative, so a zero remainder check still works correctly for exact divisibility (e.g., `-6 % 3 == 0`). After verifying exact division, compute the quotient and check if it is between 0 and UpperBound inclusive.
// - **Edge cases**: UpperBound can be 0 (single iteration), in which case only `i=0` is valid. If UpperBound is negative, the loop doesn't execute, return true (no dependence). Also, ensure that for negative coefficients, the solution may be negative or positive, and the bounds check handles that.
// The time complexity is O(1) and space complexity is O(1). The main algorithmic challenge is correctly handling signed division with negative numbers to check exact divisibility and valid range.
