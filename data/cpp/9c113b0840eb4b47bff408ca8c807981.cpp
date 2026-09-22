Write a standalone C++ function `computeColorfulness` that takes a vector representing an RGB image (normalized to [0,1]) along with its width and height, and returns a single `double` value quantifying the image's colorfulness based on the metric proposed by Hasler and Süsstrunk. The metric computes the mean and standard deviation of the pixel-wise chroma values in the opponent color space, where chroma for each pixel is defined as the Euclidean distance from the gray axis: `sqrt(a^2 + b^2)` with `a = R - G` and `b = 0.5*(R + G) - B`. The final score is `std_dev_chroma + 0.3 * mean_chroma`. The function must validate that the input vector size matches `width * height * 3` (throw `std::invalid_argument` otherwise), handle an empty image gracefully, and be robust to pixels with NaN or infinite channel values (skip such pixels when accumulating statistics). The returned score should be non-negative (clamp to zero if computed negative due to floating-point artifacts). Use appropriate const correctness and include necessary headers.
// The solution iterates over every pixel once, computing the opponent color coordinates `a` and `b` for each valid pixel. For each pixel, we first check whether any of its RGB channels is NaN or infinite; if so, that pixel is skipped entirely. We accumulate the sum of chroma values, the sum of squared chroma values, and a count of valid pixels. After the loop, the mean chroma is `sum / count` (if count > 0), and the variance is `(sum_sq / count) - (mean^2)`, which mathematically equals the average squared deviation; the standard deviation is the square root of this variance. We must guard against tiny negative variance due to floating-point rounding by clamping to zero before taking the square root. The final colorfulness score is `std_dev + 0.3 * mean`. Edge cases include: an image with zero pixels (width*height = 0) should return 0.0 (since no chroma can be measured); an image where all pixels are invalid (NaN/inf) should also return 0.0; constants like monochromatic images produce zero chroma for all pixels, so the score is 0.0. Time complexity is O(N) where N = width*height, and space complexity is O(1) beyond the input vector.
#include <vector>
#include <cmath>
#include <stdexcept>
#include <limits>

// Compute the Hasler-Süsstrunk colorfulness metric for an RGB image.
// The image is stored as a flat vector of doubles with pixel ordering row-major,
// each pixel having three channels (R, G, B) in [0,1].
// Returns a non-negative score = std_dev(chroma) + 0.3 * mean(chroma)
// where chroma for a pixel = sqrt((R-G)^2 + (0.5*(R+G)-B)^2).
// Pixels containing NaN or infinity are skipped. Empty images return 0.0.
double computeColorfulness(const std::vector<double>& rgb_image, int width, int height) {
    // Validate dimensions.
    if (width < 0 || height < 0) {
        throw std::invalid_argument("Width and height must be non-negative");
    }
    size_t expected_size = static_cast<size_t>(width) * static_cast<size_t>(height) * 3;
    if (rgb_image.size() != expected_size) {
        throw std::invalid_argument("Image vector size does not match width*height*3");
    }
    
    // Handle empty images or images with zero pixels.
    if (width == 0 || height == 0 || rgb_image.empty()) {
        return 0.0;
    }
    
    double sum_chroma = 0.0;
    double sum_sq_chroma = 0.0;
    size_t valid_count = 0;
    
    const size_t num_pixels = static_cast<size_t>(width) * static_cast<size_t>(height);
    for (size_t i = 0; i < num_pixels; ++i) {
        size_t idx = i * 3;
        double r = rgb_image[idx];
        double g = rgb_image[idx + 1];
        double b = rgb_image[idx + 2];
        
        // Skip pixels with NaN or infinite components.
        if (std::isnan(r) || std::isnan(g) || std::isnan(b) ||
            std::isinf(r) || std::isinf(g) || std::isinf(b)) {
            continue;
        }
        
        // Opponent color coordinates.
        double a = r - g;
        double b_coord = 0.5 * (r + g) - b;
        
        // Chroma = Euclidean distance from gray axis.
        double chroma = std::sqrt(a * a + b_coord * b_coord);
        sum_chroma += chroma;
        sum_sq_chroma += chroma * chroma;
        ++valid_count;
    }
    
    // If no valid pixels, return 0.
    if (valid_count == 0) {
        return 0.0;
    }
    
    double mean_chroma = sum_chroma / static_cast<double>(valid_count);
    double variance = (sum_sq_chroma / static_cast<double>(valid_count)) - (mean_chroma * mean_chroma);
    // Guard against tiny negative values due to floating-point rounding.
    if (variance < 0.0) {
        variance = 0.0;
    }
    double std_dev_chroma = std::sqrt(variance);
    
    double score = std_dev_chroma + 0.3 * mean_chroma;
    // Clamp to non-negative to be safe.
    if (score < 0.0) {
        score = 0.0;
    }
    return score;
}
#include <cassert>
#include <cmath>
#include <vector>
#include <limits>

// The solution function is declared above; here we test it.

int main() {
    // 1. Empty image (0 pixels) should return 0.
    assert(computeColorfulness({}, 0, 0) == 0.0);
    assert(computeColorfulness({}, 1, 0) == 0.0);

    // 2. Monochromatic gray image (all pixels equal) should give score 0.
    std::vector<double> gray = {0.5, 0.5, 0.5, 0.5, 0.5, 0.5}; // 2 pixels
    assert(std::abs(computeColorfulness(gray, 2, 1) - 0.0) < 1e-9);

    // 3. Pure red image (1 pixel) should have non-zero chroma.
    std::vector<double> red = {1.0, 0.0, 0.0};
    double red_score = computeColorfulness(red, 1, 1);
    // For pure red: a = 1, b = 0.5 => chroma = sqrt(1 + 0.25) = sqrt(1.25)
    double expected_chroma = std::sqrt(1.25);
    // Single pixel => std_dev = 0, mean = expected_chroma => score = 0.3 * expected_chroma
    assert(std::abs(red_score - 0.3 * expected_chroma) < 1e-9);

    // 4. Image with two different colored pixels should have non-zero std_dev.
    std::vector<double> two_colors = {1.0, 0.0, 0.0,  0.0, 1.0, 0.0}; // red, green
    double two_score = computeColorfulness(two_colors, 2, 1);
    assert(two_score > red_score); // More variation => higher score

    // 5. All-NaN pixels should return 0.
    double nan_val = std::numeric_limits<double>::quiet_NaN();
    std::vector<double> nan_img = {nan_val, nan_val, nan_val};
    assert(computeColorfulness(nan_img, 1, 1) == 0.0);

    // 6. Mixed valid and invalid pixels: only valid contribute.
    std::vector<double> mixed = {1.0, 0.0, 0.0, nan_val, nan_val, nan_val}; // 1 valid red, 1 invalid
    double mixed_score = computeColorfulness(mixed, 2, 1);
    // Same as single red pixel
    assert(std::abs(mixed_score - red_score) < 1e-9);

    // 7. Invalid size should throw.
    bool threw = false;
    try {
        computeColorfulness({0.5, 0.5, 0.5}, 2, 1); // size mismatch
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);

    // 8. Negative dimensions should throw.
    threw = false;
    try {
        computeColorfulness({}, -1, 1);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);

    // 9. Constant bright color (white) has zero chroma for all pixels => score 0.
    std::vector<double> white = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0}; // 2 white pixels
    assert(std::abs(computeColorfulness(white, 2, 1) - 0.0) < 1e-9);

    // 10. Colorfulness is non-negative for random-ish small image.
    std::vector<double> random_img = {0.1, 0.4, 0.2,  0.9, 0.3, 0.7,  0.5, 0.8, 0.6}; // 3 pixels
    double score = computeColorfulness(random_img, 3, 1);
    assert(score >= 0.0);

    return 0;
}
