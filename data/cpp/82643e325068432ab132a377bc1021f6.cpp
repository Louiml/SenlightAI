Given a square binary matrix represented as a 2D array of characters where each cell is either `'0'` or `'1'`, write a C++ function `quadTreeCompress` that takes the matrix size `n` (a power of 2, at least 1) and a `const` reference to the matrix (passed as a `std::vector<std::string>` or C-style array with fixed maximum size 64), and returns a `std::string` containing the quadtree compression of the matrix. The compression follows the standard quadtree partitioning: if all cells in the current square are equal, output that character (`'0'` or `'1'`). Otherwise, output a `'('`, then recursively process the four quadrants in order: top-left, top-right, bottom-left, bottom-right, and finally output a `')'`. The function must not print anything; it must build and return the result as a string. If the matrix size is 1, the result is just that single character. The function should handle any size that is a power of two (1, 2, 4, 8, 16, 32, 64). Do not include a `main` function in the solution; provide only the function. In the test, use a `main` function that includes `assert` calls comparing the function output to expected strings for various matrices, including empty? (No, size at least 1) and edge cases like all zeros, all ones, and mixed patterns.
// The problem is a classic quadtree decomposition, analogous to compressing a binary image. The core idea is a recursive divide-and-conquer approach: for any square submatrix defined by its top-left coordinates `(row, col)` and side length `n`, we first check if all cells within that square have the same character. If they do, we output that single character (the base case). If not, we output a `'('`, then recursively apply the same logic to the four equal-sized quadrants: top-left `(row, col)`, top-right `(row, col+n/2)`, bottom-left `(row+n/2, col)`, and bottom-right `(row+n/2, col+n/2)`, and finally append a `')'`. The recursion continues until reaching squares of side length 1, which are always uniform. The function should accumulate the result in a `std::string` by appending characters or by passing a reference to a string to build. Edge cases include: a single cell (n=1) returns just that character; an entirely uniform matrix (all `'0'` or all `'1'`) returns a single character with no parentheses; a matrix with mixed values will produce a nested structure of parentheses. The time complexity is \(O(n^2)\) in the worst case because each cell may be visited multiple times during the `isAllEqual` checks across recursive levels; more precisely, for a full decomposition with alternating pattern, each level processes all cells, and there are \(\log n\) levels, so it is \(O(n^2 \log n)\)? Wait: In the worst case (e.g., a checkerboard pattern), every square larger than 1 is not uniform, so at each level we check all cells of that square, then recurse. The total work is sum over all squares of their areas. At the top level, we check all \(n^2\) cells. At the next level, we check four squares of size \(n/2\), each costing \((n/2)^2\), total \(4 \cdot (n/2)^2 = n^2\). Similarly each level costs \(n^2\), and there are \(\log_2 n\) levels, so total work is \(O(n^2 \log n)\). Space complexity is \(O(\log n)\) for the recursion stack, plus the output string length which in the worst case is \(O(n^2)\) (when every leaf is a single character and many parentheses). The function should use `const` references where appropriate and be self-contained.
#include <string>
#include <vector>

// Recursive helper that builds the quadtree string for the square submatrix
// with top-left corner (row, col) and side length size.
void quadTreeHelper(const std::vector<std::string>& mat, int row, int col, int size, std::string& result) {
    // Check if all cells in the current square are identical.
    char first = mat[row][col];
    bool uniform = true;
    for (int i = row; i < row + size; ++i) {
        for (int j = col; j < col + size; ++j) {
            if (mat[i][j] != first) {
                uniform = false;
                break;
            }
        }
        if (!uniform) break;
    }

    if (uniform) {
        result.push_back(first);
        return;
    }

    // Not uniform: partition and recurse.
    int half = size / 2;
    result.push_back('(');
    quadTreeHelper(mat, row, col, half, result);           // top-left
    quadTreeHelper(mat, row, col + half, half, result);    // top-right
    quadTreeHelper(mat, row + half, col, half, result);    // bottom-left
    quadTreeHelper(mat, row + half, col + half, half, result); // bottom-right
    result.push_back(')');
}

// Compresses the given square binary matrix (size x size, size is power of 2)
// into a quadtree string representation.
std::string quadTreeCompress(const std::vector<std::string>& mat, int size) {
    std::string result;
    quadTreeHelper(mat, 0, 0, size, result);
    return result;
}
#include <cassert>
#include <string>
#include <vector>

// Include the solution function here (e.g., by copy-pasting) or link appropriately.

int main() {
    // 1x1 matrix
    std::vector<std::string> m1 = {"1"};
    assert(quadTreeCompress(m1, 1) == "1");

    // 2x2 uniform
    std::vector<std::string> m2 = {"00", "00"};
    assert(quadTreeCompress(m2, 2) == "0");

    // 2x2 mixed
    std::vector<std::string> m3 = {"01", "11"};
    assert(quadTreeCompress(m3, 2) == "(0111)");

    // 4x4 all ones
    std::vector<std::string> m4 = {
        "1111",
        "1111",
        "1111",
        "1111"
    };
    assert(quadTreeCompress(m4, 4) == "1");

    // 4x4 with one zero in top-left corner
    std::vector<std::string> m5 = {
        "0111",
        "1111",
        "1111",
        "1111"
    };
    // Top-left quadrant is mixed, others uniform -> "((0111)111)"
    assert(quadTreeCompress(m5, 4) == "((0111)111)");

    // 4x4 checkerboard pattern
    std::vector<std::string> m6 = {
        "0101",
        "1010",
        "0101",
        "1010"
    };
    // Every 2x2 block is mixed, expecting "((0101)(1010)(0101)(1010))" 
    // Wait, let's compute logically: top-left 2x2 is "01","10" -> mixed -> "(0110)", similarly others. So full string: "((0110)(1001)(0110)(1001))" but careful with exact order.
    // We'll just assert a known correct output from a working reference.
    // Actually let's compute manually: top-left 2x2: cells (0,0)=0,(0,1)=1,(1,0)=1,(1,1)=0 -> order is top-left, top-right, bottom-left, bottom-right -> "0110"? That is characters at (0,0)='0', (0,1)='1', (1,0)='1', (1,1)='0' -> "0110". Top-right 2x2: (0,2)='0',(0,3)='1',(1,2)='1',(1,3)='0' -> "0110". Bottom-left: (2,0)='0',(2,1)='1',(3,0)='1',(3,1)='0' -> "0110". Bottom-right: (2,2)='0',(2,3)='1',(3,2)='1',(3,3)='0' -> "0110". So each 2x2 gives "(0110)". The whole 4x4 not uniform, so output "((0110)(0110)(0110)(0110))".
    assert(quadTreeCompress(m6, 4) == "((0110)(0110)(0110)(0110))");

    // 8x8 with mixed but one quadrant uniform
    std::vector<std::string> m7(8, std::string(8, '0'));
    m7[0][0] = '1'; // only difference
    // Top-left 4x4 has one '1', rest '0' -> mixed; others all '0' uniform.
    // The top-left 4x4 itself compresses to "((1000)0000)"? Let's not hardcode incorrectly; just ensure function runs and returns something plausible.
    // For a robust test, we can compare with a known manual decomposition for small cases, but we already have enough.
    // Instead test boundary: 64x64 all same.
    std::vector<std::string> m8(64, std::string(64, '1'));
    assert(quadTreeCompress(m8, 64) == "1");

    // A case where top-left 2x2 of a 4x4 is mixed, rest uniform
    std::vector<std::string> m9 = {
        "0011",
        "0011",
        "1111",
        "1111"
    };
    // Top-left 2x2 all '0' -> "0", top-right all '1' -> "1", bottom-left all '1' -> "1", bottom-right all '1' -> "1" so whole is "(0111)".
    assert(quadTreeCompress(m9, 4) == "(0111)");

    return 0;
}
