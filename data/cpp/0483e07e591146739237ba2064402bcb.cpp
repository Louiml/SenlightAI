Write a C++ function named `reduce_until_stable` that takes two non-negative integers `a` and `b` (as `long long` values) and repeatedly applies the following operation until no further change occurs: if one number is at least twice the other, replace the larger number by its remainder when divided by twice the smaller number. The operation is applied in the order: check `a` first, then `b`, and continue iterating until neither condition holds (or either number becomes zero). The function should return a `std::pair<long long, long long>` containing the final values of `a` and `b` in that order. The function must handle cases where either or both inputs are zero, and must not modify the original inputs.
The problem is a direct simulation of the while-loop in the given snippet. The main algorithm is straightforward: inside an infinite loop, first check if either number is zero; if so, stop. Otherwise, if `a >= 2 * b`, set `a = a % (2 * b)` (this is the same as `a % (2*b)` which yields a value less than `2*b`). If not, check if `b >= 2 * a`, and if so, set `b = b % (2 * a)`. If neither condition holds, break the loop. The order matters: always check `a` first, then `b`, matching the original code. Important edge cases: (1) both zero → returns `(0,0)` immediately; (2) one zero, other positive → returns `(0,positive)` since loop breaks immediately; (3) when the remainder operation reduces a number to zero, the loop will terminate on the next iteration because one number is zero; (4) there is no risk of division by zero because we only perform modulo when the divisor (`2*b` or `2*a`) is at least 2 (since the smaller number is at least 1). Time complexity: each iteration reduces at least one number to at most half of its previous value (since `a % (2*b) < 2*b` but more precisely strictly less than `a` when `a >= 2*b`, so each reduction is at least halving). Thus the number of iterations is O(log(max(a,b))). Space complexity is O(1).
#include <utility>  // for std::pair

// Reduce (a, b) by repeatedly replacing the larger number with its remainder
// when divided by twice the smaller number, as long as the larger is at least
// twice the smaller. The process stops when either number is zero or when
// neither condition holds. Returns the final pair (a, b).
std::pair<long long, long long> reduce_until_stable(long long a, long long b) {
    while (true) {
        if (a == 0 || b == 0) {
            break;
        }
        if (a >= 2 * b) {
            a = a % (2 * b);
        } else if (b >= 2 * a) {
            b = b % (2 * a);
        } else {
            break;
        }
    }
    return {a, b};
}
#include <cassert>

int main() {
    // Basic case from the snippet: a=10, b=3 -> 10 >= 6, so 10%6=4 -> (4,3)
    auto r1 = reduce_until_stable(10, 3);
    assert(r1.first == 4 && r1.second == 3);

    // Symmetric case: b is larger
    auto r2 = reduce_until_stable(3, 10);
    assert(r2.first == 3 && r2.second == 4);

    // Both zero
    auto r3 = reduce_until_stable(0, 0);
    assert(r3.first == 0 && r3.second == 0);

    // One zero
    auto r4 = reduce_until_stable(5, 0);
    assert(r4.first == 5 && r4.second == 0);
    auto r5 = reduce_until_stable(0, 7);
    assert(r5.first == 0 && r5.second == 7);

    // Already stable when neither is at least twice the other
    auto r6 = reduce_until_stable(2, 3);
    assert(r6.first == 2 && r6.second == 3);

    // Larger numbers: 100 and 1 -> 100>=2, 100%2=0, then loop checks and stops
    auto r7 = reduce_until_stable(100, 1);
    assert(r7.first == 0 && r7.second == 1);

    // Example that reduces multiple times: 100, 30 -> 100 >= 60, 100%60=40 -> now 40>=60? no, 30>=80? no -> (40,30)
    auto r8 = reduce_until_stable(100, 30);
    assert(r8.first == 40 && r8.second == 30);
}
