Write a C++ function that simulates a simplified version of a SystemC-style finite state machine operation using fixed-width bit vectors. The function should take two 8-bit unsigned integer inputs `x` and `y` and a 4-step control sequence `step` (0 to 3). For each step, perform the following operations in order: (Step 0) compute `z = x | y`, then accumulate `acc += x` (where `acc` is an 8-bit unsigned value, wrapping on overflow), and set `result = z | acc`; (Step 1) compute `a = x ^ y`, `b = x & y`, then `z = a & b` (which equals `(x^y) & (x&y)`), and update `acc -= y` (wrapping modulo 256), set `result = z ^ acc`; (Step 2) compute `z = (x & y) ^ (x | y)`, and update `acc *= y` (wrapping modulo 256), set `result = z ^ (acc & 0xFF)`; (Step 3) compute `z = (~x) | (~y)`, and update `acc = acc + (x>>1) + (y>>2)` (wrapping), set `result = z ^ acc`. The function returns an 8-bit unsigned integer `result` for the given step. The initial value of `acc` (before any step) is 0. All arithmetic is done on unsigned 8-bit integers with modulo 256 wrapping. Avoid using bitwise operations on types wider than `uint8_t` for computation; only use `uint8_t` for all arithmetic and bitwise operations. Implement the function `uint8_t compute_step_result(uint8_t x, uint8_t y, int step)` that returns the appropriate `result`.

#include <cassert>
#include <cstdint>

uint8_t compute_step_result(uint8_t x, uint8_t y, int step); // forward declaration

int main() {
    // Test step 0: z = x|y, acc = x, result = z|acc
    assert(compute_step_result(0x0F, 0xF0, 0) == (uint8_t)((0x0F | 0xF0) | 0x0F));
    assert(compute_step_result(0xAA, 0x55, 0) == (uint8_t)((0xAA | 0x55) | 0xAA));

    // Test step 1: z = (x^y)&(x&y), acc = -y (mod 256), result = z ^ acc
    assert(compute_step_result(0x0F, 0xF0, 1) == (uint8_t)(((0x0F ^ 0xF0) & (0x0F & 0xF0)) ^ (uint8_t)(0 - 0xF0)));
    assert(compute_step_result(0x5A, 0xA5, 1) == (uint8_t)(((0x5A ^ 0xA5) & (0x5A & 0xA5)) ^ (uint8_t)(0 - 0xA5)));

    // Test step 2: z = (x&y)^(x|y), acc = 0*y = 0, result = z ^ 0
    assert(compute_step_result(0xFF, 0x01, 2) == (uint8_t)(((0xFF & 0x01) ^ (0xFF | 0x01)) ^ 0));

    // Test step 3: z = ~x | ~y, acc = (x>>1)+(y>>2), result = z ^ acc
    assert(compute_step_result(0x10, 0x20, 3) == (uint8_t)((~0x10 | ~0x20) ^ ((0x10>>1) + (0x20>>2))));

    // Edge case: both zero
    assert(compute_step_result(0, 0, 0) == (uint8_t)(0 | 0));
    assert(compute_step_result(0, 0, 1) == (uint8_t)(0 ^ 0));
    assert(compute_step_result(0, 0, 2) == (uint8_t)(0 ^ 0));
    assert(compute_step_result(0, 0, 3) == (uint8_t)(0xFF ^ 0));

    // Edge case: invalid step returns 0
    assert(compute_step_result(1, 2, 4) == 0);

    return 0;
}

#include <cstdint>

// Simulate a 4-step finite state machine operation on 8-bit unsigned inputs.
// Returns the 8-bit result for the given step, with internal accumulator initialized to 0.
uint8_t compute_step_result(uint8_t x, uint8_t y, int step) {
    uint8_t acc = 0;  // accumulator, wraps modulo 256
    uint8_t z = 0;
    uint8_t a = 0;
    uint8_t b = 0;

    switch (step) {
        case 0:
            z = x | y;
            acc += x;          // accumulate add
            return z | acc;
        case 1:
            a = x ^ y;
            b = x & y;
            z = a & b;         // (x^y) & (x&y)
            acc -= y;          // accumulate subtract
            return z ^ acc;
        case 2:
            z = (x & y) ^ (x | y);
            acc *= y;          // accumulate multiply
            return z ^ (acc & 0xFF);
        case 3:
            z = (~x) | (~y);
            acc += (x >> 1) + (y >> 2);
            return z ^ acc;
        default:
            return 0;          // invalid step, return 0
    }
}

// The solution requires careful tracking of an accumulator that persists across steps only within the function call, initialized to 0. For each step, we must compute intermediate variables `z`, `a`, `b` as per the formulas, then update `acc` using the specified arithmetic operation (addition, subtraction, multiplication, and a custom expression), all with modulo 256 wrapping. Since all operands are `uint8_t`, native arithmetic automatically wraps modulo 256 (because unsigned integer overflow is defined). The key edge cases include: step values outside 0-3 (should be handled gracefully, perhaps by returning 0 or asserting, but the task implies valid steps; we can define behavior for invalid steps by returning 0). Also, multiplication in step 2 must be computed carefully: `acc *= y` where `acc` and `y` are `uint8_t`, and the multiplication result is truncated to 8 bits automatically. For step 3, the expression `(x>>1) + (y>>2)` may overflow 8-bit addition; again, wrapping is automatic. The main algorithm is a simple switch on step, computing local `uint8_t` variables and updating `acc` accordingly. Time complexity is O(1) per call, and auxiliary space is O(1). No external libraries beyond `<cstdint>` are needed.
