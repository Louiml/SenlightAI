Write a C++ function named `sbbWithBorrow` that accepts three parameters: a 32-bit unsigned integer destination `dst`, a 32-bit unsigned integer source `src`, and a boolean `borrowIn` representing the incoming carry/borrow flag. The function must compute the operation `dst - src - borrowIn` (where `borrowIn` is treated as 1 if true, 0 if false) and return a `struct` containing the 32-bit unsigned result and four boolean flags: carry (`CF`), overflow (`OF`), sign (`SF`), and zero (`ZF`). The computation must wrap modulo 2^32, and flags must be set according to x86-64 semantics: `CF` is set if the subtraction produces a borrow out of the most significant bit (i.e., the unsigned result wraps below zero), `OF` is set if the signed result overflows (i.e., the true signed mathematical result cannot be represented in a signed 32-bit integer), `SF` is set if the most significant bit of the result is 1, and `ZF` is set if the result is exactly zero. All other flags (such as parity and auxiliary carry) are not required. The function must handle edge cases like `0 - 0 - 1`, `0x80000000 - 1 - 0` (signed overflow), and `0 - 0xFFFFFFFF - 1` (both borrow and wrap) correctly.

// The core operation is `dst - src - borrow`, where `borrow` is 0 or 1. The result must be computed modulo 2^32, so we can use unsigned 32-bit arithmetic with wrapping (`uint32_t` naturally wraps on overflow). To compute flags correctly, we need to consider both unsigned and signed interpretations. 
//
// For the result: `uint32_t result = dst - src - (borrowIn ? 1 : 0);` — this naturally wraps modulo 2^32.
//
// For `CF` (borrow out): A borrow occurs when the unsigned subtraction underflows below zero. This can be detected by checking if `dst < src + borrow` (where `src + borrow` is computed as a 64-bit value to avoid overflow). Alternatively, use the fact that a borrow occurs if the mathematical full-precision difference is negative (less than zero). Since we are using unsigned 32-bit, we can detect `CF` by checking if `(uint64_t)result + (1ULL << 32) == (uint64_t)dst - (uint64_t)src - borrow`? Better: `CF = (dst < src + borrow)`. But careful: `src + borrow` may overflow 32-bit, so cast to `uint64_t`: `CF = ((uint64_t)dst < (uint64_t)src + (borrowIn ? 1ULL : 0ULL))`.
//
// For `OF` (signed overflow): This occurs when the signed mathematical result (in infinite precision) is outside the range of signed 32-bit integers (-2147483648 to 2147483647). The signed result is `(int64_t)(int32_t)dst - (int64_t)(int32_t)src - borrow`. Then check if that value is less than INT32_MIN or greater than INT32_MAX. If yes, set `OF`; otherwise clear.
//
// For `SF`: Set if bit 31 of `result` is 1, i.e., `result & 0x80000000`.
//
// For `ZF`: Set if `result == 0`.
//
// Edge cases: When `dst=0`, `src=0`, `borrow=1` → result = 0xFFFFFFFF, no borrow (0 < 1? no, 0 < 1 is true? Wait 0 < 1 is true, so CF should be set? Let's reason: 0 - 0 - 1 = -1 modulo 2^32 = 0xFFFFFFFF. Since we subtracted more than available, we have a borrow out, so CF=1. Check: `dst < src+borrow` → 0 < 1 → true → CF=1. Correct. For `dst=0x80000000, src=1, borrow=0`: unsigned result = 0x7FFFFFFF, no borrow (0x80000000 < 1? false), signed result = -2147483648 - 1 = -2147483649, which is less than INT32_MIN → OF=1. Result has SF=0 (bit 31 is 0) and ZF=0. For `dst=0, src=0xFFFFFFFF, borrow=1`: result = 0 - 4294967295 - 1 = -4294967296, modulo 2^32 = 0. Borrow occurs because `0 < 4294967296`? Actually `src+borrow = 4294967296` (2^32), and `0 < 4294967296` is true → CF=1. Signed result = -4294967296, which is less than INT32_MIN → OF=1. Result is 0 → ZF=1, SF=0.
//
// Time complexity O(1), space O(1).

#include <cstdint>
#include <limits>

struct SBBResult {
    uint32_t result;
    bool CF; // carry/borrow flag
    bool OF; // overflow flag
    bool SF; // sign flag
    bool ZF; // zero flag
};

// Perform dst - src - borrow (with borrow as 0/1) and set x86-style flags.
SBBResult sbbWithBorrow(uint32_t dst, uint32_t src, bool borrowIn) {
    uint32_t result = dst - src - (borrowIn ? 1u : 0u);

    // Carry flag: set if unsigned subtraction produced a borrow out of the MSB.
    bool CF = ((uint64_t)dst < (uint64_t)src + (borrowIn ? 1ULL : 0ULL));

    // Overflow flag: set if the signed mathematical result is outside of int32 range.
    int64_t signedResult = (int64_t)(int32_t)dst - (int64_t)(int32_t)src - (borrowIn ? 1LL : 0LL);
    bool OF = (signedResult < std::numeric_limits<int32_t>::min() ||
               signedResult > std::numeric_limits<int32_t>::max());

    bool SF = (result & 0x80000000u) != 0;
    bool ZF = (result == 0);

    return SBBResult{result, CF, OF, SF, ZF};
}

#include <cassert>

int main() {
    // Basic: 10 - 3 - 0 = 7, no flags.
    SBBResult r1 = sbbWithBorrow(10, 3, false);
    assert(r1.result == 7);
    assert(r1.CF == false);
    assert(r1.OF == false);
    assert(r1.SF == false);
    assert(r1.ZF == false);

    // Borrow in: 10 - 3 - 1 = 6.
    SBBResult r2 = sbbWithBorrow(10, 3, true);
    assert(r2.result == 6);
    assert(r2.CF == false);
    assert(r2.OF == false);
    assert(r2.SF == false);
    assert(r2.ZF == false);

    // Borrow out: 0 - 0 - 1 = 0xFFFFFFFF, CF set.
    SBBResult r3 = sbbWithBorrow(0, 0, true);
    assert(r3.result == 0xFFFFFFFFu);
    assert(r3.CF == true);
    assert(r3.OF == false); // signed result is -1, in range
    assert(r3.SF == true);  // MSB set
    assert(r3.ZF == false);

    // Signed overflow: 0x80000000 - 1 - 0 = 0x7FFFFFFF, but signed result = -2147483649 → OF.
    SBBResult r4 = sbbWithBorrow(0x80000000u, 1, false);
    assert(r4.result == 0x7FFFFFFFu);
    assert(r4.CF == false); // 0x80000000 < 1? false
    assert(r4.OF == true);
    assert(r4.SF == false);
    assert(r4.ZF == false);

    // Zero result with borrow: 0 - 0xFFFFFFFF - 1 = 0, both CF and OF set.
    SBBResult r5 = sbbWithBorrow(0, 0xFFFFFFFFu, true);
    assert(r5.result == 0);
    assert(r5.CF == true);
    assert(r5.OF == true); // signed result = -4294967296, out of range
    assert(r5.SF == false);
    assert(r5.ZF == true);

    // Zero result without borrow: 5 - 5 - 0.
    SBBResult r6 = sbbWithBorrow(5, 5, false);
    assert(r6.result == 0);
    assert(r6.CF == false);
    assert(r6.OF == false);
    assert(r6.SF == false);
    assert(r6.ZF == true);

    // Negative result in signed sense: 0 - 5 - 0 = 0xFFFFFFFB, CF set, OF false.
    SBBResult r7 = sbbWithBorrow(0, 5, false);
    assert(r7.result == 0xFFFFFFFBu);
    assert(r7.CF == true);
    assert(r7.OF == false); // signed result -5, in range
    assert(r7.SF == true);
    assert(r7.ZF == false);

    // Extremes: 0xFFFFFFFF - 0xFFFFFFFF - 0 = 0, CF false.
    SBBResult r8 = sbbWithBorrow(0xFFFFFFFFu, 0xFFFFFFFFu, false);
    assert(r8.result == 0);
    assert(r8.CF == false);
    assert(r8.OF == false);
    assert(r8.SF == false);
    assert(r8.ZF == true);

    // 0xFFFFFFFF - 0 - 1 = 0xFFFFFFFE, no borrow (since 0xFFFFFFFF >= 1).
    SBBResult r9 = sbbWithBorrow(0xFFFFFFFFu, 0, true);
    assert(r9.result == 0xFFFFFFFEu);
    assert(r9.CF == false);
    assert(r9.OF == false); // signed result = -2
    assert(r9.SF == true);
    assert(r9.ZF == false);

    // Signed overflow on the other side: 0x7FFFFFFF - (-1) - 0, but src is unsigned 0xFFFFFFFF (interpreted as -1).
    SBBResult r10 = sbbWithBorrow(0x7FFFFFFFu, 0xFFFFFFFFu, false);
    // result = 0x7FFFFFFF - 0xFFFFFFFF = 0x80000000 (wrapped), signed result = 2147483647 - (-1) = 2147483648 → OF.
    assert(r10.result == 0x80000000u);
    assert(r10.CF == true); // 0x7FFFFFFF < 0xFFFFFFFF
    assert(r10.OF == true); // 2147483648 > INT32_MAX
    assert(r10.SF == true);
    assert(r10.ZF == false);

    return 0;
}
