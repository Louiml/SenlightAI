Write a C++ function named `insertBits` that takes four integer parameters: two 32-bit unsigned integers `n` and `m`, and two integer indices `j` and `i` (where `0 <= j <= i < 32`). The function must insert the binary representation of `m` into `n` starting at bit position `j` and ending at bit position `i` (inclusive). Specifically, it should replace the bits of `n` from position `j` to `i` with the bits of `m` (which is guaranteed to fit entirely within that range, i.e., `m` has no bits set beyond position `i - j`). The result must be returned as an unsigned integer. Assume `n`, `m`, `j`, and `i` are all non-negative and within valid 32-bit ranges; the function should not perform any checks but must handle all valid inputs correctly.
// The core operation is to clear the bits of `n` from position `j` to `i` (inclusive) and then OR in the shifted value of `m`. Since `m` is guaranteed to fit within the range of `i - j + 1` bits, shifting `m` left by `j` positions places its bits exactly in the target range. The mask to clear those bits in `n` can be constructed as the bitwise complement of a contiguous run of 1s from bit `j` to bit `i`. A common way to build that mask is `(~0 << (i + 1)) | ((1 << j) - 1)`, which keeps everything outside the range. However, to avoid undefined behavior when shifting by 32 (when `i == 31`), we can use unsigned types and handle the mask carefully. Alternatively, since the problem guarantees valid inputs, we can compute the mask as `((1 << (i - j + 1)) - 1) << j` (a run of ones of length `i - j + 1` shifted to the correct position) and then use `n & ~mask`. Since `i - j + 1` is at most 32, but `1 << 32` is undefined on typical platforms, we must guard against that. A robust approach uses `unsigned int` and checks: if `i - j + 1 == 32`, the mask is `0xFFFFFFFF`; otherwise, compute the run. But given the constraints `0 <= j <= i < 32`, the maximum length is 32 (when `j=0, i=31`), so we handle that edge case explicitly. The operation takes constant time, using a few bitwise operations, and uses constant extra space. The main edge case is the full-width mask when all 32 bits are replaced, which requires avoiding a shift by 32. Time complexity is O(1), space complexity is O(1).
#include <cstdint>

// Insert bits of m into n from bit position j to i (inclusive).
// Assumes m fits within the range and j <= i < 32.
unsigned int insertBits(unsigned int n, unsigned int m, int j, int i) {
    // Compute length of the bit range.
    const int length = i - j + 1;
    
    // Build a mask with 1s in the range [j, i].
    unsigned int mask;
    if (length == 32) {
        mask = 0xFFFFFFFFu;  // All 32 bits set.
    } else {
        mask = ((1u << length) - 1u) << j;
    }
    
    // Clear the target bits in n, then OR in the shifted m.
    return (n & ~mask) | (m << j);
}
#include <cassert>

int main() {
    // Example from original snippet: n=1024 (0b10000000000), m=19 (0b10011), j=2, i=6
    assert(insertBits(1024u, 19u, 2, 6) == 1100u); // 1024 | (19 << 2) = 1024 + 76 = 1100

    // Inserting zeros clears the range.
    assert(insertBits(0xFFu, 0u, 0, 7) == 0u);

    // Inserting all ones fills the range.
    assert(insertBits(0u, 0xFFu, 0, 7) == 0xFFu);

    // Insertion at higher bits with smaller n.
    assert(insertBits(0u, 0b101u, 3, 5) == (0b101u << 3)); // 40

    // Inserting when n already has bits in that range – they should be replaced.
    assert(insertBits(0xFFu, 0b0011u, 2, 3) == 0xCFu); // 0b11001111

    // Edge case: full 32-bit range.
    assert(insertBits(0x12345678u, 0x9ABCDEF0u, 0, 31) == 0x9ABCDEF0u);

    // Edge case: j == i (single bit).
    assert(insertBits(0b1000u, 0b1u, 2, 2) == 0b1100u);

    // Large m shifted into n with existing bits outside range.
    assert(insertBits(0xFFFF0000u, 0x00FFu, 0, 15) == 0xFFFF00FFu);

    // No-op when m=0 and n remains unchanged.
    assert(insertBits(0xABCDEF01u, 0u, 5, 10) == 0xABCDEF01u);
}
