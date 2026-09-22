/*
Write a C++ function `long long totalCost(const std::vector<std::pair<int,int>>& ranges)` that receives a list of 2D axis-aligned rectangular ranges (each defined by `(x1,y1,x2,y2)` with `0 ≤ x1 ≤ x2 ≤ 10^9` and `0 ≤ y1 ≤ y2 ≤ 10^9`). The function must compute the **total area** covered by the union of all these rectangles. Each rectangle is inclusive of its left and bottom edges, exclusive of its right and top edges, i.e., it covers the continuous set of points `[x1, x2) × [y1, y2)`. The total area is the measure (in square units) of the union of all rectangles. The function should handle overlapping rectangles, duplicate rectangles, and arbitrary ordering. It must work efficiently for up to 10^5 rectangles.
*/
#include <vector>
#include <algorithm>
#include <cstdint>
#include <map>

// Segment tree to maintain the total length of union of active y-intervals.
class SegmentTree {
    std::vector<int> count;
    std::vector<long long> length;
    std::vector<long long> ys;
    int n;

    void update(int node, int l, int r, int ql, int qr, int delta) {
        if (ql >= r || qr <= l) return;
        if (ql <= l && r <= qr) {
            count[node] += delta;
        } else {
            int mid = (l + r) / 2;
            update(node * 2, l, mid, ql, qr, delta);
            update(node * 2 + 1, mid, r, ql, qr, delta);
        }
        if (count[node] > 0) {
            length[node] = ys[r] - ys[l];
        } else if (r - l == 1) {
            length[node] = 0;
        } else {
            length[node] = length[node * 2] + length[node * 2 + 1];
        }
    }

public:
    SegmentTree(const std::vector<long long>& y_coords) : ys(y_coords), n(y_coords.size() - 1) {
        count.assign(4 * n, 0);
        length.assign(4 * n, 0);
    }

    void add(long long y1, long long y2, int delta) {
        int l = std::lower_bound(ys.begin(), ys.end(), y1) - ys.begin();
        int r = std::lower_bound(ys.begin(), ys.end(), y2) - ys.begin();
        if (l < r) {
            update(1, 0, n, l, r, delta);
        }
    }

    long long total_length() const {
        return length[1];
    }
};

// Compute the total area of union of axis-aligned rectangles.
long long totalCost(const std::vector<std::pair<int,int>>& ranges) {
    if (ranges.empty()) return 0;

    // Decompose each pair into x1,y1,x2,y2
    struct Rect { long long x1, y1, x2, y2; };
    std::vector<Rect> rects;
    rects.reserve(ranges.size());
    for (const auto& p : ranges) {
        // Assume p.first is the x-coordinate (x1) and p.second is y-coordinate (y1),
        // but we need x2,y2 as well. The problem statement is ambiguous, so we interpret
        // each pair as a width and height? Wait, we need a proper representation.
        // For correctness, we will reinterpret: each pair represents (x1, y1),
        // and we assume x2 = x1+1, y2 = y1+1? That seems too small.
        // To make a meaningful task, we treat each pair as (x1, y1) and assume
        // the rectangle is a unit square? No, that is not interesting.
        // Given the snippet context, the task is about tuple ranges with x1,y1,x2,y2,
        // so we should accept a vector of tuples (x1,y1,x2,y2) but the function signature says pair.
        // To be self-contained, we will change the signature to accept a vector of 4-ints.
    }
    // Due to the ambiguity, we will redesign the function to accept a vector of
    // structs with x1,y1,x2,y2. Since the problem statement says "rectangular ranges",
    // we will assume the input is a vector of tuples of four ints.
    // For the sake of this solution, we will define a helper struct.

    // The solution below assumes we have a vector of Rect.
    // To match the required signature, we will define:
    // using Rect = std::tuple<int,int,int,int>;
    // and the function will take std::vector<std::tuple<int,int,int,int>>.

    // For brevity, I'll provide a correct implementation for the intended problem.
    // Please interpret the function as taking a vector of rectangles with four coordinates.
}

// For the final solution, we will provide a clear implementation.
// Below is the correct solution for the intended problem:

struct Rect {
    long long x1, y1, x2, y2;
};

long long totalAreaUnion(const std::vector<Rect>& rects) {
    if (rects.empty()) return 0;
    struct Event {
        long long x;
        long long y1, y2;
        int delta;
        bool operator<(const Event& other) const {
            return x < other.x;
        }
    };
    std::vector<Event> events;
    std::vector<long long> ys;
    for (const auto& r : rects) {
        if (r.x1 >= r.x2 || r.y1 >= r.y2) continue; // zero area
        events.push_back({r.x1, r.y1, r.y2, +1});
        events.push_back({r.x2, r.y1, r.y2, -1});
        ys.push_back(r.y1);
        ys.push_back(r.y2);
    }
    if (events.empty()) return 0;
    std::sort(events.begin(), events.end());
    std::sort(ys.begin(), ys.end());
    ys.erase(std::unique(ys.begin(), ys.end()), ys.end());

    SegmentTree st(ys);
    long long prev_x = events[0].x;
    long long area = 0;
    size_t i = 0;
    while (i < events.size()) {
        long long cur_x = events[i].x;
        area += (cur_x - prev_x) * st.total_length();
        // Process all events at this x
        while (i < events.size() && events[i].x == cur_x) {
            st.add(events[i].y1, events[i].y2, events[i].delta);
            ++i;
        }
        prev_x = cur_x;
    }
    return area;
}
int main() {
    using R = Rect;
    // Single rectangle 2x3
    assert(totalAreaUnion({R{0,0,2,3}}) == 6);
    // Two disjoint rectangles
    assert(totalAreaUnion({R{0,0,1,1}, R{2,2,3,3}}) == 2);
    // Overlapping rectangles
    assert(totalAreaUnion({R{0,0,2,2}, R{1,1,3,3}}) == 7);
    // Contained rectangle
    assert(totalAreaUnion({R{0,0,5,5}, R{1,1,2,2}}) == 25);
    // Empty input
    assert(totalAreaUnion({}) == 0);
    // Zero-area rect ignored
    assert(totalAreaUnion({R{0,0,0,1}, R{0,0,1,1}}) == 1);
    // Complex union
    assert(totalAreaUnion({R{1,1,4,4}, R{2,2,5,5}, R{4,1,6,3}}) == 18);
    // Multiple events same x
    assert(totalAreaUnion({R{0,0,2,1}, R{0,2,2,3}}) == 4);
    // Large coordinates
    assert(totalAreaUnion({R{0,0,1000000000,1000000000}}) == 1000000000000000000LL);
}
// The standard approach is to use a sweep line algorithm along one axis (say x). First, for each rectangle, we create two vertical events: at x = x1 we add the interval [y1, y2) with a `+1` delta, and at x = x2 we remove it with a `-1` delta. We sort all events by their x-coordinate. Then we sweep from left to right. Between consecutive unique x-coordinates, the active set of y-intervals remains constant, so the area contributed is `(current_x - previous_x) * total_active_y_length`. To maintain the union length of active y-intervals dynamically, we use a segment tree over the compressed y-coordinates. For each unique y-coordinate between consecutive events, the segment tree tracks how many active intervals cover that segment, and the total length of segments with positive coverage. When we process an event at x, we update the segment tree with the delta for that y-interval, then compute the covered length. The area contributed is `(next_x - current_x) * covered_length`. Edge cases include empty input (return 0), zero-width or zero-height rectangles (contribute 0 area), and multiple events at the same x (process all before computing area). Time complexity is O(N log N) for sorting events and O(N log M) for segment tree updates, where M is the number of unique y-coordinates (≤ 2N). Space complexity is O(N).
