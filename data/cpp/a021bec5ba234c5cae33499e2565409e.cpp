Given a line of length `L` (in meters) and a set of `N` sensor positions, each sensor detects activity within a radius `M` meters. This means a sensor at position `x` covers the interval `[max(0, x-M), min(x+M, L)]` (inclusive). Multiple sensors can cover the same point. Write a C++ function `int minimumCoveredPoints(int n, long long m, long long l, const std::vector<long long>& positions)` that returns the minimum number of sensors that cover any single point on the line, considering all real-valued points from `0` to `L` inclusive. A point is considered covered by a sensor if it lies within the sensor's inclusive interval. Note that the endpoints `0` and `L` can be covered, and intervals may partially lie outside `[0, L]` (they are clipped).
// The problem asks for the minimum coverage over continuous positions `[0, L]`. Since coverage changes only at interval boundaries, it suffices to check all endpoints `0`, `L`, and every interval start and end (after clipping). For each interval `[a, b]` (with `0 ≤ a ≤ b ≤ L`), the coverage is constant on any open segment `(a, b)`, but the value at the segment interior can be less than at boundaries? Actually, the coverage at a point just right of `a` is the same as coverage at `a` itself if `a` is the start of an interval, because the interval includes `a`. However, coverage can increase or decrease only at boundaries. So checking all boundary points and both ends is sufficient to find the minimum over the whole line.  
// Algorithm:  
// 1. For each sensor, compute `left = max(0, pos - m)` and `right = min(pos + m, l)`.  
// 2. Collect all distinct coordinates: `0`, `l`, and all `left` and `right` values.  
// 3. For each distinct coordinate `x`, count how many intervals contain `x`. Since intervals are inclusive, a point is covered if `left ≤ x ≤ right`.  
// 4. Return the minimum count across all these coordinates.  
// Edge cases:  
// - If `m` is very large, intervals may cover the whole line, so minimum coverage equals `N`.  
// - Duplicate positions lead to overlapping intervals; must count each sensor separately.  
// - Coordinates might be the same (e.g., `left == right`), so deduplicate to avoid redundant work, but counting must still be done per coordinate.  
// Complexity: O(N log N) due to sorting/dedup, and O(N^2) worst-case if we naively count for each coordinate. Better: sweep line using events. But since N ≤ 100000 in the original problem, O(N log N) is acceptable. We'll use a map from coordinate → count of intervals covering that coordinate. For each interval `[a,b]`, increment coverage for all coordinates between `a` and `b` inclusive. That's O(N * K) where K is number of distinct coordinates, potentially O(N^2). Instead, we can use a difference array on sorted coordinates: for each interval, find the indices of `a` and `b` in the sorted vector (using lower_bound) and add +1 to a difference array, then prefix sum to get coverage per coordinate. This yields O(N log N).  
// However, for a standalone task, a simpler O(N * K) approach is acceptable if N ≤ 1000, but the original snippet uses N up to 100000, so we'll present an efficient O(N log N) sweep-line with difference array.  
// We'll implement: sort all unique coordinates. For each interval, `left_idx = lower_bound(coords.begin(), coords.end(), left)`, `right_idx = lower_bound(... right)`. Since coords includes all boundaries, these indices are exact. Then `diff[left_idx]++`, `diff[right_idx+1]--`. After prefix sum, the coverage at each coordinate is known. The minimum over all coordinates is the answer.  
// Time: O(N log N) for sorting and each lower_bound. Space: O(N).
#include <vector>
#include <algorithm>
#include <cstdint>

// Returns the minimum number of sensors covering any point from 0 to L, inclusive.
// Each sensor at position p covers [max(0, p - m), min(p + m, L)].
int minimumCoveredPoints(int n, long long m, long long l,
                         const std::vector<long long>& positions) {
    // Collect all relevant coordinates: 0, L, and all interval endpoints.
    std::vector<long long> coords;
    coords.reserve(2 * n + 2);
    coords.push_back(0);
    coords.push_back(l);

    std::vector<std::pair<long long, long long>> intervals;
    intervals.reserve(n);

    for (int i = 0; i < n; ++i) {
        long long left = std::max(0LL, positions[i] - m);
        long long right = std::min(positions[i] + m, l);
        intervals.emplace_back(left, right);
        coords.push_back(left);
        coords.push_back(right);
    }

    // Sort and unique the coordinates.
    std::sort(coords.begin(), coords.end());
    coords.erase(std::unique(coords.begin(), coords.end()), coords.end());

    // Difference array to compute coverage at each coordinate.
    std::vector<int> diff(coords.size() + 1, 0);

    for (const auto& interval : intervals) {
        long long left = interval.first;
        long long right = interval.second;

        // Find indices of these coordinates (guaranteed to be present).
        int left_idx = static_cast<int>(std::lower_bound(coords.begin(), coords.end(), left) - coords.begin());
        int right_idx = static_cast<int>(std::lower_bound(coords.begin(), coords.end(), right) - coords.begin());

        ++diff[left_idx];
        --diff[right_idx + 1];
    }

    // Prefix sum to get coverage at each coordinate.
    int min_coverage = n;  // At most n sensors cover any point.
    int current = 0;
    for (std::size_t i = 0; i < coords.size(); ++i) {
        current += diff[i];
        min_coverage = std::min(min_coverage, current);
    }

    return min_coverage;
}
#include <cassert>
#include <vector>

// Function declaration from the solution (placed here for completeness).
int minimumCoveredPoints(int n, long long m, long long l,
                         const std::vector<long long>& positions);

int main() {
    // Single sensor far from ends, covers only itself.
    // Line length 100, radius 1, sensor at 50 → covers [49,51].
    // Points 0..48 not covered, so min = 0.
    assert(minimumCoveredPoints(1, 1, 100, {50}) == 0);

    // Two sensors overlapping at a single point.
    // Sensor at 10 (radius 5) covers [5,15]; sensor at 20 (radius 5) covers [15,25].
    // Point 15 is covered by both, but points 0..4 are covered by none → min=0.
    assert(minimumCoveredPoints(2, 5, 30, {10, 20}) == 0);

    // All sensors cover entire line.
    // Two sensors with huge radius → all points covered by both.
    assert(minimumCoveredPoints(2, 100, 10, {5, 7}) == 2);

    // Sensors at 0 and L, small radius, min uncovered point exists.
    // Line length 10, radius 1, sensors at 0 and 10.
    // Sensor0 covers [0,1]; sensor1 covers [9,10]. Point 2..8 not covered → min=0.
    assert(minimumCoveredPoints(2, 1, 10, {0, 10}) == 0);

    // Three sensors with partial overlap: min coverage should be 1.
    // Sensors at 5 (radius 3 → [2,8]), 10 (radius 2 → [8,12]), 15 (radius 3 → [12,18]).
    // Point 2 only covered by first, point 12 only by third, so min=1.
    assert(minimumCoveredPoints(3, 3, 20, {5, 10, 15}) == 1);

    // Duplicate sensors: two at same position, one elsewhere.
    // Line 100, radius 4, two sensors at 50, one at 60.
    // Covers [46,54] (twice) and [56,64] (once). Gap [55] zero? Actually 55 not covered → min=0.
    assert(minimumCoveredPoints(3, 4, 100, {50, 50, 60}) == 0);

    // All sensors exactly cover the whole line with gaps only at boundaries? 
    // Line length 5, radius 2.5, sensors at 0,2.5,5 → intervals [0,2.5],[0,5],[2.5,5].
    // Every point from 0 to 5 covered at least once, but each endpoint covered once → min=1.
    assert(minimumCoveredPoints(3, 2, 5, {0, 2, 5}) == 1);

    // Larger radius causes full overlap everywhere.
    // Line length 10, radius 10, sensors at 0 and 10 → both cover [0,10] → min=2.
    assert(minimumCoveredPoints(2, 10, 10, {0, 10}) == 2);

    // Empty input? Not required, but test with n=0 → no coverage → min=0.
    assert(minimumCoveredPoints(0, 5, 10, {}) == 0);

    // Single sensor with radius covering whole line → min=1 everywhere.
    assert(minimumCoveredPoints(1, 100, 5, {3}) == 1);

    return 0;
}
