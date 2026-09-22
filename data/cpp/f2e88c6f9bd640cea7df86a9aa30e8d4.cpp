Write a C++ function that takes a square matrix of size `n` (where `n` is a power of 2) containing only 0s and 1s, and returns a `std::pair<int, int>` where the first element is the total number of monochrome (all-0 or all-1) square submatrices of size 1×1 up to n×n that appear when the matrix is recursively divided into quadrants until each submatrix is uniform. Specifically, the function must simulate a quadtree decomposition: starting from the full matrix, if the whole current submatrix is uniformly 0 or uniformly 1, count it as one "white" (0) or "black" (1) block and stop recursing into it. If not uniform, split it into four equal quadrants and recursively process each. The returned pair should be `(white_count, black_count)`. The input matrix is guaranteed to be square with dimensions that are a power of 2 (e.g., 1, 2, 4, 8, ...). Handle the edge case `n=1` directly, and ensure correctness for matrices that are entirely uniform or entirely non-uniform. The function should be `const`-correct and operate without modifying the input matrix. Include necessary headers and provide a descriptively named free function.

// The solution follows a classic divide-and-conquer / quadtree approach. At each recursive call, we examine the current submatrix defined by top-left corner `(x, y)` and size `size`. We check whether all elements in that submatrix are identical by iterating over its cells. If they are all the same, we increment the appropriate counter (0 for white, 1 for black) and stop. If not, we halve the size and recurse into the four quadrants. The base case is implicit: when `size == 1`, the check always succeeds because a single cell is uniform, so we count it directly. Key edge cases include a fully uniform matrix (counts one block of that color), a matrix where every cell differs from its neighbors (e.g., a 2×2 checkerboard), and `n=1` (returns either (1,0) or (0,1)). The time complexity is O(n² log n) in the worst case? Actually, standard analysis: each level of recursion examines every cell at most once per level, and there are O(log n) levels, so O(n² log n) time in the worst case (e.g., every submatrix non-uniform until size 1). Space complexity is O(log n) due to recursion stack depth, plus O(n²) for the input matrix storage (not copied). For clarity, we can implement a helper recursive function that takes the matrix reference and the three integers, passing a pair reference to accumulate counts. This avoids global variables and makes the function pure.

#include <vector>
#include <utility>

// Count monochrome blocks in a quadtree decomposition of a binary matrix.
// Returns (white_count, black_count) where white = 0 and black = 1 blocks.
std::pair<int, int> countMonochromeBlocks(const std::vector<std::vector<int>>& matrix) {
    int n = matrix.size();
    if (n == 0) return {0, 0};

    int white = 0;
    int black = 0;

    // Recursive helper: processes submatrix with top-left (x, y) and given size.
    // Uses const reference to avoid copying and ensures read-only access.
    auto isUniform = [&](int x, int y, int size) {
        int first = matrix[x][y];
        for (int i = x; i < x + size; ++i) {
            for (int j = y; j < y + size; ++j) {
                if (matrix[i][j] != first) return false;
            }
        }
        return true;
    };

    // Lambda recursion using std::function or a nested named lambda via auto with capture.
    // To avoid std::function overhead, use a std::function or define a function object.
    // For simplicity, use std::function here. But we can also use a helper struct.
    // However, since we want a clean free function, we'll write a private recursive function below.
    // Actually, we'll define an inner helper using std::function (requires <functional>).
    // But the spec says include necessary headers. Let's include <functional>.
    // To keep it clean, we'll implement a static helper function outside countMonochromeBlocks.
    // Let's do that instead.

    // We'll define a helper function outside. But since we must output only the solution function,
    // we can define a lambda and use std::function. Let's do it directly.
    std::function<void(int, int, int)> solve = [&](int x, int y, int size) {
        if (isUniform(x, y, size)) {
            if (matrix[x][y] == 0) ++white;
            else ++black;
            return;
        }
        int half = size / 2;
        solve(x, y, half);
        solve(x, y + half, half);
        solve(x + half, y, half);
        solve(x + half, y + half, half);
    };

    solve(0, 0, n);
    return {white, black};
}
*Note: The above solution includes `<functional>` implicitly? Actually it uses `std::function` which requires `<functional>`. To be self-contained, I'll add `#include <functional>` in the solution. Also, the lambda `isUniform` captures by reference; that's fine. The solution is correct.

But to avoid `std::function` overhead and recursion complexity, a cleaner approach is a static helper function. Let me rewrite the solution with a standalone helper that is not exposed. Since the spec says "free function" and "output code only", we can include a helper function with internal linkage using `static`. I'll provide that.

(final, improved with a helper function)
#include <vector>
#include <utility>

namespace {
    // Recursive helper: counts uniform blocks in a submatrix.
    void countHelper(const std::vector<std::vector<int>>& matrix,
                     int x, int y, int size,
                     int& white, int& black) {
        // Check if the submatrix is uniform.
        int first = matrix[x][y];
        bool uniform = true;
        for (int i = x; i < x + size && uniform; ++i) {
            for (int j = y; j < y + size; ++j) {
                if (matrix[i][j] != first) {
                    uniform = false;
                    break;
                }
            }
        }
        if (uniform) {
            if (first == 0) ++white;
            else ++black;
            return;
        }
        int half = size / 2;
        // Recursively process four quadrants.
        countHelper(matrix, x, y, half, white, black);
        countHelper(matrix, x, y + half, half, white, black);
        countHelper(matrix, x + half, y, half, white, black);
        countHelper(matrix, x + half, y + half, half, white, black);
    }
}

// Public interface: returns (white_count, black_count).
std::pair<int, int> countMonochromeBlocks(const std::vector<std::vector<int>>& matrix) {
    int n = matrix.size();
    if (n == 0) return {0, 0};
    int white = 0, black = 0;
    countHelper(matrix, 0, 0, n, white, black);
    return {white, black};
}

#include <cassert>
#include <vector>
#include <utility>

// The solution function is assumed to be defined above (not repeated here).

int main() {
    // Test 1: 1x1 matrix of 0
    std::vector<std::vector<int>> m1 = {{0}};
    assert(countMonochromeBlocks(m1) == std::make_pair(1, 0));

    // Test 2: 1x1 matrix of 1
    std::vector<std::vector<int>> m2 = {{1}};
    assert(countMonochromeBlocks(m2) == std::make_pair(0, 1));

    // Test 3: 2x2 all zeros
    std::vector<std::vector<int>> m3 = {{0,0},{0,0}};
    assert(countMonochromeBlocks(m3) == std::make_pair(1, 0));

    // Test 4: 2x2 all ones
    std::vector<std::vector<int>> m4 = {{1,1},{1,1}};
    assert(countMonochromeBlocks(m4) == std::make_pair(0, 1));

    // Test 5: 2x2 checkerboard
    std::vector<std::vector<int>> m5 = {{0,1},{1,0}};
    // Every 1x1 block counts, and no larger uniform block: total 4 white, 0 black? 
    // Actually cells: (0,0)=0, (0,1)=1, (1,0)=1, (1,1)=0 -> two 0s, two 1s.
    // Quadtree: top-left 1x1=0 -> white++, top-right=1 -> black++, bottom-left=1 -> black++, bottom-right=0 -> white++.
    // So white=2, black=2.
    assert(countMonochromeBlocks(m5) == std::make_pair(2, 2));

    // Test 6: 4x4 with a large uniform block and some structure
    // Matrix: top-left 2x2 zeros, rest ones? Let's build:
    // 0 0 1 1
    // 0 0 1 1
    // 1 1 0 0
    // 1 1 0 0
    // This is 4 quadrants each 2x2 uniform: top-left all 0 -> white++,
    // top-right all 1 -> black++, bottom-left all 1 -> black++, bottom-right all 0 -> white++.
    // So white=2, black=2.
    std::vector<std::vector<int>> m6 = {
        {0,0,1,1},
        {0,0,1,1},
        {1,1,0,0},
        {1,1,0,0}
    };
    assert(countMonochromeBlocks(m6) == std::make_pair(2, 2));

    // Test 7: 4x4 all zeros but split? Actually all zeros -> one block white=1.
    std::vector<std::vector<int>> m7(4, std::vector<int>(4, 0));
    assert(countMonochromeBlocks(m7) == std::make_pair(1, 0));

    // Test 8: 4x4 with all cells different pattern but every 2x2 subquadrant non-uniform?
    // For example, a 4x4 with a single 1 in the corner:
    // 1 0 0 0
    // 0 0 0 0
    // 0 0 0 0
    // 0 0 0 0
    // Recursion: full non-uniform -> split into four 2x2 quadrants.
    // Top-left quadrant: (0,0)=1, others 0 -> non-uniform -> split into four 1x1: 1 black, 3 white.
    // Top-right quadrant: all zeros -> uniform white.
    // Bottom-left: all zeros -> uniform white.
    // Bottom-right: all zeros -> uniform white.
    // Total white = 3 (from top-left) + 1 + 1 + 1 = 6? Let's count: top-left gives 3 white + 1 black = 4 blocks; top-right gives 1 white; bottom-left 1 white; bottom-right 1 white. Total white=3+1+1+1=6, black=1.
    std::vector<std::vector<int>> m8 = {
        {1,0,0,0},
        {0,0,0,0},
        {0,0,0,0},
        {0,0,0,0}
    };
    assert(countMonochromeBlocks(m8) == std::make_pair(6, 1));

    // Test 9: 8x8 with alternating pattern? Not needed, but we can test 2x2 uniform split.
    // Already covered.

    // Test 10: Empty matrix (size 0) -> Should return (0,0). But the problem says n is power of 2, likely >=1. We'll include it.
    std::vector<std::vector<int>> m9;
    assert(countMonochromeBlocks(m9) == std::make_pair(0, 0));

    return 0;
}
