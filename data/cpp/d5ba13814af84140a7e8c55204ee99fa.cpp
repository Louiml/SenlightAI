Write a standalone C++ function named `positionOfLowestSetBit` that takes an integer `n` (which may be positive, zero, or negative) and returns the 1-based position of the first (least significant) set bit (i.e., the rightmost `1`) in its binary representation. If `n` equals 0 (which has no set bits), the function should return 0. The position counting starts from the least significant bit (which is position 1). For example, for `n = 12` (binary `1100`), the rightmost `1` is at position 3 (since `12 = 8+4`, the least significant set bit is at bit index 2, so 1‑based position 3). The function must handle negative numbers correctly (in two's complement representation, `n & -n` isolates the lowest set bit, even for negative values) and must not rely on floating-point operations like `log2`; instead, use bitwise operations and a loop or a built-in function. Provide only the function definition (free function, no `main`) with appropriate `const` correctness for parameters if possible.

// The key insight is that in two's complement, `n & -n` isolates the lowest set bit of `n` as a power‑of‑two value (e.g., for `n=12` (1100), `n & -n` gives 4 (0100)). To convert that value to the 1‑based position, we need to count how many bits are to the right of that isolated bit. This can be done by repeatedly shifting the isolated bit right until it becomes 1, counting steps. Alternatively, we can use a loop that counts trailing zeros by checking `(n & 1)` and shifting `n` right, but that would lose negativity handling. The cleanest method: use `unsigned int` to avoid sign-extension issues, then compute `lowBit = n & -n` (works for negative if we cast appropriately) or use `unsigned` arithmetic. For a negative number like `-12` (two's complement: ...11110100), `-12 & 12`? Actually `n & -n` works: `-12` in two's complement is `...11110100`, AND with `12` (00001100) gives `00000100` = 4, so the lowest set bit is at position 3 (correct, since binary of -12 has lowest 1 at bit 2). Edge cases: `n=0` yields `0 & -0 = 0`, we must return 0. For `n` as `INT_MIN`, `n & -n` is `INT_MIN` (which is high bit), but position is number of bits (e.g., 32 on 32-bit int). A loop from position=1, shifting `lowBit` right until it becomes 0, works. Time complexity O(number of bits) ≤ O(32) for typical int, and O(1) space.

#include <climits> // For CHAR_BIT, but not strictly needed if we use fixed 32-bit assumption

// Returns 1-based position of the lowest set bit in n, or 0 if n == 0.
// Works for negative numbers using two's complement representation.
int positionOfLowestSetBit(int n) {
    if (n == 0) {
        return 0;
    }

    // Isolate the lowest set bit using bitwise AND with two's complement.
    // Cast to unsigned to avoid implementation-defined behavior for negative n.
    unsigned int lowBit = static_cast<unsigned int>(n) & static_cast<unsigned int>(-n);

    int position = 1;
    // Shift right until the isolated bit becomes 0; count steps.
    while ((lowBit & 1) == 0) {
        lowBit >>= 1;
        ++position;
    }
    return position;
}

#include <cassert>

int main() {
    // Basic positive numbers
    assert(positionOfLowestSetBit(1) == 1);   // 0001 -> bit 1
    assert(positionOfLowestSetBit(2) == 2);   // 0010 -> bit 2
    assert(positionOfLowestSetBit(4) == 3);   // 0100 -> bit 3
    assert(positionOfLowestSetBit(12) == 3);  // 1100 -> bit 3
    assert(positionOfLowestSetBit(48) == 5);  // 110000 -> bit 5

    // Zero
    assert(positionOfLowestSetBit(0) == 0);

    // Negative numbers (two's complement)
    assert(positionOfLowestSetBit(-1) == 1);   // ...1111 -> bit 1
    assert(positionOfLowestSetBit(-2) == 2);   // ...1110 -> bit 2
    assert(positionOfLowestSetBit(-12) == 3);  // ...1100 -> bit 3 (as analyzed)

    // Large positive
    assert(positionOfLowestSetBit(1024) == 11); // 2^10 -> bit 11

    // Max int (all bits set except sign) -> lowest bit is 1
    assert(positionOfLowestSetBit(INT_MAX) == 1);

    return 0;
}
