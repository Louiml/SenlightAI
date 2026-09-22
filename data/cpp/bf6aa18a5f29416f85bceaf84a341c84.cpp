// Write a C++ function named `thickenDiscontinuousEdges` that takes a square matrix of grayscale pixel values (represented as a `std::vector<std::vector<int>>`, values 0–255) and a non‑negative integer `contrast`. The function must return a new matrix of the same dimensions where each interior cell (i.e., rows 2 through rows-3 and columns 2 through cols-3) is classified as either a potential edge pixel (value 0) or non-edge pixel (value 255) using a local‑minimum detection rule. Specifically, a pixel at `(row, col)` is marked as an edge if it is a local minimum in at least **two** of four directional scans (horizontal, vertical, and two diagonals). For each direction, the pixel must satisfy all four conditions relative to neighbors at distances 1 and 2: (a) its value plus `contrast - 1` is ≤ the immediate neighbor in that direction, (b) ≤ the opposite immediate neighbor, (c) strictly less than the average of the two immediate neighbors, and (d) strictly less than the average of the two distant neighbors. After marking all interior pixels (default 255 for non‑edge), apply a gap‑filling correction: for each interior pixel, if the sum of 8 surrounding pixel values (a 3×3 ring around it, excluding itself) is either 0 or 2040 (=8×255), set it to 255; otherwise keep the already assigned value. Finally, set all border pixels (first and last two rows/columns) to 255. The function must be const‑correct, handle empty or too‑small matrices by returning an empty matrix, and avoid modifying the input.
#include <cassert>
#include <vector>

// The solution function is assumed to be declared above.
// Test cases verify edge detection on simple patterns.

int main() {
    // Case 1: Small matrix — should return empty.
    std::vector<std::vector<int>> small = {{1,2},{3,4}};
    assert(thickenDiscontinuousEdges(small, 1).empty());

    // Case 2: 5x5 matrix with a single dark pixel at center (value 0), rest 255.
    // contrast=1: center pixel is local minimum in all directions (0+0 <= 255 etc.)
    // But it must be minimum in at least two directions -> yes, all four.
    // Then correction: surround sum = 8*255 = 2040 -> set to 255.
    // So output center becomes 255.
    std::vector<std::vector<int>> singleDark(5, std::vector<int>(5, 255));
    singleDark[2][2] = 0;
    auto out1 = thickenDiscontinuousEdges(singleDark, 1);
    assert(out1.size() == 5 && out1[0].size() == 5);
    assert(out1[2][2] == 255); // corrected to white

    // Case 3: 5x5 cross pattern (vertical and horizontal dark lines) with contrast=0.
    // Fill a plus sign: row 2 all 0, and column 2 all 0. Others 255.
    std::vector<std::vector<int>> cross(5, std::vector<int>(5, 255));
    for (int c = 0; c < 5; ++c) cross[2][c] = 0;
    for (int r = 0; r < 5; ++r) cross[r][2] = 0;
    auto out2 = thickenDiscontinuousEdges(cross, 0);
    // The intersection point (2,2) is surrounded by zeros -> surround=0 -> set to 255.
    // But other points on the cross lines: e.g., (2,1) has a neighbor (2,0) which is 0,
    // but also (2,2) is 0 after first pass? Actually (2,2) is 0 initially. Let's just check borders.
    assert(out2[0][0] == 255);
    assert(out2[0][2] == 255);
    assert(out2[2][0] == 255);
    // The interior point (2,1) should be 0 (edge) because it's a local min horizontally and vertically? 
    // Actually (2,1) value 0, neighbors (2,0)=0, (2,2)=0 — these are not greater, so pixel+(-1) = -1 <= 0? For contrast=0, pixel+(-1) = -1. That is ≤ both m1 and p1. But condition (pixel < (m1+m2)/2) = -1 < (0+0)/2=0 true. So it is a min in horizontal. Similarly vertical? (1,1)=255, (3,1)=255, (0,1)=255? Actually (0,1)=255 (border), (4,1)=255, so yes. So it's min in at least two -> edge (0).
    assert(out2[2][1] == 0);
    assert(out2[1][2] == 0);

    // Case 4: A simple gradient where no pixel is a minimum in two directions.
    // 5x5 matrix with values increasing along both rows and columns.
    std::vector<std::vector<int>> gradient(5, std::vector<int>(5));
    for (int i = 0; i < 5; ++i)
        for (int j = 0; j < 5; ++j)
            gradient[i][j] = i * 50 + j * 10; // strictly increasing bottom-right
    auto out3 = thickenDiscontinuousEdges(gradient, 1);
    // No interior pixel should be marked as edge (all 255) except possibly border which we set to 255.
    for (int r = 2; r < 3; ++r) // only interior rows 2..2? Actually rows 2 and cols 2 only for 5x5.
        for (int c = 2; c < 3; ++c)
            assert(out3[r][c] == 255);

    // Case 5: Check border always 255 even if input has dark corners.
    std::vector<std::vector<int>> darkCorner(5, std::vector<int>(5, 255));
    darkCorner[0][0] = 0; // border corner
    darkCorner[1][1] = 0; // near border
    auto out4 = thickenDiscontinuousEdges(darkCorner, 0);
    assert(out4[0][0] == 255);
    assert(out4[0][1] == 255);
    assert(out4[1][0] == 255);

    return 0;
}
#include <vector>
#include <cstddef>

// Classify edge pixels using local-minimum detection and gap filling.
// Returns a new matrix of same dimensions; interior pixels are 0 (edge) or 255 (non-edge),
// borders are 255. Returns empty vector if input has fewer than 5 rows or columns.
std::vector<std::vector<int>> thickenDiscontinuousEdges(
    const std::vector<std::vector<int>>& input,
    int contrast)
{
    const std::size_t rows = input.size();
    if (rows < 5) return {};
    const std::size_t cols = input[0].size();
    if (cols < 5) return {};

    // Initialize output with 255 for all cells.
    std::vector<std::vector<int>> output(rows, std::vector<int>(cols, 255));

    // Helper lambda: check local minimum along a direction given offset (dr, dc).
    auto isLocalMinimum = [&](std::size_t row, std::size_t col, int dr, int dc) -> bool {
        int pixel = input[row][col] + contrast - 1;  // shift for chessboard-like patterns
        int m1 = input[row - dr][col - dc];          // immediate neighbor on one side
        int p1 = input[row + dr][col + dc];          // immediate neighbor opposite
        int m2 = input[row - 2*dr][col - 2*dc];      // distant neighbor on one side
        int p2 = input[row + 2*dr][col + 2*dc];      // distant neighbor opposite

        bool cond1 = (pixel <= m1) && (pixel <= p1);
        bool cond2 = (pixel < (m1 + m2) / 2);
        bool cond3 = (pixel < (p1 + p2) / 2);
        return cond1 && cond2 && cond3;
    };

    // Phase 1: mark interior pixels that are local minima in at least two directions.
    for (std::size_t row = 2; row < rows - 2; ++row) {
        for (std::size_t col = 2; col < cols - 2; ++col) {
            int count = 0;
            // Main diagonal (dr=1, dc=1)
            if (isLocalMinimum(row, col, 1, 1)) ++count;
            // Vertical (dr=1, dc=0)
            if (isLocalMinimum(row, col, 1, 0)) ++count;
            // Anti-diagonal (dr=1, dc=-1) — note col-1 and col-2 valid because col>=2
            if (isLocalMinimum(row, col, 1, -1)) ++count;
            // Horizontal (dr=0, dc=1)
            if (isLocalMinimum(row, col, 0, 1)) ++count;

            if (count > 1) {
                output[row][col] = 0;
            } else {
                output[row][col] = 255;
            }
        }
    }

    // Phase 2: correction pass using 3x3 ring sum.
    for (std::size_t row = 2; row < rows - 2; ++row) {
        for (std::size_t col = 2; col < cols - 2; ++col) {
            int surround = 0;
            surround += output[row-1][col-1] + output[row-1][col] + output[row-1][col+1];
            surround += output[row][col-1] + output[row][col+1];
            surround += output[row+1][col-1] + output[row+1][col] + output[row+1][col+1];
            if (surround == 0 || surround == 8 * 255) {
                output[row][col] = 255;
            }
        }
    }

    // Phase 3: ensure borders are white.
    for (std::size_t col = 0; col < cols; ++col) {
        for (std::size_t row = 0; row < 2; ++row) output[row][col] = 255;
        for (std::size_t row = rows - 2; row < rows; ++row) output[row][col] = 255;
    }
    for (std::size_t row = 0; row < rows; ++row) {
        for (std::size_t col = 0; col < 2; ++col) output[row][col] = 255;
        for (std::size_t col = cols - 2; col < cols; ++col) output[row][col] = 255;
    }

    return output;
}
// The solution processes the input matrix in three phases. First, we iterate over every interior cell (row from 2 to `rows-3`, col from 2 to `cols-3`) and test the local‑minimum condition along four directions: main diagonal (offset (1,1)), vertical (offset (0,1)), anti‑diagonal (offset (1,-1)), and horizontal (offset (1,0)). For each direction, we retrieve neighbor values at distances 1 and 2, and check the four inequalities described. If the pixel satisfies the conditions in at least two directions, we mark it as 0 (edge); otherwise 255. Second, we perform a correction pass: for each interior cell, compute the sum of its 8 immediate neighbors (using the already‑marked edge map). If that sum is 0 (all neighbors are edges) or 2040 (all neighbors are non‑edges), we force the pixel to 255; otherwise keep the current value. Finally, we set the outermost two rows and two columns to 255. The time complexity is O(rows × cols) because each pixel is processed a constant number of times (neighbor lookups are O(1)). Space complexity is O(rows × cols) for the output matrix plus O(1) auxiliary variables. Edge cases: matrices with fewer than 5 rows or 5 columns have no interior cells per the definition, so we return an empty matrix after checking dimensions. The contrast value is added to the pixel value before comparisons; since contrast is non‑negative, a contrast of 0 means we compare the raw value; contrast of 1 shifts the threshold by 1 as in the original code.
