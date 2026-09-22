// Write a standalone C++ function `bitFieldOperations` that implements the core logic of the Maxwell XMAD instruction as a pure bit-manipulation operation. The function takes: a 32-bit unsigned integer `src_a`, a 32-bit unsigned integer `src_b`, a 32-bit unsigned integer `src_c`, a half selector `half_b` (an integer 0 or 1 indicating which 16-bit half of `src_b` to use: 0 = low half, 1 = high half), a boolean `is_a_signed`, a boolean `is_b_signed`, a boolean `psl` (if true, shift the product left by 16 bits), a boolean `mrg` (if true, replace bits 31:16 of the result with bits 15:0 of `src_b`), and a select mode integer `mode` (0=default uses full `src_c`, 1=CLO uses low 16 bits of `src_c` zero-extended, 2=CHI uses high 16 bits of `src_c` zero-extended, 3=CBCC computes `src_c + (src_b << 16)`). For `src_a`, the function always uses its low 16 bits; if `is_a_signed` is true, interpret that 16-bit value as signed two's complement and sign-extend to 32 bits, otherwise zero-extend. For `src_b`'s selected half, similarly interpret as signed or unsigned based on `is_b_signed` and sign-extend or zero-extend to 32 bits. The product is computed as a 32-bit signed? (use unsigned multiplication of the two 32-bit extended values, but note: in practice the instruction treats these as signed/unsigned as specified, and the product is truncated to 32 bits). Then, if `psl` is true, shift the product left by 16 bits (wrapping). Then add `src_c` (or the mode-based variant) to the product using 32-bit unsigned addition with wraparound. If `mrg` is true, take bits 15:0 of the original `src_b` (zero-extended to 32 bits) and replace bits 31:16 of the result with that value (i.e., result = (low16_b << 16) | (result & 0xFFFF)). The function must return the final 32-bit unsigned integer. Handle edge cases: when `mode` is outside 0-3, default to mode 0. The function should be self-contained without any external dependencies beyond standard headers.

// The solution decomposes the XMAD logic into clear steps. First, we define a helper to extract a 16-bit field from a 32-bit value at a given half offset (0 or 16). For `src_a`, we always extract the low half. For `src_b`, we extract based on `half_b` (0 or 1). After extraction, we reinterpret the 16-bit value as signed or unsigned. If signed, we sign-extend by checking the most significant bit (bit 15) and setting all higher bits accordingly; if unsigned, we simply zero-extend to 32 bits. The product is computed as `uint32_t product = static_cast<uint32_t>(static_cast<uint64_t>(op_a) * static_cast<uint64_t>(op_b));` ensuring wraparound to 32 bits. If `psl` is true, `product <<= 16;` (since shifting a 32-bit unsigned left by 16 wraps). Next, we compute `op_c` based on the mode: for 0, it's `src_c`; for 1, it's the zero-extended low 16 bits of `src_c`; for 2, it's the zero-extended high 16 bits of `src_c`; for 3, it's `src_c + (src_b << 16)` (using 32-bit unsigned addition and left shift). We add `product + op_c` using unsigned addition (naturally wraps). Finally, if `mrg` is true, we compute `low16_b = src_b & 0xFFFF` (this is the raw low half of `src_b`, not sign-extended), and set bits 31:16 of the result to that value: `result = (low16_b << 16) | (result & 0xFFFF)`. Edge cases: mode values above 3 fall back to default; signed interpretation is applied to the selected 16-bit halves for both `src_a` and `src_b`; the `psl` shift uses unsigned shift left; the `mrg` operation ignores any prior high bits of the result. Time complexity is O(1) with a small constant, space is O(1). No special numerical issues beyond standard 32-bit wraparound.

#include <cstdint>

// Extract a 16-bit field from a 32-bit value: half=0 -> bits 15:0, half=1 -> bits 31:16.
static inline uint32_t extractHalf(uint32_t value, int half) {
    return half == 0 ? (value & 0xFFFFu) : ((value >> 16) & 0xFFFFu);
}

// Sign-extend a 16-bit value to 32 bits if is_signed, otherwise zero-extend.
static inline uint32_t extend16(uint32_t halfValue, bool is_signed) {
    if (is_signed) {
        // If bit 15 is set, set all higher bits to 1.
        uint32_t signBit = halfValue & 0x8000u;
        return signBit ? (halfValue | 0xFFFF0000u) : halfValue;
    }
    return halfValue & 0xFFFFu;
}

// Core XMAD-like bit manipulation operation.
uint32_t bitFieldOperations(
    uint32_t src_a,
    uint32_t src_b,
    uint32_t src_c,
    int half_b,           // 0 = low 16 bits, 1 = high 16 bits
    bool is_a_signed,
    bool is_b_signed,
    bool psl,             // shift product left by 16
    bool mrg,             // merge low 16 of src_b into result's high 16
    int mode              // 0=default, 1=CLO, 2=CHI, 3=CBCC
) {
    // Extract half of src_a and selected half of src_b, then extend.
    uint32_t op_a = extend16(extractHalf(src_a, 0), is_a_signed);
    uint32_t op_b = extend16(extractHalf(src_b, half_b), is_b_signed);

    // Compute product with 32-bit wraparound.
    uint32_t product = static_cast<uint32_t>(
        static_cast<uint64_t>(op_a) * static_cast<uint64_t>(op_b)
    );

    // Apply .PSL shift if requested.
    if (psl) {
        product = product << 16;  // unsigned, wraps
    }

    // Compute the C operand based on select mode.
    uint32_t op_c;
    switch (mode) {
        case 1:  // CLO: low 16 of src_c, zero-extended
            op_c = src_c & 0xFFFFu;
            break;
        case 2:  // CHI: high 16 of src_c, zero-extended
            op_c = (src_c >> 16) & 0xFFFFu;
            break;
        case 3:  // CBCC: (src_b << 16) + src_c
            op_c = src_c + (src_b << 16);
            break;
        case 0:
        default:
            op_c = src_c;
            break;
    }

    // Add product and op_c.
    uint32_t result = product + op_c;

    // Apply .MRG if requested.
    if (mrg) {
        uint32_t low16_b = src_b & 0xFFFFu;  // raw low half, not sign-extended
        result = (low16_b << 16) | (result & 0xFFFFu);
    }

    return result;
}

#include <cassert>
#include <cstdint>

// The function declaration (must match the solution).
uint32_t bitFieldOperations(
    uint32_t src_a,
    uint32_t src_b,
    uint32_t src_c,
    int half_b,
    bool is_a_signed,
    bool is_b_signed,
    bool psl,
    bool mrg,
    int mode
);

int main() {
    // Basic multiplication of unsigned low halves, no flags.
    // src_a=3, src_b=4 in low halves, product=12, op_c=100 -> 112.
    assert(bitFieldOperations(3, 4, 100, 0, false, false, false, false, 0) == 112);

    // Use high half of src_b: src_b=0x00010004, half_b=1 -> op_b=1, product=3*1=3, +100=103.
    assert(bitFieldOperations(3, 0x00010004, 100, 1, false, false, false, false, 0) == 103);

    // Signed interpretation: src_a=0xFFFF (as signed -1), src_b=5 (low), product = -1 * 5 = -5, + 10 = 5.
    // -1 as 32-bit = 0xFFFFFFFF, times 5 = 0xFFFFFFFB, +10 = 0x00000005 (5).
    assert(bitFieldOperations(0xFFFF, 5, 10, 0, true, false, false, false, 0) == 5);

    // Signed src_b: src_a=2, src_b=0xFFFF (as -1), product=-2 + 10 = 8.
    assert(bitFieldOperations(2, 0xFFFF, 10, 0, false, true, false, false, 0) == 8);

    // PSL: product = 3*4 = 12, shifted left 16 -> 12<<16 = 0x000C0000, +0 = 0x000C0000.
    assert(bitFieldOperations(3, 4, 0, 0, false, false, true, false, 0) == (12u << 16));

    // MRG: result = 5 + 100 = 105, then replace high 16 with low16_b=0x0023 (35) -> 0x00230069 = 2293865.
    uint32_t result_mrg = bitFieldOperations(5, 0x00000023, 100, 0, false, false, false, true, 0);
    assert(result_mrg == ((0x0023u << 16) | (105u & 0xFFFFu)));  // 0x00230069

    // CLO mode: compute product=2*3=6, op_c = low16(src_c)=0x1234, result=6+0x1234=0x123A.
    assert(bitFieldOperations(2, 3, 0x00001234, 0, false, false, false, false, 1) == (0x1234u + 6));

    // CHI mode: op_c = high16(src_c)=0x5678, product=1*1=1, result=0x5678+1=0x5679.
    assert(bitFieldOperations(1, 1, 0x56780000, 0, false, false, false, false, 2) == (0x5678u + 1));

    // CBCC mode: product=10*20=200, op_c = (src_b<<16)+src_c = (20<<16)+0x0000FFFF = 0x00140000+0xFFFF=0x0014FFFF, total = 0x0014FFFF+200 = 0x001500C7.
    assert(bitFieldOperations(10, 20, 0x0000FFFF, 0, false, false, false, false, 3) == (0x0014FFFFu + 200));

    // Edge: mode=99 falls back to default.
    assert(bitFieldOperations(3, 4, 100, 0, false, false, false, false, 99) == 112);

    // Edge: half_b=1 with src_b=0x00020003, select high half -> op_b=2, product=3*2=6, +0=6.
    assert(bitFieldOperations(3, 0x00020003, 0, 1, false, false, false, false, 0) == 6);

    // Overflow product wraps: 0x10000 (low half is 0) * 2 = 0, but if signed? Here unsigned low half is 0.
    assert(bitFieldOperations(0x10000, 2, 0, 0, false, false, false, false, 0) == 0);

    // Combined PSL and MRG: product=3*4=12, psl -> 12<<16 = 0x000C0000, +0 = 0x000C0000, mrg with low16_b=0x0023 -> 0x00230000.
    uint32_t result_combined = bitFieldOperations(3, 0x00000023, 0, 0, false, false, true, true, 0);
    assert(result_combined == (0x0023u << 16));  // 0x00230000
}
