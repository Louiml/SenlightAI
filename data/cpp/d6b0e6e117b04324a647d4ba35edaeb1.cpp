/*
Write a C++ function `int16_t normalize_amr_wb(int32_t x)` that, given a non-zero 32-bit signed integer, returns the number of leading zeros in its 32-bit two's complement representation (i.e., the smallest number of left shifts required to move the most significant set bit into bit 30, so that the value is in the range `0x40000000` to `0x7FFFFFFF`). The function must handle both positive and negative inputs correctly; for negative numbers, the sign bit is set, so the result should be 0 (since the MSB is already at bit 31). The algorithm should use a branch-and-binary-search approach to approximate the count quickly, then refine it by examining the shifted value’s upper bits to make the final adjustment. The function must be robust for all non-zero `int32_t` values, including edge cases like `INT32_MIN`, `INT32_MAX`, `1`, and `-1`.
*/
#include <cstdint>

// Returns the number of leading zeros in the 32-bit two's complement representation.
// Precondition: x != 0.
// For negative x, returns 0 (sign bit is set).
int16_t normalize_amr_wb(int32_t x) {
    if (x < 0) {
        return 0;  // sign bit set, so no leading zeros
    }

    int16_t i;

    if (x > 0x0FFFFFFF) {
        i = 0;  // most likely case for large positive
    } else if (x > 0x00FFFFFF) {
        i = 3;  // second most likely
    } else if (x > 0x0000FFFF) {
        i = (x > 0x000FFFFF) ? 7 : 11;
    } else {
        if (x > 0x000000FF) {
            i = (x > 0x00000FFF) ? 15 : 19;
        } else {
            i = (x > 0x0000000F) ? 23 : 27;
        }
    }

    x <<= i;  // x is positive and left shift is safe

    switch (x & 0x78000000) {
        case 0x08000000:
            i += 3;
            break;
        case 0x18000000:
        case 0x10000000:
            i += 2;
            break;
        case 0x28000000:
        case 0x20000000:
        case 0x38000000:
        case 0x30000000:
            i++;
            break;
        default:
            // already correct
            break;
    }

    return i;
}
#include <cassert>
#include <cstdint>

int16_t normalize_amr_wb(int32_t x);  // forward declaration

int main() {
    // Positive edge cases
    assert(normalize_amr_wb(1) == 30);                    // 0x00000001
    assert(normalize_amr_wb(2) == 29);                    // 0x00000002
    assert(normalize_amr_wb(0x0FFFFFFF) == 3);            // 268435455
    assert(normalize_amr_wb(0x10000000) == 3);            // 268435456
    assert(normalize_amr_wb(0x7FFFFFFF) == 0);            // INT32_MAX
    assert(normalize_amr_wb(0x40000000) == 0);            // 2^30
    assert(normalize_amr_wb(0x3FFFFFFF) == 1);            // one leading zero
    assert(normalize_amr_wb(0x20000000) == 1);            // 2^29
    assert(normalize_amr_wb(0x1FFFFFFF) == 2);
    assert(normalize_amr_wb(0x08000000) == 4);            // 2^27

    // Negative inputs (must return 0)
    assert(normalize_amr_wb(-1) == 0);
    assert(normalize_amr_wb(-2147483647) == 0);           // INT32_MIN + 1
    assert(normalize_amr_wb(-2147483648LL) == 0);         // INT32_MIN

    // More positive values
    assert(normalize_amr_wb(0x0000FFFF) == 15);           // 65535
    assert(normalize_amr_wb(0x00010000) == 15);           // 65536
    assert(normalize_amr_wb(0x000000FF) == 23);           // 255
    assert(normalize_amr_wb(0x00000100) == 23);           // 256
    assert(normalize_amr_wb(0x0000000F) == 27);           // 15
    assert(normalize_amr_wb(0x00000010) == 27);           // 16

    return 0;
}
// The problem reduces to finding the bit-length of the absolute value’s binary representation, but with a twist: we need to count leading zeros in the 32-bit signed representation. For positive numbers, leading zeros are `31 - floor(log2(x))`. For negative numbers, the sign bit is set, so there are zero leading zeros. The given snippet uses a clever two-stage approach: first, it narrows down the count using a decision tree based on the value’s magnitude, shifting left by a rough estimate. After shifting, it examines bits 27-30 (mask `0x78000000`) to determine if the first set bit is now in the top four bits; if not, it adds a small correction. This works because after the initial shift, the leading bit is guaranteed to be in the top 4 bits (indices 27-30). The switch statement checks whether the top nibble has the highest bit at position 30 (case `0x08000000` → add 3), position 29 (cases `0x18000000`, `0x10000000` → add 2), position 28 (cases `0x28000000`, `0x20000000`, `0x38000000`, `0x30000000` → add 1), or position 27 (default → add 0). The initial decision tree handles all positive values correctly, while negative values skip the first branch (since `x > 0x0FFFFFFF` is false for negatives) and fall through to the else cases, but then the switch on the shifted value will still work because the shift amount computed from the negative value’s magnitude is not based on the sign bit; however, in the original code, negative inputs are not explicitly handled, and the logic would fail for them (e.g., `-1` would be treated as a large positive number? Actually, `-1` as `int32` is `0xFFFFFFFF`, which is > `0x0FFFFFFF`, so `i=0`, then `x <<= 0` leaves `x = 0xFFFFFFFF`, and `x & 0x78000000` is `0x78000000`, which falls into the default case, giving `i=0` – that is correct because `-1` has 0 leading zeros. But for other negatives like `-2` (`0xFFFFFFFE`), `x > 0x0FFFFFFF` is true, so `i=0`, then `x & 0x78000000` is `0x78000000`? Actually `0xFFFFFFFE & 0x78000000 = 0x78000000`, default → `i=0`, correct. For `-2147483648` (`0x80000000`), `x > 0x0FFFFFFF` is true, `i=0`, then `x & 0x78000000` is `0x00000000` (since `0x80000000 & 0x78000000 = 0x00000000`), default → `i=0`, correct. So the original code actually handles negatives correctly because the sign bit makes the value large positive in unsigned comparison? But here `int32` is signed, `x > 0x0FFFFFFF` with `x` negative is false because negative numbers are less than positive constants. Wait, but `x` is `int32` signed, so `-1` is `0xFFFFFFFF` but as a signed integer, it is `-1`, which is not > `0x0FFFFFFF` (which is `268435455`). So the first branch is false. Then it goes to `x > 0x00FFFFFF`? For `-1`, this is false. Then `x > 0x0000FFFF`? false. Then `else` branch: `x > 0x000000FF`? false, then `x > 0x0000000F`? false, so `i = 27`. Then `x <<= 27` on `-1` (as `int32`, shifting negative left is undefined behavior in C++? Actually, left shift of negative values is undefined behavior in C++11 and later. So the original code is unsafe for negatives. Therefore, we need to handle negatives explicitly. The task likely expects the same mathematical result: for negative numbers, return 0 (since leading zeros in signed 32-bit representation is 0 for any negative number because the sign bit is 1). For positive numbers, the function should return the number of leading zeros, which is `31 - floor(log2(x))`. The provided snippet’s algorithm is optimized for positive numbers; we can adapt it by first checking if `x < 0` and returning 0, else apply the logic. The time complexity is O(1) with a constant number of comparisons and a switch; space is O(1). The edge cases include `x = 1` (leading zeros = 30), `x = 0x0FFFFFFF` (leading zeros = 3), `x = 0x10000000` (leading zeros = 3? Actually `0x10000000` is 2^28, so leading zeros = 3), and `x = INT32_MAX` (leading zeros = 0).
