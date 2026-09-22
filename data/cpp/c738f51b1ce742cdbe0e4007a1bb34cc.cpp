Write a C++ function `canAchieveScore(int x, int t, int a, int b, int da, int db)` that determines whether a contestant can achieve exactly `x` points by solving at most one problem of type A and at most one problem of type B. A problem of type A initially gives `a` points, but each minute that passes reduces its value by `da` points (so if solved at minute `k`, it gives `a - da*k` points). Similarly, type B initially gives `b` points and decreases by `db` per minute. The contest lasts exactly `t` minutes, meaning each problem can be solved at any integer minute from 0 to `t` inclusive. The contestant may also choose to solve no problem at all, or only one problem, or both problems (each problem at most once). The function should return `true` if there exists a time (possibly different for each problem) such that the sum of the points from the chosen problems equals exactly `x`, and `false` otherwise.

The solution is a brute‑force enumeration over all possible solve times. For each problem, we consider all `t+1` possible minutes (0 through t) plus the option of not solving it at all, which we represent by time `-1`. The total gain for a pair of choices is computed as the sum of the points from each problem if solved, or zero if not solved. We check every combination of these `(t+2)` options for problem A and `(t+2)` options for problem B, giving `(t+2)^2` iterations. If any combination yields exactly `x`, we return true; otherwise false. Important edge cases: `x` could be zero (solved by choosing both problems not solved), `t` could be zero (only minute 0 allowed), and the point values after decrementing could become negative (but that's fine because we only compare the sum to `x`). Also note that `a - da*t1` might be negative if `da` is large, but that's allowed by the problem statement because the sum must equal `x` exactly; negative contributions are possible. The time complexity is `O(t^2)`, and space complexity is `O(1)`.

#include <vector>
#include <algorithm>

// Determine if exactly x points can be achieved by solving at most one
// problem of type A and at most one of type B within the time 0..t.
// A problem solved at minute k yields (initial - rate * k) points.
// Passing -1 for a problem means it is not solved.
bool canAchieveScore(int x, int t, int a, int b, int da, int db) {
    // Enumerate all possible minutes for each problem, plus the "not solved" option.
    for (int t1 = -1; t1 <= t; ++t1) {
        for (int t2 = -1; t2 <= t; ++t2) {
            int gain = 0;
            if (t1 != -1) gain += a - da * t1;
            if (t2 != -1) gain += b - db * t2;
            if (gain == x) {
                return true;
            }
        }
    }
    return false;
}

int main() {
    // x=0 can always be achieved by solving nothing.
    assert(canAchieveScore(0, 5, 10, 10, 1, 1) == true);
    
    // Example from typical problem: x=5, t=2, a=10, b=10, da=3, db=3
    // Possible gains: 0, 7, 4, 10, 7, 4, 8, 5, 2, ... 
    assert(canAchieveScore(5, 2, 10, 10, 3, 3) == true);
    
    // Impossible case: x=1, t=0, only minute 0 available, gains 0, 5, 6, 11.
    assert(canAchieveScore(1, 0, 5, 6, 0, 0) == false);
    
    // At t=0 only minute 0 available, both problems solved gives 5+6=11.
    assert(canAchieveScore(11, 0, 5, 6, 0, 0) == true);
    
    // Negative gains possible: a=2, da=5, at minute 1 gives -3.
    assert(canAchieveScore(-3, 1, 2, 3, 5, 0) == true); // solve A at minute 1, skip B.
    
    // Large t, only one problem needed.
    assert(canAchieveScore(7, 10, 7, 100, 1, 1) == true); // solve A at minute 0.
    
    // Both problems can be solved at different times.
    assert(canAchieveScore(6, 3, 5, 5, 1, 2) == true); // A at 0 (5), B at 2 (1) => 6.
    
    // x not achievable.
    assert(canAchieveScore(3, 2, 2, 2, 0, 0) == false); // gains are 0,2,4.
    
    // t can be zero, x=0 works.
    assert(canAchieveScore(0, 0, 1, 2, 3, 4) == true);
    
    // Both not solved gives 0 always.
    assert(canAchieveScore(-1, 3, 0, 0, 0, 0) == false); // gain always 0.
    
    return 0;
}
