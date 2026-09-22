Write a C++ function that takes a vector of time intervals, where each interval is given as a pair of strings in `HH:MM:SS` format (24-hour clock) representing the start and end times of a period during which a resource is occupied. The function should return the maximum number of concurrently occupied periods at any single moment. Intervals are inclusive at the start and exclusive at the end (i.e., an interval [start, end) means the resource is occupied from `start` up to but not including `end`). There may be multiple identical start or end times, and intervals may overlap arbitrarily. If the input vector is empty, return 0.

#include <cassert>
#include <string>
#include <vector>
#include <utility>

// (Solution function declaration is assumed to be above.)

int main() {
    // Basic overlapping case.
    std::vector<std::pair<std::string, std::string>> intervals1 = {
        {"09:00:00", "10:00:00"},
        {"09:30:00", "10:30:00"},
        {"10:00:00", "11:00:00"}  // Starts when first ends, half-open so no overlap at exactly 10:00:00
    };
    assert(maxConcurrentOccupied(intervals1) == 2);

    // Empty input.
    assert(maxConcurrentOccupied({}) == 0);

    // Single interval.
    std::vector<std::pair<std::string, std::string>> intervals2 = {
        {"08:00:00", "09:00:00"}
    };
    assert(maxConcurrentOccupied(intervals2) == 1);

    // All overlapping.
    std::vector<std::pair<std::string, std::string>> intervals3 = {
        {"10:00:00", "12:00:00"},
        {"10:30:00", "11:30:00"},
        {"11:00:00", "12:30:00"}
    };
    assert(maxConcurrentOccupied(intervals3) == 3);

    // Exact touch at same second (end exclusive).
    std::vector<std::pair<std::string, std::string>> intervals4 = {
        {"09:00:00", "10:00:00"},
        {"10:00:00", "11:00:00"}
    };
    assert(maxConcurrentOccupied(intervals4) == 1);

    // Same start and end times (multiple identical intervals).
    std::vector<std::pair<std::string, std::string>> intervals5 = {
        {"12:00:00", "13:00:00"},
        {"12:00:00", "13:00:00"},
        {"12:00:00", "13:00:00"}
    };
    assert(maxConcurrentOccupied(intervals5) == 3);

    // Intervals that end at same time as others start (multiple boundaries).
    std::vector<std::pair<std::string, std::string>> intervals6 = {
        {"00:00:00", "12:00:00"},
        {"12:00:00", "18:00:00"},
        {"12:00:00", "15:00:00"},
        {"15:00:00", "20:00:00"}
    };
    assert(maxConcurrentOccupied(intervals6) == 2);

    // Large times (near midnight rollover) — no rollover since 24-hour clock only up to 23:59:59.
    std::vector<std::pair<std::string, std::string>> intervals7 = {
        {"23:00:00", "23:59:59"},
        {"23:30:00", "23:59:58"},
        {"23:59:58", "23:59:59"}
    };
    assert(maxConcurrentOccupied(intervals7) == 2);

    return 0;
}

#include <string>
#include <vector>
#include <algorithm>
#include <utility>

// Given a vector of intervals (start, end) in HH:MM:SS format,
// return the maximum number of concurrent occupied intervals at any moment.
// Intervals are half-open: [start, end) — a resource is occupied from start
// up to but not including end. Empty input returns 0.
int maxConcurrentOccupied(const std::vector<std::pair<std::string, std::string>>& intervals) {
    // Helper to convert "HH:MM:SS" to seconds since midnight.
    auto toSeconds = [](const std::string& timeStr) -> int {
        int h = std::stoi(timeStr.substr(0, 2));
        int m = std::stoi(timeStr.substr(3, 2));
        int s = std::stoi(timeStr.substr(6, 2));
        return h * 3600 + m * 60 + s;
    };

    // Events: (time, type) where type=+1 for start, type=-1 for end.
    // We use a small tie-breaker so that at the same time, removals come first.
    std::vector<std::pair<int, int>> events;
    events.reserve(intervals.size() * 2);

    for (const auto& interval : intervals) {
        int startSec = toSeconds(interval.first);
        int endSec = toSeconds(interval.second);
        // Sort key: (time, type) — we want -1 (end) to appear before +1 (start) at the same time.
        events.emplace_back(startSec, 1);
        events.emplace_back(endSec, -1);
    }

    // Sort by time, then by type (so -1 comes before +1).
    std::sort(events.begin(), events.end(),
        [](const std::pair<int, int>& a, const std::pair<int, int>& b) {
            if (a.first != b.first) return a.first < b.first;
            return a.second < b.second;  // -1 < 1, so ends before starts
        });

    int current = 0;
    int maxCount = 0;
    for (const auto& event : events) {
        current += event.second;
        if (current > maxCount) {
            maxCount = current;
        }
    }

    return maxCount;
}

// The classic sweep-line algorithm solves this in O(n log n) time. Convert each start time to seconds since midnight (h*3600 + m*60 + s) and treat it as an "add" event (+1); convert each end time and treat it as a "remove" event (-1). Sort all events first by time, then by type such that if a start and end occur at the same second, the end (removal) is processed before the start (addition) — this correctly handles the half-open interval semantics (end is exclusive). Then scan the sorted events, maintaining a running count: increment on add, decrement on remove, and update the answer with the maximum count seen. Edge cases: empty input returns 0; intervals that exactly touch (one ends at t, another starts at t) do not overlap; multiple intervals with the same boundaries are handled naturally. Time complexity is O(n log n) for sorting, and O(n) for scanning, with O(n) auxiliary space for the event list.
