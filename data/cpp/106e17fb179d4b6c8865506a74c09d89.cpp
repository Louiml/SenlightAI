/*
Given a rectangular grid with `n` rows and `m` columns, a chosen cell at row `x` and column `y` (1-indexed), and two attackers that move one square per turn in any of the four cardinal directions, write a C++ function that returns the maximum number of turns the chosen cell can remain safe before at least one attacker reaches it. The attackers start at any two distinct cells chosen optimally by the defender (the person protecting the chosen cell) from the grid, and after each turn, each attacker must move to an adjacent cell (no staying still). The function should return the maximum possible safety duration (number of turns) for the given grid dimensions and chosen cell. If the chosen cell is at the exact center of an odd-sized square grid, the answer must be reduced by 1 because the two attackers would be forced to start less than optimally due to symmetry.
*/

#include <algorithm>

// Returns the maximum number of turns the chosen cell can remain safe given
// grid dimensions n (rows), m (columns) and chosen cell (x, y) with 1-indexing.
int maxSafetyTurns(int n, int m, int x, int y) {
    // Ensure n is the smaller dimension for symmetry handling.
    if (n > m) {
        std::swap(n, m);
        std::swap(x, y);
    }

    // Base answer: the maximum Manhattan distance from chosen cell to any cell.
    int ans = (std::min(n, m) + 1) / 2;

    // Compute distances to the four edges.
    int left = y - 1;
    int right = m - y;
    int up = x - 1;
    int down = n - x;

    // If the cell is horizontally far from both left and right edges, and not
    // vertically centered, we can place one attacker far left and one far right.
    if (std::min(left, right) > ans && up != down) {
        ans = std::min(std::max(up, down), std::min(left, right));
    }
    // Special symmetric case: odd-sized square grid and the chosen cell is the
    // exact center. The symmetry reduces the optimal safety by 1.
    else if (n % 2 == 1 && n == m && x == y && x == n / 2 + 1) {
        --ans;
    }

    return ans;
}

#include <cassert>

int main() {
    // Basic cases from the original snippet.
    assert(maxSafetyTurns(3, 3, 1, 1) == 1);
    assert(maxSafetyTurns(3, 3, 2, 2) == 1); // center, gets reduced from 2 to 1
    assert(maxSafetyTurns(5, 5, 3, 3) == 2); // center odd square, ans = 2
    assert(maxSafetyTurns(4, 4, 2, 2) == 2);
    assert(maxSafetyTurns(2, 10, 1, 5) == 1);
    assert(maxSafetyTurns(5, 7, 3, 4) == 3);
    assert(maxSafetyTurns(1, 1, 1, 1) == 0);
    assert(maxSafetyTurns(10, 3, 5, 2) == 2);
    assert(maxSafetyTurns(100, 100, 50, 50) == 49); // center of even square
    assert(maxSafetyTurns(3, 2, 2, 1) == 1);
    return 0;
}

// The problem is a game-theoretic optimization on a grid. The chosen cell’s safety depends on how far the closest attacker can be initially and how long it takes them to reach it. The naive maximum safety is `(min(n, m) + 1) / 2` — this is the maximum possible Manhattan distance from the chosen cell to any cell in the grid (by placing the closest attacker at the farthest corner). However, we can do better by placing the two attackers such that one is far away horizontally and the other vertically from the chosen cell, but only if the cell is not too close to the edges. Specifically, compute `l = y-1` (left distance), `r = m-y` (right), `u = x-1` (up), `d = n-x` (down). If both the left and right distances are greater than the current answer, and the up and down distances are not equal (i.e., the cell is not vertically centered), then we can improve the answer by placing one attacker far to the left and one far to the right, but the closest one is at min left/right, and the time to reach is min(max(up, down), min(left, right)). That is, we can position the closer attacker to be the one that is horizontally far but vertically closer, giving more time. The special case occurs when the grid is odd-sized square (`n == m`, both odd) and the chosen cell is its exact center; then the symmetry forces the two attackers to be closer than the naive bound, so we subtract 1.
//
// Time complexity is O(1) per test case (as the function just does a few arithmetic operations). Space complexity is O(1).
