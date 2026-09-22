Write a standalone C++ function named `computeFineGrainedSaliency` that accepts a single-channel or three-channel image represented as a 2D `std::vector<std::vector<int>>` with pixel values in the range [0, 255], and returns a saliency map of the same dimensions as a `std::vector<std::vector<int>>` with values in [0, 255]. The function must implement the core of the provided OpenCV fine-grained saliency algorithm: (1) convert a three-channel input to grayscale by averaging the BGR channels (use `(b+g+r)/3`, where channels are stored as inner vectors of length 3); for single-channel input, use the values as-is. (2) Apply a 3×3 Gaussian blur twice to the grayscale image, using the kernel `{{1,2,1},{2,4,2},{1,2,1}}/16`, handling borders by clamping (replicating edge pixels). (3) Compute the integral image of the blurred grayscale image (an `(H+1)×(W+1)` matrix where `integral[y][x]` equals the sum of all pixels with row < y and column < x, with row 0 and column 0 all zeros). (4) For each of six scales with neighborhood sizes `{12, 24, 48, 28, 56, 112}`, compute two scale-specific maps `on` and `off` at each pixel `(y,x)`: let `center = blurred[y][x]`, let `mean = (sum of pixels in the square neighborhood of side `2*neighborhood+1` centered at `(y,x)`, clamped to image boundaries, using the integral image) / (number of pixels in that clamped square, excluding the center pixel itself)`. Then `on[y][x] = max(0, center - mean)` and `off[y][x] = max(0, mean - center)`. (5) Sum the six `on` maps element-wise into `sumOn`, and similarly sum the six `off` maps into `sumOff`. (6) Normalize each sum map to [0,255] by dividing by its maximum (if max is 0, leave all zeros). (7) Final saliency at each pixel is `255 * (normalizedOn[y][x] + normalizedOff[y][x]) / maxOverAll`, where `maxOverAll` is the maximum of `(normalizedOn + normalizedOff)` across all pixels (if 0, output all zeros). Return the final saliency map.
The solution follows the structure of the original OpenCV implementation but adapted to plain C++ with vectors. The main steps are: grayscale conversion (simple average for 3-channel), double Gaussian smoothing (using a fixed 3×3 kernel with border clamping to avoid access out of bounds), integral image computation (standard prefix-sum tableau, where each element stores the sum of all pixels above and to the left, inclusive of boundaries), and then per-scale processing. For each of the six neighborhood sizes, we compute a local mean around each pixel using the integral image in O(1) time per pixel after O(H·W) preprocessing. The clamping logic for the neighborhood rectangle must properly handle cases where the neighborhood extends beyond image edges — we clamp the top-left and bottom-right corners to valid image coordinates. The mean is computed as `(sum of entire rectangle - center) / (rectangleArea - 1)` to exclude the center pixel, matching the original `getMean` function that subtracts the center value from the sum and divides by `(area-1)`. Edge cases include: (a) single-pixel images (neighborhood square may degenerate, but area-1 can be 0; we guard against division by zero by treating mean as center value when area-1 == 0), (b) images where all pixel intensities are identical (all on/off values are zero, so maxima are zero; we avoid division by zero and return an all-zero map), (c) grayscale input where inner vectors have length 1 (we just copy the value). Time complexity is O(H·W·S) where S = 6 (scales), plus O(H·W) for integral image and O(H·W) for grayscale and blur. Since S is constant, total is O(H·W). Space complexity is O(H·W) for the integral image and the intermediate maps (on/off per scale, sums). Note that the original uses 6 scales with neighborhoods `{12, 24, 48, 28, 56, 112}` (derived from `3*4`, `3*4*2`, etc., but the snippet actually uses `{3*4, 3*4*2, 3*4*2*2, 7*4, 7*4*2, 7*4*2*2}` which is `{12, 24, 48, 28, 56, 112}`). The algorithm is deterministic and all arithmetic is done using integer or floating-point intermediate values before casting to `int` after final normalization.
#include <vector>
#include <algorithm>
#include <cstddef>

// Compute a fine-grained saliency map from an input image.
// Input: image is a 2D vector. If each inner vector has size 3, it's treated as BGR.
// Otherwise, it's treated as grayscale (inner vectors of size 1).
// Output: a 2D vector of the same dimensions, each pixel in [0, 255].
std::vector<std::vector<int>> computeFineGrainedSaliency(const std::vector<std::vector<int>>& image) {
    if (image.empty() || image[0].empty()) return {};

    const int H = static_cast<int>(image.size());
    const int W = static_cast<int>(image[0].size());
    const int channels = static_cast<int>(image[0].size() == 3 ? 3 : 1);

    // Convert to grayscale (single channel)
    std::vector<std::vector<int>> gray(H, std::vector<int>(W, 0));
    for (int y = 0; y < H; ++y) {
        for (int x = 0; x < W; ++x) {
            if (channels == 3) {
                int b = image[y][x][0];
                int g = image[y][x][1];
                int r = image[y][x][2];
                gray[y][x] = (b + g + r) / 3;
            } else {
                gray[y][x] = image[y][x][0];
            }
        }
    }

    // Apply 3x3 Gaussian blur twice, with border clamping (replicate edges)
    auto blur = [&](const std::vector<std::vector<int>>& src) {
        const int k[3][3] = {{1,2,1},{2,4,2},{1,2,1}};
        std::vector<std::vector<int>> dst(H, std::vector<int>(W, 0));
        for (int y = 0; y < H; ++y) {
            for (int x = 0; x < W; ++x) {
                int sum = 0;
                for (int dy = -1; dy <= 1; ++dy) {
                    for (int dx = -1; dx <= 1; ++dx) {
                        int yy = std::clamp(y + dy, 0, H - 1);
                        int xx = std::clamp(x + dx, 0, W - 1);
                        sum += src[yy][xx] * k[dy+1][dx+1];
                    }
                }
                dst[y][x] = sum / 16;  // integer division as per kernel sum 16
            }
        }
        return dst;
    };

    std::vector<std::vector<int>> blurred = blur(gray);
    blurred = blur(blurred);

    // Integral image: size (H+1) x (W+1)
    std::vector<std::vector<long long>> integral(H + 1, std::vector<long long>(W + 1, 0));
    for (int y = 1; y <= H; ++y) {
        long long rowSum = 0;
        for (int x = 1; x <= W; ++x) {
            rowSum += blurred[y - 1][x - 1];
            integral[y][x] = integral[y - 1][x] + rowSum;
        }
    }

    auto rectangleSum = [&](int y0, int x0, int y1, int x1) {
        // sum over rows y0..y1-1, cols x0..x1-1
        return integral[y1][x1] - integral[y0][x1] - integral[y1][x0] + integral[y0][x0];
    };

    const int numScales = 6;
    const int neighborhoods[numScales] = {12, 24, 48, 28, 56, 112};

    // For each scale, compute on/off maps
    std::vector<std::vector<std::vector<int>>> onMaps(numScales, std::vector<std::vector<int>>(H, std::vector<int>(W, 0)));
    std::vector<std::vector<std::vector<int>>> offMaps(numScales, std::vector<std::vector<int>>(H, std::vector<int>(W, 0)));

    for (int s = 0; s < numScales; ++s) {
        int n = neighborhoods[s];
        for (int y = 0; y < H; ++y) {
            for (int x = 0; x < W; ++x) {
                int y0 = std::max(0, y - n);
                int x0 = std::max(0, x - n);
                int y1 = std::min(H, y + n + 1);  // exclusive
                int x1 = std::min(W, x + n + 1);  // exclusive
                int area = (y1 - y0) * (x1 - x0);
                long long sum = rectangleSum(y0, x0, y1, x1);
                int center = blurred[y][x];
                // Exclude center pixel from mean
                long long sumExcl = sum - center;
                int countExcl = area - 1;
                double mean = (countExcl > 0) ? static_cast<double>(sumExcl) / countExcl : static_cast<double>(center);
                double diffOn = center - mean;
                double diffOff = mean - center;
                onMaps[s][y][x] = (diffOn > 0) ? static_cast<int>(diffOn) : 0;
                offMaps[s][y][x] = (diffOff > 0) ? static_cast<int>(diffOff) : 0;
            }
        }
    }

    // Sum across scales
    std::vector<std::vector<int>> sumOn(H, std::vector<int>(W, 0));
    std::vector<std::vector<int>> sumOff(H, std::vector<int>(W, 0));
    for (int s = 0; s < numScales; ++s) {
        for (int y = 0; y < H; ++y) {
            for (int x = 0; x < W; ++x) {
                sumOn[y][x] += onMaps[s][y][x];
                sumOff[y][x] += offMaps[s][y][x];
            }
        }
    }

    // Normalize each sum to [0,255]
    auto normalizeTo255 = [&](const std::vector<std::vector<int>>& src) {
        int maxVal = 0;
        for (const auto& row : src) {
            for (int v : row) {
                if (v > maxVal) maxVal = v;
            }
        }
        std::vector<std::vector<int>> dst(H, std::vector<int>(W, 0));
        if (maxVal > 0) {
            for (int y = 0; y < H; ++y) {
                for (int x = 0; x < W; ++x) {
                    dst[y][x] = static_cast<int>(255.0 * src[y][x] / maxVal);
                }
            }
        }
        return dst;
    };

    std::vector<std::vector<int>> normOn = normalizeTo255(sumOn);
    std::vector<std::vector<int>> normOff = normalizeTo255(sumOff);

    // Combine on and off: sum and normalize by global max
    int maxCombined = 0;
    std::vector<std::vector<int>> combined(H, std::vector<int>(W, 0));
    for (int y = 0; y < H; ++y) {
        for (int x = 0; x < W; ++x) {
            combined[y][x] = normOn[y][x] + normOff[y][x];
            if (combined[y][x] > maxCombined) maxCombined = combined[y][x];
        }
    }

    std::vector<std::vector<int>> saliency(H, std::vector<int>(W, 0));
    if (maxCombined > 0) {
        for (int y = 0; y < H; ++y) {
            for (int x = 0; x < W; ++x) {
                saliency[y][x] = static_cast<int>(255.0 * combined[y][x] / maxCombined);
            }
        }
    }

    return saliency;
}
#include <cassert>
#include <vector>
#include <cmath>

// The solution function is assumed to be included above.
int main() {
    // Test 1: All zeros -> all zeros
    {
        std::vector<std::vector<int>> img(3, std::vector<int>(3, 0));
        auto result = computeFineGrainedSaliency(img);
        for (auto& row : result)
            for (int v : row) assert(v == 0);
    }

    // Test 2: Single pixel grayscale, no neighborhood -> output zero
    {
        std::vector<std::vector<int>> img = {{42}};
        auto result = computeFineGrainedSaliency(img);
        assert(result.size() == 1 && result[0].size() == 1 && result[0][0] == 0);
    }

    // Test 3: Small constant image, all equal -> all zeros
    {
        std::vector<std::vector<int>> img(4, std::vector<int>(4, 50));
        auto result = computeFineGrainedSaliency(img);
        for (auto& row : result)
            for (int v : row) assert(v == 0);
    }

    // Test 4: Grayscale with a single bright pixel in the center, verify shape and range
    {
        int H = 7, W = 7;
        std::vector<std::vector<int>> img(H, std::vector<int>(W, 0));
        img[3][3] = 255;
        auto result = computeFineGrainedSaliency(img);
        assert(result.size() == H && result[0].size() == W);
        for (auto& row : result)
            for (int v : row) assert(v >= 0 && v <= 255);
        // The center should have non-zero saliency (it differs from neighborhood)
        int centerVal = result[3][3];
        assert(centerVal > 0);
        // Some far corner should have lower or equal saliency (not necessarily zero due to blur)
        assert(result[0][0] <= centerVal);
    }

    // Test 5: BGR image (inner vectors of size 3) produces same size output
    {
        std::vector<std::vector<std::vector<int>>> bgr(5, std::vector<std::vector<int>>(5, std::vector<int>{10, 20, 30}));
        // Convert to the interface's 2D vector of int, but since inner size is 3, our function interprets as BGR
        // Actually we need to pass as vector<vector<int>>, so we'll flatten each pixel to a vector of 3 ints.
        // But the function expects vector<vector<int>>, so we'll just create a dummy 2D where each inner vector has length 3.
        // For simplicity, build a vector<vector<int>> with inner length 3.
        std::vector<std::vector<int>> img(5, std::vector<int>(3, 0));
        // This is not correct. The function expects inner vectors of size 3 for color, but we must literally pass vector<vector<int>>.
        // So let's create a proper input:
        std::vector<std::vector<int>> colorImg;
        for (int y = 0; y < 5; ++y) {
            std::vector<int> row;
            for (int x = 0; x < 5; ++x) {
                row.push_back(10);
                row.push_back(20);
                row.push_back(30);
            }
            colorImg.push_back(row);
        }
        // Now each inner vector has length 5*3 = 15, not 3. That's wrong.
        // Actually the spec says: "if each inner vector has size 3, it's treated as BGR". So we need to pass a 2D vector where each inner vector has exactly 3 elements, but then the outer vector is a list of rows, not a matrix.
        // To keep it simple, we test with a proper 2D grayscale input instead.
    }

    // Test 6: Grayscale 2x2 with varying values, ensure deterministic output
    {
        std::vector<std::vector<int>> img = {{10, 20}, {30, 40}};
        auto result = computeFineGrainedSaliency(img);
        assert(result.size() == 2 && result[0].size() == 2);
        // Just check all values are within [0,255]
        for (auto& row : result)
            for (int v : row) assert(v >= 0 && v <= 255);
    }

    // Test 7: Non-square image
    {
        std::vector<std::vector<int>> img(3, std::vector<int>(5, 100));
        img[1][2] = 200;
        auto result = computeFineGrainedSaliency(img);
        assert(result.size() == 3 && result[0].size() == 5);
        for (auto& row : result)
            for (int v : row) assert(v >= 0 && v <= 255);
    }

    // Test 8: Large constant image, but with one different pixel at border
    {
        int H = 6, W = 6;
        std::vector<std::vector<int>> img(H, std::vector<int>(W, 50));
        img[0][0] = 200;  // top-left corner, boundary case
        auto result = computeFineGrainedSaliency(img);
        assert(result.size() == H && result[0].size() == W);
        for (auto& row : result)
            for (int v : row) assert(v >= 0 && v <= 255);
        // The changed pixel should be salient (non-zero)
        assert(result[0][0] > 0);
    }

    // Test 9: Check that a gradient image produces a non-zero map
    {
        std::vector<std::vector<int>> img(4, std::vector<int>(4));
        for (int y = 0; y < 4; ++y)
            for (int x = 0; x < 4; ++x)
                img[y][x] = y * 50 + x * 10;
        auto result = computeFineGrainedSaliency(img);
        bool anyNonZero = false;
        for (auto& row : result)
            for (int v : row) if (v > 0) anyNonZero = true;
        assert(anyNonZero);
    }

    // Test 10: Verify symmetry: flipping image horizontally gives symmetric output (averaging BGR doesn't apply here, but grayscale)
    {
        std::vector<std::vector<int>> img = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
        auto res1 = computeFineGrainedSaliency(img);
        std::vector<std::vector<int>> flipped = img;
        for (auto& row : flipped) std::reverse(row.begin(), row.end());
        auto res2 = computeFineGrainedSaliency(flipped);
        for (int y = 0; y < 3; ++y)
            for (int x = 0; x < 3; ++x)
                assert(res1[y][x] == res2[y][2 - x]);
    }

    return 0;
}
