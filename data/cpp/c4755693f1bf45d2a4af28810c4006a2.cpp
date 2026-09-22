/*
Write a C++ standalone function named `nms_filter` that performs non-maximum suppression on a vector of detections. Each detection is represented by a struct `Detection` containing `double x, y, width, height, score;`. The function must accept a `std::vector<Detection>` (by value or const reference), an `float iou_threshold` (the IoU overlap threshold above which a box is suppressed), and return a new `std::vector<Detection>` containing the surviving detections, sorted by descending score. If two boxes have the same score, the one appearing first in the input should be preferred (i.e., suppression should use stable sorting). The intersection over union (IoU) is computed as the area of intersection divided by the area of union between two rectangles. Boxes that do not overlap at all (IoU = 0) are never suppressed. The function must handle empty input gracefully. An additional requirement: when a box is suppressed due to high overlap with a kept box, the kept box's score is increased by the suppressed box's score (score accumulation), but the kept box's geometry remains unchanged. This must be done iteratively as you process boxes in score order.
*/

#include <vector>
#include <algorithm>
#include <cstddef>

struct Detection {
    double x, y, width, height, score;
};

// Perform non-maximum suppression with score accumulation.
// Returns surviving detections sorted by descending score.
std::vector<Detection> nms_filter(std::vector<Detection> detections, float iou_threshold) {
    if (detections.empty()) return {};

    // Stable sort by descending score (equal scores keep original order).
    std::stable_sort(detections.begin(), detections.end(),
        [](const Detection& a, const Detection& b) {
            return a.score > b.score;
        });

    std::vector<bool> suppressed(detections.size(), false);
    std::vector<Detection> result;
    result.reserve(detections.size());

    for (std::size_t i = 0; i < detections.size(); ++i) {
        if (suppressed[i]) continue;

        // Keep the current detection.
        result.push_back(detections[i]);
        Detection& kept = result.back();

        // Area of the kept box.
        double area1 = kept.width * kept.height;
        double x1 = kept.x;
        double y1 = kept.y;
        double x2 = kept.x + kept.width - 1.0;
        double y2 = kept.y + kept.height - 1.0;

        // Check all later boxes that are not yet suppressed.
        for (std::size_t j = i + 1; j < detections.size(); ++j) {
            if (suppressed[j]) continue;

            const Detection& cand = detections[j];
            double cand_w = cand.width;
            double cand_h = cand.height;

            // Skip boxes with zero area to avoid division by zero.
            if (cand_w <= 0.0 || cand_h <= 0.0 || area1 <= 0.0) continue;

            // Compute intersection.
            double inter_x = std::max(x1, cand.x);
            double inter_y = std::max(y1, cand.y);
            double inter_w = std::min(x2, cand.x + cand_w - 1.0) - inter_x + 1.0;
            double inter_h = std::min(y2, cand.y + cand_h - 1.0) - inter_y + 1.0;
            if (inter_w <= 0.0 || inter_h <= 0.0) continue;  // no overlap

            double inter_area = inter_w * inter_h;
            double union_area = area1 + cand_w * cand_h - inter_area;
            if (union_area <= 0.0) continue;  // safeguard

            double iou = inter_area / union_area;
            if (iou > iou_threshold) {
                suppressed[j] = true;
                kept.score += cand.score;  // accumulate score
            }
        }
    }
    return result;
}

#include <cassert>
#include <cmath>
#include <vector>

// The function and struct are assumed to be defined above.

int main() {
    // Test 1: Non-overlapping boxes -> all kept.
    std::vector<Detection> input1 = {{0, 0, 10, 10, 0.5}, {20, 20, 10, 10, 0.3}};
    auto out1 = nms_filter(input1, 0.5f);
    assert(out1.size() == 2);
    assert(std::abs(out1[0].score - 0.5) < 1e-6);
    assert(std::abs(out1[1].score - 0.3) < 1e-6);

    // Test 2: Two identical boxes with same score -> first kept, score doubled.
    std::vector<Detection> input2 = {{0, 0, 5, 5, 0.8}, {0, 0, 5, 5, 0.8}};
    auto out2 = nms_filter(input2, 0.5f);
    assert(out2.size() == 1);
    assert(std::abs(out2[0].score - 1.6) < 1e-6);
    assert(out2[0].x == 0.0 && out2[0].y == 0.0);

    // Test 3: High overlap with threshold 0.9 -> both kept (IoU=1 not > 0.9? Actually IoU=1 > 0.9, so only one kept)
    // Correct: identical boxes have IoU=1 > 0.9, so only one.
    std::vector<Detection> input3 = {{0, 0, 10, 10, 0.9}, {0, 0, 10, 10, 0.1}};
    auto out3 = nms_filter(input3, 0.9f);
    assert(out3.size() == 1);
    assert(std::abs(out3[0].score - 1.0) < 1e-6);

    // Test 4: Partial overlap (IoU = 0.5) with threshold 0.3 -> merged; with threshold 0.6 -> kept separately.
    std::vector<Detection> input4 = {{0, 0, 10, 10, 0.7}, {5, 0, 10, 10, 0.2}};
    auto out4a = nms_filter(input4, 0.3f);
    assert(out4a.size() == 1);
    assert(std::abs(out4a[0].score - 0.9) < 1e-6);
    auto out4b = nms_filter(input4, 0.6f);
    assert(out4b.size() == 2);
    assert(std::abs(out4b[0].score - 0.7) < 1e-6);
    assert(std::abs(out4b[1].score - 0.2) < 1e-6);

    // Test 5: Empty input.
    assert(nms_filter({}, 0.5f).empty());

    // Test 6: Box with zero area (degenerate) is never suppressed.
    std::vector<Detection> input6 = {{0, 0, 0, 10, 0.9}, {0, 0, 10, 10, 0.5}};
    auto out6 = nms_filter(input6, 0.5f);
    assert(out6.size() == 2);
    // First kept (higher score), second also kept because IoU with degenerate is 0.

    // Test 7: Stable sorting for equal scores – original order preserved.
    std::vector<Detection> input7 = {{5, 5, 1, 1, 0.5}, {0, 0, 10, 10, 0.5}};
    auto out7 = nms_filter(input7, 0.0f); // threshold 0: only identical boxes merge, here no overlap, both kept
    // Since scores equal, order should be same as input.
    assert(out7[0].x == 5.0 && out7[1].x == 0.0);
    // But with high overlap, first in input is kept.
    std::vector<Detection> input7b = {{0, 0, 10, 10, 0.5}, {0, 0, 10, 10, 0.5}};
    auto out7b = nms_filter(input7b, 0.5f);
    assert(out7b.size() == 1);
    assert(out7b[0].x == 0.0 && out7b[0].y == 0.0); // first one kept

    return 0;
}

// The algorithm follows a greedy, sort-based approach. First, sort the input detections by descending score. To maintain stability for equal scores, use `std::stable_sort` with a comparator that only compares scores (since equal scores preserve original order). Then iterate over the sorted list, maintaining a boolean `suppressed` array parallel to the sorted order. For each index `i` from left to right, if not suppressed, it becomes a kept box: copy it into the result, then accumulate scores from any later boxes that have IoU > threshold with it (marking them suppressed and adding their scores to the result's last element). Important edge cases: (1) If a box has zero width or height, its area is zero, and IoU becomes zero (or division by zero) – to avoid division by zero, treat zero-area boxes as having IoU = 0, so they are never suppressed and never suppress others. (2) If threshold is 0 or negative, only boxes with perfect overlap (IoU > threshold, but note IoU is between 0 and 1, so a threshold <= 0 means all boxes with any overlap get merged, which may be too aggressive; typical threshold is positive). (3) Empty input returns empty vector. (4) Duplicate identical boxes: the first one (by score then original order) is kept, the rest are suppressed and their scores accumulated into the first. Time complexity is O(n log n) for sorting plus O(n^2) for the pair checks, which is dominated by O(n^2) in the worst case. Space complexity is O(n) for the sorted copy and mask.
