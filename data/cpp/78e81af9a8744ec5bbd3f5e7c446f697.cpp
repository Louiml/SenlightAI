Given a grid with R rows and C columns where every cell is initially available, and a sequence of Q rectangular queries each specified by top-left corner (lx, ly) and bottom-right corner (rx, ry), write a C++ function that processes all queries and outputs, for each query in order, the count and list (in row-major order) of cells that are still available and lie within the rectangle, and then marks those cells as no longer available for future queries. The grid coordinates are 1-indexed (1 ≤ row ≤ R, 1 ≤ col ≤ C). The function should return a vector of strings, one per query, formatted as: `<count> (<row1>, <col1>) (<row2>, <col2>) ...` (with a space before each parenthesis and no trailing space; if count is 0, just "0"). The function signature is `std::vector<std::string> processQueries(int R, int C, const std::vector<std::array<int,4>>& queries)` where each query is `{lx, ly, rx, ry}` with 1 ≤ lx ≤ rx ≤ R and 1 ≤ ly ≤ ry ≤ C. You must ensure that once a cell is reported as available in an earlier query, it is never reported again in later queries.
The core challenge is to efficiently find and remove all remaining available cells in a subrectangle. A naive approach iterating over all cells in each query would take O(R*C*Q) time. Instead, we can use a set of available columns for each row. For each row, store the columns that are still available in a `std::set<int>`. Initially every column 1..C is present in each row's set. For each query, iterate over rows from lx to rx. For a given row, use `lower_bound(ly)` to find the first available column ≥ ly, then iterate through the set while the column ≤ ry. For each found column, record the cell `(row, col)` in a temporary list and erase it from that row's set (using the iterator to avoid invalidation). This way, each cell is processed and removed exactly once across all queries. The total number of removals across all queries is at most R*C. For each query, the time spent is proportional to the number of removed cells plus the number of rows scanned (even if a row has no available columns in range, we still perform a lower_bound). The asymptotic complexity is O(R*C + Q*R*log(C)) in the worst case, since for each row in each query we do a binary search (log C) even if no cells are found. However, the sum of `ret.size()` over all queries is O(R*C). Space complexity is O(R*C) for the sets (storing all initial columns). Edge cases include empty rectangles (not possible given constraints), rows with no available columns, and queries where the rectangle contains cells all already removed—then the result is just "0". Also note that coordinates are 1-indexed, so we need to allocate sets of size R+1.
#include <vector>
#include <string>
#include <set>
#include <array>
#include <sstream>

// Process rectangle queries and return formatted results.
// Each query is {lx, ly, rx, ry} with 1-based coordinates.
// Returns a string per query: "<count> (<row>, <col>) ..."
std::vector<std::string> processQueries(int R, int C, const std::vector<std::array<int,4>>& queries) {
    // Each row has a set of available columns (1..C initially).
    std::vector<std::set<int>> available(R + 1);
    for (int row = 1; row <= R; ++row) {
        for (int col = 1; col <= C; ++col) {
            available[row].insert(col);
        }
    }

    std::vector<std::string> results;
    results.reserve(queries.size());

    for (const auto& q : queries) {
        int lx = q[0], ly = q[1], rx = q[2], ry = q[3];
        std::vector<std::pair<int,int>> found;
        found.reserve((rx - lx + 1) * (ry - ly + 1)); // upper bound

        for (int row = lx; row <= rx; ++row) {
            auto it = available[row].lower_bound(ly);
            while (it != available[row].end() && *it <= ry) {
                found.emplace_back(row, *it);
                it = available[row].erase(it); // erase returns next iterator
            }
        }

        // Build output string
        std::ostringstream oss;
        oss << found.size();
        for (const auto& cell : found) {
            oss << " (" << cell.first << ", " << cell.second << ")";
        }
        results.push_back(oss.str());
    }

    return results;
}
#include <cassert>
#include <vector>
#include <array>
#include <string>

// Include the solution function here (or link it) — for brevity, assume it's above.

int main() {
    // Example 1: basic rectangle, all cells available
    {
        int R = 3, C = 4;
        std::vector<std::array<int,4>> q = {{1,1,2,2}};
        auto res = processQueries(R, C, q);
        std::string expected = "4 (1, 1) (1, 2) (2, 1) (2, 2)";
        assert(res.size() == 1);
        assert(res[0] == expected);
    }

    // Example 2: overlapping queries — cells not repeated
    {
        int R = 2, C = 3;
        std::vector<std::array<int,4>> q = {{1,1,2,2}, {1,2,2,3}};
        auto res = processQueries(R, C, q);
        assert(res.size() == 2);
        assert(res[0] == "4 (1, 1) (1, 2) (2, 1) (2, 2)");
        // Second query: remaining cells in rectangle (1,2)-(2,3) are only (1,3) and (2,3)
        assert(res[1] == "2 (1, 3) (2, 3)");
    }

    // Example 3: query after all cells in region already taken
    {
        int R = 1, C = 2;
        std::vector<std::array<int,4>> q = {{1,1,1,2}, {1,1,1,2}};
        auto res = processQueries(R, C, q);
        assert(res[0] == "2 (1, 1) (1, 2)");
        assert(res[1] == "0");
    }

    // Example 4: single row, single column
    {
        int R = 1, C = 1;
        std::vector<std::array<int,4>> q = {{1,1,1,1}};
        auto res = processQueries(R, C, q);
        assert(res[0] == "1 (1, 1)");
    }

    // Example 5: non-square rectangle, multiple queries with gaps
    {
        int R = 2, C = 3;
        std::vector<std::array<int,4>> q = {{1,1,2,1}, {1,2,1,3}, {2,2,2,3}};
        auto res = processQueries(R, C, q);
        assert(res[0] == "2 (1, 1) (2, 1)");
        assert(res[1] == "2 (1, 2) (1, 3)");
        assert(res[2] == "2 (2, 2) (2, 3)");
    }
}
