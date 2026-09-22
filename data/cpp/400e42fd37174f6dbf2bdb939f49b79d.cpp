// Given a 2D vector of integer intervals `nums`, where each interval is represented as `[start, end]` inclusive, write a C++ function `countCoveredPoints` that returns the total number of distinct integer points that belong to at least one interval. For example, if intervals are `[1,4]` and `[3,6]`, the covered points are `{1,2,3,4,5,6}` so the result is 6. The input is non-empty, intervals may overlap arbitrarily, and `start <= end` always holds. The function should not modify the input and must handle up to 10^5 intervals with coordinate values fitting in a 32-bit signed integer.
#include <cassert>
#include <vector>

// The solution function is declared above; include its definition before main in a full program.

int main() {
    // Single interval
    assert(countCoveredPoints({{1, 5}}) == 5);
    
    // Overlapping intervals
    assert(countCoveredPoints({{1, 4}, {3, 6}}) == 6);
    
    // Adjacent intervals (no gap)
    assert(countCoveredPoints({{1, 2}, {3, 4}}) == 4);
    
    // Nested intervals
    assert(countCoveredPoints({{1, 10}, {3, 5}}) == 10);
    
    // Disjoint intervals with gap
    assert(countCoveredPoints({{1, 2}, {5, 7}}) == 5);
    
    // Multiple overlaps and gaps mixed
    assert(countCoveredPoints({{2, 5}, {4, 8}, {10, 12}}) == 9);
    
    // All points same (single point interval)
    assert(countCoveredPoints({{3, 3}, {3, 3}}) == 1);
    
    // Large coordinates and single point
    assert(countCoveredPoints({{100000000, 100000001}}) == 2);
    
    // Negative and positive coordinates
    assert(countCoveredPoints({{-3, -1}, {0, 2}}) == 6);
    
    // Empty input is not expected per task, but we can still return 0 safely
    // (uncomment the next line if you test empty input)
    // assert(countCoveredPoints({}) == 0);
    
    return 0;
}
#include <map>
#include <vector>
#include <cstddef>

// Count the number of distinct integer points covered by at least one inclusive interval.
// Each interval is {start, end} with start <= end. The input vector is not modified.
int countCoveredPoints(const std::vector<std::vector<int>>& nums) {
    // Use long long for keys to avoid overflow when computing end+1 for large intervals.
    std::map<long long, int> delta;
    for (const auto& interval : nums) {
        long long start = interval[0];
        long long end   = interval[1];
        ++delta[start];
        --delta[end + 1];
    }

    long long total = 0;      // total covered points
    long long active = 0;     // number of active intervals at current sweep position
    long long previous = 0;   // last processed coordinate

    bool first = true;
    for (const auto& [coord, change] : delta) {
        if (!first && active > 0) {
            total += coord - previous;
        }
        active += change;
        previous = coord;
        first = false;
    }

    return static_cast<int>(total);
}
// The problem reduces to merging a set of inclusive ranges and summing their lengths. A direct approach of marking points in a boolean array is infeasible if coordinates are large or sparse. Instead, use a difference array technique with a coordinate-compressed map. Iterate through each interval, increment a counter at its start coordinate and decrement the counter just after its end (at `end+1`) in a `std::map<int,int>`. Then sweep through the map in increasing coordinate order. Maintain a running sum `current` of active intervals. When `current > 0` before processing a new coordinate, the gap between the previous coordinate and the current coordinate is fully covered; add that distance to the answer. Then apply the delta at the current coordinate (add or subtract) and update `last` coordinate. Edge case: if an interval ends at `INT_MAX`, `end+1` overflows, so use `long long` for map keys or clamp appropriately; here we can safely use `long long` as map key. Time complexity is O(n log n) due to map operations, and space is O(n) for the map. The algorithm is robust for overlapping, adjacent, and nested intervals.
