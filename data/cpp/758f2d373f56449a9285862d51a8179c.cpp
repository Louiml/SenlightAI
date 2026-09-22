// Write a C++ function named `halfFloor` that takes a single `long long` integer parameter `n` and returns the result of integer division of `n` by 2, truncated toward zero (i.e., the floor for non-negative numbers and the ceiling for negative numbers, matching C++'s built-in division semantics). The function must not print anything; it must simply return the computed value. The function should handle the full range of `long long` values, including `LLONG_MIN` (the minimum possible value), without overflow or undefined behavior. The solution must be self-contained, include necessary headers, and be usable in a standalone program.

// The solution directly uses the built-in integer division operator `/` on two `long long` operands. In C++ (since C++11), integer division truncates toward zero, meaning that for positive `n`, `n/2` gives the floor, while for negative `n`, it gives the ceiling. For example, `7/2 = 3`, `-7/2 = -3` (not `-4`). This matches the problem requirement exactly. The only edge case is potential overflow: if `n` is `LLONG_MIN` and we attempted to compute `-n/2`, that would overflow, but we do not negate `n`; we directly divide `n` by `2`. `LLONG_MIN / 2` is a well-defined value equal to approximately `-4.611e18`, and no overflow occurs because the divisor is `2`, not `-1`.  
//
// Time complexity is O(1) since the operation is a single arithmetic instruction. Space complexity is O(1) as no additional data structures are used. No special handling is needed for `n = 0`, `n = 1`, or any other value; the division operator works correctly for all representable `long long` values. The function is `const` correct in the sense that it does not mutate any external state, and we mark the parameter as passed by value (which is appropriate for a primitive type) but could also make it `const long long n` for clarity, though it is unnecessary for a scalar parameter.

#include <cstdint>

// Return the integer division of n by 2, truncated toward zero.
// Works for all long long values including LLONG_MIN.
long long halfFloor(const long long n) {
    return n / 2;
}

#include <cassert>
#include <cstdint>
#include <limits>

int main() {
    assert(halfFloor(0) == 0);
    assert(halfFloor(1) == 0);
    assert(halfFloor(2) == 1);
    assert(halfFloor(7) == 3);
    assert(halfFloor(-1) == 0);
    assert(halfFloor(-2) == -1);
    assert(halfFloor(-7) == -3);
    assert(halfFloor(1000000000000000000LL) == 500000000000000000LL);
    assert(halfFloor(-1000000000000000000LL) == -500000000000000000LL);
    assert(halfFloor(std::numeric_limits<long long>::min()) == std::numeric_limits<long long>::min() / 2);
}
