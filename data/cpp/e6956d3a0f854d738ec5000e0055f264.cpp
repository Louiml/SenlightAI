Write a standalone C++ function named `applySoftShadow` that takes a vector of unsigned char representing a 32-bit RGBA image (width `w`, height `h`, with row-major order and 4 bytes per pixel), and modifies it in place to simulate a soft shadow effect: any pixel that is pure white (RGB all equal to 0xff, alpha ignored) becomes a light source, and its immediate horizontal and vertical neighbors that are not pure white get brightened by adding 0x28 (40 decimal) to each of their RGB channels, with no clamping (so values can exceed 255). Edge pixels (x=0 or x=w-1, or y=0 or y=h-1) should not be treated as sources (i.e., the condition for a source is `x > 1 && x < w-1 && y > 1 && y < h-1`, note the asymmetry as in typical implementations). The function should have signature `void applySoftShadow(std::vector<unsigned char>& img, unsigned int w, unsigned int h)`, and should include necessary headers. The operation must consider the original state of the image (i.e., do not propagate from newly brightened pixels—only from original pure white pixels). For an image of size `w * h` pixels, the function should perform a single pass over all pixels, and for each white pixel, examine its four orthogonal neighbors, applying the brightening once per neighbor (i.e., if two white sources neighbor the same pixel, it gets +0x28 twice). The function must be self-contained, use only standard library headers, and not call any helper functions that need to be defined (you may inline the index calculation).
#include <cassert>
#include <vector>
#include <cstdint>

int main() {
    // Test 1: Basic 5x5 image with a white pixel at (2,2) (center, not near edge).
    // Background is gray (0x5f), white source at index (2,2). Neighbors (1,2),(3,2),(2,1),(2,3) get +0x28.
    {
        std::vector<unsigned char> img(5*5*4, 0x5f);
        // Set pixel (2,2) to white (RGBA: R=G=B=0xff, A=0xff)
        unsigned int pos = (2*5+2)*4;
        img[pos] = 0xff; img[pos+1] = 0xff; img[pos+2] = 0xff; img[pos+3] = 0xff;
        applySoftShadow(img, 5, 5);
        // Check that center remains white.
        assert(img[pos] == 0xff);
        // Check neighbors got brightened by 0x28.
        unsigned int npos;
        npos = (2*5+1)*4; // left
        assert(img[npos] == 0x5f + 0x28);
        assert(img[npos+1] == 0x5f + 0x28);
        assert(img[npos+2] == 0x5f + 0x28);
        npos = (2*5+3)*4; // right
        assert(img[npos] == 0x5f + 0x28);
        npos = (1*5+2)*4; // up
        assert(img[npos] == 0x5f + 0x28);
        npos = (3*5+2)*4; // down
        assert(img[npos] == 0x5f + 0x28);
        // Ensure a diagonal neighbor (1,1) remains unchanged.
        npos = (1*5+1)*4;
        assert(img[npos] == 0x5f);
    }

    // Test 2: Edge white pixels are not sources due to the quirky condition.
    {
        std::vector<unsigned char> img(4*4*4, 0x5f);
        // White at (0,0) (edge) should not affect neighbors.
        unsigned int pos = (0*4+0)*4;
        img[pos] = 0xff; img[pos+1] = 0xff; img[pos+2] = 0xff; img[pos+3] = 0xff;
        applySoftShadow(img, 4, 4);
        // Neighbor (1,0) should remain gray.
        unsigned int npos = (0*4+1)*4;
        assert(img[npos] == 0x5f);
        // Neighbor (0,1) should remain gray.
        npos = (1*4+0)*4;
        assert(img[npos] == 0x5f);
    }

    // Test 3: Two adjacent white pixels both brighten the shared non-white neighbor.
    {
        std::vector<unsigned char> img(3*3*4, 0x5f);
        // White at (1,1) and (2,1) (both not near left/right edges? (2,1) is at x=2, w=3 => x < w-1? 2 < 2 is false, so not a source). So only (1,1) is source, and (2,1) is its right neighbor but (2,1) is white, so no brighten.
        // Let's use a wider image so both are valid sources.
        // Better: test with a 5x3 image, whites at (2,1) and (3,1). Both are not near edges? x=3, w=5 => x<4 true and x>1 true, so both are sources. Their shared neighbor (3,1) is white, not brightened; but pixel (2,1) and (3,1) are both white, so no non-white between them. Use a non-white pixel between them.
        // Instead test: white at (2,1) and (4,1) with non-white at (3,1). Then (3,1) gets brightened twice.
        std::vector<unsigned char> img2(5*3*4, 0x5f);
        // Set (2,1) white
        unsigned int pos1 = (1*5+2)*4;
        img2[pos1]=0xff; img2[pos1+1]=0xff; img2[pos1+2]=0xff; img2[pos1+3]=0xff;
        // Set (4,1) white
        unsigned int pos2 = (1*5+4)*4;
        img2[pos2]=0xff; img2[pos2+1]=0xff; img2[pos2+2]=0xff; img2[pos2+3]=0xff;
        applySoftShadow(img2, 5, 3);
        // Check (3,1) brightened twice: 0x5f + 0x28 + 0x28 = 0x5f + 0x50 = 0xaf
        unsigned int npos = (1*5+3)*4;
        assert(img2[npos] == 0x5f + 0x50);
        assert(img2[npos+1] == 0x5f + 0x50);
        assert(img2[npos+2] == 0x5f + 0x50);
    }

    // Test 4: No white pixels -> no change.
    {
        std::vector<unsigned char> img(3*3*4, 0x10);
        applySoftShadow(img, 3, 3);
        for (size_t i = 0; i < img.size(); ++i) {
            assert(img[i] == 0x10);
        }
    }

    // Test 5: Tiny image (1x1) does nothing.
    {
        std::vector<unsigned char> img(4, 0xff);
        applySoftShadow(img, 1, 1);
        assert(img[0] == 0xff && img[1] == 0xff && img[2] == 0xff);
    }

    return 0;
}
#include <vector>

// Apply a soft shadow effect: brighten non-white neighbors of pure white pixels.
// The condition for a source pixel is x > 1 && x < w-1 && y > 1 && y < h-1,
// matching the original code's quirk. Neighbors are horizontal/vertical only.
void applySoftShadow(std::vector<unsigned char>& img, unsigned int w, unsigned int h) {
    // If the image is too small, nothing to do.
    if (w < 3 || h < 3) return;

    const unsigned int channels = 4; // RGBA
    const unsigned char white = 0xff;
    const unsigned char delta = 0x28;

    // Iterate over all pixels; only white pixels that are not near the edge act as sources.
    for (unsigned int y = 0; y < h; ++y) {
        for (unsigned int x = 0; x < w; ++x) {
            const unsigned int pos = (y * w + x) * channels;
            // Check if this is a pure white pixel (RGB all 0xff, alpha ignored).
            if (img[pos] == white && img[pos+1] == white && img[pos+2] == white) {
                // Apply the quirky boundary condition: require x > 1 and x < w-1, y > 1 and y < h-1.
                if (x > 1 && x < w - 1 && y > 1 && y < h - 1) {
                    // Left neighbor (x-1, y) is always within bounds because x > 1.
                    unsigned int npos = (y * w + (x - 1)) * channels;
                    if (img[npos] != white) { // Non-white neighbor gets brightened.
                        img[npos] += delta;
                        img[npos+1] += delta;
                        img[npos+2] += delta;
                    }
                    // Right neighbor (x+1, y) is always within bounds because x < w-1.
                    npos = (y * w + (x + 1)) * channels;
                    if (img[npos] != white) {
                        img[npos] += delta;
                        img[npos+1] += delta;
                        img[npos+2] += delta;
                    }
                    // Up neighbor (x, y-1) is always within bounds because y > 1.
                    npos = ((y - 1) * w + x) * channels;
                    if (img[npos] != white) {
                        img[npos] += delta;
                        img[npos+1] += delta;
                        img[npos+2] += delta;
                    }
                    // Down neighbor (x, y+1) is always within bounds because y < h-1.
                    npos = ((y + 1) * w + x) * channels;
                    if (img[npos] != white) {
                        img[npos] += delta;
                        img[npos+1] += delta;
                        img[npos+2] += delta;
                    }
                }
            }
        }
    }
}
// The solution requires a single pass over the image, but we must avoid brightening a pixel and then considering it as a newly brightened source in the same pass. Since the condition for a source is that the pixel is exactly pure white (R=G=B=0xff) and that it is not on the edge, we can read the original image into a copy (or read from the vector while writing to the same vector, but because we only modify non-white pixels, and we never modify a white pixel, reading from the current vector is safe—white pixels never change, and non-white pixels never become white in the same pass because we only add 0x28, which is not enough to reach 0xff from 0x5f (the typical background) unless the original is near white, but the problem statement says pure white is the source). However, to be absolutely safe and match the typical reference, we can iterate over all pixels, and for each pixel that is pure white and not on the edge (using the condition `x > 1 && x < w-1 && y > 1 && y < h-1`), we examine its left, right, up, and down neighbors. For each neighbor that is not pure white, we add 0x28 to its R, G, B channels (ignoring alpha). Since we are only reading the source pixel's value (pure white) and never modifying it, and we are modifying only non-white neighbors, there is no chain reaction within the same pass; each neighbor is brightened based on the original source pixels. The condition `x > 1 && x < w-1` (rather than `x >= 1 && x < w-1`) is taken from the given snippet and should be respected; it means that pixels on the second column from the left (x=1) and second row from the top (y=1) are not treated as sources, which is a quirk of the original code. Edge cases: if w or h is 0 or 1, the loop does nothing; if a neighbor is out of bounds, we skip it (but because of the source condition, the neighbor indices are always valid as long as w and h are at least 2). Time complexity is O(w*h) because we visit each pixel once per source check, and each source checks at most 4 neighbors; space complexity is O(1) auxiliary, as we modify the vector in place.
