// Write a C++ function `nmsRotatedKeepIndices` that accepts a vector of rotated bounding boxes (each represented by 5 doubles: center x, center y, width, height, angle in radians), a vector of corresponding scores, and an IoU threshold. The function must return a vector of indices (in original order) for boxes to keep after greedy non-maximum suppression: sort boxes by descending score, iterate in that order, and for each kept box, suppress any later lower-scoring box whose IoU with it is at least the threshold. Boxes with very small areas or degenerate dimensions (width or height ≤ 0) should be skipped entirely (never kept or used to suppress others). The input vectors must have equal length and be non-empty; if either condition fails, return an empty vector. The IoU calculation for rotated boxes must follow the standard intersection‑over‑union formula using polygon clipping (Sutherland–Hodgman) and area computation via the shoelace formula, with all computations in double precision. The function should be a free function named `nmsRotatedKeepIndices` in the global namespace, not require any external libraries beyond the C++ standard library, and handle numeric edge cases such as identical boxes, overlapping boxes, and boxes with negative or zero areas.
#include <cassert>
#include <vector>
#include <cmath>

int main() {
    // Test 1: Non-overlapping boxes -> all kept.
    std::vector<std::vector<double>> boxes1 = {{0,0,2,1,0}, {5,5,2,1,0}};
    std::vector<double> scores1 = {0.9, 0.8};
    auto keep1 = nmsRotatedKeepIndices(boxes1, scores1, 0.5);
    assert(keep1.size() == 2);
    assert(keep1[0] == 0 && keep1[1] == 1);

    // Test 2: Identical boxes -> only highest score kept.
    std::vector<std::vector<double>> boxes2 = {{1,1,2,2,0}, {1,1,2,2,0}, {1,1,2,2,0}};
    std::vector<double> scores2 = {0.5, 0.9, 0.7};
    auto keep2 = nmsRotatedKeepIndices(boxes2, scores2, 0.5);
    assert(keep2.size() == 1);
    assert(keep2[0] == 1);

    // Test 3: Overlapping different sizes, lower IoU below threshold -> both kept.
    std::vector<std::vector<double>> boxes3 = {{0,0,4,2,0}, {2,0,2,2,0}}; // IoU = area overlap = 2? 
    // Compute: box0 area=8, box1 area=4, intersection width=2,height=2 -> 4, union=8+4-4=8, IoU=0.5.
    std::vector<double> scores3 = {0.8, 0.9}; // higher score for smaller box, but overlap 0.5.
    auto keep3 = nmsRotatedKeepIndices(boxes3, scores3, 0.75);
    assert(keep3.size() == 2);
    assert(keep3[0] == 1 || keep3[0] == 0); // both kept regardless of order.

    // Test 4: Invalid box with zero width -> skipped.
    std::vector<std::vector<double>> boxes4 = {{0,0,0,5,0}, {3,0,2,2,0}};
    std::vector<double> scores4 = {0.9, 0.1};
    auto keep4 = nmsRotatedKeepIndices(boxes4, scores4, 0.5);
    assert(keep4.size() == 1);
    assert(keep4[0] == 1);

    // Test 5: Angle rotation, two perpendicular rectangles overlapping in a small area.
    std::vector<std::vector<double>> boxes5 = {{0,0,4,1,0}, {0,0,1,4,3.14159/2}}; // same center, rotated 90 deg.
    std::vector<double> scores5 = {0.9, 0.8};
    auto keep5 = nmsRotatedKeepIndices(boxes5, scores5, 0.5);
    // Intersection is 1x1 square area=1, union=4+4-1=7, IoU≈0.1429 < 0.5 -> both kept.
    assert(keep5.size() == 2);

    // Test 6: Suppression chain: highest suppresses second, but third not affected.
    std::vector<std::vector<double>> boxes6 = {{0,0,4,4,0}, {2,0,4,4,0}, {10,10,2,2,0}};
    std::vector<double> scores6 = {0.9, 0.8, 0.7};
    auto keep6 = nmsRotatedKeepIndices(boxes6, scores6, 0.5);
    // box0 and box1 overlap: intersection width=2,height=4 -> area=8, union=16+16-8=24, IoU=1/3 <0.5? Actually 8/24=0.333. Continue.
    // box0 and box2 no overlap.
    assert(keep6.size() == 3);

    // Test 7: Empty input -> empty result.
    std::vector<std::vector<double>> boxes7;
    std::vector<double> scores7;
    auto keep7 = nmsRotatedKeepIndices(boxes7, scores7, 0.5);
    assert(keep7.empty());

    // Test 8: Mismatched sizes -> empty.
    std::vector<std::vector<double>> boxes8 = {{0,0,1,1,0}, {1,1,1,1,0}};
    std::vector<double> scores8 = {0.5};
    auto keep8 = nmsRotatedKeepIndices(boxes8, scores8, 0.5);
    assert(keep8.empty());

    // Test 9: Exactly overlapping with threshold 0.0 -> still suppresses.
    std::vector<std::vector<double>> boxes9 = {{0,0,2,2,0}, {0,0,2,2,0}};
    std::vector<double> scores9 = {0.6, 0.5};
    auto keep9 = nmsRotatedKeepIndices(boxes9, scores9, 0.0);
    assert(keep9.size() == 1);
    assert(keep9[0] == 0);

    return 0;
}
#include <vector>
#include <algorithm>
#include <cmath>
#include <utility>

// Helper to compute polygon area using shoelace formula (absolute value / 2).
double polygonArea(const std::vector<std::pair<double,double>>& poly) {
    double area = 0.0;
    for (size_t i = 0; i < poly.size(); ++i) {
        size_t j = (i + 1) % poly.size();
        area += poly[i].first * poly[j].second;
        area -= poly[j].first * poly[i].second;
    }
    return std::abs(area) * 0.5;
}

// Clip a subject polygon against a single half-plane edge from p1 to p2.
void clipEdge(const std::vector<std::pair<double,double>>& subject,
              const std::pair<double,double>& p1,
              const std::pair<double,double>& p2,
              std::vector<std::pair<double,double>>& result) {
    result.clear();
    double x1 = p1.first, y1 = p1.second;
    double x2 = p2.first, y2 = p2.second;
    double A = y2 - y1;
    double B = x1 - x2;
    double C = x2 * y1 - x1 * y2; // line: A*x + B*y + C = 0

    for (size_t i = 0; i < subject.size(); ++i) {
        size_t j = (i + 1) % subject.size();
        double xi = subject[i].first, yi = subject[i].second;
        double xj = subject[j].first, yj = subject[j].second;
        double f_i = A * xi + B * yi + C;
        double f_j = A * xj + B * yj + C;

        bool in_i = f_i >= 0.0; // inside half-plane (assuming positive side)
        bool in_j = f_j >= 0.0;

        if (in_i) result.push_back(subject[i]);
        if (in_i != in_j) {
            double t = f_i / (f_i - f_j);
            double x = xi + t * (xj - xi);
            double y = yi + t * (yj - yi);
            result.push_back({x, y});
        }
    }
}

// Compute intersection polygon of two convex polygons (Sutherland–Hodgman).
std::vector<std::pair<double,double>> polygonIntersection(
        const std::vector<std::pair<double,double>>& a,
        const std::vector<std::pair<double,double>>& b) {
    std::vector<std::pair<double,double>> current = a;
    std::vector<std::pair<double,double>> next;
    for (size_t i = 0; i < b.size(); ++i) {
        if (current.empty()) break;
        clipEdge(current, b[i], b[(i+1) % b.size()], next);
        current.swap(next);
    }
    return current;
}

// Convert rotated box (cx, cy, w, h, angle) to 4 corner points.
// Angle in radians, rotation counterclockwise around center.
std::vector<std::pair<double,double>> boxCorners(double cx, double cy, double w, double h, double angle) {
    std::vector<std::pair<double,double>> corners(4);
    double cos_a = std::cos(angle), sin_a = std::sin(angle);
    double dx = w / 2.0, dy = h / 2.0;
    // Local coordinates relative to center before rotation.
    std::vector<std::pair<double,double>> local = {{-dx, -dy}, {dx, -dy}, {dx, dy}, {-dx, dy}};
    for (int i = 0; i < 4; ++i) {
        double lx = local[i].first, ly = local[i].second;
        double rx = lx * cos_a - ly * sin_a;
        double ry = lx * sin_a + ly * cos_a;
        corners[i] = {cx + rx, cy + ry};
    }
    return corners;
}

// Compute IoU between two rotated boxes.
double iouRotated(const std::vector<double>& box1, const std::vector<double>& box2) {
    double area1 = box1[2] * box1[3]; // width*height
    double area2 = box2[2] * box2[3];
    if (area1 <= 0.0 || area2 <= 0.0) return 0.0;

    auto corners1 = boxCorners(box1[0], box1[1], box1[2], box1[3], box1[4]);
    auto corners2 = boxCorners(box2[0], box2[1], box2[2], box2[3], box2[4]);

    auto interPoly = polygonIntersection(corners1, corners2);
    double interArea = polygonArea(interPoly);
    double unionArea = area1 + area2 - interArea;
    if (unionArea <= 0.0) return 0.0;
    return interArea / unionArea;
}

// Perform non-maximum suppression for rotated boxes.
std::vector<int> nmsRotatedKeepIndices(const std::vector<std::vector<double>>& boxes,
                                       const std::vector<double>& scores,
                                       double iouThreshold) {
    if (boxes.empty() || boxes.size() != scores.size()) return {};

    size_t n = boxes.size();
    std::vector<int> order(n);
    for (size_t i = 0; i < n; ++i) order[i] = static_cast<int>(i);
    // Stable sort by descending score.
    std::stable_sort(order.begin(), order.end(), [&](int i, int j) {
        return scores[i] > scores[j];
    });

    std::vector<bool> suppressed(n, false);
    std::vector<int> keep;
    keep.reserve(n);

    for (size_t i = 0; i < n; ++i) {
        int idx = order[i];
        if (suppressed[idx]) continue;
        if (boxes[idx][2] <= 0.0 || boxes[idx][3] <= 0.0) continue; // skip invalid
        keep.push_back(idx);
        for (size_t j = i + 1; j < n; ++j) {
            int cand = order[j];
            if (suppressed[cand]) continue;
            if (boxes[cand][2] <= 0.0 || boxes[cand][3] <= 0.0) continue;
            double iou = iouRotated(boxes[idx], boxes[cand]);
            if (iou >= iouThreshold) suppressed[cand] = true;
        }
    }
    return keep;
}
// The solution sorts the box indices by descending score using a stable sort to preserve original order for equal scores. It maintains a boolean `suppressed` vector initialized to false. For each box in sorted order, if it is already suppressed or has invalid dimensions (width ≤ 0 or height ≤ 0), it is skipped. Otherwise, its index is appended to the result, and for every later box in sorted order that is not suppressed and has valid dimensions, we compute the IoU between the current kept box and that candidate. The IoU is computed by transforming both rotated rectangles into polygon vertices (8 corners each), then clipping the first polygon against all edges of the second using the Sutherland–Hodgman algorithm (handling concave clipping polygons by clipping against both convex hulls, but for rotated rectangles both are convex, so direct clipping works). If the clipped polygon has zero area, IoU = 0. Otherwise, compute the intersection area via the shoelace formula, then IoU = intersectionArea / (area1 + area2 − intersectionArea). If IoU ≥ threshold, suppress the candidate. Time complexity is O(n² · p) where p is the polygon vertex count (max 8 after clipping), and space complexity is O(n) for the result and suppression flags. Edge cases include identical boxes (IoU = 1, suppress all duplicates except first), boxes that only touch (IoU = 0 due to zero intersection area), and invalid boxes that must be ignored.
