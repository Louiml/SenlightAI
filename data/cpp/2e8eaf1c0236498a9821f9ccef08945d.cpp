// Write a standalone C++ function `gradientMagnitudeAndOrientation` that takes a single-channel grayscale image stored as a contiguous `std::vector<float>` of size `h * w` (row-major order, meaning index `y * w + x` corresponds to row `y`, column `x`), along with its height `h` and width `w`, and returns a `std::pair<std::vector<float>, std::vector<float>>` containing the gradient magnitude and gradient orientation (in radians, in the range [0, π)) at every pixel. The gradient at each pixel is computed using central differences: `Gx = 0.5 * (I[x+1] - I[x-1])` and `Gy = 0.5 * (I[y+1] - I[y-1])`, where boundary pixels (first/last column or first/last row) use one‑sided differences (i.e., for the leftmost column `Gx = I[1] - I[0]`, for the rightmost column `Gx = I[w-1] - I[w-2]`, and similarly for top/bottom rows for `Gy`). The magnitude is `sqrt(Gx*Gx + Gy*Gy)`, and the orientation is `atan2(Gy, Gx)` normalized to be non‑negative by adding π if the result is negative. The function must handle images with `h` and `w` both at least 2 and return vectors of exactly `h * w` floats each. The function should be `const`‑correct, take the input by `const std::vector<float>&`, and use only standard C++ library components (no external libraries).

// The solution computes gradients for each pixel independently, but its core is handling boundaries correctly. For each pixel at `(y,x)`, we compute `Gx` and `Gy` using neighbor values. For interior pixels (0 < x < w-1 and 0 < y < h-1), we use central differences: `Gx = 0.5 * (I[y*w + x+1] - I[y*w + x-1])` and `Gy = 0.5 * (I[(y+1)*w + x] - I[(y-1)*w + x])`. For boundary pixels, we replace the missing neighbor with the current pixel: for `x == 0`, `Gx = I[y*w+1] - I[y*w]`; for `x == w-1`, `Gx = I[y*w + w-1] - I[y*w + w-2]`; for `y == 0`, `Gy = I[w + x] - I[x]`; for `y == h-1`, `Gy = I[(h-1)*w + x] - I[(h-2)*w + x]`. This matches the behavior of the reference gradients in the snippet (the `grad1` function uses similar boundary adjustments). After obtaining `Gx` and `Gy`, magnitude is `sqrt(Gx*Gx + Gy*Gy)` (guarding against potential floating‑point underflow with `+ 1e-30` is optional but harmless). Orientation is `atan2(Gy, Gx)`; if negative, add π to map to [0, π). The algorithm runs in O(h*w) time, using O(1) extra space beyond the output vectors (which are O(h*w) as required). Edge cases: the smallest image is 2×2, where all four pixels are boundaries; the formulas still work. The function must not modify the input; return two vectors inside a `std::pair`. Complexity is linear in the number of pixels.

#include <vector>
#include <cmath>
#include <utility>

// Compute gradient magnitude and orientation (in [0, π)) for a grayscale image.
// Input: `image` is a contiguous vector of size h*w in row-major order (y * w + x).
// Returns: a pair of vectors, first = magnitude, second = orientation (radians).
std::pair<std::vector<float>, std::vector<float>> gradientMagnitudeAndOrientation(
    const std::vector<float>& image, int h, int w)
{
    // Assume h >= 2 and w >= 2, and image.size() == h*w.
    std::vector<float> magnitude(h * w);
    std::vector<float> orientation(h * w);
    
    const float half = 0.5f;
    const float pi = 3.14159265358979323846f;
    
    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            // Compute Gx with correct boundary handling.
            float gx;
            if (x == 0) {
                // Left boundary: one-sided forward difference.
                gx = image[y * w + 1] - image[y * w];
            } else if (x == w - 1) {
                // Right boundary: one-sided backward difference.
                gx = image[y * w + w - 1] - image[y * w + w - 2];
            } else {
                // Interior: central difference.
                gx = half * (image[y * w + x + 1] - image[y * w + x - 1]);
            }
            
            // Compute Gy with correct boundary handling.
            float gy;
            if (y == 0) {
                // Top boundary: one-sided forward difference.
                gy = image[w + x] - image[x];
            } else if (y == h - 1) {
                // Bottom boundary: one-sided backward difference.
                gy = image[(h - 1) * w + x] - image[(h - 2) * w + x];
            } else {
                // Interior: central difference.
                gy = half * (image[(y + 1) * w + x] - image[(y - 1) * w + x]);
            }
            
            // Magnitude.
            float mag = std::sqrt(gx * gx + gy * gy);
            magnitude[y * w + x] = mag;
            
            // Orientation in [0, π).
            float ang = std::atan2(gy, gx);
            if (ang < 0.0f) {
                ang += pi;
            }
            orientation[y * w + x] = ang;
        }
    }
    
    return std::make_pair(std::move(magnitude), std::move(orientation));
}

#include <cassert>
#include <cmath>
#include <vector>
#include <utility>

// The solution function is assumed to be declared above.
// Test the gradient magnitude and orientation function.
int main() {
    // Test 1: 2x2 uniform image → all gradients are zero, orientation arbitrary (0).
    {
        std::vector<float> img = {1.0f, 1.0f, 1.0f, 1.0f};
        auto result = gradientMagnitudeAndOrientation(img, 2, 2);
        assert(result.first.size() == 4);
        assert(result.second.size() == 4);
        for (int i = 0; i < 4; ++i) {
            assert(std::fabs(result.first[i]) < 1e-6f);
            assert(std::fabs(result.second[i]) < 1e-6f);
        }
    }
    
    // Test 2: 2x2 with horizontal gradient: left column 0, right column 2.
    // Each row: pixel (0,0)=0, (0,1)=2, (1,0)=0, (1,1)=2
    // For left column: Gx = I[right] - I[self] = 2-0 = 2; Gy = 0 (vertical identical).
    // For right column: Gx = I[self] - I[left] = 2-0 = 2; Gy = 0.
    // Magnitude = 2, orientation = atan2(0, 2) = 0.
    {
        std::vector<float> img = {0.0f, 2.0f, 0.0f, 2.0f};
        auto result = gradientMagnitudeAndOrientation(img, 2, 2);
        for (int i = 0; i < 4; ++i) {
            assert(std::fabs(result.first[i] - 2.0f) < 1e-6f);
            assert(std::fabs(result.second[i]) < 1e-6f);
        }
    }
    
    // Test 3: 2x2 with vertical gradient: top row 0, bottom row 2.
    // For top row: Gy = I[bottom] - I[self] = 2-0 = 2; Gx = 0.
    // For bottom row: Gy = I[self] - I[top] = 2-0 = 2; Gx = 0.
    // Magnitude = 2, orientation = atan2(2, 0) = π/2.
    {
        std::vector<float> img = {0.0f, 0.0f, 2.0f, 2.0f};
        auto result = gradientMagnitudeAndOrientation(img, 2, 2);
        for (int i = 0; i < 4; ++i) {
            assert(std::fabs(result.first[i] - 2.0f) < 1e-6f);
            assert(std::fabs(result.second[i] - 3.14159265f/2.0f) < 1e-5f);
        }
    }
    
    // Test 4: 3x3 with a single central pixel spike.
    // Image: [0,0,0; 0,2,0; 0,0,0] → central pixel has Gx = 0.5*(0-0)=0, Gy=0.5*(0-0)=0,
    // but neighbors have finite gradients. Let's check the top-center pixel (0,1):
    // x=1 interior, y=0 top boundary → Gy = I[1] - I[0] = 0-0 = 0; Gx = 0.5*(I[0,2]-I[0,0]) = 0.
    // But left-center (1,0): x=0 left boundary → Gx = I[1,1]-I[1,0] = 2-0=2; Gy = 0.5*(I[2,0]-I[0,0])=0.
    // Right-center (1,2): x=2 right boundary → Gx = I[1,2]-I[1,1] = 0-2 = -2; Gy = 0.
    // Bottom-center (2,1): y=2 bottom → Gy = I[2,1]-I[1,1] = 0-2 = -2; Gx = 0.
    // Check the left-center pixel: magnitude = 2, orientation = 0 (since Gy=0, Gx>0).
    {
        std::vector<float> img = {
            0, 0, 0,
            0, 2, 0,
            0, 0, 0
        };
        auto result = gradientMagnitudeAndOrientation(img, 3, 3);
        // Left-center pixel at (1,0) → index 1*3+0 = 3.
        assert(std::fabs(result.first[3] - 2.0f) < 1e-6f);
        assert(std::fabs(result.second[3]) < 1e-6f);
        // Right-center at (1,2) → index 1*3+2 = 5.
        assert(std::fabs(result.first[5] - 2.0f) < 1e-6f);
        assert(std::fabs(result.second[5] - 3.14159265f) < 1e-5f); // Gx negative → orientation π
        // Bottom-center at (2,1) → index 2*3+1 = 7.
        assert(std::fabs(result.first[7] - 2.0f) < 1e-6f);
        assert(std::fabs(result.second[7] - 3.14159265f/2.0f) < 1e-5f); // Gy negative → atan2(-2,0) = -π/2 → +π = π/2? Actually atan2(-2,0) = -π/2, +π = π/2. Wait, but expected magnitude 2, orientation should be π/2? No: atan2(gy,gx) with gy=-2,gx=0 gives -π/2, add π gives π/2. But the actual gradient direction points downward, so orientation is 3π/2? But we map to [0,π), so it's π/2. That's correct per our mapping.
    }
    
    // Test 5: 3x3 with linear ramp: I[y][x] = x + y.
    // For any interior pixel (1,1) = 2, Gx = 0.5*( (1+1) - (1+1) )? Wait: I[1,2] = 3, I[1,0] = 1 → Gx = 0.5*(3-1)=1. Gy = 0.5*(I[2,1]-I[0,1]) = 0.5*(3-1)=1. Magnitude = sqrt(2), orientation = atan2(1,1)=π/4.
    {
        std::vector<float> img = {
            0,1,2,
            1,2,3,
            2,3,4
        };
        auto result = gradientMagnitudeAndOrientation(img, 3, 3);
        // Interior pixel (1,1) index 4.
        float expected_mag = std::sqrt(2.0f);
        assert(std::fabs(result.first[4] - expected_mag) < 1e-6f);
        assert(std::fabs(result.second[4] - 3.14159265f/4.0f) < 1e-5f);
    }
    
    // Test 6: Boundary pixel of the ramp: top-left (0,0): Gx = I[0,1]-I[0,0] = 1-0=1; Gy = I[1,0]-I[0,0] = 1-0=1. Magnitude sqrt(2), orientation π/4.
    {
        std::vector<float> img = {
            0,1,2,
            1,2,3,
            2,3,4
        };
        auto result = gradientMagnitudeAndOrientation(img, 3, 3);
        assert(std::fabs(result.first[0] - std::sqrt(2.0f)) < 1e-6f);
        assert(std::fabs(result.second[0] - 3.14159265f/4.0f) < 1e-5f);
    }
    
    // Test 7: Ensure input is not modified (const correctness).
    {
        std::vector<float> img = {1,2,3,4};
        auto original = img;
        auto result = gradientMagnitudeAndOrientation(img, 2, 2);
        assert(img == original); // unchanged
    }
    
    return 0;
}
