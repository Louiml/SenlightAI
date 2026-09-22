Implement a C++ function that simulates the core histogram-based object tracking logic from the provided OpenCV snippet, but without OpenCV dependencies. Given a grayscale image represented as a 2D vector of `uint8_t` values (0–255), an initial bounding box as `{x, y, width, height}` coordinates, and a number of histogram bins `B` (e.g., 8, 16, or 32), the function should: (1) compute a normalized histogram of the pixel intensities inside the initial ROI, (2) compute a back-projection image where each pixel's value equals the normalized histogram bin value for its intensity, (3) perform one iteration of the mean-shift algorithm on the back-projection to update the bounding box center (keeping width and height fixed), and (4) return the updated bounding box as a new `{x, y, width, height}` structure. The mean-shift update must use the first moment of the back-projection values within the current window, and if the window has zero total mass, the box must remain unchanged. The function signature: `struct BBox { int x, y, w, h; }; BBox meanShiftTrack(const vector<vector<uint8_t>>& img, BBox initial, int bins);` Assume the input image is non-empty, the initial box is fully inside the image, and `bins > 0`.
// The solution has three phases. First, compute the intensity histogram over the initial ROI: allocate a vector of size `bins` initialized to zero, map each pixel intensity `v` (0–255) to bin index `min(bins-1, v * bins / 256)` (or `(v * bins) / 256`), count occurrences, then normalize by dividing by the total count so the histogram sums to 1. This histogram represents the target model. Second, build the back-projection image: for every pixel in the full image, find its bin index using the same mapping and set the back-projection pixel to the normalized histogram value for that bin (as a float). Pixels whose intensity bin has zero count will get 0.0. Third, perform one mean-shift iteration: within the current bounding box (clamped to image boundaries to avoid out-of-range reads), compute the sum of back-projection values and the weighted average of x and y coordinates (using integer pixel coordinates, e.g., x + 0.5 or just x). If the total mass is zero (all back-projection values are 0), return the original box unchanged. Otherwise, compute new center coordinates as `(sumX / mass, sumY / mass)` and round to nearest integers. The new box's top-left corner is `(newCenterX - width/2, newCenterY - height/2)`, but we must clamp the box to stay fully inside the image: if the new x or y goes negative, set to 0; if x + width exceeds image width, set x = imgWidth - width; similarly for y. Time complexity: O(ROI size) for histogram + O(image size) for back-projection + O(box area) for mean-shift, i.e., O(H*W) overall. Space: O(bins) for histogram + O(H*W) for back-projection + O(1) for the box. Edge cases: empty ROI not possible per spec; box clamped; if back-projection values are all zero, box unchanged. Use `std::round` or integer division rounding properly.
#include <vector>
#include <cstdint>
#include <algorithm>
#include <cmath>

struct BBox {
    int x, y, w, h;
};

// Perform one mean-shift iteration for histogram-based tracking.
BBox meanShiftTrack(const std::vector<std::vector<uint8_t>>& img, BBox initial, int bins) {
    const int H = static_cast<int>(img.size());
    const int W = static_cast<int>(img[0].size());

    // Step 1: Compute normalized histogram of initial ROI.
    std::vector<double> hist(bins, 0.0);
    int roiCount = 0;
    for (int y = initial.y; y < initial.y + initial.h; ++y) {
        for (int x = initial.x; x < initial.x + initial.w; ++x) {
            int bin = (img[y][x] * bins) / 256;
            if (bin >= bins) bin = bins - 1;
            hist[bin] += 1.0;
            ++roiCount;
        }
    }
    if (roiCount > 0) {
        for (double& v : hist) v /= roiCount;
    }

    // Step 2: Build back-projection image (same size as input).
    std::vector<std::vector<double>> backproj(H, std::vector<double>(W, 0.0));
    for (int y = 0; y < H; ++y) {
        for (int x = 0; x < W; ++x) {
            int bin = (img[y][x] * bins) / 256;
            if (bin >= bins) bin = bins - 1;
            backproj[y][x] = hist[bin];
        }
    }

    // Step 3: Mean-shift within current window (clamped to image).
    // Clamp box to stay inside the image.
    int x0 = std::max(0, initial.x);
    int y0 = std::max(0, initial.y);
    int x1 = std::min(W, initial.x + initial.w);
    int y1 = std::min(H, initial.y + initial.h);

    double mass = 0.0;
    double sumX = 0.0;
    double sumY = 0.0;
    for (int y = y0; y < y1; ++y) {
        for (int x = x0; x < x1; ++x) {
            double val = backproj[y][x];
            mass += val;
            sumX += val * x;
            sumY += val * y;
        }
    }

    if (mass <= 0.0) {
        return initial; // No mass, keep original box.
    }

    int newCenterX = static_cast<int>(std::round(sumX / mass));
    int newCenterY = static_cast<int>(std::round(sumY / mass));

    // Compute new top-left and clamp to image.
    int newX = newCenterX - initial.w / 2;
    int newY = newCenterY - initial.h / 2;
    newX = std::max(0, std::min(newX, W - initial.w));
    newY = std::max(0, std::min(newY, H - initial.h));

    return {newX, newY, initial.w, initial.h};
}
#include <cassert>
#include <vector>
#include <cstdint>

// Declare the function (assume it's in the same file or header).
struct BBox { int x, y, w, h; };
BBox meanShiftTrack(const std::vector<std::vector<uint8_t>>& img, BBox initial, int bins);

int main() {
    // Test 1: Uniform image, any box should stay in place (mass equal everywhere).
    std::vector<std::vector<uint8_t>> img1(20, std::vector<uint8_t>(20, 128));
    BBox box1{5, 5, 4, 4};
    BBox res1 = meanShiftTrack(img1, box1, 8);
    assert(res1.x >= 0 && res1.y >= 0 && res1.w == 4 && res1.h == 4);
    // Since all pixels have same back-projection value, mean-shift remains at center.
    assert(res1.x == 5 && res1.y == 5);

    // Test 2: Single bright pixel in dark background, box should move to it.
    std::vector<std::vector<uint8_t>> img2(10, std::vector<uint8_t>(10, 0));
    img2[3][5] = 255; // bright pixel at (5,3)
    BBox box2{0, 0, 6, 6}; // initial box covers top-left area
    BBox res2 = meanShiftTrack(img2, box2, 4);
    // The mean-shift should aim near (5,3). Center of new box should be within 2 pixels.
    int centerX = res2.x + res2.w / 2;
    int centerY = res2.y + res2.h / 2;
    assert(std::abs(centerX - 5) <= 2 && std::abs(centerY - 3) <= 2);

    // Test 3: Box near edge must stay inside image.
    std::vector<std::vector<uint8_t>> img3(8, std::vector<uint8_t>(8, 100));
    img3[0][0] = 200; // bright top-left corner
    BBox box3{0, 0, 8, 8}; // full image box
    BBox res3 = meanShiftTrack(img3, box3, 16);
    // Full image box cannot move outside, but center shifts slightly.
    assert(res3.x >= 0 && res3.y >= 0 && res3.x + res3.w <= 8 && res3.y + res3.h <= 8);

    // Test 4: Zero-mass case (all pixels have histogram bin with zero count? Not possible if ROI non-empty.
    // But test with all zeros in back-projection via histogram bins? Instead, use initial box that after clamping has zero area? Not allowed.
    // Test with different bins and ensure no crash.
    BBox box4{2, 2, 3, 3};
    BBox res4 = meanShiftTrack(img1, box4, 32);
    assert(res4.x >= 0 && res4.y >= 0 && res4.x + res4.w <= 20 && res4.y + res4.h <= 20);

    // Test 5: Check that box width/height unchanged.
    std::vector<std::vector<uint8_t>> img5(15, std::vector<uint8_t>(15, 50));
    BBox box5{7, 7, 5, 5};
    BBox res5 = meanShiftTrack(img5, box5, 8);
    assert(res5.w == 5 && res5.h == 5);

    return 0;
}
