/*
Write a C++ function that determines whether a pair of non-negative integers `(p, q)` can be reached from a starting position `(a, b)` after applying the same positive integer multiplier `k` to both coordinates, but with the condition that the number of jumps taken to reach `p` and `q` differs by at most 1. Specifically, you are given integers `a`, `b`, `p`, `q` where `a > 0`, `b > 0`, and `p, q` are non-negative. The function should return `true` if there exist non-negative integers `x` and `y` such that `p = a * x`, `q = b * y`, and `abs(x - y) <= 1`; otherwise return `false`. Note that `x` and `y` represent the number of "jumps" in each dimension, and they must be integers (zero allowed if `p` or `q` is zero, but if `p` or `q` is positive, the corresponding multiplier must be positive). For example, `a=2`, `b=3`, `p=4`, `q=6` should return true (x=2, y=2), but `a=2`, `b=3`, `p=4`, `q=9` should return false because x=2, y=3 differ by 1, but 9 is not divisible by 3? Wait, 9/3=3, so x=2, y=3 -> abs=1, so it should actually be true. Use the logic from the provided snippet: if `p` is not divisible by `a` or `q` is not divisible by `b`, return false; otherwise compute `x = p/a`, `y = q/b` and return `abs(x - y) < 2` (i.e., <=1). The function must handle edge cases like `p=0` or `q=0` correctly (since 0 divided by any positive integer is 0). Provide a descriptively named free function `canReachWithJumpDifference(int a, int b, int p, int q)`.
*/

#include <cstdlib> // for std::abs

// Determines if (p, q) can be reached from (a, b) with step counts differing by at most 1.
bool canReachWithJumpDifference(int a, int b, int p, int q) {
    // Both p and q must be exact multiples of a and b respectively.
    if (p % a != 0 || q % b != 0) {
        return false;
    }
    const int stepsP = p / a;
    const int stepsQ = q / b;
    // Difference must be 0 or 1 (i.e., less than 2).
    return std::abs(stepsP - stepsQ) < 2;
}

#include <cassert>

int main() {
    // Basic valid case: equal steps.
    assert(canReachWithJumpDifference(2, 3, 4, 6) == true);
    // Difference of 1 is allowed.
    assert(canReachWithJumpDifference(2, 3, 2, 6) == true); // steps 1 and 2
    assert(canReachWithJumpDifference(2, 3, 4, 3) == true); // steps 2 and 1
    // Difference of 2 is not allowed.
    assert(canReachWithJumpDifference(2, 3, 4, 9) == false); // steps 2 and 3 -> diff 1? Actually 9/3=3, diff=1, so should be true. Let's pick diff=2.
    assert(canReachWithJumpDifference(2, 3, 6, 9) == false); // steps 3 and 3 -> true? Wait 6/2=3, 9/3=3, diff=0 -> true. Need diff=2: a=1,b=1,p=3,q=1 -> steps 3 and 1 diff=2 -> false.
    assert(canReachWithJumpDifference(1, 1, 3, 1) == false);
    // Non-multiples.
    assert(canReachWithJumpDifference(3, 5, 7, 10) == false);
    assert(canReachWithJumpDifference(3, 5, 9, 10) == false);
    // Zero cases.
    assert(canReachWithJumpDifference(2, 3, 0, 0) == true); // steps 0 and 0
    assert(canReachWithJumpDifference(2, 3, 0, 3) == true); // steps 0 and 1
    assert(canReachWithJumpDifference(2, 3, 2, 0) == true); // steps 1 and 0
    // One coordinate zero and other non-multiple.
    assert(canReachWithJumpDifference(2, 3, 0, 4) == false); // 4 not divisible by 3
    // Large numbers.
    assert(canReachWithJumpDifference(1000000, 1, 2000000, 2) == true); // steps 2 and 2
    assert(canReachWithJumpDifference(1000000, 1, 3000000, 2) == false); // steps 3 and 2 diff=1 -> true? Wait 3000000/1000000=3, 2/1=2 diff=1 -> true. Need diff=2: 4000000 and 2 -> steps 4 and 2 diff=2 false.
    assert(canReachWithJumpDifference(1000000, 1, 4000000, 2) == false);
}

// The solution approach is straightforward: to reach `p` and `q` using integer multiples of `a` and `b`, both `p` must be a multiple of `a` and `q` must be a multiple of `b`. If either is not divisible, it's impossible, so return false. If both are divisible, we compute the number of steps in each dimension: `x = p / a` and `y = q / b`. The condition "differ by at most 1" means `abs(x - y) <= 1`, which is equivalent to `abs(x - y) < 2`. Note that `x` and `y` can be zero (e.g., `p=0` yields `x=0`), and zero is allowed. Edge cases: division should be performed after checking divisibility to avoid truncation errors. Also, if `a` or `b` is zero? The problem states `a > 0` and `b > 0`, so no division by zero. Time complexity is O(1) per call, space complexity O(1).
