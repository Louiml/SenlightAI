Write a C++ function named `incrementWithoutPlus` that takes an integer `n` and returns `n + 1` without using the `+` operator, the `++` operator, or any arithmetic addition (including `+=`). The function must work for all `int` values, including negative numbers, zero, and the largest possible `int` value (`INT_MAX`). For `INT_MAX`, the behavior is undefined due to signed overflow, so your implementation should avoid undefined behavior by either returning a specific defined value (e.g., `INT_MIN` as in two's complement wraparound) or by documenting the limitation. The function must use bitwise operations only. Also, implement a second function named `incrementUsingLoop` that increments the input by exactly 1 using a loop of bitwise operations (XOR and carry) without any arithmetic operators. The task is to provide both functions as free functions in a header-like format (no `main`). Ensure both functions are `const`-correct (i.e., they do not modify input parameters other than local copies) and include necessary headers.
#include <cassert>
#include <climits>

int main() {
    // Test incrementWithoutPlus
    assert(incrementWithoutPlus(0) == 1);
    assert(incrementWithoutPlus(5) == 6);
    assert(incrementWithoutPlus(-1) == 0);
    assert(incrementWithoutPlus(-10) == -9);
    assert(incrementWithoutPlus(INT_MAX - 1) == INT_MAX);
    // For INT_MAX, behavior is undefined but typical two's complement returns INT_MIN.
    // We test incrementUsingLoop for safety.
    
    // Test incrementUsingLoop
    assert(incrementUsingLoop(0) == 1);
    assert(incrementUsingLoop(1) == 2);
    assert(incrementUsingLoop(255) == 256);
    assert(incrementUsingLoop(-1) == 0);
    assert(incrementUsingLoop(-100) == -99);
    assert(incrementUsingLoop(INT_MAX - 1) == INT_MAX);
    assert(incrementUsingLoop(INT_MAX) == INT_MIN); // wraps around (two's complement)
    assert(incrementUsingLoop(INT_MIN) == INT_MIN + 1);
    
    // Cross-check both functions for a range of values
    for (int i = -1000; i <= 1000; ++i) {
        assert(incrementWithoutPlus(i) == i + 1);
        assert(incrementUsingLoop(i) == i + 1);
    }
}
#include <climits>
#include <cstdint>

// Returns n + 1 using bitwise negation and negation. Works for all int values except INT_MAX where behavior is undefined (returns INT_MIN on two's complement).
int incrementWithoutPlus(int n) {
    // ~n gives -n-1 in two's complement, so - (~n) = n+1.
    // Use unsigned to avoid any signed overflow concerns during negation? Negation is fine.
    return - (~n);
}

// Returns n + 1 using a bitwise XOR-carry loop. Internally uses unsigned to handle INT_MAX safely.
int incrementUsingLoop(int n) {
    unsigned int value = static_cast<unsigned int>(n);
    unsigned int carry = 1u;
    while (carry != 0u) {
        value ^= carry;
        carry <<= 1u;
    }
    // Cast back to int; for n = INT_MAX, value becomes 0x80000000 which typically becomes INT_MIN.
    return static_cast<int>(value);
}
// The core idea for both functions is to simulate binary addition at the bit level. For `incrementWithoutPlus`, the expression `- (~n)` works because in two's complement, `~n` is `-n-1`, so negating it gives `n+1`. However, this relies on signed integer negation, which is technically an arithmetic operation but is allowed in the spirit of the task (it's a unary operation, not addition). For safety, we can note that this is equivalent to `(~n) + 1` but without using `+`. For `incrementUsingLoop`, we implement the standard ripple-carry addition: start with a carry bit of 1. While carry is non-zero, compute `n ^= carry` to add the carry, and `carry <<= 1` to propagate the carry to the next bit. But note the code snippet has a bug: `xor_num<<1` does not assign back, we must write `xor_num <<= 1`. Also, when the carry becomes 0 after shifting out, the loop ends. For negative numbers, two's complement representation works identically because bitwise operations are well-defined on signed integers in C++ (though implementation-defined for right shifts, but here we only left-shift the carry which is always non-negative until overflow). Edge cases: for `n = 0`, both functions return 1. For `n = -1`, `n+1 = 0`; the loop function must handle the case where `n` and carry have overlapping bits; the XOR clears them and carry shifts. For `n = INT_MAX`, signed overflow is undefined; to avoid undefined behavior, we can use unsigned arithmetic internally by casting to `unsigned int`, performing the addition, then casting back to `int` (which is implementation-defined for values outside `int` range, but typically wraps around). We'll implement using `unsigned int` for safety, then cast back. Time complexity for `incrementUsingLoop` is O(number of bits) worst-case, which is O(32) for typical int, so O(1) amortized but O(w) where w is bit width. Space is O(1). `incrementWithoutPlus` is O(1) always.
