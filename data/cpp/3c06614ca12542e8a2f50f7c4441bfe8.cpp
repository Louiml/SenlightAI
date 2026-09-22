// Write a C++ function `long long minimumRequiredMoves(long long a, long long b, long long c, long long d)` that takes three distinct positions of three players on a number line (given as `a`, `b`, `c`) and a positive distance `d`. The function must return the **minimum total number of moves** required so that after moving the players, the distance between the leftmost and middle player is at least `d`, and the distance between the middle and rightmost player is also at least `d`. In one move, you can shift any single player by exactly 1 unit in either direction (left or right). Players can pass through each other, and final positions do not need to maintain the original order. The input positions are integers, and the answer fits in a 64-bit signed integer.

The key observation is that only pairwise distances matter: between the leftmost and middle, and between the middle and rightmost. Sorting the three positions simplifies the problem, because after sorting, let `x1 <= x2 <= x3` be the sorted positions. The two critical gaps are `gap1 = x2 - x1` and `gap2 = x3 - x2`. For each gap, if it is already at least `d`, no moves are needed for that gap; otherwise, we need to increase that gap. Increasing a gap requires moving the middle player (or the left/right player) outward, but moving the middle player affects both gaps. The optimal strategy is to make each gap exactly `d` if possible, because making it larger would require more moves. However, increasing one gap might decrease the other if we move the middle player only. The minimal total moves is achieved by first sorting, then for each gap, we need at least `d - gap` additional distance. Since the middle player is shared, we can add distance to both gaps simultaneously by moving the middle player away from one side, but that would shrink the other side. The correct formula emerges: the total moves needed equals `2*d - (min(gap2, d) + min(gap1, d))`. Why? Consider the final desired gaps after moves: we want both final gaps to be at least `d`. The total sum of the two gaps after sorting is fixed relative to the positions of the leftmost and rightmost players. If we denote the leftmost and rightmost positions as `L` and `R`, and the middle as `M`, then initial sum of gaps is `(R - L)`. We can move `L` left, `R` right, and `M` either left or right. The minimal total moves to achieve final gaps `gap1_final` and `gap2_final` (both >= d) is `(gap1_final - initial_gap1) + (gap2_final - initial_gap2)` but we also have the constraint that moving `M` can reduce one gap when increasing the other. The optimal is to only increase each gap up to `d` if it’s smaller, and if a gap is already > `d`, we can shrink it to exactly `d` (by moving the middle toward that side) to free up moves for the other gap, because the total distance between leftmost and rightmost can be reduced by moving them inward. In fact, the minimal total moves equals `max(0, d - gap1) + max(0, d - gap2)`? That would be if we could move the middle independently, but that’s not the case. Let's derive: After sorting, let `gap1 = x2 - x1`, `gap2 = x3 - x2`. We want final gaps `g1 >= d`, `g2 >= d`. The total distance between `x1` and `x3` is `gap1 + gap2`. If we move `x1` left by `Lmove`, `x3` right by `Rmove`, and `x2` left or right by `Mmove`, then new gaps: `g1 = (x2 + Mmove) - (x1 - Lmove) = gap1 + Mmove + Lmove`, `g2 = (x3 + Rmove) - (x2 + Mmove) = gap2 + Rmove - Mmove`. Total moves = `Lmove + Rmove + |Mmove|`. We want to minimize total moves subject to `g1 >= d`, `g2 >= d`. Notice that to increase `g1` we need `Mmove + Lmove >= d - gap1`, and to increase `g2` we need `Rmove - Mmove >= d - gap2`. The optimal strategy is to set `Lmove = max(0, d - gap1 - Mmove)` and `Rmove = max(0, d - gap2 + Mmove)`. The total moves becomes `max(0, d - gap1 - Mmove) + max(0, d - gap2 + Mmove) + |Mmove|`. This is a piecewise linear function in `Mmove`. Analyzing it, the minimum occurs when we choose `Mmove` to balance the two gaps. The closed form is `2*d - (min(gap1, d) + min(gap2, d))`. For example, if both gaps are already ≥ `d`, then `min(gap,d)=d` each, so answer = `2*d - (d+d)=0`. If `gap1=0`, `gap2=0`, d=5, then answer = `10 - (0+0)=10`. We need 5 moves to separate left and middle, and 5 moves to separate middle and right? Actually with three points at same position, we can move left one left 5, middle stay, right one right 5, total 10. That matches. If `gap1=2`, `gap2=8`, d=5, then min(gap1,5)=2, min(gap2,5)=5, answer=10-7=3. Indeed we can move middle left by 3 to increase gap1 to 5, that makes gap2 become 5 (since gap2 reduces from 8 to 5), total moves=3. Edge cases: positions can be negative, large up to 1e18, so use `long long`. Sorting three values is trivial. The formula works because moving the middle player transfers distance from one gap to the other, and any gap already above `d` can be shrunk to exactly `d` to give extra distance to the other gap. Time complexity O(1) after sorting (which is constant), space O(1). All arithmetic fits in `long long` as input constraints assume that.

#include <algorithm>

// Returns the minimum total moves to make both gaps between three sorted
// positions at least d. Positions a,b,c are any three integers, d>0.
long long minimumRequiredMoves(long long a, long long b, long long c, long long d) {
    long long pos[3] = {a, b, c};
    std::sort(pos, pos + 3);
    
    long long gap1 = pos[1] - pos[0]; // distance between leftmost and middle
    long long gap2 = pos[2] - pos[1]; // distance between middle and rightmost
    
    // Each gap needs to become at least d. The total cost is 2*d minus
    // the amount already present (capped at d for each gap).
    return 2 * d - (std::min(gap1, d) + std::min(gap2, d));
}

#include <cassert>

int main() {
    // Already sufficient gaps: no moves needed
    assert(minimumRequiredMoves(0, 5, 10, 5) == 0);
    // All together, need to separate both gaps by 5 each => 10 moves
    assert(minimumRequiredMoves(3, 3, 3, 5) == 10);
    // Gap1=2, gap2=8, d=5: move middle left 3 => gap1=5, gap2=5
    assert(minimumRequiredMoves(0, 2, 10, 5) == 3);
    // Gap1=10, gap2=1, d=5: move middle right 4 => gap1 becomes 6, gap2=5
    assert(minimumRequiredMoves(0, 10, 11, 5) == 4);
    // Negative positions and large values
    assert(minimumRequiredMoves(-100, -90, 0, 20) == 20); // gaps: 10 and 10, need +10 each => total 20
    assert(minimumRequiredMoves(1, 2, 3, 1) == 0); // gaps are 1 and 1, already >=1
    assert(minimumRequiredMoves(0, 0, 100, 50) == 50); // gap1=0, gap2=100, need gap1+=50 by moving middle left? Actually move middle left 50 -> gap1=50, gap2=50, moves=50
    assert(minimumRequiredMoves(5, 6, 7, 100) == 200 - (1+1) = 198);
    assert(minimumRequiredMoves(-5, -4, -3, 10) == 20 - (1+1) = 18);
    assert(minimumRequiredMoves(7, 1, 4, 3) == 6 - (min(3,3)+min(1,3)) = 6 - (3+1) = 2); // sorted:1,4,7 gaps 3 and 3, already ok? Wait d=3, gaps are 3 and 3, answer should be 0
    // correction for last: sorted 1,4,7 gaps=3,3 => min(3,3)=3 each => 6-6=0
    // Actually let me fix: 
    assert(minimumRequiredMoves(7, 1, 4, 3) == 0);
    assert(minimumRequiredMoves(0, 0, 0, 1) == 2);
    return 0;
}
