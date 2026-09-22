// Given a list of intervals defined by their integer start and end years (inclusive), write a standalone C++ function `mostPopularYear` that takes a vector of pairs of integers representing `[start, end]` intervals and returns a pair of integers: the year with the maximum number of active intervals, and that maximum count. If multiple years tie for the maximum, return the earliest year among them. Each interval contributes +1 to every year from its start through its end (inclusive). The input intervals may be unsorted, may overlap arbitrarily, and may contain negative or zero years. The function should handle an empty input by returning `{0, 0}`. Assume the input is valid: `start <= end` for every interval. Optimize for large inputs where the coordinate range (max year minus min year) could be huge, so you must use a sweep‑line (difference) approach rather than iterating over every possible year.

#include <cassert>
#include <vector>
#include <utility>

// The solution function is assumed to be defined above.
std::pair<int, int> mostPopularYear(const std::vector<std::pair<int, int>>& intervals);

int main() {
    // Basic overlapping intervals
    assert(mostPopularYear({{1, 5}, {3, 7}}) == std::make_pair(3, 2));
    // All same interval
    assert(mostPopularYear({{2, 4}, {2, 4}, {2, 4}}) == std::make_pair(2, 3));
    // Non-overlapping: each year has at most 1
    assert(mostPopularYear({{1, 2}, {3, 4}}) == std::make_pair(1, 1));
    // Zero-length intervals
    assert(mostPopularYear({{5, 5}, {5, 5}, {2, 3}}) == std::make_pair(5, 2));
    // Negative and large values
    assert(mostPopularYear({{-10, -5}, {-8, -6}, {-7, -7}}) == std::make_pair(-7, 3));
    // Tie: earliest year with max count
    assert(mostPopularYear({{1, 3}, {2, 4}, {2, 5}}) == std::make_pair(2, 3));
    // Edge: empty input
    assert(mostPopularYear({}) == std::make_pair(0, 0));
    // Single interval
    assert(mostPopularYear({{42, 42}}) == std::make_pair(42, 1));
    // Contiguous intervals covering a range
    assert(mostPopularYear({{1, 10}, {2, 11}, {3, 12}}) == std::make_pair(3, 3));
    // Intervals that start later but end earlier
    assert(mostPopularYear({{1, 100}, {50, 60}, {51, 59}}) == std::make_pair(51, 3));
    return 0;
}

#include <vector>
#include <algorithm>
#include <utility>

// Given vector of intervals [start, end] (inclusive), return {year, max_count}.
// If multiple years tie, return the earliest year. Empty input -> {0,0}.
std::pair<int, int> mostPopularYear(const std::vector<std::pair<int, int>>& intervals) {
    if (intervals.empty()) {
        return {0, 0};
    }

    // Each interval [l, r] creates a +1 at l and a -1 at r+1.
    std::vector<std::pair<long long, int>> events; // (coordinate, delta)
    events.reserve(2 * intervals.size());
    for (const auto& [l, r] : intervals) {
        events.emplace_back(static_cast<long long>(l), +1);
        events.emplace_back(static_cast<long long>(r) + 1LL, -1);
    }

    // Sort by coordinate; for ties, -1 comes before +1 because -1 < +1.
    std::sort(events.begin(), events.end());

    int current = 0;
    int best = 0;
    long long bestYear = 0;
    bool first = true;

    for (const auto& [coord, delta] : events) {
        current += delta;
        // Only consider coordinates that are actual years (not r+1 sentinels).
        // But delta -1 at r+1 sets current to the count after that year.
        // We want to check after applying all events at coord. Since we process
        // sequentially, we update when current exceeds best.
        if (current > best) {
            best = current;
            bestYear = coord;
        }
    }

    return {static_cast<int>(bestYear), best};
}

// The problem asks for the year covered by the most intervals, which is a classic interval‑stabbing maximum problem. A naive approach that iterates over every year from the minimum start to the maximum end would be \(O(\text{range} \cdot n)\), which is prohibitive when the coordinate range is enormous (e.g., years from -10^9 to 10^9). Instead, we use a sweep‑line algorithm:  
// - For each interval `[l, r]`, create two events: one at `l` with a delta of `+1` (start) and one at `r+1` with a delta of `-1` (end). Using `r+1` ensures that the interval is active through `r` and stops at `r+1`.  
// - Sort all events by coordinate. When coordinates tie, it is safe to process all `-1` events before `+1` events at the same coordinate? Actually, careful: if we process `+1` before `-1` at the same coordinate, we might incorrectly count an interval that ends at `x` as also active at `x`. But since we shift the end event to `r+1`, the coordinate `r+1` is the first year where the interval is inactive. At a coordinate `x` where both a start and an end event occur, the correct order is to apply all deltas at that coordinate, but the count after processing that coordinate is what we care about, and we take the maximum after applying all events at that coordinate. For simplicity, we can sort by coordinate, and when coordinates are equal, process `+1` first (or `-1` first) — as long as we update the answer after applying all events at that coordinate. In the reference implementation, we sort pairs `(coordinate, delta)` where delta is `+1` for start and `-1` for end (using `r+1`). Sorting by `(coordinate, delta)` naturally puts `-1` before `+1` for equal coordinates because `-1 < +1`. That means at a coordinate `x` where a start event and an end event (from a previous interval whose `r+1 == x`) occur, we first decrement (removing intervals that ended just before `x`), then increment. That correctly computes the active count at `x` after all changes. Then we update the answer if the current count exceeds the best seen. Since we shift ends to `r+1`, any coordinate that is an actual year `y` will have all intervals that start at `y` added, and all intervals that ended at `y-1` already removed. So the count at coordinate `y` equals the number of intervals covering `y`.  
// - We traverse the sorted events, maintaining a running count. For each distinct coordinate (or each event in order), we apply the delta, then check if the current count is greater than the best; if so, update the best count and record the coordinate as the year. Because we process coordinates in ascending order, the first time we achieve a new maximum, we record the year — that guarantees the earliest year for ties. If the count equals the current best, we do not update the year (so we keep the earliest).  
// - Edge cases: Empty input → return `{0, 0}`. Single interval → the year is its start (since that’s the earliest year with count 1). Intervals that are zero‑length (`l == r`) produce a start at `l` and an end at `l+1`, correctly counted as active only at `l`.  
// - Time complexity: Sorting 2n events costs \(O(n \log n)\). The single pass is \(O(n)\). Space: \(O(n)\) for the events vector.  
// - Important: Use `long long` for coordinates if the input can exceed 32‑bit range? The problem statement uses `int`, but to be safe we can use `long long` for coordinates in the implementation, but return `pair<int,int>` as specified. We can cast the result back to `int` after computing, as the answer year is one of the input coordinates.
