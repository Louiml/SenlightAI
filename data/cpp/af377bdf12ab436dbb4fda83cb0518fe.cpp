Write a C++ function named `countTrajectorySegments` that takes a vector of `double` values representing the field value `phi` at each time step of a stochastic trajectory (simulating the evolution in the provided cosmological code snippet), along with a positive integer `numBins`. The function should count how many times the trajectory crosses each bin boundary between consecutive field values (i.e., a sign change or crossing of a bin edge), and return a vector of `int` of size `numBins+1`, where index `i` corresponds to the boundary between bin `i-1` and bin `i` (where bin `0` spans from `-infinity` to the first boundary, and the last bin extends to `+infinity`). The bin boundaries are determined by dividing the range from `min(phi)` to `max(phi)` into `numBins` equal intervals, but boundaries at the exact minimum and maximum are not counted. Specifically, for each consecutive pair `(phi[i], phi[i+1])`, if the two values straddle a boundary value `b` (meaning one is <= b and the other > b, or vice versa, with equality not counted as crossing on the lower side), increment the count for that boundary index. If both values are equal to the boundary or on the same side, do not count. The function must handle empty input (return a zero-filled vector) and constant input (no crossings, return zero-filled vector). Provide the solution as a free function with appropriate `const` correctness.

// The solution iterates over consecutive pairs of field values. For each pair, it determines the minimum and maximum of the two values. If the two values are equal, no crossing exists. Otherwise, for each boundary `b` in the range between `minVal` and `maxVal` (exclusive on at least one side), we need to check if `b` lies strictly between the two values, but with the nuance that if `b` equals one of the endpoints, it is not counted as a crossing (since the trajectory touches but does not cross). The boundaries are precomputed as `min_phi + i * step` for `i = 1` to `numBins-1`, where `step = (max_phi - min_phi) / numBins`. For each pair, we can directly compute which boundary indices fall between the two values by using integer arithmetic: if `minVal < b < maxVal` (strictly between), then count. Using a loop over boundaries for each pair is fine for small `numBins`; the total time is O(n * numBins), where n is the number of field values. Space is O(numBins) for the result. Edge cases: empty input returns a vector of zeros of size `numBins+1`; constant input (max == min) returns zeros; negative values are handled naturally since we use floating-point comparisons. Important nuance: if `max_phi` equals `min_phi`, there are no boundaries, so return zeros. Also, the number of boundaries is `numBins+1` but only boundaries `1` to `numBins-1` are actual internal boundaries; index 0 and index `numBins` would correspond to the extremes, but since we never count crossings at the exact min or max (because the range is defined to include them), those indices always remain zero. The function should be robust to floating-point precision by using simple comparisons `minVal < b && b < maxVal` (strict inequality) to avoid counting exact touches.

#include <vector>
#include <algorithm>
#include <cstddef>

// Count how many times a trajectory crosses each bin boundary.
// Bins are defined by dividing the range [min(phi), max(phi)] into numBins equal intervals.
// Returns a vector of size numBins+1, where index i corresponds to boundary i (i=0 and numBins are always zero).
std::vector<int> countTrajectorySegments(const std::vector<double>& phi, int numBins) {
    // Validate input: numBins must be positive, but for empty or constant input we return zeros.
    if (numBins <= 0) {
        return std::vector<int>(numBins + 1, 0);
    }

    // Handle empty input
    if (phi.empty()) {
        return std::vector<int>(numBins + 1, 0);
    }

    // Find global min and max
    double minPhi = *std::min_element(phi.begin(), phi.end());
    double maxPhi = *std::max_element(phi.begin(), phi.end());

    // If all values are equal, no crossings
    if (maxPhi == minPhi) {
        return std::vector<int>(numBins + 1, 0);
    }

    // Precompute internal boundaries: indices 1..numBins-1
    double step = (maxPhi - minPhi) / numBins;
    std::vector<double> bounds(numBins + 1);
    for (int i = 0; i <= numBins; ++i) {
        bounds[i] = minPhi + i * step;
    }

    // Count crossings
    std::vector<int> crossings(numBins + 1, 0);

    for (std::size_t i = 1; i < phi.size(); ++i) {
        double val1 = phi[i-1];
        double val2 = phi[i];
        if (val1 == val2) continue;

        // Determine which side each value is on
        // For each internal boundary (index 1..numBins-1), check if the pair straddles it
        for (int b = 1; b < numBins; ++b) {
            double boundary = bounds[b];
            // Strict inequality: if boundary is exactly equal to either value, it's not a crossing
            if ((val1 < boundary && val2 > boundary) || (val1 > boundary && val2 < boundary)) {
                ++crossings[b];
            }
        }
    }

    return crossings;
}

#include <cassert>
#include <vector>

// Function signature from the solution
std::vector<int> countTrajectorySegments(const std::vector<double>& phi, int numBins);

int main() {
    // Basic case: crossing a single boundary
    std::vector<double> traj1 = {0.0, 5.0, 10.0};  // range [0,10], numBins=2 => boundaries at 5 and 10
    // Boundaries: index0=0, index1=5, index2=10. Internal boundaries: only index1=5
    // Crossings: (0,5) -> 5 is exactly at val2, not counted. (5,10) -> 10 is at val2, not counted. So count=0
    assert(countTrajectorySegments(traj1, 2) == std::vector<int>({0,0,0}));

    // Crossing a boundary strictly between values
    std::vector<double> traj2 = {0.0, 6.0, 12.0};  // range [0,12], numBins=3 => boundaries at 4,8,12
    // Boundaries: index0=0, index1=4, index2=8, index3=12. Internal: 4 and 8
    // Pair (0,6): crosses 4 -> count[1]++
    // Pair (6,12): crosses 8 -> count[2]++ ; 12 is exactly at end, not counted
    // Result: [0,1,1,0]
    assert(countTrajectorySegments(traj2, 3) == std::vector<int>({0,1,1,0}));

    // Empty input
    std::vector<double> empty;
    assert(countTrajectorySegments(empty, 4) == std::vector<int>({0,0,0,0,0}));

    // Constant input
    std::vector<double> constant = {3.0, 3.0, 3.0};
    assert(countTrajectorySegments(constant, 2) == std::vector<int>({0,0,0}));

    // Multiple crossings of same boundary
    std::vector<double> traj3 = {-1.0, 1.0, -1.0, 1.0};  // range [-1,1], numBins=2 => boundaries at -1,0,1? Actually min=-1, max=1, step=1, boundaries: -1,0,1. Internal boundary at 0
    // Pairs: (-1,1) crosses 0 -> count[1]++. (1,-1) crosses 0 -> count[1]++. (-1,1) crosses 0 -> count[1]++
    // Total 3 crossings at index1
    assert(countTrajectorySegments(traj3, 2) == std::vector<int>({0,3,0}));

    // Check that exact equality on boundary is not counted
    std::vector<double> traj4 = {0.0, 5.0, 0.0};  // range [0,5], numBins=1 => boundaries: 0,5. No internal boundaries
    // Internal boundaries: none (numBins-1 = 0), so all zeros
    assert(countTrajectorySegments(traj4, 1) == std::vector<int>({0,0}));

    // Boundary touching but not crossing: value exactly at boundary, then moves away
    std::vector<double> traj5 = {2.0, 4.0, 6.0};  // range [2,6], numBins=2 => boundaries 2,4,6. Internal boundary at 4
    // Pair (2,4): 4 is exactly at val2, not counted. Pair (4,6): 4 is at val1, not counted. So count=0
    assert(countTrajectorySegments(traj5, 2) == std::vector<int>({0,0,0}));

    return 0;
}
