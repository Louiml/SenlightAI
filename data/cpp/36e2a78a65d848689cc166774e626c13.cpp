Given an IEEE 754 single-precision floating-point value represented as a `float`, write a C++ function that converts it to a decimal string in scientific-like fixed-point notation without using any standard library floating-point formatting functions (such as `std::to_string`, `std::ostringstream`, or `printf` family). The function must manually extract the sign, exponent, and mantissa using bit manipulation, reconstruct the value as an integer significand, and produce a string that contains all significant digits (no leading zeros) and correctly handles rounding to the nearest digit. The output string must begin with a flag byte (as an `unsigned char`) that encodes the numeric classification (zero, infinity, NaN, negative, carry) using the following bitmask constants: `FTOA_ZERO=1`, `FTOA_INF=2`, `FTOA_NAN=4`, `FTOA_MINUS=8`, `FTOA_CARRY=16`; if none apply the byte is 0. The flag byte is followed by the decimal digits (each as a `char` in `'0'`..`'9'`) with no decimal point. Additionally, return an `int16_t` representing the base‑10 exponent (power of ten) for the first digit. The function signature is: `int16_t float_to_manual_string(float value, unsigned char* outBuffer, uint8_t precisionDigits)` where `precisionDigits` is the number of digits to produce (1 to 7, inclusive), and `outBuffer` must have at least `precisionDigits+1` bytes. The rounding rules follow standard decimal rounding: if the next digit is 5 or greater, round up; if rounding causes a carry that produces a new leading digit (e.g., 999 → 1000), set the `FTOA_CARRY` flag and increment the exponent.

##
// The core challenge is converting a binary floating-point number to decimal without using standard library conversion routines. The approach uses a known technique: represent the value as `(-1)^sign * F * 2^(E-127)` where `F` is the 24‑bit significand (implicit leading 1 for normalized numbers) and `E` is the biased exponent. By decomposing the exponent into a multiple of 8 and a remainder, we can precompute two lookup tables: one for powers of 10 and one for scaling factors. Specifically, `2^b ≈ f * r * 10^e` where `b = E-127`, `r = 2^(b mod 8)`, and the tables provide `f` and `e`. To avoid floating-point multiplication, we multiply the significand (as a 64‑bit integer) by the factor and shift right by `(15 - (b mod 8))` to obtain an integer that approximates the value in units of 10^e. The decimal digits are then extracted by repeated division by 10. Edge cases include: zero (all exponent and mantissa bits zero) outputs "0" and exponent 0; infinity outputs `FTOA_INF` plus "0" digits; NaN outputs `FTOA_NAN`; subnormal numbers (exponent=0 but mantissa≠0) have no implicit leading 1; negative numbers have the `FTOA_MINUS` flag; rounding may produce a carry that increments the exponent and sets `FTOA_CARRY`. Time complexity is O(precisionDigits) since we extract at most 7 digits, and space is O(1) beyond the output buffer.
//
// ##
#include <cstdint>
#include <cstddef>

// Bitmask flags for the first output byte
enum : unsigned char {
    FTOA_ZERO  = 1,
    FTOA_INF   = 2,
    FTOA_NAN   = 4,
    FTOA_MINUS = 8,
    FTOA_CARRY = 16
};

// Precomputed tables: 2^(8*i) approximated as factor * 10^exponent
static const int8_t exp10Table[32] = {
    -36, -33, -31, -29, -26, -24, -21, -19,
    -17, -14, -12, -9,  -7,  -4,  -2,   0,
      3,   5,   8,  10,  12,  15,  17,  20,
     22,  24,  27,  29,  32,  34,  36,  39
};

static const uint32_t factorTable[32] = {
    2295887404UL, 587747175UL, 1504632769UL, 3851859889UL,
    986076132UL,  2524354897UL, 646234854UL,  1654361225UL,
    4235164736UL, 1084202172UL, 2775557562UL, 710542736UL,
    1818989404UL, 465661287UL,  1192092896UL, 3051757813UL,
    781250000UL,  2000000000UL, 512000000UL,  1310720000UL,
    3355443200UL, 858993459UL,  2199023256UL, 562949953UL,
    1441151881UL, 3689348815UL, 944473297UL,  2417851639UL,
    618970020UL,  1584563250UL, 4056481921UL, 1038459372UL
};

// Convert a float to decimal digits without standard formatting.
// outBuffer receives: [0]=flags, [1..precisionDigits]=digits ('0'-'9').
// Returns base-10 exponent for the first digit.
int16_t float_to_manual_string(float value, unsigned char* outBuffer, uint8_t precisionDigits) {
    if (precisionDigits > 7) precisionDigits = 7;

    // Reinterpret the float as a 32-bit unsigned integer.
    uint32_t raw;
    __builtin_memcpy(&raw, &value, sizeof(value));

    unsigned char flags = 0;
    uint8_t expBits = (raw >> 23) & 0xFF;
    uint32_t frac = raw & 0x007FFFFFUL;

    // Sign bit.
    if (raw & (1UL << 31)) flags |= FTOA_MINUS;

    // Handle special cases: zero, infinity, NaN.
    if (expBits == 0 && frac == 0) {
        outBuffer[0] = flags | FTOA_ZERO;
        for (uint8_t i = 0; i <= precisionDigits; ++i) outBuffer[i+1] = '0';
        return 0;
    }
    if (expBits == 0xFF) {
        if (frac == 0) flags |= FTOA_INF;
        else           flags |= FTOA_NAN;
        outBuffer[0] = flags | FTOA_ZERO;  // Digits are zeros after flag.
        for (uint8_t i = 0; i <= precisionDigits; ++i) outBuffer[i+1] = '0';
        return 0;
    }

    // Normal number: add implicit leading 1.
    if (expBits != 0) frac |= (1UL << 23);
    else              flags |= FTOA_ZERO; // subnormal: no leading 1, still finite.

    // Decompose exponent into multiple of 8 and remainder.
    int16_t exponent = static_cast<int16_t>(expBits) - 127;
    uint8_t idx = exponent >> 3;
    int8_t rem = exponent & 7;

    // Use lookup table to get approximate scaling.
    int8_t exp10 = exp10Table[idx];
    uint32_t factor = factorTable[idx];

    // Multiply significand by factor, then shift to align.
    int64_t product = static_cast<int64_t>(frac) * static_cast<int64_t>(factor);
    product >>= (15 - rem);

    // Extract decimal digits.
    uint8_t numDigits = 0;
    bool foundNonzero = false;
    int16_t resultExp = exp10;
    int64_t decimal = 100000000000000LL; // 10^14

    while (numDigits < precisionDigits) {
        int digit = 0;
        while (product >= decimal) {
            product -= decimal;
            ++digit;
        }
        decimal /= 10;

        if (!foundNonzero && digit == 0) {
            --resultExp;  // Skip leading zeros, adjust exponent.
            continue;
        }
        foundNonzero = true;

        if (digit > 9) {
            // Rounding overflow: all digits become 9.
            for (uint8_t i = 0; i <= numDigits; ++i) outBuffer[i+1] = '9';
            goto round_up;
        }
        outBuffer[++numDigits] = static_cast<unsigned char>('0' + digit);
    }

    // Rounding: check the next digit.
    decimal *= 10;
    if (product >= (decimal >> 1)) {
        goto round_up;
    }
    outBuffer[0] = flags;
    return resultExp;

round_up:
    // Round up the last digit chain.
    while (numDigits > 0) {
        if (outBuffer[numDigits] == '9') {
            outBuffer[numDigits] = '0';
            --numDigits;
        } else {
            ++outBuffer[numDigits];
            outBuffer[0] = flags;
            return resultExp;
        }
    }
    // We got a carry to a new leading digit.
    outBuffer[1] = '1';
    flags |= FTOA_CARRY;
    outBuffer[0] = flags;
    return resultExp + 1;
}

##
#include <cassert>
#include <cstring>

int main() {
    unsigned char buf[8];

    // Zero.
    assert(float_to_manual_string(0.0f, buf, 7) == 0);
    assert(buf[0] & FTOA_ZERO);
    assert(buf[1] == '0' && buf[2] == '0');

    // Negative zero.
    assert(float_to_manual_string(-0.0f, buf, 7) == 0);
    assert((buf[0] & FTOA_MINUS) && (buf[0] & FTOA_ZERO));

    // Simple integer 1.0 -> digits "1000000", exponent 0.
    assert(float_to_manual_string(1.0f, buf, 7) == 0);
    assert(buf[0] == 0);
    assert(strncmp((char*)buf+1, "1000000", 7) == 0);

    // 2.5 -> digits "2500000", exponent 0.
    assert(float_to_manual_string(2.5f, buf, 7) == 0);
    assert(strncmp((char*)buf+1, "2500000", 7) == 0);

    // 123.456 with 6 digits -> "123456", exponent 2.
    assert(float_to_manual_string(123.456f, buf, 6) == 2);
    assert(strncmp((char*)buf+1, "123456", 6) == 0);

    // 0.0012345 with 5 digits -> "12345", exponent -2.
    assert(float_to_manual_string(0.0012345f, buf, 5) == -2);
    assert(strncmp((char*)buf+1, "12345", 5) == 0);

    // Negative value.
    assert(float_to_manual_string(-3.14f, buf, 3) == 0);
    assert(buf[0] & FTOA_MINUS);
    assert(strncmp((char*)buf+1, "314", 3) == 0);

    // Rounding up: 9.999 with 3 digits -> "100", exponent 1, carry flag.
    assert(float_to_manual_string(9.999f, buf, 3) == 1);
    assert(buf[0] & FTOA_CARRY);
    assert(strncmp((char*)buf+1, "100", 3) == 0);

    // Infinity.
    float inf = 1.0f / 0.0f;
    assert(float_to_manual_string(inf, buf, 3) == 0);
    assert((buf[0] & FTOA_INF) && (buf[0] & FTOA_ZERO));

    // NaN.
    float nan = 0.0f / 0.0f;
    assert(float_to_manual_string(nan, buf, 3) == 0);
    assert((buf[0] & FTOA_NAN) && (buf[0] & FTOA_ZERO));

    return 0;
}
