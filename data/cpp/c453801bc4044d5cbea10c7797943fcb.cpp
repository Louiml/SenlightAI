Write a C++ function that takes a positive integer `n` (where `n >= 1`) and returns the number of colored cells after `n` minutes in the following process: starting with one colored cell (at minute 1), each minute every currently colored cell causes all its four adjacent (up, down, left, right) cells to become colored. The resulting shape is a diamond (plus-shaped pattern expanding outward). Your function should accept the integer `n` and return the total count of colored cells as a `long long` to avoid overflow. The input is guaranteed to be a positive integer, but you should still handle it safely.
// The process expands symmetrically in four directions. At minute 1, there is 1 cell. At each subsequent minute, a new "layer" is added around the diamond. The number of cells added at minute `k` (for `k >= 2`) is `4*(k-1)`, because each side of the diamond grows by one cell per minute, and the four corners are shared? Actually the formula: the total cells after `n` minutes follows the pattern: `1, 5, 13, 25, 41, ...` which is `2*n*(n-1)+1`. Derivation: the diamond can be seen as the union of two arithmetic progressions. An alternative view: the number of cells on the "cross" vertical and horizontal axes plus the quadrants. More simply, the formula can be derived by summing the layers: total = 1 + sum_{i=1}^{n-1} 4*i = 1 + 4*(n-1)*n/2 = 1 + 2n(n-1) = 2n^2 - 2n + 1. The function directly returns that expression. Edge cases: `n=1` returns 1; for larger `n`, using `long long` avoids overflow since `2*n*(n-1)` can exceed int range for n > ~46340. Time complexity is O(1), space O(1).
#include <cstdint>

// Returns the number of colored cells after n minutes of the expansion process.
// n must be a positive integer.
long long coloredCellsCount(int n) {
    // For n minutes, total = 2*n*(n-1) + 1.
    // Use long long to avoid overflow for large n.
    return 2LL * n * (n - 1) + 1;
}
#include <cassert>

int main() {
    // Base case: minute 1 has exactly 1 colored cell.
    assert(coloredCellsCount(1) == 1);
    // Minute 2: 1 + 4 = 5
    assert(coloredCellsCount(2) == 5);
    // Minute 3: 5 + 8 = 13
    assert(coloredCellsCount(3) == 13);
    // Minute 4: 13 + 12 = 25
    assert(coloredCellsCount(4) == 25);
    // A larger value to verify formula scaling.
    assert(coloredCellsCount(10) == 181);
    // Boundary check for typical int maximum: n=46340 -> 2*46340*46339+1 fits in long long.
    assert(coloredCellsCount(46340) == 2LL * 46340 * 46339 + 1);
    return 0;
}
