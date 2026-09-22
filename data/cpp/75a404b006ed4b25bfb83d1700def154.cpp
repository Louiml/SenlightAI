// Write a C++ function `int unionArea(const vector<vector<int>>& rectangles)` that takes a list of axis-aligned rectangles, each represented as `[x1, y1, x2, y2]` where `(x1, y1)` is the bottom-left corner and `(x2, y2)` is the top-right corner (with `x1 < x2` and `y1 < y2`), and returns the total area covered by the union of all rectangles, modulo `1'000'000'007`. Coordinates are integers in a continuous plane. Rectangles may overlap arbitrarily, and the list may be empty (in which case the answer is `0`). The function must compute the union area efficiently, handling up to 1000 rectangles with coordinates up to 10^9. The solution must not use any external libraries beyond the standard C++ library, and must be self-contained.
// The problem is a classic "rectangle union area" solved using a sweep line algorithm over the x-axis. The idea is to process events at each unique x-coordinate where a rectangle starts or ends. At any given x-position, we maintain a list of active y-intervals (the vertical spans of rectangles that cover that x). The height of the union of these intervals is the total y-length covered by any active rectangle at that x. The area contribution between two consecutive x-events is the width (difference in x) times that height. 
//
// **Algorithm steps:**
// 1. For each rectangle `[x1, y1, x2, y2]`, create two events: a "start" event at `x1` with interval `[y1, y2]`, and an "end" event at `x2` with the same interval.
// 2. Sort all events by x-coordinate. If multiple events have the same x, the order between start and end does not affect correctness, as long as we process all events at that x before computing the next width (since the height at that exact x is the same regardless of ordering).
// 3. Iterate through the sorted events. Before processing events at a new x, compute the width from the previous x and multiply by the current active height (from the list of y-intervals) and add to the answer modulo 1e9+7.
// 4. After processing the width, handle the event: if it's a start, add the y-interval; if it's an end, remove the y-interval. For simplicity, we can maintain a vector of pairs (y1, y2) and after each addition sort it, and for removal use `find` to erase. Since the number of events is small (2 per rectangle), this is acceptable.
// 5. The function `getHeight` computes the union length of the sorted y-intervals: iterate through them, keep track of the current covered end, and add only the non-overlapping segments.
//
// **Edge cases:** 
// - Empty input returns 0.
// - Rectangles that touch at an edge (e.g., `[0,0,1,1]` and `[1,0,2,1]`) – they share a boundary, but the area is simply sum of widths, and the sweep handles it because the height at x=1 is computed correctly.
// - Integer overflow: use `long long` for intermediate products and sums.
// - The modulo is applied after each multiplication and addition to keep values within range.
//
// **Time complexity:** Sorting events: O(n log n). For each event, we do at most O(n) work for sorting/erasing in the y-interval list (since we re-sort after each insertion and use linear find for removal). Thus, O(n^2) in the worst case for n rectangles. With n up to 1000, this is fine. **Space complexity:** O(n) for storing events and the y-interval list.
#include <vector>
#include <algorithm>
#include <utility>

using namespace std;

struct RectEvent {
    int x;
    int y1, y2;
    char type; // 's' for start, 'e' for end
    bool operator<(const RectEvent& other) const {
        return x < other.x;
    }
};

int getHeight(const vector<pair<int,int>>& yPairs) {
    int height = 0;
    int prevY = 0;
    for (const auto& p : yPairs) {
        prevY = max(prevY, p.first);
        if (p.second > prevY) {
            height += p.second - prevY;
            prevY = p.second;
        }
    }
    return height;
}

int unionArea(const vector<vector<int>>& rectangles) {
    constexpr int MOD = 1000000007;
    vector<RectEvent> events;
    events.reserve(rectangles.size() * 2);
    
    for (const auto& r : rectangles) {
        // r = [x1, y1, x2, y2]
        events.push_back({r[0], r[1], r[3], 's'});
        events.push_back({r[2], r[1], r[3], 'e'});
    }
    
    sort(events.begin(), events.end());
    
    long long ans = 0;
    int prevX = 0;
    vector<pair<int,int>> activeIntervals;
    
    for (const auto& e : events) {
        if (e.x > prevX) {
            int width = e.x - prevX;
            ans = (ans + (long long)width * getHeight(activeIntervals)) % MOD;
            prevX = e.x;
        }
        if (e.type == 's') {
            activeIntervals.push_back({e.y1, e.y2});
            sort(activeIntervals.begin(), activeIntervals.end());
        } else { // 'e'
            auto it = find(activeIntervals.begin(), activeIntervals.end(), make_pair(e.y1, e.y2));
            if (it != activeIntervals.end()) {
                activeIntervals.erase(it);
            }
        }
    }
    
    return (int)(ans % MOD);
}
#include <cassert>
#include <vector>
using namespace std;

int main() {
    // Empty input
    assert(unionArea({}) == 0);
    
    // Single rectangle
    assert(unionArea({{0,0,2,2}}) == 4);
    
    // Two non-overlapping rectangles
    assert(unionArea({{0,0,1,1}, {2,2,3,3}}) == 2);
    
    // Two overlapping rectangles (area = 3)
    assert(unionArea({{0,0,2,2}, {1,1,3,3}}) == 7);
    
    // Rectangles sharing an edge (union area = 2)
    assert(unionArea({{0,0,1,1}, {1,0,2,1}}) == 2);
    
    // One inside another
    assert(unionArea({{0,0,5,5}, {1,1,2,2}}) == 25);
    
    // Large coordinates with modulo (2000000000 * 1 = 2000000000 % 1e9+7 = 999999993)
    assert(unionArea({{0,0,2000000000,1}}) == 999999993);
    
    // Negative coordinates (area = 4)
    assert(unionArea({{-2,-2,0,0}}) == 4);
    
    // Multiple overlapping complex shape
    assert(unionArea({{0,0,3,3}, {1,1,4,4}, {2,2,5,5}}) == 17);
    
    // Rectangles touching at a corner
    assert(unionArea({{0,0,1,1}, {1,1,2,2}}) == 2);
    
    return 0;
}
