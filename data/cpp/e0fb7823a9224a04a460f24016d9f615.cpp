Create a C++ program that simulates the range-merging logic used in the HotSpot JVM's switch bytecode compiler. Implement a function `mergeSwitchRanges` that takes a vector of ranges, where each range is defined by an inclusive lower bound (`lo`), inclusive upper bound (`hi`), a destination identifier (`dest`), and a hit count (`cnt`). The function must merge adjacent ranges that have the same destination and non-zero counts, following the specific rules: two adjacent ranges can only be merged if they are contiguous (the first's `hi + 1 == second's `lo`), have identical destinations, and both have non-zero counts. After merging, any range with a zero count should have its destination changed to a special sentinel value `-1` (simulating `never_reached`). The function should return the merged ranges in order and also output the number of resulting ranges. The input ranges are guaranteed to be sorted by `lo` and non-overlapping, but may not include the full integer domain (i.e., the first range's `lo` may not be `INT_MIN` and the last range's `hi` may not be `INT_MAX`). The merging process must be stable and preserve the order, and the function should handle edge cases including empty input, a single range, and multiple identical adjacent ranges.

// The solution mirrors the logic from the provided HotSpot JVM code, specifically the `merge_ranges` and `SwitchRange::adjoinRange` methods. The core algorithm iterates through the sorted ranges once, maintaining a "write" index for the result array. For each new range encountered, we check if it can be merged with the last range currently in the result. A merge is possible if: (1) the current range's `lo` equals the previous range's `hi + 1` (contiguity), (2) both ranges have the same destination, and (3) both have non-zero counts. When a merge is possible, we extend the previous range's `hi` to the current range's `hi`, and sum the counts. If a merge is not possible, we move to the next slot in the result array. After the merging loop, we iterate through the result array again and set any range with a `cnt == 0` to the sentinel destination `-1`. The time complexity is O(n), where n is the number of input ranges, since we make only a single pass for merging and another for post-processing. Space complexity is O(n) for the output vector in the worst case (if no merges occur) and O(1) auxiliary space beyond that. Edge cases include: empty input (return empty vector), a single range (unchanged), ranges that are adjacent but have different destinations (not merged), ranges with zero counts adjacent to other zero-count ranges (merged only if same destination, then post-processed to sentinel), and ranges with non-zero counts adjacent to zero-count ranges with same destination (not merged because one count is zero).

#include <vector>
#include <cstdint>
#include <limits>

struct SwitchRange {
    int32_t lo;
    int32_t hi;
    int dest;
    double cnt;
};

// Merge adjacent ranges with same destination and non-zero counts.
// Ranges with zero counts are marked with destination = -1 (never_reached).
std::vector<SwitchRange> mergeSwitchRanges(const std::vector<SwitchRange>& input) {
    if (input.empty()) {
        return {};
    }

    std::vector<SwitchRange> result;
    result.reserve(input.size());

    for (const auto& current : input) {
        if (result.empty()) {
            result.push_back(current);
            continue;
        }

        SwitchRange& last = result.back();
        bool contiguous = (current.lo == last.hi + 1);
        bool same_dest = (current.dest == last.dest);
        bool both_non_zero = (current.cnt != 0 && last.cnt != 0);

        if (contiguous && same_dest && both_non_zero) {
            // Merge: extend the previous range's hi, sum the counts.
            last.hi = current.hi;
            last.cnt += current.cnt;
        } else {
            // Cannot merge, add as a new range.
            result.push_back(current);
        }
    }

    // Post-process: mark zero-count ranges with sentinel destination.
    for (auto& range : result) {
        if (range.cnt == 0 && range.dest != -1) {
            range.dest = -1;
        }
    }

    return result;
}

#include <cassert>
#include <cmath>

int main() {
    // Case 1: Basic adjacent mergers with same destination and non-zero counts.
    {
        std::vector<SwitchRange> input = {
            {1, 10, 100, 5.0},
            {11, 20, 100, 3.0},
            {21, 25, 200, 2.0}
        };
        auto result = mergeSwitchRanges(input);
        assert(result.size() == 2);
        assert(result[0].lo == 1 && result[0].hi == 20 && result[0].dest == 100);
        assert(std::abs(result[0].cnt - 8.0) < 1e-9);
        assert(result[1].lo == 21 && result[1].hi == 25 && result[1].dest == 200);
    }

    // Case 2: Adjacent ranges with different destinations should not merge.
    {
        std::vector<SwitchRange> input = {
            {0, 5, 1, 4.0},
            {6, 10, 2, 4.0}
        };
        auto result = mergeSwitchRanges(input);
        assert(result.size() == 2);
        assert(result[0].dest == 1 && result[1].dest == 2);
    }

    // Case 3: Adjacent zero-count ranges with same destination merge, then become sentinel.
    {
        std::vector<SwitchRange> input = {
            {10, 15, 7, 0.0},
            {16, 20, 7, 0.0},
            {21, 30, 8, 1.0}
        };
        auto result = mergeSwitchRanges(input);
        assert(result.size() == 2);
        assert(result[0].lo == 10 && result[0].hi == 20);
        assert(result[0].dest == -1); // sentinel
        assert(result[1].lo == 21 && result[1].hi == 30 && result[1].dest == 8);
    }

    // Case 4: Zero-count range adjacent to non-zero range with same destination should not merge.
    {
        std::vector<SwitchRange> input = {
            {1, 4, 9, 0.0},
            {5, 8, 9, 2.0}
        };
        auto result = mergeSwitchRanges(input);
        assert(result.size() == 2);
        assert(result[0].dest == -1);
        assert(result[1].dest == 9);
    }

    // Case 5: Non-contiguous ranges should never merge.
    {
        std::vector<SwitchRange> input = {
            {1, 5, 3, 1.0},
            {7, 10, 3, 1.0}
        };
        auto result = mergeSwitchRanges(input);
        assert(result.size() == 2);
    }

    // Case 6: Empty input.
    {
        std::vector<SwitchRange> input = {};
        auto result = mergeSwitchRanges(input);
        assert(result.empty());
    }

    // Case 7: Single range.
    {
        std::vector<SwitchRange> input = {
            {INT32_MIN, INT32_MAX, 4, 10.0}
        };
        auto result = mergeSwitchRanges(input);
        assert(result.size() == 1);
        assert(result[0].lo == INT32_MIN && result[0].hi == INT32_MAX && result[0].dest == 4);
    }

    // Case 8: All ranges with zero counts, multiple adjacent same dest should collapse into one sentinel.
    {
        std::vector<SwitchRange> input = {
            {0, 1, 5, 0.0},
            {2, 3, 5, 0.0},
            {4, 5, 5, 0.0}
        };
        auto result = mergeSwitchRanges(input);
        assert(result.size() == 1);
        assert(result[0].lo == 0 && result[0].hi == 5);
        assert(result[0].dest == -1);
    }

    return 0;
}
