/*
Write a standalone C++ function named `computeLocalBinaryPattern` that takes a 2D grayscale image represented as a `std::vector<std::vector<int>>` (with non-negative pixel values) and returns a `std::vector<std::vector<int>>` of the same dimensions, where each pixel in the output is replaced by an 8-bit unsigned integer (stored as `int`) encoding the local binary pattern (LBP) code. The LBP code for a pixel at `(i,j)` is computed by comparing its value with its 8 immediate neighbors (up, down, left, right, and four diagonals). For each neighbor, starting from the top-left neighbor and proceeding clockwise (top-left, top, top-right, right, bottom-right, bottom, bottom-left, left), set the corresponding bit (bit 7 for the first neighbor down to bit 0 for the last) to 1 if the neighbor's value is strictly greater than the center pixel's value, otherwise 0. For pixels on the border (where not all 8 neighbors exist), set the output value to 0. Ensure the function is reusable across multiple image sizes and handle inputs gracefully for images smaller than 3×3 (return an all-zero image of same dimensions). The function should not modify the input image.
*/
#include <vector>

/**
 * Computes the 8-neighbor Local Binary Pattern (LBP) code for each interior pixel.
 * For border pixels, sets the output value to 0.
 *
 * @param image Input grayscale image with non-negative pixel values.
 * @return Output image of same dimensions, each pixel holding an 8-bit LBP code as int.
 */
std::vector<std::vector<int>> computeLocalBinaryPattern(const std::vector<std::vector<int>>& image) {
    if (image.empty() || image[0].empty()) {
        return {};
    }

    const int rows = static_cast<int>(image.size());
    const int cols = static_cast<int>(image[0].size());
    std::vector<std::vector<int>> result(rows, std::vector<int>(cols, 0));

    // If image is too small, all pixels are border pixels -> all zeros already set.
    if (rows < 3 || cols < 3) {
        return result;
    }

    // Neighbor offsets in clockwise order: top-left, top, top-right, right,
    // bottom-right, bottom, bottom-left, left.
    const int dx[8] = {-1, -1, -1, 0, 1, 1, 1, 0};
    const int dy[8] = {-1, 0, 1, 1, 1, 0, -1, -1};

    for (int i = 1; i < rows - 1; ++i) {
        for (int j = 1; j < cols - 1; ++j) {
            int center = image[i][j];
            int code = 0;
            for (int n = 0; n < 8; ++n) {
                int ni = i + dx[n];
                int nj = j + dy[n];
                if (image[ni][nj] > center) {
                    code |= (1 << (7 - n)); // bit 7 for n=0, bit 0 for n=7
                }
            }
            result[i][j] = code;
        }
    }

    return result;
}
#include <cassert>
#include <vector>

// The solution function is declared here (assume it is included above).
std::vector<std::vector<int>> computeLocalBinaryPattern(const std::vector<std::vector<int>>& image);

int main() {
    // Test 1: Simple 3x3 image with known pattern (center=5, neighbors produce code)
    std::vector<std::vector<int>> img1 = {{1, 9, 2}, {8, 5, 7}, {3, 6, 4}};
    auto out1 = computeLocalBinaryPattern(img1);
    // Neighbors clockwise from top-left: 1,9,2,7,4,6,3,8
    // Comparisons >5: 9,7,6,8 => bits: 7(no),6(yes),5(no),4(yes),3(no),2(yes),1(no),0(yes)
    // binary: 0 1 0 1 0 1 0 1 = 0x55 = 85
    assert(out1[1][1] == 85);
    // Border pixels should be 0
    assert(out1[0][0] == 0);
    assert(out1[2][2] == 0);

    // Test 2: All neighbors equal to center -> all bits 0
    std::vector<std::vector<int>> img2 = {{5,5,5},{5,5,5},{5,5,5}};
    auto out2 = computeLocalBinaryPattern(img2);
    assert(out2[1][1] == 0);

    // Test 3: All neighbors strictly greater -> all bits 1 = 255
    std::vector<std::vector<int>> img3 = {{10,10,10},{10,0,10},{10,10,10}};
    auto out3 = computeLocalBinaryPattern(img3);
    assert(out3[1][1] == 255);

    // Test 4: Image smaller than 3x3 -> all zeros
    std::vector<std::vector<int>> img4 = {{1,2},{3,4}};
    auto out4 = computeLocalBinaryPattern(img4);
    assert(out4.size() == 2 && out4[0].size() == 2);
    assert(out4[0][0] == 0 && out4[1][1] == 0);

    // Test 5: Larger image, check a few interior codes manually
    // 5x5 with center at (2,2)=0, all other neighbors = 1
    std::vector<std::vector<int>> img5(5, std::vector<int>(5, 1));
    img5[2][2] = 0;
    auto out5 = computeLocalBinaryPattern(img5);
    // All 8 neighbors are 1 > 0 -> all bits set = 255
    assert(out5[2][2] == 255);
    // Check a corner interior pixel (1,1): center=1, neighbors: (0,0)=1,(0,1)=1,(0,2)=1,(1,2)=1,(2,2)=0,(2,1)=1,(2,0)=1,(1,0)=1
    // Comparisons >1: only (2,2)=0 is not greater, all others equal -> none greater -> 0
    assert(out5[1][1] == 0);

    // Test 6: Single row image (1xN) -> all zeros
    std::vector<std::vector<int>> img6 = {{1,2,3}};
    auto out6 = computeLocalBinaryPattern(img6);
    assert(out6.size() == 1 && out6[0].size() == 3);
    assert(out6[0][0] == 0 && out6[0][1] == 0 && out6[0][2] == 0);

    // Test 7: Negative values not allowed? Assume non-negative only. Test with zero and positive values.
    // Test 8: Image with zeros and ones, center=0, others 1 -> all bits set.
    std::vector<std::vector<int>> img8 = {{1,1,1},{1,0,1},{1,1,1}};
    auto out8 = computeLocalBinaryPattern(img8);
    assert(out8[1][1] == 255);

    return 0;
}
// The solution iterates over every pixel in the input image using nested loops. For each interior pixel (indices from 1 to rows-2 and 1 to cols-2), we collect the values of its 8 neighbors in clockwise order starting from the top-left. We initialize an `unsigned char` (but stored as `int` in the output) code to 0. For each neighbor, we compare it with the center pixel; if strictly greater, we set the corresponding bit using bitwise OR with `(1 << bitPosition)`, where bitPosition starts at 7 and decrements to 0 for each neighbor in the specified order. After processing all neighbors, we assign the resulting code to the output at the same coordinates. For border pixels, we assign 0. Edge cases: if the image has fewer than 3 rows or 3 columns, no pixel has all 8 neighbors, so the output is entirely zeros of the same dimensions. The algorithm runs in O(rows × cols) time because each pixel is visited a constant number of times (8 neighbor accesses per interior pixel). Space complexity is O(rows × cols) for the output matrix, plus O(1) extra space for temporary variables, not counting the input storage.
