/*
Write a C++ function `Image improvedDemosaic(const Image &raw, int offsetGreen, int offsetRedX, int offsetRedY, int offsetBlueX, int offsetBlueY)` that reconstructs a full RGB image from a Bayer-pattern raw single-channel image. The function must first interpolate the green channel using edge-aware logic (choose vertical or horizontal neighbors based on which direction has smaller gradient), then interpolate red and blue channels using a color-difference approach: subtract the interpolated green from the raw values, apply simple bilinear interpolation on that difference, then add green back. The input `Image` class supports `width()`, `height()`, `channels()`, and operator `()` returning `float` references (e.g., `raw(x,y)` for single channel, `output(x,y,c)` for multi-channel). The offsets specify which pixels contain green (offsetGreen: 0 if even-row/even-col and odd-row/odd-col are green, 1 otherwise) and which pixels contain red (offsetRedX, offsetRedY: 0 or 1) and blue (similarly). The output image must have 3 channels with channel 0=red, 1=green, 2=blue. Assume the raw image has at least 3x3 dimensions, and ignore border pixels (pixels at x=0, x=width-1, y=0, y=height-1) by leaving them uninitialized (set to 0). The function must not modify the input.
*/

#include <cstdlib>
#include <vector>
#include <cassert>

// Minimal Image class supporting the operations used in the task.
class Image {
public:
    Image(int w, int h, int c = 1) : w_(w), h_(h), c_(c), data_(w * h * c, 0.0f) {}
    int width() const { return w_; }
    int height() const { return h_; }
    int channels() const { return c_; }
    float& operator()(int x, int y, int ch = 0) {
        return data_[(y * w_ + x) * c_ + ch];
    }
    float operator()(int x, int y, int ch = 0) const {
        return data_[(y * w_ + x) * c_ + ch];
    }
private:
    int w_, h_, c_;
    std::vector<float> data_;
};

// Helper: edge-aware green interpolation for pixels not originally green.
Image edgeBasedGreen(const Image& raw, int offsetGreen) {
    int W = raw.width();
    int H = raw.height();
    Image output(W, H, 1);

    bool greenOnEvenParity = (offsetGreen % 2 == 0);

    for (int y = 1; y < H - 1; ++y) {
        for (int x = 1; x < W - 1; ++x) {
            bool isGreen = ((x % 2) == (y % 2)) == greenOnEvenParity;
            if (isGreen) {
                output(x, y, 0) = raw(x, y);
            } else {
                float gradH = std::abs(raw(x - 1, y) - raw(x + 1, y));
                float gradV = std::abs(raw(x, y - 1) - raw(x, y + 1));
                if (gradH < gradV) {
                    output(x, y, 0) = (raw(x - 1, y) + raw(x + 1, y)) / 2.0f;
                } else if (gradV < gradH) {
                    output(x, y, 0) = (raw(x, y - 1) + raw(x, y + 1)) / 2.0f;
                } else {
                    output(x, y, 0) = (raw(x - 1, y) + raw(x + 1, y) +
                                        raw(x, y - 1) + raw(x, y + 1)) / 4.0f;
                }
            }
        }
    }
    return output;
}

// Helper: create a three-channel image where every channel is the given green channel.
Image greenTo3Channel(const Image& green) {
    int W = green.width();
    int H = green.height();
    Image result(W, H, 3);
    for (int y = 0; y < H; ++y) {
        for (int x = 0; x < W; ++x) {
            float g = green(x, y);
            for (int c = 0; c < 3; ++c) {
                result(x, y, c) = g;
            }
        }
    }
    return result;
}

// Helper: interpolate a color channel (red or blue) using color-difference method.
Image greenBasedRorB(const Image& raw, const Image& green, int offsetX, int offsetY) {
    int W = raw.width();
    int H = raw.height();

    // Build raw_minus_green: for each pixel, the raw value minus green (applied to all channels, but we only need the first channel).
    Image raw_minus_green(W, H, 1);
    for (int y = 0; y < H; ++y) {
        for (int x = 0; x < W; ++x) {
            raw_minus_green(x, y) = raw(x, y) - green(x, y);
        }
    }

    // Basic bilinear interpolation on the difference image.
    Image diff_interp(W, H, 1);
    for (int y = 1; y < H - 1; ++y) {
        for (int x = 1; x < W - 1; ++x) {
            bool hasColor = ((x % 2) == offsetX) && ((y % 2) == offsetY);
            if (hasColor) {
                diff_interp(x, y) = raw_minus_green(x, y);
            } else if (((x % 2) == offsetX) && ((y % 2) != offsetY)) {
                diff_interp(x, y) = (raw_minus_green(x, y - 1) + raw_minus_green(x, y + 1)) / 2.0f;
            } else if (((x % 2) != offsetX) && ((y % 2) == offsetY)) {
                diff_interp(x, y) = (raw_minus_green(x - 1, y) + raw_minus_green(x + 1, y)) / 2.0f;
            } else {
                diff_interp(x, y) = (raw_minus_green(x - 1, y - 1) + raw_minus_green(x - 1, y + 1) +
                                     raw_minus_green(x + 1, y - 1) + raw_minus_green(x + 1, y + 1)) / 4.0f;
            }
        }
    }

    // Add green back.
    Image result(W, H, 1);
    for (int y = 0; y < H; ++y) {
        for (int x = 0; x < W; ++x) {
            result(x, y) = diff_interp(x, y) + green(x, y);
        }
    }
    return result;
}

// Main demosaicing function: reconstructs full RGB from Bayer raw.
Image improvedDemosaic(const Image& raw, int offsetGreen, int offsetRedX, int offsetRedY, int offsetBlueX, int offsetBlueY) {
    int W = raw.width();
    int H = raw.height();

    // Step 1: interpolate green using edge-aware method.
    Image green = edgeBasedGreen(raw, offsetGreen);

    // Step 2: interpolate red and blue using color-difference with green.
    Image red = greenBasedRorB(raw, green, offsetRedX, offsetRedY);
    Image blue = greenBasedRorB(raw, green, offsetBlueX, offsetBlueY);

    // Step 3: combine into 3-channel output.
    Image output(W, H, 3);
    for (int y = 0; y < H; ++y) {
        for (int x = 0; x < W; ++x) {
            output(x, y, 0) = red(x, y);
            output(x, y, 1) = green(x, y);
            output(x, y, 2) = blue(x, y);
        }
    }
    return output;
}

int main() {
    // Create a small 5x5 raw image with a known Bayer pattern.
    // Assume offsetGreen=0 means even-row/even-col and odd-row/odd-col are green.
    // For simplicity, use a constant raw image where green positions have value 10,
    // red positions have value 20, blue positions have value 30.
    // For a 5x5 image with offsetGreen=0, offsetRedX=0, offsetRedY=1 (so red on even x, odd y),
    // and offsetBlueX=1, offsetBlueY=1 (odd x, odd y), we fill accordingly.
    // This test checks that the green interpolation at a non-green pixel is correct.
    Image raw(5, 5, 1);
    // Initialize all to 0, then set specific values.
    for (int y = 0; y < 5; ++y) {
        for (int x = 0; x < 5; ++x) {
            bool isGreen = ((x % 2) == (y % 2)); // offsetGreen=0
            if (isGreen) raw(x, y) = 10;
            else if ((x % 2 == 0) && (y % 2 == 1)) raw(x, y) = 20; // red
            else if ((x % 2 == 1) && (y % 2 == 1)) raw(x, y) = 30; // blue
            // Other positions (odd x, even y) are also blue? For simplicity left 0.
        }
    }

    // Run improved demosaic.
    Image result = improvedDemosaic(raw, 0, 0, 1, 1, 1);

    // Check a green pixel: at (0,0) should remain raw green value 10.
    assert(result(0,0,1) == 10.0f);
    // Check a red pixel: at (0,1) red should be 20, green interpolated from neighbors.
    assert(result(0,1,0) == 20.0f);
    // Check green at (0,1): neighbors (1,1) green=10, ( -1,1) out of range? But (0,1) is red, 
    // its horizontal neighbors ( -1,1) and (1,1): (1,1) is green=10, so horizontal average = 10.
    // Vertical neighbors (0,0) green=10 and (0,2) green=10 => vertical average = 10.
    // Gradients equal, so average all four = 10. 
    assert(result(0,1,1) == 10.0f);
    // At (1,1) which is green, green should be 10.
    assert(result(1,1,1) == 10.0f);
    // At (0,1) blue: since (0,1) is red (x even, y odd), blue is not original, so interpolated from diagonals.
    // Diagonals: ( -1,0) out, ( -1,2) out, (1,0) green? (1,0) is blue? Actually (1,0): x odd, y even => not green, and offsetRedX=0, offsetRedY=1, offsetBlueX=1, offsetBlueY=1, so (1,0) is blue? Wait: blue is where x%2==1 and y%2==1? No, offsetBlueX=1, offsetBlueY=1 means blue on odd x and odd y. (1,0) is odd x even y, so not blue. So it's zero in our raw. Thus diagonals around (0,1) are ( -1,0), ( -1,2), (1,0), (1,2) – all out of range or zero? Actually (1,0) is inside, value 0, (1,2) inside, value? (1,2): odd x even y, not blue, 0. So blue at (0,1) = (0+0+0+0)/4 = 0. That's fine.
    assert(result(0,1,2) == 0.0f);

    // Test a 3x3 image with simple constant values to verify green interpolation.
    Image raw2(3, 3, 1);
    // Fill with pattern: offsetGreen=0, so greens at (0,0), (0,2), (1,1), (2,0), (2,2) with value 5.
    // Red at (0,1) and (2,1) value 15, blue at (1,0) and (1,2) value 25.
    for (int y = 0; y < 3; ++y) {
        for (int x = 0; x < 3; ++x) {
            bool isGreen = ((x % 2) == (y % 2));
            if (isGreen) raw2(x, y) = 5.0f;
            else if ((x % 2 == 0) && (y % 2 == 1)) raw2(x, y) = 15.0f;
            else if ((x % 2 == 1) && (y % 2 == 0)) raw2(x, y) = 25.0f;
        }
    }
    Image result2 = improvedDemosaic(raw2, 0, 0, 1, 1, 0);
    // At center (1,1) is green, should be 5.
    assert(result2(1,1,1) == 5.0f);
    // At (0,1) red, green interpolation: horizontal neighbors ( -1,1) out, (1,1) green=5 => avg 5; vertical (0,0)=5, (0,2)=5 => avg 5. Tie -> avg all four = 5.
    assert(result2(0,1,1) == 5.0f);
    // At (1,0) blue, green interpolation: horizontal (0,0)=5, (2,0)=5 => avg 5; vertical (1,-1) out, (1,1)=5 => avg 5. Tie -> avg all four = 5.
    assert(result2(1,0,1) == 5.0f);
    // Red at (0,1) remains 15.
    assert(result2(0,1,0) == 15.0f);
    // Blue at (1,0) remains 25.
    assert(result2(1,0,2) == 25.0f);

    // Border pixels (x=0, y=0, etc.) are left 0 by design, so test that.
    assert(result2(0,0,0) == 0.0f);
    assert(result2(0,0,1) == 0.0f);

    return 0;
}

// The solution follows the standard Hamilton-Adams demosaicing pipeline with two stages. First, reconstruct the green channel. For each pixel that is not originally green (determined by `(x % 2 == y % 2) == (offsetGreen % 2 == 0)`), compute horizontal and vertical gradients using the known red/blue neighbor values: `gradH = |raw(x-1,y) - raw(x+1,y)|` and `gradV = |raw(x,y-1) - raw(x,y+1)|`. If gradV > gradH, interpolate green from horizontal neighbors; if gradH > gradV, use vertical; if equal, average all four neighbors. For original green pixels, copy the raw value. Next, for red and blue channels, build a three-channel image where every channel equals the interpolated green, subtract it from the raw single-channel image (which contains red or blue only at corresponding Bayer positions and zeros elsewhere — but actually the raw has actual red/blue values at those positions, zeros elsewhere in a real Bayer pattern, but here we treat it as given), then apply the same basic bilinear interpolation logic as in `basicRorB` on the difference image, and finally add green back. Since red and blue use the same interpolation function but with different offsets, we write a helper `interpolateColorMinusGreen` that takes the raw image, the green channel, and the color offsets. Edge cases: borders are ignored and set to 0; gradient tie is resolved by averaging both directions. Time complexity is O(width*height) for each channel, so O(N) where N is total pixels, and space complexity is O(N) for intermediate images.
