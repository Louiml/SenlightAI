// Write a C++ function `std::string drawHistogram(const std::vector<int>& values)` that takes a non-empty vector of positive integers and returns a string representing a bar chart made of backslash `\` and forward slash `/` characters. Each integer in the vector represents the height change at that step: if the value increases from the previous step, draw `\` characters going upward (one column per unit increase, starting from the previous height+1); if the value decreases, draw `/` characters going downward (one column per unit decrease, starting from the previous height). The chart is drawn on a grid where rows are numbered from 1 upward (row 1 is the bottom), and columns are numbered from 0 leftward. After drawing all steps, the output string contains rows from row 1 up to the highest row that has any non-space character, and columns from column 0 up to the rightmost column that has any non-space character. Empty positions are filled with spaces. The last row printed must be the highest non-space row; any rows above it are omitted. The returned string should have no trailing spaces on any line, and each line should end with `\n` (including the last line). The first value in the vector is treated as a change from an implicit height of 0 (so a positive first value draws `\` upward, and a negative first value would be invalid, but you may assume all inputs produce non-negative intermediate heights after shifting, and all values are integers).  
// Important: The vector may contain negative numbers, and the absolute height values may go below zero; however, your drawing must shift all heights so that the minimum height becomes 0 (i.e., the bottom row is at height 0). For example, if the heights before shifting are { -2, -1, -3 }, then after shifting by +3 they become {1,2,0}, and the drawing uses these shifted heights. The shift does not affect the shape; it only moves the whole chart vertically.  
// The function must return the complete drawn chart as a single string with newlines.
// The solution follows a two‑pass approach. First, compute the absolute height at each point by accumulating the given deltas: start with a running sum `current = 0`; for each element `v[i]`, set `current += v[i]`, and store this cumulative height in an array indexed from 0 to n (where index 0 is the initial height before any step). Then find the minimum cumulative height; shift all heights by subtracting that minimum, so the smallest height becomes 0. This ensures the drawing fits in non‑negative rows. Next, traverse each segment from i‑1 to i (for i from 1 to n). If the new shifted height is greater than the previous, draw `\` characters: for each unit increase, place a `\` at row = previousHeight + j + 1 (since row 1 is the lowest), column = runningColumn, and increment both row and column. If the new shifted height is smaller, draw `/` characters: for each unit decrease, place a `/` at row = previousHeight - j (starting at previousHeight), column = runningColumn, and decrement row while incrementing column. Track the maximum row and maximum column used. After all segments, build the output string by iterating rows from 1 to maxRow and columns from 0 to maxCol, copying the character (space if unused). Remove trailing spaces from each line, then append `\n`. Edge cases: empty vector? The problem guarantees non‑empty. Single element: just draw accordingly. Negative deltas that temporarily go below zero are handled by the shifting step. Time complexity is O(n + R*C) where R and C are the maximum row and column used (in the worst case proportional to the sum of absolute changes). Space complexity is O(R*C) for the grid and O(n) for the cumulative array.
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>

// Draw a bar chart of backslashes and slashes based on height changes.
// Each element of `values` is a change in height from the previous step (starting at 0).
// The chart is shifted so that the minimum height becomes row 0.
// Returns a string with rows from 1 to the highest row used, columns 0 to the rightmost used.
std::string drawHistogram(const std::vector<int>& values) {
    int n = static_cast<int>(values.size());
    if (n == 0) return "";

    // Step 1: compute cumulative heights (absolute) before shifting
    std::vector<int> heights(n + 1, 0);
    int current = 0;
    for (int i = 1; i <= n; ++i) {
        current += values[i - 1];
        heights[i] = current;
    }

    // Step 2: shift so that minimum height becomes 0
    int minHeight = *std::min_element(heights.begin(), heights.end());
    for (int& h : heights) {
        h -= minHeight;
    }

    // Step 3: draw the chart into a grid
    // We will allocate a large enough grid (max possible rows = sum of positive changes + 1)
    // For simplicity, use a vector of strings, but we need to track max row and col.
    // Since we don't know size in advance, use a dynamic approach with a reserve.
    int maxRow = 0, maxCol = 0;
    // We'll use a map-like structure: vector of strings, expanded as needed.
    // But to avoid dynamic resizing, allocate a fixed large area (sum of all absolute changes + 2)
    int totalDelta = 0;
    for (int v : values) totalDelta += std::abs(v);
    int maxPossibleRows = totalDelta + 2; // safe upper bound
    int maxPossibleCols = totalDelta + 2;
    std::vector<std::vector<char>> grid(maxPossibleRows, std::vector<char>(maxPossibleCols, ' '));

    // Start at column 0, row is based on height value (row 1 = height 0 after shift)
    int col = 0;
    for (int i = 1; i <= n; ++i) {
        int prevRow = heights[i - 1] + 1; // +1 because row 1 is height 0
        int curRow = heights[i] + 1;
        if (heights[i] > heights[i - 1]) {
            int startRow = prevRow;
            int step = 0;
            for (int j = 0; j < heights[i] - heights[i - 1]; ++j) {
                int r = startRow + j;
                grid[r][col] = '\\';
                maxRow = std::max(maxRow, r);
                maxCol = std::max(maxCol, col);
                ++col;
            }
        } else {
            int startRow = prevRow;
            int step = 0;
            for (int j = 0; j < heights[i - 1] - heights[i]; ++j) {
                int r = startRow - j;
                grid[r][col] = '/';
                maxRow = std::max(maxRow, r);
                maxCol = std::max(maxCol, col);
                ++col;
            }
        }
    }

    // Step 4: build the output string
    std::ostringstream out;
    for (int r = 1; r <= maxRow; ++r) {
        // Remove trailing spaces from this line
        int lastNonSpace = -1;
        for (int c = 0; c <= maxCol; ++c) {
            if (grid[r][c] != ' ') lastNonSpace = c;
        }
        for (int c = 0; c <= lastNonSpace; ++c) {
            out << grid[r][c];
        }
        out << '\n';
    }
    return out.str();
}
#include <cassert>
#include <string>
#include <vector>

// The solution function is declared above; here we test it.
int main() {
    // Simple increase then decrease
    std::string s1 = drawHistogram({2, -1});
    // heights: 0,2,1; shifted same (min 0). rows: 1: "\\/" (col0 '\' on row1, col1 '/' on row1? actually after increase row1 backslash, then decrease goes from row3 to row2, so rows: row1: "  "? Let's compute manually:
    // heights: [0,2,1]; drawing: increase: row1 '\' at col0, row2 '\' at col1? Wait start at prevRow=1, for two steps: (r=1,c=0) '\' then (r=2,c=1) '\'. Then decrease: prevRow=heights[1]+1=3, start at row3, one step: (r=3,c=2) '/'. maxRow=3, maxCol=2. Output rows 1..3:
    // row1: col0 '\', col1 ' ', col2 ' ' -> "\ "
    // row2: col0 ' ', col1 '\', col2 ' ' -> " \ "
    // row3: col0 ' ', col1 ' ', col2 '/' -> " /"
    // with trailing spaces removed: row1 "\", row2 " \", row3 " /"
    assert(s1 == "\\\n \\\n /\n");

    // Single increase
    std::string s2 = drawHistogram({3});
    // heights: [0,3]; drawing: (1,0) '\' , (2,1) '\' , (3,2) '\'. maxRow=3, maxCol=2.
    // Row1: col0 '\' only -> "\\\n"
    // Row2: col1 '\' -> " \\\n" (actually " \\\n"? col0 space, col1 backslash -> " \\\n")
    // Row3: col2 '\' -> "  \\\n"
    assert(s2 == "\\\n \\\n  \\\n");

    // Negative then positive
    std::string s3 = drawHistogram({-1, 2});
    // heights: [0,-1,1]; shift min -1 -> [1,0,2]
    // increase from 1 to 2? Actually sequence: from height0 to -1 (decrease), then -1 to 1 (increase)
    // Shifted: [1,0,2]; start row = prevRow from height1+1=2? But row must be at least 1. We use height+1 conversion.
    // decrease: prevRow = 1+1=2, one step: (2,0) '/'
    // increase: prevRow = 0+1=1, two steps: (1,1) '\' , (2,2) '\'
    // maxRow=2, maxCol=2.
    // Row1: col0 space, col1 '\', col2 space -> " \\\n"
    // Row2: col0 '/', col1 space, col2 '\' -> "/ \\\n"
    assert(s3 == " \\\n/ \\\n");

    // All zeros? Not allowed because values must be integers but can be zero.
    std::string s4 = drawHistogram({0});
    // heights: [0,0]; no drawing, maxRow=0, maxCol=0, output empty string
    assert(s4 == "");

    // Complex case with multiple segments
    std::string s5 = drawHistogram({1, -1, 1, -1});
    // heights: 0,1,0,1,0 -> shift none
    // Drawing: increase (1,0) '\', decrease (1,1) '/', increase (1,2) '\', decrease (1,3) '/'
    // maxRow=1, maxCol=3
    // Row1: "\\/\\/" (4 chars) -> "\\/\\/\n"
    assert(s5 == "\\/\\/\n");

    return 0;
}
