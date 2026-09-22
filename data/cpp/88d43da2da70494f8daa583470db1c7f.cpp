// Write a C++ function named `canReachTarget` that takes two positive integers `x` and `y`, and determines whether starting from the smaller value, repeatedly doubling that smaller value (only the current smaller one, not the original) will eventually produce a value equal to the larger original number. The function should return `true` if the doubling sequence ever equals the larger original value, and `false` otherwise. For example, given `x=4` and `y=8`, the smaller is `4`, doubling gives `8`, which matches `8`, so return `true`; but given `x=6` and `y=10`, doubling the smaller `6` gives `12`, `24`, etc., never reaching `10`, so return `false`. The function must handle cases where `x == y` (return `true` because the starting value already matches) and must not modify the input parameters.
#include <cassert>

int main() {
    // Basic cases
    assert(canReachTarget(4, 8) == true);    // 4 -> 8
    assert(canReachTarget(8, 4) == true);    // smaller is 4, same as above
    assert(canReachTarget(6, 10) == false);  // 6 ->12 ->24 ... never 10
    assert(canReachTarget(10, 6) == false);  // same as above, swapped

    // Equal values
    assert(canReachTarget(7, 7) == true);    // already equal
    assert(canReachTarget(1, 1) == true);

    // Powers of two relationship
    assert(canReachTarget(3, 24) == true);   // 3->6->12->24
    assert(canReachTarget(24, 3) == true);   // same from other order
    assert(canReachTarget(5, 80) == true);   // 5->10->20->40->80

    // Unreachable cases
    assert(canReachTarget(5, 100) == false); // 5->10->20->40->80->160 (overshoots)
    assert(canReachTarget(9, 18) == true);   // 9->18 (simple double)
    assert(canReachTarget(18, 9) == true);   // swapped order

    // Larger numbers
    assert(canReachTarget(1000, 16000) == true);  // 1000->2000->...->16000 (4 doublings)
    assert(canReachTarget(1000, 16001) == false); // overshoots or skips

    return 0;
}
#include <cstdint>

// Returns true if repeatedly doubling the smaller of x and y
// eventually produces a value equal to the larger original number.
bool canReachTarget(int x, int y) {
    // Preserve original inputs and determine the target (the larger original).
    const int target = (x > y) ? x : y;
    int current = (x < y) ? x : y;  // start from the smaller

    // Loop while current is still <= target (no need to go beyond).
    while (current <= target) {
        if (current == target) {
            return true;
        }
        // Double the current value for the next iteration.
        current *= 2;
    }
    return false;
}
// The core algorithm is straightforward: identify the smaller of the two inputs and the larger original value, then simulate the repeated doubling of the current smaller value inside a loop that continues while the doubled value is less than or equal to the target (the larger original). If at any iteration the current value equals the target, return `true` immediately. If the loop exits without a match, return `false`. The main edge case is when `x == y`: the smaller equals the larger, so the initial condition matches and the function should return `true` without any doubling. Another subtlety is that the doubling should apply to the current value, not the original smaller, which naturally happens by updating the variable. Since the smaller value doubles each step, the number of iterations is at most logarithmically proportional to the ratio between the larger and smaller values, making the time complexity `O(log2(target / smaller))` and the space complexity `O(1)` (only a few integer variables). No overflow concern arises for typical 32-bit integers as long as the problem inputs are reasonable, but the loop condition `<=` with a positive multiplier will naturally stop before infinite growth.
