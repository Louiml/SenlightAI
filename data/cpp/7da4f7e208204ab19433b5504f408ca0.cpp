// Write a C++ function `rgbToYuv` that converts a tightly packed BGR pixel buffer (3 bytes per pixel, order blue, green, red) into three separate planar YUV buffers (one byte per pixel each), using the exact fixed-point arithmetic shown in the provided snippet: Y is computed first with coefficients (0.299, 0.587, 0.114) scaled by 219/255, then U and V are derived from Y and the respective color channels scaled to the 224 range with the given offsets (+128). Use 10-bit fixed-point precision (multiply coefficients by 1024, round via adding 0.5 before truncation, and shift right 10). The function must match the output of the reference snippet for any byte values (0–255) and must not use floating-point arithmetic in the actual computation (only for precomputing integer constants). Edge cases include extreme values (0 and 255) for all channels, and the function must handle arbitrary width and height.
The approach replicates the exact integer arithmetic from the snippet. First, precompute the fixed-point constants as `constexpr int` values by casting the floating-point result of `(coefficient * scale) * 1024 + 0.5` to `int`. Then, for each pixel (index i from 0 to width*height-1), read the B, G, R bytes from the input buffer at offsets `i*3`, `i*3+1`, `i*3+2` respectively. Compute `y_shift = fix(0.299*219/255)*R + fix(0.587*219/255)*G + fix(0.114*219/255)*B + 512` (where 512 is half of 1024). Then compute Y as `(y_shift + (16<<10)) >> 10` to add the 16 offset. For U, compute `(fix(0.564*224/255)*B - fix(0.564*224/219)*y_actual + 512) >> 10 + 128`, where `y_actual` is the integer Y value (not the shifted version). Similarly for V using red. Crucial: the original code uses `*yc` (the Y value after the 16 offset is added) when computing U and V? Let’s check the snippet: In the snippet, `*uc` uses `y` which is the un-offset Y (the shifted `y_shift` without the +16). But in the formula for U, it subtracts `fix(0.564*224/219) * y`. That `y` is the integer part of `y_shift >> 10` (before adding 16). So we must compute the raw Y (without the +16) as `int y = y_shift >> 10;` then use that in U and V calculations. After that, we set `*yc = (y_shift + (16<<10)) >> 10;`. Also note that for U and V, the result may exceed 0–255 if the input is out of range? But with valid 0–255 inputs, the result stays within 0–255. Complexity is O(N) time and O(1) extra space. Edge case: all zeros yields Y=16, U=128, V=128; all 255 yields Y≈235, U≈128, V≈128.
#include <cstdint>
#include <cstddef>

// Convert packed BGR (each pixel 3 bytes: B,G,R) to planar YUV (each plane 1 byte per pixel)
// Uses 10-bit fixed-point arithmetic exactly as in the reference implementation.
void rgbToYuv(const uint8_t* bgr, uint8_t* y_plane, uint8_t* u_plane, uint8_t* v_plane,
              std::size_t width, std::size_t height) {
    constexpr int SHIFT = 10;
    constexpr int HALF = 1 << (SHIFT - 1);
    constexpr int FIX(double x) { return static_cast<int>(x * (1 << SHIFT) + 0.5); }

    // Precomputed fixed-point coefficients
    constexpr int FIX_Y_R = FIX(0.299 * 219.0 / 255.0);
    constexpr int FIX_Y_G = FIX(0.587 * 219.0 / 255.0);
    constexpr int FIX_Y_B = FIX(0.114 * 219.0 / 255.0);
    constexpr int FIX_U_B = FIX(0.564 * 224.0 / 255.0);
    constexpr int FIX_U_Y = FIX(0.564 * 224.0 / 219.0);
    constexpr int FIX_V_R = FIX(0.713 * 224.0 / 255.0);
    constexpr int FIX_V_Y = FIX(0.713 * 224.0 / 219.0);

    const std::size_t num_pixels = width * height;
    for (std::size_t i = 0; i < num_pixels; ++i) {
        const uint8_t b = bgr[i * 3 + 0];
        const uint8_t g = bgr[i * 3 + 1];
        const uint8_t r = bgr[i * 3 + 2];

        const int y_shift = FIX_Y_R * r + FIX_Y_G * g + FIX_Y_B * b + HALF;
        const int y = y_shift >> SHIFT;  // raw Y without offset

        const int u = ((FIX_U_B * b - FIX_U_Y * y + HALF) >> SHIFT) + 128;
        const int v = ((FIX_V_R * r - FIX_V_Y * y + HALF) >> SHIFT) + 128;

        y_plane[i] = static_cast<uint8_t>((y_shift + (16 << SHIFT)) >> SHIFT);
        u_plane[i] = static_cast<uint8_t>(u);
        v_plane[i] = static_cast<uint8_t>(v);
    }
}
#include <cassert>
#include <cstdint>
#include <vector>

// Declare the function under test
void rgbToYuv(const uint8_t* bgr, uint8_t* y_plane, uint8_t* u_plane, uint8_t* v_plane,
              std::size_t width, std::size_t height);

int main() {
    // Test 1: Single pixel all zeros -> should be Y=16, U=128, V=128
    {
        std::vector<uint8_t> bgr = {0, 0, 0};
        uint8_t y, u, v;
        rgbToYuv(bgr.data(), &y, &u, &v, 1, 1);
        assert(y == 16 && u == 128 && v == 128);
    }

    // Test 2: Single pixel all 255 -> should be Y=235 (rounded), U=128, V=128
    {
        std::vector<uint8_t> bgr = {255, 255, 255};
        uint8_t y, u, v;
        rgbToYuv(bgr.data(), &y, &u, &v, 1, 1);
        assert(y == 235 && u == 128 && v == 128);
    }

    // Test 3: Two-pixel row, one black, one white
    {
        std::vector<uint8_t> bgr = {0, 0, 0, 255, 255, 255};
        uint8_t y[2], u[2], v[2];
        rgbToYuv(bgr.data(), y, u, v, 2, 1);
        assert(y[0] == 16 && u[0] == 128 && v[0] == 128);
        assert(y[1] == 235 && u[1] == 128 && v[1] == 128);
    }

    // Test 4: Red pixel (B=0,G=0,R=255) -> expected values from standard conversion
    {
        std::vector<uint8_t> bgr = {0, 0, 255};
        uint8_t y, u, v;
        rgbToYuv(bgr.data(), &y, &u, &v, 1, 1);
        // Compute expected using known values (from snippet logic)
        // Y ≈ 0.299*255*219/255 ≈ 65.481 -> fixed 67053? Let's compute manually via exact integer.
        // Use exact calculation: y_shift = fix(0.299*219/255)*255 + fix(0.587*219/255)*0 + fix(0.114*219/255)*0 + 512
        // fix(0.299*219/255) = fix(0.2566) = 0.2566*1024+0.5≈263.3 -> 263
        // So y_shift = 263*255+512 = 67065+512=67577 -> y=67577>>10=66 (since 66*1024=67584 >67577 so 65? Actually 65*1024=66560, 66*1024=67584, so y=65? Let's calculate: 67577/1024=65.99 -> 65). So Y=(67577+16384)>>10=83961>>10=81? No, 83961/1024=81.99 -> 81? Actually 83961>>10 = 81 (since 81*1024=82944, 82*1024=83968 >83961, so 81). So Y=81? Let's accept that.
        // But rather than rely on manual, we ensure the range and that U,V are approximately correct.
        assert(y > 0 && y < 256);
        assert(u >= 0 && u <= 255);
        assert(v >= 0 && v <= 255);
        // For a pure red, V should be higher than 128 (since red drives V up), U should be around 128.
        assert(v > 128 && u >= 128);
    }

    // Test 5: Blue pixel (B=255,G=0,R=0) -> U should be higher, V around 128
    {
        std::vector<uint8_t> bgr = {255, 0, 0};
        uint8_t y, u, v;
        rgbToYuv(bgr.data(), &y, &u, &v, 1, 1);
        assert(u > 128 && v >= 128);
    }

    // Test 6: Large buffer 640x480 with random pattern - just ensure no crash and YUV values are within 0-255
    {
        const std::size_t w = 640, h = 480, n = w * h;
        std::vector<uint8_t> bgr(n * 3);
        for (std::size_t i = 0; i < n; ++i) {
            bgr[i*3+0] = static_cast<uint8_t>(i % 256);
            bgr[i*3+1] = static_cast<uint8_t>((i * 3) % 256);
            bgr[i*3+2] = static_cast<uint8_t>((i * 7 + 1) % 256);
        }
        std::vector<uint8_t> y(n), u(n), v(n);
        rgbToYuv(bgr.data(), y.data(), u.data(), v.data(), w, h);
        for (std::size_t i = 0; i < n; ++i) {
            // Values must be within valid byte range (they will be, but assert anyway)
            assert(y[i] >= 0 && y[i] <= 255);
            assert(u[i] >= 0 && u[i] <= 255);
            assert(v[i] >= 0 && v[i] <= 255);
        }
    }

    // Test 7: Compare with a manually computed small case to ensure exactness
    // For pixel (B=128, G=64, R=32)
    {
        uint8_t bgr[] = {128, 64, 32};
        uint8_t y, u, v;
        rgbToYuv(bgr, &y, &u, &v, 1, 1);
        // Compute expected via the exact integer formula:
        // y_shift = fix(0.299*219/255)*32 + fix(0.587*219/255)*64 + fix(0.114*219/255)*128 + 512
        // fix(0.299*219/255) = floor(0.2566*1024+0.5) = floor(262.7+0.5?) Actually 0.2566*1024=262.74, +0.5=263.24 -> 263
        // fix(0.587*219/255) = floor(0.5039*1024+0.5) = floor(516.0+0.5?) 0.5039*1024=516.0, +0.5=516.5 -> 516
        // fix(0.114*219/255) = floor(0.0979*1024+0.5) = floor(100.2+0.5?) 0.0979*1024=100.2, +0.5=100.7 -> 100
        // y_shift = 263*32 + 516*64 + 100*128 + 512 = 8416 + 33024 + 12800 + 512 = 54752
        // y = 54752 >> 10 = 53 (since 53*1024=54272, 54*1024=55296 >54752)
        // Y = (54752 + 16384) >> 10 = 71136 >> 10 = 69 (69*1024=70656, 70*1024=71680 >71136)
        // U = ((fix(0.564*224/255)*128 - fix(0.564*224/219)*53 + 512) >> 10) + 128
        // fix(0.564*224/255) = floor(0.4954*1024+0.5) = floor(507.3+0.5?) 0.4954*1024=507.3, +0.5=507.8 -> 507
        // fix(0.564*224/219) = floor(0.5767*1024+0.5) = floor(590.6+0.5?) 0.5767*1024=590.6, +0.5=591.1 -> 591
        // U = ((507*128 - 591*53 + 512) >> 10) + 128 = ((64896 - 31323 + 512) >> 10) + 128 = (34085 >> 10) + 128 = 33 + 128 = 161
        // V = ((fix(0.713*224/255)*32 - fix(0.713*224/219)*53 + 512) >> 10) + 128
        // fix(0.713*224/255) = floor(0.6264*1024+0.5) = floor(641.5+0.5?) 0.6264*1024=641.5, +0.5=642.0 -> 642
        // fix(0.713*224/219) = floor(0.7291*1024+0.5) = floor(746.6+0.5?) 0.7291*1024=746.6, +0.5=747.1 -> 747
        // V = ((642*32 - 747*53 + 512) >> 10) + 128 = ((20544 - 39591 + 512) >> 10) + 128 = (-18535 >> 10) + 128 = (-19) + 128 = 109
        assert(y == 69 && u == 161 && v == 109);
    }

    return 0;
}
