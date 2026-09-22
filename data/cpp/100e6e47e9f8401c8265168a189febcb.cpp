/*
Write a C++ function that takes a 2D grid of characters representing a partially filled puzzle, where each cell is either `.` (empty) or a digit `'1'`–`'5'`. The function must fill every empty cell with the smallest digit `1`–`5` that does not appear in any of the four cardinal neighbors (up, down, left, right) of that cell. Digits can repeat elsewhere in the grid (no row/column constraints, only neighbor constraints). The function should return the filled grid as a `vector<string>` (each string is a row of digits without separators). Input grid dimensions are at least 1×1 and at most 100×100. The initial grid is guaranteed to be valid (no two filled adjacent cells share the same digit). If an empty cell has all five digits `1`–`5` appearing among its neighbors (which cannot happen under the guarantee, but for robustness), leave it as `'.'`. Process cells in row-major order (top-to-bottom, left-to-right), so the fill decision for an earlier cell may affect later cells. Ensure the function is `const`-correct (takes input by `const vector<string>&`) and uses no global state.
*/

#include <vector>
#include <string>

// Fill each '.' with smallest digit 1-5 not present in orthogonal neighbors.
// Returns the completed grid as a vector of strings.
std::vector<std::string> fillPuzzle(const std::vector<std::string>& grid) {
    int h = static_cast<int>(grid.size());
    if (h == 0) return {};
    int w = static_cast<int>(grid[0].size());

    // Work on a mutable copy.
    std::vector<std::string> result = grid;

    // Direction vectors: up, right, down, left (order doesn't matter).
    const int di[4] = {-1, 0, 1, 0};
    const int dj[4] = {0, 1, 0, -1};

    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            if (result[i][j] != '.') continue; // already filled

            // Mark which digits are used by neighbors.
            bool used[6] = {false}; // indices 1..5
            for (int k = 0; k < 4; ++k) {
                int ni = i + di[k];
                int nj = j + dj[k];
                if (ni >= 0 && ni < h && nj >= 0 && nj < w) {
                    char ch = result[ni][nj];
                    if (ch >= '1' && ch <= '5') {
                        used[ch - '0'] = true;
                    }
                }
            }

            // Pick the smallest unused digit, or leave '.' if none.
            for (int d = 1; d <= 5; ++d) {
                if (!used[d]) {
                    result[i][j] = static_cast<char>('0' + d);
                    break;
                }
            }
            // If all used, we leave it as '.' (robustness).
        }
    }

    return result;
}

#include <cassert>
#include <vector>
#include <string>

// Declare the function (from the solution above).
std::vector<std::string> fillPuzzle(const std::vector<std::string>& grid);

int main() {
    // Basic case with single empty cell.
    {
        std::vector<std::string> in = {"12", "3."};
        std::vector<std::string> out = fillPuzzle(in);
        assert(out[0] == "12");
        assert(out[1] == "34");
    }

    // Empty cell with neighbors 1,2,3,4 -> should pick 5.
    {
        std::vector<std::string> in = {"1234", "5..."}; // just test one cell
        // Actually let's craft a 2x2: top-left '.' with neighbors 1,2,3,4?
        // Need a 3x3 for that. Use a simple 3x3:
        std::vector<std::string> in2 = {
            "1.3",
            "5.4",  
            "2.."
        };
        // Cell (0,1) neighbors: up none, right '3', down '5', left '1' -> used {1,3,5} -> pick 2.
        // Cell (1,1) neighbors: up (filled 2), right '4', down '.', left '.' -> used {2,4} -> pick 1.
        // Cell (2,1) neighbors: up '1', left '2', right '.', down none -> used {1,2} -> pick 3.
        // Cell (2,2) neighbors: up '4', left '3', right none, down none -> used {3,4} -> pick 1.
        std::vector<std::string> out = fillPuzzle(in2);
        assert(out[0] == "123");
        assert(out[1] == "514");
        assert(out[2] == "231");
    }

    // No empty cells -> unchanged.
    {
        std::vector<std::string> in = {"12345", "54321"};
        assert(fillPuzzle(in) == in);
    }

    // Edge case: 1x1 empty cell -> picks 1.
    {
        std::vector<std::string> in = {"."};
        assert(fillPuzzle(in) == std::vector<std::string>{"1"});
    }

    // 2x2 all empty -> greedy picks:
    // (0,0) no neighbors -> 1
    // (0,1) neighbors (0,0)=1 -> pick 2
    // (1,0) neighbors (0,0)=1 -> pick 2 (but (1,1) not filled yet so fine)
    // (1,1) neighbors (0,1)=2, (1,0)=2 -> pick 1
    {
        std::vector<std::string> in = {"..", ".."};
        std::vector<std::string> out = fillPuzzle(in);
        assert(out[0] == "12");
        assert(out[1] == "21");
    }

    // Ensure original input not modified (const correctness).
    {
        std::vector<std::string> in = {".1", "2."};
        std::vector<std::string> orig = in;
        (void)fillPuzzle(in);
        assert(in == orig);
    }

    return 0;
}

// The algorithm is straightforward greedy filling in row-major order. For each cell, if it is empty, we examine its four immediate neighbors (within bounds) and collect the digits they currently hold (after previous fillings). Then we choose the smallest digit from `1` to `5` that is not among those neighbor digits. Because the initial grid is valid and we always pick a digit that avoids conflicts with already processed neighbors (and also with unprocessed neighbors that are initially filled), the greedy approach never creates a conflict for already filled cells; however, it might create a conflict with a later initially‑filled neighbor? Actually, since we only fill empty cells, and we check all current neighbors (including initially filled ones that are not yet processed), we always avoid conflicts with any initial filled neighbor. For a neighbor that is empty and not yet filled, it hasn't been assigned, so no conflict. Processing row-major means we never revisit a cell. The guarantee that the initial grid has no adjacent equal digits ensures that no matter which digit we pick (avoiding filled neighbors), we won't accidentally cause a conflict with a later filled neighbor because later cells check their neighbors when they are filled. So greedy works. Edge case: if an empty cell is surrounded by all five digits (impossible under guarantee but we handle), leave as `'.'`. Time complexity: O(H*W*5) since for each cell we check up to 4 neighbors and iterate 5 candidate digits. Space: O(1) extra aside from output.
