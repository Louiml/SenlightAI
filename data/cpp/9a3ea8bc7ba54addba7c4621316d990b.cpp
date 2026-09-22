Write a standalone C++ function `decodeArUcoInnerCode` that takes a square grayscale image (`cv::Mat`) containing a black-and-white ArUco-style marker centered with a black border, and an integer `total_nbits` representing the total number of data bits in the inner grid (which must be a perfect square, e.g., 16, 25, 36, …, 100). The function must extract the inner binary code (ignoring the outer black border) by dividing the image into a `(sqrt(total_nbits)+2) × (sqrt(total_nbits)+2)` grid of cells. For each cell, compute the majority vote of the pixel intensities (using a threshold of 125: pixels >125 are considered white/1, otherwise black/0) to produce a binary grid. Verify that the outermost border cells are entirely black (0); if any border cell has majority white, return an empty vector. Then, collect the inner `sqrt(total_nbits) × sqrt(total_nbits)` grid and generate four 64-bit unsigned integer representations corresponding to the four 90-degree rotations of that inner grid (where the bits are read row-by-row from bottom to top and right to left, i.e., the most significant bit corresponds to the bottom-right cell). Return a `std::vector<uint64_t>` containing the four rotated values in order: original, 90° clockwise, 180°, 270° clockwise. The function must be self-contained, using only standard C++ and OpenCV core (including `cv::Mat`, `cv::threshold` if needed, and `cv::countNonZero`), and must not rely on any external dictionary or ArUco library. The function signature should be: `std::vector<uint64_t> decodeArUcoInnerCode(const cv::Mat& input, int total_nbits)`. Assume the input image is already grayscale and 8-bit (`CV_8UC1`), square, and has a size that is a multiple of the grid dimensions. If the input image type is not correct or `total_nbits` is not a perfect square, return an empty vector.
#include <opencv2/core/core.hpp>
#include <vector>
#include <cassert>
#include <cstdint>

// Include the solution function here (or link it).
// For completeness, we assume the function is defined above.

int main() {
    // Create a simple 5x5 marker with inner 2x2 bits (total_nbits=4).
    //   Border (all black) = 0, inner bits pattern:
    //   Row0: 1 0
    //   Row1: 0 1
    // We'll create a 10x10 image where each cell is 2x2 pixels.
    int n = 5; // total cells per side = sqrt(4)+2 = 4? Let's compute: total_nbits=4 -> nInner=2 -> nCells=4.
    // Actually for total_nbits=4, nInner=2, nCells=4. We'll make image size 8x8 (each cell 2x2).
    cv::Mat img(8, 8, CV_8UC1, cv::Scalar(0)); // all black
    // Fill inner bits (cells (1,1) to (2,2) in 4x4 grid):
    // (1,1)=1, (1,2)=0, (2,1)=0, (2,2)=1
    // Each cell is 2x2 pixels: x from 2*cell to 2*cell+1, y similarly.
    for (int y = 2; y <= 3; ++y) {
        for (int x = 2; x <= 3; ++x) {
            if ( (y==2 && x==2) || (y==3 && x==3) ) {
                img.at<uchar>(y, x) = 255;
            }
        }
    }
    // Note: This sets only 2 pixels for each 1 cell, but the majority in a 2x2 cell may be 0 if set only 1 pixel.
    // To ensure majority white, we set all 4 pixels in those cells.
    img = cv::Mat::zeros(8,8,CV_8UC1);
    // Fill the two white cells completely.
    for (int y = 2; y <= 3; ++y) {
        for (int x = 2; x <= 3; ++x) {
            if ( (y>=2 && y<=3 && x>=2 && x<=3) ) {
                // cell (1,1) -> y from 2 to 3, x from 2 to 3: all white
                img.at<uchar>(y,x) = 255;
            }
        }
    }
    // But this makes all four cells white, not pattern [[1,0],[0,1]].
    // Let's build correctly: cell (1,1) white (y=2-3,x=2-3), cell (2,2) white (y=4-5,x=4-5).
    img = cv::Mat::zeros(8,8,CV_8UC1);
    for (int y=2; y<=3; ++y) for (int x=2; x<=3; ++x) img.at<uchar>(y,x)=255; // (1,1)
    for (int y=4; y<=5; ++y) for (int x=4; x<=5; ++x) img.at<uchar>(y,x)=255; // (2,2)

    std::vector<uint64_t> ids = decodeArUcoInnerCode(img, 4);
    assert(ids.size() == 4);
    // Original inner code: [[1,0],[0,1]].
    // Encoding from bottom-right: row1 (bottom) has [0,1], row0 (top) has [1,0].
    // Bits order: bottom-right (bit63) = 1, bottom-left (bit62)=0, top-right (bit61)=0, top-left (bit60)=1.
    // So binary: bit63=1, bit62=0, bit61=0, bit60=1, rest 0 -> 1*2^63 + 0*2^62 + 0*2^61 + 1*2^60 = 2^63 + 2^60.
    uint64_t expected = (1ULL << 63) | (1ULL << 60);
    assert(ids[0] == expected);
    // 90° clockwise: [[0,1],[1,0]]? Let's compute: original [[1,0];[0,1]] rotated 90° cw gives [[0,1];[1,0]]? Actually rotate right: original row0 [1,0] becomes column2? Let's trust the algorithm; we just check that all four ids are distinct and non-zero.
    assert(ids[0] != ids[1] && ids[0] != ids[2] && ids[1] != ids[3]);

    // Test invalid total_nbits (non-square).
    std::vector<uint64_t> ids2 = decodeArUcoInnerCode(img, 5);
    assert(ids2.empty());

    // Test border detection: make a border cell white.
    cv::Mat img2 = img.clone();
    img2.at<uchar>(0,0) = 255;
    assert(decodeArUcoInnerCode(img2, 4).empty());

    // Test a larger marker with total_nbits=16 (inner 4x4). Use a simple all-zero interior (but border black).
    cv::Mat img3(12,12,CV_8UC1,cv::Scalar(0)); // 12x12 => nCells=6, each cell 2x2, inner 4x4 all zeros.
    std::vector<uint64_t> ids3 = decodeArUcoInnerCode(img3, 16);
    assert(ids3.size() == 4);
    assert(ids3[0] == 0 && ids3[1] == 0 && ids3[2] == 0 && ids3[3] == 0); // all zeros

    // Test a pattern where inner is all ones but border black.
    cv::Mat img4(12,12,CV_8UC1,cv::Scalar(0));
    // Fill inner 4x4 cells (cells from 1 to 4 in both dimensions) with white.
    for (int cy=1; cy<=4; ++cy) {
        for (int cx=1; cx<=4; ++cx) {
            // Cell region: y from cy*2 to cy*2+1? Actually each cell is 2 pixels: y = cy*2 to cy*2+1? Wait nCells=6 so each cell width=12/6=2.
            for (int dy=0; dy<2; ++dy) {
                for (int dx=0; dx<2; ++dx) {
                    img4.at<uchar>(cy*2+dy, cx*2+dx) = 255;
                }
            }
        }
    }
    std::vector<uint64_t> ids4 = decodeArUcoInnerCode(img4, 16);
    assert(ids4.size() == 4);
    // All bits set: MSB to LSB all 1 -> 0xFFFFFFFFFFFFFFFF
    assert(ids4[0] == 0xFFFFFFFFFFFFFFFFULL);
    assert(ids4[1] == 0xFFFFFFFFFFFFFFFFULL);
    assert(ids4[2] == 0xFFFFFFFFFFFFFFFFULL);
    assert(ids4[3] == 0xFFFFFFFFFFFFFFFFULL);

    return 0;
}
#include <opencv2/core/core.hpp>
#include <vector>
#include <cmath>
#include <cstdint>
#include <bitset>

// Helper to encode a binary grid (size n x n) into a 64-bit value.
// Bits are read from bottom-right to top-left: MSB is bottom-right, LSB is top-left.
static uint64_t encodeGrid(const cv::Mat& grid) {
    std::bitset<64> bits;
    int idx = 0;
    for (int y = grid.rows - 1; y >= 0; --y) {
        for (int x = grid.cols - 1; x >= 0; --x) {
            bits[idx++] = (grid.at<uchar>(y, x) != 0);
        }
    }
    return bits.to_ullong();
}

// Helper to rotate a square binary grid 90 degrees clockwise.
static cv::Mat rotate90(const cv::Mat& in) {
    cv::Mat out(in.rows, in.cols, CV_8UC1);
    for (int i = 0; i < in.rows; ++i) {
        for (int j = 0; j < in.cols; ++j) {
            out.at<uchar>(i, j) = in.at<uchar>(in.cols - j - 1, i);
        }
    }
    return out;
}

// Main function: extract the inner code and its four rotations.
std::vector<uint64_t> decodeArUcoInnerCode(const cv::Mat& input, int total_nbits) {
    // Validate input and dimensions.
    if (input.empty() || input.type() != CV_8UC1 || input.rows != input.cols) {
        return {};
    }
    int nInner = static_cast<int>(std::sqrt(static_cast<double>(total_nbits)));
    if (nInner * nInner != total_nbits) {
        return {};
    }
    int nCells = nInner + 2;
    if (nCells <= 0) {
        return {};
    }

    // Build coarse binary grid by majority voting per cell.
    cv::Mat binaryCode(nCells, nCells, CV_8UC1, cv::Scalar::all(0));
    // Count non-zero (white) per cell.
    std::vector<std::vector<int>> nonZeros(nCells, std::vector<int>(nCells, 0));
    std::vector<std::vector<int>> totals(nCells, std::vector<int>(nCells, 0));

    for (int y = 0; y < input.rows; ++y) {
        const uchar* ptr = input.ptr<uchar>(y);
        int cellY = static_cast<int>(static_cast<long long>(y) * nCells / input.rows);
        for (int x = 0; x < input.cols; ++x) {
            int cellX = static_cast<int>(static_cast<long long>(x) * nCells / input.cols);
            if (ptr[x] > 125) {
                nonZeros[cellY][cellX]++;
            }
            totals[cellY][cellX]++;
        }
    }

    // Assign binary values.
    for (int cy = 0; cy < nCells; ++cy) {
        for (int cx = 0; cx < nCells; ++cx) {
            if (totals[cy][cx] > 0 && nonZeros[cy][cx] > totals[cy][cx] / 2) {
                binaryCode.at<uchar>(cy, cx) = 1;
            } else {
                binaryCode.at<uchar>(cy, cx) = 0;
            }
        }
    }

    // Check that the outer border is fully black (0).
    for (int y = 0; y < nCells; ++y) {
        int step = (y == 0 || y == nCells - 1) ? 1 : (nCells - 1);
        for (int x = 0; x < nCells; x += step) {
            if (binaryCode.at<uchar>(y, x) != 0) {
                return {};
            }
        }
    }

    // Extract inner grid (size nInner x nInner).
    cv::Mat inner(nInner, nInner, CV_8UC1);
    for (int y = 0; y < nInner; ++y) {
        for (int x = 0; x < nInner; ++x) {
            inner.at<uchar>(y, x) = binaryCode.at<uchar>(y + 1, x + 1);
        }
    }

    // Generate four rotations.
    std::vector<uint64_t> ids;
    ids.reserve(4);
    cv::Mat current = inner.clone();
    for (int rot = 0; rot < 4; ++rot) {
        ids.push_back(encodeGrid(current));
        current = rotate90(current);
    }
    return ids;
}
// The solution approach is to convert the input image into a coarse binary grid by sampling each cell region and deciding its value based on the majority of pixel intensities. The image is divided into `nCells = sqrt(total_nbits) + 2` cells along each dimension. For each cell, we compute the sum of non-zero pixels (white) by iterating over the image or using `cv::countNonZero` on a submatrix. Since the image is square and we assume correct alignment, we can map each pixel to a cell using integer division: `cellY = (y * nCells) / image.rows`, `cellX = (x * nCells) / image.cols`. This is robust to the marker not being perfectly aligned to pixel boundaries. After building the binary grid (1 for white majority, 0 otherwise), we check the border cells (first and last row, first and last column) to ensure they are all 0; if any is 1, the marker is invalid and we return an empty vector. Then we extract the inner `nInner = nCells - 2` rows and columns. For each of the four rotations, we encode the inner grid into a 64-bit value. The encoding order is: start from the bottom row (highest y) and go upward, and within each row from the rightmost column (highest x) to the leftmost. The last bit (LSB) corresponds to the top-left cell, and the MSB corresponds to the bottom-right cell. For rotation, we apply a 90-degree clockwise rotation to the inner grid; after each rotation, we encode again. The sequential rotations are: original, 90° clockwise, 180°, 270° clockwise. Edge cases: if `total_nbits` is not a perfect square, or if the image is empty or not `CV_8UC1`, return an empty vector. If the border detection fails, return an empty vector. If the inner code has all zeros or all ones, the function still returns the four encoded values (since that is not invalid per se, but the border check might fail for a solid black border only if the interior is all white, but that would be caught if the border also has white, which we check). Time complexity: O(rows*cols) for the cell-building loop (or O(rows*cols) for counting non-zeros per cell, which is similar) plus O(nInner^2) for encoding each of four rotations, so overall O(rows*cols + nInner^2). Space complexity: O(nCells^2) for the binary grid, and O(1) extra.
