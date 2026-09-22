Given two distinct positive integers `a` and `b`, write a C++ function `minMovesToEqual` that returns the minimum number of moves required to make `a` and `b` equal. In one move, you can either increase or decrease either number by 1, or you can increase or decrease either number by 2. However, you cannot mix different step sizes in a single move (i.e., each move changes one number by exactly 1 or exactly 2). The function should return an integer: 0 if the numbers are already equal, 1 if a single move of size 1 or 2 can make them equal, and 2 otherwise. The solution must be O(1) time and O(1) space.

#include <cassert>

int main() {
    assert(minMovesToEqual(5, 5) == 0);
    assert(minMovesToEqual(1, 2) == 1);
    assert(minMovesToEqual(2, 4) == 1);
    assert(minMovesToEqual(3, 5) == 1);
    assert(minMovesToEqual(1, 4) == 2);
    assert(minMovesToEqual(10, 15) == 2);  // diff=5 → two moves: 2 then 1
    assert(minMovesToEqual(100, 102) == 1);
    assert(minMovesToEqual(7, 9) == 1);    // diff=2
    assert(minMovesToEqual(7, 8) == 1);    // diff=1
    assert(minMovesToEqual(20, 23) == 2);  // diff=3 → two moves: 2 then 1
    return 0;
}

#include <cstdlib>  // for abs

// Returns the minimum number of moves (each changing one number by 1 or 2) 
// needed to make a and b equal.
int minMovesToEqual(int a, int b) {
    int diff = std::abs(a - b);
    if (diff == 0) return 0;
    if (diff == 1 || diff == 2) return 1;
    return 2;
}

// The problem reduces to determining the minimum number of moves needed to close the absolute difference `d = |a - b|`. Since each move can change one number by either 1 or 2, the minimum moves is: 0 if `d == 0`; 1 if `d == 1` or `d == 2` (because a single move of size 1 or 2 directly covers that difference); otherwise, we need 2 moves. But careful: if `d` is odd and greater than 2, can we do it in 2? Yes, because we can use a move of size 2 followed by a move of size 1 (or vice versa) to cover any odd difference ≥3. For even differences ≥4, two moves of size 2 suffice. Thus, the answer is simply: if `d == 0` → 0; if `d == 1` or `d == 2` → 1; otherwise → 2. Alternatively, a compact formula: `(d == 0) ? 0 : (d % 2 == 0 ? (d == 2 ? 1 : 2) : (d == 1 ? 1 : 2))`. Edge cases: `a` and `b` are positive integers, but the logic works for any integers; no overflow concerns since difference is within int range typical for the task. Time complexity: O(1). Space complexity: O(1).
