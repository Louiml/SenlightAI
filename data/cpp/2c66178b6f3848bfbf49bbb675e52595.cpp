Write a standalone C++ function named `merge_inference_results` that takes a vector of floating-point detection outputs in the format `[num_detections, det1 (5 floats: cx, cy, w, h, conf), det2, ...]` (where each detection may optionally include a class_id at the end, but for this task assume exactly 5 floats per detection, i.e., no class_id), a confidence threshold, and an IoU threshold. The function must perform Non-Maximum Suppression (NMS) globally across all detections (ignoring class distinction, since no class_id is present) and return a sorted list (by confidence descending) of `Detection` structs (defined as `struct Detection { float bbox[4]; float conf; };`) that have confidence greater than the threshold and survive NMS. The input vector represents a single image’s raw output—do not handle batching or coordinate transformation. The function signature must be `std::vector<Detection> merge_inference_results(const std::vector<float>& raw_output, float conf_thresh, float nms_thresh);`.

#include <cassert>
#include <vector>

// (The solution code above is assumed to be included here.)

int main() {
    // Test 1: Empty input
    std::vector<float> empty;
    auto res1 = merge_inference_results(empty, 0.5, 0.5);
    assert(res1.empty());

    // Test 2: Single detection, high confidence
    std::vector<float> single = {1.0f, 10, 10, 20, 20, 0.9f};
    auto res2 = merge_inference_results(single, 0.5, 0.5);
    assert(res2.size() == 1);
    assert(res2[0].conf == 0.9f);
    assert(res2[0].bbox[0] == 10.0f);
    assert(res2[0].bbox[1] == 10.0f);
    assert(res2[0].bbox[2] == 20.0f);
    assert(res2[0].bbox[3] == 20.0f);

    // Test 3: Confidence threshold filters all
    std::vector<float> low_conf = {2.0f, 5,5,10,10, 0.3f, 6,6,10,10, 0.4f};
    auto res3 = merge_inference_results(low_conf, 0.5, 0.5);
    assert(res3.empty());

    // Test 4: Two overlapping detections, NMS removes one
    std::vector<float> overlap = {2.0f, 10,10,20,20, 0.9f, 12,10,20,20, 0.8f};
    auto res4 = merge_inference_results(overlap, 0.5, 0.4);
    assert(res4.size() == 1);
    assert(res4[0].conf == 0.9f);

    // Test 5: Two non-overlapping detections, both kept
    std::vector<float> separate = {2.0f, 10,10,10,10, 0.9f, 50,50,10,10, 0.8f};
    auto res5 = merge_inference_results(separate, 0.5, 0.5);
    assert(res5.size() == 2);
    assert(res5[0].conf == 0.9f);
    assert(res5[1].conf == 0.8f);

    // Test 6: Sorted by confidence descending
    std::vector<float> three = {3.0f, 0,0,5,5, 0.7f, 100,100,5,5, 0.9f, 200,200,5,5, 0.8f};
    auto res6 = merge_inference_results(three, 0.5, 0.5);
    assert(res6.size() == 3);
    assert(res6[0].conf == 0.9f);
    assert(res6[1].conf == 0.8f);
    assert(res6[2].conf == 0.7f);

    // Test 7: IoU threshold high blocks removal
    std::vector<float> mild_overlap = {2.0f, 10,10,20,20, 0.9f, 15,10,20,20, 0.8f};
    auto res7 = merge_inference_results(mild_overlap, 0.5, 0.6);
    assert(res7.size() == 2); // IoU ~0.2 < 0.6, both kept

    // Test 8: Invalid number of detections (truncated) should not crash
    std::vector<float> truncated = {3.0f, 0,0,5,5, 0.9f}; // only one detection provided
    auto res8 = merge_inference_results(truncated, 0.5, 0.5);
    assert(res8.size() == 1);
    assert(res8[0].conf == 0.9f);

    return 0;
}

#include <vector>
#include <algorithm>
#include <cmath>

struct Detection {
    float bbox[4]; // cx, cy, w, h
    float conf;
};

// Compute Intersection over Union between two boxes (cx, cy, w, h format).
static float compute_iou(const float lbox[4], const float rbox[4]) {
    float left = std::max(lbox[0] - lbox[2] / 2.0f, rbox[0] - rbox[2] / 2.0f);
    float right = std::min(lbox[0] + lbox[2] / 2.0f, rbox[0] + rbox[2] / 2.0f);
    float top = std::max(lbox[1] - lbox[3] / 2.0f, rbox[1] - rbox[3] / 2.0f);
    float bottom = std::min(lbox[1] + lbox[3] / 2.0f, rbox[1] + rbox[3] / 2.0f);
    
    if (left >= right || top >= bottom) return 0.0f;
    
    float inter_area = (right - left) * (bottom - top);
    float union_area = lbox[2] * lbox[3] + rbox[2] * rbox[3] - inter_area;
    return inter_area / union_area;
}

// Merge detections from a flat array, apply confidence threshold and NMS.
std::vector<Detection> merge_inference_results(const std::vector<float>& raw_output, float conf_thresh, float nms_thresh) {
    std::vector<Detection> detections;
    
    if (raw_output.empty()) return detections;
    
    int num_dets = static_cast<int>(raw_output[0]);
    int det_size = 4 + 1; // bbox (4) + conf (1)
    
    for (int i = 0; i < num_dets && i < static_cast<int>((raw_output.size() - 1) / det_size); ++i) {
        int offset = 1 + i * det_size;
        Detection det;
        for (int j = 0; j < 4; ++j) {
            det.bbox[j] = raw_output[offset + j];
        }
        det.conf = raw_output[offset + 4];
        if (det.conf > conf_thresh) {
            detections.push_back(det);
        }
    }
    
    // Sort by confidence descending.
    std::sort(detections.begin(), detections.end(), [](const Detection& a, const Detection& b) {
        return a.conf > b.conf;
    });
    
    std::vector<Detection> result;
    for (size_t i = 0; i < detections.size(); ++i) {
        const Detection& current = detections[i];
        bool keep = true;
        // Compare against already kept boxes (those before i in sorted order).
        // Since we iterate in descending confidence, any earlier kept box has higher confidence.
        for (size_t j = 0; j < i; ++j) {
            // We only check boxes that were not suppressed by earlier boxes.
            // But we need to know which were kept. Use result vector as kept list.
            // Simpler: iterative removal approach as in classic NMS.
            // However, the simplest correct approach: maintain a keep flag per index.
            // But here, we use the result vector to store kept boxes.
        }
    }
    
    // Classic greedy NMS: iterate and erase overlapping boxes.
    std::vector<Detection> nms_result;
    std::vector<bool> suppressed(detections.size(), false);
    
    for (size_t i = 0; i < detections.size(); ++i) {
        if (suppressed[i]) continue;
        nms_result.push_back(detections[i]);
        for (size_t j = i + 1; j < detections.size(); ++j) {
            if (suppressed[j]) continue;
            if (compute_iou(detections[i].bbox, detections[j].bbox) > nms_thresh) {
                suppressed[j] = true;
            }
        }
    }
    
    return nms_result;
}

// The core task is to parse a flat array of floats that encodes a variable-length detection list, filter by confidence, and then apply greedy NMS. The first element of `raw_output` is the number of detections; subsequent elements are grouped in blocks of 5 floats: [center_x, center_y, width, height, confidence]. The output must preserve only detections with confidence > `conf_thresh`, and then perform NMS: sort candidates by confidence descending, greedily select the highest-confidence remaining detection, remove any other detection whose IoU with it exceeds `nms_thresh`, and repeat. IoU is computed from bounding boxes in the format `[cx, cy, w, h]` (center coordinates and dimensions). The algorithm must handle edge cases: zero detections, all detections filtered by confidence, and degenerate boxes with zero width/height (IoU returns 0 if no overlap). Time complexity is O(N log N) for sorting plus O(N²) for NMS in the worst case (all detections overlap), where N is the number of detections passing the confidence threshold. Space complexity is O(N) for storing the filtered detections and the result.
