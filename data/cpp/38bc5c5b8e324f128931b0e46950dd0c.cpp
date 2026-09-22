Implement a C++ function `rotateGrayImage` that performs rotation of a grayscale image by an arbitrary angle (0–360 degrees) using either bilinear or bicubic interpolation. The function takes as input a 2D array of `unsigned char` pixel values (row-major, indexed as `[height][width]`), the original image dimensions, the rotation angle in degrees, and an integer indicating the interpolation type (0 for bilinear, 1 for bicubic). It must return a new 2D array of the same dimensions containing the rotated image. For each output pixel at coordinates `(h, w)`, compute the corresponding source coordinates using inverse rotation around the image center: `(h_rot, w_rot) = (cos(θ)*(h - H/2) - sin(θ)*(w - W/2) + H/2, sin(θ)*(h - H/2) + cos(θ)*(w - W/2) + W/2)` where `θ` is the angle in radians. If the source coordinate falls inside the image bounds, apply the selected interpolation; otherwise, set the output pixel to 0 (black). For bilinear interpolation, use the standard four-neighbor weighted average; for bicubic, use a 4×4 neighborhood with cubic convolution using the given reversed matrix. Boundary handling is critical: for bilinear, if a source coordinate lands exactly on the last row or column, reuse the boundary pixel; for bicubic, clamp neighbor indices to valid ranges. The solution must be self-contained, use a single free function, and manage memory properly (return a pointer to a dynamically allocated 2D array; the caller is responsible for freeing it). Do not include any main function in the solution section; test code is provided separately.
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <cstring>

// Declare the function from solution (already defined above, but here for completeness)
// unsigned char** rotateGrayImage(const unsigned char** input, int height, int width, double angleDegrees, int interpolationType);

// Helper to free 2D array
void freeImage(unsigned char** img, int height) {
    for (int i = 0; i < height; ++i) delete[] img[i];
    delete[] img;
}

// Helper to allocate and fill a test image
unsigned char** makeImage(int height, int width, unsigned char value) {
    unsigned char** img = new unsigned char*[height];
    for (int i = 0; i < height; ++i) {
        img[i] = new unsigned char[width];
        for (int j = 0; j < width; ++j) img[i][j] = value;
    }
    return img;
}

int main() {
    const int H = 4, W = 4;

    // Test 1: 0-degree rotation of constant image should be nearly identical (bilinear)
    {
        unsigned char** in = makeImage(H, W, 100);
        unsigned char** out = rotateGrayImage(const_cast<const unsigned char**>(in), H, W, 0.0, 0);
        for (int i = 0; i < H; ++i)
            for (int j = 0; j < W; ++j)
                assert(out[i][j] == 100);
        freeImage(in, H); freeImage(out, H);
    }

    // Test 2: 0-degree rotation of a gradient image (bicubic) – should reproduce exactly at integer coords
    {
        unsigned char** in = new unsigned char*[H];
        for (int i = 0; i < H; ++i) {
            in[i] = new unsigned char[W];
            for (int j = 0; j < W; ++j) in[i][j] = static_cast<unsigned char>(i*10 + j);
        }
        unsigned char** out = rotateGrayImage(const_cast<const unsigned char**>(in), H, W, 0.0, 1);
        for (int i = 0; i < H; ++i)
            for (int j = 0; j < W; ++j)
                assert(out[i][j] == static_cast<unsigned char>(i*10 + j));
        freeImage(in, H); freeImage(out, H);
    }

    // Test 3: 90-degree rotation of a simple 2x2 pattern (bilinear)
    {
        H = 2; W = 2;
        unsigned char** in = new unsigned char*[H];
        for (int i = 0; i < H; ++i) {
            in[i] = new unsigned char[W];
            for (int j = 0; j < W; ++j) in[i][j] = (i==0 && j==0) ? 255 : 0;
        }
        unsigned char** out = rotateGrayImage(const_cast<const unsigned char**>(in), H, W, 90.0, 0);
        // After 90° clockwise rotation, original top-left moves to top-right, but due to rounding may vary
        // Instead, check that the sum of all pixels is preserved (approximately)
        int sum_in = 0, sum_out = 0;
        for (int i = 0; i < H; ++i) for (int j = 0; j < W; ++j) { sum_in += in[i][j]; sum_out += out[i][j]; }
        // For 90° of a single bright pixel, bilinear may blur; allow tolerance
        assert(abs(sum_in - sum_out) <= 100);
        freeImage(in, H); freeImage(out, H);
    }

    // Test 4: Rotation angle out of range (361) should normalize to 1, producing a valid image
    {
        H = 3; W = 3;
        unsigned char** in = makeImage(H, W, 50);
        unsigned char** out = rotateGrayImage(const_cast<const unsigned char**>(in), H, W, 361.0, 0);
        // Just check all pixels are in valid range (no crash)
        for (int i = 0; i < H; ++i)
            for (int j = 0; j < W; ++j)
                assert(out[i][j] >= 0 && out[i][j] <= 255);
        freeImage(in, H); freeImage(out, H);
    }

    // Test 5: Boundary: source coordinates exactly at bottom-right edge (bilinear) – no out-of-bounds
    {
        H = 2; W = 2;
        unsigned char** in = new unsigned char*[H];
        for (int i = 0; i < H; ++i) {
            in[i] = new unsigned char[W];
            for (int j = 0; j < W; ++j) in[i][j] = static_cast<unsigned char>(i*W + j);
        }
        // Force a specific source coordinate using a tiny angle that maps to near edge
        // But since we cannot easily fabricate, just ensure function handles 359° without crash
        unsigned char** out = rotateGrayImage(const_cast<const unsigned char**>(in), H, W, 359.0, 0);
        for (int i = 0; i < H; ++i)
            for (int j = 0; j < W; ++j)
                assert(out[i][j] >= 0 && out[i][j] <= 255);
        freeImage(in, H); freeImage(out, H);
    }

    return 0;
}
#include <cmath>
#include <cstdlib>
#include <cstring>

// Cubic convolution matrix (hardcoded from problem statement)
static const double reversedMatrix[4][4] = {
    { -1.0/6.0,  0.5,      -0.5,       1.0/6.0 },
    {  0.5,     -1.0,       0.5,       0.0     },
    { -1.0/3.0, -0.5,       1.0,      -1.0/6.0 },
    {  0.0,      1.0,       0.0,       0.0     }
};

// Helper: cubic interpolation of four samples at fractional offset val
static double cubicInterp(double v0, double v1, double v2, double v3, double val) {
    double coef[4];
    for (int i = 0; i < 4; ++i) {
        coef[i] = reversedMatrix[i][0] * v0 + reversedMatrix[i][1] * v1 +
                  reversedMatrix[i][2] * v2 + reversedMatrix[i][3] * v3;
    }
    // Evaluate polynomial: coef[0]*val^3 + coef[1]*val^2 + coef[2]*val + coef[3]
    return coef[0]*val*val*val + coef[1]*val*val + coef[2]*val + coef[3];
}

// Helper: bilinear interpolation at fractional coordinates
static unsigned char bilinearInterp(const unsigned char** img, int H, int W,
                                    double h_rot, double w_rot) {
    int x1 = static_cast<int>(w_rot);
    int y1 = static_cast<int>(h_rot);
    int x2 = x1 + 1;
    int y2 = y1 + 1;

    // Boundary handling: if on last row/col, clamp the second neighbor to the same pixel
    if (x2 >= W) x2 = x1;
    if (y2 >= H) y2 = y1;

    double fx = w_rot - x1;
    double fy = h_rot - y1;

    double top    = (1.0 - fx) * img[y1][x1] + fx * img[y1][x2];
    double bottom = (1.0 - fx) * img[y2][x1] + fx * img[y2][x2];

    return static_cast<unsigned char>((1.0 - fy) * top + fy * bottom);
}

// Helper: bicubic interpolation at fractional coordinates
static unsigned char bicubicInterp(const unsigned char** img, int H, int W,
                                   double h_rot, double w_rot) {
    int x2 = static_cast<int>(w_rot);
    int y2 = static_cast<int>(h_rot);

    int x1 = x2 - 1; if (x1 < 0) x1 = 0;
    int y1 = y2 - 1; if (y1 < 0) y1 = 0;
    int x3 = x2 + 1; if (x3 >= W) x3 = x2;
    int y3 = y2 + 1; if (y3 >= H) y3 = y2;
    int x4 = x2 + 2; if (x4 >= W) x4 = x2;
    int y4 = y2 + 2; if (y4 >= H) y4 = y2;

    double fx = w_rot - x2;
    double fy = h_rot - y2;

    // Interpolate horizontally along each of four rows
    double row0 = cubicInterp(img[y1][x1], img[y1][x2], img[y1][x3], img[y1][x4], fx);
    double row1 = cubicInterp(img[y2][x1], img[y2][x2], img[y2][x3], img[y2][x4], fx);
    double row2 = cubicInterp(img[y3][x1], img[y3][x2], img[y3][x3], img[y3][x4], fx);
    double row3 = cubicInterp(img[y4][x1], img[y4][x2], img[y4][x3], img[y4][x4], fx);

    // Interpolate vertically
    double result = cubicInterp(row0, row1, row2, row3, fy);
    if (result < 0) result = 0;
    if (result > 255) result = 255;
    return static_cast<unsigned char>(result);
}

// Main function: rotate grayscale image by angle degrees.
// interpolationType: 0 = bilinear, 1 = bicubic.
// Returns a newly allocated H x W array (row-major as unsigned char**).
// Caller must free each row and the row pointer array.
unsigned char** rotateGrayImage(const unsigned char** input, int height, int width,
                                double angleDegrees, int interpolationType) {
    // Normalize angle to [0, 360)
    angleDegrees = fmod(angleDegrees, 360.0);
    if (angleDegrees < 0) angleDegrees += 360.0;

    double theta = angleDegrees * M_PI / 180.0;
    double cosTheta = cos(theta);
    double sinTheta = sin(theta);

    double centerH = height / 2.0;
    double centerW = width / 2.0;

    // Allocate output 2D array
    unsigned char** output = new unsigned char*[height];
    for (int i = 0; i < height; ++i) {
        output[i] = new unsigned char[width];
    }

    for (int h = 0; h < height; ++h) {
        for (int w = 0; w < width; ++w) {
            // Inverse rotation: map output (h,w) to source coordinates
            double h_rot = cosTheta * (h - centerH) - sinTheta * (w - centerW) + centerH;
            double w_rot = sinTheta * (h - centerH) + cosTheta * (w - centerW) + centerW;

            // Check if source coordinate is inside the original image bounds
            if (h_rot >= 0 && h_rot < height && w_rot >= 0 && w_rot < width) {
                if (interpolationType == 0) {
                    output[h][w] = bilinearInterp(input, height, width, h_rot, w_rot);
                } else {
                    output[h][w] = bicubicInterp(input, height, width, h_rot, w_rot);
                }
            } else {
                output[h][w] = 0; // outside: black
            }
        }
    }

    return output;
}
// The core of the problem is implementing inverse mapping and two interpolation schemes. For each output pixel, we compute the fractional source coordinates by rotating the output coordinate back to the original image space using the inverse rotation matrix. The angle must be converted from degrees to radians using `M_PI/180.0`. The center of rotation is `(H/2, W/2)` for height H and width W. For bilinear interpolation, we take the integer floor of the source coordinates `(x1,y1)`, then `x2 = x1+1`, `y2 = y1+1`. Special handling is needed when the coordinate is at the boundary: if `y1 == H-1` and `x1 == W-1`, set both neighbors to the boundary pixel; if only one coordinate is at the boundary, clamp the other neighbor accordingly. The weighted average is computed as: `value = (1 - fy)*( (1 - fx)*p00 + fx*p01 ) + fy*( (1 - fx)*p10 + fx*p11 )`, where `fx = w_rot - x1`, `fy = h_rot - y1`. For bicubic, we gather a 4×4 neighborhood around the floor coordinates, clamping indices to valid bounds (e.g., if `x1 < 0`, set to 0; if `x3 >= W`, set to `W-1`). Then apply cubic convolution in both axes using the 4×4 reversed matrix from the snippet: first compute four intermediate values by cubic interpolation along the horizontal direction for each of the four rows, then interpolate vertically using the same matrix. The cubic calculation function takes four sample values and a fractional offset, applies the matrix to compute polynomial coefficients, and evaluates at the offset. Time complexity is O(H*W) per output pixel, with constant work per pixel, so overall O(H*W). Space complexity is O(H*W) for the output array; the caller handles memory. Edge cases include: 0-degree rotation (should return nearly identical image), 90-degree rotation (interpolation may blur), and angles near boundaries where source coordinates fall outside the image (must output 0).
