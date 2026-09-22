/*
Given a list of `n` segments on a line, each defined by integer endpoints `[l, r]` with `l <= r`, write a C++ function `findOverlappingSegmentPair` that returns a pair of 1-based indices `(i, j)` such that segment `i` is completely contained within segment `j` (i.e., `l_j <= l_i` and `r_i <= r_j`), or `(-1, -1)` if no such pair exists. The input is provided as a vector of pairs `(l, r)`. Your function must detect such a pair in a single pass over the segments plus a later sweep over sorted events, as in the reference code snippet, and it should handle duplicate endpoints and multiple segments correctly. If multiple valid pairs exist, return any one of them.
*/
#include <vector>
#include <utility>
#include <algorithm>
#include <queue>
#include <unordered_map>
#include <cstdlib>

// Returns a pair of 1-based indices (i, j) such that segment i is contained in segment j,
// or (-1, -1) if no such pair exists.
std::pair<int, int> findOverlappingSegmentPair(const std::vector<std::pair<int, int>>& segments) {
    const int n = static_cast<int>(segments.size());
    std::unordered_map<int, int> lastLeft;  // left endpoint -> segment index
    std::unordered_map<int, int> lastRight; // right endpoint -> segment index

    // Phase 1: check for containment via shared endpoints
    for (int i = 0; i < n; ++i) {
        int l = segments[i].first;
        int r = segments[i].second;

        auto itL = lastLeft.find(l);
        if (itL != lastLeft.end()) {
            int j = itL->second;
            if (segments[j].second >= segments[i].second) {
                return {i + 1, j + 1}; // i contained in j
            } else {
                return {j + 1, i + 1}; // j contained in i
            }
        }

        auto itR = lastRight.find(r);
        if (itR != lastRight.end()) {
            int j = itR->second;
            if (segments[j].first <= segments[i].first) {
                return {i + 1, j + 1}; // i contained in j
            } else {
                return {j + 1, i + 1}; // j contained in i
            }
        }

        lastLeft[l] = i;
        lastRight[r] = i;
    }

    // Phase 2: sweep over sorted events
    std::vector<std::pair<int, int>> events; // (coordinate, encoded id)
    events.reserve(2 * n);
    for (int i = 0; i < n; ++i) {
        events.emplace_back(segments[i].first, -(i + 1)); // start event
        events.emplace_back(segments[i].second, (i + 1)); // end event
    }
    std::sort(events.begin(), events.end());

    std::queue<int> active; // stores 1-based segment indices
    for (const auto& ev : events) {
        bool isStart = ev.second < 0;
        int who = std::abs(ev.second) - 1; // 0-based index
        if (isStart) {
            active.push(who + 1);
        } else {
            if (active.front() != who + 1) {
                return {who + 1, active.front()}; // ending segment contained in front segment
            }
            active.pop();
        }
    }

    return {-1, -1};
}
#include <cassert>
#include <vector>
#include <utility>

// free function declared above (include the solution code here)

int main() {
    // Test 1: no overlapping containment
    std::vector<std::pair<int,int>> s1 = {{1,2},{3,4}};
    assert(findOverlappingSegmentPair(s1) == std::make_pair(-1,-1));

    // Test 2: shared left endpoints, one contains other
    std::vector<std::pair<int,int>> s2 = {{1,5},{1,3}};
    assert(findOverlappingSegmentPair(s2) == std::make_pair(2,1)); // segment 2 inside segment 1

    // Test 3: shared right endpoints
    std::vector<std::pair<int,int>> s3 = {{2,10},{5,10}};
    assert(findOverlappingSegmentPair(s3) == std::make_pair(2,1));

    // Test 4: non-sharing endpoints, nested via sweep
    std::vector<std::pair<int,int>> s4 = {{1,10},{2,5},{6,9}};
    auto res4 = findOverlappingSegmentPair(s4);
    assert((res4 == std::make_pair(2,1)) || (res4 == std::make_pair(3,1)));

    // Test 5: identical segments
    std::vector<std::pair<int,int>> s5 = {{3,7},{3,7}};
    assert(findOverlappingSegmentPair(s5) == std::make_pair(2,1));

    // Test 6: no overlap at all, separate
    std::vector<std::pair<int,int>> s6 = {{1,2},{2,3},{3,4}};
    assert(findOverlappingSegmentPair(s6) == std::make_pair(-1,-1));

    // Test 7: more complex nesting
    std::vector<std::pair<int,int>> s7 = {{1,8},{2,7},{3,6}};
    auto res7 = findOverlappingSegmentPair(s7);
    assert((res7 == std::make_pair(3,2)) || (res7 == std::make_pair(2,1)) || (res7 == std::make_pair(3,1)));

    // Test 8: single segment
    std::vector<std::pair<int,int>> s8 = {{5,5}};
    assert(findOverlappingSegmentPair(s8) == std::make_pair(-1,-1));

    // Test 9: overlapping but not containing
    std::vector<std::pair<int,int>> s9 = {{1,4},{2,5}};
    assert(findOverlappingSegmentPair(s9) == std::make_pair(-1,-1));

    // Test 10: nested with same start and end on different segments
    std::vector<std::pair<int,int>> s10 = {{1,6},{1,6},{1,5}};
    assert(findOverlappingSegmentPair(s10) != std::make_pair(-1,-1));

    return 0;
}
// The solution processes segments in two phases. First, while reading each segment, we maintain maps from the left endpoint `l` to the most recently seen segment index with that `l`, and from the right endpoint `r` to the most recently seen segment index with that `r`. When we encounter a new segment `i`, we check if there is already a segment `j` with the same left endpoint; if the existing segment's right endpoint is >= new segment's right endpoint, then the new segment is contained in `j`. Otherwise, the existing segment is contained in the new one (since they share the same left endpoint). Similarly, for a matching right endpoint, if the existing segment's left endpoint is <= the new segment's left endpoint, then the new segment is contained within the existing one, else the existing is contained within the new one. This O(n) pass catches all pairs that share an endpoint.  
// If no such pair is found, we sort all events (each segment contributes a left "start" event and a right "end" event) and sweep from left to right, maintaining a queue of active segments as we encounter their start events. When we encounter an end event, the segment that is ending must be the one at the front of the queue (FIFO), because segments are processed in order of their starts and no overlapping nesting has been detected yet. If the front does not match the ending segment, then we have found a containment: the ending segment must have started before the current front's start but ends earlier, meaning it is contained by the front segment. This O(n log n) sort plus O(n) sweep handles all remaining cases.  
// Edge cases: segments with identical endpoints should be detected by the endpoint map checks. If no pair exists, return `(-1, -1)`. Time complexity is O(n log n) due to sorting, but the first phase is O(n). Space complexity is O(n) for storing segments, maps, and events.
