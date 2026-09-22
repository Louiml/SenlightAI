Write a standalone C++ function that reads a 2D binary grid (where 1 represents land and 0 represents water) along with its dimensions from a file named `nilaparvata.inp`, identifies each connected component of 1s using 4-directional adjacency (up, down, left, right), and writes to a file named `nilaparvata.out` the size of each component followed by the 1-based coordinates of all cells in that component, one component per line, and finally a single line at the end with the total number of 1s (sum of all component sizes). The function must handle arbitrary grids up to 10000×10000, avoid recursion stack overflow, and not modify the original input grid if possible. The input file format: first line contains two integers H and W (height and width), then H lines each with W integers (0 or 1) separated by spaces. The output file format: for each component in any order, write its size as an integer, then for each cell in that component write a space, then `[row, col]` with row and col being 1-based indices. After all components, write the total number of 1s on its own line. If there are no 1s, the output should contain only `0` on a single line.

// The problem is a classic connected-component labeling (flood fill) on a grid. The provided snippet uses recursion, which is unsafe for large grids due to stack overflow; we replace it with an explicit stack (iterative DFS) or BFS queue. The algorithm: parse H and W, then read the grid into a 2D vector. Create a mutable copy for traversal (or mark visited) to avoid modifying the original. Iterate over all cells; when a cell with value 1 is found, start a flood fill: push the cell into a stack/queue, mark it as visited (e.g., set to 0), and while the stack is not empty, pop a cell, record its coordinates in a temporary list, increment the component count, and check its four neighbors (if within bounds and value 1), push them and mark as visited. After the flood fill ends, output the component size and the coordinates (in any order, but we can record them in traversal order). After all components, output the total sum. Edge cases: zero rows or columns (but constraints likely positive), grid with all 0s, grid with one giant component, coordinates must be 1-based. Time complexity: O(H*W) because each cell is visited at most once. Space complexity: O(H*W) for the grid copy plus the stack in worst case also O(H*W) (e.g., a snake-like component), but we can reuse the same visited marking without extra memory for visited separately.

#include <fstream>
#include <vector>
#include <stack>
#include <utility>

// Reads a binary grid from "nilaparvata.inp" and writes connected components
// and total number of 1s to "nilaparvata.out". Uses iterative flood fill.
void processNilaparvata() {
    std::ifstream in("nilaparvata.inp");
    std::ofstream out("nilaparvata.out");

    int H, W;
    if (!(in >> H >> W)) {
        // If file missing or empty, write 0 and return.
        out << "0\n";
        return;
    }

    std::vector<std::vector<int>> grid(H, std::vector<int>(W));
    for (int i = 0; i < H; ++i) {
        for (int j = 0; j < W; ++j) {
            in >> grid[i][j];
        }
    }

    const int dr[4] = {-1, 1, 0, 0};
    const int dc[4] = {0, 0, -1, 1};

    long long total = 0;
    for (int r = 0; r < H; ++r) {
        for (int c = 0; c < W; ++c) {
            if (grid[r][c] == 1) {
                // Start new component
                std::vector<std::pair<int, int>> cells;
                std::stack<std::pair<int, int>> st;
                st.push({r, c});
                grid[r][c] = 0; // mark visited
                while (!st.empty()) {
                    auto [cr, cc] = st.top();
                    st.pop();
                    cells.emplace_back(cr + 1, cc + 1); // store 1-based
                    for (int dir = 0; dir < 4; ++dir) {
                        int nr = cr + dr[dir];
                        int nc = cc + dc[dir];
                        if (nr >= 0 && nr < H && nc >= 0 && nc < W && grid[nr][nc] == 1) {
                            grid[nr][nc] = 0;
                            st.push({nr, nc});
                        }
                    }
                }

                // Output this component
                out << cells.size();
                for (const auto& p : cells) {
                    out << " [" << p.first << ", " << p.second << "]";
                }
                out << '\n';
                total += cells.size();
            }
        }
    }

    out << total << '\n';
}

#include <cassert>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

// Forward declaration of solution function
void processNilaparvata();

// Helper to create an input file and run the function, then read output file.
std::string runWithInput(const std::string& inputContent) {
    std::ofstream inFile("nilaparvata.inp");
    inFile << inputContent;
    inFile.close();

    processNilaparvata();

    std::ifstream outFile("nilaparvata.out");
    std::ostringstream ss;
    ss << outFile.rdbuf();
    outFile.close();
    return ss.str();
}

int main() {
    // Test 1: simple 2x2 with two separate components
    std::string out1 = runWithInput("2 2\n1 0\n0 1\n");
    // Valid outputs: either "1 [1, 1]\n1 [2, 2]\n2" or order swapped. We'll check size.
    assert(out1.find("1 [1, 1]") != std::string::npos);
    assert(out1.find("1 [2, 2]") != std::string::npos);
    assert(out1.find("\n2\n") != std::string::npos);

    // Test 2: all zeros -> only total line
    std::string out2 = runWithInput("2 2\n0 0\n0 0\n");
    assert(out2 == "0\n");

    // Test 3: single component 2x2 all ones
    std::string out3 = runWithInput("2 2\n1 1\n1 1\n");
    assert(out3.substr(0, 2) == "4 ");
    assert(out3.find("\n4\n") != std::string::npos);

    // Test 4: component with an L shape
    std::string out4 = runWithInput("3 3\n1 0 0\n1 0 0\n1 1 0\n");
    // Size 4 component
    assert(out4.substr(0, 2) == "4 ");
    assert(out4.find("\n4\n") != std::string::npos);

    // Test 5: larger grid to ensure iterative works (e.g., 1x5)
    std::string out5 = runWithInput("1 5\n1 1 0 1 1\n");
    // Two components sizes 2 and 2; total 4
    assert(out5.find("2 [1, 1]") != std::string::npos);
    assert(out5.find("2 [1, 4]") != std::string::npos);
    assert(out5.find("\n4\n") != std::string::npos);

    return 0;
}
