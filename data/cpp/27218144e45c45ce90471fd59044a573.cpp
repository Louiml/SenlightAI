// Write a C++ function `insertInterval` that takes a sorted vector of non-overlapping `Interval` objects and a single `Interval` to insert, then returns a new vector of intervals with the new interval merged into the existing ones while preserving sorted order and non-overlapping property. Each `Interval` has integer `start` and `end` fields, with `start <= end`. The input intervals are guaranteed initially sorted by start time and non-overlapping. The function must handle all edge cases: inserting before the first interval, after the last, in between non-overlapping intervals, overlapping one or multiple intervals exactly or partially, and inserting into an empty vector. Implement the function as a free function named `insertInterval` that is `const`-correct (does not modify the input vector) and returns the result by value. Do not include a main function; that will be provided in the test section.
The core idea is a single linear pass through the input intervals, merging the new interval along the way. Since the input is sorted, we can process intervals sequentially. Maintain a mutable copy of the new interval (call it `newInt`). For each interval `curr` in the input, compare it with `newInt`:

- If `newInt` is still active and the two intervals overlap (i.e., `newInt.start <= curr.end && curr.start <= newInt.end`), merge them by setting `newInt.start = min(newInt.start, curr.start)` and `newInt.end = max(newInt.end, curr.end)`. Do not push `curr` yet; continue to the next interval because the merged interval may still overlap later ones.
- If `newInt` is active but does not overlap `curr`, then decide order: if `newInt.end < curr.start`, then `newInt` comes entirely before `curr`, so push `newInt` into the result, mark it as inactive (e.g., via a boolean flag), then push `curr`. If `curr.end < newInt.start`, then `curr` comes before `newInt`, so push `curr` and continue.
- If `newInt` is already placed (inactive), just push `curr`.

After the loop, if `newInt` is still active (meaning it extends beyond all intervals or was never placed), push it at the end. This handles the case where the new interval is larger than everything else. The algorithm runs in O(n) time because we visit each input interval once, and O(1) auxiliary space (excluding the output vector). Edge cases: empty input (just return a vector with the new interval), new interval before all (will be placed first because its end < first start), new interval after all (will remain active until end and be pushed last), new interval exactly touching an existing interval (handled by overlap condition using <=), and multiple overlapping intervals (all merged in one pass).
#include <vector>
#include <algorithm>

struct Interval {
    int start;
    int end;
    Interval() : start(0), end(0) {}
    Interval(int s, int e) : start(s), end(e) {}
};

// Insert a new interval into a sorted vector of non-overlapping intervals,
// merging as needed, and return the new vector.
std::vector<Interval> insertInterval(const std::vector<Interval>& intervals, Interval newInterval) {
    std::vector<Interval> result;
    bool inserted = false;

    for (const auto& curr : intervals) {
        if (!inserted) {
            // Check if the current interval overlaps with newInterval
            if (curr.start <= newInterval.end && newInterval.start <= curr.end) {
                // Merge: extend newInterval boundaries
                newInterval.start = std::min(newInterval.start, curr.start);
                newInterval.end = std::max(newInterval.end, curr.end);
            } else if (newInterval.end < curr.start) {
                // newInterval comes entirely before curr, place it now
                result.push_back(newInterval);
                inserted = true;
                result.push_back(curr);
            } else {
                // curr comes entirely before newInterval, push curr
                result.push_back(curr);
            }
        } else {
            result.push_back(curr);
        }
    }

    // If newInterval was never placed, it goes at the end
    if (!inserted) {
        result.push_back(newInterval);
    }

    return result;
}
#include <cassert>
#include <vector>

// Assume Interval and insertInterval are defined as in the solution.

int main() {
    // Example 1
    std::vector<Interval> input1 = {Interval(1,3), Interval(6,9)};
    std::vector<Interval> out1 = insertInterval(input1, Interval(2,5));
    std::vector<Interval> expected1 = {Interval(1,5), Interval(6,9)};
    assert(out1.size() == expected1.size());
    for (size_t i = 0; i < out1.size(); ++i) {
        assert(out1[i].start == expected1[i].start && out1[i].end == expected1[i].end);
    }

    // Example 2
    std::vector<Interval> input2 = {Interval(1,2), Interval(3,5), Interval(6,7), Interval(8,10), Interval(12,16)};
    std::vector<Interval> out2 = insertInterval(input2, Interval(4,9));
    std::vector<Interval> expected2 = {Interval(1,2), Interval(3,10), Interval(12,16)};
    assert(out2.size() == expected2.size());
    for (size_t i = 0; i < out2.size(); ++i) {
        assert(out2[i].start == expected2[i].start && out2[i].end == expected2[i].end);
    }

    // Insert before all
    std::vector<Interval> input3 = {Interval(5,7), Interval(10,12)};
    std::vector<Interval> out3 = insertInterval(input3, Interval(1,3));
    std::vector<Interval> expected3 = {Interval(1,3), Interval(5,7), Interval(10,12)};
    assert(out3.size() == expected3.size());
    for (size_t i = 0; i < out3.size(); ++i) {
        assert(out3[i].start == expected3[i].start && out3[i].end == expected3[i].end);
    }

    // Insert after all
    std::vector<Interval> input4 = {Interval(1,2), Interval(3,4)};
    std::vector<Interval> out4 = insertInterval(input4, Interval(5,6));
    std::vector<Interval> expected4 = {Interval(1,2), Interval(3,4), Interval(5,6)};
    assert(out4.size() == expected4.size());
    for (size_t i = 0; i < out4.size(); ++i) {
        assert(out4[i].start == expected4[i].start && out4[i].end == expected4[i].end);
    }

    // Insert between with no overlap
    std::vector<Interval> input5 = {Interval(1,2), Interval(5,6)};
    std::vector<Interval> out5 = insertInterval(input5, Interval(3,4));
    std::vector<Interval> expected5 = {Interval(1,2), Interval(3,4), Interval(5,6)};
    assert(out5.size() == expected5.size());
    for (size_t i = 0; i < out5.size(); ++i) {
        assert(out5[i].start == expected5[i].start && out5[i].end == expected5[i].end);
    }

    // Insert into empty vector
    std::vector<Interval> input6;
    std::vector<Interval> out6 = insertInterval(input6, Interval(2,3));
    assert(out6.size() == 1);
    assert(out6[0].start == 2 && out6[0].end == 3);

    // Exact overlap (merge into one)
    std::vector<Interval> input7 = {Interval(2,5), Interval(8,9)};
    std::vector<Interval> out7 = insertInterval(input7, Interval(4,6));
    std::vector<Interval> expected7 = {Interval(2,6), Interval(8,9)};
    assert(out7.size() == expected7.size());
    for (size_t i = 0; i < out7.size(); ++i) {
        assert(out7[i].start == expected7[i].start && out7[i].end == expected7[i].end);
    }

    // Touching intervals (no overlap by < but we use <= so they merge)
    std::vector<Interval> input8 = {Interval(1,2), Interval(3,4)};
    std::vector<Interval> out8 = insertInterval(input8, Interval(2,3));
    std::vector<Interval> expected8 = {Interval(1,4)};
    assert(out8.size() == expected8.size());
    assert(out8[0].start == 1 && out8[0].end == 4);

    // Large interval covering all
    std::vector<Interval> input9 = {Interval(1,2), Interval(4,5), Interval(7,8)};
    std::vector<Interval> out9 = insertInterval(input9, Interval(0,10));
    std::vector<Interval> expected9 = {Interval(0,10)};
    assert(out9.size() == expected9.size());
    assert(out9[0].start == 0 && out9[0].end == 10);

    // Single interval in input, new interval before it
    std::vector<Interval> input10 = {Interval(5,6)};
    std::vector<Interval> out10 = insertInterval(input10, Interval(1,2));
    std::vector<Interval> expected10 = {Interval(1,2), Interval(5,6)};
    assert(out10.size() == expected10.size());
    for (size_t i = 0; i < out10.size(); ++i) {
        assert(out10[i].start == expected10[i].start && out10[i].end == expected10[i].end);
    }

    return 0;
}
