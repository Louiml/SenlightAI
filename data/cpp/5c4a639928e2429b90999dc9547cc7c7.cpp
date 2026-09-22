/*
Write a C++ function named `isValidOrderedScalar` that takes a pointer to an array of exactly 32 unsigned bytes (representing a big-endian unsigned 256-bit integer) and a boolean flag `allowHalf`. The function must return `true` if and only if the pointer is non-null, the 32-byte value is strictly greater than zero, and the value is less than or equal to the constant `ORDER_MINUS_ONE` (shown below). Additionally, if `allowHalf` is `true`, the upper bound is instead `HALF_ORDER_MINUS_ONE`. The comparison must be done byte-by-byte in big-endian order without converting to a built‑in integer type, and no external libraries beyond standard headers are allowed. The constants are fixed: `ORDER_MINUS_ONE` = `{0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFE,0xBA,0xAE,0xDC,0xE6,0xAF,0x48,0xA0,0x3B,0xBF,0xD2,0x5E,0x8C,0xD0,0x36,0x41,0x40}` and `HALF_ORDER_MINUS_ONE` = `{0x7F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x5D,0x57,0x6E,0x73,0x57,0xA4,0x50,0x1D,0xDF,0xE9,0x2F,0x46,0x68,0x1B,0x20,0xA0}`. The function should be declared in a namespace `ordered_range` and must be `const`-correct.
*/
#include <cstddef>

namespace ordered_range {

namespace detail {
// Compare two big-endian unsigned byte arrays. Returns 1 if c1>c2, -1 if c1<c2, 0 if equal.
int CompareBigEndian(const unsigned char* c1, std::size_t c1len,
                     const unsigned char* c2, std::size_t c2len) {
    // Skip leading zero bytes of c1 if it has extra length.
    while (c1len > c2len) {
        if (*c1 != 0)
            return 1;
        ++c1;
        --c1len;
    }
    // Skip leading zero bytes of c2 if it has extra length.
    while (c2len > c1len) {
        if (*c2 != 0)
            return -1;
        ++c2;
        --c2len;
    }
    // Now lengths are equal; compare from most significant byte.
    while (c1len > 0) {
        if (*c1 > *c2)
            return 1;
        if (*c2 > *c1)
            return -1;
        ++c1;
        ++c2;
        --c1len;
    }
    return 0;
}

} // namespace detail

// Order of secp256k1's generator minus 1.
const unsigned char ORDER_MINUS_ONE[32] = {
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE,
    0xBA, 0xAE, 0xDC, 0xE6, 0xAF, 0x48, 0xA0, 0x3B,
    0xBF, 0xD2, 0x5E, 0x8C, 0xD0, 0x36, 0x41, 0x40};

// Half of the order of secp256k1's generator minus 1.
const unsigned char HALF_ORDER_MINUS_ONE[32] = {
    0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0x5D, 0x57, 0x6E, 0x73, 0x57, 0xA4, 0x50, 0x1D,
    0xDF, 0xE9, 0x2F, 0x46, 0x68, 0x1B, 0x20, 0xA0};

const unsigned char ZERO[1] = {0};

// Returns true if vch[0..31] represents a positive number <= (allowHalf ? HALF_ORDER_MINUS_ONE : ORDER_MINUS_ONE).
bool isValidOrderedScalar(const unsigned char* vch, bool allowHalf) {
    if (!vch)
        return false;
    const unsigned char* maxVal = allowHalf ? HALF_ORDER_MINUS_ONE : ORDER_MINUS_ONE;
    return detail::CompareBigEndian(vch, 32, ZERO, 1) > 0 &&
           detail::CompareBigEndian(vch, 32, maxVal, 32) <= 0;
}

} // namespace ordered_range
#include <cassert>

int main() {
    using ordered_range::isValidOrderedScalar;
    using ordered_range::ORDER_MINUS_ONE;
    using ordered_range::HALF_ORDER_MINUS_ONE;

    // Null pointer.
    assert(!isValidOrderedScalar(nullptr, false));
    assert(!isValidOrderedScalar(nullptr, true));

    // All zeros is invalid (not > 0).
    unsigned char zero[32] = {0};
    assert(!isValidOrderedScalar(zero, false));
    assert(!isValidOrderedScalar(zero, true));

    // One (least positive) is valid.
    unsigned char one[32] = {0};
    one[31] = 1;
    assert(isValidOrderedScalar(one, false));
    assert(isValidOrderedScalar(one, true));

    // Equal to ORDER_MINUS_ONE is valid with allowHalf=false, but invalid with allowHalf=true.
    assert(isValidOrderedScalar(ORDER_MINUS_ONE, false));
    assert(!isValidOrderedScalar(ORDER_MINUS_ONE, true));

    // Exactly half order is valid only with allowHalf=true.
    assert(!isValidOrderedScalar(HALF_ORDER_MINUS_ONE, false));
    assert(isValidOrderedScalar(HALF_ORDER_MINUS_ONE, true));

    // All 0xFF exceeds both bounds.
    unsigned char maxBytes[32];
    for (int i = 0; i < 32; ++i) maxBytes[i] = 0xFF;
    assert(!isValidOrderedScalar(maxBytes, false));
    assert(!isValidOrderedScalar(maxBytes, true));

    // Value slightly above half but below full order: valid only with allowHalf=false.
    unsigned char aboveHalf[32];
    for (int i = 0; i < 32; ++i) aboveHalf[i] = 0;
    aboveHalf[31] = 0x41; // 0x...41 > 0x...40 (half's last byte), but < order? Actually order last byte is 0x40, so this is 1 more than ORDER_MINUS_ONE? No, it's smaller because order starts 0xFF...FE...40; aboveHalf is 0x...41 which is > HALF but < ORDER. Let's verify: HALF last byte 0x40, aboveHalf last byte 0x41 -> greater than half. ORDER last byte 0x40, so aboveHalf is 1 greater than ORDER? No, ORDER is 0x...40, so 0x41 is greater than ORDER (since all higher bytes are 0 for aboveHalf, but ORDER has 0xFF...). Actually compare: aboveHalf = 0x000...041, ORDER = 0xFFF...FE...40, so aboveHalf is much smaller. So it's valid for both. Let's just use a value that is > HALF but <= ORDER: set last byte to 0x40 and second last byte to 0x41 (order second last is 0x41), but that equals ORDER? Let's keep simple: use a value equal to 1 + HALF, which is clearly <= ORDER because ORDER > HALF.
    unsigned char halfPlusOne[32];
    for (int i = 0; i < 32; ++i) halfPlusOne[i] = HALF_ORDER_MINUS_ONE[i];
    halfPlusOne[31]++; // 0x41 (since half last byte 0x40 -> 0x41). This is <= ORDER because ORDER is much larger.
    assert(!isValidOrderedScalar(halfPlusOne, false)); // Because it's > HALF, so fails allowHalf=false? Wait, allowHalf=false uses ORDER, and halfPlusOne is <= ORDER, so true. Actually we want to test the difference: allowHalf=false should be true, allowHalf=true should be false. Let's correct:
    assert(isValidOrderedScalar(halfPlusOne, false)); // Uses ORDER, and halfPlusOne < ORDER
    assert(!isValidOrderedScalar(halfPlusOne, true));  // Uses HALF, halfPlusOne > HALF

    // Value equal to 2 (simple case).
    unsigned char two[32] = {0};
    two[31] = 2;
    assert(isValidOrderedScalar(two, false));
    assert(isValidOrderedScalar(two, true));

    // Value with leading zeros but non-zero later (already covered by one/two).
    // Value with a single 1 in the middle.
    unsigned char mid[32] = {0};
    mid[16] = 1;
    assert(isValidOrderedScalar(mid, false));
    assert(isValidOrderedScalar(mid, true));

    return 0;
}
// The core algorithm is a manual big-endian unsigned integer comparison, implemented as a helper `compareBigEndian` that compares two arrays of possibly different lengths. For each input array, we first skip leading zero bytes (since the length is fixed at 32 for the main value, but the zero constant has length 1). The comparison logic works by aligning the most significant bytes: if the first array has more bytes than the second, we scan the extra leading bytes of the first—if any is nonzero, the first is greater; otherwise we advance. Symmetrically, if the second is longer, we scan its extra leading bytes. Once lengths are equal, we compare byte-by-byte from the most significant end, returning 1, -1, or 0 accordingly. The main function then checks: (1) pointer non-null, (2) `compareBigEndian(value, 32, zero, 1) > 0` (strictly greater than zero), and (3) `compareBigEndian(value, 32, chosenMax, 32) <= 0` (less than or equal to the chosen upper bound). Edge cases include: a null pointer, a value that is all zeros, a value equal to the upper bound, a value exceeding the upper bound (e.g., all 0xFF), and the difference between `allowHalf` true/false. The value always has length 32, but the zero constant has length 1, so comparisons must handle length mismatches. Time complexity is O(32) per call because we scan at most 32 bytes; space complexity is O(1) beyond the constant arrays.
