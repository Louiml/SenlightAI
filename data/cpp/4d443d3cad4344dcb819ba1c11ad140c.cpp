Given a sequence of coin outputs where each output has a value and an availability flag (available outputs are non-zero, spent outputs are zero), write a C++ function that takes a vector of unsigned 64-bit integers representing output values (where 0 means spent/unavailable) and returns a pair of unsigned integers: (a) the number of bytes needed to encode availability of all outputs starting from the third output (index 2) onward, where each byte packs 8 consecutive outputs (bit = 1 if output is non-zero, bit = 0 if zero), and (b) the count of such bytes that contain at least one non-zero bit. The first two outputs (indices 0 and 1) are handled separately and are excluded from the mask. If no outputs exist beyond index 1, both returned numbers must be 0. Atomicity: treat the output vector as immutable; do not modify it. Your function should be named `computeOutputAvailabilityMaskSize` and must accept a `const std::vector<uint64_t>&` by const reference.
// The task is a direct algorithmic translation of the `CalcMaskSize` method from the provided snippet. The main idea is to iterate over groups of 8 outputs starting from index 2. For each group (i.e., for each byte index `b`), we check whether any output in that group (positions `2 + b*8 + i` for `i = 0..7`) is non-zero. If at least one is non-zero, we mark that byte as "used" and increment the non-zero byte counter. We also track the index of the last used byte (starting from 0). After processing all groups, `nBytes` is simply (last used byte index + 1) if there was at least one non-zero byte, otherwise 0; `nNonzeroBytes` is the total count of used bytes. Edge cases: (1) If `vout.size()` is 2 or less, loop never executes, both results are 0. (2) The last group may have fewer than 8 outputs; we must stop reading beyond `vout.size()`. (3) We treat value 0 as "null" (spent) and any non-zero value as available, matching the `IsNull()` semantics from the snippet (where a `CTxOut` is null if its value is zero and script is empty; here we simplify to value==0). The loop runs over `b` from 0 while `2 + b*8 < size`, so the number of iterations is approximately `(size - 2) / 8`. Complexity: O(n) time where n is the number of outputs, O(1) auxiliary space (we only use few integer counters).
#include <vector>
#include <utility>

/**
 * Compute the number of bytes needed to encode availability (non-zero) of outputs
 * starting from index 2, and how many of those bytes contain at least one non-zero bit.
 * Each byte encodes 8 consecutive outputs; a bit is 1 if the corresponding output value is non-zero.
 * This is a simplified version of CCoins::CalcMaskSize.
 *
 * @param vout Vector of output values; 0 means spent/unavailable, non-zero means available.
 * @return Pair of (total_bytes_needed, non_zero_bytes_count). Both are 0 if no outputs beyond index 1 exist.
 */
std::pair<unsigned int, unsigned int> computeOutputAvailabilityMaskSize(const std::vector<uint64_t>& vout)
{
    unsigned int nBytes = 0;
    unsigned int nNonzeroBytes = 0;
    unsigned int nLastUsedByte = 0;

    // Iterate over groups of 8 outputs starting from index 2
    for (unsigned int b = 0; 2 + b * 8 < vout.size(); ++b) {
        bool fZero = true;
        for (unsigned int i = 0; i < 8 && 2 + b * 8 + i < vout.size(); ++i) {
            if (vout[2 + b * 8 + i] != 0) {
                fZero = false;
                break;
            }
        }
        if (!fZero) {
            nLastUsedByte = b + 1;
            nNonzeroBytes++;
        }
    }

    nBytes = nLastUsedByte;
    return {nBytes, nNonzeroBytes};
}
#include <cassert>
#include <vector>
#include <cstdint>

// Declaration (already in solution)
std::pair<unsigned int, unsigned int> computeOutputAvailabilityMaskSize(const std::vector<uint64_t>& vout);

int main() {
    // Empty vector: no outputs beyond index 1, both zero
    assert(computeOutputAvailabilityMaskSize({}) == std::make_pair(0u, 0u));

    // Only two outputs: no mask needed
    assert(computeOutputAvailabilityMaskSize({5, 0}) == std::make_pair(0u, 0u));

    // Exactly 8 outputs beyond index 2 (indices 2..9): one byte needed, it's non-zero
    std::vector<uint64_t> v1 = {1, 2, 3, 0, 0, 0, 0, 0, 0, 5}; // size=10, outputs 2..9 includes 3 and 5
    assert(computeOutputAvailabilityMaskSize(v1) == std::make_pair(1u, 1u));

    // 16 outputs beyond index 2 (indices 2..17), all zero except first of second byte
    // Byte0 (outputs 2..9) all zero => not counted
    // Byte1 (outputs 10..17) has a non-zero at output 10 => counted
    std::vector<uint64_t> v2(18, 0);
    v2[2] = 0; // all byte0 zero
    v2[10] = 7; // byte1 has a non-zero
    assert(computeOutputAvailabilityMaskSize(v2) == std::make_pair(2u, 1u)); // last used byte index 1 => nBytes=2, nonzero=1

    // All non-zero: every byte used
    std::vector<uint64_t> v3(10, 42); // indices 0..9, mask covers indices 2..9 = 8 outputs = 1 byte
    assert(computeOutputAvailabilityMaskSize(v3) == std::make_pair(1u, 1u));

    // Exact multiple of 8 outputs beyond index 2: 8 outputs => 1 byte; 16 outputs => 2 bytes, all non-zero
    std::vector<uint64_t> v4(18, 1); // size=18, outputs 2..17 = 16 outputs
    assert(computeOutputAvailabilityMaskSize(v4) == std::make_pair(2u, 2u));

    // Partial last byte: indices 2..4 (3 outputs) all zero -> no bytes at all
    std::vector<uint64_t> v5 = {1, 1, 0, 0, 0}; // size=5, outputs 2,3,4 zero
    assert(computeOutputAvailabilityMaskSize(v5) == std::make_pair(0u, 0u));

    // Mixed: indices 2..9 has some non-zero, indices 10..12 all zero (partial byte, doesn't count)
    std::vector<uint64_t> v6(13, 0);
    v6[2] = 1; // byte0 non-zero
    v6[3] = 0;
    v6[4] = 0;
    v6[5] = 0;
    v6[6] = 0;
    v6[7] = 0;
    v6[8] = 0;
    v6[9] = 0;
    // outputs 10..12 are zero
    assert(computeOutputAvailabilityMaskSize(v6) == std::make_pair(1u, 1u));

    return 0;
}
