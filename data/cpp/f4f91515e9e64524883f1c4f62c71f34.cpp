Write a C++ function named `rayCubeExitTime` that, given a ray (with source point `(sx, sy, sz)` and direction vector `(dx, dy, dz)`) and an axis-aligned bounding box defined by minimum corner `(minX, minY, minZ)` and maximum corner `(maxX, maxY, maxZ)`, returns the smallest positive parameter `t > 0` such that the point `(sx + t*dx, sy + t*dy, sz + t*dz)` lies exactly on one of the six faces of the box, but is strictly outside the box for all smaller positive `t` (i.e., the first exit point along the ray). If the ray never exits the box in the positive direction (including when the ray starts inside and never leaves, or when the direction is effectively zero on all axes and the ray stays inside forever), return `-1.0`. Use double precision and treat any value with absolute difference less than `1e-9` as zero for direction components. The box is considered closed (including its boundaries), and the ray may start anywhere (inside, outside, or on the boundary). The function must handle all cases where the direction component on any axis is zero (the ray is parallel to that axis pair of faces) — in that case, that axis contributes no exit constraint. Provide a self-contained free function without a main.
#include <cassert>
#include <cmath>

// Declaration (in real code this would be in a header)
double rayCubeExitTime(
    double sx, double sy, double sz,
    double dx, double dy, double dz,
    double minX, double minY, double minZ,
    double maxX, double maxY, double maxZ
);

int main() {
    // Ray from inside box moving +x: exits at x=maxX
    double t = rayCubeExitTime(0,0,0, 1,0,0, -1,-1,-1, 1,1,1);
    assert(std::abs(t - 1.0) < 1e-9);

    // Ray starting at center, moving diagonally: exits at corner distance sqrt(3)
    t = rayCubeExitTime(0,0,0, 1,1,1, -1,-1,-1, 1,1,1);
    assert(std::abs(t - std::sqrt(3.0)) < 1e-9);

    // Ray starting outside pointing away: no exit
    t = rayCubeExitTime(2,0,0, 1,0,0, -1,-1,-1, 1,1,1);
    assert(t < 0);

    // Ray starting on boundary moving inward: no positive exit (t=0 rejected)
    t = rayCubeExitTime(1,0,0, -1,0,0, -1,-1,-1, 1,1,1);
    assert(t < 0);

    // Ray starting outside but passing through: exits on opposite side
    t = rayCubeExitTime(-2,0,0, 1,0,0, -1,-1,-1, 1,1,1);
    assert(std::abs(t - 3.0) < 1e-9);

    // Direction zero on y,z, inside box: exits through x only
    t = rayCubeExitTime(0,0,0, 0.5,0,0, -1,-1,-1, 1,1,1);
    assert(std::abs(t - 2.0) < 1e-9);

    // Ray that never exits because direction is zero and starts inside
    t = rayCubeExitTime(0,0,0, 0,0,0, -1,-1,-1, 1,1,1);
    assert(t < 0);

    // Ray starting on corner moving outward: exit is immediate (t=0 rejected, next positive maybe none)
    t = rayCubeExitTime(1,1,1, 1,1,1, -1,-1,-1, 1,1,1);
    assert(t < 0);

    // Ray passing exactly through a corner: should find exit at some t
    t = rayCubeExitTime(-2,-2,-2, 1,1,1, -1,-1,-1, 1,1,1);
    assert(std::abs(t - 1.0) < 1e-9);

    // Ray with one zero direction, starts outside, exits through a side
    t = rayCubeExitTime(-2,0,0, 1,0,1, -1,-1,-1, 1,1,1);
    // y stays 0, z increases, x reaches max at t=3, z reaches max at t=2, so exit at t=2
    assert(std::abs(t - 2.0) < 1e-9);

    return 0;
}
#include <cmath>
#include <limits>

// Return the smallest positive t > 0 where the ray exits the axis-aligned box,
// or -1.0 if no exit occurs in the positive direction.
double rayCubeExitTime(
    double sx, double sy, double sz,
    double dx, double dy, double dz,
    double minX, double minY, double minZ,
    double maxX, double maxY, double maxZ
) {
    const double epsDir = 1e-9;
    const double epsT = 1e-12;

    double bestT = -1.0;
    double candidates[6][4] = {
        {sx, dx, minX, 0}, // x min
        {sx, dx, maxX, 0}, // x max
        {sy, dy, minY, 1}, // y min
        {sy, dy, maxY, 1}, // y max
        {sz, dz, minZ, 2}, // z min
        {sz, dz, maxZ, 2}  // z max
    };

    for (int i = 0; i < 6; ++i) {
        double s = candidates[i][0];
        double d = candidates[i][1];
        double boundary = candidates[i][2];
        int axis = static_cast<int>(candidates[i][3]);

        if (std::abs(d) < epsDir) continue; // parallel to this axis

        double t = (boundary - s) / d;
        if (t <= epsT) continue; // t must be strictly positive

        // Compute point at t
        double px = sx + t * dx;
        double py = sy + t * dy;
        double pz = sz + t * dz;

        // Check if it's outside the box (must be outside to be an exit)
        bool outside = false;
        if (px < minX - epsT || px > maxX + epsT) outside = true;
        if (py < minY - epsT || py > maxY + epsT) outside = true;
        if (pz < minZ - epsT || pz > maxZ + epsT) outside = true;

        if (outside) {
            if (bestT < 0 || t < bestT) {
                bestT = t;
            }
        }
    }

    // If no candidate found, or the best is still negative, return -1
    return bestT;
}
// The core algorithm iterates over the three coordinate axes. For each axis `i`, the ray’s coordinate evolves as `pos_i(t) = start_i + t * dir_i`. To find when the ray exits through the two planes perpendicular to that axis, solve for `t` such that `pos_i(t) == min_i` and `pos_i(t) == max_i`. Each equation gives `t = (boundary - start_i) / dir_i` if `|dir_i| > 1e-9`; if the direction component is effectively zero, the ray never crosses that pair of planes, so we skip that axis. For each solved `t`, we only accept it if `t > 1e-12` (to avoid reporting t=0 when starting exactly on a boundary) and if at that `t` the ray is actually outside the box — but because we only consider the minimal positive t among all six planes and the box is convex, the smallest positive t from any plane is the first exit, provided that at that t the point is outside (which is guaranteed if the direction moves outward from the interior or from the boundary). However, to be safe, we should explicitly verify: after finding a candidate `t`, compute the point and check that it is strictly outside at least one dimension (i.e., less than min or greater than max) because if the ray starts exactly on a face and moves inward, that t would be an entry not an exit. So the robust approach: for each of the six boundary planes (min_x, max_x, min_y, max_y, min_z, max_z), solve for t. Accept a candidate only if `t > 0` and the point at that t is outside the box (i.e., there exists a coordinate where it is less than min or greater than max). Track the smallest such accepted t. If no accepted t exists, return -1.0. Edge cases: direction zero on all axes and the ray starts inside — no exit; start outside but direction points away — no exit; start on boundary and direction points inward — candidate t=0 is rejected; if direction is zero on an axis, that axis yields no candidates. Complexity is O(1) time and O(1) space. Note: we must include `<cmath>` and `<limits>`.
