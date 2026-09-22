Implement a C++ function `simulateLFSR` that takes a 32-bit unsigned integer `seed` and an integer `steps` (non-negative), and returns the final 32-bit state obtained by iteratively applying a Galois linear-feedback shift register (LFSR) update exactly `steps` times. The LFSR update rule is: compute `new_bit = bit31 ^ bit21 ^ bit1 ^ bit0` (using 0-based bit indexing from least significant bit, where bit31 is the most significant bit, bit21 is bit index 21, bit1 is bit index 1, bit0 is bit index 0), then shift the register right by one, set bit30 to `new_bit`, and set bit31 to 0. The function must use only bitwise operations and avoid loops over bits; the update must be performed in O(1) per step regardless of the integer width. The input `seed` is arbitrary 32-bit unsigned, and `steps` can be up to 10^9; handle large values efficiently by noting the LFSR is periodic with period 2^32-1 (assuming a non-zero seed; if seed is 0, the state remains 0 forever). For non-zero seeds, reduce `steps` modulo the period to avoid long loops. The function should be `const`-correct and self-contained with only standard headers.

// The core is a 32-bit LFSR with taps at positions 31, 21, 1, and 0. The update operator is linear over GF(2) on the 32-bit state. For a non-zero initial state, the sequence has maximal length: the period is 2^32–1 because the polynomial is primitive. Therefore, applying `steps` updates is equivalent to applying `steps % (2^32-1)` updates when the seed is non-zero, and the state remains zero for a zero seed. Since `steps` can be huge, we reduce modulo the period. Each update is O(1) using bit operations: extract the four bits, XOR them, shift right by one, clear bit31, set bit30 to the new bit. Numerically, bit indices correspond as follows: bit31 = (state >> 31) & 1, bit21 = (state >> 21) & 1, bit1 = (state >> 1) & 1, bit0 = state & 1. The shift right by 1 moves bit i to i-1, then we set bit30 (which becomes bit29 after shift? careful: after right shift, the old bit30 moved to bit29, and bit30 is vacant; so we set bit30 directly). So the update is `state = (state >> 1) | (new_bit << 30)` but with bit31 cleared—but after right shift, bit31 is 0 automatically, so fine. Edge cases: seed=0 stays 0; steps=0 returns seed; large steps handled via modulo. Time complexity: O(1) per step, but with reduction it's O(log steps) if we use fast exponentiation on the linear map, but since period is fixed, we directly mod reduce, making it O(1). Space complexity O(1). We must also handle the case where seed=0: mod reduction would give period 1? Actually zero state is fixed point, so we return 0 immediately.

#include <cstdint>

/**
 * Simulate a 32-bit LFSR with taps at bits 31, 21, 1, 0.
 * The update is: new_bit = bit31 ^ bit21 ^ bit1 ^ bit0;
 * state = (state >> 1) | (new_bit << 30);  // bit31 automatically cleared
 * For non-zero seed, the period is 2^32-1, so reduce steps modulo that.
 * For zero seed, the state remains zero.
 */
std::uint32_t simulateLFSR(std::uint32_t seed, std::uint64_t steps) {
    if (seed == 0) {
        return 0;
    }
    // Period of maximal-length LFSR with primitive polynomial of degree 32.
    const std::uint64_t period = (std::uint64_t(1) << 32) - 1;
    steps %= period;
    std::uint32_t state = seed;
    for (std::uint64_t i = 0; i < steps; ++i) {
        std::uint32_t bit31 = (state >> 31) & 1u;
        std::uint32_t bit21 = (state >> 21) & 1u;
        std::uint32_t bit1  = (state >>  1) & 1u;
        std::uint32_t bit0  = state & 1u;
        std::uint32_t new_bit = bit31 ^ bit21 ^ bit1 ^ bit0;
        state = (state >> 1) | (new_bit << 30);
    }
    return state;
}

#include <cassert>
#include <cstdint>

// Include the solution function here (or link)

int main() {
    // Zero seed always remains zero.
    assert(simulateLFSR(0, 0) == 0);
    assert(simulateLFSR(0, 1000000) == 0);

    // Zero steps returns the original seed.
    std::uint32_t seed = 0x12345678u;
    assert(simulateLFSR(seed, 0) == seed);

    // Manual single-step test: compute expected using direct bit operations.
    std::uint32_t s = 0x80000000u; // bit31 set
    std::uint32_t expected = (s >> 1) | (1u << 30); // new_bit=1 because bit31=1, others 0
    assert(simulateLFSR(s, 1) == expected);

    // One step from a known state.
    s = 0xFFFFFFFFu;
    // bit31=1, bit21=1, bit1=1, bit0=1 => new_bit=1^1^1^1 = 0
    expected = (s >> 1) | (0u << 30); // = 0x7FFFFFFF
    assert(simulateLFSR(s, 1) == expected);

    // Two steps: apply the update twice manually from a simple seed.
    s = 0x00000001u; // bit0=1, others 0
    // step1: new_bit=0^0^0^1=1 => state=(0>>1)|(1<<30)=0x40000000
    std::uint32_t step1 = 0x40000000u;
    assert(simulateLFSR(s, 1) == step1);
    // step2: from step1, bit31=0, bit21=0, bit1=0, bit0=0 => new_bit=0 => state=(0x40000000>>1)=0x20000000
    assert(simulateLFSR(s, 2) == 0x20000000u);

    // Period check: after period steps from non-zero seed, we return to seed.
    const std::uint64_t period = (std::uint64_t(1) << 32) - 1;
    s = 0xDEADBEEFu;
    assert(simulateLFSR(s, period) == s);
    assert(simulateLFSR(s, period * 2) == s);
    // Large steps beyond period are reduced.
    assert(simulateLFSR(s, period + 1) == simulateLFSR(s, 1));
    assert(simulateLFSR(s, 1000000000) == simulateLFSR(s, 1000000000 % period));

    // Edge case: steps=1 on a state that becomes zero? Actually non-zero never hits zero.
    // But we can test a state that after one step gives a known value.
    s = 0x00000002u; // bit1=1
    // new_bit=0^0^1^0=1 => state=(2>>1)|(1<<30)=0x40000001
    assert(simulateLFSR(s, 1) == 0x40000001u);

    return 0;
}
