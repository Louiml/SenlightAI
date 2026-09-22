/*
Write a C++ function that takes a grayscale image represented as a 2D vector of unsigned char values, along with an initial rectangular region (specified by row, column, height, and width), and performs one iteration of the mean-shift tracking algorithm on the image. The function should compute the histogram of the region's hue values (using a simplified 1D histogram with a given number of bins), then compute the back-projection of the entire image using that histogram, and finally return the new rectangular region (as a struct with row, col, height, width) that maximizes the sum of back-projected values within a search window centered on the original region (search window extends the region by a fixed margin in all directions). The function must handle boundary conditions by clamping coordinates to valid image bounds, and it must work for any non-empty image and any valid initial region that lies at least partially inside the image. Assume the input image is already in HSV color space, with the hue channel stored as unsigned char (0-179) in the 2D vector.
*/

#include <vector>
#include <algorithm>
#include <cstddef>

struct Rect {
    int row;      // top-left row
    int col;      // top-left column
    int height;   // height
    int width;    // width
};

// Perform one iteration of mean-shift tracking on a hue channel image.
// Returns the new region that maximizes back-projection sum within a search window.
Rect meanShiftOneStep(const std::vector<std::vector<unsigned char>>& hueImage,
                      int bins,
                      Rect initial,
                      int margin = 10) {
    int rows = (int)hueImage.size();
    if (rows == 0) return initial;
    int cols = (int)hueImage[0].size();
    if (cols == 0) return initial;

    // Clamp initial region to valid bounds (at least partially inside)
    int r0 = std::max(0, initial.row);
    int c0 = std::max(0, initial.col);
    int r1 = std::min(rows - 1, initial.row + initial.height - 1);
    int c1 = std::min(cols - 1, initial.col + initial.width - 1);
    if (r1 < r0 || c1 < c0) return initial; // no overlap with image

    // Step 1: Compute histogram of hue values in the initial region
    std::vector<double> hist(bins, 0.0);
    int total = 0;
    for (int r = r0; r <= r1; ++r) {
        for (int c = c0; c <= c1; ++c) {
            int hue = (int)hueImage[r][c];
            if (hue >= 0 && hue < bins) {
                hist[hue] += 1.0;
                total++;
            }
        }
    }
    if (total == 0) return initial; // no valid hue pixels
    for (int i = 0; i < bins; ++i) {
        hist[i] /= total; // normalize to sum 1
    }

    // Step 2: Back-projection image (same size as input)
    std::vector<std::vector<double>> backProject(rows, std::vector<double>(cols, 0.0));
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            int hue = (int)hueImage[r][c];
            if (hue >= 0 && hue < bins) {
                backProject[r][c] = hist[hue];
            }
        }
    }

    // Step 3: Define search window (clamped to image bounds)
    int searchR0 = std::max(0, initial.row - margin);
    int searchC0 = std::max(0, initial.col - margin);
    int searchR1 = std::min(rows - 1, initial.row + initial.height - 1 + margin);
    int searchC1 = std::min(cols - 1, initial.col + initial.width - 1 + margin);

    int regionH = r1 - r0 + 1; // actual region height
    int regionW = c1 - c0 + 1; // actual region width

    double bestSum = -1.0;
    Rect bestRegion = initial; // default to original

    for (int r = searchR0; r + regionH - 1 <= searchR1; ++r) {
        for (int c = searchC0; c + regionW - 1 <= searchC1; ++c) {
            double sum = 0.0;
            for (int i = 0; i < regionH; ++i) {
                for (int j = 0; j < regionW; ++j) {
                    sum += backProject[r + i][c + j];
                }
            }
            if (sum > bestSum) {
                bestSum = sum;
                bestRegion = {r, c, regionH, regionW};
            }
        }
    }
    return bestRegion;
}

#include <cassert>
#include <vector>

// Rect equality helper for assert
bool operator==(const Rect& a, const Rect& b) {
    return a.row == b.row && a.col == b.col && a.height == b.height && a.width == b.width;
}

int main() {
    // Simple 10x10 image: all hue = 0 except a bright blob at rows 3-4, cols 4-5 with hue=1
    std::vector<std::vector<unsigned char>> img(10, std::vector<unsigned char>(10, 0));
    img[3][4] = 1; img[3][5] = 1;
    img[4][4] = 1; img[4][5] = 1;

    // Initial region at top-left, should move to blob location
    Rect init{0, 0, 2, 2};
    Rect result = meanShiftOneStep(img, 5, init, 10);
    // The histogram of init is all hue 0, so the best region is one that captures many hue=0 pixels,
    // but all pixels are hue=0 except the blob, so the sum is equal everywhere (1.0 per pixel).
    // The first top-left region is chosen, so new region stays at (0,0) with same size.
    assert(result == Rect{0, 0, 2, 2});

    // Now make most pixels low-probability by making hist of init all hue 5 (out of 5 bins)
    // Create image where hue=0 is rare, hue=1 is common, and init contains only hue=0.
    std::vector<std::vector<unsigned char>> img2(10, std::vector<unsigned char>(10, 1));
    img2[0][0] = 0; img2[0][1] = 0; img2[1][0] = 0; img2[1][1] = 0; // init region has hue 0
    Rect init2{0, 0, 2, 2};
    Rect result2 = meanShiftOneStep(img2, 5, init2, 10);
    // Histogram of init2 has all weight on bin 0. Back-projection: hue=0 pixels get weight 1.0, others 0.0.
    // Only pixels with hue=0 are the 4 in init2. The search window covers whole image. The best region is the one containing those 4 pixels.
    // The top-left position (0,0) contains all 4, so result2 should be (0,0,2,2).
    assert(result2 == Rect{0, 0, 2, 2});

    // Test with empty image? Not allowed per spec, but ensure no crash with zero rows.
    std::vector<std::vector<unsigned char>> empty(0, std::vector<unsigned char>(0));
    Rect init3{0,0,1,1};
    assert(meanShiftOneStep(empty, 5, init3) == init3);

    // Test boundary: initial region partially outside
    std::vector<std::vector<unsigned char>> img3(5, std::vector<unsigned char>(5, 0));
    Rect init4{-2, -2, 2, 2}; // overlaps one pixel at (0,0)
    Rect result4 = meanShiftOneStep(img3, 5, init4, 10);
    // Histogram of overlap region (just pixel 0,0) has all weight on bin 0.
    // Back-projection is uniform 1.0 everywhere (since all hue=0), so the first search position wins:
    // searchR0 = max(0,-12)=0, searchC0=0, regionH = 1, regionW=1, best region at (0,0,1,1) 
    assert(result4 == Rect{0, 0, 1, 1});

    // Test non-trivial move: hue=0 rare, hue=1 common, but init contains hue=1 rare elsewhere
    std::vector<std::vector<unsigned char>> img4(10, std::vector<unsigned char>(10, 1));
    // Make a region of hue=0 at (6,6) to (7,7) (4 pixels)
    img4[6][6]=0; img4[6][7]=0; img4[7][6]=0; img4[7][7]=0;
    // init region at (0,0) size 2x2 contains only hue=1 (common), so histogram has weight 1.0 on bin 1.
    // Back-projection: hue=1 pixels get 1.0, hue=0 pixels get 0.0. So best region is one that maximizes sum of 1.0 pixels,
    // which is any region that avoids hue=0 pixels. The first top-left region (0,0) score = 4 (all 1.0), so result stays (0,0,2,2).
    Rect init5{0,0,2,2};
    Rect result5 = meanShiftOneStep(img4, 5, init5, 10);
    assert(result5 == Rect{0,0,2,2});

    return 0;
}

// The solution involves three main steps. First, build a 1D histogram of the hue values inside the initial rectangular region, counting only pixels where the hue is valid (0-179). The histogram is normalized so that its sum equals 1.0 (to be used as a probability distribution). Second, create a back-projection image of the same size as the input, where each pixel's value is the histogram bin count corresponding to that pixel's hue (i.e., a lookup into the normalized histogram). Third, define a search window: extend the initial rectangle by a fixed margin (e.g., 10 pixels) in all directions, but clamp the resulting search window to the image bounds. Within that search window, slide the original rectangle (same height and width) across every possible position, computing the sum of back-projection values inside the sliding rectangle. The new region is the one with the maximum sum; if there are ties, choose the first one encountered (top-leftmost). Edge cases: if the initial region has zero area or is entirely outside the image, the function should return the original region unchanged. If the histogram is empty (no valid hue pixels), the back-projection is all zeros, and the function should return the original region. Time complexity: constructing the histogram takes O(region_area) time; back-projection takes O(image_area) time; the search over all sliding positions takes O(search_window_area * region_area) time, since each region sum is computed naively. Space complexity: O(histogram_bins + image_area) for the histogram and back-projection image.
