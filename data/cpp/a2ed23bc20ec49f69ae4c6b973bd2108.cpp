// Write a C++ function named `countSetBits` that takes a 32-bit unsigned integer (`uint32_t`) as input and returns the number of set bits (i.e., bits with value 1) in its binary representation. The function must not use any loop that iterates over all 32 bits individually; instead, it should use the bit-manipulation trick `n = n & (n - 1)` in a loop. The function should be declared `const`-correct (i.e., take the parameter by value and mark the function itself as `const` if it were a member, but as a free function just use `const` on the parameter if appropriate—here, since it's a primitive type passed by value, simply ensure no modifications to any external state). Handle the edge case where the input is zero, which should return 0. The solution must be self-contained, including appropriate headers, and must not include a `main` function in the solution section.
// The core algorithm uses the classic bit manipulation trick: `n = n & (n - 1)`. This operation clears the lowest set bit in `n` (i.e., changes the rightmost 1-bit to 0). For example, if `n` is `12` (binary `1100`), then `n - 1` is `1011`, and `n & (n - 1)` yields `1000` (8). By repeatedly applying this operation, each iteration removes exactly one set bit. The loop continues until `n` becomes 0, and the count increments each time. Since each iteration removes one set bit, the number of iterations equals the number of set bits, making the time complexity `O(k)` where `k` is the number of set bits (worst-case `O(32)` for a 32-bit integer). Space complexity is `O(1)` because we only use a single integer counter. The edge case of `n = 0` is naturally handled because the loop condition `while (n > 0)` is false from the start, returning 0. The function works correctly for all values from `0` to `UINT32_MAX` (4294967295). No special handling for negative numbers is needed because the input type is unsigned. The parameter is passed by value, so `const` correctness is inherent (no modification of the caller's data), and we can mark the function itself as `const` only if it were a member; here it is a free function, so we just ensure the parameter is `const` by value (which is redundant but acceptable) or simply pass by value without `const`. For clarity, we pass by value and note that the function does not modify any external state.
#include <cstdint>

// Count the number of set bits (1s) in a 32-bit unsigned integer.
// Uses the n = n & (n - 1) trick to clear the lowest set bit each iteration.
std::uint32_t countSetBits(const std::uint32_t n) {
    std::uint32_t count = 0;
    std::uint32_t value = n;  // local copy to preserve const-correctness (though by-value)
    while (value > 0) {
        value = value & (value - 1);
        ++count;
    }
    return count;
}
#include <cstdint>
#include <cassert>

// The solution function declaration is assumed to be available (as defined above).
std::uint32_t countSetBits(const std::uint32_t n);

int main() {
    // Test zero -> 0 set bits
    assert(countSetBits(0) == 0);
    
    // Test powers of two: each has exactly 1 set bit
    assert(countSetBits(1) == 1);
    assert(countSetBits(2) == 1);
    assert(countSetBits(4) == 1);
    assert(countSetBits(8) == 1);
    assert(countSetBits(16) == 1);
    assert(countSetBits(1U << 31) == 1);  // highest bit of uint32_t
    
    // Test values with multiple set bits
    assert(countSetBits(3) == 2);       // 0b11
    assert(countSetBits(7) == 3);       // 0b111
    assert(countSetBits(0xFF) == 8);    // 255
    assert(countSetBits(0xFFFFFFFF) == 32); // all 32 bits set
    
    // Test a random known value
    assert(countSetBits(0xA5) == 4);    // 0b10100101 -> 4 bits
    
    return 0;
}
