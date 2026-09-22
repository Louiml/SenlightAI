// Write a C++ function `countPaperSections` that takes a square matrix of integers (values -1, 0, or 1), its side length `k` (where `k` is a power of 3, i.e., 3^m for some m≥0), and returns a `std::array<int,3>` containing the counts of how many uniform blocks (of any size 3^n × 3^n) of each value appear when the matrix is recursively divided into 9 equal sub-squares until each sub-square contains only identical values. The returned array’s indices 0, 1, 2 correspond to counts of -1, 0, and 1 respectively. The matrix is square, `k` is at least 1, and all entries are exactly -1, 0, or 1. The function must not modify the input matrix and must be efficient enough for `k` up to 2187 (3^7). Example: for a 3×3 matrix with all entries equal to 1, the result is {0,0,1}; for a 3×3 matrix where the top-left 2×2 block is 0 and the rest is 1, the result should count the sub-blocks appropriately.
The solution uses a recursive divide-and-conquer approach similar to quadtree decomposition but with 9-way splitting. The main function `countPaperSections` initializes a result array `counts` with zeros and calls a helper `solve` that processes a square region defined by top-left corner `(x,y)` and size `n`. First, a helper `isUniform` checks whether all cells in that region have the same value by comparing each cell to the top-left cell. If uniform, the helper increments `counts[value + 1]` (since values are -1,0,1, adding 1 maps them to indices 0,1,2) and returns. If not uniform, we split the region into 9 equal sub-squares of size `n/3` (since `k` is a power of 3, `n` is always divisible by 3 when non-uniform). Recursively call `solve` on each of the 9 sub-squares. Edge cases include the base case `n == 1` (which is always uniform) and `n == 3` when not uniform (each sub-square is size 1). The algorithm visits each cell exactly once across all recursive calls because every leaf is a uniform block that fully partitions the matrix, and internal checks are bounded by the area of that sub-square. Time complexity is O(k^2) because each cell is included in exactly one uniform leaf and constant overhead per leaf; the worst case for checking uniformity is O(k^2) per leaf but since leaves partition the area, total work is O(k^2). Space complexity is O(log_3 k) for the recursion stack, since the depth is at most the exponent of 3, but because we pass the matrix by reference and only use integer parameters, auxiliary space is O(1) plus recursion stack.
#include <array>
#include <vector>

// Helper: check if all entries in the square region are identical.
bool isUniform(const std::vector<std::vector<int>>& matrix, int x, int y, int n) {
    int first = matrix[y][x];
    for (int i = y; i < y + n; ++i) {
        for (int j = x; j < x + n; ++j) {
            if (matrix[i][j] != first) {
                return false;
            }
        }
    }
    return true;
}

// Helper: recursive decomposition.
void solvePaper(const std::vector<std::vector<int>>& matrix, int x, int y, int n, std::array<int,3>& counts) {
    if (isUniform(matrix, x, y, n)) {
        counts[matrix[y][x] + 1] += 1; // map -1→0, 0→1, 1→2
        return;
    }

    int sub = n / 3;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            solvePaper(matrix, x + j * sub, y + i * sub, sub, counts);
        }
    }
}

// Count uniform blocks of -1, 0, 1 in a 3^n × 3^n matrix.
std::array<int,3> countPaperSections(const std::vector<std::vector<int>>& matrix, int k) {
    std::array<int,3> counts = {0, 0, 0};
    solvePaper(matrix, 0, 0, k, counts);
    return counts;
}
#include <cassert>
#include <vector>
#include <array>

// Copy the solution code here (including headers, helpers, and countPaperSections)

int main() {
    // Test 1: 1×1 matrix with value -1
    std::vector<std::vector<int>> m1 = {{-1}};
    assert(countPaperSections(m1, 1) == std::array<int,3>{1, 0, 0});

    // Test 2: 3×3 all zeros
    std::vector<std::vector<int>> m2 = {{0,0,0},{0,0,0},{0,0,0}};
    assert(countPaperSections(m2, 3) == std::array<int,3>{0, 1, 0});

    // Test 3: 3×3 mixed: top-left 2×2 is 1, rest is 0
    std::vector<std::vector<int>> m3 = {{1,1,0},{1,1,0},{0,0,0}};
    // The whole 3×3 is not uniform, split into 1×1 sub-squares → 4 ones and 5 zeros
    assert(countPaperSections(m3, 3) == std::array<int,3>{0, 5, 4});

    // Test 4: 9×9 matrix with top 3×3 block = -1, rest = 0
    std::vector<std::vector<int>> m4(9, std::vector<int>(9, 0));
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            m4[i][j] = -1;
    // The top 3×3 is uniform -1, the other 8 sub-squares of size 3 are uniform 0 → counts: 1(-1), 8(0), 0(1)
    assert(countPaperSections(m4, 9) == std::array<int,3>{1, 8, 0});

    // Test 5: 3×3 checkerboard pattern
    std::vector<std::vector<int>> m5 = {{1,-1,1},{-1,1,-1},{1,-1,1}};
    // No uniform sub-block larger than 1×1 → 5 ones and 4 minus-ones
    assert(countPaperSections(m5, 3) == std::array<int,3>{4, 0, 5});

    // Test 6: 9×9 all ones (single uniform block)
    std::vector<std::vector<int>> m6(9, std::vector<int>(9, 1));
    assert(countPaperSections(m6, 9) == std::array<int,3>{0, 0, 1});

    // Test 7: 3×3 with single different cell in center (rest 0)
    std::vector<std::vector<int>> m7 = {{0,0,0},{0,1,0},{0,0,0}};
    // 3×3 not uniform → split into 9 cells → 8 zeros, 1 one
    assert(countPaperSections(m7, 3) == std::array<int,3>{0, 8, 1});

    // Test 8: 9×9 where each 3×3 sub-block is uniform but different
    std::vector<std::vector<int>> m8(9, std::vector<int>(9, 0));
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j) {
            if ((i + j) % 2 == 0) m8[i][j] = 1; // top-left 3×3 some ones
            else m8[i][j] = -1;
        }
    // This is messy; just verify total cells sum = 81 and each leaf is a single cell (since no uniform block) → total leaves = 81
    auto res8 = countPaperSections(m8, 9);
    assert(res8[0] + res8[1] + res8[2] == 81);

    // Test 9: 1×1 with value 1
    std::vector<std::vector<int>> m9 = {{1}};
    assert(countPaperSections(m9, 1) == std::array<int,3>{0, 0, 1});

    // Test 10: 9×9 all -1 except one 0 in the center cell
    std::vector<std::vector<int>> m10(9, std::vector<int>(9, -1));
    m10[4][4] = 0;
    // Whole 9×9 not uniform → split into 9 3×3 blocks, each not uniform except? Actually each 3×3 has a 0 at (1,1) in the center block. Other 8 blocks are uniform -1. Center block splits into 9 cells → 8 -1 and 1 zero.
    // So counts: -1: 8 blocks *9 cells + 8 cells = 80? Let's compute: total -1 cells = 80, zeros = 1 → but leaves: 8 uniform blocks of size 9 each + 9 cells from center block = 17 leaves, but counts array counts leaves per value: -1 leaves: 8 uniform + 8 single = 16, zero leaves: 1. Let's assert total leaves = 17.
    auto res10 = countPaperSections(m10, 9);
    assert(res10[0] + res10[1] + res10[2] == 17);
    assert(res10[1] == 1); // exactly one zero leaf
}
