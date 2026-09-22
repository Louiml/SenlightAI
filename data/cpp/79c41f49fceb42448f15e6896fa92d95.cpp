// Write a C++ function that takes an integer `n` (odd, 1 ≤ n ≤ 101) and an `n`×`n` square matrix of integers, and returns the sum of all elements located on the main diagonal, the anti-diagonal, the middle row, and the middle column. The center element (intersection of all four) must be counted only once in the final sum. The function signature should be `int sumSpecialLines(const std::vector<std::vector<int>>& matrix, int n)`.
#include <cassert>
#include <vector>

// Function declaration (implementation above in the solution)
int sumSpecialLines(const std::vector<std::vector<int>>& matrix, int n);

int main() {
    // n = 1
    std::vector<std::vector<int>> m1 = {{5}};
    assert(sumSpecialLines(m1, 1) == 5);

    // n = 3, simple pattern
    std::vector<std::vector<int>> m3 = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };
    // Main diag: 1+5+9=15, anti: 3+5+7=15, middle row: 4+5+6=15, middle col: 2+5+8=15
    // Sum lines: 15+15+15+15=60, subtract 3*5=15 -> 45
    assert(sumSpecialLines(m3, 3) == 45);

    // n = 3 with zeros
    std::vector<std::vector<int>> m3z = {
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0}
    };
    assert(sumSpecialLines(m3z, 3) == 0);

    // n = 3 negative values
    std::vector<std::vector<int>> m3neg = {
        {-1, -2, -3},
        {-4, -5, -6},
        {-7, -8, -9}
    };
    // Sum lines: (-15)+(-15)+(-15)+(-15) = -60, subtract 3*(-5) = -15 -> -45
    assert(sumSpecialLines(m3neg, 3) == -45);

    // n = 5, all ones
    std::vector<std::vector<int>> m5(5, std::vector<int>(5, 1));
    // Each line has 5 ones, sum lines = 4*5 = 20, subtract 3*1 = 3 -> 17
    assert(sumSpecialLines(m5, 5) == 17);

    // n = 5, all zeros except center is 7
    std::vector<std::vector<int>> m5c(5, std::vector<int>(5, 0));
    m5c[2][2] = 7;
    // Only the center is on all lines, sum lines = 4*7 = 28, subtract 3*7 = 21 -> 7
    assert(sumSpecialLines(m5c, 5) == 7);

    // n = 7, check with arbitrary values by manual computation
    std::vector<std::vector<int>> m7 = {
        {0,1,2,3,4,5,6},
        {7,8,9,10,11,12,13},
        {14,15,16,17,18,19,20},
        {21,22,23,24,25,26,27},
        {28,29,30,31,32,33,34},
        {35,36,37,38,39,40,41},
        {42,43,44,45,46,47,48}
    };
    // main diag: 0+8+16+24+32+40+48 = 168
    // anti diag: 6+12+18+24+30+36+42 = 168
    // middle row (i=3): 21+22+23+24+25+26+27 = 168
    // middle col (j=3): 3+10+17+24+31+38+45 = 168
    // sum lines = 672, subtract 3*24=72 -> 600
    assert(sumSpecialLines(m7, 7) == 600);

    return 0;
}
#include <vector>

// Sums all elements on the main diagonal, anti-diagonal, middle row, and middle column.
// The center element (intersection of all four) is counted only once.
int sumSpecialLines(const std::vector<std::vector<int>>& matrix, int n) {
    int center = n / 2;
    int sum = 0;
    
    for (int i = 0; i < n; ++i) {
        sum += matrix[i][i];               // main diagonal
        sum += matrix[i][n - 1 - i];       // anti-diagonal
        sum += matrix[center][i];          // middle row
        sum += matrix[i][center];          // middle column
    }
    
    // The center cell was added 4 times, but should be included only once.
    sum -= 3 * matrix[center][center];
    
    return sum;
}
// The four special lines are: main diagonal (indices `[i][i]`), anti-diagonal (`[i][n-1-i]`), middle row (`[n/2][i]`), and middle column (`[i][n/2]`). If we simply sum all four lines independently, elements at intersections will be counted multiple times. The intersections are:
// - Main diagonal ∩ anti-diagonal: only the center cell (since n is odd, both diagonals cross at `(n/2, n/2)`).
// - Main diagonal ∩ middle row: cells at `(n/2, n/2)` and `(n/2, n/2)` (same).
// - Main diagonal ∩ middle column: `(n/2, n/2)`.
// - Anti-diagonal ∩ middle row: `(n/2, n/2)`.
// - Anti-diagonal ∩ middle column: `(n/2, n/2)`.
// - Middle row ∩ middle column: `(n/2, n/2)`.
// All intersections collapse to the center. The center appears in all four sets, so it is counted 4 times. We need it counted once, so subtract 3 times the center value. Edge cases: when n=1, the matrix has only one cell, and the formula gives `a[0][0] + a[0][0] + a[0][0] + a[0][0] - 3*a[0][0] = a[0][0]` correct. Time complexity is O(n²) to read the matrix (but the function only iterates O(n) times because we sum directly from the passed matrix), and O(1) auxiliary space.
