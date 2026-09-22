/*
Write a standalone C++ function `countStableSegments` that takes a vector of integer values representing a time series and returns the number of maximal contiguous segments where the absolute difference between consecutive values is at most a given threshold `delta`. A segment must have at least two elements. For example, with `delta = 2`, the input `{1, 3, 5, 10, 12, 15}` has two stable segments: `[1,3,5]` (differences 2,2) and `[10,12,15]` (differences 2,3), so the answer is 2. The function should handle empty input, single-element input, and cases where no segment meets the criteria.
*/

#include <vector>
#include <cstdlib> // for abs

/**
 * Count the number of maximal contiguous segments of length at least 2
 * where the absolute difference between consecutive elements is <= delta.
 *
 * @param values Input time series.
 * @param delta  Maximum allowed absolute difference between consecutive elements.
 * @return Number of stable contiguous segments.
 */
int countStableSegments(const std::vector<int>& values, int delta) {
    if (values.size() < 2) {
        return 0; // no segment can have length >= 2
    }

    int segmentCount = 0;
    int segmentStart = 0; // index of first element of current segment

    for (int i = 1; i < static_cast<int>(values.size()); ++i) {
        if (std::abs(values[i] - values[i - 1]) > delta) {
            // A segment ends at (i-1). Check its length.
            if (i - segmentStart >= 2) {
                ++segmentCount;
            }
            segmentStart = i; // start a new segment at i
        }
    }

    // Handle the last segment that extends to the end of the array.
    int lastSegmentLength = static_cast<int>(values.size()) - segmentStart;
    if (lastSegmentLength >= 2) {
        ++segmentCount;
    }

    return segmentCount;
}

#include <cassert>
#include <vector>

int main() {
    // Basic examples
    assert(countStableSegments({1, 3, 5, 10, 12, 15}, 2) == 2);
    assert(countStableSegments({1, 2, 3, 4}, 1) == 1);           // whole vector is stable
    assert(countStableSegments({1, 1, 1}, 0) == 1);              // zero delta, equal values
    assert(countStableSegments({1, 5, 6, 10}, 2) == 1);          // only [5,6] is stable

    // Edge cases
    assert(countStableSegments({}, 5) == 0);                     // empty
    assert(countStableSegments({7}, 3) == 0);                    // single element
    assert(countStableSegments({10, 20}, 5) == 0);               // no stable pair
    assert(countStableSegments({10, 15}, 5) == 1);               // exactly at threshold
    assert(countStableSegments({1, 4, 2, 5, 8, 9}, 3) == 2);     // [1,4] and [5,8,9]

    // Negative numbers and zero delta
    assert(countStableSegments({-5, -4, -2, 0, 1}, 2) == 1);     // entire sequence stable
    assert(countStableSegments({1, 1, 1}, 0) == 1);
    assert(countStableSegments({1, 2, 2, 3}, 2) == 1);           // [1,2,2,3] all stable

    return 0;
}

// The solution iterates through the array once, maintaining a "current segment" that starts at the first element when the array has at least two elements. For each pair of consecutive elements, compute the absolute difference. If the difference is within `delta`, the current segment continues; if not, a segment ends. Whenever a segment either ends (due to a breach) or reaches the end of the array, we check if its length is at least 2 and increment the segment counter. Edge cases include: empty input (return 0), single element (return 0 since no segment can have length ≥2), and the case where the entire array is one stable segment, which is counted once after the loop finishes. Time complexity is O(n) since we traverse the array once; space complexity is O(1) besides the input storage.
