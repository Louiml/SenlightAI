// Write a C++ function named `countSameColorNeighbors` that takes a 2D vector of strings `board`, along with two integers `h` and `w` representing a row and column index. The function must return the number of cells that are directly adjacent (up, down, left, right) to the cell at `(h, w)` and have exactly the same string value (color) as the cell at `(h, w)`. The board is guaranteed to be rectangular (all rows have the same length) and non-empty. The indices `h` and `w` are guaranteed to be valid (within bounds). Adjacent cells outside the board boundaries are ignored. The function must not modify the input board. Use `const` references for parameters where appropriate, and ensure the implementation is robust and self-contained.
#include <cassert>
#include <string>
#include <vector>

int countSameColorNeighbors(const std::vector<std::vector<std::string>>& board, int h, int w);

int main() {
    // Test 1: Basic 3x3 board, center cell has two neighbors of same color
    std::vector<std::vector<std::string>> b1 = {
        {"red", "blue", "red"},
        {"blue", "blue", "blue"},
        {"red", "blue", "red"}
    };
    assert(countSameColorNeighbors(b1, 1, 1) == 4); // all four neighbors are "blue"

    // Test 2: Corner cell in a 2x2 board with all same color
    std::vector<std::vector<std::string>> b2 = {
        {"x", "x"},
        {"x", "x"}
    };
    assert(countSameColorNeighbors(b2, 0, 0) == 2); // right and down

    // Test 3: Edge cell with no same neighbors
    std::vector<std::vector<std::string>> b3 = {
        {"a", "b", "c"},
        {"d", "e", "f"},
        {"g", "h", "i"}
    };
    assert(countSameColorNeighbors(b3, 0, 1) == 0); // neighbors: a, c, e (none equal "b")

    // Test 4: Single-cell board
    std::vector<std::vector<std::string>> b4 = {{"only"}};
    assert(countSameColorNeighbors(b4, 0, 0) == 0);

    // Test 5: Board with duplicates and different string lengths
    std::vector<std::vector<std::string>> b5 = {
        {"aa", "bb", "aa"},
        {"bb", "aa", "bb"},
        {"aa", "bb", "aa"}
    };
    assert(countSameColorNeighbors(b5, 1, 1) == 0); // center "aa" neighbors all "bb"

    // Test 6: Bottom-right corner with one same neighbor
    std::vector<std::vector<std::string>> b6 = {
        {"p", "q"},
        {"q", "q"}
    };
    assert(countSameColorNeighbors(b6, 1, 1) == 2); // left and up are "q"

    return 0;
}
#include <string>
#include <vector>

// Counts how many of the four orthogonal neighbors of board[h][w] share the same string value.
int countSameColorNeighbors(const std::vector<std::vector<std::string>>& board, int h, int w) {
    int count = 0;
    const std::string& centerColor = board[h][w];
    const int rows = static_cast<int>(board.size());
    const int cols = static_cast<int>(board[0].size());

    // Direction offsets: up, down, left, right
    const int dr[4] = {-1, 1, 0, 0};
    const int dc[4] = {0, 0, -1, 1};

    for (int i = 0; i < 4; ++i) {
        int nh = h + dr[i];
        int nw = w + dc[i];

        if (nh >= 0 && nh < rows && nw >= 0 && nw < cols) {
            if (board[nh][nw] == centerColor) {
                ++count;
            }
        }
    }

    return count;
}
// The solution is straightforward: define the four possible directions as coordinate offsets (up, down, left, right). Retrieve the color (string) of the center cell. For each direction, compute the candidate row and column by adding the offsets to `h` and `w`. Check if the candidate coordinates are within the board's bounds — specifically, the row must be between 0 and `board.size() - 1`, and the column must be between 0 and `board[0].size() - 1` (since the board is rectangular, any row's size is valid). If inside bounds, compare the candidate cell's string with the center color and increment the counter if equal. Edge cases: if the board has only one cell, there are no valid neighbors, result is 0. If the center is on an edge or corner, some directions will fall outside bounds and are ignored. Time complexity is O(1) because we only check at most four neighbors. Space complexity is O(1) auxiliary, ignoring the input storage. The function should be marked `const`-correct by taking `const std::vector<std::vector<std::string>>&` and not modifying it.
