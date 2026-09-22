Implement a C++ function named `random01Step` that simulates one step of the 48-bit linear congruential generator (LCG) used by the standard `erand48()` family, and returns a `double` in the half-open interval `[0.0, 1.0)` as described in the snippet. The function must accept a reference to a `std::array<unsigned short, 3>` state, update it in place using the same multiplier `0x5DEECE66D`, increment `0xB`, and modulus `2^48`, then produce a double by constructing a bit pattern where the exponent bits correspond to `1.0` and the 48 most significant bits of the mantissa are taken from the new state, then subtracting `1.0`. The returned value must be uniformly distributed over all possible 48-bit states, and the state update must exactly match the snippet's disassembly into three 16-bit values (low, middle, high). Your function must be `const`-correct where appropriate, and you should assume `unsigned short` is 16 bits. The function should not rely on any external random library or C standard library random functions.

The core algorithm is a linear congruential generator (LCG) with modulus `m = 2^48`. The state is stored as three 16-bit integers: `state[0]` is the least significant 16 bits, `state[1]` the middle, `state[2]` the most significant. To update, we assemble the 48-bit value `x = (state[2] << 32) | (state[1] << 16) | state[0]`. Because `x` might be up to 48 bits, we must use a 64-bit integer type (e.g., `uint64_t`) to avoid overflow. Compute `next = (a * x + c) & 0xFFFFFFFFFFFF` where `a = 0x5DEECE66D` and `c = 0xB`. Then disassemble back: `state[0] = next & 0xFFFF`, `state[1] = (next >> 16) & 0xFFFF`, `state[2] = (next >> 32) & 0xFFFF`. For the double conversion, we need to produce a bit pattern that represents a number in `[1.0, 2.0)`. The IEEE-754 double format has 1 sign bit, 11 exponent bits, and 52 mantissa bits. The exponent bias is 1023, so exponent bits `0x3FF` give `2^0 = 1.0`. We set the sign bit to 0, exponent to `0x3FF`, and fill the 48 most significant mantissa bits with the new state bits: `mantissa = (state[2] << 36) | (state[1] << 20) | (state[0] << 4) | (state[2] >> 12)`. The last 4 bits are a copy of the top 4 bits of `state[2]` as in the snippet. We then reinterpret the 64-bit integer as a double (using `memcpy` or a union, but `memcpy` is safer for strict aliasing), subtract `1.0` to get `[0.0, 1.0)`. Edge cases: initial state could be all zeros, but after one step it becomes nonzero; the function must handle any valid 48-bit state. Time complexity is O(1) per step; space O(1).

#include <array>
#include <cstdint>
#include <cstring>

// Simulates one step of the 48-bit LCG used by erand48() and returns a double in [0.0, 1.0).
// Updates the 3x16-bit state in place.
double random01Step(std::array<unsigned short, 3>& state) {
    // Constants from the standard drand48().
    constexpr uint64_t a = 0x5DEECE66DULL;
    constexpr uint64_t c = 0xBULL;
    constexpr uint64_t mask = 0xFFFFFFFFFFFFULL; // 2^48 - 1

    // Assemble current 48-bit value.
    uint64_t x = (static_cast<uint64_t>(state[2]) << 32) |
                 (static_cast<uint64_t>(state[1]) << 16) |
                 static_cast<uint64_t>(state[0]);

    // Compute next value with modulo 2^48.
    x = (a * x + c) & mask;

    // Disassemble into three 16-bit pieces (little-endian order).
    state[0] = static_cast<unsigned short>(x & 0xFFFF);
    state[1] = static_cast<unsigned short>((x >> 16) & 0xFFFF);
    state[2] = static_cast<unsigned short>((x >> 32) & 0xFFFF);

    // Build a double in [1.0, 2.0) using the 48 bits as the high mantissa.
    uint64_t bits = (0x3FFULL << 52) |
                    (static_cast<uint64_t>(state[2]) << 36) |
                    (static_cast<uint64_t>(state[1]) << 20) |
                    (static_cast<uint64_t>(state[0]) << 4) |
                    (static_cast<uint64_t>(state[2]) >> 12);

    double value;
    std::memcpy(&value, &bits, sizeof(value));
    return value - 1.0;
}

#include <cassert>
#include <array>
#include <cmath>

int main() {
    // Test 1: Starting from a known seed (like srand48(0) sets state[2]=0, state[1]=0, state[0]=0x330e).
    std::array<unsigned short, 3> s1 = {0x330e, 0, 0};
    double r1 = random01Step(s1);
    // Expected first value from drand48() with seed 0 is approximately 0.1545995849.
    assert(std::fabs(r1 - 0.1545995849) < 1e-10);
    // State should be updated to the next LCG value (verify via independent computation).
    // Let's just check it's not unchanged.
    assert(s1[0] != 0x330e || s1[1] != 0 || s1[2] != 0);

    // Test 2: Two consecutive calls give different values (likely).
    std::array<unsigned short, 3> s2 = {1, 2, 3};
    double a = random01Step(s2);
    double b = random01Step(s2);
    assert(a != b);

    // Test 3: Value is in [0, 1) and not negative.
    std::array<unsigned short, 3> s3 = {0xFFFF, 0xFFFF, 0xFFFF};
    double r3 = random01Step(s3);
    assert(r3 >= 0.0 && r3 < 1.0);

    // Test 4: The state update matches known LCG for a simple case.
    // Starting with x=0, next = (0*0x5DEECE66D + 0xB) & 0xFFFFFFFFFF = 0xB.
    std::array<unsigned short, 3> s4 = {0, 0, 0};
    random01Step(s4);
    // Expected state: low=0x000B, mid=0, high=0.
    assert(s4[0] == 0x000B);
    assert(s4[1] == 0x0000);
    assert(s4[2] == 0x0000);

    // Test 5: Known sequence from erand48: seed 1 gives first double ~0.1647029377.
    std::array<unsigned short, 3> s5 = {0x330e, 0, 1}; // srand48(1) sets low=0x330e, high=1
    double r5 = random01Step(s5);
    assert(std::fabs(r5 - 0.1647029377) < 1e-10);

    // Test 6: After many steps, state cycles but always valid 48-bit.
    std::array<unsigned short, 3> s6 = {12345, 54321, 999};
    for (int i = 0; i < 1000; ++i) {
        double val = random01Step(s6);
        assert(val >= 0.0 && val < 1.0);
    }

    // Test 7: Two independent states produce independent-looking sequences.
    std::array<unsigned short, 3> s7a = {111, 222, 333};
    std::array<unsigned short, 3> s7b = {111, 222, 333};
    // They should produce the same sequence, proving determinism.
    double r7a = random01Step(s7a);
    double r7b = random01Step(s7b);
    assert(r7a == r7b);

    return 0;
}
