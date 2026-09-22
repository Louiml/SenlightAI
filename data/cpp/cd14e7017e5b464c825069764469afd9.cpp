Write a C++ function that takes a single integer `n` and returns the number of pieces a pizza can be cut into if the cuts are made using straight lines that must pass through the center of the pizza, with the restriction that no two lines are parallel and all lines are distinct. The possible number of cuts is between 1 and 10^9 inclusive. The function should return 0 if `n` is less than 2, and otherwise return `n - 2`. This is inspired by a simple coding problem where each additional cut through the center increases the number of slices by 2, but the problem asks for a specific transformation: the result equals the number of cuts minus 2, with a minimum of 0.
#include <cassert>

int main() {
    assert(extraSlices(0) == 0);
    assert(extraSlices(1) == 0);
    assert(extraSlices(2) == 0);
    assert(extraSlices(3) == 1);
    assert(extraSlices(4) == 2);
    assert(extraSlices(10) == 8);
    assert(extraSlices(1000000000) == 999999998);
    assert(extraSlices(2) == 0);
    return 0;
}
#include <cstdint>

// Returns the number of "extra" pizza slices as defined by the problem:
// result = n - 2 if n >= 2, otherwise 0.
// The parameter represents the number of straight cuts through the center.
int64_t extraSlices(int64_t n) {
    if (n - 2 < 1) {
        return 0;
    }
    return n - 2;
}
// The problem is straightforward: given the number of cuts `n` (each cut passes through the center, so each cut adds exactly 2 new slices to the pizza, because each new line crosses all previous lines at the center, dividing two existing slices into four). Normally, the number of slices after `n` cuts through the center is `2 * n`. However, the task asks for a different quantity: `n - 2`, but only if `n >= 2`; otherwise return 0. The edge case is when `n` is 1 or less, where no valid "extra" cuts exist, so the result is 0. For `n >= 2`, simply compute `n - 2`. The time complexity is O(1) and space complexity is O(1). The function must handle very large inputs (up to 10^9) and should use a 64-bit integer to avoid overflow if needed (though `n - 2` fits in 32-bit signed, but using `long long` is safer).
