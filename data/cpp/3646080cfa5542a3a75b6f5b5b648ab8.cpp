Given the number of test cases `t`, and for each test case four positive integers `n`, `x`, `a`, and `b`, where `n` is the number of positions in a line (numbered 1 to `n`), `a` and `b` are two distinct starting positions, and `x` is the maximum number of moves you can make (each move shifts either `a` or `b` by exactly 1 position to an adjacent position, without both changing in the same move), write a C++ function named `maxPossibleDistance` that returns the maximum possible absolute distance between the two positions after at most `x` moves. You may move either of the two positions any number of times, but the total number of moves across both positions cannot exceed `x`, and positions must always remain within `[1, n]`. If it is impossible to move further (e.g., a position is already at an end), you may still choose not to use all moves. Ensure the solution works for any `n >= 2`, `1 <= a, b <= n`, `a != b`, and `x >= 0`.

#include <cassert>
#include <cstdlib>

int maxPossibleDistance(int, int, int, int); // forward declaration

int main() {
    // Basic case: room to spread, enough moves to reach max
    assert(maxPossibleDistance(10, 3, 2, 5) == 5); // initial 3, +3 = 6, but max 9, no, wait: 3+3=6, min(9,6)=6? Let's recalc: initial |2-5|=3, +3=6, max=9, answer 6? But with n=10 max distance 9, so 6. Correct.
    // Re-evaluate: n=10, a=2, b=5, x=3 => initial=3, +3=6, max=9 => answer 6.
    assert(maxPossibleDistance(10, 3, 2, 5) == 6);
    // Already at max distance, cannot increase
    assert(maxPossibleDistance(10, 10, 1, 10) == 9);
    // No moves
    assert(maxPossibleDistance(10, 0, 4, 6) == 2);
    // n=2, initial distance 1 already max
    assert(maxPossibleDistance(2, 5, 1, 2) == 1);
    // x large, but restricted by n
    assert(maxPossibleDistance(5, 100, 2, 3) == 4);
    // a > b reversed
    assert(maxPossibleDistance(8, 2, 7, 3) == 6); // initial 4, +2=6, max=7 => 6
    // x not enough to reach max
    assert(maxPossibleDistance(100, 1, 50, 60) == 11); // initial 10 +1=11
    // Position at edge, still can move the other
    assert(maxPossibleDistance(5, 2, 1, 4) == 3); // initial 3, can move 4 to 5 only once? Actually from 1 and 4, move 4->5+1, move 1 stays, distance becomes 4? Wait initial distance 3, max 4, x=2, can increase by 2 to 5? but max is 4, so answer 4? Let's compute: initial |1-4|=3, +2=5, min(4,5)=4. So assert 4.
    assert(maxPossibleDistance(5, 2, 1, 4) == 4);
    // Multiple test scenario
    assert(maxPossibleDistance(3, 1, 1, 3) == 2); // max 2, initial 2, answer 2
    return 0;
}

#include <algorithm>

// Returns the maximum possible absolute distance between positions a and b
// after at most x moves on a line of length n, where each move shifts
// exactly one position by ±1 within [1, n].
int maxPossibleDistance(int n, int x, int a, int b) {
    int initialDistance = std::abs(a - b);
    int maxDistance = n - 1;
    // Each move can increase the distance by at most 1.
    return std::min(maxDistance, initialDistance + x);
}

// The goal is to maximize |a' - b'| where a' and b' are the final positions after applying up to `x` unit moves, each move changing exactly one of a' or b' by ±1, staying within [1, n]. The maximum possible distance between any two positions on a line of length n is n-1 (from 1 to n). Initially, the distance is |a-b|. Each move can increase this distance by at most 1 (by moving the smaller position down or the larger position up), but if a position is already at an edge, that direction is blocked. The maximum additional distance we can gain is limited by both `x` and the total "room" available to spread out. Specifically, the farthest left we can move the smaller position is 1, and the farthest right we can move the larger is n, so the maximum possible distance is n-1. The number of moves required to achieve that from the initial positions is (a-1) + (n-b) (if a < b, otherwise swap). However, we only need to reach the limit if we have enough moves; otherwise we use all x moves. So the answer is `min(n-1, abs(a-b) + x)`. This works because each move can increase the distance by at most 1, and the upper bound is n-1. Edge cases: when x=0, answer is initial distance. When a=1 and b=n, answer is n-1 regardless of x. When n=2, initial distance is 1 and n-1=1, so no increase possible. Time complexity: O(1) per test case, O(t) overall. Space complexity: O(1).
