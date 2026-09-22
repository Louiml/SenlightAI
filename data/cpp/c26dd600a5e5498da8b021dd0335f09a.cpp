Implement a C++ function `computeStratifiedTileSamples` that, given an integer pixel width `W`, height `H`, samples per pixel `S`, and a sub-tile split factor `T` (where `T` divides both `W` and `H`), partitions the image into `T*T` equal rectangular tiles. For each tile (in row-major order), generate `S` random 2D points within each pixel of that tile, but only for pixels whose coordinates satisfy `(x + y) % 2 == 0` (i.e., checkerboard pattern), and collect all generated points into a single output vector of `std::pair<float,float>`. The function must use a provided random number generator (RNG) that is a `std::mt19937` reference, and must guarantee that for every pixel processed, exactly `S` points are generated, each with `x` and `y` coordinates lying in the half-open interval `[pixelX, pixelX+1)` and `[pixelY, pixelY+1)` respectively. The order of points in the output must be: tile by tile (row-major), within a tile pixel by pixel (row-major), and within a pixel the `S` samples in the order they were generated. The function should return the vector of points, and must not modify the RNG state except through calls to `std::uniform_real_distribution`. Handle edge cases: if `W`, `H`, `S`, or `T` are non-positive, or if `T` does not divide `W` or `H`, the function should return an empty vector. The function signature is: `std::vector<std::pair<float,float>> computeStratifiedTileSamples(int W, int H, int S, int T, std::mt19937& rng);`

#include <cassert>
#include <vector>
#include <random>
#include <utility>
#include <cmath>

int main() {
    // Test 1: Basic 2x2 image, 1 spp, 1 tile (no subdivision).
    {
        std::mt19937 rng(42);
        auto pts = computeStratifiedTileSamples(2, 2, 1, 1, rng);
        // Checkerboard pixels: (0,0), (1,1) are even; (0,1),(1,0) are odd.
        // So we expect 2 points.
        assert(pts.size() == 2);
        // Each point must be within correct pixel bounds.
        assert(pts[0].first >= 0.0f && pts[0].first < 1.0f);
        assert(pts[0].second >= 0.0f && pts[0].second < 1.0f);
        assert(pts[1].first >= 1.0f && pts[1].first < 2.0f);
        assert(pts[1].second >= 1.0f && pts[1].second < 2.0f);
    }
    
    // Test 2: 4x4 image, 2 spp, 2x2 tiles (T=2).
    {
        std::mt19937 rng(7);
        auto pts = computeStratifiedTileSamples(4, 4, 2, 2, rng);
        // Total pixels = 16, half are checkerboard = 8, each with 2 samples = 16 points.
        assert(pts.size() == 16);
        // Check all points are within [0,4) range.
        for (const auto& p : pts) {
            assert(p.first >= 0.0f && p.first < 4.0f);
            assert(p.second >= 0.0f && p.second < 4.0f);
        }
    }
    
    // Test 3: Invalid inputs return empty.
    {
        std::mt19937 rng(1);
        assert(computeStratifiedTileSamples(0, 4, 1, 1, rng).empty());
        assert(computeStratifiedTileSamples(4, 0, 1, 1, rng).empty());
        assert(computeStratifiedTileSamples(4, 4, 0, 1, rng).empty());
        assert(computeStratifiedTileSamples(4, 4, 1, 0, rng).empty());
        assert(computeStratifiedTileSamples(3, 4, 1, 2, rng).empty()); // 3%2 !=0
        assert(computeStratifiedTileSamples(4, 3, 1, 2, rng).empty());
    }
    
    // Test 4: Single pixel 1x1, 5 spp, 1 tile.
    {
        std::mt19937 rng(99);
        auto pts = computeStratifiedTileSamples(1, 1, 5, 1, rng);
        // Only pixel (0,0) is even, so 5 points.
        assert(pts.size() == 5);
        for (const auto& p : pts) {
            assert(p.first >= 0.0f && p.first < 1.0f);
            assert(p.second >= 0.0f && p.second < 1.0f);
        }
    }
    
    // Test 5: Ensure tile ordering matches expected pattern for small case.
    {
        std::mt19937 rng(123);
        auto pts = computeStratifiedTileSamples(2, 2, 1, 2, rng);
        // T=2, tileW=1, tileH=1.
        // Tile (0,0): pixel (0,0) even -> sample from pixel (0,0)
        // Tile (0,1): pixel (0,1) odd -> no sample
        // Tile (1,0): pixel (1,0) odd -> no sample
        // Tile (1,1): pixel (1,1) even -> sample from pixel (1,1)
        assert(pts.size() == 2);
        // First point should be from pixel (0,0), second from (1,1).
        assert(pts[0].first >= 0.0f && pts[0].first < 1.0f);
        assert(pts[0].second >= 0.0f && pts[0].second < 1.0f);
        assert(pts[1].first >= 1.0f && pts[1].first < 2.0f);
        assert(pts[1].second >= 1.0f && pts[1].second < 2.0f);
    }
    
    // Test 6: Large image with odd dimensions but T divides them.
    {
        std::mt19937 rng(5);
        auto pts = computeStratifiedTileSamples(5, 5, 3, 5, rng);
        // T=5, tileW=1, tileH=1. Total checkerboard pixels: 13 (since 5x5 has 13 even-sum pixels)
        // Each has 3 samples = 39 points.
        assert(pts.size() == 39);
    }
    
    return 0;
}

#include <vector>
#include <random>
#include <utility>

// Generate stratified samples for checkerboard pixels in tiles.
// Returns empty vector if inputs invalid or T does not divide W and H.
std::vector<std::pair<float,float>> computeStratifiedTileSamples(
    int W, int H, int S, int T, std::mt19937& rng) {
    
    // Validate inputs.
    if (W <= 0 || H <= 0 || S <= 0 || T <= 0) return {};
    if (W % T != 0 || H % T != 0) return {};
    
    int tileW = W / T;
    int tileH = H / T;
    
    std::vector<std::pair<float,float>> result;
    result.reserve(static_cast<size_t>( (W * H) / 2 ) * S); // approximate
    
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);
    
    // Iterate tiles in row-major order.
    for (int ty = 0; ty < T; ++ty) {
        for (int tx = 0; tx < T; ++tx) {
            int x0 = tx * tileW;
            int x1 = x0 + tileW - 1;
            int y0 = ty * tileH;
            int y1 = y0 + tileH - 1;
            
            // Iterate pixels inside this tile.
            for (int y = y0; y <= y1; ++y) {
                for (int x = x0; x <= x1; ++x) {
                    // Checkerboard condition.
                    if ((x + y) % 2 == 0) {
                        // Generate S samples for this pixel.
                        for (int s = 0; s < S; ++s) {
                            float fx = static_cast<float>(x) + dist(rng);
                            float fy = static_cast<float>(y) + dist(rng);
                            result.emplace_back(fx, fy);
                        }
                    }
                }
            }
        }
    }
    
    return result;
}

// The solution needs to iterate over all tiles in row-major order. For each tile, compute its pixel bounds: `x0 = tileCol * (W/T)`, `x1 = (tileCol+1)*(W/T)-1`, similarly for y. Then iterate over every pixel in that tile using nested loops for `y` from `y0` to `y1`, `x` from `x0` to `x1`. For each pixel, check the checkerboard condition `(x + y) % 2 == 0`. If true, generate `S` samples using `std::uniform_real_distribution<float>` from `0.0` to `1.0` added to the integer pixel coordinates. We must ensure that the distribution is constructed once per pixel? Actually it's fine to construct it each time, but to be efficient, we can create it once outside the loops since it's stateless. However, each call to `operator()` consumes from the RNG. The output vector is appended in the correct order by simply pushing back as we iterate. Edge cases: validate all inputs; if any condition fails (non-positive or divisibility), return empty vector. Also, if `S` is 0, we could return empty vector (but the problem says non-positive, so S=0 is non-positive? Actually S=0 is non-positive? No, 0 is not positive. So treat S<=0 as invalid). Time complexity: we iterate over all pixels (W*H), but only half of them (checkerboard) generate S points each, so total points = (W*H/2)*S. So O(W*H*S) time. Space complexity is O(output size) = O(W*H*S) since we store all points.
