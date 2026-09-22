// Write a C++ function `ycc_to_rgb` that converts a planar YCbCr image (with 8-bit sample values in the range 0–255) into an interleaved RGB image using the BT.601 conversion equations with integer fixed-point arithmetic. The function must take a 2D vector-of-vectors representing three input planes (Y, Cb, Cr), each of size `height × width`, and produce an output 2D vector of size `height × (width * 3)` where each pixel stores red, green, blue in order. Use the constants: `R = Y + 1.402*(Cb-128)`, `G = Y - 0.344136286*(Cb-128) - 0.714136286*(Cr-128)`, `B = Y + 1.772*(Cb-128)`. Apply range limiting to clamp the final RGB values to 0–255. Precompute lookup tables indexed by Cb and Cr for each component to avoid multiplications in the inner loop, using `FIX(x) = round(x * 2^16)` and rounding after the green sum. The function must be efficient, deterministic, and match the IEEE 8-bit rounding behavior.
The core approach is to precompute four tables: `Cr_to_R[256]` (nearest integer of `1.402 * (i-128)`), `Cb_to_B[256]` (nearest integer of `1.772 * (i-128)`), `Cr_to_G[256]` (scaled integer of `-0.714136286 * (i-128)`), and `Cb_to_G[256]` (scaled integer of `-0.344136286 * (i-128)` with an added `ONE_HALF` so it can be added directly before shifting). Use 16-bit fixed-point scaling (`SCALEBITS = 16`). For each pixel, compute red as `Y + Cr_to_R[cr]`, blue as `Y + Cb_to_B[cb]`, and green as `Y + ((Cb_to_G[cb] + Cr_to_G[cr]) >> 16)`. Then clamp each result to the range 0–255 using `std::min(std::max(value, 0), 255)`. The tables are computed once outside the pixel loop. For an image with `N = width * height` pixels, the time complexity is `O(N)` for the conversion after an `O(1)` table setup (constant 256 entries). Space complexity is `O(1)` for the four fixed-size tables plus `O(N * 3)` for the output, which is required. Edge cases include Cb or Cr values near 0 or 255 causing negative or >255 results, which are handled by clamping; also ensure the `width` and `height` are consistent across all three input planes.
#include <vector>
#include <cstdint>
#include <algorithm>
#include <cmath>

// Fixed-point scaling constants
static constexpr int SCALEBITS = 16;
static constexpr int32_t ONE_HALF = 1 << (SCALEBITS - 1);
static constexpr int32_t FIX(double x) {
    return static_cast<int32_t>(x * (1 << SCALEBITS) + 0.5);
}

// Convert a planar YCbCr image (8-bit per sample) to interleaved RGB.
// Input: three 2D vectors of size height x width for Y, Cb, Cr.
// Output: 2D vector of size height x (width * 3), each pixel [R, G, B].
std::vector<std::vector<uint8_t>> ycc_to_rgb(
    const std::vector<std::vector<uint8_t>>& Y,
    const std::vector<std::vector<uint8_t>>& Cb,
    const std::vector<std::vector<uint8_t>>& Cr)
{
    const size_t height = Y.size();
    const size_t width = (height > 0) ? Y[0].size() : 0;

    // Precompute lookup tables for speed (256 entries each, since 8-bit)
    int32_t Cr_to_R[256], Cb_to_B[256];
    int32_t Cr_to_G[256], Cb_to_G[256];

    for (int i = 0; i < 256; ++i) {
        int x = i - 128; // center value
        // Round to nearest integer
        Cr_to_R[i] = (FIX(1.402) * x + ONE_HALF) >> SCALEBITS;
        Cb_to_B[i] = (FIX(1.772) * x + ONE_HALF) >> SCALEBITS;
        // Keep scaled for sum before rounding; add ONE_HALF for green only
        Cr_to_G[i] = -FIX(0.714136286) * x;
        Cb_to_G[i] = -FIX(0.344136286) * x + ONE_HALF;
    }

    // Output interleaved RGB
    std::vector<std::vector<uint8_t>> RGB(height, std::vector<uint8_t>(width * 3));

    for (size_t row = 0; row < height; ++row) {
        for (size_t col = 0; col < width; ++col) {
            int y  = Y[row][col];
            int cb = Cb[row][col];
            int cr = Cr[row][col];

            int r = y + Cr_to_R[cr];
            int g = y + ((Cb_to_G[cb] + Cr_to_G[cr]) >> SCALEBITS);
            int b = y + Cb_to_B[cb];

            // Clamp to [0, 255]
            r = std::clamp(r, 0, 255);
            g = std::clamp(g, 0, 255);
            b = std::clamp(b, 0, 255);

            size_t out_index = col * 3;
            RGB[row][out_index]     = static_cast<uint8_t>(r);
            RGB[row][out_index + 1] = static_cast<uint8_t>(g);
            RGB[row][out_index + 2] = static_cast<uint8_t>(b);
        }
    }

    return RGB;
}
#include <cassert>
#include <cmath>

// The solution function must be declared above or included here.
int main() {
    // Test 1: Neutral gray (Cb=128, Cr=128) should produce same Y value in all channels
    std::vector<std::vector<uint8_t>> Y1 = {{100, 200}};
    std::vector<std::vector<uint8_t>> Cb1 = {{128, 128}};
    std::vector<std::vector<uint8_t>> Cr1 = {{128, 128}};
    auto out1 = ycc_to_rgb(Y1, Cb1, Cr1);
    assert(out1[0][0] == 100 && out1[0][1] == 100 && out1[0][2] == 100);
    assert(out1[0][3] == 200 && out1[0][4] == 200 && out1[0][5] == 200);

    // Test 2: Pure blue in YCbCr (Y=29, Cb=255, Cr=107) should produce near (0,0,255)
    std::vector<std::vector<uint8_t>> Y2 = {{29}};
    std::vector<std::vector<uint8_t>> Cb2 = {{255}};
    std::vector<std::vector<uint8_t>> Cr2 = {{107}};
    auto out2 = ycc_to_rgb(Y2, Cb2, Cr2);
    assert(out2[0][0] <= 5 && out2[0][1] <= 5 && out2[0][2] == 255);

    // Test 3: Pure red in YCbCr (Y=76, Cb=85, Cr=255) should produce near (255,0,0)
    std::vector<std::vector<uint8_t>> Y3 = {{76}};
    std::vector<std::vector<uint8_t>> Cb3 = {{85}};
    std::vector<std::vector<uint8_t>> Cr3 = {{255}};
    auto out3 = ycc_to_rgb(Y3, Cb3, Cr3);
    assert(out3[0][0] == 255 && out3[0][1] <= 5 && out3[0][2] <= 5);

    // Test 4: Extreme values must be clamped to 0..255
    std::vector<std::vector<uint8_t>> Y4 = {{255}};
    std::vector<std::vector<uint8_t>> Cb4 = {{0}};
    std::vector<std::vector<uint8_t>> Cr4 = {{0}};
    auto out4 = ycc_to_rgb(Y4, Cb4, Cr4);
    assert(out4[0][0] >= 0 && out4[0][0] <= 255);
    assert(out4[0][1] >= 0 && out4[0][1] <= 255);
    assert(out4[0][2] >= 0 && out4[0][2] <= 255);

    // Test 5: Multiple rows and columns, check dimensions and a known value
    std::vector<std::vector<uint8_t>> Y5 = {{0, 10}, {20, 30}};
    std::vector<std::vector<uint8_t>> Cb5 = {{128, 128}, {128, 128}};
    std::vector<std::vector<uint8_t>> Cr5 = {{128, 128}, {128, 128}};
    auto out5 = ycc_to_rgb(Y5, Cb5, Cr5);
    assert(out5.size() == 2);
    assert(out5[0].size() == 6);
    assert(out5[0][0] == 0 && out5[0][1] == 0 && out5[0][2] == 0);
    assert(out5[1][3] == 30 && out5[1][4] == 30 && out5[1][5] == 30);

    return 0;
}
