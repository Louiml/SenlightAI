// Write a C++ function named `mergeTrackedObjects` that takes four inputs: a `std::vector<cv::Rect>` of bounding boxes from a tracking algorithm, a `std::vector<int32_t>` of track IDs, a `std::vector<uint64_t>` of timestamps, and a `std::vector<int>` of class labels. The function should merge consecutive entries that share the same track ID and class label, but only if their timestamps differ by exactly 1 (indicating successive frames). For each group of consecutive matching entries, output a single merged entry consisting of the union of all bounding boxes (the smallest rectangle that contains all of them), the shared track ID, the largest timestamp in the group, and the shared class label. The function returns a `std::tuple<std::vector<cv::Rect>, std::vector<int32_t>, std::vector<uint64_t>, std::vector<int>>` containing the merged results in order. If an input vector is empty, return empty vectors. Assume all input vectors have equal length, and that they are ordered by time (timestamps are non-decreasing). Handle cases where the same track ID appears with different class labels or with non-consecutive timestamps as separate groups.

// The solution iterates through the input vectors while maintaining a current group. For each new entry, we check if it can be merged with the current group: the track ID must equal the current group's track ID, the class label must equal the current group's label, and the new timestamp must equal the current group's last timestamp + 1. If all conditions hold, we expand the bounding box to include the new box (using `cv::Rect`'s union logic: we compute the minimum top-left and maximum bottom-right). We also update the timestamp to the current entry's timestamp. If not, we finalize the current group by adding its bounding box, ID, timestamp, and label to the output vectors, then start a new group with the current entry. Edge cases: empty input returns empty outputs; a single entry forms its own group; consecutive entries with same ID but different labels or gaps in timestamps are not merged. Time complexity is O(n) where n is the number of entries, as each entry is processed once. Space complexity is O(n) for the output vectors, plus O(1) auxiliary space for the current group state. Note that `cv::Rect` union can be computed manually: `rect = cv::Rect(min(r.x, other.x), min(r.y, other.y), max(r.x+r.width, other.x+other.width) - min(r.x, other.x), max(r.y+r.height, other.y+other.height) - min(r.y, other.y))`.

#include <opencv2/core/types.hpp>
#include <vector>
#include <tuple>
#include <cstdint>

// Merge consecutive tracked objects that share the same ID and class label,
// and whose timestamps differ by exactly 1, into a single bounding box union.
std::tuple<std::vector<cv::Rect>, std::vector<int32_t>, std::vector<uint64_t>, std::vector<int>>
mergeTrackedObjects(const std::vector<cv::Rect>& boxes,
                    const std::vector<int32_t>& track_ids,
                    const std::vector<uint64_t>& timestamps,
                    const std::vector<int>& class_labels) {
    std::vector<cv::Rect> merged_boxes;
    std::vector<int32_t> merged_ids;
    std::vector<uint64_t> merged_timestamps;
    std::vector<int> merged_labels;

    if (boxes.empty()) {
        return {merged_boxes, merged_ids, merged_timestamps, merged_labels};
    }

    cv::Rect current_box = boxes[0];
    int32_t current_id = track_ids[0];
    uint64_t current_timestamp = timestamps[0];
    int current_label = class_labels[0];

    for (size_t i = 1; i < boxes.size(); ++i) {
        bool same_id = (track_ids[i] == current_id);
        bool same_label = (class_labels[i] == current_label);
        bool consecutive_timestamp = (timestamps[i] > current_timestamp) && 
                                     (timestamps[i] - current_timestamp == 1);

        if (same_id && same_label && consecutive_timestamp) {
            // Expand current_box to include boxes[i]
            int new_x = std::min(current_box.x, boxes[i].x);
            int new_y = std::min(current_box.y, boxes[i].y);
            int new_right = std::max(current_box.x + current_box.width, boxes[i].x + boxes[i].width);
            int new_bottom = std::max(current_box.y + current_box.height, boxes[i].y + boxes[i].height);
            current_box = cv::Rect(new_x, new_y, new_right - new_x, new_bottom - new_y);
            current_timestamp = timestamps[i];
        } else {
            merged_boxes.push_back(current_box);
            merged_ids.push_back(current_id);
            merged_timestamps.push_back(current_timestamp);
            merged_labels.push_back(current_label);

            current_box = boxes[i];
            current_id = track_ids[i];
            current_timestamp = timestamps[i];
            current_label = class_labels[i];
        }
    }

    merged_boxes.push_back(current_box);
    merged_ids.push_back(current_id);
    merged_timestamps.push_back(current_timestamp);
    merged_labels.push_back(current_label);

    return {merged_boxes, merged_ids, merged_timestamps, merged_labels};
}

#include <cassert>
#include <iostream>

int main() {
    // Test case 1: Empty input
    {
        std::vector<cv::Rect> boxes;
        std::vector<int32_t> ids;
        std::vector<uint64_t> ts;
        std::vector<int> labels;
        auto result = mergeTrackedObjects(boxes, ids, ts, labels);
        assert(std::get<0>(result).empty());
        assert(std::get<1>(result).empty());
        assert(std::get<2>(result).empty());
        assert(std::get<3>(result).empty());
    }

    // Test case 2: Single entry
    {
        std::vector<cv::Rect> boxes = {cv::Rect(10, 20, 30, 40)};
        std::vector<int32_t> ids = {1};
        std::vector<uint64_t> ts = {100};
        std::vector<int> labels = {0};
        auto result = mergeTrackedObjects(boxes, ids, ts, labels);
        assert(std::get<0>(result).size() == 1);
        assert(std::get<0>(result)[0] == cv::Rect(10, 20, 30, 40));
        assert(std::get<1>(result)[0] == 1);
        assert(std::get<2>(result)[0] == 100);
        assert(std::get<3>(result)[0] == 0);
    }

    // Test case 3: Consecutive frames merge into union
    {
        std::vector<cv::Rect> boxes = {cv::Rect(0, 0, 10, 10), cv::Rect(5, 5, 10, 10), cv::Rect(2, 3, 8, 8)};
        std::vector<int32_t> ids = {7, 7, 7};
        std::vector<uint64_t> ts = {1, 2, 3};
        std::vector<int> labels = {2, 2, 2};
        auto result = mergeTrackedObjects(boxes, ids, ts, labels);
        assert(std::get<0>(result).size() == 1);
        assert(std::get<0>(result)[0] == cv::Rect(0, 0, 15, 15)); // union of all three
        assert(std::get<1>(result)[0] == 7);
        assert(std::get<2>(result)[0] == 3);
        assert(std::get<3>(result)[0] == 2);
    }

    // Test case 4: Timestamp gap breaks merge group
    {
        std::vector<cv::Rect> boxes = {cv::Rect(0, 0, 5, 5), cv::Rect(10, 10, 5, 5)};
        std::vector<int32_t> ids = {3, 3};
        std::vector<uint64_t> ts = {1, 3};
        std::vector<int> labels = {1, 1};
        auto result = mergeTrackedObjects(boxes, ids, ts, labels);
        assert(std::get<0>(result).size() == 2);
        assert(std::get<0>(result)[0] == cv::Rect(0, 0, 5, 5));
        assert(std::get<0>(result)[1] == cv::Rect(10, 10, 5, 5));
        assert(std::get<1>(result)[0] == 3 && std::get<1>(result)[1] == 3);
        assert(std::get<2>(result)[0] == 1 && std::get<2>(result)[1] == 3);
    }

    // Test case 5: Different labels break group
    {
        std::vector<cv::Rect> boxes = {cv::Rect(0, 0, 5, 5), cv::Rect(10, 10, 5, 5)};
        std::vector<int32_t> ids = {5, 5};
        std::vector<uint64_t> ts = {1, 2};
        std::vector<int> labels = {0, 1};
        auto result = mergeTrackedObjects(boxes, ids, ts, labels);
        assert(std::get<0>(result).size() == 2);
    }

    // Test case 6: Repeated ID with time gap and different groups
    {
        std::vector<cv::Rect> boxes = {cv::Rect(0, 0, 2, 2), cv::Rect(1, 1, 2, 2), cv::Rect(10, 10, 2, 2), cv::Rect(12, 12, 2, 2)};
        std::vector<int32_t> ids = {1, 1, 1, 1};
        std::vector<uint64_t> ts = {1, 2, 5, 6};
        std::vector<int> labels = {0, 0, 0, 0};
        auto result = mergeTrackedObjects(boxes, ids, ts, labels);
        assert(std::get<0>(result).size() == 2);
        assert(std::get<0>(result)[0] == cv::Rect(0, 0, 3, 3));
        assert(std::get<0>(result)[1] == cv::Rect(10, 10, 4, 4));
        assert(std::get<2>(result)[0] == 2);
        assert(std::get<2>(result)[1] == 6);
    }

    std::cout << "All tests passed." << std::endl;
    return 0;
}
