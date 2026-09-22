/*
Write a C++ function `computeKnownBitsForConstantExpression` that takes a `std::vector<int64_t>` representing the operands of a unary or binary bitwise/arithmetic operation and an `int64_t opcode` (where `1` = ADD, `2` = AND, `3` = OR, `4` = XOR, `5` = SHL, `6` = LSHR, `7` = ASHR, `8` = MUL). The function should perform constant propagation to compute the known bits (i.e., bits that are guaranteed to be 0 or 1 in the result) for the operation on those constant operands, and return a `std::pair<uint64_t, uint64_t>` where the first element is the `knownZero` mask (bits known to be 0) and the second is the `knownOne` mask (bits known to be 1), both restricted to 64-bit width. For unary operations like MUL with only one operand, treat the second operand as 1. For shifts, the shift amount is taken from the second operand (0 if not provided). The function must handle constant folding: if both operands are fully known (i.e., their knownOne mask equals the value and knownZero is the complement), compute the exact result and return fully known bits; otherwise, apply bitwise propagation rules for each operation (e.g., for AND, knownOne = knownOne₁ & knownOne₂, knownZero = knownZero₁ | knownZero₂; for ADD, use carry propagation; for SHL, shift the known bits; for MUL, propagate low bits; for unknown shifts, set all bits unknown). The input operands are given as a vector of actual constant values, but the function must treat them as if they were partially known: for each operand value, set knownOne = value, knownZero = ~value. The result must be an exact 64-bit pair with no unknown bits outside the 64-bit range.
*/
#include <cstdint>
#include <utility>
#include <vector>
#include <limits>

// Represent known bits: zero mask (bits known to be 0) and one mask (bits known to be 1)
using KnownBitsPair = std::pair<uint64_t, uint64_t>;

constexpr uint64_t WIDTH = 64;

// Combine two KnownBitsPair objects for bitwise AND
KnownBitsPair knownAnd(const KnownBitsPair& a, const KnownBitsPair& b) {
    return {a.first | b.first, a.second & b.second};
}

// Combine for bitwise OR
KnownBitsPair knownOr(const KnownBitsPair& a, const KnownBitsPair& b) {
    return {a.first & b.first, a.second | b.second};
}

// Combine for bitwise XOR
KnownBitsPair knownXor(const KnownBitsPair& a, const KnownBitsPair& b) {
    return {a.first | b.first, (a.second ^ b.second) & ~(a.first | b.first)};
}

// For addition: propagate carry bit by bit
KnownBitsPair knownAdd(const KnownBitsPair& a, const KnownBitsPair& b) {
    uint64_t zeros = 0;
    uint64_t ones = 0;
    bool carryKnown = true;
    bool carryValue = false;
    for (int i = 0; i < WIDTH; ++i) {
        bool aKnown = ((a.first >> i) & 1) || ((a.second >> i) & 1);
        bool bKnown = ((b.first >> i) & 1) || ((b.second >> i) & 1);
        bool aVal = (a.second >> i) & 1;
        bool bVal = (b.second >> i) & 1;
        if (aKnown && bKnown) {
            bool sum = aVal ^ bVal ^ carryValue;
            if (sum) ones |= (1ULL << i);
            else zeros |= (1ULL << i);
            carryValue = (aVal & bVal) | (aVal & carryValue) | (bVal & carryValue);
            carryKnown = true;
        } else {
            // If carry is known, we can determine sum if one operand is known and carry plus other is known
            if (carryKnown && aKnown) {
                bool sum = aVal ^ carryValue;
                if (sum) ones |= (1ULL << i);
                else zeros |= (1ULL << i);
                // carry remains unknown because b is unknown
                carryKnown = false;
            } else if (carryKnown && bKnown) {
                bool sum = bVal ^ carryValue;
                if (sum) ones |= (1ULL << i);
                else zeros |= (1ULL << i);
                carryKnown = false;
            } else {
                // both or one unknown without carry -> stop
                // mark remaining bits as unknown and break
                uint64_t mask = ~0ULL << i;
                zeros |= mask;
                ones |= mask;
                break;
            }
        }
    }
    // Ensure no overlap
    zeros &= ~ones;
    return {zeros, ones};
}

// For logical shift left: shift known bits, clear low bits (fill with 0)
KnownBitsPair knownShl(const KnownBitsPair& a, uint64_t shift) {
    if (shift >= WIDTH) {
        return {std::numeric_limits<uint64_t>::max(), 0};
    }
    uint64_t zeros = (a.first << shift) | ((1ULL << shift) - 1);
    uint64_t ones = a.second << shift;
    zeros &= ~ones;
    return {zeros, ones};
}

// For logical shift right: shift known bits, clear high bits (fill with 0)
KnownBitsPair knownLshr(const KnownBitsPair& a, uint64_t shift) {
    if (shift >= WIDTH) {
        return {std::numeric_limits<uint64_t>::max(), 0};
    }
    uint64_t zeros = (a.first >> shift) | (~0ULL << (WIDTH - shift));
    uint64_t ones = a.second >> shift;
    zeros &= ~ones;
    return {zeros, ones};
}

// For arithmetic shift right: shift known bits, fill with sign bit (bit 63)
KnownBitsPair knownAshr(const KnownBitsPair& a, uint64_t shift) {
    if (shift >= WIDTH) {
        // If sign bit is known, all bits become that sign bit
        bool signKnown = ((a.first >> 63) & 1) || ((a.second >> 63) & 1);
        if (signKnown) {
            bool sign = (a.second >> 63) & 1;
            uint64_t v = sign ? ~0ULL : 0;
            return {v ^ 0, v};
        }
        return {0, 0};
    }
    uint64_t zeros = a.first >> shift;
    uint64_t ones = a.second >> shift;
    // If sign bit is known, fill high bits with that bit
    bool signKnown = ((a.first >> 63) & 1) || ((a.second >> 63) & 1);
    if (signKnown) {
        bool sign = (a.second >> 63) & 1;
        uint64_t fill = sign ? ~0ULL : 0;
        uint64_t highMask = ~0ULL << (WIDTH - shift);
        if (sign) ones |= highMask;
        else zeros |= highMask;
    } else {
        // unknown sign, high bits unknown
        uint64_t highMask = ~0ULL << (WIDTH - shift);
        zeros |= highMask;
        ones |= highMask;
    }
    zeros &= ~ones;
    return {zeros, ones};
}

// Main function: compute known bits for a constant expression
KnownBitsPair computeKnownBitsForConstantExpression(const std::vector<int64_t>& operands, int64_t opcode) {
    // Convert each operand to a fully known KnownBitsPair
    std::vector<KnownBitsPair> knowns;
    for (int64_t v : operands) {
        uint64_t uv = static_cast<uint64_t>(v);
        knowns.push_back({~uv, uv});
    }
    // Ensure at least one operand for unary-like operations
    if (knowns.empty()) {
        return {std::numeric_limits<uint64_t>::max(), 0};
    }
    // If only one operand and opcode is MUL, treat second as 1
    if (knowns.size() == 1 && opcode == 8) {
        knowns.push_back({~1ULL, 1ULL});
    }
    // If only one operand and opcode is shift, treat second as 0
    if (knowns.size() == 1 && (opcode == 5 || opcode == 6 || opcode == 7)) {
        knowns.push_back({~0ULL, 0ULL});
    }

    switch (opcode) {
        case 1: // ADD
            return knownAdd(knowns[0], knowns[1]);
        case 2: // AND
            return knownAnd(knowns[0], knowns[1]);
        case 3: // OR
            return knownOr(knowns[0], knowns[1]);
        case 4: // XOR
            return knownXor(knowns[0], knowns[1]);
        case 5: // SHL
            if (knowns[1].first == 0 && knowns[1].second != 0) { // fully known shift amount
                return knownShl(knowns[0], knowns[1].second);
            }
            return {0, 0}; // unknown shift
        case 6: // LSHR
            if (knowns[1].first == 0 && knowns[1].second != 0) {
                return knownLshr(knowns[0], knowns[1].second);
            }
            return {0, 0};
        case 7: // ASHR
            if (knowns[1].first == 0 && knowns[1].second != 0) {
                return knownAshr(knowns[0], knowns[1].second);
            }
            return {0, 0};
        case 8: // MUL
            // If both fully known, constant fold
            if ((knowns[0].first == 0 && knowns[0].second != 0) &&
                (knowns[1].first == 0 && knowns[1].second != 0)) {
                uint64_t prod = knowns[0].second * knowns[1].second;
                return {~prod, prod};
            }
            // If one operand is 1, result is other operand's known bits
            if (knowns[0].second == 1 && knowns[0].first == ~1ULL) {
                return knowns[1];
            }
            if (knowns[1].second == 1 && knowns[1].first == ~1ULL) {
                return knowns[0];
            }
            return {0, 0}; // otherwise unknown
        default:
            return {0, 0}; // unknown opcode
    }
}
#include <cassert>
#include <cstdint>
#include <vector>
#include <utility>

// Declaration of the solution function (assume it's included from above)
KnownBitsPair computeKnownBitsForConstantExpression(const std::vector<int64_t>& operands, int64_t opcode);

int main() {
    // ADD: 3 + 5 = 8 (known bits: zero ~8, one 8)
    auto r1 = computeKnownBitsForConstantExpression({3, 5}, 1);
    assert(r1.first == ~8ULL && r1.second == 8ULL);

    // AND: 12 & 10 = 8 (bits: 1100 & 1010 = 1000)
    auto r2 = computeKnownBitsForConstantExpression({12, 10}, 2);
    assert(r2.first == ~8ULL && r2.second == 8ULL);

    // OR: 12 | 10 = 14 (1110)
    auto r3 = computeKnownBitsForConstantExpression({12, 10}, 3);
    assert(r3.first == ~14ULL && r3.second == 14ULL);

    // XOR: 12 ^ 10 = 6 (0110)
    auto r4 = computeKnownBitsForConstantExpression({12, 10}, 4);
    assert(r4.first == ~6ULL && r4.second == 6ULL);

    // SHL: 1 << 4 = 16
    auto r5 = computeKnownBitsForConstantExpression({1, 4}, 5);
    assert(r5.first == ~16ULL && r5.second == 16ULL);

    // LSHR: 16 >> 2 = 4
    auto r6 = computeKnownBitsForConstantExpression({16, 2}, 6);
    assert(r6.first == ~4ULL && r6.second == 4ULL);

    // ASHR: -8 >> 1 = -4 (arithmetic shift, sign extension)
    auto r7 = computeKnownBitsForConstantExpression({-8, 1}, 7);
    assert(r7.first == ~static_cast<uint64_t>(-4) && r7.second == static_cast<uint64_t>(-4));

    // MUL: 6 * 7 = 42
    auto r8 = computeKnownBitsForConstantExpression({6, 7}, 8);
    assert(r8.first == ~42ULL && r8.second == 42ULL);

    // MUL with single operand (treat as *1): 5 * 1 = 5
    auto r9 = computeKnownBitsForConstantExpression({5}, 8);
    assert(r9.first == ~5ULL && r9.second == 5ULL);

    // AND with zero: 5 & 0 = 0
    auto r10 = computeKnownBitsForConstantExpression({5, 0}, 2);
    assert(r10.first == ~0ULL && r10.second == 0ULL);

    // OR with all ones: 5 | 0xFFFFFFFFFFFFFFFF = all ones
    auto r11 = computeKnownBitsForConstantExpression({5, -1}, 3);
    assert(r11.first == 0ULL && r11.second == ~0ULL);

    return 0;
}
// The core idea is to simulate a simplified version of LLVM's `KnownBits` propagation for constant operands. For each input constant, we immediately have full knowledge: `Zero = ~value` and `One = value`. We then process the operation recursively by computing known bits for each source operand and combining them per operation-specific rules. For binary operations, we extract known bits from both operands. For shifts, we must handle the case where the shift amount is unknown (i.e., the operand has unknown bits) by conservatively setting the result to unknown, or if the shift amount is fully known, apply the shift to the known zero and one masks, clearing bits shifted in (for logical shifts, fill with 0; for arithmetic, fill with the sign bit; for left shift, fill with 0). For addition, we use the standard carry-propagation algorithm: initialize carry unknown, but when both input bits are known, compute sum bit and carry; if a carry is unknown, the sum bit may still be known if both inputs are known to be 0 or known to be 1 with known carry, but to keep it simple we propagate bit by bit with a known-carry flag; if carry becomes unknown, only bits where both inputs are known can be determined. For multiplication, we can start from the LSB: for each bit position, if both operands have a known bit at that position, we can determine the contribution; however, since a full multiplier propagation is complex, we handle the case where one operand is 1 (identity) or where both are fully known (constant fold), otherwise return fully unknown. Edge cases: shifts with shift amount >= 64 or negative (treat as unknown), division/modulo not included, and operands must be treated as unsigned 64-bit values. Time complexity is O(64) for each operation, which is constant; space O(1). The function must be `const`-correct and use `uint64_t` to avoid signed overflow issues.
