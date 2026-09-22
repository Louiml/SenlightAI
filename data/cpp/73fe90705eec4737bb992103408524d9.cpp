Given a list of source control points and their corresponding destination control points, write a standalone C++ function that performs inverse distance weighting (IDW) warping on a single pixel coordinate. The function should take an integer `x`, integer `y`, a vector of source points (each with `.x` and `.y` fields), and a vector of destination points (same structure), and return a `std::pair<int, int>` representing the warped coordinate. The weight for each source point should be the inverse of the squared Euclidean distance (i.e., `1 / ((x - sx)^2 + (y - sy)^2)`), normalized so the weights sum to 1. The warped output position is `sum_i (weight_i * (dest_i + (x - source_i)))` for the x-coordinate, and similarly for y. Handle the edge case where the input source point list is empty by returning the original `(x, y)` unchanged. Also handle a potential division-by-zero: if the pixel exactly coincides with a source point, that point should dominate fully (i.e., the warped result is exactly that source point's corresponding destination plus offset zero), and the function should avoid division by zero in the weight computation.

// The core algorithm iterates over each source point, computing the inverse squared Euclidean distance from the input pixel to that source point. These raw weights are accumulated into a total sum `Sigma`. After computing all raw weights, each raw weight is normalized by dividing by `Sigma`, producing a normalized weight in `[0,1]`. For each normalized weight, the contribution to the new x coordinate is `weight * (dest_x + x - source_x)`, and similarly for y. Summing these contributions gives the final floating-point coordinates, which are then cast to `int` (truncation toward zero). Edge cases: (1) empty source list → return original coordinates. (2) If the pixel coincides exactly with a source point, the raw weight for that point becomes infinite (division by zero). We must detect this case explicitly (e.g., when the squared distance is exactly 0) and immediately return the destination point for that source plus the offset (0,0), which simplifies to the destination point itself. Without this special case, division by zero would cause undefined behavior. Time complexity is O(n) per pixel, where n is the number of source points, since each point is processed once. Space complexity is O(n) for the temporary weight and sigma vectors (though the vector of weights could be avoided by computing normalized weights in a second pass; the code as given uses O(n) extra space for the weights vector). The solution below uses a two-pass approach to avoid storing raw weights: first compute `Sigma`, then iterate again to compute the sum, maintaining O(1) extra space beyond the input vectors.

#include <utility>   // for std::pair
#include <vector>    // for std::vector
#include <cmath>     // for std::pow (though we use manual multiplication)

// A simple 2D point structure with x and y public members.
struct Point {
    float x;
    float y;
};

// Perform inverse distance weighting warping on a pixel coordinate.
// source: control points in the original image.
// dest: corresponding control points in the target image.
// Returns the warped coordinate as (int, int).
std::pair<int, int> idw_warp(
    int x,
    int y,
    const std::vector<Point>& source,
    const std::vector<Point>& dest)
{
    // If no control points, return the input unchanged.
    if (source.empty()) {
        return { x, y };
    }

    // Validate that source and dest have the same length.
    // (According to the problem, they always do, but we guard.)
    if (source.size() != dest.size()) {
        // In a robust implementation, we might throw, but here we return input.
        return { x, y };
    }

    // First pass: compute total sum of inverse squared distances.
    // Also detect if the pixel coincides exactly with a source point.
    double total_weight = 0.0;
    for (size_t i = 0; i < source.size(); ++i) {
        double dx = static_cast<double>(x) - source[i].x;
        double dy = static_cast<double>(y) - source[i].y;
        double dist_sq = dx * dx + dy * dy;
        if (dist_sq == 0.0) {
            // Exactly on a control point: return its destination directly.
            // The offset (x - source.x, y - source.y) is zero here.
            return { static_cast<int>(dest[i].x),
                     static_cast<int>(dest[i].y) };
        }
        total_weight += 1.0 / dist_sq;
    }

    // Second pass: compute normalized weighted sum.
    double new_x = 0.0;
    double new_y = 0.0;
    for (size_t i = 0; i < source.size(); ++i) {
        double dx = static_cast<double>(x) - source[i].x;
        double dy = static_cast<double>(y) - source[i].y;
        double dist_sq = dx * dx + dy * dy;
        double weight = (1.0 / dist_sq) / total_weight;

        // Contribution to the warped position.
        new_x += weight * (dest[i].x + dx);
        new_y += weight * (dest[i].y + dy);
    }

    return { static_cast<int>(new_x), static_cast<int>(new_y) };
}

#include <cassert>
#include <cmath>
#include <vector>

// Point struct is assumed to be defined in the solution above.
// In a real test file, you would include the solution header.
// For this self-contained test, we duplicate the Point definition.

struct Point {
    float x;
    float y;
};

// Copy of the solution function (or just declare it if linking).
std::pair<int, int> idw_warp(
    int x, int y,
    const std::vector<Point>& source,
    const std::vector<Point>& dest);

int main() {
    // Case 1: empty source list -> return original.
    std::vector<Point> empty_src;
    std::vector<Point> empty_dst;
    assert(idw_warp(10, 20, empty_src, empty_dst) == std::make_pair(10, 20));

    // Case 2: single control point, pixel far away.
    // With one point, weight is 1.0, result = dest + (x - src).
    std::vector<Point> src1 = { {5.0f, 5.0f} };
    std::vector<Point> dst1 = { {100.0f, 200.0f} };
    // x=10, y=10 -> offset (5,5) -> result (105, 205)
    assert(idw_warp(10, 10, src1, dst1) == std::make_pair(105, 205));

    // Case 3: exactly on a source point -> return its destination.
    std::vector<Point> src2 = { {3.0f, 4.0f}, {10.0f, 10.0f} };
    std::vector<Point> dst2 = { {30.0f, 40.0f}, {80.0f, 90.0f} };
    // Pixel (3,4) is exactly on first source -> output (30,40)
    assert(idw_warp(3, 4, src2, dst2) == std::make_pair(30, 40));

    // Case 4: two points, symmetric middle.
    // Source at (0,0) dest (10,0) and source (10,0) dest (0,0).
    // Pixel at (5,0): distances to both sources equal, weights 0.5 each.
    // Offset from source0: (5,0), contribution: 0.5*(10+5,0+0)=7.5,0
    // Offset from source1: (-5,0), contribution: 0.5*(0-5,0+0)=-2.5,0
    // Sum = (5.0, 0) -> truncates to (5,0).
    std::vector<Point> src3 = { {0.0f, 0.0f}, {10.0f, 0.0f} };
    std::vector<Point> dst3 = { {10.0f, 0.0f}, {0.0f, 0.0f} };
    assert(idw_warp(5, 0, src3, dst3) == std::make_pair(5, 0));

    // Case 5: asymmetrical weights, check correct weighted sum.
    // Source A at (0,0) dest (0,0), Source B at (10,0) dest (100,0).
    // Pixel at (0,0) is on A -> should return dest A (0,0), handled by special case.
    // Pixel at (1,0): distances: to A=1, to B=81 -> weights: 1/(1) vs 1/(81) normalized.
    // total = 1 + 1/81 = 82/81. wA = (1)/(82/81)=81/82, wB = (1/81)/(82/81)=1/82.
    // offsetA = (1-0,0)=1 -> contributionA x = (81/82)*(0+1)=81/82
    // offsetB = (1-10,-0)=-9 -> contributionB x = (1/82)*(100-9)=91/82
    // sum = (81+91)/82 = 172/82 ≈ 2.09756 -> truncates to 2.
    // y all zeros. So result (2,0)
    std::vector<Point> src4 = { {0.0f, 0.0f}, {10.0f, 0.0f} };
    std::vector<Point> dst4 = { {0.0f, 0.0f}, {100.0f, 0.0f} };
    assert(idw_warp(1, 0, src4, dst4) == std::make_pair(2, 0));

    // Case 6: negative coordinates.
    std::vector<Point> src5 = { {-2.0f, -3.0f}, {4.0f, 5.0f} };
    std::vector<Point> dst5 = { {10.0f, 20.0f}, {30.0f, 40.0f} };
    // Pixel at (-2,-3) is exactly on first source -> returns (10,20)
    assert(idw_warp(-2, -3, src5, dst5) == std::make_pair(10, 20));

    // Case 7: float truncation behavior with near-zero.
    // Single source at (0,0) dest (0,0). Pixel (0,0) -> returns (0,0)
    std::vector<Point> src6 = { {0.0f, 0.0f} };
    std::vector<Point> dst6 = { {0.0f, 0.0f} };
    assert(idw_warp(0, 0, src6, dst6) == std::make_pair(0, 0));

    return 0;
}
