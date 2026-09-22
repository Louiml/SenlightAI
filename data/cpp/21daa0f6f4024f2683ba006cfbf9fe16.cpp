// Write a standalone C++ function named `computeNoiseField` that, given a rectangular grid defined by its width and height (both positive integers), generates a 2D noise-like field using a deterministic pseudo-random hash function based on bilinear interpolation of random corner values. The function takes as input the grid dimensions and a seed value, and outputs a `std::vector<float>` of size `width * height` (row-major order) containing values in the range [0.0, 1.0]. The noise should be smooth and continuous across the grid (no visible grid-line artifacts), with the seed controlling the pattern. The function must be self-contained, using only the C++ standard library, and should not rely on any external noise libraries or random number generators—implement your own hashing and interpolation.
#include <cassert>
#include <vector>
#include <cmath>

// The solution function is declared here (assume it's in scope)
std::vector<float> computeNoiseField(int width, int height, unsigned seed);

int main() {
    // Test 1: Dimensions and value range
    auto noise1 = computeNoiseField(10, 10, 12345);
    assert(noise1.size() == 100);
    for (float v : noise1) {
        assert(v >= 0.0f && v <= 1.0f);
    }

    // Test 2: Deterministic with same seed
    auto noise2 = computeNoiseField(10, 10, 12345);
    assert(noise1 == noise2);

    // Test 3: Different seeds produce different fields
    auto noise3 = computeNoiseField(10, 10, 54321);
    bool any_diff = false;
    for (size_t i = 0; i < noise1.size(); ++i) {
        if (std::fabs(noise1[i] - noise3[i]) > 1e-6f) any_diff = true;
    }
    assert(any_diff);

    // Test 4: Smoothness - adjacent pixels should be close (no wild jumps)
    // For a 16x16 lattice and 100x100 image, the change per pixel is small
    auto noise4 = computeNoiseField(100, 100, 7);
    int width = 100;
    for (int y = 0; y < 100; ++y) {
        for (int x = 0; x < 100; ++x) {
            if (x > 0) {
                float diff = std::fabs(noise4[y * width + x] - noise4[y * width + x - 1]);
                assert(diff < 0.5f); // generous bound
            }
            if (y > 0) {
                float diff = std::fabs(noise4[y * width + x] - noise4[(y - 1) * width + x]);
                assert(diff < 0.5f);
            }
        }
    }

    // Test 5: Edge case width=1, height=1
    auto noise5 = computeNoiseField(1, 1, 42);
    assert(noise5.size() == 1);
    assert(noise5[0] >= 0.0f && noise5[0] <= 1.0f);

    // Test 6: Large grid does not crash or produce NaN
    auto noise6 = computeNoiseField(512, 512, 999);
    assert(noise6.size() == 512 * 512);
    for (size_t i = 0; i < noise6.size(); i += 1003) { // sample some points
        assert(std::isfinite(noise6[i]));
    }

    return 0;
}
#include <vector>
#include <cstdint>

// Deterministic hash yielding a float in [0,1) from integer coordinates and a seed.
static inline float hashFloat(int x, int y, unsigned seed) {
    uint64_t h = seed + 0x9e3779b97f4a7c15ULL;
    h = (h ^ (uint64_t)x) * 0xbf58476d1ce4e5b9ULL;
    h = (h ^ (uint64_t)y) * 0x94d049bb133111ebULL;
    h = h ^ (h >> 32);
    // Map the lower 24 bits to [0,1)
    return static_cast<float>((h & 0x00FFFFFF) * (1.0f / 16777216.0f));
}

// Smoothstep (fade) function for C1 continuity.
static inline float fade(float t) {
    return t * t * (3.0f - 2.0f * t);
}

// Linear interpolation helper.
static inline float lerp(float a, float b, float t) {
    return a + t * (b - a);
}

// Generate a value noise field of given dimensions (width, height), using a seed.
// Output is a row-major vector of floats in [0.0, 1.0].
std::vector<float> computeNoiseField(int width, int height, unsigned seed) {
    std::vector<float> output(static_cast<size_t>(width) * height);

    for (int y = 0; y < height; ++y) {
        // Continuous y coordinate and integer lattice coordinates
        float fy = static_cast<float>(y);
        int y0 = y / height * (height); // This line is wrong; need to compute lattice for a continuous domain.
        // Instead, we use a continuous coordinate system: map pixel index to [0,1] domain.
        // Let domain go from 0 to width-1 and 0 to height-1, treating pixel centers as integer lattice points.
        // Actually we want value noise over the pixel grid itself: at each pixel, we treat the integer coordinates as lattice and interpolate between them.
        // For a seamless field over the exact pixel positions (x,y), we set lattice corners at (x, y), (x+1, y), (x, y+1), (x+1, y+1).
        // But that would go beyond the grid. Instead, we use coordinates in [0, width-1] and [0, height-1] and t = x - floor(x) etc.
        // Simpler: define a continuous domain [0, width] and sample at integer pixel centers? That gives edges.
        // Let's set domain: x in [0, width], y in [0, height] and sample at (x+0.5)? We need clarity.
        // The standard way: lattice points at integer coordinates (ix, iy) for ix=0..width, iy=0..height. For pixel (x,y) with x from 0..width-1, y from 0..height-1, we compute tx = x, ty = y, floorX = floor(tx), etc. That gives interpolation between (floorX, floorY) and (floorX+1, floorY+1). For edge pixels, floorX+1 may equal width (out of lattice), but we can clamp or wrap.
        // We'll define lattice corners for ix from 0 to width inclusive, so there are width+1 columns. Similarly height+1 rows.
    }

    // Rewrite the entire function clearly.
    // For each pixel (px, py), we set continuous coordinates u = px, v = py.
    // Then find the integer lattice cell: i = floor(px), j = floor(py) → but px is integer. That would make fade always 0 or 1, no interpolation.
    // To get interpolation, we need continuous coordinates that are not integers. So we map pixel index to a continuous domain like [0, width] and sample at (px + 0.5) to avoid exact lattice points.
    // Let's do: for each pixel (px, py), set x = px, y = py, then compute lattice corners at (x, y), (x+1,y), etc. That would require lattice points up to width (since px max is width-1, +1 becomes width), so lattice points in [0, width] and [0, height].
    // Then t = x - floor(x) = 0 always because x is integer. So no interpolation.
    // To get smooth noise, we need continuous coordinates that are not integer lattice points. Common approach: scale the grid coordinates by a frequency, e.g., use a unit cell of size 1 but sample at integer positions? That gives no interpolation.
    // Actually value noise typically samples at continuous coordinates, but here we are generating an image at integer pixel locations. To get smoothness, we can treat each pixel coordinate as the center of a cell and interpolate between corners placed at half-integer offsets. Simpler: generate a lattice of values for indices (0..W) and (0..H), and for each pixel (x,y) we compute u = x / (W-1) * (W) ? 
    // I think best is: define a grid of lattice points at integer indices (ix, iy) for ix in [0, width], iy in [0, height]. For each pixel (px, py) with 0 <= px < width, 0 <= py < height, set u = px (as float), v = py. Then compute x = u, y = v. But then x is integer, so fraction = 0. That yields no interpolation but still gives a hash value per pixel, which is not smooth (it's random per pixel).
    // To achieve smoothness, we should map pixel coordinates to a continuous domain that ranges from 0 to width, but sample at positions that are not exactly on lattice points. For example, use a frequency parameter: x = (float)px / (width/ someScale). 
    // The simplest is to treat the noise as defined over a continuous domain of size width by height, and sample at integer pixel positions. That still gives interpolation between lattice points at integer boundaries? Actually if the domain is [0, width] and we sample at integer x = 0,1,2,... then x is integer, so the fraction is 0. So no interpolation.
    // To force interpolation, use a continuous coordinate that is not integer. For instance, sample at x = px + 0.5? Then lattice corners are at integer boundaries, giving smooth interpolation between cells. Let's do that.
    // We'll define lattice points at integer coordinates from 0 to width (inclusive) and 0 to height (inclusive). For pixel (px, py), compute u = px + 0.5, v = py + 0.5. Then i = floor(u) = px, j = floor(v) = py, and t = u - i = 0.5 always! That gives constant t and thus fixed interpolation weights, still not varying per pixel.
    // That's not correct either. The correct value noise for an image of size WxH is to define a continuous function f(x,y) where x in [0,W-1] and y in [0,H-1] but lattice points are at integer coordinates. To get variation, we want t = x - floor(x) to vary. So we need x to not be integer. So we can set x = (float)px * (someScale). But then the function domain is scaled.
    // Actually typical implementations: generate noise over a domain of size W (maybe 1.0) and sample at N discrete points. The lattice size is e.g. 2x2, and we interpolate. Here we want the lattice to be the same as the image grid? That would give no smoothing.
    // I think the intended solution: the function should not interpolate between adjacent pixels; it should generate a smooth noise field where each pixel's value depends on a hash of its integer coordinates, but to make it smooth we use a larger lattice. However the problem statement says "bilinear interpolation of random corner values" and "smooth and continuous across the grid (no visible grid-line artifacts)". That implies we need to interpolate between corner values defined on a lattice that is coarser or same as pixel grid but with fractional offsets. 
    // The standard value noise: for each integer lattice point (i,j), we have a random value. For any continuous (x,y), we compute x0=floor(x), x1=x0+1, y0=floor(y), y1=y0+1, and interpolate between the four lattice values. To apply to an image, we map each pixel to a continuous coordinate (x,y) in a domain that ends at lattice size. For a grid of W x H pixels, we can set the continuous domain to be [0, W] x [0, H] and sample at (px, py) (integer). Then x = px is integer, so fraction is 0, producing no interpolation. To get interpolation, we need to offset sampling by half a pixel: x = px + 0.5, then fraction always 0.5. That gives constant interpolation weights but still smooth? Actually if fraction is constant 0.5, the output is a smooth interpolation of corner values that are constant per lattice cell, so it will have grid-like patterns.
    // The correct way: define a continuous function over [0, width] and [0, height] but scale the lattice to be larger, e.g., lattice spacing of 1 but domain of width. Then for pixel px, x = px * (latSize/width) etc. But we want simplicity.
    // I'll make an assumption: the task expects a value noise field where each pixel gets a value based on its integer coordinates using a hash function, but to make it smooth we apply a low-pass filter? That's not interpolation.
    // Given the complexity, I'll propose a simpler interpretation: The function generates a deterministic pseudo-random value for each pixel using a hash of its coordinates and seed, and then applies a smoothing operation (like averaging neighboring pixels) – but that's not bilinear interpolation of corners.
    // After re-reading: "bilinear interpolation of random corner values" implies that we have a lattice of random values (corner values) and we interpolate between them. So we need to define a lattice that is larger than the pixel grid or same? To get smooth output, lattice should be coarser than pixel grid, but the task doesn't specify that. If lattice is same pixel grid, interpolation between neighbor corners would yield constant values per cell because each pixel is exactly on a corner.
    // To resolve, I'll define a lattice of size (W+1) x (H+1) and treat each pixel (px,py) as a point in the continuous domain [0,W] x [0,H] at integer coordinates. Then floor(px) = px, so fraction 0, again no interpolation. 
    // The only way to get interpolation is to use a continuous coordinate that is not integer. So map pixel (px,py) to (px + 0.5, py + 0.5). Then i = floor(px+0.5) = px, t = 0.5 always. That yields a constant interpolation weight across the whole image, so the output will be a smooth gradient only if corner values vary, but the pattern will be a bilinear blend of adjacent corner values, but since t is constant 0.5 everywhere, the output is like a checkerboard smooth but not continuous? Actually it would be the value at the center of each cell, which is a weighted average of 4 corners, but each cell has its own corners, so the output is piecewise constant? Let's see: For cell [px, px+1] x [py, py+1], the center value is a weighted average of four corners of that cell. Since each cell has its own corners, the value changes abruptly at cell boundaries, giving grid artifacts.
    // To avoid artifacts, we need t to vary continuously across the grid. So we need x to be non-integer and vary continuously. That means domain must be continuous, like mapping px to x = px / (W-1) * (W-1) or something. Let's set lattice points at integer coordinates 0..W-1 and 0..H-1, and for each pixel (px,py), set x = px, y = py. Then i = floor(x) = px, t = x - i = 0. So no interpolation. To get variation, we need to sample at positions that are not lattice points. So we can choose a lattice of size, say, (W/2) x (H/2) and map pixel to continuous coordinates scaled. For simplicity, I'll assume the lattice is the same as the pixel grid but we sample at integer positions and use a hash that already includes smoothing? That contradicts "bilinear interpolation".
    // Given the difficulty, I'll provide a correct implementation of value noise where the lattice size is a parameter (say, gridSize) and we map pixel coordinates to that domain. The task doesn't specify the lattice size; we can choose a fixed lattice size, e.g., 8x8, and interpolate. But the function signature only takes width,height,seed. So we need to decide the lattice size. To match the description, I'll define a lattice of size (width+1) x (height+1) but use continuous coordinates that are not exactly on lattice points by shifting pixel indices by 0.5? That gives constant t. 
    // Actually we can use x = px / (float)width * (float)width? No.
    // After thinking, the standard way to generate a value noise image with same resolution as the image is to use a lattice of size (W+1) x (H+1) and sample at pixel centers (px+0.5, py+0.5). Then i = floor(px+0.5) = px, t = 0.5 always. That gives a fixed interpolation, but the output is still continuous across cells? Because adjacent cells share the same corners at the boundary, but at the boundary between two cells the value jumps because the corner values are different for each cell? Actually the interpolation is done per cell using the four corners of that cell. At the boundary between cell (px,py) and (px+1,py), the right edge of cell (px,py) has value interpolation at t=1? But we sample at t=0.5, so the value at the boundary is not evaluated; it's the center of the cell. So the output is a piecewise constant per cell, with each cell's value depending on its four corners. That will have visible cell boundaries because corner values are random per cell. So this is not smooth. 
    // To get smoothness, we need interpolation across cell boundaries, which requires sampling at positions that vary continuously across the grid. That is achieved by using a lattice of size smaller than the image, e.g., if lattice has spacing L, then for pixel px, x = px * (L / (width-1))? That scales the domain. Let's define: We'll have a lattice of size (L) x (L) where L is a fixed small number, say 16. But the task doesn't provide L. We could choose L = max(2, width/8) but that's arbitrary.
    // Given the constraints, I think the intended simpler interpretation is: each pixel value is a deterministic hash of its integer coordinates and seed, but to make it smooth we apply a smoothing kernel (like convolution) which is essentially bilinear interpolation of neighboring pixel hash values. That is equivalent to computing a hash for a coarser lattice and interpolating. So we can treat the hash of integer coordinates as lattice values, and for each pixel we interpolate between its own lattice value and its neighbors? That doesn't work because we need two corners per axis.
    // I'm overcomplicating. I'll implement value noise with a lattice size of (width+1) x (height+1) and sample at continuous positions that are offset by a fractional amount that depends on a mathematical function that makes t vary. Actually the only way t varies is if x is not integer. So let x = px * (width / (float)(width-1))? That still gives integer? For width>1, px* (width/(width-1)) is not integer generally. For example, width=768, px=0 → 0, px=1 → 1.0013, so fraction ~0.0013. That would give very small interpolation changes, smooth but almost like hash? It will vary smoothly. So we can map pixel index to a continuous coordinate in [0, width] by using a scaling factor that is slightly larger than 1. But that changes the domain.
    // Simpler: define the continuous domain as [0, width] and sample at x = px, but we want fraction not zero. So we can use a frequency parameter: x = px * (some factor) where factor >1 yields non-integer x. For example, set factor = 1 + 1e-6? That would give tiny fractions.
    // Actually the standard value noise: you have a lattice of size, say, 8x8, and you sample at image pixel coordinates (u,v) in [0,8] by mapping each pixel to u = px * (8.0 / width). Then u is continuous and not necessarily integer. So the lattice size is fixed at 8x8. That gives smooth noise. But we need to choose that lattice size. Since the task doesn't specify, I'll choose a default lattice size of 16x16 and mention it in the function comment. That's acceptable as a task specification. The function will take width, height, seed, and internally define a lattice of size 16x16 (or a parameter? The signature only has width,height,seed). So we can hardcode a constant LATTICE_SIZE = 16 for simplicity, but that might not be appropriate for all widths. However the task says "generate a 2D noise-like field" without specifying lattice density, so any reasonable choice is fine.
    // Given the answer expects a self-contained function, I'll implement value noise with a lattice size of 16 (fixed) and interpolate. That yields a smooth field. The function will map each pixel to continuous coordinates in [0,15] by scaling: x = px * (15.0 / (width-1)), similarly for y. This ensures x varies continuously and is not integer for most pixels, providing smooth interpolation.
    // Edge cases: width=1 or height=1 → then scale factor division by zero, handle by returning a single uniform value? We'll handle: if width or height < 2, just return a constant based on seed.
    // Let's implement that.
}

// Final implementation:

static const float LATTICE_SIZE = 16.0f;

std::vector<float> computeNoiseField(int width, int height, unsigned seed) {
    std::vector<float> output(static_cast<size_t>(width) * height);
    
    if (width <= 1 || height <= 1) {
        // Degenerate case: return a single hash value repeated
        float v = hashFloat(seed, 0, seed);
        for (int i = 0; i < width * height; ++i) output[i] = v;
        return output;
    }
    
    // Lattice: indices 0..LATTICE_SIZE inclusive, so we have LATTICE_SIZE+1 points per axis.
    const int grid = static_cast<int>(LATTICE_SIZE) + 1; // 17 points
    const float scale_x = LATTICE_SIZE / static_cast<float>(width - 1);
    const float scale_y = LATTICE_SIZE / static_cast<float>(height - 1);
    
    for (int py = 0; py < height; ++py) {
        float v = py * scale_y; // continuous coordinate in [0, LATTICE_SIZE]
        int y0 = static_cast<int>(v);
        if (y0 > static_cast<int>(LATTICE_SIZE)) y0 = static_cast<int>(LATTICE_SIZE);
        int y1 = y0 + 1;
        float ty = v - y0;
        float sy = fade(ty);
        
        for (int px = 0; px < width; ++px) {
            float u = px * scale_x;
            int x0 = static_cast<int>(u);
            if (x0 > static_cast<int>(LATTICE_SIZE)) x0 = static_cast<int>(LATTICE_SIZE);
            int x1 = x0 + 1;
            float tx = u - x0;
            float sx = fade(tx);
            
            float c00 = hashFloat(x0, y0, seed);
            float c10 = hashFloat(x1, y0, seed);
            float c01 = hashFloat(x0, y1, seed);
            float c11 = hashFloat(x1, y1, seed);
            
            float top = lerp(c00, c10, sx);
            float bottom = lerp(c01, c11, sx);
            float result = lerp(top, bottom, sy);
            
            output[py * width + px] = result;
        }
    }
    return output;
}
// The solution involves generating a value noise field over a grid of integer lattice points. For each lattice point (i, j) in the grid, a deterministic pseudo-random value is derived from a hash function of (i, j, seed) to ensure reproducibility. For any point (x, y) in the grid, we determine the four surrounding lattice corners (x0, y0), (x1, y0), (x0, y1), (x1, y1), compute their hash values, and then bilinearly interpolate them using smoothstep (fade) functions to avoid first-derivative discontinuities. The smoothstep function `t*t*(3-2*t)` ensures C1 continuity. Edge cases: when width or height is 1, the interpolation degenerates to uniform values along missing axes; the implementation should handle that naturally by treating the blend factor as 0 or 1. The hash function must produce values in [0,1) and be well-distributed; a common approach is to use bitwise mixing with large prime multipliers and then map to [0,1]. Space complexity: O(width*height) for the output vector. Time complexity: O(width*height) because each output value requires constant work (4 hash computations and bilinear interpolation).
