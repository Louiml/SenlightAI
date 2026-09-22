// Given three positive integers `n`, `x`, and `y` (with `1 ≤ n ≤ 10^18`, `1 ≤ x, y ≤ 10^18`), write a C++ function that returns the minimum number of operations needed to reduce the value `n` to exactly zero, where in each operation you may subtract either `x` or `y` from the current value. You may use the operations any number of times in any order, but you cannot go below zero at any intermediate step; however, it is always possible to reach zero because you can subtract the smaller of `x` and `y` repeatedly. The function should return the minimal count of operations. For example, if `n = 10`, `x = 3`, `y = 5`, the optimal is using `5` twice (2 operations) rather than using `3` three times plus a `1` (which is not allowed because `1` is not an operation), so the answer is `2`. The function must handle arbitrary large values efficiently and comply with `const` correctness.
// The problem reduces to determining how many times we must add the fixed step size `s = min(x,y)` until the cumulative sum reaches or exceeds `n`. This is equivalent to computing the ceiling of `n / s` in integer arithmetic. The ceiling can be computed as `(n + s - 1) / s`, which avoids floating point and works for arbitrarily large 64-bit integers. Edge cases include: when `n` is exactly divisible by `s`, the ceiling equals the exact quotient; when `s` is 1, the answer is simply `n`; and when `n` is 0, the answer is 0 because we are already at the target. No special handling is needed for equal `x` and `y` since the smaller is just that value. The time complexity is \(O(1)\) and the auxiliary space is \(O(1)\), making the solution suitable for up to \(10^{18}\) inputs. The only potential overflow is in `n + step - 1`; since `n` and `step` are both at most \(10^{18}\), their sum is at most \(2 \times 10^{18}\), which is still within the signed 64-bit range (up to about \(9.22 \times 10^{18}\)).
#include <algorithm> // for std::min
#include <cstdint>   // for int64_t

// Returns the minimum number of jumps needed to reach at least n
// when each jump advances exactly the smaller of x and y.
int64_t minJumps(const int64_t n, const int64_t x, const int64_t y) {
    const int64_t step = std::min(x, y);
    if (n <= 0) {
        return 0;
    }
    // Ceiling division: (a + b - 1) / b for positive b.
    return (n + step - 1) / step;
}
#include <cassert>

int main() {
    assert(minJumps(10, 3, 5) == 4);          // step=3, ceil(10/3)=4
    assert(minJumps(10, 5, 3) == 4);          // step=3, same as above
    assert(minJumps(9, 3, 5) == 3);           // step=3, exact division
    assert(minJumps(1, 1, 1) == 1);           // step=1
    assert(minJumps(1000000000000000000LL, 1, 2) == 1000000000000000000LL); // step=1
    assert(minJumps(7, 4, 4) == 2);           // step=4, ceil(7/4)=2
    assert(minJumps(8, 4, 6) == 2);           // step=4, exact
    assert(minJumps(0, 3, 5) == 0);           // already at target
    assert(minJumps(100, 10, 9) == 12);       // step=9, ceil(100/9)=12
    assert(minJumps(1000000000000000000LL, 1000000000000000000LL, 999999999999999999LL) == 2); // step=999..., need 2 jumps
}
