// Write a C++ function that takes a vector of building tuples, where each tuple is `{left, right, height}` representing a rectangle on the x-axis from `left` to `right` (exclusive of `right`) with a given height, and returns a vector of `pair<int,int>` representing the skyline. The skyline is a list of points `(x, height)` where the visible height changes, with each point marking the start of a new height in ascending x-order. If multiple buildings overlap, only the tallest height at each x-position is visible. The input may contain non-overlapping, overlapping, or identical buildings, and the coordinates are non-negative integers. The function must handle an empty input by returning an empty skyline. For example, given `{{0, 15, 3}, {4, 11, 5}, {19, 23, 4}}`, the output should be `{(0,3), (4,5), (11,3), (15,0), (19,4), (23,0)}`, where the ground level is height 0 at the end of each building. Ensure the output is sorted by x-coordinate.

#include <cassert>
#include <vector>
#include <utility>

// The solution function is assumed to be defined above.
int main() {
    // Example from the problem.
    std::vector<std::vector<int>> buildings1 = {{0, 15, 3}, {4, 11, 5}, {19, 23, 4}};
    std::vector<std::pair<int,int>> expected1 = {{0,3}, {4,5}, {11,3}, {15,0}, {19,4}, {23,0}};
    assert(findSkyline(buildings1) == expected1);

    // Empty input.
    assert(findSkyline({}).empty());

    // Single building.
    std::vector<std::vector<int>> buildings2 = {{1, 5, 10}};
    std::vector<std::pair<int,int>> expected2 = {{1,10}, {5,0}};
    assert(findSkyline(buildings2) == expected2);

    // Two non-overlapping buildings.
    std::vector<std::vector<int>> buildings3 = {{0, 2, 3}, {3, 5, 4}};
    std::vector<std::pair<int,int>> expected3 = {{0,3}, {2,0}, {3,4}, {5,0}};
    assert(findSkyline(buildings3) == expected3);

    // Overlapping with same height.
    std::vector<std::vector<int>> buildings4 = {{0, 5, 2}, {2, 7, 2}};
    std::vector<std::pair<int,int>> expected4 = {{0,2}, {7,0}};
    assert(findSkyline(buildings4) == expected4);

    // Overlapping with nested buildings.
    std::vector<std::vector<int>> buildings5 = {{0, 10, 3}, {2, 8, 5}, {4, 6, 7}};
    std::vector<std::pair<int,int>> expected5 = {{0,3}, {2,5}, {4,7}, {6,5}, {8,3}, {10,0}};
    assert(findSkyline(buildings5) == expected5);

    // Multiple starts and ends at same x coordinate.
    std::vector<std::vector<int>> buildings6 = {{0, 4, 1}, {0, 2, 2}, {2, 5, 2}};
    std::vector<std::pair<int,int>> expected6 = {{0,2}, {5,0}};
    assert(findSkyline(buildings6) == expected6);

    // End at same x as another start.
    std::vector<std::vector<int>> buildings7 = {{0, 3, 5}, {3, 6, 4}};
    std::vector<std::pair<int,int>> expected7 = {{0,5}, {3,4}, {6,0}};
    assert(findSkyline(buildings7) == expected7);

    // All zero-height buildings (should just return ground level).
    std::vector<std::vector<int>> buildings8 = {{0, 2, 0}, {1, 3, 0}};
    std::vector<std::pair<int,int>> expected8 = {{0,0}, {3,0}};
    assert(findSkyline(buildings8) == expected8);

    return 0;
}

#include <vector>
#include <map>
#include <algorithm>
#include <utility>

// Compute the skyline from a list of buildings represented as {left, right, height}.
std::vector<std::pair<int,int>> findSkyline(const std::vector<std::vector<int>>& buildings) {
    if (buildings.empty()) {
        return {};
    }

    // Events: {x, height, type} where type=1 for start, type=0 for end.
    std::vector<std::vector<int>> events;
    events.reserve(buildings.size() * 2);
    for (const auto& b : buildings) {
        events.push_back({b[0], b[2], 1});
        events.push_back({b[1], b[2], 0});
    }
    // Sort by x-coordinate (if equal, order is irrelevant for correctness).
    std::sort(events.begin(), events.end(),
              [](const std::vector<int>& a, const std::vector<int>& b) {
                  return a[0] < b[0];
              });

    // Multiset of active heights using a map: height -> count.
    std::map<int, int> activeHeights;
    activeHeights[0] = 1;  // Ground level.

    std::vector<std::pair<int,int>> skyline;
    int prevHeight = 0;

    for (const auto& event : events) {
        int x = event[0];
        int h = event[1];
        bool isStart = (event[2] == 1);

        if (isStart) {
            ++activeHeights[h];
        } else {
            --activeHeights[h];
            if (activeHeights[h] == 0) {
                activeHeights.erase(h);
            }
        }

        int currMax = activeHeights.rbegin()->first;
        if (currMax != prevHeight) {
            skyline.emplace_back(x, currMax);
            prevHeight = currMax;
        }
    }

    return skyline;
}

// The solution uses a sweep-line algorithm over the x-axis, processing all building start and end events sorted by their x-coordinate. For each building, we create two events: a "start" event at the left coordinate with the building's height (type 1) and an "end" event at the right coordinate with the building's height (type 0). We sort all events by x-coordinate; if multiple events share the same x, the order among them does not affect correctness because we process all events at that x before checking height changes. We maintain a multiset (implemented using a `std::map<int,int>` that counts occurrences of each height) to track the active building heights. Initially, the map contains height 0 with count 1 to represent ground. For each event, if it's a start, we increment the count for that height; if it's an end, we decrement the count and remove the height if the count becomes zero. After processing an event, we get the current maximum active height via the last key in the map (`rbegin()`). If this maximum differs from the previously recorded height, we append `(point_x, current_max_height)` to the skyline and update the previous height. Since events are processed in x-order, the output is naturally sorted by x. Edge cases: the ground height 0 is always present, so after the last building ends, a point with height 0 is emitted at the rightmost x. If the input is empty, the function returns an empty vector. Time complexity is O(n log n) due to sorting and map operations, where n is the number of buildings. Space complexity is O(n) for storing events and the map.
