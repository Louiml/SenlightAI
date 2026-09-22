// Write a standalone C++ function `inpaintHoles` that takes a grayscale image represented as a 1D `std::vector<float>` of size `width * height`, a corresponding binary validity mask (1 = valid pixel, 0 = hole to inpaint), and an integer `radius`. The function must iteratively fill all holes using a patch-based inpainting approach: at each iteration, select the boundary hole pixel with the highest priority (computed as product of a confidence term and a data term), search within a limited neighborhood of size `search` (fixed at 10 in this task) for the best-matching fully valid source patch of size `(2*radius+1)x(2*radius+1)` using Sum of Absolute Differences (SAD) over valid pixels of the target patch, then copy the source patch colors into the target hole pixels, updating validity. Continue until no holes remain or a maximum of 1000 iterations is reached. The function must return a new vector with inpainted values; original valid pixels remain unchanged. Holes on the image border (within `radius` pixels of edge) need not be filled; treat them as permanently invalid. Implement helper functions for computing confidence, priority, gradient, normal, and patch matching. Use `const` where appropriate, and assume the input is non-empty and `width`/`height` are positive. The function signature: `std::vector<float> inpaintHoles(const std::vector<float>& image, const std::vector<bool>& validMask, int width, int height, int radius);`.

The solution is inspired by the Criminisi et al. exemplar-based inpainting algorithm but simplified for grayscale and without texture synthesis. We maintain a copy `out` of the input image and a working valid mask `valid` (where 1 means known). Initially, holes are pixels where `validMask` is false. We iterate: (1) Compute a confidence map `conf` initialized to 1 for valid pixels and 0 for holes. Use an integral image of confidence to compute the average confidence over a `(2*radius+1)` window centered at each boundary pixel (a hole adjacent to at least one valid pixel, and not on the image border within `radius`). (2) Compute gradient magnitude and normal direction for boundary pixels. For each boundary hole pixel, priority = confidence * dataTerm, where dataTerm = |gradient · normal| + small epsilon (0.001) to avoid zero. (3) Select the boundary pixel with highest priority. (4) Search for a source patch: iterate over all valid pixels whose entire surrounding patch is fully valid (precomputed boolean `sourceAvailable`), but restrict to a bounding box of size `(2*search+1)` centered on target pixel (with `search=10`). For each candidate source, compute SAD between source patch and target patch only over pixels that are valid in the target patch (including the target hole itself, but since hole pixels have no color, compare only over valid pixels of target). If no valid pixels exist in the target patch (shouldn't happen because boundary pixel has at least one valid neighbor), skip. Choose the source patch with minimum average SAD. (5) Copy colors from source patch to all hole pixels in the target patch (within image bounds). Update valid mask and confidence for those pixels to the confidence of the target center. (6) Recompute sourceAvailable and gradients before next iteration. Repeat until no boundary holes remain or max iterations reached. Edge cases: holes near borders that cannot be fully covered by a patch (because patch extends beyond image) remain unfilled—we simply skip them during boundary detection. Complexity: Each iteration processes O(height*width) pixels for priority computation and O((2*search+1)^2 * (2*radius+1)^2) for searching per target, and with up to 1000 iterations this is O(1000 * HW * search^2 * radius^2), which is acceptable for small test cases (e.g., 20x20 images with radius=1, search=10). Space is O(HW) for auxiliary arrays.

#include <vector>
#include <cmath>
#include <algorithm>
#include <limits>
#include <cassert>
#include <iostream>

// Helper to compute integral image of a 2D array (represented as 1D, row-major)
static std::vector<float> computeIntegral(const std::vector<float>& data, int width, int height) {
    std::vector<float> integral((width+1)*(height+1), 0.0f);
    for (int y = 0; y < height; ++y) {
        float rowSum = 0.0f;
        for (int x = 0; x < width; ++x) {
            rowSum += data[y*width + x];
            int idx = (y+1)*(width+1) + (x+1);
            integral[idx] = integral[y*(width+1)+(x+1)] + rowSum;
        }
    }
    return integral;
}

// Get average of data over window [x-radius, x+radius] x [y-radius, y+radius] using integral image
static float getWindowAverage(const std::vector<float>& integral, int width, int height, int x, int y, int radius) {
    int x0 = std::max(0, x - radius);
    int y0 = std::max(0, y - radius);
    int x1 = std::min(width-1, x + radius);
    int y1 = std::min(height-1, y + radius);
    int total = (x1-x0+1)*(y1-y0+1);
    if (total == 0) return 0.0f;
    int stride = width + 1;
    float sum = integral[(y1+1)*stride + (x1+1)]
              - integral[(y0)*stride + (x1+1)]
              - integral[(y1+1)*stride + (x0)]
              + integral[(y0)*stride + (x0)];
    return sum / static_cast<float>(total);
}

// Compute gradient magnitude and direction at a valid pixel (using finite differences)
static void computeGradient(const std::vector<float>& image, const std::vector<bool>& valid, int width, int height, 
                            int x, int y, float& gx, float& gy) {
    int xl = std::max(0, x-1);
    int xr = std::min(width-1, x+1);
    int yu = std::max(0, y-1);
    int yd = std::min(height-1, y+1);
    float left = valid[y*width+xl] ? image[y*width+xl] : image[y*width+x];
    float right = valid[y*width+xr] ? image[y*width+xr] : image[y*width+x];
    float up = valid[yu*width+x] ? image[yu*width+x] : image[y*width+x];
    float down = valid[yd*width+x] ? image[yd*width+x] : image[y*width+x];
    gx = (left - right) / 255.0f;
    gy = (up - down) / 255.0f;
}

// Compute normal direction (pointing from hole toward valid region) at boundary pixel
static void computeNormal(const std::vector<bool>& valid, int width, int height, int x, int y, float& nx, float& ny) {
    int xl = std::max(0, x-1);
    int xr = std::min(width-1, x+1);
    int yu = std::max(0, y-1);
    int yd = std::min(height-1, y+1);
    nx = -static_cast<float>(valid[y*width+xl] ? 1 : 0) + static_cast<float>(valid[y*width+xr] ? 1 : 0);
    ny = -static_cast<float>(valid[yu*width+x] ? 1 : 0) + static_cast<float>(valid[yd*width+x] ? 1 : 0);
    float len = std::sqrt(nx*nx + ny*ny);
    if (len > 0) {
        nx /= len;
        ny /= len;
    }
}

// Main inpainting function
std::vector<float> inpaintHoles(const std::vector<float>& image, const std::vector<bool>& validMask, int width, int height, int radius) {
    assert(image.size() == static_cast<size_t>(width*height));
    assert(validMask.size() == static_cast<size_t>(width*height));
    assert(width > 0 && height > 0 && radius >= 1);

    const int total = width * height;
    std::vector<float> out(image.begin(), image.end());
    std::vector<bool> valid(validMask.begin(), validMask.end());

    // Quick check: if no holes, return original
    bool hasHoles = false;
    for (int i = 0; i < total; ++i) {
        if (!valid[i]) { hasHoles = true; break; }
    }
    if (!hasHoles) return out;

    const int maxIter = 1000;
    const int search = 10;
    const float nan = std::numeric_limits<float>::quiet_NaN();

    // Confidence map: 1 for valid, 0 for holes
    std::vector<float> confidence(total, 0.0f);
    for (int i = 0; i < total; ++i) confidence[i] = valid[i] ? 1.0f : 0.0f;

    // Precompute sourceAvailability: a pixel is a source if its entire patch is valid and within bounds
    auto isSource = [&](int x, int y) -> bool {
        if (x < radius || x >= width-radius || y < radius || y >= height-radius) return false;
        for (int dy = -radius; dy <= radius; ++dy) {
            for (int dx = -radius; dx <= radius; ++dx) {
                if (!valid[(y+dy)*width + (x+dx)]) return false;
            }
        }
        return true;
    };

    // Initial source mask
    std::vector<bool> sourceMask(total, false);
    for (int y = radius; y < height-radius; ++y) {
        for (int x = radius; x < width-radius; ++x) {
            sourceMask[y*width+x] = isSource(x, y);
        }
    }

    int iteration = 0;
    while (iteration < maxIter) {
        // Compute integral image of confidence
        std::vector<float> integral = computeIntegral(confidence, width, height);

        // Find boundary holes (not valid, but adjacent to at least one valid, and not too close to border)
        float bestPriority = -1.0f;
        int bestIdx = -1;
        float bestConfidence = 0.0f;

        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                int idx = y*width + x;
                if (valid[idx]) continue;
                // skip if patch would be partially out of bounds (cannot be fully filled)
                if (x < radius || x >= width-radius || y < radius || y >= height-radius) continue;
                // check if it's boundary: has at least one valid 4-neighbor
                bool hasValidNeighbor = false;
                if (x > 0 && valid[idx-1]) hasValidNeighbor = true;
                if (x < width-1 && valid[idx+1]) hasValidNeighbor = true;
                if (y > 0 && valid[idx-width]) hasValidNeighbor = true;
                if (y < height-1 && valid[idx+width]) hasValidNeighbor = true;
                if (!hasValidNeighbor) continue;

                // confidence term: average confidence over window
                float conf = getWindowAverage(integral, width, height, x, y, radius);
                if (conf <= 0.0f) continue; // avoid zero confidence

                // data term: gradient dot normal
                float gx, gy;
                // Use nearest valid neighbor's gradient as approximation; simpler: compute gradient using valid neighbors only
                // Here we compute gradient at the hole using valid neighbors; if none exist, gradient is zero
                int xl = std::max(0, x-1);
                int xr = std::min(width-1, x+1);
                int yu = std::max(0, y-1);
                int yd = std::min(height-1, y+1);
                // find a valid neighbor to compute gradient (use the first valid one found)
                bool found = false;
                float val_left = 0.0f, val_right = 0.0f, val_up = 0.0f, val_down = 0.0f;
                if (x>0 && valid[idx-1]) { val_left = out[idx-1]; found = true; }
                if (x<width-1 && valid[idx+1]) { val_right = out[idx+1]; found = true; }
                if (y>0 && valid[idx-width]) { val_up = out[idx-width]; found = true; }
                if (y<height-1 && valid[idx+width]) { val_down = out[idx+width]; found = true; }
                if (!found) continue; // shouldn't happen because boundary has valid neighbor
                // Use only available neighbors for gradient (if a side missing, use the pixel's value as default? We'll use 0)
                gx = (val_left - val_right) / 255.0f;
                gy = (val_up - val_down) / 255.0f;
                // normal
                float nx, ny;
                computeNormal(valid, width, height, x, y, nx, ny);
                float data = std::abs(gx*nx + gy*ny);
                if (data < 0.001f) data = 0.001f;
                float priority = conf * data;
                if (priority > bestPriority) {
                    bestPriority = priority;
                    bestIdx = idx;
                    bestConfidence = conf;
                }
            }
        }

        if (bestIdx < 0) break; // no boundary holes remain

        int bx = bestIdx % width;
        int by = bestIdx / width;

        // Search for best source patch within search window around target
        float bestDiff = std::numeric_limits<float>::max();
        int bestSourceIdx = -1;
        int sxStart = std::max(radius, bx - search);
        int sxEnd = std::min(width-1-radius, bx + search);
        int syStart = std::max(radius, by - search);
        int syEnd = std::min(height-1-radius, by + search);

        for (int sy = syStart; sy <= syEnd; ++sy) {
            for (int sx = sxStart; sx <= sxEnd; ++sx) {
                int sidx = sy*width + sx;
                if (!sourceMask[sidx]) continue;
                // Compute SAD over valid pixels of target patch
                float diff = 0.0f;
                int pixels = 0;
                for (int dy = -radius; dy <= radius; ++dy) {
                    int tgy = by + dy;
                    int sgy = sy + dy;
                    if (tgy < 0 || tgy >= height || sgy < 0 || sgy >= height) continue;
                    for (int dx = -radius; dx <= radius; ++dx) {
                        int tgx = bx + dx;
                        int sgx = sx + dx;
                        if (tgx < 0 || tgx >= width || sgx < 0 || sgx >= width) continue;
                        int tindex = tgy*width + tgx;
                        if (!valid[tindex]) continue; // only compare over valid target pixels
                        int sindex = sgy*width + sgx;
                        diff += std::abs(out[sindex] - out[tindex]);
                        pixels++;
                    }
                }
                if (pixels == 0) continue;
                diff /= static_cast<float>(pixels);
                if (diff < bestDiff) {
                    bestDiff = diff;
                    bestSourceIdx = sidx;
                }
            }
        }

        if (bestSourceIdx < 0) {
            // No source found; mark this hole as unfillable (set valid to false but break to avoid infinite loop)
            // To avoid infinite loop, mark as permanently invalid by setting a flag? Simplest: set valid to true with original value and break
            // But better: just break out of loop (will exit while)
            break;
        }

        int sxx = bestSourceIdx % width;
        int syy = bestSourceIdx / width;

        // Copy source patch into target holes
        bool anyUpdated = false;
        for (int dy = -radius; dy <= radius; ++dy) {
            int tgy = by + dy;
            int sgy = syy + dy;
            if (tgy < 0 || tgy >= height || sgy < 0 || sgy >= height) continue;
            for (int dx = -radius; dx <= radius; ++dx) {
                int tgx = bx + dx;
                int sgx = sxx + dx;
                if (tgx < 0 || tgx >= width || sgx < 0 || sgx >= width) continue;
                int tindex = tgy*width + tgx;
                if (!valid[tindex]) {
                    int sindex = sgy*width + sgx;
                    out[tindex] = out[sindex];
                    valid[tindex] = true;
                    confidence[tindex] = bestConfidence;
                    anyUpdated = true;
                }
            }
        }

        if (!anyUpdated) break; // shouldn't happen but safety

        // Recompute source mask (only needed if we want to update, but we can recompute fully)
        for (int y = radius; y < height-radius; ++y) {
            for (int x = radius; x < width-radius; ++x) {
                int idx = y*width + x;
                sourceMask[idx] = isSource(x, y);
            }
        }

        iteration++;
    }

    // Return the inpainted image; holes near border (within radius) may remain unfilled and are left with original NaN? 
    // Since input is float but we assume holes contain arbitrary values (e.g., 0). We'll leave them as original.
    return out;
}

#include <cassert>
#include <vector>
#include <cmath>

// Function under test is declared above
std::vector<float> inpaintHoles(const std::vector<float>& image, const std::vector<bool>& validMask, int width, int height, int radius);

int main() {
    // Test 1: Simple 3x3 image with one hole in center, radius=1
    {
        int w = 3, h = 3, r = 1;
        std::vector<float> img = {
            1, 2, 3,
            4, 0, 6,
            7, 8, 9
        };
        std::vector<bool> valid = {
            true, true, true,
            true, false, true,
            true, true, true
        };
        auto result = inpaintHoles(img, valid, w, h, r);
        // center should be filled with some value from neighbors (likely average of surrounding or from a source patch)
        // Since radius=1, the center patch includes all 8 neighbors, all valid, so source patch can be any centered at a valid pixel.
        // The algorithm will copy from best matching source, which likely is a neighbor. We just assert it's not 0 and within reasonable range.
        assert(std::abs(result[1*w+1] - 0.0f) > 0.01f);
        // All other pixels unchanged
        for (int i = 0; i < w*h; ++i) {
            if (i != 1*w+1) {
                assert(std::abs(result[i] - img[i]) < 1e-6);
            }
        }
    }

    // Test 2: No holes, image unchanged
    {
        int w = 4, h = 4, r = 1;
        std::vector<float> img = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
        std::vector<bool> valid(w*h, true);
        auto result = inpaintHoles(img, valid, w, h, r);
        for (size_t i = 0; i < img.size(); ++i) {
            assert(result[i] == img[i]);
        }
    }

    // Test 3: Image with a large hole, radius=1, check that interior holes get filled and border holes remain (since patch would go out of bounds)
    {
        int w = 5, h = 5, r = 1;
        // Build 5x5 image, valid except a 3x3 hole in middle (rows 1-3, cols 1-3)
        std::vector<float> img(w*h, 0.0f);
        std::vector<bool> valid(w*h, true);
        // Set background 0, and some pattern around hole
        for (int y = 0; y < h; ++y) {
            for (int x = 0; x < w; ++x) {
                img[y*w+x] = float(x + y*10);
            }
        }
        // Hollow out the 3x3 center
        for (int y = 1; y <= 3; ++y) {
            for (int x = 1; x <= 3; ++x) {
                int idx = y*w+x;
                valid[idx] = false;
                img[idx] = 0.0f;
            }
        }
        auto result = inpaintHoles(img, valid, w, h, r);
        // The hole's interior (e.g., center at (2,2)) should be filled (not 0)
        assert(std::abs(result[2*w+2]) > 0.01f);
        // Corner hole pixels (e.g., (1,1)) are within radius=1 of border? Actually (1,1) has patch from 0 to 2, all inside width 5, so should be filled.
        assert(std::abs(result[1*w+1]) > 0.01f);
        // Hole at edge? None here because hole is not touching border
        // Outer border pixels remain unchanged
        for (int y = 0; y < h; ++y) {
            for (int x = 0; x < w; ++x) {
                if (valid[y*w+x]) {
                    assert(result[y*w+x] == img[y*w+x]);
                }
            }
        }
    }

    // Test 4: All holes (except maybe border) - should not crash and return something
    {
        int w = 4, h = 4, r = 1;
        std::vector<float> img(w*h, 0.0f);
        std::vector<bool> valid(w*h, false);
        // Make one valid pixel at corner? Actually if all invalid, no boundary exists, so loop won't run, returns original.
        valid[1] = true; // pixel (1,0)
        img[1] = 5.0f;
        auto result = inpaintHoles(img, valid, w, h, r);
        // At least that pixel remains
        assert(result[1] == 5.0f);
        // Others may be filled or not, but no crash
        assert(result.size() == w*h);
    }

    // Test 5: Larger radius, ensure function works with radius=2 on a 7x7 image
    {
        int w = 7, h = 7, r = 2;
        std::vector<float> img(w*h, 0.0f);
        std::vector<bool> valid(w*h, true);
        for (int i = 0; i < w*h; ++i) img[i] = float(i % 10);
        // Create a hole in center
        valid[3*w+3] = false;
        img[3*w+3] = 0.0f;
        auto result = inpaintHoles(img, valid, w, h, r);
        // Center should be filled with some value not 0
        assert(std::abs(result[3*w+3]) > 0.01f);
    }

    return 0;
}
