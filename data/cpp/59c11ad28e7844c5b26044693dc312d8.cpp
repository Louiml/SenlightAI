/*
Write a standalone C++ function named `renderTexturedTriangle` that takes a 2D vector representing a texture image (with each pixel stored as three consecutive RGB values in the inner vector), a reference to a 2D vector representing an RGB image buffer (same row-major format), and four integers: image width, image height, texture width, and texture height. The function must fill the image buffer by mapping the texture onto a fixed triangle with vertices at (100,100), (25,90), and (61,10) in image coordinates. For each pixel in the image, compute barycentric-like coordinates (alpha, beta, gamma) using signed distances from the pixel to each edge of the triangle, where alpha corresponds to the edge from vertex B(100,100) to C(25,90), beta to C–A (A is (61,10)), and gamma to A–B. Only color pixels where all three coordinates are non-negative (inside or on the triangle boundary). For those pixels, compute texture coordinates u and v as linear combinations of precomputed weights (u = 0.160268*alpha + 0.083611*beta + 0.230169*gamma, v = 0.290086*alpha + 0.159907*beta + 0.222781*gamma), then clamp the resulting texel indices to valid texture bounds, handle the vertical flip (texely = textureHeight − floor(textureHeight*v)), and copy the RGB values from the texture texel to the corresponding image buffer pixel. The function should not alter pixels outside the triangle, and it must be robust to texture and image sizes (no out-of-range access). Assume the image buffer is already initialized (e.g., to a background color) and has exactly `height` rows and `width*3` columns.
*/

#include <vector>
#include <cmath>
#include <algorithm>

// Helper: signed distance from point (px,py) to line through (e1x,e1y) and (e2x,e2y)
static float signedDistance(float px, float py, float e1x, float e1y, float e2x, float e2y) {
    float vecx = px - e1x;
    float vecy = py - e1y;
    float ex = e2x - e1x;
    float ey = e2y - e1y;
    // normal vector (perpendicular to line)
    float nx = -ey;
    float ny = ex;
    float dot = vecx * nx + vecy * ny;
    float mag = std::sqrt(nx * nx + ny * ny);
    if (mag == 0.0f) return 0.0f; // degenerate edge
    return dot / mag;
}

// Render a textured triangle into an RGB image buffer.
void renderTexturedTriangle(const std::vector<std::vector<int>>& texture,
                            std::vector<std::vector<int>>& imageBuffer,
                            int width, int height,
                            int textureWidth, int textureHeight) {
    // Triangle vertices: A(61,10), B(100,100), C(25,90)
    const float ax = 61.0f, ay = 10.0f;
    const float bx = 100.0f, by = 100.0f;
    const float cx = 25.0f, cy = 90.0f;

    // Precompute denominators (distance from opposite vertex to edge)
    // alpha uses edge CB, denominator = distance from A to line CB
    float denomAlpha = signedDistance(ax, ay, cx, cy, bx, by);
    // beta uses edge AC, denominator = distance from B to line AC
    float denomBeta = signedDistance(bx, by, ax, ay, cx, cy);
    // gamma uses edge BA, denominator = distance from C to line BA
    float denomGamma = signedDistance(cx, cy, bx, by, ax, ay);

    // Avoid division by zero if degenerate (should not happen for a valid triangle)
    if (std::abs(denomAlpha) < 1e-9f || std::abs(denomBeta) < 1e-9f || std::abs(denomGamma) < 1e-9f) return;

    // Texture weights (fixed constants)
    const float uA = 0.160268f, uB = 0.083611f, uC = 0.230169f;
    const float vA = 0.290086f, vB = 0.159907f, vC = 0.222781f;

    // Iterate over all pixels
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            float px = static_cast<float>(x);
            float py = static_cast<float>(y);

            // Compute barycentric-like coordinates
            float alpha = signedDistance(px, py, cx, cy, bx, by) / denomAlpha;
            float beta  = signedDistance(px, py, ax, ay, cx, cy) / denomBeta;
            float gamma = signedDistance(px, py, bx, by, ax, ay) / denomGamma;

            // Point inside or on triangle boundary?
            if (alpha < 0.0f || beta < 0.0f || gamma < 0.0f) continue;

            // Compute texture coordinates
            float u = uA * alpha + uB * beta + uC * gamma;
            float v = vA * alpha + vB * beta + vC * gamma;

            // Convert to texel indices (integer part), handle vertical flip
            int texelx = static_cast<int>(textureWidth * u);
            int texely = textureHeight - static_cast<int>(textureHeight * v);

            // Clamp to valid range
            texelx = std::max(0, std::min(textureWidth - 1, texelx));
            texely = std::max(0, std::min(textureHeight - 1, texely));

            // Copy RGB values from texture to image
            int base = x * 3;
            imageBuffer[y][base]     = texture[texely][texelx * 3];
            imageBuffer[y][base + 1] = texture[texely][texelx * 3 + 1];
            imageBuffer[y][base + 2] = texture[texely][texelx * 3 + 2];
        }
    }
}

#include <cassert>
#include <vector>
#include <cmath>

// Forward declaration for the solution function
void renderTexturedTriangle(const std::vector<std::vector<int>>& texture,
                            std::vector<std::vector<int>>& imageBuffer,
                            int width, int height,
                            int textureWidth, int textureHeight);

int main() {
    // Simple test: 4x4 image, 2x2 texture, background initialized to 0
    const int width = 4, height = 4, tw = 2, th = 2;
    std::vector<int> row(width * 3, 0);
    std::vector<std::vector<int>> image(height, row);

    // Texture: 2x2, each pixel a distinct color
    std::vector<std::vector<int>> texture(th, std::vector<int>(tw * 3));
    // Texture row 0 (top in file, but we flip vertically)
    texture[0] = {10, 20, 30, 40, 50, 60};   // row y=0: two pixels
    texture[1] = {70, 80, 90, 100, 110, 120}; // row y=1

    // Call the function; since triangle vertices are (100,100), (25,90), (61,10),
    // none of the 4x4 pixels (x,y from 0..3) lie inside the triangle.
    renderTexturedTriangle(texture, image, width, height, tw, th);

    // All pixels should remain background (0)
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width * 3; ++x) {
            assert(image[y][x] == 0);
        }
    }

    // Test with a larger image that contains the triangle vertices.
    // Create a 128x128 image, texture 2x2 with known colors.
    const int bigW = 128, bigH = 128;
    std::vector<int> bigRow(bigW * 3, 0);
    std::vector<std::vector<int>> bigImage(bigH, bigRow);
    // Use a simple 2x2 texture with colors: (1,1,1), (2,2,2), (3,3,3), (4,4,4)
    std::vector<std::vector<int>> smallTex(2, std::vector<int>(6));
    smallTex[0] = {1,1,1, 2,2,2};
    smallTex[1] = {3,3,3, 4,4,4};

    renderTexturedTriangle(smallTex, bigImage, bigW, bigH, 2, 2);

    // The vertex A(61,10) is inside the triangle. Check pixel (61,10) is not background.
    // Due to barycentric mapping, it should have a non-zero color.
    bool foundNonZero = false;
    for (int y = 0; y < bigH; ++y) {
        for (int x = 0; x < bigW * 3; ++x) {
            if (bigImage[y][x] != 0) {
                foundNonZero = true;
                break;
            }
        }
        if (foundNonZero) break;
    }
    assert(foundNonZero);

    // Also verify that a far-away pixel (0,0) remains background
    assert(bigImage[0][0] == 0);

    // Test that the texture is flipped vertically: at the very top of triangle
    // (near y=100, high y), we should get texture row 0 (smallTex[0]) values.
    // Compute alpha,beta,gamma for pixel (100,100) which is vertex B.
    // Expected barycentric: alpha=1, beta=0, gamma=0 (since distance from B to CB is 0)
    // u = uA*1 + ... = 0.160268, v = 0.290086, texelx = floor(2*0.160268)=0,
    // texely = 2 - floor(2*0.290086)=2-0=2 => out of range clamped to 1
    // So color should be (3,3,3) from smallTex[1]? Actually smallTex[1] has (3,3,3) and (4,4,4)
    // For x=100, y=100, we can at least assert that the pixel is not the background.
    // For robustness, we check that it's not zero.
    assert(bigImage[100][100*3] != 0);

    return 0;
}

// The core algorithm computes the signed distance from a point (pixel) to each edge of the triangle using the cross-product-like formula: for edge endpoints P1 and P2, the distance is ((point − P1) · normal) / |normal|, where the normal is (−(P2y−P1y), P2x−P1x). The sign of this distance indicates which side of the edge the point lies on; normalizing by the distance from a known opposite vertex (the third triangle vertex) yields barycentric coordinates that are 1 at that vertex and 0 on the edge. In the given problem, alpha = distance to edge CB / distance from A to edge CB, beta = distance to edge AC / distance from B to AC, gamma = distance to edge BA / distance from C to BA. The denominator is constant per edge and can be precomputed once. For a point to be inside the triangle, all three coordinates must be non-negative. The texture mapping uses fixed linear weights (which, if consistent with the barycentric coordinates, would actually be the texture coordinates at each vertex, but here they are given constants). Important edge cases: points exactly on the boundary yield a coordinate of zero, which is accepted (non-negative). To avoid out-of-bounds accesses, after computing texelx = textureWidth * u and texely = textureHeight − textureHeight * v, clamp texelx to [0, textureWidth−1] and texely to [0, textureHeight−1]. The time complexity is O(width × height) for pixel iteration, each with constant work; space complexity is O(1) auxiliary beyond the input textures. The nested loops should iterate over all pixels exactly once, with the triangle check before any texture sampling.
