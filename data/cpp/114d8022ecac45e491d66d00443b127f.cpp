// Write a C++ function that, given a grayscale image represented as a 2D vector of unsigned chars (values 0–255) and a set of candidate column positions (a vector of ints, sorted ascending, representing the left edge of each character region in a license plate), performs a simple rule-based refinement of each character bounding box. For each candidate position except the first, the function must: (1) extract a vertical strip from the image starting at that candidate position with a fixed width of 20 pixels (clamped to image bounds), (2) apply a simple threshold (e.g., pixel value > 128 becomes 255, else 0) to obtain a binary strip, (3) find all connected components (using a simple 4-connected flood-fill) in the binary strip, (4) select the component whose centroid is closest in horizontal distance to the strip's center and whose height is at least half the image height, (5) compute a bounding box for that component, then expand it by 2 pixels on the left/right and top/bottom (clamped to strip bounds), and finally (6) offset the bounding box by the strip's column offset to get coordinates in the original image. Return a vector of `cv::Rect`-like structs (define your own simple struct with `x, y, width, height` as ints) in the same order as the input candidate positions, with the first candidate simply producing a full-height rectangle from that position to position+20 (clamped). Do not use any external libraries—implement everything with standard C++ only (vectors, queues, etc.).
#include <cassert>
#include <vector>
#include <iostream>

// Include the solution function here (for brevity, assume it's above)

int main() {
    // Create a simple synthetic image: 36 rows, 136 cols
    // Simulate a license plate: first char at col 0-19 (full strip), then chars at 20-39, 40-59, etc.
    // For testing, we'll create an image with white blobs at expected positions.
    int rows = 36, cols = 136;
    std::vector<std::vector<unsigned char>> img(rows, std::vector<unsigned char>(cols, 0));
    // Put a white rectangle (character) at columns 22-28, rows 5-30 (representing a char inside strip starting at 20)
    for (int r = 5; r <= 30; ++r)
        for (int c = 22; c <= 28; ++c)
            img[r][c] = 255;
    // Another char at columns 42-48 (strip starting at 40)
    for (int r = 5; r <= 30; ++r)
        for (int c = 42; c <= 48; ++c)
            img[r][c] = 255;
    // Small noise component (should be ignored due to height < 18)
    for (int r = 0; r < 5; ++r)
        for (int c = 25; c <= 26; ++c)
            img[r][c] = 255;
    
    // Candidate positions: 0, 20, 40, 60
    std::vector<int> candidates = {0, 20, 40, 60};
    
    auto rects = refineRegions(img, candidates);
    
    // First rect should be full strip at x=0, width=20 (clamped to 20), height=36
    assert(rects.size() == 4);
    assert(rects[0].x == 0);
    assert(rects[0].width == 20);
    assert(rects[0].y == 0);
    assert(rects[0].height == 36);
    
    // Second rect: after refinement, the blob is at columns 22-28 within strip (0-19 offset), so x in image = 20+ (x strip). Bounding box before padding: x=2, y=5, w=7, h=26. After padding: x=0, y=3, w=9, h=30? Actually padding=2 on both sides: x=0 (2-2), y=3 (5-2), w=7+4=11, h=26+4=30. Check.
    assert(rects[1].x >= 20 && rects[1].x <= 20 + 2); // may be clamped
    assert(rects[1].y >= 3);
    assert(rects[1].width >= 9 && rects[1].width <= 11);
    assert(rects[1].height >= 26 && rects[1].height <= 30);
    
    // Third rect: similar, blob at columns 42-48, offset 40+ (2 to 8)
    assert(rects[2].x >= 40 && rects[2].x <= 42);
    assert(rects[2].y >= 3);
    
    // Fourth candidate: no blob (all background), so default full strip
    assert(rects[3].x == 60);
    assert(rects[3].y == 0);
    assert(rects[3].width == 20); // clamped if near edge? 60+20=80 < 136, so width=20
    assert(rects[3].height == 36);
    
    // Test edge case: candidate near right edge
    std::vector<int> candidates2 = {130}; // only one candidate
    auto rects2 = refineRegions(img, candidates2);
    assert(rects2.size() == 1);
    assert(rects2[0].x == 130);
    assert(rects2[0].width == 6); // 136-130 = 6
    assert(rects2[0].height == 36);
    
    std::cout << "All tests passed." << std::endl;
    return 0;
}
#include <vector>
#include <queue>
#include <algorithm>
#include <cstdint>

// Simple rectangle structure
struct Rect {
    int x, y, width, height;
    Rect(int x_, int y_, int w_, int h_) : x(x_), y(y_), width(w_), height(h_) {}
};

// Function to compute refined character rectangles from candidate column positions
std::vector<Rect> refineRegions(const std::vector<std::vector<unsigned char>>& image,
                                const std::vector<int>& candidatePts,
                                int stripWidth = 20,
                                int padding = 2) {
    int rows = static_cast<int>(image.size());
    int cols = (rows > 0) ? static_cast<int>(image[0].size()) : 0;
    std::vector<Rect> result;
    
    // Process each candidate
    for (size_t idx = 0; idx < candidatePts.size(); ++idx) {
        int left = candidatePts[idx];
        if (left >= cols) {
            // Invalid position, push empty rect (or skip? We'll push a zero-size rect)
            result.push_back(Rect(left, 0, 0, rows));
            continue;
        }
        
        int right = std::min(left + stripWidth, cols - 1);
        int width = right - left + 1;  // actual strip width
        
        // First candidate: full-height strip
        if (idx == 0) {
            result.push_back(Rect(left, 0, width, rows));
            continue;
        }
        
        // Extract strip and binarize
        std::vector<std::vector<unsigned char>> binary(rows, std::vector<unsigned char>(width, 0));
        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < width; ++c) {
                binary[r][c] = (image[r][left + c] > 128) ? 255 : 0;
            }
        }
        
        // Connected-component labeling (BFS 4-connected)
        std::vector<std::vector<bool>> visited(rows, std::vector<bool>(width, false));
        int bestDist = INT32_MAX;
        Rect bestRect(0, 0, 0, 0);
        bool found = false;
        
        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < width; ++c) {
                if (binary[r][c] != 0 && !visited[r][c]) {
                    // BFS this component
                    std::queue<std::pair<int,int>> q;
                    q.push({r, c});
                    visited[r][c] = true;
                    int minR = r, maxR = r, minC = c, maxC = c;
                    long long sumX = 0, count = 0;
                    
                    while (!q.empty()) {
                        auto [cr, cc] = q.front(); q.pop();
                        minR = std::min(minR, cr);
                        maxR = std::max(maxR, cr);
                        minC = std::min(minC, cc);
                        maxC = std::max(maxC, cc);
                        sumX += cc;
                        count++;
                        
                        // 4-neighbors
                        static const int dr[] = {-1, 1, 0, 0};
                        static const int dc[] = {0, 0, -1, 1};
                        for (int k = 0; k < 4; ++k) {
                            int nr = cr + dr[k], nc = cc + dc[k];
                            if (nr >= 0 && nr < rows && nc >= 0 && nc < width &&
                                !visited[nr][nc] && binary[nr][nc] != 0) {
                                visited[nr][nc] = true;
                                q.push({nr, nc});
                            }
                        }
                    }
                    
                    // Check height condition
                    if (maxR - minR + 1 < rows / 2) continue;
                    
                    // Compute centroid x
                    double centroidX = (count > 0) ? (double)sumX / count : (minC + maxC) / 2.0;
                    double centerX = width / 2.0;
                    int dist = static_cast<int>(std::abs(centroidX - centerX));
                    
                    if (!found || dist < bestDist) {
                        bestDist = dist;
                        // Build bounding box
                        Rect bbox(minC, minR, maxC - minC + 1, maxR - minR + 1);
                        // Expand by padding
                        bbox.x = std::max(0, bbox.x - padding);
                        bbox.y = std::max(0, bbox.y - padding);
                        bbox.width = std::min(width - bbox.x, bbox.width + 2 * padding);
                        bbox.height = std::min(rows - bbox.y, bbox.height + 2 * padding);
                        bestRect = bbox;
                        found = true;
                    }
                }
            }
        }
        
        // If no suitable component, default to full strip
        if (!found) {
            bestRect = Rect(0, 0, width, rows);
        }
        
        // Offset to original image coordinates
        bestRect.x += left;
        result.push_back(bestRect);
    }
    
    return result;
}
// The problem is essentially a simplified version of the license plate character segmentation refinement step. The main algorithm processes each candidate column position independently after the first. For each such position, we extract a 20-pixel-wide vertical strip. Because the strip may extend beyond the image, we clamp the right edge to the image width-1. The strip is binarized with a fixed threshold, which is simpler than Otsu or Niblack but sufficient for a self-contained task. We then perform connected-component labeling using a breadth-first search (BFS) over 4-neighbors. For each component, we compute its bounding box and its centroid's x-coordinate (average of x's). We select the component with the smallest absolute horizontal distance from the strip center, but only if its height is at least half the image height (to avoid noise). If no such component exists, we default to a rectangle covering the entire strip. Once selected, we expand the bounding box by 2 pixels in all directions, clamp to strip bounds, and then add the strip's left column offset to get the final rectangle in the original image coordinates. For the first candidate, we return a rectangle from that column to column+20 (clamped to image width) spanning the full image height.
//
// Edge cases: the candidate position may be near the right edge, so the strip width may be less than 20; clamping handles this. The connected components may have zero area if the strip is all background; we handle this by defaulting. The height condition ensures we ignore small blobs. Time complexity: for each candidate position, we scan the strip of size at most 20 × height, and each BFS visits each pixel once, so O(W*H) per strip where W ≤ 20 and H is image height. With k candidates, total O(k*H*20) = O(k*H). Space complexity: O(H*20) for the binary strip and O(H*20) for the visited array.
