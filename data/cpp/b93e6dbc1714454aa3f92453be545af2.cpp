// Write a C++ function that takes a vector of non-overlapping intervals sorted by their start times, and a new interval, and returns a new vector of intervals after inserting the new interval while merging any overlapping intervals to keep the resulting intervals non-overlapping and sorted. Each interval is a struct with `start` and `end` integer fields (inclusive). The input intervals are guaranteed sorted and non-overlapping, but the new interval may overlap with zero or more of them, and may have `start > end` (invalid) in which case it should be inserted as-is without merging (the result remains sorted based on the original intervals, but the invalid interval is placed according to its `start` value; if it overlaps, since it's invalid, we still treat it as a normal interval for merging purposes? — clarify: the original code treats it normally, so we follow that). The function should not modify the input vector; it should return a new vector. The function signature is `vector<Interval> insertInterval(const vector<Interval>& intervals, Interval newInterval)`. The input may be empty (then the result contains just the new interval). Assume intervals are already sorted by start and non-overlapping; do not validate this assumption. The output must be sorted and non-overlapping.

#include <cassert>
#include <vector>

// Assume Interval and insertInterval are available from above.

int main() {
    // Case 1: Insert into non-overlapping middle
    std::vector<Interval> v1 = {Interval(1,2), Interval(5,6)};
    auto r1 = insertInterval(v1, Interval(3,4));
    assert(r1.size() == 3);
    assert(r1[0].start == 1 && r1[0].end == 2);
    assert(r1[1].start == 3 && r1[1].end == 4);
    assert(r1[2].start == 5 && r1[2].end == 6);

    // Case 2: Overlap with one interval
    std::vector<Interval> v2 = {Interval(1,5), Interval(7,9)};
    auto r2 = insertInterval(v2, Interval(3,8));
    assert(r2.size() == 1);
    assert(r2[0].start == 1 && r2[0].end == 9);

    // Case 3: Insert at beginning
    std::vector<Interval> v3 = {Interval(5,6), Interval(8,9)};
    auto r3 = insertInterval(v3, Interval(1,2));
    assert(r3.size() == 3);
    assert(r3[0].start == 1 && r3[0].end == 2);
    assert(r3[1].start == 5 && r3[1].end == 6);
    assert(r3[2].start == 8 && r3[2].end == 9);

    // Case 4: Insert at end
    std::vector<Interval> v4 = {Interval(1,2), Interval(3,4)};
    auto r4 = insertInterval(v4, Interval(6,7));
    assert(r4.size() == 3);
    assert(r4[2].start == 6 && r4[2].end == 7);

    // Case 5: Completely contained
    std::vector<Interval> v5 = {Interval(1,10)};
    auto r5 = insertInterval(v5, Interval(3,4));
    assert(r5.size() == 1);
    assert(r5[0].start == 1 && r5[0].end == 10);

    // Case 6: Empty input
    std::vector<Interval> v6;
    auto r6 = insertInterval(v6, Interval(2,5));
    assert(r6.size() == 1);
    assert(r6[0].start == 2 && r6[0].end == 5);

    // Case 7: Insert between two adjacent intervals
    std::vector<Interval> v7 = {Interval(1,2), Interval(3,4)};
    auto r7 = insertInterval(v7, Interval(2,3));
    assert(r7.size() == 1);
    assert(r7[0].start == 1 && r7[0].end == 4);

    // Case 8: Overlaps all intervals
    std::vector<Interval> v8 = {Interval(1,2), Interval(4,5), Interval(7,8)};
    auto r8 = insertInterval(v8, Interval(0,9));
    assert(r8.size() == 1);
    assert(r8[0].start == 0 && r8[0].end == 9);

    // Case 9: Invalid interval (start > end) but no overlap
    std::vector<Interval> v9 = {Interval(1,2), Interval(5,6)};
    auto r9 = insertInterval(v9, Interval(3,2));
    assert(r9.size() == 3);
    assert(r9[1].start == 3 && r9[1].end == 2);

    // Case 10: New interval exactly matches an existing one
    std::vector<Interval> v10 = {Interval(2,3)};
    auto r10 = insertInterval(v10, Interval(2,3));
    assert(r10.size() == 1);
    assert(r10[0].start == 2 && r10[0].end == 3);

    return 0;
}

#include <vector>
#include <algorithm>

struct Interval {
    int start;
    int end;
    Interval() : start(0), end(0) {}
    Interval(int s, int e) : start(s), end(e) {}
};

// Insert newInterval into a sorted, non-overlapping list of intervals,
// merging any overlapping intervals. Returns a new sorted vector.
std::vector<Interval> insertInterval(const std::vector<Interval>& intervals, Interval newInterval) {
    std::vector<Interval> result;
    bool inserted = false;
    for (const Interval& current : intervals) {
        // If newInterval ends before current starts and hasn't been inserted,
        // insert it here.
        if (newInterval.end < current.start && !inserted) {
            result.push_back(newInterval);
            result.push_back(current);
            inserted = true;
        }
        // If newInterval is already inserted or there's no overlap,
        // just keep the current interval.
        else if (inserted || current.end < newInterval.start) {
            result.push_back(current);
        }
        // Otherwise, they overlap; merge into newInterval.
        else {
            newInterval.start = std::min(newInterval.start, current.start);
            newInterval.end = std::max(newInterval.end, current.end);
        }
    }
    // If newInterval hasn't been inserted yet, it goes at the end.
    if (!inserted) {
        result.push_back(newInterval);
    }
    return result;
}

// The algorithm iterates through the sorted intervals once. For each existing interval, we compare it with the new interval. There are three cases:
// 1. If the new interval ends before the current interval starts and we haven't inserted the new interval yet, we insert the new interval, then the current interval, and mark that insertion is done.
// 2. If we have already inserted the new interval, or the current interval ends before the new interval starts (i.e., no overlap), we simply push the current interval.
// 3. Otherwise, there is an overlap; we merge by updating the new interval's `start` to the minimum of the two starts and `end` to the maximum of the two ends. The merged interval will be inserted later when we find the next non-overlapping interval or at the end.
//
// After the loop, if the new interval has not been inserted yet, we append it. This ensures correct insertion into the correct position. Edge cases include: empty input (just return a vector containing the new interval), new interval placed before all existing intervals, after all existing intervals, overlapping all intervals, or being completely contained in one interval. Time complexity is O(n) where n is the number of input intervals, because each existing interval is processed once. Space complexity is O(n) for the output vector, not counting the input vector.
