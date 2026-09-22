// Given three positive integers `a`, `b`, and `d` representing step sizes and a total distance, write a C++ function `journeyCost(int a, int b, int d)` that simulates two travel modes. In the "forward" mode, starting with distance `d`, you repeatedly subtract `a` if the remaining distance is at least `a`, adding `a` to the cost for each full `a`-step and adding an extra `b` after each full step (except when the remaining distance becomes exactly 0 after the subtraction, in which case you stop without the extra `b`). If the remaining distance is less than `a`, you add that remaining remainder and stop. In the "backward" mode, you perform the symmetric operation: repeatedly subtract `b` if the remaining distance is at least `b`, adding `b` to the cost and adding an extra `a` after each full `b`-step (no special zero case—always add the extra `a`), and if the remainder is less than `b`, add that remainder and stop. The function must return the total cost of running both modes sequentially on the same initial distance `d`. Note that `a`, `b`, and `d` can each be up to 10^9, so use 64-bit integers to avoid overflow, and assume all inputs are positive. The function must be self-contained and not read from standard input.
The problem simulates two independent processes on the same initial value `d`, and we need the sum of their costs. The forward process: while the current distance `k` is positive, if `k >= a`, we subtract `a` from `k`. After subtraction, if `k` became `0`, we add just `a` to the cost; otherwise we add `a + b`. If `k < a`, we add `k` to the cost and break. The backward process: while `k` is positive, if `k >= b`, subtract `b` from `k` and add `b + a` to the cost; otherwise add `k` and break. The key edge case in the forward mode is when `d` is an exact multiple of `a`: the last full step doesn't add the extra `b`, which reduces the cost compared to the general full-step pattern. The backward mode always adds the extra `a` after every full step. Time complexity is O(d/min(a,b)) in the worst case if we literally loop, which is infeasible for d up to 1e9. Instead, we observe that the forward process performs floor(d/a) full steps, and if d % a == 0, the cost is (floor(d/a) * a) + ((floor(d/a)-1) * b) because the last step doesn't add b; otherwise, if there is a remainder r = d % a (with r>0), the cost is (floor(d/a) * (a+b)) + r. Similarly, the backward process performs floor(d/b) full steps, each contributing (b+a), plus the remainder r = d % b if non-zero. So the total cost can be computed in O(1) using integer division and modulo. Time complexity is O(1), space O(1).
#include <cstdint>

// Returns the total cost of forward and backward travel given step sizes a, b and distance d.
// All parameters are positive integers; uses 64-bit arithmetic to avoid overflow.
std::int64_t journeyCost(std::int64_t a, std::int64_t b, std::int64_t d) {
    // Forward mode: subtract a repeatedly, cost = a per full step, plus extra b after each full step except when the remainder becomes exactly 0.
    std::int64_t full_a = d / a;
    std::int64_t remainder_a = d % a;
    std::int64_t forward_cost = full_a * a;
    if (remainder_a == 0) {
        if (full_a > 0) {
            forward_cost += (full_a - 1) * b;
        }
    } else {
        forward_cost += full_a * b + remainder_a;
    }

    // Backward mode: subtract b repeatedly, cost = b per full step, plus extra a after every full step.
    std::int64_t full_b = d / b;
    std::int64_t remainder_b = d % b;
    std::int64_t backward_cost = full_b * (b + a);
    if (remainder_b != 0) {
        backward_cost += remainder_b;
    }

    return forward_cost + backward_cost;
}
#include <cassert>
#include <cstdint>

// Forward declaration for the function under test (already defined above in Solution).
std::int64_t journeyCost(std::int64_t a, std::int64_t b, std::int64_t d);

int main() {
    // Example from typical small input: a=2, b=3, d=10
    // Forward: d=10: subtract2 (10->8) cost=2+3=5; subtract2 (8->6) cost=5+5=10; subtract2 (6->4) cost=15; subtract2 (4->2) cost=20; subtract2 (2->0) cost=20+2=22 (no extra b because reached 0). Forward=22.
    // Backward: d=10: subtract3 (10->7) cost=3+2=5; subtract3 (7->4) cost=10; subtract3 (4->1) cost=15; remainder1<3 add1 => cost=16. Backward=16. Total=38.
    assert(journeyCost(2, 3, 10) == 38);

    // Exact multiple for forward: a=5, b=7, d=15
    // Forward: three full steps, last no extra b => cost=3*5 + 2*7 = 15+14=29.
    // Backward: d=15, b=7: full_b=2, remainder=1 => cost=2*(7+5)+1 = 24+1=25. Total=54.
    assert(journeyCost(5, 7, 15) == 54);

    // d smaller than both a and b: a=10, b=20, d=5 => forward: remainder 5 => cost 5; backward: remainder 5 => cost 5; total=10.
    assert(journeyCost(10, 20, 5) == 10);

    // a=1, b=1, d=1 => forward: full_a=1 remainder 0 => cost=1; backward: full_b=1 remainder 0 => cost=1*(1+1)=2; total=3.
    assert(journeyCost(1, 1, 1) == 3);

    // Large values to check overflow: a=1e9, b=1e9, d=1e9 (max)
    // Forward: full_a=1, remainder 0 => cost=1e9
    // Backward: full_b=1, remainder 0 => cost=2e9
    // Total=3e9 fits in int64_t.
    assert(journeyCost(1000000000LL, 1000000000LL, 1000000000LL) == 3000000000LL);

    return 0;
}
