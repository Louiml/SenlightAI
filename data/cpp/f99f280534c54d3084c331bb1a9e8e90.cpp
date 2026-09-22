Given a transaction represented as a simplified struct with a vector of inputs (each having a sequence number) and a lock time (a 32-bit unsigned integer), write a C++ function `bool IsFinalTxBasic(const std::vector<uint32_t>& inputSequences, uint32_t lockTime, int blockHeight, int64_t blockTime)` that determines whether the transaction is final according to Bitcoin consensus rules: a transaction is final if its lock time is zero, or if the lock time is less than the given block height (if lock time is below the threshold constant `LOCKTIME_THRESHOLD = 500000000`) or less than the given block time (if lock time is at or above the threshold), and all input sequence numbers equal `UINT32_MAX` (the constant for `SEQUENCE_FINAL`). If any input sequence is not `UINT32_MAX`, the transaction is not final regardless of the lock time. Return `true` if final, `false` otherwise.
// The problem mirrors the logic in the provided `IsFinalTx` function, but simplified to work with basic primitives instead of the full `CTransaction`/`CTxIn` classes. The algorithm first checks if `lockTime == 0`; if so, return `true` immediately. Otherwise, interpret `lockTime` as either a block height (if `lockTime < 500000000`) or a Unix timestamp (if `lockTime >= 500000000`); compare it against the provided `blockHeight` or `blockTime` respectively. If `lockTime` is strictly less than the corresponding block parameter, the lock time condition is satisfied; otherwise, the transaction is not final. After the lock time check passes, iterate over every element in `inputSequences`. If any element is not equal to `UINT32_MAX` (the maximum possible value representing "final" sequence), return `false`. Only if all sequences are final and the lock time condition holds, return `true`. Edge cases include an empty input vector (trivially final if lock time condition passes) and the exact equality of `lockTime` with the block height/time (must be strictly less to be final). Time complexity is \(O(n)\) where \(n\) is the size of the input sequences vector; space complexity is \(O(1)\) auxiliary.
#include <cstdint>
#include <vector>
#include <algorithm>

// Threshold below which a lock time is considered a block height.
constexpr uint32_t LOCKTIME_THRESHOLD = 500000000;

// Determine if a transaction (with given input sequences and lock time) is final.
bool IsFinalTxBasic(const std::vector<uint32_t>& inputSequences,
                    uint32_t lockTime,
                    int blockHeight,
                    int64_t blockTime) {
    // Zero lock time is always final.
    if (lockTime == 0) {
        return true;
    }

    // Compare lock time against the relevant block parameter.
    if (lockTime < LOCKTIME_THRESHOLD) {
        // Interpretation: block height.
        if (static_cast<int64_t>(lockTime) >= static_cast<int64_t>(blockHeight)) {
            return false;
        }
    } else {
        // Interpretation: Unix timestamp.
        if (static_cast<int64_t>(lockTime) >= blockTime) {
            return false;
        }
    }

    // All input sequences must be final (UINT32_MAX).
    for (uint32_t seq : inputSequences) {
        if (seq != UINT32_MAX) {
            return false;
        }
    }

    return true;
}
#include <cassert>
#include <cstdint>
#include <vector>

// Declaration of the function under test (assume provided elsewhere).
bool IsFinalTxBasic(const std::vector<uint32_t>& inputSequences,
                    uint32_t lockTime,
                    int blockHeight,
                    int64_t blockTime);

int main() {
    // Zero lock time: always final, regardless of sequences.
    assert(IsFinalTxBasic({0, 0}, 0, 100, 1000000) == true);
    assert(IsFinalTxBasic({UINT32_MAX, UINT32_MAX}, 0, 0, 0) == true);

    // Lock time as height (below threshold), height 105, lock 100: final if sequences final.
    assert(IsFinalTxBasic({UINT32_MAX, UINT32_MAX}, 100, 105, 0) == true);
    // Lock time equals height: not final.
    assert(IsFinalTxBasic({UINT32_MAX}, 105, 105, 0) == false);
    // Lock time greater than height: not final.
    assert(IsFinalTxBasic({UINT32_MAX}, 200, 105, 0) == false);

    // Lock time as timestamp (at or above threshold), time 2000000, lock 1500000: final.
    assert(IsFinalTxBasic({UINT32_MAX}, 1500000, 0, 2000000) == true);
    // Lock time equals time: not final.
    assert(IsFinalTxBasic({UINT32_MAX}, 2000000, 0, 2000000) == false);
    // Lock time greater than time: not final.
    assert(IsFinalTxBasic({UINT32_MAX}, 2500000, 0, 2000000) == false);

    // Non-final sequence makes transaction non-final, even if lock time passes.
    assert(IsFinalTxBasic({UINT32_MAX - 1}, 100, 200, 0) == false);
    assert(IsFinalTxBasic({0, UINT32_MAX}, 0, 0, 0) == true); // Lock time zero overrides sequences.
    assert(IsFinalTxBasic({0, UINT32_MAX}, 50, 100, 0) == false);

    // Empty input vector: only lock time matters.
    assert(IsFinalTxBasic({}, 50, 100, 0) == true);
    assert(IsFinalTxBasic({}, 150, 100, 0) == false);
    assert(IsFinalTxBasic({}, 500000000, 0, 1000000) == true);
    assert(IsFinalTxBasic({}, 500000001, 0, 1000000) == false);

    return 0;
}
