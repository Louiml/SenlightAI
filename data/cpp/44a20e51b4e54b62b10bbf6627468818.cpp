Write a C++ function that takes a 2D integer matrix (represented as a vector of vectors) with even dimensions (number of rows and columns both even and at least 2), a list of query integers, and returns for each query a string containing all positions (row and column, space-separated, each pair on a new line) where the query value appears in the matrix, but only searching the four quadrants in this specific order:  
1) top-left quadrant (rows 0..n/2-1, columns 0..m/2-1),  
2) bottom-left quadrant (rows n/2..n-1, columns 0..m/2-1),  
3) top-right quadrant (rows 0..n/2-1, columns m/2..m-1),  
4) bottom-right quadrant (rows n/2..n-1, columns m/2..m-1).  
If the value appears multiple times in the same quadrant, include all positions, but do not include duplicates that appear in different quadrants (each position appears in exactly one quadrant). If the value is not found at all, return "NONE" for that query. The output for each query should be a single string with positions separated by newlines, and no trailing newline at the end. The function must be `const`-correct and handle edge cases like queries that appear zero, one, or many times, and possibly negative numbers.

#include <cassert>
#include <vector>
#include <string>

int main() {
    // Test 1: Simple 2x2 matrix, value appears once
    std::vector<std::vector<int>> m1 = {{1, 2}, {3, 4}};
    assert(findPositionsInQuadrants(m1, 1) == "0 0");
    assert(findPositionsInQuadrants(m1, 4) == "1 1");
    assert(findPositionsInQuadrants(m1, 5) == "NONE");

    // Test 2: 4x4 matrix, value appears in multiple quadrants
    std::vector<std::vector<int>> m2 = {
        {1, 1, 2, 2},
        {1, 1, 2, 2},
        {3, 3, 4, 4},
        {3, 3, 4, 4}
    };
    // Query 2 appears in top-right quadrant (row 0, col 2 and row 0, col 3)
    // and also row 1, col 2 and row 1, col 3. Quadrant order: top-left first,
    // then bottom-left, then top-right, then bottom-right.
    assert(findPositionsInQuadrants(m2, 2) == "0 2\n0 3\n1 2\n1 3");
    // Query 3 appears in bottom-left quadrant (rows 2-3, cols 0-1)
    assert(findPositionsInQuadrants(m2, 3) == "2 0\n2 1\n3 0\n3 1");

    // Test 3: Value appears multiple times within same quadrant
    std::vector<std::vector<int>> m3 = {
        {7, 7, 1, 1},
        {7, 7, 1, 1},
        {2, 2, 3, 3},
        {2, 2, 3, 3}
    };
    assert(findPositionsInQuadrants(m3, 7) == "0 0\n0 1\n1 0\n1 1");
    assert(findPositionsInQuadrants(m3, 3) == "2 2\n2 3\n3 2\n3 3");

    // Test 4: Larger matrix with negative numbers and duplicate across quadrants
    std::vector<std::vector<int>> m4 = {
        {-5, 0, -5, 10},
        {1, -5, 11, 12},
        {-5, 20, -5, 30},
        {40, 50, 60, -5}
    };
    // -5 appears in top-left (0,0),(1,1); bottom-left (2,0); top-right (0,2);
    // bottom-right (3,3). Order: TL, BL, TR, BR.
    assert(findPositionsInQuadrants(m4, -5) == "0 0\n1 1\n2 0\n0 2\n3 3");

    // Test 5: No matches
    assert(findPositionsInQuadrants(m4, 999) == "NONE");

    return 0;
}

#include <vector>
#include <string>
#include <sstream>

// Given a matrix with even dimensions, returns a string listing all positions
// of each query value found in the four quadrants in the order: top-left,
// bottom-left, top-right, bottom-right. Returns "NONE" if not found.
std::string findPositionsInQuadrants(
        const std::vector<std::vector<int>>& matrix,
        int query) {
    const int n = static_cast<int>(matrix.size());
    const int m = n > 0 ? static_cast<int>(matrix[0].size()) : 0;
    const int halfRows = n / 2;
    const int halfCols = m / 2;

    std::vector<std::pair<int,int>> positions;

    // Quadrant 1: top-left (rows 0..halfRows-1, cols 0..halfCols-1)
    for (int i = 0; i < halfRows; ++i) {
        for (int j = 0; j < halfCols; ++j) {
            if (matrix[i][j] == query) {
                positions.emplace_back(i, j);
            }
        }
    }

    // Quadrant 2: bottom-left (rows halfRows..n-1, cols 0..halfCols-1)
    for (int i = halfRows; i < n; ++i) {
        for (int j = 0; j < halfCols; ++j) {
            if (matrix[i][j] == query) {
                positions.emplace_back(i, j);
            }
        }
    }

    // Quadrant 3: top-right (rows 0..halfRows-1, cols halfCols..m-1)
    for (int i = 0; i < halfRows; ++i) {
        for (int j = halfCols; j < m; ++j) {
            if (matrix[i][j] == query) {
                positions.emplace_back(i, j);
            }
        }
    }

    // Quadrant 4: bottom-right (rows halfRows..n-1, cols halfCols..m-1)
    for (int i = halfRows; i < n; ++i) {
        for (int j = halfCols; j < m; ++j) {
            if (matrix[i][j] == query) {
                positions.emplace_back(i, j);
            }
        }
    }

    if (positions.empty()) {
        return "NONE";
    }

    std::ostringstream output;
    for (size_t idx = 0; idx < positions.size(); ++idx) {
        if (idx > 0) {
            output << '\n';
        }
        output << positions[idx].first << ' ' << positions[idx].second;
    }
    return output.str();
}

// The solution splits the matrix into four rectangular blocks defined by the half indices `n/2` and `m/2`. For each query, we iterate through the quadrants in the exact order specified. For each quadrant, we scan every cell and if the cell equals the query value, we append `"row col"` to a string (adding a newline before each subsequent pair). After scanning all quadrants, if the string is empty, return `"NONE"`; otherwise return the built string. Important edge cases: dimensions are guaranteed even and at least 2, so `n/2` and `m/2` are at least 1; the same value may appear in multiple quadrants and that's fine because we scan all quadrants, but a value can appear multiple times within a single quadrant and we must include all occurrences. Time complexity is \(O(t \cdot n \cdot m)\) where `t` is the number of queries, because for each query we scan all cells once. Space complexity is \(O(n \cdot m)\) for the input matrix plus the output string size, but auxiliary space is proportional to the number of matches per query.
