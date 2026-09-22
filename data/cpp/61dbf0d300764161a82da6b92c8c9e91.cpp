/*
Write a C++ function named `minimumSteps` that takes a single positive integer `x` (with `1 ≤ x ≤ 1,000,000`) representing the distance from an elephant's home (point 0) to its friend's home (point x) on a coordinate line. The elephant can move exactly 1, 2, 3, 4, or 5 positions forward in one step. The function must return the minimum number of steps required to reach exactly position `x`. You may assume the input is always a valid positive integer within the given range, so no error handling for invalid input is required. The solution must be self-contained and avoid using any global variables.
*/

#include <cstdint>

// Returns the minimum number of steps for an elephant to travel distance x,
// where each step can be of length 1, 2, 3, 4, or 5.
int minimumSteps(int x) {
    // The minimum number of steps is the ceiling of x / 5.
    // Using integer arithmetic: (x + 4) / 5 gives ceil(x/5) for positive x.
    return (x + 4) / 5;
}

#include <cassert>

// The solution function is declared above; here we test it directly.
int main() {
    assert(minimumSteps(1) == 1);          // one step of size 1
    assert(minimumSteps(5) == 1);          // one step of size 5
    assert(minimumSteps(6) == 2);          // 5 + 1
    assert(minimumSteps(9) == 2);          // 5 + 4
    assert(minimumSteps(10) == 2);         // two steps of size 5
    assert(minimumSteps(14) == 3);         // 5 + 5 + 4
    assert(minimumSteps(100) == 20);       // exactly 20 steps of size 5
    assert(minimumSteps(101) == 21);       // 20 steps of 5 + 1
    assert(minimumSteps(999999) == 200000); // (999999 + 4)/5 = 200000
    assert(minimumSteps(1000000) == 200000); // exactly divisible
    return 0;
}

// The problem reduces to finding the minimum number of terms (each from the set {1,2,3,4,5}) that sum exactly to `x`. The optimal strategy is to use as many steps of size 5 as possible, because larger steps reduce the step count. The minimum number of steps is simply `ceil(x / 5)`. This is because after taking `floor(x / 5)` steps of size 5, the remainder `r` (where `0 ≤ r < 5`) can be covered in either 0 steps if `r == 0`, or 1 additional step of size `r` (since `r` is between 1 and 4, which is allowed). Thus the answer is:
// - If `x % 5 == 0`, then exactly `x / 5` steps.
// - Otherwise, `x / 5 + 1` steps.
// This can be expressed compactly as `(x + 4) / 5` using integer division. Edge cases: `x = 1` gives 1 step (size 1), `x = 5` gives 1 step (size 5), and `x = 1000000` gives 200000 steps. The algorithm runs in O(1) time and uses O(1) auxiliary space, since it only performs a few arithmetic operations.
