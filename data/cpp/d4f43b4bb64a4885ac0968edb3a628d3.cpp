// Write a C++ function `reduceGrid` that takes a square grid of characters (represented as a `std::vector<std::string>`), along with an integer `k`, and returns a new grid where only every `k`-th row and every `k`-th column of the original grid is kept (i.e., starting from row 0 and column 0, include elements at indices `0, k, 2k, ...`). The input grid is guaranteed to be square (rows == columns), `k` is a positive integer that divides the grid size evenly (so the reduced grid is also square), and each string in the vector has exactly the same length equal to the number of rows. The function should preserve the order of the selected elements. For example, if the original grid is `[["a","b","c"],["d","e","f"],["g","h","i"]]` and `k=2`, the result should be `[["a","c"],["g","i"]]`. The function must not modify the input and should return a new vector of strings.
#include <cassert>
#include <vector>
#include <string>

// The solution function is declared above; this is just the test harness.
int main() {
    // Basic 3x3 with k=2
    std::vector<std::string> g1 = {"abc", "def", "ghi"};
    std::vector<std::string> r1 = reduceGrid(g1, 2);
    assert((r1 == std::vector<std::string>{"ac", "gi"}));

    // k=1 returns the original grid
    std::vector<std::string> g2 = {"xy", "zw"};
    assert((reduceGrid(g2, 1) == g2));

    // k equal to grid size returns 1x1 with top-left
    std::vector<std::string> g3 = {"abc", "def", "ghi"};
    assert((reduceGrid(g3, 3) == std::vector<std::string>{"a"}));

    // 4x4 with k=2, all characters are distinct digits
    std::vector<std::string> g4 = {"abcd", "efgh", "ijkl", "mnop"};
    assert((reduceGrid(g4, 2) == std::vector<std::string>{"ac", "ik"}));

    // 2x2 with k=2
    std::vector<std::string> g5 = {"12", "34"};
    assert((reduceGrid(g5, 2) == std::vector<std::string>{"1"}));

    // 6x6 with k=3, using '.' and '#' pattern
    std::vector<std::string> g6 = {
        "######",
        "#....#",
        "#....#",
        "#....#",
        "#....#",
        "######"
    };
    assert((reduceGrid(g6, 3) == std::vector<std::string>{"###", "#.#"}));

    // Single cell grid with any k=1
    std::vector<std::string> g7 = {"x"};
    assert((reduceGrid(g7, 1) == std::vector<std::string>{"x"}));

    // 5x5 with k=1 (odd size)
    std::vector<std::string> g8 = {"abcde", "fghij", "klmno", "pqrst", "uvwxy"};
    assert((reduceGrid(g8, 1) == g8));

    return 0;
}
#include <vector>
#include <string>

// Given a square vector of strings 'grid' and a positive integer 'k',
// return a reduced grid containing every k-th row and every k-th column.
std::vector<std::string> reduceGrid(const std::vector<std::string>& grid, int k) {
    std::vector<std::string> result;
    const int n = static_cast<int>(grid.size());
    
    for (int i = 0; i < n; i += k) {
        std::string row;
        for (int j = 0; j < n; j += k) {
            row.push_back(grid[i][j]);
        }
        result.push_back(row);
    }
    return result;
}
// The main algorithm iterates over the rows of the input grid with a step size of `k`, starting at index 0. For each such row, it builds a new string by iterating over the columns with the same step size `k` and appending the character at that position. This directly constructs the reduced grid. Edge cases: `k=1` returns the original grid (since every element is selected), and `k` equal to the grid size returns a 1x1 grid containing only the top-left character. Since the problem guarantees `k` divides the grid size, no handling for non-divisible sizes is necessary, but the code is safe even if it didn't divide (the last partial row/column would simply be ignored). The time complexity is O((n/k)^2) where n is the original grid size, because each element in the reduced grid is visited exactly once. The space complexity is O((n/k)^2) for the output grid, plus O(1) extra auxiliary space. The function uses `const` references for the input and iterates with direct indexing for clarity and speed.
