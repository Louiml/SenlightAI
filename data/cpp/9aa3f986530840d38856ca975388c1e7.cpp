// Write a C++ function `int shortestChessPath(int x1, int y1, int x2, int y2)` that computes the minimum number of moves a king (which moves one square in any direction, including diagonally) needs to travel on an infinite chessboard from square `(x1, y1)` to square `(x2, y2)`. The board has no obstacles, and coordinates can be any integers (including negative values). The function must return the minimum number of king moves. Input coordinates are read from standard input in the original problem, but your function should just take the four integers as parameters and return the answer. Assume the function will be called with valid integers (no overflow concerns for typical 32-bit ints).

A king can move one step horizontally, vertically, or diagonally. Therefore, in a single move it changes both x and y coordinates by at most 1, and it can change either one or both. The minimum number of moves is exactly the maximum of the absolute differences in x and y coordinates: `max(|x1-x2|, |y1-y2|)`.  
Why? Because in each move, the king can reduce the larger coordinate difference by at most 1, and it can also reduce the smaller difference by at most 1 (if both need changing). The smaller difference will always be covered “for free” while fixing the larger one, since diagonal moves fix both at once. Thus the limiting factor is the larger difference. Edge cases: if both differences are 0, answer is 0. If one is 0, answer is the other difference. Negative coordinates are handled by using absolute differences. The solution is O(1) time and O(1) space.

#include <cstdlib> // for std::abs

// Returns the minimum number of king moves on an infinite board.
// A king moves one square in any of 8 directions.
int shortestChessPath(int x1, int y1, int x2, int y2) {
    const int dx = std::abs(x1 - x2);
    const int dy = std::abs(y1 - y2);
    return (dx > dy) ? dx : dy;
}

#include <cassert>

int main() {
    // Same square: 0 moves
    assert(shortestChessPath(0, 0, 0, 0) == 0);
    // Straight horizontal: 5 moves
    assert(shortestChessPath(0, 0, 5, 0) == 5);
    // Straight vertical: 3 moves
    assert(shortestChessPath(2, 2, 2, -1) == 3);
    // Diagonal: 4 moves
    assert(shortestChessPath(0, 0, 4, 4) == 4);
    // Unequal differences: larger dominates
    assert(shortestChessPath(0, 0, 4, 2) == 4);
    assert(shortestChessPath(0, 0, -3, -7) == 7);
    // One coordinate same, other large
    assert(shortestChessPath(10, 10, 10, -5) == 15);
    // Negative and positive mix
    assert(shortestChessPath(-1, -1, 2, 3) == 4);
    // Large values
    assert(shortestChessPath(-1000000, 1000000, 1000000, -1000000) == 2000000);
    // Both differences equal
    assert(shortestChessPath(0, 0, 3, 3) == 3);
    return 0;
}
