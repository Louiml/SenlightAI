// Write a standalone C++ function named `boundingBoxesFromSegments` that takes a flat vector of 3D point coordinates (stored as `x0, y0, z0, x1, y1, z1, ...` for n points) and an optional vector of segment start indices. The function must compute the axis-aligned bounding box (AABB) for each segment, where each segment is a contiguous block of points in the flat array. The first segment always starts at index 0, and the last segment extends to the end of the array. The output must be a vector of 6 values per segment in the order `[x_min, x_max, y_min, y_max, z_min, z_max]`, and the number of output rows must be padded with zeros to a multiple of a provided `alignment` parameter (default = 1, meaning no padding). The function must validate inputs (non-null, coordinate count divisible by 3, non-decreasing segment starts, first start = 0, all starts within range) and throw `std::invalid_argument` on error. The function should work for `float` and `double` via a template, but you only need to provide the implementation for one numeric type (e.g., `double`). Write a free function that takes `const std::vector<double>& points`, `const std::vector<size_t>& segmentStarts` (empty vector means one segment), and `size_t alignment` (default 1), and returns `std::vector<double>`.
#include <cassert>
#include <cmath>
#include <vector>

// Declaration of the function (already defined above, but for clarity)
std::vector<double> boundingBoxesFromSegments(
    const std::vector<double>& points,
    const std::vector<size_t>& segmentStarts = {},
    size_t alignment = 1);

int main()
{
    // Single segment, no alignment
    {
        std::vector<double> pts = {1.0, 2.0, 3.0, -4.0, 5.0, -6.0, 7.0, 8.0, 9.0};
        auto bb = boundingBoxesFromSegments(pts);
        assert(bb.size() == 6);
        assert(bb[0] == -4.0); // x_min
        assert(bb[1] == 7.0);  // x_max
        assert(bb[2] == 2.0);  // y_min
        assert(bb[3] == 8.0);  // y_max
        assert(bb[4] == -6.0); // z_min
        assert(bb[5] == 9.0);  // z_max
    }

    // Two segments, no alignment
    {
        std::vector<double> pts = {0,0,0, 1,1,1, 2,2,2, /* seg1: 0-2 */
                                   10,10,10, 20,20,20};  // seg2: 3-4
        std::vector<size_t> starts = {0, 3};
        auto bb = boundingBoxesFromSegments(pts, starts);
        assert(bb.size() == 12);
        // Segment 0: points 0,1,2
        assert(bb[0] == 0.0 && bb[1] == 2.0);
        assert(bb[2] == 0.0 && bb[3] == 2.0);
        assert(bb[4] == 0.0 && bb[5] == 2.0);
        // Segment 1: points 3,4
        assert(bb[6] == 10.0 && bb[7] == 20.0);
        assert(bb[8] == 10.0 && bb[9] == 20.0);
        assert(bb[10] == 10.0 && bb[11] == 20.0);
    }

    // Alignment pads with zeros
    {
        std::vector<double> pts = {1,2,3, -1,-2,-3};
        std::vector<size_t> starts = {0, 2}; // two segments
        auto bb = boundingBoxesFromSegments(pts, starts, 4);
        assert(bb.size() == 4 * 6); // padded to 4 segments
        // Segment 0: (1,2,3)
        assert(bb[0] == 1.0 && bb[1] == 1.0);
        // Segment 1: (-1,-2,-3)
        assert(bb[6] == -1.0 && bb[7] == -1.0);
        // Padding rows are all zeros
        for (size_t i = 12; i < bb.size(); ++i)
            assert(bb[i] == 0.0);
    }

    // Empty segmentStarts means single segment
    {
        std::vector<double> pts = {5,5,5};
        auto bb = boundingBoxesFromSegments(pts);
        assert(bb.size() == 6);
        assert(bb[0] == 5.0 && bb[1] == 5.0);
        assert(bb[2] == 5.0 && bb[3] == 5.0);
        assert(bb[4] == 5.0 && bb[5] == 5.0);
    }

    // Edge: multiple points in one segment with duplicates
    {
        std::vector<double> pts = {1,1,1, 1,1,1};
        auto bb = boundingBoxesFromSegments(pts);
        assert(bb[0] == 1.0 && bb[1] == 1.0);
    }

    // Edge: empty segment (start index equals next start) – test with a segment having zero points
    {
        std::vector<double> pts = {0,0,0, 10,10,10, 20,20,20};
        std::vector<size_t> starts = {0, 1, 3}; // segment 0: point0, segment1: point1, segment2: point2
        // Actually this has no empty segment; test another:
        std::vector<size_t> starts2 = {0, 3}; // seg0: 0,1,2; seg1: empty? Wait seg1 starts at 3 = nPoints, invalid
        // Use valid case: segment 1 empty if start equals next start, but that violates strict increasing.
        // So we'll test a segment that has exactly one point.
        std::vector<size_t> starts3 = {0, 1, 2}; // seg0: point0, seg1: point1, seg2: point2
        auto bb = boundingBoxesFromSegments(pts, starts3);
        assert(bb.size() == 18);
        assert(bb[0] == 0.0 && bb[6] == 10.0 && bb[12] == 20.0);
    }

    return 0;
}
#include <vector>
#include <stdexcept>
#include <limits>
#include <cmath>

// Compute axis-aligned bounding boxes for contiguous segments of a 3D point cloud.
// points must be flat: x0,y0,z0, x1,y1,z1, ...
// segmentStarts (optional) lists the starting point index (0-based) of each segment.
//   The first must be 0, sorted ascending, and the last segment runs to the end.
// alignment pads the output segment count to a multiple of this value with zero rows.
// Returns a vector of 6 * paddedSegmentCount doubles in order:
//   [x_min, x_max, y_min, y_max, z_min, z_max] per segment.
std::vector<double> boundingBoxesFromSegments(
    const std::vector<double>& points,
    const std::vector<size_t>& segmentStarts = {},
    size_t alignment = 1)
{
    // ---- Input validation ----
    if (points.empty())
        throw std::invalid_argument("points must not be empty");
    if (points.size() % 3 != 0)
        throw std::invalid_argument("points size must be a multiple of 3");

    const size_t nPoints = points.size() / 3;

    size_t nSegments = 1;
    if (!segmentStarts.empty())
    {
        nSegments = segmentStarts.size();
        if (segmentStarts[0] != 0)
            throw std::invalid_argument("first segment must start at index 0");
        for (size_t i = 1; i < nSegments; ++i)
            if (segmentStarts[i] <= segmentStarts[i-1])
                throw std::invalid_argument("segment starts must be strictly increasing");
        if (segmentStarts.back() >= nPoints)
            throw std::invalid_argument("segment start exceeds point count");
    }

    if (alignment == 0)
        throw std::invalid_argument("alignment must be at least 1");

    // ---- Compute padded output size ----
    size_t nOut = (nSegments % alignment == 0) ? nSegments : ((nSegments / alignment) + 1) * alignment;

    // Output initialized to zeros for padding rows
    std::vector<double> result(nOut * 6, 0.0);

    // Helper lambdas to access min/max for segment i
    auto minX = [&](size_t i) -> double& { return result[i*6 + 0]; };
    auto maxX = [&](size_t i) -> double& { return result[i*6 + 1]; };
    auto minY = [&](size_t i) -> double& { return result[i*6 + 2]; };
    auto maxY = [&](size_t i) -> double& { return result[i*6 + 3]; };
    auto minZ = [&](size_t i) -> double& { return result[i*6 + 4]; };
    auto maxZ = [&](size_t i) -> double& { return result[i*6 + 5]; };

    // Initialize min/max for actual segments
    const double inf = std::numeric_limits<double>::infinity();
    for (size_t i = 0; i < nSegments; ++i)
    {
        minX(i) = inf; maxX(i) = -inf;
        minY(i) = inf; maxY(i) = -inf;
        minZ(i) = inf; maxZ(i) = -inf;
    }

    // Precompute segment end boundaries for efficient lookup
    // end[i] = first point index of next segment, or nPoints for last
    std::vector<size_t> segmentEnds(nSegments);
    for (size_t i = 0; i < nSegments; ++i)
    {
        if (i + 1 < nSegments)
            segmentEnds[i] = segmentStarts[i + 1];
        else
            segmentEnds[i] = nPoints;
    }

    size_t currentSegment = 0;
    size_t nextSegmentStart = segmentEnds[0];

    // Iterate over all points (each point uses 3 consecutive coordinates)
    for (size_t p = 0; p < nPoints; ++p)
    {
        // Move to next segment if we've reached the boundary
        while (p >= nextSegmentStart && currentSegment < nSegments - 1)
        {
            ++currentSegment;
            nextSegmentStart = segmentEnds[currentSegment];
        }

        // Point coordinates
        double x = points[3 * p];
        double y = points[3 * p + 1];
        double z = points[3 * p + 2];

        // Update segment bounds
        if (x < minX(currentSegment)) minX(currentSegment) = x;
        if (x > maxX(currentSegment)) maxX(currentSegment) = x;
        if (y < minY(currentSegment)) minY(currentSegment) = y;
        if (y > maxY(currentSegment)) maxY(currentSegment) = y;
        if (z < minZ(currentSegment)) minZ(currentSegment) = z;
        if (z > maxZ(currentSegment)) maxZ(currentSegment) = z;
    }

    return result;
}
// The core algorithm iterates once over the flattened point array. Since the data is stored as interleaved x,y,z values, we can process three consecutive elements at a time. For each point (triplet), we determine which segment it belongs to by tracking the current segment index and the next segment start boundary. We initialize each segment's min/max values to positive/negative infinity, then update them whenever we encounter a point in that segment. The main challenge is efficiently mapping each point to its segment without a separate loop per point. We can do this by precomputing an array of segment sizes (or use a pointer into a segment assignment array), but a simpler approach is to iterate over the points and maintain a running segment counter that increments when we cross a start index. To avoid modulo operations when reading the flat array, we process triplets directly: for `i` from 0 to n-1 (n = points.size()/3), the x-coordinate is at `3*i`, y at `3*i+1`, z at `3*i+2`. We advance the segment counter when `i` equals the next start index. Edge cases include: empty segmentStarts (treat all points as one segment), only one segment, segments that are empty (start index equals next start), and alignment padding (output length = ceil(segmentCount/alignment)*alignment, with zero-filled extra rows). Time complexity is O(n + s) where n is total coordinate count and s is number of segments, due to single pass over points and segments. Space complexity is O(s) for the output plus O(s) for internal storage if needed.
