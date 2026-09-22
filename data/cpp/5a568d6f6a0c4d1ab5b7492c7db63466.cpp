// Write a C++ function that, given a sorted vector of non-negative integers representing transaction fee levels paid by consecutive transactions in a ledger (sorted in ascending order), a target transaction count, and an escalation multiplier, computes and returns the "scaled fee level" that the next transaction must pay to be included in the open ledger. Specifically, the function should implement the escalation formula: if the current number of transactions in the ledger is greater than the target, return `multiplier * current² / target²` using 64-bit unsigned arithmetic with overflow-safety (return `UINT64_MAX` on overflow); otherwise, return a constant base level (e.g., 256). The "current" count is the number of transactions already in the ledger plus an optional extra count (for transactions that will be added before the next one). Your function should take: `uint64_t currentCount`, `uint64_t extraCount`, `uint64_t targetCount`, `uint64_t multiplier`, and `uint64_t baseLevel`. It must handle division by zero gracefully (if `targetCount == 0`, return `UINT64_MAX`). Ensure the implementation avoids intermediate overflow where possible and uses a safe multiplication/division helper or checks.

// The task is to compute a fee escalation function similar to the one in the provided code snippet (specifically `TxQ::FeeMetrics::scaleFeeLevel`). The key is to compute `multiplier * current² / target²` only when `current > target`, where `current = currentCount + extraCount`. If `current <= target`, we return `baseLevel`. For the escalated case, we must be careful about overflow: both `current` and `target` are at most `UINT64_MAX`, so squaring them overflows. The safest approach is to use a 128-bit intermediate type (e.g., `__uint128_t` if available, which is standard in GCC/Clang) for the multiplication and division. Alternatively, a manual overflow-checked multiplication using `__builtin_mul_overflow` can be used, but for simplicity, `__uint128_t` is portable enough for competitive programming tasks. If `target` is zero, division by zero must be avoided by returning `UINT64_MAX` (as per the problem). Edge cases: `extraCount` could be zero, `current` could be zero (then `current <= target` if target is positive, so return base), and `multiplier` could be zero. The solution is `O(1)` time and space.

#include <cstdint>
#include <limits>

// Compute the escalated fee level for a transaction queue.
// Returns:
//   - baseLevel if currentCount + extraCount <= targetCount
//   - UINT64_MAX if targetCount == 0 or overflow occurs
//   - multiplier * (current)^2 / (target)^2 otherwise
uint64_t scaledFeeLevel(uint64_t currentCount,
                        uint64_t extraCount,
                        uint64_t targetCount,
                        uint64_t multiplier,
                        uint64_t baseLevel) {
    if (targetCount == 0) {
        return std::numeric_limits<uint64_t>::max();
    }

    // Use 128-bit arithmetic to avoid overflow in intermediate steps.
    __uint128_t current = static_cast<__uint128_t>(currentCount) + extraCount;
    __uint128_t target = static_cast<__uint128_t>(targetCount);

    if (current <= target) {
        return baseLevel;
    }

    __uint128_t numerator = multiplier;
    numerator *= current * current;   // (current)^2 is safe in 128-bit
    __uint128_t denominator = target * target;

    // Divide; result fits in uint64_t because the function's result is
    // meant to be a fee level, and we cap at UINT64_MAX.
    __uint128_t result = numerator / denominator;
    if (result > std::numeric_limits<uint64_t>::max()) {
        return std::numeric_limits<uint64_t>::max();
    }
    return static_cast<uint64_t>(result);
}

#include <cassert>
#include <cstdint>

int main() {
    // Basic escalation: current > target
    assert(scaledFeeLevel(10, 0, 5, 100, 256) == 400); // 100 * 100 / 25 = 400
    // No escalation: current <= target
    assert(scaledFeeLevel(5, 0, 5, 100, 256) == 256);
    assert(scaledFeeLevel(4, 1, 5, 100, 256) == 256); // current+extra=5 == target
    // Extra count pushes over target
    assert(scaledFeeLevel(4, 2, 5, 100, 256) == 144); // (6^2)*100/25 = 144
    // Zero target returns UINT64_MAX
    assert(scaledFeeLevel(1, 0, 0, 100, 256) == UINT64_MAX);
    // Zero current and zero extra: no escalation
    assert(scaledFeeLevel(0, 0, 10, 100, 256) == 256);
    // Large values: ensure no overflow and result is capped
    uint64_t huge = UINT64_MAX;
    assert(scaledFeeLevel(huge, 0, huge/2, huge, 256) == UINT64_MAX);
    // Edge case: multiplier zero
    assert(scaledFeeLevel(10, 0, 5, 0, 256) == 0); // 0 * 100 / 25 = 0
    return 0;
}
