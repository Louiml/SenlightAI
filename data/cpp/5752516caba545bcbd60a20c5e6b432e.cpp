// Write a standalone C++ function `bool isEvenParity(uint16_t value)` that determines whether the number of set bits (i.e., the popcount) in a 16-bit unsigned integer is even. The function should return `true` if the popcount is even and `false` if odd. This task isolates the parity-checking logic from the provided 8086 simulator's flag computation, requiring you to implement a bit-counting algorithm without using built-in popcount functions. The input is a single 16-bit unsigned integer, and the output is a boolean. Handle all possible 16-bit values, including 0 (which has zero set bits, an even count, so return `true`).

// The solution needs to count the number of 1-bits in a 16-bit unsigned integer and check if that count is even. A straightforward approach is to loop through all 16 bits, checking each bit with a mask and counting. However, a more efficient method uses the classic "divide and conquer" bit-counting technique: repeatedly add adjacent bit groups to accumulate the total popcount. The algorithm:
// 1. Start with the value as-is.
// 2. Use masks to isolate pairs of bits, compute sums of these pairs.
// 3. Then compute sums of 4-bit groups, 8-bit groups, and finally the full 16-bit count.
// 4. This yields the total number of set bits in a small constant number of operations (O(1) time).
// 5. Finally, check if the total count is even by taking modulo 2 or checking the least significant bit.
//
// Edge cases: 
// - Input of 0 → popcount 0 → even → return `true`.
// - All bits set (0xFFFF) → popcount 16 → even → `true`.
// - Single bit set (e.g., 0x0001) → popcount 1 → odd → `false`.
//
// The bit-counting method is O(1) time and O(1) auxiliary space, though a simple loop would be O(16) time which is also effectively constant for a fixed-width integer. For clarity and self-containedness, a simple loop over the 16 bits with a mask is acceptable and easier to verify, but the optimized version demonstrates deeper understanding. I will implement the optimized version with a mask-based reduction for efficiency, but also include a clear comment about the approach. The complexity is O(1) in both time and space for a 16-bit input.

#include <cstdint>

// Returns true if the number of set bits in a 16-bit unsigned integer is even.
bool isEvenParity(uint16_t value) {
    // Step 1: Count bits in 2-bit groups.
    // 0x5555 = 0101 0101 ... (pairs)
    value = value - ((value >> 1) & 0x5555);
    
    // Step 2: Sum counts in 4-bit groups.
    value = (value & 0x3333) + ((value >> 2) & 0x3333);
    
    // Step 3: Sum counts in 8-bit groups.
    value = (value + (value >> 4)) & 0x0F0F;
    
    // Step 4: Sum counts in 16-bit group.
    value = (value + (value >> 8)) & 0x00FF;
    
    // Value is now the popcount (0 to 16).
    // Return true if popcount is even.
    return (value % 2) == 0;
}

#include <cassert>
#include <cstdint>

bool isEvenParity(uint16_t value);

int main() {
    // Zero has zero set bits (even) → true
    assert(isEvenParity(0x0000) == true);
    // One set bit → odd → false
    assert(isEvenParity(0x0001) == false);
    // Two set bits → even → true
    assert(isEvenParity(0x0003) == true);
    // Three set bits → odd → false
    assert(isEvenParity(0x0007) == false);
    // All 16 bits set → 16 (even) → true
    assert(isEvenParity(0xFFFF) == true);
    // Pattern with 8 set bits → even → true
    assert(isEvenParity(0xAAAA) == true);
    // Pattern with 9 set bits → odd → false
    assert(isEvenParity(0xABAB) == false);
    // Max value minus one (0xFFFE) has 15 set bits → odd → false
    assert(isEvenParity(0xFFFE) == false);
    // A value with 4 set bits → even → true
    assert(isEvenParity(0x00F0) == true);
    return 0;
}
