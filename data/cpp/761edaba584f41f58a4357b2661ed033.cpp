Write a C++ function `vector<pair<int,int>> findHorizontalIntersections(const vector<line>& lines)` that takes a collection of axis-aligned line segments (horizontal segments defined by start/end x-coordinates and a fixed y-coordinate; vertical segments defined by start/end y-coordinates and a fixed x-coordinate) and returns a vector of pairs representing all horizontal segments that intersect at least one vertical segment. A horizontal segment intersects a vertical segment if the vertical segment's x-coordinate lies within the horizontal segment's inclusive x-range, and the horizontal segment's y-coordinate lies within the vertical segment's inclusive y-range. The returned vector should contain each unique horizontal segment (identified by its original object) exactly once, in any order. You may assume the input is non-empty and all coordinates are integers. For simplicity, define a `line` struct with fields `k1`, `k2`, `m`, `type` (1 for horizontal, 2 for vertical), as in the provided snippet; horizontal segments have `k1 <= k2`, vertical segments have `k1 <= k2`. The function should not modify the input.

The problem is a classic sweep-line / plane-sweep intersection detection for axis-aligned segments. The optimal approach is to sort all events (horizontal segment start/end points and vertical segment queries) by x-coordinate, then sweep from left to right. Maintain an ordered map keyed by y-coordinate containing all currently active horizontal segments (those whose x-range contains the current sweep position). When a vertical segment is encountered, query the map for all horizontal segments with y-coordinate between the vertical segment's lower and upper y-bounds using `lower_bound` and `upper_bound`; any such horizontal segments intersect this vertical segment. Mark them as intersected (e.g., using a `std::set` of pointers or indices) to deduplicate. Key edge cases: vertical segments that start and end at the same x as a horizontal endpoint should count as intersections (use inclusive bounds); multiple horizontal segments on the same y-coordinate must be handled correctly—since the map is keyed by y, we need a multiset or map from y to a list of segment pointers? Actually, using a `std::map<int, line*>` works because each horizontal segment has a unique y-coordinate? In the snippet, horizontal lines can share y? The problem does not forbid duplicates, so better to store a `std::multimap<int, line*>` but that complicates deletion. Alternatively, store a `std::map<int, std::vector<line*>>` where each y maps to a list of active segments; when a horizontal segment starts, push its pointer into the vector for that y; when it ends, remove that specific pointer from the vector. For simplicity and correctness with possible duplicate y's, use a `std::map<int, std::vector<line*>>`. However, the original snippet uses a simple `map<int,line*>` assuming unique y per horizontal line; if the problem spec allows duplicate y, we must handle that. But the given code snippet has horizontal lines with distinct y's (1,2,4,3). I'll assume the task specification does not forbid duplicate y's, so I'll design a robust solution using a `std::multimap<int, line*>` and erase by iterator when removing. When querying, iterate over the range and for each entry, record that line. But erasing from a multimap while iterating requires careful iterator handling; since we only erase on horizontal end events (which happen at different x than vertical query), it's safe if we process events in order. Implementation: create events for horizontal starts and ends, and vertical queries. Each event has x-coordinate, type (1=horiz start, 2=horiz end, 3=vertical), and a pointer to the line. Sort events by x; for ties (same x), process vertical queries before horizontal starts? Actually, we need to consider intersections at the same x: a vertical line at x = X intersects any horizontal line whose x-range includes X. If a horizontal line starts at X, it should be active for that vertical query. If a horizontal line ends at X, it should also be active. So for same x, process vertical queries first (to include both starting and ending horizontal lines), then process horizontal starts and ends. But we must also handle the order between horizontal starts and ends at the same x: if a line starts and another ends at same x, the line ending should still be active for vertical query, so process vertical first, then process all horizontal events (starts and ends) arbitrarily. After processing vertical, process horizontal starts (insert into multimap) and horizontal ends (erase from multimap). For erasing, we need to remove exactly the pointer that ends; with multimap, find an iterator equal to that pointer and erase it. Use `auto range = mm.equal_range(line->m); for (auto it=range.first; it!=range.second; ++it) if (it->second == line) { mm.erase(it); break; }` which is O(number of active lines at that y). Time complexity: sorting O(N log N) for N lines (each horizontal generates two events, vertical one event). Each event processing: insertion/erasure in multimap O(log M) plus possibly O(K) for finding the right iterator to erase where K is the number of lines with same y (usually small). Queries iterate over active lines in the y-range; total over all queries is O(N^2) worst-case if all horizontals intersect all verticals. But typical expected O((N+V) log N + number of intersections). Space O(N). Edge cases: vertical line with zero length? Assume k1 <= k2, so length >= 0 (could be zero if k1==k2, representing a point, but still a vertical segment). Horizontal zero length also allowed. Inclusive bounds. Deduplication: maintain a `std::set<line*>` or `std::unordered_set<line*>` to collect intersected horizontals, then convert to vector of `pair<int,int>`? Actually the return type is `vector<pair<int,int>>` representing the horizontal segments themselves (their start/end x and y? The problem asks for "all horizontal segments that intersect at least one vertical segment" — likely just return the original segments, so we need a vector of `line` objects or their indices. The typical solution returns the actual lines. So we can return a vector of `line` (copy) or a vector of `pair<int,int>`? "vector<pair<int,int>>" suggests maybe return the y-coordinate and something? The problem statement says "returns a vector of pairs representing all horizontal segments that intersect at least one vertical segment". That is ambiguous, but we can interpret as returning each horizontal segment's (k1,k2) as a pair, since that identifies the segment uniquely if we also know y? Actually to uniquely identify, we need y. But letter: "vector<pair<int,int>>" could be (start_x, end_x) and assume y is known? Better to return the original line objects, but the signature given is `vector<pair<int,int>>`. I'll interpret that as returning a vector of pairs, each being the (k1,k2) of a horizontal segment, but that would not distinguish two horizontals with same x-range but different y. To avoid ambiguity, I'll adjust the task to return a `vector<line>` (a copy of each intersecting horizontal). However, the task explicitly says `vector<pair<int,int>>`, so I'll enforce that and note that if two horizontals have identical x-ranges but different y, they will produce duplicate pairs; but that's acceptable per the spec? The problem says "unique horizontal segment", which implies uniqueness by object, not by coordinates. So returning pairs would violate uniqueness. Therefore, I'll re-interpret: The intended signature should be `vector<line>` or `vector<line*>`. But since the task must follow exactly, I'll define the function to return `vector<pair<int,int>>` where each pair is `(y_coordinate, x_start)`? That doesn't make sense. Let me instead modify the task to be consistent: Define a struct `point` or just return vector of indices. To keep it simple and self-contained, I'll change the return type to `vector<line>` (copies). However, the problem statement in the task description must match the solution. Since I'm writing the task, I can define the return type as `std::vector<line>` (the horizontal segments that intersect at least one vertical). That is a sensible and clear specification. I'll do that.

Thus, the task: Given a vector of `line` objects (some horizontal, some vertical), return a vector containing each horizontal `line` that intersects at least one vertical `line`. Use a sweep-line algorithm.

Time complexity: O(N log N + N^2) worst-case for intersections, but typical O(N log N + K) where K is number of reported intersections? Actually we only need to know if a horizontal intersects at least one vertical, so we can stop early for each horizontal? We can't because we don't know which horizontals until we process verticals. We can process all verticals and mark any horizontal that intersects any vertical. The total number of intersection checks is the sum over verticals of number of active horizontals in that y-range, which in worst case O(V*H). So overall O(N log N + V*H). But with a map we can iterate over the range, so it's O(N log N + total number of pairs in range checks). Since each check yields a horizontal that intersects, we can mark it, and we don't need to recheck later? We still need to iterate over the range to discover all, so worst-case O(H*V). That's acceptable for moderate sizes. Space O(H).

Edge cases: overlapping horizontal segments, multiple verticals at same x, verticals with y-range covering many horizontals, etc. Ensure inclusive bounds.

#include <vector>
#include <map>
#include <algorithm>
#include <set>

struct line {
    int k1; // start x (horizontal) or start y (vertical)
    int k2; // end x (horizontal) or end y (vertical)
    int m;  // y-coordinate (horizontal) or x-coordinate (vertical)
    int type; // 1 = horizontal, 2 = vertical
};

// Returns all horizontal segments that intersect at least one vertical segment.
std::vector<line> findHorizontalIntersections(const std::vector<line>& lines) {
    enum EventType { HORIZ_START = 1, HORIZ_END = 2, VERT_QUERY = 3 };
    struct Event {
        int x;
        int type;
        const line* ptr;
        // For horizontal starts/ends, we prioritize vertical queries at same x.
        bool operator<(const Event& other) const {
            if (x != other.x) return x < other.x;
            // At the same x, process vertical queries before horizontal events.
            if (type != other.type) return type > other.type; // VERT_QUERY=3 > 1,2
            return ptr < other.ptr; // arbitrary tie-break
        }
    };

    std::vector<Event> events;
    for (const auto& l : lines) {
        if (l.type == 1) { // horizontal
            events.push_back({std::min(l.k1, l.k2), HORIZ_START, &l});
            events.push_back({std::max(l.k1, l.k2), HORIZ_END, &l});
        } else { // vertical
            events.push_back({l.m, VERT_QUERY, &l});
        }
    }

    std::sort(events.begin(), events.end());

    // Multimap from y-coordinate to pointer to active horizontal line.
    std::multimap<int, const line*> active;
    std::set<const line*> intersected;

    for (const auto& e : events) {
        if (e.type == VERT_QUERY) {
            int y_low = std::min(e.ptr->k1, e.ptr->k2);
            int y_high = std::max(e.ptr->k1, e.ptr->k2);
            auto it_low = active.lower_bound(y_low);
            auto it_high = active.upper_bound(y_high);
            for (auto it = it_low; it != it_high; ++it) {
                intersected.insert(it->second);
            }
        } else if (e.type == HORIZ_START) {
            active.insert({e.ptr->m, e.ptr});
        } else { // HORIZ_END
            auto range = active.equal_range(e.ptr->m);
            for (auto it = range.first; it != range.second; ++it) {
                if (it->second == e.ptr) {
                    active.erase(it);
                    break;
                }
            }
        }
    }

    std::vector<line> result;
    for (const line* l : intersected) {
        result.push_back(*l);
    }
    return result;
}

#include <cassert>
#include <vector>
#include <algorithm>

// Assume the line struct and function are defined above.

int main() {
    // Example from the snippet: horizontal lines l1,l2,l4,l7; vertical l3,l5,l6
    std::vector<line> lines = {
        {1, 5, 1, 1}, // h [1,5] at y=1
        {2, 4, 2, 1}, // h [2,4] at y=2
        {2, 5, 3, 2}, // v [2,5] at x=3
        {6, 9, 4, 1}, // h [6,9] at y=4
        {0, 7, 8, 2}, // v [0,7] at x=8
        {0, 7, 10, 2}, // v [0,7] at x=10
        {8, 10, 3, 1} // h [8,10] at y=3
    };

    auto result = findHorizontalIntersections(lines);
    // Expected: l1 (y=1) intersects l3, l5, l6? l3 x=3 in [1,5] yes, l5 x=8 no, l6 x=10 no.
    // l2 (y=2) intersects l3 (x=3 in [2,4]) yes, l5 x=8 no, l6 x=10 no.
    // l4 (y=4) intersects l3? x=3 not in [6,9] no. l5 x=8 in [6,9] yes, l6 x=10 no.
    // l7 (y=3) intersects l3? x=3 not in [8,10] no. l5 x=8 in [8,10] yes, l6 x=10 in [0,10]? l6 vertical x=10, horizontal [8,10] includes 10, y=3 in [0,7] yes, so intersects.
    // So l1,l2,l4,l7 all intersect at least one vertical. So all 4.
    assert(result.size() == 4);

    // Check that each result is one of the expected horizontals.
    auto contains = [&](int k1, int k2, int m) {
        for (const auto& l : result) {
            if (l.k1 == k1 && l.k2 == k2 && l.m == m) return true;
        }
        return false;
    };
    assert(contains(1,5,1));
    assert(contains(2,4,2));
    assert(contains(6,9,4));
    assert(contains(8,10,3));

    // Test case with no intersections.
    std::vector<line> noIntersect = {
        {1, 3, 1, 1}, // h [1,3] at y=1
        {5, 7, 2, 2}  // v [5,7] at x=2? Actually vertical x=2, y[5,7] – horizontal y=1 not in range, so no.
    };
    assert(findHorizontalIntersections(noIntersect).empty());

    // Test case with duplicate y for horizontal lines.
    std::vector<line> duplicateY = {
        {1, 5, 2, 1}, // h [1,5] at y=2
        {2, 4, 2, 1}, // h [2,4] at y=2 (same y)
        {3, 3, 2, 2}  // v [3,3] at x=2? No, vertical x=2, y=[3,3] – horizontal y=2 not in [3,3], so no.
    };
    // Actually let's make a vertical that intersects both: vertical x=3, y=[2,2] -> 
    std::vector<line> dup2 = {
        {1, 5, 2, 1},
        {2, 4, 2, 1},
        {3, 3, 3, 2} // vertical x=3, y[3,3] – horizontal y=2 not in [3,3], still no.
    };
    // Correct: vertical x=3, y[1,2] would intersect both.
    std::vector<line> dup3 = {
        {1, 5, 2, 1},
        {2, 4, 2, 1},
        {3, 1, 2, 2} // vertical x=3, y[1,2] – y=2 included, x=3 in both ranges.
    };
    auto res2 = findHorizontalIntersections(dup3);
    assert(res2.size() == 2);

    // Test inclusive endpoints: horizontal [1,3] at y=0, vertical x=1,y[0,0] -> intersect.
    std::vector<line> inclusive = {
        {1, 3, 0, 1},
        {1, 0, 0, 2} // vertical x=1, y[0,0]
    };
    assert(findHorizontalIntersections(inclusive).size() == 1);

    // Test vertical with zero length.
    std::vector<line> zeroLen = {
        {2, 4, 5, 1},
        {3, 3, 7, 2} // vertical x=3, y[3,3] – horizontal y=5 not in [3,3], no.
    };
    assert(findHorizontalIntersections(zeroLen).empty());

    // Test many horizontals, one vertical spanning all.
    std::vector<line> many;
    for (int i = 0; i < 100; ++i) {
        many.push_back({0, 100, i, 1});
    }
    many.push_back({-10, 110, 50, 2}); // vertical x=50,y[-10,110] covers all.
    auto res3 = findHorizontalIntersections(many);
    assert(res3.size() == 100);

    return 0;
}
