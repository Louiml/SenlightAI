/*
Write a standalone C++ function `bool fitLine2D(const std::vector<gp_Pnt2d>& points, double tolerance, double& deviation, gp_Lin2d& line)` that determines whether a set of 2D points can be approximated by a straight line within a given Euclidean tolerance. The function must return `true` if all points lie within `tolerance` distance from the best-fitting line (determined by the two farthest-apart points in the set), and `false` otherwise. When `true`, the function must output the maximum observed deviation from the line using the `deviation` output parameter, and provide the fitted line in the `line` output parameter using OpenCascade’s `gp_Lin2d` type. The input must contain at least two distinct points; if fewer than two points are provided, or if all points are coincident (so that no meaningful direction can be established), the function returns `false`. The tolerance is a positive value in the same units as the point coordinates; you may assume it is always > 0. The function must be self-contained with respect to includes and not rely on any external plotting or logging utilities.
*/

#include <gp_Lin2d.hxx>
#include <gp_Pnt2d.hxx>
#include <gp_Vec2d.hxx>
#include <gp_Dir2d.hxx>
#include <Precision.hxx>
#include <vector>
#include <cmath>

// Check whether a set of 2D points can be approximated by a straight line within tolerance.
// If true, returns the fitted line and the maximum deviation.
bool fitLine2D(const std::vector<gp_Pnt2d>& points,
               double tolerance,
               double& deviation,
               gp_Lin2d& line)
{
    const int n = static_cast<int>(points.size());

    // Need at least two distinct points to define a line.
    if (n < 2)
        return false;

    // Find the pair of points that are farthest apart.
    double maxSqDist = 0.0;
    int idx1 = -1, idx2 = -1;
    for (int i = 0; i < n; ++i)
    {
        for (int j = i + 1; j < n; ++j)
        {
            const double sqDist = points[i].SquareDistance(points[j]);
            if (sqDist > maxSqDist)
            {
                maxSqDist = sqDist;
                idx1 = i;
                idx2 = j;
            }
        }
    }

    // If the farthest pair is essentially coincident, no line is defined.
    const double minSqDist = Precision::Confusion() * Precision::Confusion();
    if (maxSqDist < minSqDist)
        return false;

    // Construct candidate line from the two farthest points.
    const gp_Pnt2d& p1 = points[idx1];
    const gp_Pnt2d& p2 = points[idx2];
    gp_Vec2d vec(p1, p2);
    gp_Dir2d dir(vec);
    line = gp_Lin2d(p1, dir);

    // Check that all points lie within the tolerance distance from the line.
    const double tolSq = tolerance * tolerance;
    double maxDevSq = 0.0;
    for (int i = 0; i < n; ++i)
    {
        const double sqDist = line.SquareDistance(points[i]);
        if (sqDist > tolSq)
            return false;
        if (sqDist > maxDevSq)
            maxDevSq = sqDist;
    }

    deviation = std::sqrt(maxDevSq);
    return true;
}

#include <cassert>
#include <vector>
#include <gp_Pnt2d.hxx>

// The solution function is declared here (or included from the solution header).
// Assume it is available in this translation unit.

int main()
{
    // Test 1: Three points exactly collinear.
    {
        std::vector<gp_Pnt2d> pts = { gp_Pnt2d(0.0, 0.0), gp_Pnt2d(1.0, 1.0), gp_Pnt2d(2.0, 2.0) };
        double dev;
        gp_Lin2d line;
        assert(fitLine2D(pts, 1e-6, dev, line) == true);
        assert(dev < 1e-6);
    }

    // Test 2: Points nearly collinear but within tolerance.
    {
        std::vector<gp_Pnt2d> pts = { gp_Pnt2d(0.0, 0.0), gp_Pnt2d(1.0, 0.1), gp_Pnt2d(2.0, 0.2) };
        double dev;
        gp_Lin2d line;
        assert(fitLine2D(pts, 0.15, dev, line) == true);
        assert(dev <= 0.15);
    }

    // Test 3: Points that deviate beyond tolerance.
    {
        std::vector<gp_Pnt2d> pts = { gp_Pnt2d(0.0, 0.0), gp_Pnt2d(1.0, 1.0), gp_Pnt2d(2.0, 0.0) };
        double dev;
        gp_Lin2d line;
        assert(fitLine2D(pts, 0.1, dev, line) == false);
    }

    // Test 4: Only one point -> returns false.
    {
        std::vector<gp_Pnt2d> pts = { gp_Pnt2d(1.0, 2.0) };
        double dev;
        gp_Lin2d line;
        assert(fitLine2D(pts, 1e-3, dev, line) == false);
    }

    // Test 5: All points coincident -> returns false.
    {
        std::vector<gp_Pnt2d> pts = { gp_Pnt2d(0.0, 0.0), gp_Pnt2d(0.0, 0.0), gp_Pnt2d(0.0, 0.0) };
        double dev;
        gp_Lin2d line;
        assert(fitLine2D(pts, 1e-3, dev, line) == false);
    }

    // Test 6: Two distinct points always pass.
    {
        std::vector<gp_Pnt2d> pts = { gp_Pnt2d(3.0, -1.0), gp_Pnt2d(-2.0, 4.0) };
        double dev;
        gp_Lin2d line;
        assert(fitLine2D(pts, 1e-6, dev, line) == true);
        assert(dev < 1e-6);
    }

    // Test 7: Vertical line (all x equal) should pass.
    {
        std::vector<gp_Pnt2d> pts = { gp_Pnt2d(1.0, 0.0), gp_Pnt2d(1.0, 10.0), gp_Pnt2d(1.0, -5.0) };
        double dev;
        gp_Lin2d line;
        assert(fitLine2D(pts, 1e-6, dev, line) == true);
        assert(dev < 1e-6);
    }

    // Test 8: Tolerance too small for a slight deviation.
    {
        std::vector<gp_Pnt2d> pts = { gp_Pnt2d(0.0, 0.0), gp_Pnt2d(1.0, 0.001), gp_Pnt2d(2.0, 0.002) };
        double dev;
        gp_Lin2d line;
        assert(fitLine2D(pts, 1e-4, dev, line) == false);
    }

    return 0;
}

// The core idea is to first identify the two points in the input set that are farthest apart. These two points define a candidate axis for the line. If the farthest distance is below a tiny threshold based on `Precision::Confusion()`, it means all points are nearly coincident, and no line can be meaningfully defined, so return `false`. The candidate line is constructed from the first of these two points and the direction vector pointing to the second. Then, for every point in the input, compute the squared distance from the point to this line. If any squared distance exceeds `tolerance * tolerance`, the points are not collinear within the given tolerance, and return `false`. Otherwise, track the maximum squared distance encountered; after the loop, compute the square root to get the maximum deviation and store it in the `deviation` parameter, and store the candidate line in the `line` parameter. The algorithm runs in O(n^2) time in the worst case due to the nested loop used to find the farthest pair, and uses O(1) auxiliary space beyond the input vector. Edge cases include an empty or single‑point input, degenerate point sets where all points coincide, and the need to handle floating‑point comparisons carefully by using squared distances to avoid unnecessary square roots until the final step.
