// Write a C++ function named `applyImagePyramid` that takes a grayscale image represented as a 2D vector of integers (`std::vector<std::vector<int>>`) and a string direction (`"expand"` or `"reduce"`), and returns a new 2D vector of integers containing the resized image. For `"expand"`, resize the image to dimensions `(rows*2 - 1, cols*2 - 1)` using nearest-neighbor interpolation (i.e., each source pixel maps to a block, but since the size is odd, the mapping must handle non-integer scaling). For `"reduce"`, resize to `(rows/2, cols/2)` using simple averaging of 2x2 blocks (if rows/cols are odd, ignore the last row/column). If the direction is neither `"expand"` nor `"reduce"`, or if the input image is empty, return an empty vector. The function must be `const`-correct and use `const` references where appropriate.
#include <cassert>
#include <vector>
#include <string>

int main() {
    // Basic expand test
    std::vector<std::vector<int>> img1 = {{1, 2}, {3, 4}};
    std::vector<std::vector<int>> exp1 = applyImagePyramid(img1, "expand");
    assert(exp1.size() == 3 && exp1[0].size() == 3);
    assert(exp1[0][0] == 1 && exp1[0][1] == 1 && exp1[0][2] == 2);
    assert(exp1[1][0] == 1 && exp1[1][1] == 1 && exp1[1][2] == 2);
    assert(exp1[2][0] == 3 && exp1[2][1] == 3 && exp1[2][2] == 4);
    
    // Basic reduce test
    std::vector<std::vector<int>> img2 = {{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}, {13, 14, 15, 16}};
    std::vector<std::vector<int>> red2 = applyImagePyramid(img2, "reduce");
    assert(red2.size() == 2 && red2[0].size() == 2);
    assert(red2[0][0] == 3 && red2[0][1] == 5);
    assert(red2[1][0] == 11 && red2[1][1] == 13);
    
    // Odd dimension reduction ignores last row/col
    std::vector<std::vector<int>> img3 = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    std::vector<std::vector<int>> red3 = applyImagePyramid(img3, "reduce");
    assert(red3.size() == 1 && red3[0].size() == 1);
    assert(red3[0][0] == 3); // average of 1,2,4,5 = 12/4 = 3
    
    // Expand a 1x1 image
    std::vector<std::vector<int>> img4 = {{7}};
    std::vector<std::vector<int>> exp4 = applyImagePyramid(img4, "expand");
    assert(exp4.size() == 1 && exp4[0].size() == 1);
    assert(exp4[0][0] == 7);
    
    // Reduce a 1xN image returns empty
    std::vector<std::vector<int>> img5 = {{1, 2, 3}};
    assert(applyImagePyramid(img5, "reduce").empty());
    
    // Empty input returns empty
    std::vector<std::vector<int>> img6;
    assert(applyImagePyramid(img6, "expand").empty());
    
    // Invalid direction returns empty
    std::vector<std::vector<int>> img7 = {{1}, {2}};
    assert(applyImagePyramid(img7, "rotate").empty());
    
    // Large values
    std::vector<std::vector<int>> img8 = {{1000000, -1000000}, {500, -500}};
    std::vector<std::vector<int>> red8 = applyImagePyramid(img8, "reduce");
    assert(red8[0][0] == (1000000 - 1000000 + 500 - 500) / 4); // 0
    std::vector<std::vector<int>> exp8 = applyImagePyramid(img8, "expand");
    assert(exp8[2][2] == -500); // bottom-right corner
}
#include <vector>
#include <string>
#include <algorithm>

// Resize a grayscale image (2D vector of ints) using a pyramid-style expand or reduce.
// expand: enlarge to (2*rows - 1) x (2*cols - 1)
// reduce: shrink to (rows/2) x (cols/2) using 2x2 block averaging
// Returns an empty vector for invalid input or undefined operations.
std::vector<std::vector<int>> applyImagePyramid(
    const std::vector<std::vector<int>>& image,
    const std::string& direction) {
    
    if (image.empty() || image[0].empty()) {
        return {};
    }
    
    const int rows = static_cast<int>(image.size());
    const int cols = static_cast<int>(image[0].size());
    
    if (direction == "expand") {
        const int new_rows = rows * 2 - 1;
        const int new_cols = cols * 2 - 1;
        std::vector<std::vector<int>> result(new_rows, std::vector<int>(new_cols));
        
        for (int i = 0; i < new_rows; ++i) {
            int src_r = i / 2;
            if (src_r >= rows) src_r = rows - 1;
            for (int j = 0; j < new_cols; ++j) {
                int src_c = j / 2;
                if (src_c >= cols) src_c = cols - 1;
                result[i][j] = image[src_r][src_c];
            }
        }
        return result;
    }
    else if (direction == "reduce") {
        const int new_rows = rows / 2;
        const int new_cols = cols / 2;
        if (new_rows == 0 || new_cols == 0) {
            return {};
        }
        std::vector<std::vector<int>> result(new_rows, std::vector<int>(new_cols));
        
        for (int i = 0; i < new_rows; ++i) {
            for (int j = 0; j < new_cols; ++j) {
                int sum = image[2*i][2*j] + image[2*i][2*j+1] +
                          image[2*i+1][2*j] + image[2*i+1][2*j+1];
                result[i][j] = sum / 4;
            }
        }
        return result;
    }
    
    return {};
}
// The solution involves two distinct resizing algorithms. For `"expand"`, the target dimensions are `(2*rows - 1, 2*cols - 1)`. A common approach for nearest-neighbor with this specific size is to map each output pixel `(i,j)` to the source pixel `(i/2, j/2)` when the scaling factor is roughly 2 but with an odd target size. However, a more precise method is to use direct interpolation: for output index `i` in `[0, 2*rows-2]`, the corresponding source index is `min(i/2, rows-1)` (integer division), and similarly for columns. This ensures each source pixel is replicated appropriately, producing a smooth expansion. For `"reduce"`, the target dimensions are `(rows/2, cols/2)`. The straightforward approach is to iterate over each 2x2 block in the source image (starting from the top-left, ignoring the last row and column if dimensions are odd), compute the integer average of the four pixels, and place it in the output at `(block_row, block_col)`. Edge cases include an empty input (return empty vector), rows or cols of 1 (for expand, works fine; for reduce, target dimensions become 0, so return empty vector), and invalid direction string. Time complexity is `O(rows*cols)` for both operations since we iterate over output pixels. Space complexity is `O(rows*cols)` for the output.
