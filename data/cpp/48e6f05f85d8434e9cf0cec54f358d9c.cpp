/*
You are given a sequence of \( N \) boxes numbered 1 to \( N \), each initially containing exactly one red ball. A red ball's position is tracked: initially, the ball is in box 1. You are given \( M \) operations, each described by two integers \( x \) and \( y \). In each operation, one ball is moved from box \( x \) to box \( y \) (guaranteed that box \( x \) has at least one ball). It is possible that the ball being moved is the red ball; if the red ball is in box \( x \) and box \( x \) loses its last ball during the operation, the red ball moves to box \( y \). However, if box \( x \) retains at least one ball after the move, the red ball stays in box \( x \). More precisely, after each operation, the red ball's location is updated as follows: if the red ball was in box \( x \) before the move, and after the move box \( x \) becomes empty, then the red ball is now in box \( y \); otherwise, the red ball's position does not change. Write a C++ function `int countPossiblePositions(int N, const std::vector<std::pair<int,int>>& operations)` that processes all operations and returns the number of boxes that could possibly contain the red ball at the end. Note that during the process, the red ball is never split, and only one ball is moved per operation—the identity of which ball is moved is not given to you; you only know the source and destination boxes. A box is "possible" if there exists at least one sequence of ball choices (respecting the rule that the red ball is moved only when its current box becomes empty) that results in the red ball ending in that box. All given operations are valid (box \( x \) has at least one ball at that time). The input size satisfies \( 1 \le N \le 10^5 \), \( 0 \le M \le 2\times 10^5 \). Provide your solution as a standalone function.
*/

#include <vector>
#include <cstdint>

// Returns the number of boxes that could contain the red ball after all operations.
// operations: each pair (x, y) means one ball moves from box x to box y.
int countPossiblePositions(int N, const std::vector<std::pair<int,int>>& operations) {
    // 1-indexed arrays for ball counts and possibility flags
    std::vector<int> balls(N + 1, 1);
    std::vector<bool> possible(N + 1, false);
    possible[1] = true;

    for (const auto& op : operations) {
        int x = op.first;
        int y = op.second;

        // Move one ball from x to y
        --balls[x];
        ++balls[y];

        // If red could be in x and x becomes empty, red must go to y
        if (possible[x] && balls[x] == 0) {
            possible[y] = true;
            possible[x] = false;
        }
        // If red was possible in x and balls remain, it stays in x (already true)
        // If red was not possible in x, it cannot be there now either.
        // No other changes needed.
    }

    int ans = 0;
    for (int i = 1; i <= N; ++i) {
        if (possible[i]) ++ans;
    }
    return ans;
}

#include <cassert>
#include <vector>
#include <utility>

// Declaration of the solution function (as defined above)
int countPossiblePositions(int N, const std::vector<std::pair<int,int>>& operations);

int main() {
    // Test 1: Simple move from 1 to 2, only one ball in 1, so red moves to 2
    assert(countPossiblePositions(3, {{1,2}}) == 1);

    // Test 2: Move from 1 to 2, but also add an extra ball to 1 first
    // Start: each box has 1. Operation: (2,1) -> box2 becomes 0, box1 becomes 2.
    // Red is in box1, box1 not empty, so red stays in 1.
    // Then operation (1,3) -> box1 becomes 1 (still not empty), red stays in 1.
    // Final possible: only box1.
    assert(countPossiblePositions(3, {{2,1},{1,3}}) == 1);

    // Test 3: Red moves through multiple boxes
    // (1,2): box1 empty, red ->2
    // (2,3): box2 empty (only red there), red ->3
    // (3,1): box3 empty, red ->1
    // Final: only box1 possible
    assert(countPossiblePositions(3, {{1,2},{2,3},{3,1}}) == 1);

    // Test 4: Multiple possible positions when extra balls prevent forced movement
    // Start: all boxes 1. Op1: (1,2) -> box1=0, red moves to2. box2=2.
    // Op2: (2,1) -> box2=1 (still has red), box1=1. Red stays in 2.
    // Op3: (2,3) -> box2=0, red moves to3. box3=2.
    // Final: only box3 possible.
    assert(countPossiblePositions(3, {{1,2},{2,1},{2,3}}) == 1);

    // Test 5: Ambiguity – red might be in one of two boxes
    // Start: all boxes 1. Op1: (1,2) -> box1=0, red moves to2. box2=2.
    // Op2: (2,3) -> box2=1 (still has red), box3=2. Red stays in2.
    // Op3: (3,2) -> box3=1, box2=2. Red still in2.
    // Final: only box2 possible.
    assert(countPossiblePositions(3, {{1,2},{2,3},{3,2}}) == 1);

    // Test 6: No operations -> only box1
    assert(countPossiblePositions(5, {}) == 1);

    // Test 7: Two possible boxes if red could be in either due to non-emptying moves
    // Start: all 1. Op1: (1,2) -> box1=0, red->2. box2=2.
    // Op2: (2,1) -> box2=1, box1=1. Red stays in2.
    // Op3: (2,3) -> box2=0, red->3. box3=2.
    // Op4: (3,2) -> box3=1, box2=1. Red stays in3.
    // Final: only box3 possible.
    assert(countPossiblePositions(4, {{1,2},{2,1},{2,3},{3,2}}) == 1);

    // Test 8: Larger N with no moves to box1, red stuck in box1
    int N = 10;
    std::vector<std::pair<int,int>> ops;
    for (int i = 2; i <= N; ++i) {
        ops.push_back({i, 1}); // move from i to 1, but box i had only 1 ball, so box i becomes 0, but red is in 1, unaffected.
    }
    assert(countPossiblePositions(N, ops) == 1);

    // Test 9: Red moves away then returns, possible only in final box
    assert(countPossiblePositions(2, {{1,2},{2,1}}) == 1);

    // Test 10: Multiple possible positions when red can stay in a box that has extra balls
    // Start: all 1. Op1: (1,2) -> box1=0, red->2, box2=2.
    // Op2: (2,1) -> box2=1, box1=1. Red stays in2.
    // Op3: (2,1) -> box2=0, red->1, box1=2.
    // Op4: (1,2) -> box1=1, box2=1. Red stays in1.
    // Final: only box1.
    assert(countPossiblePositions(2, {{1,2},{2,1},{2,1},{1,2}}) == 1);

    // Additional test to show that two boxes can be possible
    // Start: all 1. Op1: (1,2) -> box1=0, red->2, box2=2.
    // Op2: (2,3) -> box2=1, box3=2. Red stays in2.
    // Op3: (3,2) -> box3=1, box2=2. Red stays in2.
    // Op4: (2,1) -> box2=1, box1=1. Red stays in2.
    // Now, if we do (2,4) -> box2 becomes 0? No, box2=1, so red stays.
    // Let's design a case with two possible: Need a situation where red could be in x or y because moving from x to y does not empty x.
    // Start: box1 has 1, box2 has 1, box3 has 1.
    // Op1: (1,2) -> box1=0, red->2, box2=2. possible: {2}
    // Op2: (2,1) -> box2=1, box1=1. red in2. possible {2}
    // Op3: (3,2) -> box3=0, box2=2. red in2. possible {2}
    // Op4: (2,3) -> box2=1, box3=1. red in2. possible {2}
    // No ambiguity. To get ambiguity, need red to be in a box that is not forced.
    // Actually, ambiguity happens when a box with multiple balls is possible and a move from it does not empty it.
    // Example: start box1 has 1, box2 has 1. 
    // Op1: (1,2) -> box1=0, red->2, box2=2. possible {2}
    // Op2: (2,1) -> box2=1, box1=1. possible {2}
    // Op3: (1,2) -> box1=0, red->2, box2=2. possible {2}
    // Still only one.
    // To have two, we need an operation that could move the red or not: e.g., box x has 2 balls, one is red. Moving one ball from x to y: if we move the red, x becomes 1, red goes to y. If we move the other, red stays in x. Both are possible. So after such an operation, both x and y are possible. So we create this:
    // Start: box1 has 1, box2 has 1.
    // Op1: (1,2) -> box1=0, red->2, box2=2. possible {2}
    // Op2: (2,1) -> box2=1, box1=1. possible {2} (still only 2)
    // Op3: (1,2) -> box1=0, red->2, box2=2. possible {2}
    // Now box2 has 2. Op4: (2,3) -> box2 becomes 1. If red was in box2, it stays (box not empty). So red stays in 2. But if we moved the red? No, we moved one ball from box2 to box3. If we moved the red, then red goes to 3, but box2 would become 1 (still has the other ball). That is allowed. So after op4, both box2 and box3 are possible. Let's test:
    // countPossiblePositions(3, { {1,2}, {2,1}, {1,2}, {2,3} }) -> should be 2.
    assert(countPossiblePositions(3, {{1,2},{2,1},{1,2},{2,3}}) == 2);

    return 0;
}

// The key is to track, for each box, two pieces of information: the current number of balls in that box (initialized to 1 for every box), and a boolean indicating whether the red ball *could* be in that box. Initially, only box 1 is possible. For each operation `(x, y)`, we decrease the ball count of box `x` and increase that of box `y`. Then we update the possibility flags: any box that was possible before and is not `x` remains possible. For box `x`, if it was possible and its new ball count is zero, then the red ball must have moved to `y`, so `y` becomes possible. If box `x` was possible but still has at least one ball after the move, then `x` remains possible (the moved ball was not necessarily the red one). Also, if `x` was not possible before, nothing changes for `x` or `y` except the counts. Note that a box can become possible only via the `y` side when it is the destination of a move that empties `x` while `x` was possible. Since the red ball is unique, the possibility set always contains at least one box. The answer is the number of `true` flags at the end. This is a straightforward simulation. Edge cases: `M=0` returns 1 (only box 1). Also, if multiple moves happen, a box that becomes possible later can later be emptied, moving the red ball further. The algorithm uses an array of size `N+1` for counts and a vector of booleans for possible flags. Time complexity is \(O(N+M)\), space \(O(N)\).
