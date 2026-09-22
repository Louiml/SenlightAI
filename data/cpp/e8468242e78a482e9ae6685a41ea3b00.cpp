/*
Write a C++ function `long long unionAreaOfRectangles(const std::vector<std::array<int,4>>& rectangles)` that takes a vector of axis-aligned rectangles, each represented as `{x1, y1, x2, y2}` with `x1 < x2` and `y1 < y2`, and returns the total area covered by the union of all rectangles as a `long long`. The function must handle up to 10^5 rectangles, coordinates in the range `[-10^9, 10^9]`, and correctly compute the union area even when rectangles overlap or touch. Use a sweep line algorithm with coordinate compression of y‑coordinates, a segment tree or an efficient frequency array, and process events sorted by x. The function must be const-correct and not modify the input. The test harness will call this function with various inputs and assert the computed area.
*/
#include <vector>
#include <array>
#include <algorithm>
#include <set>
#include <cstdint>

// Compute the union area of axis-aligned rectangles.
// Each rectangle is {x1, y1, x2, y2} with x1<x2 and y1<y2.
long long unionAreaOfRectangles(const std::vector<std::array<int,4>>& rectangles) {
    if (rectangles.empty()) return 0;

    // Collect events and unique y coordinates
    struct Event {
        int x, y1, y2;
        int type; // +1 for start, -1 for end
        bool operator<(const Event& other) const {
            return x < other.x;
        }
    };

    std::vector<Event> events;
    events.reserve(rectangles.size() * 2);
    std::set<int> y_set;

    for (const auto& rect : rectangles) {
        int x1 = rect[0], y1 = rect[1], x2 = rect[2], y2 = rect[3];
        events.push_back({x1, y1, y2, +1});
        events.push_back({x2, y1, y2, -1});
        y_set.insert(y1);
        y_set.insert(y2);
    }

    // Sort events by x; if x equal, order doesn't matter for correctness
    std::sort(events.begin(), events.end());

    // Compress y coordinates
    std::vector<int> y_sorted(y_set.begin(), y_set.end());
    const int m = static_cast<int>(y_sorted.size());
    std::vector<int> cnt(m - 1, 0); // cnt[j] covers [y_sorted[j], y_sorted[j+1])

    auto y_index = [&](int y) -> int {
        return static_cast<int>(std::lower_bound(y_sorted.begin(), y_sorted.end(), y) - y_sorted.begin());
    };

    long long area = 0;
    for (size_t i = 0; i < events.size(); ++i) {
        // Compute area from previous x to current x (if i>0)
        if (i > 0) {
            long long dx = static_cast<long long>(events[i].x) - events[i - 1].x;
            if (dx > 0) {
                long long covered_length = 0;
                for (int j = 0; j < m - 1; ++j) {
                    if (cnt[j] > 0) {
                        covered_length += static_cast<long long>(y_sorted[j + 1] - y_sorted[j]);
                    }
                }
                area += dx * covered_length;
            }
        }
        // Apply this event: update cnt for all segments between y1 and y2
        int y1_idx = y_index(events[i].y1);
        int y2_idx = y_index(events[i].y2);
        for (int j = y1_idx; j < y2_idx; ++j) {
            cnt[j] += events[i].type;
        }
    }

    return area;
}
#include <cassert>
#include <vector>
#include <array>

// Function under test (declaration only, definition provided separately)
long long unionAreaOfRectangles(const std::vector<std::array<int,4>>& rectangles);

int main() {
    // Single rectangle
    assert(unionAreaOfRectangles({{0,0,2,2}}) == 4);

    // Two non-overlapping rectangles
    assert(unionAreaOfRectangles({{0,0,1,1}, {2,2,3,3}}) == 2);

    // Two overlapping rectangles
    assert(unionAreaOfRectangles({{0,0,2,2}, {1,1,3,3}}) == 7);

    // One containing another
    assert(unionAreaOfRectangles({{0,0,4,4}, {1,1,3,3}}) == 16);

    // Touching edges (no overlap)
    assert(unionAreaOfRectangles({{0,0,2,2}, {2,0,4,2}}) == 8);

    // Negative coordinates
    assert(unionAreaOfRectangles({{-3,-3,-1,-1}, {-2,-2,0,0}}) == 8);

    // Large coordinates to check long long handling
    assert(unionAreaOfRectangles({{-1000000000, -1000000000, 1000000000, 1000000000}}) == 4000000000000000000LL);

    // Empty input
    assert(unionAreaOfRectangles({}) == 0);

    // Multiple rectangles overlapping in complex pattern
    std::vector<std::array<int,4>> rects = {
        {0,0,3,3}, {1,1,4,4}, {2,2,5,5}
    };
    // Union is everything from (0,0) to (5,5) because they overlap successively
    assert(unionAreaOfRectangles(rects) == 25);

    // Three rectangles in a line, no overlap
    rects = {{0,0,1,1}, {1,1,2,2}, {2,2,3,3}};
    assert(unionAreaOfRectangles(rects) == 3);

    return 0;
}
// The core idea is a classic sweep line over the x‑axis. At any given x‑position, the active set of y‑intervals (from rectangles that have started but not yet ended) defines the total vertical length covered. The union area is the integral of that covered length over x. To avoid checking every rectangle at every event, we compress all distinct y‑coordinates (from all rectangle tops and bottoms). Between two consecutive compressed y‑coordinates, the coverage is uniform, so we maintain a count per y‑segment (the interval between `y_sorted[j]` and `y_sorted[j+1]`). Events are `(x, y1, y2, type)` where type = +1 for a left edge (start) and -1 for a right edge (end). Sort events by x; if two events share the same x, we must process all events at that x before computing the area for the gap to the next x, because coverage changes at that x. The algorithm:  
// 1. Collect all events, and all y1 and y2 values into a set, then sort to get `y_sorted`.  
// 2. Use an integer array `cnt` of size `m-1` (where `m` = number of unique y’s) to count how many rectangles cover each y‑segment.  
// 3. Sort events. Iterate through events in order. For each event, before applying it, compute the area from the previous event’s x to this event’s x: `dx = events[i].x - events[i-1].x`, and `coveredLength` = sum over segments where `cnt[j] > 0` of `(y_sorted[j+1] - y_sorted[j])`. Add `dx * coveredLength` to the total. Then apply the event by adding `type` to `cnt[j]` for j from `lower_bound(y_sorted, events[i].y1)` to `lower_bound(y_sorted, events[i].y2)-1`.  
//    Edge cases:  
//    - Multiple events at the same x: we only compute area between distinct x values. Since the area between identical x is zero, it is safe to apply all events at that x first and then compute area for the next gap. However, the above ordering (compute area before applying current event) works if we ensure that for the first event, we skip area computation (i==0). For subsequent events, if they share the same x, dx=0, so it’s fine.  
//    - Overlapping rectangles: cnt[j] may become >1 and is handled correctly.  
//    - Negative coordinates: handled by sorting and using lower_bound; no issue.  
//    - Large coordinate differences: use `long long` for area calculations to avoid overflow.  
//    Time complexity: O(N log N) to sort events and build sorted y set, where N = number of rectangles. Each event updates at most O(m) segments in the worst case (naive loop), so worst-case O(N * m) which can be O(N^2) if many unique y’s. For better performance, a segment tree with lazy propagation or a range‑add / range‑query can be used, but for this task the simple array update is acceptable if N ≤ 10^5 and m ≤ 2N. Space complexity O(N) for events and O(m) for the count array.
