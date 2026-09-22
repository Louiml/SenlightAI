// You are given a row of `N` boxes (numbered 1 through `N` from left to right), where each box initially contains a known number of birds (`arr_boxes[i]` for box `i`, using 0-based indexing internally). Then `K` shooting events occur. For each event, you are given a box index `box` and a bird position `bird` (1-based bird position within that box). When a shot hits the `bird`-th bird in that box, all birds to the left of that position (i.e., `bird-1` birds) fly to the box immediately to the left (if it exists), and all birds to the right of that position (i.e., `arr_boxes[box] - bird` birds) fly to the box immediately to the right (if it exists). The shot itself kills the bird at that exact position, so the box where the shot occurred becomes empty. If the shot box is the first box, no birds fly left; if it is the last box, no birds fly right. Write a C++ function `void simulateShots(std::vector<long long>& boxes, const std::vector<std::pair<int,int>>& shots)` that modifies the `boxes` vector in place according to these rules, given `shots` as pairs of 1-based box index and 1-based bird position. Your function should handle up to `500` boxes and any number of shots.
The solution directly simulates each shot in order. For each shot `(box, bird)`, convert the 1-based box index to 0-based index `i = box-1`. If `i` is the first box (`i==0`), then all birds to the right (i.e., `boxes[i]-bird`) are added to `boxes[i+1]`, and `boxes[i]` becomes 0. If `i` is the last box (`i == boxes.size()-1`), then all birds to the left (i.e., `bird-1`) are added to `boxes[i-1]`, and `boxes[i]` becomes 0. For a middle box, both transfers happen: add `bird-1` to `boxes[i-1]` and `boxes[i]-bird` to `boxes[i+1]`, then set `boxes[i]` to 0. No validation is needed because the problem guarantees that `bird` is between 1 and the current number of birds in that box (since shots happen after previous modifications). The time complexity is `O(K)`, because each shot performs constant-time operations. Space complexity is `O(1)` beyond the input storage. Edge cases include shots on the first or last box, and the possibility that a box becomes empty but later receives birds from adjacent boxes.
#include <vector>
#include <utility>

// Simulate the bird-shooting process on the boxes.
// boxes: vector of long long, where boxes[i] is the number of birds in box i (0-based).
// shots: vector of pairs (box, bird), both 1-based. bird is the position within that box.
// The function modifies boxes in place.
void simulateShots(std::vector<long long>& boxes, const std::vector<std::pair<int, int>>& shots) {
    int n = static_cast<int>(boxes.size());
    for (const auto& shot : shots) {
        int box = shot.first;  // 1-based
        long long bird = shot.second; // 1-based bird position
        int i = box - 1; // convert to 0-based

        if (i == 0) {
            // First box: no left neighbor, all birds to the right fly to box 2.
            boxes[i + 1] += (boxes[i] - bird);
            boxes[i] = 0;
        } else if (i == n - 1) {
            // Last box: no right neighbor, all birds to the left fly to box N-1.
            boxes[i - 1] += (bird - 1);
            boxes[i] = 0;
        } else {
            // Middle box: both sides receive birds.
            boxes[i + 1] += (boxes[i] - bird);
            boxes[i - 1] += (bird - 1);
            boxes[i] = 0;
        }
    }
}
#include <cassert>
#include <vector>
#include <utility>

// The solution function is declared above (by inclusion or copy).
// For testing, we include the function here (simplified duplicate for clarity).
void simulateShots(std::vector<long long>& boxes, const std::vector<std::pair<int, int>>& shots) {
    int n = static_cast<int>(boxes.size());
    for (const auto& shot : shots) {
        int box = shot.first;
        long long bird = shot.second;
        int i = box - 1;
        if (i == 0) {
            boxes[i + 1] += (boxes[i] - bird);
            boxes[i] = 0;
        } else if (i == n - 1) {
            boxes[i - 1] += (bird - 1);
            boxes[i] = 0;
        } else {
            boxes[i + 1] += (boxes[i] - bird);
            boxes[i - 1] += (bird - 1);
            boxes[i] = 0;
        }
    }
}

int main() {
    // Test 1: Single shot in the middle.
    std::vector<long long> boxes1 = {10, 20, 30};
    std::vector<std::pair<int,int>> shots1 = {{2, 5}};
    simulateShots(boxes1, shots1);
    assert(boxes1 == std::vector<long long>({15, 0, 45}));

    // Test 2: Shot on first box.
    std::vector<long long> boxes2 = {7, 3};
    std::vector<std::pair<int,int>> shots2 = {{1, 4}};
    simulateShots(boxes2, shots2);
    assert(boxes2 == std::vector<long long>({0, 6}));

    // Test 3: Shot on last box.
    std::vector<long long> boxes3 = {5, 9};
    std::vector<std::pair<int,int>> shots3 = {{2, 3}};
    simulateShots(boxes3, shots3);
    assert(boxes3 == std::vector<long long>({7, 0}));

    // Test 4: Multiple shots sequentially.
    std::vector<long long> boxes4 = {1, 1, 1};
    std::vector<std::pair<int,int>> shots4 = {{2, 1}, {1, 1}};
    simulateShots(boxes4, shots4);
    // After first shot: box2 empties, left gets 0, right gets 0 -> {1,0,1}
    // After second shot on box1: box1 empties, right gets 0 -> {0,0,1}
    assert(boxes4 == std::vector<long long>({0, 0, 1}));

    // Test 5: Large values and middle shot.
    std::vector<long long> boxes5 = {1000000000000LL, 1LL, 1LL};
    std::vector<std::pair<int,int>> shots5 = {{1, 1}};
    simulateShots(boxes5, shots5);
    assert(boxes5 == std::vector<long long>({0, 1000000000000LL, 1LL}));

    // Test 6: Shot with bird at the last position (all left birds fly left).
    std::vector<long long> boxes6 = {10, 20};
    std::vector<std::pair<int,int>> shots6 = {{2, 20}};
    simulateShots(boxes6, shots6);
    assert(boxes6 == std::vector<long long>({29, 0}));

    return 0;
}
