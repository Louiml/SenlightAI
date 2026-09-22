// Write a C++ function that validates and processes two grayscale images (depth and intensity) along with a binary noise mask and two S-element parameter vectors (sigmaS, sigmaR). The function must first validate that all three images have the same dimensions and that the parameter vectors have equal length S, returning a boolean indicating whether validation passed. If validation succeeds, it should simulate a simplified joint bilateral filter by computing, for each pixel where the noise mask is false, the weighted average of neighboring depth values where the weight depends on both spatial distance (via sigmaS[0]) and intensity difference (via sigmaR[0]) using a fixed 3x3 neighborhood. Pixels where the noise mask is true should be set to 0 in the output depth image. The function should be self-contained, use `const` correctly, and handle edge pixels by only considering neighbors that exist within the image bounds.

The solution requires a two-phase approach: (1) validation and (2) filtering. In the validation phase, we check that all three images are 2D (H×W), have the same height and width, and that both sigma vectors have the same length S (and are at least size 1). If any check fails, return `false`. In the filtering phase, we iterate over every pixel. For pixels where the noise mask is `true`, set output depth to 0. For valid pixels, we initialize a weighted sum and total weight to zero, then loop over a 3×3 neighborhood (dx,dy from -1 to 1). For each neighbor that lies within bounds [0,H-1]×[0,W-1], compute spatial weight `exp(-(dx²+dy²)/(2*sigmaS[0]²))` and range weight `exp(-((intensity[pixel]-intensity[neighbor])²)/(2*sigmaR[0]²))`. The combined weight is the product, and we accumulate `weight * depth[neighbor]` into the sum, and `weight` into total. Finally, if total weight > 0, output = sum/total; otherwise output = original depth. Edge cases include sigma values being zero (we should guard against division by zero by using a small epsilon like 1e-6) and empty images (H or W zero → validation fails). Time complexity is O(H*W*9) = O(H*W) since 9 is constant, and space complexity is O(H*W) for the output image plus O(1) auxiliary.

#include <vector>
#include <cmath>
#include <cstdint>

// Validate and apply a simplified joint bilateral filter to depth images.
// Returns true on success, false if validation fails.
// depth, intensity: H×W grayscale images (0-255)
// noise_mask: H×W boolean mask (true = noisy)
// sigmaS, sigmaR: S-element vectors of spatial and range sigmas
// output_depth: preallocated H×W vector to receive result
bool jointBilateralFilter(
    const std::vector<uint8_t>& depth,
    const std::vector<uint8_t>& intensity,
    const std::vector<bool>& noise_mask,
    const std::vector<double>& sigmaS,
    const std::vector<double>& sigmaR,
    std::vector<uint8_t>& output_depth,
    size_t height,
    size_t width)
{
    // Validate dimensions and vector lengths
    if (height == 0 || width == 0) return false;
    size_t img_size = height * width;
    if (depth.size() != img_size || intensity.size() != img_size || noise_mask.size() != img_size) return false;
    if (sigmaS.empty() || sigmaR.empty() || sigmaS.size() != sigmaR.size()) return false;

    // Resize output if needed
    if (output_depth.size() != img_size) output_depth.resize(img_size);

    // Use the first sigma values for the filter
    double sigma_s = sigmaS[0];
    double sigma_r = sigmaR[0];
    // Guard against division by zero
    sigma_s = std::max(sigma_s, 1e-6);
    sigma_r = std::max(sigma_r, 1e-6);

    // Precompute the 3x3 offsets (row,col)
    const int offsets[9][2] = {
        {-1,-1}, {-1,0}, {-1,1},
        {0,-1},  {0,0},  {0,1},
        {1,-1},  {1,0},  {1,1}
    };

    for (size_t y = 0; y < height; ++y) {
        for (size_t x = 0; x < width; ++x) {
            size_t idx = y * width + x;
            if (noise_mask[idx]) {
                output_depth[idx] = 0;
                continue;
            }

            double sum = 0.0;
            double total_weight = 0.0;
            double center_intensity = static_cast<double>(intensity[idx]);

            for (int d = 0; d < 9; ++d) {
                int ny = static_cast<int>(y) + offsets[d][0];
                int nx = static_cast<int>(x) + offsets[d][1];
                if (ny < 0 || ny >= static_cast<int>(height) || nx < 0 || nx >= static_cast<int>(width)) continue;

                size_t nidx = static_cast<size_t>(ny) * width + static_cast<size_t>(nx);
                double spatial_dist_sq = offsets[d][0]*offsets[d][0] + offsets[d][1]*offsets[d][1];
                double spatial_weight = std::exp(-spatial_dist_sq / (2.0 * sigma_s * sigma_s));

                double intensity_diff = center_intensity - static_cast<double>(intensity[nidx]);
                double range_weight = std::exp(-(intensity_diff * intensity_diff) / (2.0 * sigma_r * sigma_r));

                double weight = spatial_weight * range_weight;
                sum += weight * static_cast<double>(depth[nidx]);
                total_weight += weight;
            }

            if (total_weight > 0.0) {
                output_depth[idx] = static_cast<uint8_t>(std::round(sum / total_weight));
            } else {
                output_depth[idx] = depth[idx]; // fallback to original
            }
        }
    }
    return true;
}

#include <cassert>
#include <vector>
#include <cstdint>

// The solution function is declared above (or included here)

int main() {
    // Test 1: Basic 2x2 image with no noise, uniform intensity
    {
        std::vector<uint8_t> depth = {100, 120, 140, 160};
        std::vector<uint8_t> intensity = {50, 50, 50, 50};
        std::vector<bool> noise = {false, false, false, false};
        std::vector<double> sigmaS = {2.0};
        std::vector<double> sigmaR = {20.0};
        std::vector<uint8_t> output;
        bool ok = jointBilateralFilter(depth, intensity, noise, sigmaS, sigmaR, output, 2, 2);
        assert(ok);
        assert(output.size() == 4);
        // Center pixels (indices 1 and 2) should be influenced by all neighbors
        // With uniform intensity, output should be close to original but not exact due to spatial weights
        assert(output[0] >= 90 && output[0] <= 150);
        assert(output[2] >= 90 && output[2] <= 190);
    }

    // Test 2: Noise mask sets pixels to 0
    {
        std::vector<uint8_t> depth = {100, 120, 140, 160};
        std::vector<uint8_t> intensity = {50, 50, 50, 50};
        std::vector<bool> noise = {true, false, false, false};
        std::vector<double> sigmaS = {2.0};
        std::vector<double> sigmaR = {20.0};
        std::vector<uint8_t> output;
        bool ok = jointBilateralFilter(depth, intensity, noise, sigmaS, sigmaR, output, 2, 2);
        assert(ok);
        assert(output[0] == 0); // noisy pixel
        assert(output[1] > 0);  // valid pixel
    }

    // Test 3: Mismatched vector lengths returns false
    {
        std::vector<uint8_t> depth = {1, 2, 3, 4};
        std::vector<uint8_t> intensity = {1, 2, 3, 4};
        std::vector<bool> noise = {false, false, false, false};
        std::vector<double> sigmaS = {1.0, 2.0}; // length 2
        std::vector<double> sigmaR = {1.0};      // length 1
        std::vector<uint8_t> output;
        bool ok = jointBilateralFilter(depth, intensity, noise, sigmaS, sigmaR, output, 2, 2);
        assert(!ok);
    }

    // Test 4: Invalid image dimensions (height/width don't match data)
    {
        std::vector<uint8_t> depth = {1, 2, 3}; // only 3 elements, but 2x2 needs 4
        std::vector<uint8_t> intensity = {1, 2, 3, 4};
        std::vector<bool> noise = {false, false, false, false};
        std::vector<double> sigmaS = {1.0};
        std::vector<double> sigmaR = {1.0};
        std::vector<uint8_t> output;
        bool ok = jointBilateralFilter(depth, intensity, noise, sigmaS, sigmaR, output, 2, 2);
        assert(!ok);
    }

    // Test 5: Edge pixel with all neighbors valid (3x3 image)
    {
        std::vector<uint8_t> depth = {10, 20, 30, 40, 50, 60, 70, 80, 90};
        std::vector<uint8_t> intensity = {100, 100, 100, 100, 100, 100, 100, 100, 100};
        std::vector<bool> noise(9, false);
        std::vector<double> sigmaS = {100.0}; // large spatial sigma
        std::vector<double> sigmaR = {1000.0}; // large range sigma
        std::vector<uint8_t> output;
        bool ok = jointBilateralFilter(depth, intensity, noise, sigmaS, sigmaR, output, 3, 3);
        assert(ok);
        // Center pixel (index 4) should be close to average of all, which is 50
        assert(output[4] >= 45 && output[4] <= 55);
        // Corner pixel (index 0) should be closer to its neighborhood average
        assert(output[0] >= 5 && output[0] <= 45);
    }

    // Test 6: Zero sigma values should not crash
    {
        std::vector<uint8_t> depth = {10, 20, 30, 40};
        std::vector<uint8_t> intensity = {10, 20, 30, 40};
        std::vector<bool> noise = {false, false, false, false};
        std::vector<double> sigmaS = {0.0};
        std::vector<double> sigmaR = {0.0};
        std::vector<uint8_t> output;
        bool ok = jointBilateralFilter(depth, intensity, noise, sigmaS, sigmaR, output, 2, 2);
        assert(ok);
    }

    return 0;
}
