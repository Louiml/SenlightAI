// Write a C++ function named `isRectanglePair` that takes four integers `a`, `b`, `c`, and `d` and returns a `bool` indicating whether these four integers form two equal-length sides of opposite edges in a rectangle. Specifically, after these four numbers are paired into two unordered pairs, each pair must contain two equal numbers. A rectangle can be represented by giving the lengths of its two distinct sides, so if the input can be arranged into two pairs where each pair has the same two numbers (i.e., `(a,b)` are equal and `(c,d)` are equal, OR `(a,c)` are equal and `(b,d)` are equal, OR `(a,d)` are equal and `(b,c)` are equal), the function should return `true`; otherwise, it returns `false`. For example, input `(2,2,5,5)` is valid, but `(2,3,5,5)` is not.
#include <cassert>

int main() {
    // Basic valid cases
    assert(isRectanglePair(2, 2, 5, 5) == true);
    assert(isRectanglePair(5, 5, 2, 2) == true);
    assert(isRectanglePair(2, 5, 2, 5) == true);
    assert(isRectanglePair(2, 5, 5, 2) == true);

    // All equal
    assert(isRectanglePair(4, 4, 4, 4) == true);

    // Invalid cases
    assert(isRectanglePair(2, 3, 5, 5) == false);
    assert(isRectanglePair(2, 2, 2, 5) == false);
    assert(isRectanglePair(1, 2, 3, 4) == false);
    assert(isRectanglePair(2, 2, 3, 4) == false);

    // Edge cases with negatives and zeros
    assert(isRectanglePair(-1, -1, 0, 0) == true);
    assert(isRectanglePair(0, 0, 0, 1) == false);
}
#include <algorithm>

// Return true if the four integers can be paired into two equal-length pairs.
bool isRectanglePair(int a, int b, int c, int d) {
    // Check the three possible pairings.
    if (a == b && c == d) {
        return true;
    }
    if (a == c && b == d) {
        return true;
    }
    if (a == d && b == c) {
        return true;
    }
    return false;
}
// The core idea is to check all possible ways to pair the four given integers into two pairs, where each pair must consist of two identical values. Since there are only four numbers, there are exactly three distinct ways to pair them: `(a,b)` with `(c,d)`, `(a,c)` with `(b,d)`, and `(a,d)` with `(b,c)`. For each of these pairings, we simply compare the equality of the two numbers within each pair. If any pairing yields both pairs equal, the function returns `true`. Otherwise, after checking all three pairings, it returns `false`. Edge cases: all four numbers equal is valid because any pairing works; two numbers equal but the other two different fails; values can be negative or zero because equality is the only condition. The algorithm runs in constant time `O(1)` and uses `O(1)` auxiliary space, as it only performs a fixed number of integer comparisons and returns a boolean.
