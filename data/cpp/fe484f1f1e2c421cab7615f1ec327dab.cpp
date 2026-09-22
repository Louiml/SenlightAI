Given a grayscale image represented as a flat `std::vector<uint8_t>` with explicit `width` and `height`, along with a vector of four 2D corner points (obtained from a detection step) expressed in image coordinates, write a C++ function that computes the perspective-corrected QR code binary image of a fixed size (e.g., 250×250 pixels) using the four-point perspective transform. The four corners are assumed to be ordered (e.g., top-left, top-right, bottom-right, bottom-left), but the function should handle arbitrary ordering by automatically identifying the correct correspondence to the output rectangle corners. The input image is already binarized (0=black, 255=white). The function must return the straightened binary image as a `std::vector<uint8_t>` of size `250*250`, where each pixel is either 0 or 255. The transformation must be based on solving for a perspective transform using the four point correspondences and applying a perspective warp with nearest-neighbor interpolation. Handle degenerate cases (e.g., collinear points or invalid point coordinates) by returning an empty vector.
The core task is to implement a 2D perspective transform from an arbitrary quadrilateral in the source image to a fixed-size square in the output. The standard approach:
1. **Order the corners**: Determine the correspondence between the four input points and the four output corners (top-left, top-right, bottom-right, bottom-left). This is done by computing the centroid and then sorting points by their angle around the centroid; this yields a consistent clockwise (or counterclockwise) ordering. Then identify the top-left point as the one with the smallest combined x+y (if axes are x-right, y-down) or use more robust geometric checks.
2. **Compute the homography matrix**: Given four point correspondences `(src_0->(0,0), src_1->(W,0), src_2->(W,H), src_3->(0,H))`, we solve the linear system for the 3x3 homography matrix `H` (with `h33=1`). For each point correspondence `(x,y) -> (u,v)`, we get two linear equations: `x*(h31*u + h32*v + h33) = h11*u + h12*v + h13` and `y*(h31*u + h32*v + h33) = h21*u + h22*v + h23`. Collect these into an 8x8 or 8x9 matrix and solve using Gaussian elimination (since we have exactly 4 points, the system is determined).
3. **Apply the warp**: For each output pixel `(u,v)`, compute the corresponding source coordinate using the inverse homography: `(x,y) = H^{-1} * (u,v,1)^T`, then use nearest-neighbor sampling (rounding to the nearest integer). Ensure bounds checking: if the source coordinate is outside the image, put a white pixel (255) as a placeholder.
4. **Edge cases**: Check that the four input points are not collinear (area > threshold). Also, if any coordinate is outside the image, clamp or return empty. The output is fixed at 250×250 as per the task.

Time complexity: `O(250*250)` for pixel sampling plus constant time for homography computation. Space complexity: output buffer of `250*250` bytes plus constant.
#include <vector>
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <numeric>

// Helper: gaussian elimination for solving an 8x9 augmented matrix
static bool solveLinearSystem(double a[8][9], double out[8]) {
    for (int col = 0; col < 8; ++col) {
        // Find pivot row
        int pivot = col;
        for (int row = col + 1; row < 8; ++row) {
            if (std::fabs(a[row][col]) > std::fabs(a[pivot][col])) {
                pivot = row;
            }
        }
        if (std::fabs(a[pivot][col]) < 1e-12) {
            return false; // Singular matrix
        }
        if (pivot != col) {
            for (int c = 0; c < 9; ++c) {
                std::swap(a[pivot][c], a[col][c]);
            }
        }
        // Normalize pivot row
        double div = a[col][col];
        for (int c = 0; c < 9; ++c) {
            a[col][c] /= div;
        }
        // Eliminate other rows
        for (int row = 0; row < 8; ++row) {
            if (row == col) continue;
            double factor = a[row][col];
            if (std::fabs(factor) < 1e-15) continue;
            for (int c = 0; c < 9; ++c) {
                a[row][c] -= factor * a[col][c];
            }
        }
    }
    for (int i = 0; i < 8; ++i) {
        out[i] = a[i][8];
    }
    return true;
}

// Compute homography matrix (3x3) mapping src[i] -> dst[i] for i=0..3
// Returns true on success, stores 9 coefficients in H (row-major, H[8]=1)
static bool computeHomography(const std::vector<std::pair<double,double>>& src,
                              const std::vector<std::pair<double,double>>& dst,
                              double H[9]) {
    if (src.size() != 4 || dst.size() != 4) return false;
    double a[8][9] = {0};
    for (int i = 0; i < 4; ++i) {
        double x = src[i].first;
        double y = src[i].second;
        double u = dst[i].first;
        double v = dst[i].second;
        // Equation: x*(h31*u + h32*v + h33) = h11*u + h12*v + h13
        // => u*h11 + v*h12 + h13 - x*u*h31 - x*v*h32 = x
        int row = 2 * i;
        a[row][0] = u; a[row][1] = v; a[row][2] = 1;
        a[row][3] = 0; a[row][4] = 0; a[row][5] = 0;
        a[row][6] = -x * u; a[row][7] = -x * v;
        a[row][8] = x;
        // Equation: y*(h31*u + h32*v + h33) = h21*u + h22*v + h23
        // => u*h21 + v*h22 + h23 - y*u*h31 - y*v*h32 = y
        int row2 = 2 * i + 1;
        a[row2][0] = 0; a[row2][1] = 0; a[row2][2] = 0;
        a[row2][3] = u; a[row2][4] = v; a[row2][5] = 1;
        a[row2][6] = -y * u; a[row2][7] = -y * v;
        a[row2][8] = y;
    }
    double h[8];
    if (!solveLinearSystem(a, h)) return false;
    // Fill H in row-major order: h11,h12,h13,h21,h22,h23,h31,h32,h33=1
    H[0]=h[0]; H[1]=h[1]; H[2]=h[2];
    H[3]=h[3]; H[4]=h[4]; H[5]=h[5];
    H[6]=h[6]; H[7]=h[7]; H[8]=1.0;
    return true;
}

// Invert a 3x3 matrix (row-major). Returns false if singular.
static bool invertMatrix(const double M[9], double inv[9]) {
    double det = M[0]*(M[4]*M[8]-M[5]*M[7])
               - M[1]*(M[3]*M[8]-M[5]*M[6])
               + M[2]*(M[3]*M[7]-M[4]*M[6]);
    if (std::fabs(det) < 1e-12) return false;
    inv[0] = (M[4]*M[8]-M[5]*M[7]) / det;
    inv[1] = (M[2]*M[7]-M[1]*M[8]) / det;
    inv[2] = (M[1]*M[5]-M[2]*M[4]) / det;
    inv[3] = (M[5]*M[6]-M[3]*M[8]) / det;
    inv[4] = (M[0]*M[8]-M[2]*M[6]) / det;
    inv[5] = (M[2]*M[3]-M[0]*M[5]) / det;
    inv[6] = (M[3]*M[7]-M[4]*M[6]) / det;
    inv[7] = (M[1]*M[6]-M[0]*M[7]) / det;
    inv[8] = (M[0]*M[4]-M[1]*M[3]) / det;
    return true;
}

// Main function: perspective correct a QR code region to 250x250 binary image
std::vector<uint8_t> perspectiveCorrectQR(const std::vector<uint8_t>& image,
                                          int width, int height,
                                          const std::vector<std::pair<float,float>>& corners) {
    const int OUT_SIZE = 250;
    std::vector<uint8_t> result;

    // Validate inputs
    if (image.empty() || width <= 0 || height <= 0 || corners.size() != 4) {
        return result;
    }
    // Check that image size matches
    if (image.size() != static_cast<size_t>(width) * height) {
        return result;
    }

    // Compute centroid
    double cx = 0, cy = 0;
    for (const auto& p : corners) {
        cx += p.first;
        cy += p.second;
    }
    cx /= 4.0; cy /= 4.0;

    // Sort points by angle around centroid (clockwise or counterclockwise)
    std::vector<size_t> idx(4);
    std::iota(idx.begin(), idx.end(), 0);
    std::sort(idx.begin(), idx.end(), [&](size_t a, size_t b) {
        double ang_a = std::atan2(corners[a].second - cy, corners[a].first - cx);
        double ang_b = std::atan2(corners[b].second - cy, corners[b].first - cx);
        return ang_a < ang_b;
    });

    // Now idx[0..3] are in circular order. Determine top-left:
    // Among the four, top-left has smallest x+y (assuming y down) and bottom-right largest
    // Find index with min x+y, then set as idx[0]
    size_t min_idx = 0;
    double min_sum = corners[idx[0]].first + corners[idx[0]].second;
    for (size_t i = 1; i < 4; ++i) {
        double s = corners[idx[i]].first + corners[idx[i]].second;
        if (s < min_sum) {
            min_sum = s;
            min_idx = i;
        }
    }
    // Rotate so that min_idx is first
    std::vector<size_t> ordered(4);
    for (size_t i = 0; i < 4; ++i) {
        ordered[i] = idx[(min_idx + i) % 4];
    }
    // Ensure orientation: top-left, top-right, bottom-right, bottom-left in clockwise order
    // The angle sort gives CCW if angles increase (since y down, CCW visually is counterclockwise? careful)
    // We'll normalize by checking cross product of vectors (TL->TR) and (TL->BL)
    // For y-down, we want TL->TR to be to the right, and TL->BL to be down, so cross (TR-TL, BL-TL) > 0
    std::pair<float,float> tl = corners[ordered[0]];
    std::pair<float,float> tr = corners[ordered[1]];
    std::pair<float,float> br = corners[ordered[2]];
    std::pair<float,float> bl = corners[ordered[3]];
    double cross = (tr.first - tl.first)*(bl.second - tl.second) - (tr.second - tl.second)*(bl.first - tl.first);
    if (cross > 0) {
        // Current order is TL,TR,BR,BL but we might need BL,TL,TR,BR? Actually if cross > 0, order is TL->TR->BR->BL (clockwise in y-down)
        // It's already correct; if cross < 0, swap tr and bl effectively
    } else {
        // Swap BR and BL to fix orientation: now order becomes TL,TR,BL,BR? We'll reorder
        std::swap(ordered[2], ordered[3]);
    }

    // Build source points in order: TL, TR, BR, BL
    std::vector<std::pair<double,double>> src(4);
    for (size_t i = 0; i < 4; ++i) {
        src[i] = {corners[ordered[i]].first, corners[ordered[i]].second};
    }
    // Destination points: (0,0), (OUT_SIZE,0), (OUT_SIZE,OUT_SIZE), (0,OUT_SIZE)
    std::vector<std::pair<double,double>> dst = {
        {0.0, 0.0}, {double(OUT_SIZE), 0.0}, {double(OUT_SIZE), double(OUT_SIZE)}, {0.0, double(OUT_SIZE)}
    };

    // Compute homography from src to dst
    double H[9];
    if (!computeHomography(src, dst, H)) {
        return result;
    }

    // Invert H to map output->input
    double Hinv[9];
    if (!invertMatrix(H, Hinv)) {
        return result;
    }

    // Perform perspective warp with nearest neighbor
    result.resize(OUT_SIZE * OUT_SIZE, 255); // default white
    for (int v = 0; v < OUT_SIZE; ++v) {
        for (int u = 0; u < OUT_SIZE; ++u) {
            // Compute source coordinate: (x,y) = Hinv * (u,v,1)
            double denom = Hinv[6]*u + Hinv[7]*v + Hinv[8];
            if (std::fabs(denom) < 1e-12) continue;
            double x = (Hinv[0]*u + Hinv[1]*v + Hinv[2]) / denom;
            double y = (Hinv[3]*u + Hinv[4]*v + Hinv[5]) / denom;
            int xi = static_cast<int>(std::round(x));
            int yi = static_cast<int>(std::round(y));
            if (xi >= 0 && xi < width && yi >= 0 && yi < height) {
                uint8_t val = image[yi * width + xi];
                result[v * OUT_SIZE + u] = (val == 0) ? 0 : 255;
            } else {
                // Out of bounds, leave white
                result[v * OUT_SIZE + u] = 255;
            }
        }
    }
    return result;
}
#include <cassert>
#include <vector>
#include <cstdint>

int main() {
    // Create a synthetic 100x100 binary image with a black square in the middle
    const int W = 100, H = 100;
    std::vector<uint8_t> img(W * H, 255);
    // Fill a 20x20 black square at center (40,40) to (60,60)
    for (int y = 40; y < 60; ++y) {
        for (int x = 40; x < 60; ++x) {
            img[y * W + x] = 0;
        }
    }

    // Define corners in arbitrary order (shuffled)
    std::vector<std::pair<float,float>> corners = {
        {60.f, 60.f},  // bottom-right
        {40.f, 40.f},  // top-left
        {60.f, 40.f},  // top-right
        {40.f, 60.f}   // bottom-left
    };

    auto result = perspectiveCorrectQR(img, W, H, corners);
    assert(result.size() == 250 * 250);
    // The center of the output should be black (since the source black square maps to a central area)
    // Check a few points near center
    bool has_black = false;
    for (int y = 120; y < 130; ++y) {
        for (int x = 120; x < 130; ++x) {
            if (result[y * 250 + x] == 0) has_black = true;
        }
    }
    assert(has_black);
    // Corners of output should be white
    assert(result[0] == 255);
    assert(result[249] == 255);
    assert(result[249 * 250] == 255);
    assert(result[249 * 250 + 249] == 255);

    // Test with all-white image: result should be all white
    std::vector<uint8_t> img_white(W * H, 255);
    auto res_white = perspectiveCorrectQR(img_white, W, H, corners);
    assert(res_white.size() == 250 * 250);
    bool all_white = true;
    for (uint8_t v : res_white) if (v != 255) { all_white = false; break; }
    assert(all_white);

    // Test with invalid corner count
    std::vector<std::pair<float,float>> bad_corners(3);
    auto res_bad = perspectiveCorrectQR(img, W, H, bad_corners);
    assert(res_bad.empty());

    // Test with degenerate (collinear) corners
    std::vector<std::pair<float,float>> collinear = {{0,0},{1,1},{2,2},{3,3}};
    auto res_collinear = perspectiveCorrectQR(img, W, H, collinear);
    assert(res_collinear.empty());

    // Test with a simple identity mapping (corners = full image corners)
    std::vector<std::pair<float,float>> full = {{0,0},{W-1,0},{W-1,H-1},{0,H-1}};
    auto res_full = perspectiveCorrectQR(img, W, H, full);
    // The full image is mostly white, but the black square should appear somewhere
    // Just check size and that it's not empty
    assert(res_full.size() == 250 * 250);

    return 0;
}
