/*
Write a C++ function that, given a 2D integer matrix representing a grayscale image (where each element is a pixel intensity) and a list of affine transformation parameters (each row contains 6 floats: a, b, c, d, e, f representing a 2D affine transform), applies each transformation to the entire image and extracts a fixed-size patch around the transformed center of the image. The function should return a vector of square matrices, each of size 48×48, representing the warped patches for each set of parameters. The affine transform maps source coordinates (x, y) to destination coordinates (x', y') using: x' = a*x + c*y + e, y' = b*x + d*y + f. For each sample, the patch is computed by iterating over destination patch coordinates (px, py) from 0 to 47 (inclusive), computing the corresponding source coordinate via the inverse affine transform, and sampling the nearest valid pixel from the input image; if the source coordinate is outside the image bounds, use 0. The function signature should be: `std::vector<std::vector<std::vector<int>>> warpPatches(const std::vector<std::vector<int>>& image, const std::vector<std::array<float,6>>& affineParams)`. Assume the image is at least 1×1 and affineParams is non-empty.
*/

#include <vector>
#include <array>
#include <cmath>

// Apply a set of affine transforms to an image and extract 48x48 warped patches.
// image: 2D vector where image[row][col] is pixel intensity.
// affineParams: each row contains {a, b, c, d, e, f} for transform:
//   x' = a*x + c*y + e, y' = b*x + d*y + f.
// Returns a vector of patches; each patch is a 48x48 matrix (row-major).
std::vector<std::vector<std::vector<int>>> warpPatches(
    const std::vector<std::vector<int>>& image,
    const std::vector<std::array<float,6>>& affineParams) {
    
    const int patchSize = 48;
    const int rows = static_cast<int>(image.size());
    const int cols = rows > 0 ? static_cast<int>(image[0].size()) : 0;
    
    std::vector<std::vector<std::vector<int>>> result;
    result.reserve(affineParams.size());
    
    for (const auto& params : affineParams) {
        float a = params[0], b = params[1], c = params[2];
        float d = params[3], e = params[4], f = params[5];
        
        // Compute the determinant of the linear part.
        float det = a * d - b * c;
        
        std::vector<std::vector<int>> patch(patchSize, 
            std::vector<int>(patchSize, 0));
        
        // If degenerate transform, return a zero patch.
        if (std::fabs(det) < 1e-6f) {
            result.push_back(patch);
            continue;
        }
        
        // Inverse matrix: [[invA, invC], [invB, invD]].
        float invA = d / det;
        float invB = -b / det;
        float invC = -c / det;
        float invD = a / det;
        
        // For each destination patch pixel, compute source and sample.
        for (int py = 0; py < patchSize; ++py) {
            for (int px = 0; px < patchSize; ++px) {
                // Map from patch coordinates to image coordinates.
                float dx = static_cast<float>(px) - e;
                float dy = static_cast<float>(py) - f;
                float srcX = invA * dx + invC * dy;
                float srcY = invB * dx + invD * dy;
                
                // Round to nearest integer pixel.
                int sx = static_cast<int>(std::lround(srcX));
                int sy = static_cast<int>(std::lround(srcY));
                
                // Check bounds; if inside, copy the pixel, else 0.
                if (sx >= 0 && sx < cols && sy >= 0 && sy < rows) {
                    patch[py][px] = image[sy][sx];
                }
            }
        }
        
        result.push_back(patch);
    }
    
    return result;
}

#include <cassert>
#include <vector>
#include <array>

// The solution function is declared above; here is the test harness.
int main() {
    // Test 1: Identity transform on a 5x5 image, patch size 48.
    // The patch should mostly be zeros except the top-left 5x5 area, but
    // the center of the patch corresponds to source (0,0) via translation.
    // For simplicity, test with a tiny image and check a few known values.
    std::vector<std::vector<int>> img1 = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };
    std::vector<std::array<float,6>> params1 = {{1,0,0,1,0,0}};
    auto result1 = warpPatches(img1, params1);
    assert(result1.size() == 1);
    assert(result1[0].size() == 48 && result1[0][0].size() == 48);
    // For identity and translation (0,0), patch pixel (0,0) maps to source (0,0) -> pixel 1.
    // But our mapping uses inverse: source = inv * (px - e). With identity and e=0,
    // source = px, so (0,0) gets image[0][0] = 1.
    assert(result1[0][0][0] == 1);
    // Pixel (1,0) maps to source (1,0) -> image[0][1] = 2.
    assert(result1[0][0][1] == 2);
    // Pixel (0,1) maps to source (0,1) -> image[1][0] = 4.
    assert(result1[0][1][0] == 4);
    // Pixel (2,2) maps to source (2,2) -> image[2][2] = 9.
    assert(result1[0][2][2] == 9);
    // Pixel (3,3) maps to source (3,3) which is out of bounds -> 0.
    assert(result1[0][3][3] == 0);

    // Test 2: Translation e=10, f=10. Then patch (0,0) maps to source (-10,-10) -> 0.
    std::vector<std::array<float,6>> params2 = {{1,0,0,1,10,10}};
    auto result2 = warpPatches(img1, params2);
    assert(result2[0][0][0] == 0);
    // Patch (10,10) maps to source (0,0) -> image[0][0]=1.
    assert(result2[0][10][10] == 1);

    // Test 3: Scaling by 2: x' = 2*x, y' = 2*y. Then patch (0,0) maps to source (0,0) ->1.
    std::vector<std::array<float,6>> params3 = {{2,0,0,2,0,0}};
    auto result3 = warpPatches(img1, params3);
    assert(result3[0][0][0] == 1);
    // Patch (2,0) maps to source (1,0) -> image[0][1] = 2.
    assert(result3[0][0][2] == 2);
    // Patch (4,0) maps to source (2,0) -> image[0][2] = 3.
    assert(result3[0][0][4] == 3);

    // Test 4: Multiple samples.
    std::vector<std::array<float,6>> params4 = {{1,0,0,1,0,0}, {1,0,0,1,5,5}};
    auto result4 = warpPatches(img1, params4);
    assert(result4.size() == 2);
    assert(result4[0][0][0] == 1);
    assert(result4[1][0][0] == 0); // Translation moves out of bounds, but source (0,0) maps to patch (5,5), not (0,0).
    assert(result4[1][5][5] == 1);

    // Test 5: Degenerate transform (zero determinant) returns all zeros.
    std::vector<std::array<float,6>> params5 = {{1,1,1,1,0,0}}; // det = 1*1 - 1*1 = 0
    auto result5 = warpPatches(img1, params5);
    assert(result5.size() == 1);
    bool allZero = true;
    for (int r = 0; r < 48; ++r)
        for (int c = 0; c < 48; ++c)
            if (result5[0][r][c] != 0) allZero = false;
    assert(allZero);

    // Test 6: Empty image edge case? Not required by spec, but ensure no crash.
    std::vector<std::vector<int>> imgEmpty = {};
    std::vector<std::array<float,6>> params6 = {{1,0,0,1,0,0}};
    auto result6 = warpPatches(imgEmpty, params6);
    // Since rows=0, cols=0, all samples are out of bounds, so all zeros.
    assert(result6.size() == 1);
    assert(result6[0][0][0] == 0);

    return 0;
}

// The core algorithm involves, for each affine parameter set, computing the inverse 2×2 matrix of the linear part (a, b, c, d). The inverse exists unless the determinant is zero; for the task, assume the determinant is non-zero, but handle degenerate cases by skipping that sample and returning an empty patch or zero matrix. For each of the 48×48 patch pixels (px, py), map to source coordinates using the inverse transform: source_x = (invA * (px - e) + invC * (py - f)) and source_y = (invB * (px - e) + invD * (py - f)), where invA, invB, invC, invD are the inverse matrix entries. Then, round the source coordinates to nearest integers using `std::lround`, and check if they fall within the image dimensions [0, cols-1] and [0, rows-1]. If yes, copy the pixel value; otherwise use 0. The patch is filled row-major (46 iterations for rows, 46 for columns). Complexity: For m affine samples and image size H×W, the time complexity is O(m * 48 * 48) = O(m * 2304), independent of H and W because we only sample fixed coordinates; the space complexity is O(m * 48 * 48) for the output. Edge cases include images smaller than 48×48 (sampling outside bounds yields 0), affine parameters with zero determinant (skip and output a zero 48×48 patch), and non-integer source coordinates (use rounding). The inverse matrix is computed as det = a*d - b*c, then invA = d/det, invB = -b/det, invC = -c/det, invD = a/det. The translation is applied after the linear mapping: (source_x, source_y) = M_inv * (px - e, py - f).
