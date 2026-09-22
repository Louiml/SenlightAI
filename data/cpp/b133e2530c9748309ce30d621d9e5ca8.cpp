// Given two binary images represented as 2D matrices of `unsigned char` values (0 for background, 255 for foreground), write a C++ function that determines whether the foreground regions in both images are "paired" based on their centroids. Specifically, for each foreground connected component (blob) in the left image, find the closest foreground blob in the right image whose centroid has a strictly greater x-coordinate and is within a given maximum Euclidean distance (specified as `maxDist`). If such a right blob exists, mark the pair as valid. Additionally, apply a "selection relaxation" rule: if multiple right blobs are within 1.2 times the minimum distance, prefer the one with the smallest angle (between the horizontal axis and the line from left centroid to right centroid), where the angle is measured in radians in the range `[-π, π]`. The function must return a `std::vector<std::pair<size_t, size_t>>` containing indices of the matched left blob (first) and right blob (second) for each valid left blob. If no match is found for a left blob, skip it (do not include it in the output). The input matrices may have different dimensions, and all foreground pixels are assumed to form well-separated blobs (no blobs touch across images, but within an image blobs are distinct connected components using 4-connectivity). You may assume that each connected component has at least one pixel, and that centroids are computed as the average of pixel coordinates (floating-point, not rounded). The function must be self-contained, use no external libraries beyond the standard C++ libraries, and be documented with const-correctness.
#include <cassert>
#include <cmath>
#include <vector>
#include <utility>

// The solution function is declared above (or included from the solution file).
// Provide a main function that tests various scenarios.

int main() {
    // Test 1: Simple case with one blob each, left blob at (1,1), right at (5,1), maxDist=10.
    std::vector<std::vector<unsigned char>> left1 = {
        {0,0,0},
        {0,255,0},
        {0,0,0}
    };
    std::vector<std::vector<unsigned char>> right1 = {
        {0,0,0,0,0,0},
        {0,0,0,0,255,0},
        {0,0,0,0,0,0}
    };
    auto pairs1 = matchBlobPairs(left1, right1, 10.0);
    assert(pairs1.size() == 1);
    assert(pairs1[0].first == 0 && pairs1[0].second == 0);

    // Test 2: No match because right blob not to the right (same x).
    std::vector<std::vector<unsigned char>> left2 = {
        {255,0},
        {0,0}
    };
    std::vector<std::vector<unsigned char>> right2 = {
        {255,0},
        {0,0}
    };
    auto pairs2 = matchBlobPairs(left2, right2, 5.0);
    assert(pairs2.empty());

    // Test 3: Two left blobs, one matches, the other is too far.
    std::vector<std::vector<unsigned char>> left3(5, std::vector<unsigned char>(5, 0));
    left3[0][0] = 255; // blob 0 at (0,0)
    left3[4][4] = 255; // blob 1 at (4,4)
    std::vector<std::vector<unsigned char>> right3(5, std::vector<unsigned char>(5, 0));
    right3[0][2] = 255; // blob 0 at (2,0) - dist 2 from left blob 0
    right3[4][0] = 255; // blob 1 at (0,4) - not to right of (4,4)? x=0 < 4, so no match
    auto pairs3 = matchBlobPairs(left3, right3, 3.0);
    assert(pairs3.size() == 1);
    assert(pairs3[0].first == 0 && pairs3[0].second == 0);

    // Test 4: Relaxation chooses smaller angle when multiple within 1.2x minDist.
    // Left at (0,0), right blobs at (10,0) (angle 0) and (10,1) (angle ~0.1) and (10,-1) (angle ~-0.1)
    std::vector<std::vector<unsigned char>> left4(1, std::vector<unsigned char>(1, 255));
    std::vector<std::vector<unsigned char>> right4(3, std::vector<unsigned char>(11, 0));
    right4[1][10] = 255; // (10,1)
    right4[0][10] = 255; // (10,0)
    right4[2][10] = 255; // (10,-1)
    auto pairs4 = matchBlobPairs(left4, right4, 20.0);
    assert(pairs4.size() == 1);
    // All distances are sqrt(101) ~10.05, sqrt(100)=10, sqrt(101). The min is 10 (middle blob).
    // Relaxation: all within 1.2*10=12. So all three are candidates.
    // Angles: atan2(1,10)≈0.0997, atan2(0,10)=0, atan2(-1,10)≈-0.0997. Smallest abs angle is 0 -> middle blob.
    assert(pairs4[0].second == 1); // index 1 in right4 is the middle row

    // Test 5: Tie angle, choose closer one. Left at (0,0), right at (5,0) and (10,0).
    // Both have angle 0, distances 5 and 10. The min distance is 5, so that's the first candidate.
    // Relaxation: both within 1.2*5=6, only the first is within, so it's selected.
    std::vector<std::vector<unsigned char>> left5(1, std::vector<unsigned char>(1, 255));
    std::vector<std::vector<unsigned char>> right5(1, std::vector<unsigned char>(11, 0));
    right5[0][5] = 255;
    right5[0][10] = 255;
    auto pairs5 = matchBlobPairs(left5, right5, 20.0);
    assert(pairs5.size() == 1);
    assert(pairs5[0].second == 0); // the one at x=5

    // Test 6: Empty images.
    std::vector<std::vector<unsigned char>> left6;
    std::vector<std::vector<unsigned char>> right6;
    auto pairs6 = matchBlobPairs(left6, right6, 10.0);
    assert(pairs6.empty());

    // Test 7: Multiple blobs, ensure indices preserved correctly.
    std::vector<std::vector<unsigned char>> left7(4, std::vector<unsigned char>(4, 0));
    left7[0][0] = 255; // (0,0)
    left7[3][3] = 255; // (3,3)
    std::vector<std::vector<unsigned char>> right7(4, std::vector<unsigned char>(4, 0));
    right7[0][1] = 255; // (1,0)
    right7[3][0] = 255; // (0,3) - not to right of (3,3), so no match for left blob 1
    auto pairs7 = matchBlobPairs(left7, right7, 2.0);
    assert(pairs7.size() == 1);
    assert(pairs7[0].first == 0 && pairs7[0].second == 0);

    return 0;
}
#include <vector>
#include <queue>
#include <cmath>
#include <utility>
#include <algorithm>
#include <limits>

// Compute centroids of 4-connected foreground components (value==255) in a 2D binary image.
// Returns a vector of (x, y) floating-point centroids in row-major order.
std::vector<std::pair<double, double>> computeCentroids(const std::vector<std::vector<unsigned char>>& img) {
    int rows = (int)img.size();
    if (rows == 0) return {};
    int cols = (int)img[0].size();

    std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));
    std::vector<std::pair<double, double>> centroids;

    // 4-connected neighbor offsets
    const int dx[4] = {1, -1, 0, 0};
    const int dy[4] = {0, 0, 1, -1};

    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (img[r][c] != 255 || visited[r][c]) continue;

            // BFS to collect all pixels of this component
            std::queue<std::pair<int,int>> q;
            q.push({r, c});
            visited[r][c] = true;
            long long sumX = 0, sumY = 0;
            long long count = 0;

            while (!q.empty()) {
                auto [cr, cc] = q.front();
                q.pop();
                sumX += cc;
                sumY += cr;
                ++count;

                for (int k = 0; k < 4; ++k) {
                    int nr = cr + dy[k];
                    int nc = cc + dx[k];
                    if (nr >= 0 && nr < rows && nc >= 0 && nc < cols &&
                        !visited[nr][nc] && img[nr][nc] == 255) {
                        visited[nr][nc] = true;
                        q.push({nr, nc});
                    }
                }
            }

            if (count > 0) {
                centroids.emplace_back((double)sumX / count, (double)sumY / count);
            }
        }
    }
    return centroids;
}

// Match left blobs to right blobs based on centroid proximity and angle relaxation.
// Returns pairs of (left_blob_index, right_blob_index).
std::vector<std::pair<size_t, size_t>> matchBlobPairs(
    const std::vector<std::vector<unsigned char>>& leftImg,
    const std::vector<std::vector<unsigned char>>& rightImg,
    double maxDist) {

    auto leftC = computeCentroids(leftImg);
    auto rightC = computeCentroids(rightImg);

    std::vector<std::pair<size_t, size_t>> result;

    for (size_t i = 0; i < leftC.size(); ++i) {
        double lx = leftC[i].first;
        double ly = leftC[i].second;

        double minDist = std::numeric_limits<double>::max();
        size_t bestRight = std::numeric_limits<size_t>::max();

        // First pass: find the minimum distance among valid candidates (x > lx and dist <= maxDist)
        for (size_t j = 0; j < rightC.size(); ++j) {
            double rx = rightC[j].first;
            double ry = rightC[j].second;
            if (rx <= lx) continue; // must be strictly to the right
            double dist = std::sqrt((rx - lx) * (rx - lx) + (ry - ly) * (ry - ly));
            if (dist <= maxDist && dist < minDist) {
                minDist = dist;
                bestRight = j;
            }
        }

        if (bestRight == std::numeric_limits<size_t>::max()) continue; // no valid candidate

        // Second pass: relaxation - among all right blobs with dist <= 1.2 * minDist
        // and same x condition, pick the one with smallest absolute angle.
        double bestAngle = std::numeric_limits<double>::max();
        size_t selectedRight = bestRight; // fallback to the min-distance one
        double bestAngleDist = minDist;   // remember distance for tie-breaking

        for (size_t j = 0; j < rightC.size(); ++j) {
            double rx = rightC[j].first;
            double ry = rightC[j].second;
            if (rx <= lx) continue;
            double dist = std::sqrt((rx - lx) * (rx - lx) + (ry - ly) * (ry - ly));
            if (dist > maxDist || dist > 1.2 * minDist) continue;

            double angle = std::atan2(ry - ly, rx - lx);
            double absAngle = std::fabs(angle);
            if (absAngle < bestAngle - 1e-12) {
                bestAngle = absAngle;
                selectedRight = j;
            } else if (std::fabs(absAngle - bestAngle) < 1e-12 && dist < bestAngleDist) {
                // Tie on angle: pick the closer one
                bestAngleDist = dist;
                selectedRight = j;
            }
        }

        result.emplace_back(i, selectedRight);
    }

    return result;
}
// The solution involves several steps: (1) Compute centroids for each connected component in the left and right images using a flood-fill (BFS/DFS) over 4-connected foreground pixels. (2) For each left blob, iterate over all right blobs and find those satisfying the conditions: right centroid x > left centroid x, and Euclidean distance ≤ `maxDist`. Among these candidates, find the one with the minimum distance. (3) Then apply the relaxation: among all right blobs that satisfy the same x and distance conditions but also are within 1.2× the minimum distance, select the one with the smallest absolute angle (measured from horizontal), where angle = atan2(right_y - left_y, right_x - left_x). If tie in angle, keep the first encountered. (4) Return the matched pairs. Time complexity: Let L be the number of foreground pixels in left, R in right, and consider the number of blobs B_L and B_R. Flood fill for each image is O(L) and O(R). Matching is O(B_L × B_R). In the worst case where every pixel is a separate blob, this becomes O(L × R) which is potentially quadratic, but for typical blob structures it is efficient. Space complexity is O(L + R) for the visited arrays and O(B_L + B_R) for storing centroids. Edge cases: no blobs in either image yields empty result; if no right blob satisfies the x-coordinate and distance conditions, the left blob is skipped; ensure distances are computed as double to avoid integer overflow on large coordinates.
