Write a C++ function named `findDelayedArrivalTime` that takes two integer parameters, `arrivalTime` and `delayedTime`, representing the scheduled arrival hour (0–23, on a 24-hour clock) and the number of hours the arrival is delayed (non-negative). The function must return the actual arrival hour on the same 24-hour clock, wrapping around past midnight if the sum exceeds 23. For example, if an arrival is scheduled at 20:00 and is delayed by 5 hours, the actual arrival is at 01:00 the next day, so the function returns 1. The function must handle sums equal to or greater than 24 by subtracting 24, and sums less than 24 by returning the sum directly. Values for `arrivalTime` are guaranteed to be in [0, 23], and `delayedTime` is guaranteed to be non-negative and small enough that the sum fits within a standard `int`. Provide a self-contained C++ implementation with appropriate `const` correctness, and no `main` function in the solution section.
// The solution computes the total time as `arrivalTime + delayedTime`. Since both inputs are non-negative and the sum fits in an `int`, the only edge case is when the sum is at least 24, which indicates the arrival occurs after midnight. In that case, subtract 24 to wrap around to the correct hour on the 24-hour clock. For sums less than 24, the sum is already a valid hour. There is no need to handle negative numbers or invalid hour ranges because the constraints guarantee valid inputs. Time complexity is O(1) — only a constant number of arithmetic operations are performed. Space complexity is O(1) — no additional data structures are used. The implementation is trivial and robust for all valid inputs.
#include <algorithm> // for std::max (not strictly needed, but for clarity)

// Return the actual arrival hour on a 24-hour clock given a scheduled
// arrival time (0-23) and a non-negative delay in hours.
int findDelayedArrivalTime(int arrivalTime, int delayedTime) {
    const int totalTime = arrivalTime + delayedTime;
    // Wrap around if the sum exceeds 23, since the clock resets after midnight.
    return (totalTime >= 24) ? (totalTime - 24) : totalTime;
}
#include <cassert>

int main() {
    // Basic cases with no wrap-around
    assert(findDelayedArrivalTime(0, 0) == 0);
    assert(findDelayedArrivalTime(10, 5) == 15);
    assert(findDelayedArrivalTime(23, 0) == 23);
    
    // Wrap-around cases
    assert(findDelayedArrivalTime(20, 5) == 1);   // 25 -> 1
    assert(findDelayedArrivalTime(23, 1) == 0);   // 24 -> 0
    assert(findDelayedArrivalTime(23, 24) == 23); // 47 -> 23 (delay of full day)
    
    // Edge cases with sum exactly 24 or greater
    assert(findDelayedArrivalTime(12, 12) == 0);
    assert(findDelayedArrivalTime(0, 24) == 0);
    assert(findDelayedArrivalTime(18, 7) == 1);
    
    // Larger delay ensuring multiple days (but still wraps once)
    assert(findDelayedArrivalTime(5, 30) == 11); // 35 -> 11
}
