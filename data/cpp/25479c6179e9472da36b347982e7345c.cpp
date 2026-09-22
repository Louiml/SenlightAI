// Write a standalone C++ function `computeBriefDescriptor` that takes a grayscale image represented as a 2D `std::vector<std::vector<unsigned char>>` (row-major, pixel values 0–255), a keypoint given by integer `(x, y)` coordinates and an optional orientation angle in degrees, and a descriptor byte length (16, 32, or 64). The function must return a `std::vector<unsigned char>` representing a simplified BRIEF descriptor: for each byte, it compares 8 pairs of pixels within a patch of radius 24 around the keypoint, using a deterministic pseudo-random pair generator (e.g., a fixed seed with a simple LCG) to select pair offsets. For each pair, compare the smoothed intensity at two locations — smoothed via a 9×9 box filter (approximated by summing a 9×9 neighborhood) — and set the corresponding bit to 1 if the first pixel's smoothed value is greater than the second, else 0. If `use_orientation` is true, rotate the pair offsets by the given angle (in degrees) before applying them. Pixels outside the image bounds (after rotation offset) should be treated as having value 0. The function should handle any descriptor size (16/32/64 bytes) and validate that the keypoint is at least 25 pixels from the border (patch radius 24 + kernel half-size) — if not, return an empty descriptor. The pair generation must be deterministic: use a simple LCG with seed 12345, generating two offsets per bit (x and y for each of the two pixels, each within [-24, 24]). The output must be exactly `bytes` bytes, with bits packed MSB-first (bit 7 of byte 0 is the first comparison result). Provide a standalone function with the signature: `std::vector<unsigned char> computeBriefDescriptor(const std::vector<std::vector<unsigned char>>& image, int x, int y, float angle_deg, int bytes, bool use_orientation)`. No external libraries (like OpenCV) are allowed — only standard C++.
The solution requires: (1) validating the keypoint is far enough from the border (at least 24 + 4 = 28 pixels from edges, since a 9×9 box filter needs 4 pixels on each side; but the problem states use radius 24 + kernel half-size, which is 24+4=28, but the problem text says "patch radius 24", so use 24+4=28 for safety—however the task says "radius 24" and "kernel half-size" — follow the task: require x >= 28 and x < width-28, similarly y. If out of bounds, return empty vector). (2) Precompute a cumulative sum table (integral image) for fast box sum queries. The integral image `sum` of size (height+1) × (width+1) where sum[i+1][j+1] = sum of image[0..i][0..j]. Box sum for region (y1, x1) to (y2, x2) inclusive = sum[y2+1][x2+1] - sum[y1][x2+1] - sum[y2+1][x1] + sum[y1][x1]. For a 9×9 box centered at (cx, cy) with half=4, the region is (cy-4, cx-4) to (cy+4, cx+4). (3) Generate 8*bytes pairs deterministically. Use an LCG: state = (state * 1103515245 + 12345) & 0x7fffffff, then extract x1, y1, x2, y2 each as (state % 49) - 24 (i.e., -24..24). For each bit, generate 4 numbers (x1, y1, x2, y2). If `use_orientation`, rotate each offset by angle θ: apply rotation matrix [cosθ, -sinθ; sinθ, cosθ] to the 2D vector (dx, dy) — but note the original code uses some sine/cosine swap; we'll do standard rotation: new_x = dx*cos - dy*sin, new_y = dx*sin + dy*cos, then round to nearest integer. Then compute the smoothed sum at (x + offset_x, y + offset_y) using the box filter. If the resulting pixel coordinate is outside the image (0..width-1, 0..height-1), treat smoothed sum as 0. (4) Compare the two sums; if sum1 > sum2, set bit=1, else 0. Accumulate bits into bytes. (5) Time complexity: O(width*height) for integral image + O(bytes*8) for pairs — effectively O(W*H + bytes). Space: O(W*H) for the integral image. Edge cases: keypoint near border → return empty vector; image empty → return empty; bytes must be 16/32/64 (if not, return empty or perhaps throw — we'll return empty for invalid). Also handle negative offsets; rotation rounding must be inside the patch range; but we clamp? In the original code they clamp rx/ry to [-24,24], we can do similar. We'll implement a helper function for box sum.
#include <vector>
#include <cmath>
#include <cstdint>

// Compute the smoothed 9x9 box sum at integer (cx, cy) using integral image.
// integral is (height+1) x (width+1); imageWidth/imageHeight are original dimensions.
// If the box extends outside the image, we clamp to valid region? The original code assumes keypoint is far from border, so we can assume valid. We'll trust the border check.
int boxSum(const std::vector<std::vector<int>>& integral, int cx, int cy, int width, int height) {
    const int half = 4; // 9x9 kernel half
    int x1 = cx - half;
    int y1 = cy - half;
    int x2 = cx + half;
    int y2 = cy + half;
    if (x1 < 0) x1 = 0;
    if (y1 < 0) y1 = 0;
    if (x2 >= width) x2 = width - 1;
    if (y2 >= height) y2 = height - 1;
    if (x2 < x1 || y2 < y1) return 0;
    int topLeft = integral[y1][x1];
    int topRight = integral[y1][x2+1];
    int bottomLeft = integral[y2+1][x1];
    int bottomRight = integral[y2+1][x2+1];
    return bottomRight - topRight - bottomLeft + topLeft;
}

std::vector<unsigned char> computeBriefDescriptor(
    const std::vector<std::vector<unsigned char>>& image,
    int x, int y, float angle_deg, int bytes, bool use_orientation) {
    // Validate inputs
    if (bytes != 16 && bytes != 32 && bytes != 64) return {};
    int height = static_cast<int>(image.size());
    if (height == 0) return {};
    int width = static_cast<int>(image[0].size());
    if (width == 0) return {};

    // Border check: patch radius 24 + kernel half 4 = 28
    const int border = 28;
    if (x < border || x >= width - border || y < border || y >= height - border) {
        return {};
    }

    // Build integral image
    std::vector<std::vector<int>> integral(height + 1, std::vector<int>(width + 1, 0));
    for (int i = 0; i < height; ++i) {
        int rowSum = 0;
        for (int j = 0; j < width; ++j) {
            rowSum += static_cast<int>(image[i][j]);
            integral[i+1][j+1] = integral[i][j+1] + rowSum;
        }
    }

    // Precompute rotation coefficients if needed
    float cos_a = 1.0f, sin_a = 0.0f;
    if (use_orientation) {
        float rad = angle_deg * static_cast<float>(M_PI / 180.0);
        cos_a = std::cos(rad);
        sin_a = std::sin(rad);
    }

    // Deterministic LCG for pair generation
    uint32_t state = 12345;
    auto nextRand = [&state]() -> int {
        state = state * 1103515245 + 12345;
        return static_cast<int>((state >> 16) & 0x7FFF); // 15 bits
    };

    // Generate pairs and compute descriptor
    std::vector<unsigned char> descriptor(bytes, 0);
    int bitIndex = 0;
    int byteIndex = 0;
    int bitInByte = 0;

    auto processPair = [&](int dx1, int dy1, int dx2, int dy2) {
        // Apply rotation if needed
        int sx1, sy1, sx2, sy2;
        if (use_orientation) {
            // Rotate and round
            float rx1 = dx1 * cos_a - dy1 * sin_a;
            float ry1 = dx1 * sin_a + dy1 * cos_a;
            float rx2 = dx2 * cos_a - dy2 * sin_a;
            float ry2 = dx2 * sin_a + dy2 * cos_a;
            // Clamp to [-24,24]
            sx1 = static_cast<int>(std::round(rx1));
            sy1 = static_cast<int>(std::round(ry1));
            sx2 = static_cast<int>(std::round(rx2));
            sy2 = static_cast<int>(std::round(ry2));
            if (sx1 > 24) sx1 = 24; if (sx1 < -24) sx1 = -24;
            if (sy1 > 24) sy1 = 24; if (sy1 < -24) sy1 = -24;
            if (sx2 > 24) sx2 = 24; if (sx2 < -24) sx2 = -24;
            if (sy2 > 24) sy2 = 24; if (sy2 < -24) sy2 = -24;
        } else {
            sx1 = dx1; sy1 = dy1; sx2 = dx2; sy2 = dy2;
        }

        int px1 = x + sx1, py1 = y + sy1;
        int px2 = x + sx2, py2 = y + sy2;

        int sum1 = 0, sum2 = 0;
        if (px1 >= 0 && px1 < width && py1 >= 0 && py1 < height) {
            sum1 = boxSum(integral, px1, py1, width, height);
        }
        if (px2 >= 0 && px2 < width && py2 >= 0 && py2 < height) {
            sum2 = boxSum(integral, px2, py2, width, height);
        }

        int bit = (sum1 > sum2) ? 1 : 0;
        if (bit) {
            descriptor[byteIndex] |= (1 << (7 - bitInByte));
        }
        ++bitInByte;
        if (bitInByte == 8) {
            bitInByte = 0;
            ++byteIndex;
        }
    };

    int totalBits = bytes * 8;
    for (int i = 0; i < totalBits; ++i) {
        // Generate 4 offsets: dx1, dy1, dx2, dy2 each in [-24, 24]
        int dx1 = (nextRand() % 49) - 24;
        int dy1 = (nextRand() % 49) - 24;
        int dx2 = (nextRand() % 49) - 24;
        int dy2 = (nextRand() % 49) - 24;
        processPair(dx1, dy1, dx2, dy2);
    }

    return descriptor;
}
#include <cassert>
#include <vector>
#include <iostream>

// Include the solution function here (or link appropriately)
// For simplicity, assume the solution code is above in the same file.

int main() {
    // Create a simple 100x100 image with a constant gradient
    int w = 100, h = 100;
    std::vector<std::vector<unsigned char>> img(h, std::vector<unsigned char>(w));
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            img[i][j] = static_cast<unsigned char>((i + j) % 256);
        }
    }

    // Test 1: descriptor size
    auto d16 = computeBriefDescriptor(img, 50, 50, 0.0f, 16, false);
    assert(d16.size() == 16);
    auto d32 = computeBriefDescriptor(img, 50, 50, 0.0f, 32, false);
    assert(d32.size() == 32);
    auto d64 = computeBriefDescriptor(img, 50, 50, 0.0f, 64, false);
    assert(d64.size() == 64);

    // Test 2: deterministic output
    auto d32a = computeBriefDescriptor(img, 50, 50, 0.0f, 32, false);
    auto d32b = computeBriefDescriptor(img, 50, 50, 0.0f, 32, false);
    assert(d32a == d32b);

    // Test 3: different keypoints give different descriptors (likely)
    auto d32_at_60 = computeBriefDescriptor(img, 60, 60, 0.0f, 32, false);
    assert(d32a != d32_at_60);

    // Test 4: orientation matters
    auto d32_rot = computeBriefDescriptor(img, 50, 50, 45.0f, 32, true);
    assert(d32_rot.size() == 32);
    assert(d32_rot != d32a); // likely different

    // Test 5: border keypoint returns empty
    auto d_border = computeBriefDescriptor(img, 10, 50, 0.0f, 32, false);
    assert(d_border.empty());

    // Test 6: invalid bytes returns empty
    auto d_invalid = computeBriefDescriptor(img, 50, 50, 0.0f, 24, false);
    assert(d_invalid.empty());

    // Test 7: empty image
    std::vector<std::vector<unsigned char>> empty_img;
    auto d_empty = computeBriefDescriptor(empty_img, 50, 50, 0.0f, 32, false);
    assert(d_empty.empty());

    // Test 8: all zero image -> all bits zero because sums equal (0 vs 0) => bit=0
    std::vector<std::vector<unsigned char>> zeros(100, std::vector<unsigned char>(100, 0));
    auto d_zeros = computeBriefDescriptor(zeros, 50, 50, 0.0f, 16, false);
    assert(d_zeros.size() == 16);
    for (unsigned char byte : d_zeros) {
        assert(byte == 0);
    }

    // Test 9: image with single bright spot at center -> first pixel likely brighter than many pairs
    std::vector<std::vector<unsigned char>> spot(100, std::vector<unsigned char>(100, 0));
    spot[50][50] = 255;
    auto d_spot = computeBriefDescriptor(spot, 50, 50, 0.0f, 32, false);
    assert(d_spot.size() == 32);
    bool has_one = false;
    for (unsigned char byte : d_spot) {
        if (byte != 0) { has_one = true; break; }
    }
    assert(has_one); // at least some bits set

    std::cout << "All tests passed.\n";
    return 0;
}
