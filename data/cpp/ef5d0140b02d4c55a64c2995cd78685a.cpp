// Write a C++ function `std::vector<int> minimumCuts(const std::vector<std::pair<double,double>>& points)` that, given a non-empty set of distinct points in the plane (with real-valued coordinates), returns for each point the minimum number of other points that must be removed so that all remaining points (including the original point) lie on at least one closed half-plane whose boundary passes through the original point. In other words, for each point `i`, you may delete some other points; after deletion, there must exist a straight line through point `i` such that all remaining points are on one side of or on that line. The function must return a vector of integers, where the `i`-th element is the minimum number of deletions required for point `i`. Note: if the set has only one point, the answer for that point is 0 (no deletions needed). Points are distinct, but multiple points may share the same angle as seen from the reference point (i.e., collinear with the reference point). The coordinates are given with sufficient precision; treat all floating‑point comparisons using a tolerance of `1e-9`.
// The core idea is to fix each point as the “reference” point and treat it as the center of a coordinate system. For every other point, compute the angle of the vector from the reference to that point using `atan2`. After sorting these angles, we want to find, for the reference point, the smallest number of points to discard so that the remaining points lie in some closed half‑plane whose boundary passes through the reference. This is equivalent to finding an angular interval of length `π` (180°) that contains as many points as possible; then the answer for that reference is `n-1 - maxInHalfPlane`, because we keep the points inside that half‑plane and delete the rest (excluding the reference itself).  
// To find the maximum number of points that fit in a half‑plane, we sort the angles and duplicate the array by adding `2π` to each angle, then use a two‑pointer (sliding window) technique. For each starting index `h1` (grouping identical angles together to handle collinear points), we advance a pointer `t1` to the end of the group of identical angles, then advance `h2` to the first angle that differs by at least `π` (strictly less than `π`), and `t2` to the first angle that is greater than or equal to `π` after the start. The number of points inside a half‑plane containing the entire group of identical angles at `h1` is either `h2 - t1` (points strictly between the group and the opposite boundary) or `cntAng - (t2 - h1)` (points outside the opposite window, i.e., the wrap‑around side). We take the minimum deletions as `min(n-1 - count)` over all groups.  
// Edge cases: When there are only two points, for each reference the other point can be kept, so the answer is 0. When many points share the same angle (collinear with reference), the grouping ensures we don’t split them incorrectly. The total time is O(n² log n) due to sorting for each of the n references, and space is O(n) for the angle array.
#include <vector>
#include <algorithm>
#include <cmath>
#include <utility>

const double PI = acos(-1.0);
const double EPS = 1e-9;

inline int sgn(double a) {
    return a > EPS ? 1 : (a < -EPS ? -1 : 0);
}

// Compute the minimum number of deletions for each point so that all remaining points lie in a closed half-plane through that point.
std::vector<int> minimumCuts(const std::vector<std::pair<double, double>>& points) {
    int n = static_cast<int>(points.size());
    std::vector<int> result(n, 0);

    if (n <= 1) {
        // For one point, answer is 0.
        return result;
    }

    for (int i = 0; i < n; ++i) {
        std::vector<double> angles;
        angles.reserve(n - 1);
        const double xi = points[i].first;
        const double yi = points[i].second;

        for (int j = 0; j < n; ++j) {
            if (j != i) {
                double dx = points[j].first - xi;
                double dy = points[j].second - yi;
                angles.push_back(atan2(dy, dx));
            }
        }

        std::sort(angles.begin(), angles.end());

        int m = n - 1; // number of angles
        // Duplicate the angles with +2π to handle wrap-around.
        std::vector<double> ext(2 * m);
        for (int j = 0; j < m; ++j) {
            ext[j] = angles[j];
            ext[j + m] = angles[j] + 2 * PI;
        }

        int maxKeep = 0;
        int h1 = 0;
        int t1 = 0;
        int h2 = 0;
        int t2 = 0;

        while (h1 < m) {
            // Move t1 to the end of the group with same angle as h1
            while (t1 < m && sgn(ext[t1] - ext[h1]) == 0) t1++;

            // Move h2 to the first angle that is >= ext[h1] + PI (strictly less than)
            while (h2 < m && sgn(ext[h2] - ext[h1] - PI) < 0) h2++;

            // Move t2 to the first angle that is > ext[h1] + PI (i.e., >= PI + EPS)
            while (t2 < m && sgn(ext[t2] - ext[h1] - PI) <= 0) t2++;

            // Number of points inside the half-plane defined by the group at h1:
            // Between the group end and h2 (strictly less than opposite boundary)
            int countA = h2 - t1;
            // Also the complement window (wrap-around side)
            int countB = m - (t2 - h1);
            int count = std::max(countA, countB);
            if (count > maxKeep) maxKeep = count;

            // Move to the next group
            h1 = t1;
            h2 = std::max(h2, t1);
            t2 = std::max(t2, t1);
        }

        // We need to delete all points that are not in the chosen half-plane.
        result[i] = (n - 1) - maxKeep;
    }

    return result;
}
#include <cassert>
#include <vector>
#include <utility>
#include <cmath>

// (Include the solution code here or assume it is above)

int main() {
    // Single point
    {
        std::vector<std::pair<double,double>> pts = {{0.0, 0.0}};
        auto res = minimumCuts(pts);
        assert(res.size() == 1 && res[0] == 0);
    }

    // Two points: each needs 0 deletions
    {
        std::vector<std::pair<double,double>> pts = {{0.0, 0.0}, {1.0, 1.0}};
        auto res = minimumCuts(pts);
        assert(res.size() == 2 && res[0] == 0 && res[1] == 0);
    }

    // Three points forming a triangle: for each point, one of the other two must be deleted because the remaining two are on opposite sides of any line through the vertex? 
    // For an equilateral triangle, the other two points are at angles 120° and 240° from each vertex, so they do not lie in any half-plane of width 180°. Thus each vertex requires deleting 1 point.
    {
        std::vector<std::pair<double,double>> pts = {{0.0, 0.0}, {1.0, 0.0}, {0.5, std::sqrt(3.0)/2.0}};
        auto res = minimumCuts(pts);
        assert(res.size() == 3);
        for (int v : res) {
            assert(v == 1);
        }
    }

    // Four points: all on a line (collinear from each reference? Actually from a middle point, the other three are on both sides). 
    // For the middle point (index 1), the other three are at angles 0°, 180°, and 0°? Let's use points (0,0), (1,0), (2,0), (3,0).
    // For point (1,0): other points are at angles: (0,0) -> π (or -π), (2,0) -> 0, (3,0) -> 0. So all lie in half-plane of width π (e.g., from -π to 0). So no deletions needed.
    // For point (0,0): others at angles 0,0,0 all same, so no deletions.
    // So all answers 0.
    {
        std::vector<std::pair<double,double>> pts = {{0.0, 0.0}, {1.0, 0.0}, {2.0, 0.0}, {3.0, 0.0}};
        auto res = minimumCuts(pts);
        assert(res.size() == 4);
        for (int v : res) {
            assert(v == 0);
        }
    }

    // Five points: (0,0), (1,0), (0,1), (-1,0), (0,-1) (a cross). For the center (0,0), the other four are at angles 0,90,180,270. No half-plane of width 180 contains all four, so we need to delete at least 1 (the one opposite to the chosen half-plane). For each of the axis points, the other four include the opposite axis point at 180°, so they cannot fit all four either; each needs 1 deletion.
    {
        std::vector<std::pair<double,double>> pts = {
            {0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}, {-1.0, 0.0}, {0.0, -1.0}
        };
        auto res = minimumCuts(pts);
        assert(res.size() == 5);
        for (int v : res) {
            assert(v == 1);
        }
    }

    // Six points: all on a circle but with angles 0, 60, 120, 180, 240, 300 degrees. For each point, the opposite point is at exactly 180°, and the other four are within ±150°? Actually from 0°, the angles are 60,120,180,240,300 relative. A half-plane of width 180 can contain 0,60,120,180 (that's 4 points including the reference? wait the reference itself is not in the list). For point at 0°, the other five have angles 60,120,180,240,300. A half-plane from -30° to 150° contains 60,120,180 (3 points) but not 240,300. Another half-plane from 120° to 300° contains 180,240,300 (3). So max keep is 3, so deletions = (6-1)-3 = 2.
    // For each point, because it's symmetric, answer should be 2.
    {
        std::vector<std::pair<double,double>> pts;
        for (int k = 0; k < 6; ++k) {
            double angle = k * M_PI / 3.0;
            pts.push_back({std::cos(angle), std::sin(angle)});
        }
        auto res = minimumCuts(pts);
        assert(res.size() == 6);
        for (int v : res) {
            assert(v == 2);
        }
    }

    return 0;
}
