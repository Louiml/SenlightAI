// Write a C++ function `sphereCapsuleContact` that determines whether a sphere and a capsule (a cylinder with hemispherical caps) intersect in 3D space, and if so, returns a contact point, unit contact normal (pointing from the capsule toward the sphere), and separation distance (negative when overlapping, positive when within a specified contact distance but not yet touching). The function should take the sphere center, sphere radius, capsule segment endpoints (the two centers of the hemispherical caps), capsule radius, and a contact distance threshold. It must return a boolean indicating whether a contact exists, and fill reference parameters for the contact point, normal, and separation. The algorithm must compute the closest point on the capsule's central line segment to the sphere center, then use that distance to determine overlap. Handle edge cases: a degenerate capsule segment (zero length) where the capsule behaves like a sphere, and the case where the sphere center exactly lies on the segment axis (in which case the normal should default to a fixed direction, e.g., positive X-axis). The separation is defined as: distance from sphere center to closest point on segment minus (sphere radius + capsule radius). A contact exists if the squared distance from the sphere center to the closest point on the segment is less than or equal to the squared sum of the two radii plus the contact distance. Use `const` correctness and ensure all floating-point comparisons are robust.

#include <cassert>
#include <cmath>

// The solution function prototype is as shown above; we include the declaration here.
bool sphereCapsuleContact(
    const double sphereCenter[3], double sphereRadius,
    const double capStart[3], const double capEnd[3], double capsuleRadius,
    double contactDist,
    double contactPoint[3], double contactNormal[3], double& separation);

int main() {
    // Case 1: sphere overlapping the capsule's cylindrical side
    {
        double sphereCenter[3] = {0.0, 0.0, 1.0};
        double sphereR = 0.5;
        double capStart[3] = {0.0, 0.0, -2.0};
        double capEnd[3]   = {0.0, 0.0,  2.0};
        double capR = 0.5;
        double contactDist = 0.1;
        double cp[3], cn[3], sep;
        bool hit = sphereCapsuleContact(sphereCenter, sphereR, capStart, capEnd, capR, contactDist, cp, cn, sep);
        assert(hit);
        assert(sep < 0.0); // overlapping
        // normal should be +Y because the sphere is above the axis (y direction)
        assert(std::abs(cn[1] - 1.0) < 1e-9);
        // contact point should be sphereCenter - normal * sphereR
        assert(std::abs(cp[0] - 0.0) < 1e-9);
        assert(std::abs(cp[1] - sphereCenter[1] + sphereR) < 1e-9);
        assert(std::abs(cp[2] - 1.0) < 1e-9);
    }

    // Case 2: sphere exactly touching the capsule's end cap (distance = radii sum)
    {
        double sphereCenter[3] = {0.0, 0.0, 3.0}; // end at z=2, so touching at z=3 if radii sum = 1.0
        double sphereR = 0.5;
        double capStart[3] = {0.0, 0.0, -2.0};
        double capEnd[3]   = {0.0, 0.0,  2.0};
        double capR = 0.5;
        double contactDist = 0.0;
        double cp[3], cn[3], sep;
        bool hit = sphereCapsuleContact(sphereCenter, sphereR, capStart, capEnd, capR, contactDist, cp, cn, sep);
        assert(hit);
        assert(std::abs(sep) < 1e-9); // exactly touching
        assert(std::abs(cn[2] - 1.0) < 1e-9); // normal along +Z
        assert(std::abs(cp[2] - 2.0) < 1e-9); // contact point at cap end
    }

    // Case 3: sphere far away, no contact
    {
        double sphereCenter[3] = {0.0, 10.0, 0.0};
        double sphereR = 0.5;
        double capStart[3] = {0.0, 0.0, -2.0};
        double capEnd[3]   = {0.0, 0.0,  2.0};
        double capR = 0.5;
        double contactDist = 0.0;
        double cp[3], cn[3], sep;
        bool hit = sphereCapsuleContact(sphereCenter, sphereR, capStart, capEnd, capR, contactDist, cp, cn, sep);
        assert(!hit);
    }

    // Case 4: degenerate capsule (zero-length segment) behaving like a sphere
    {
        double sphereCenter[3] = {0.1, 0.0, 0.0};
        double sphereR = 0.5;
        double capStart[3] = {0.0, 0.0, 0.0};
        double capEnd[3]   = {0.0, 0.0, 0.0}; // same point
        double capR = 0.5;
        double contactDist = 0.0;
        double cp[3], cn[3], sep;
        bool hit = sphereCapsuleContact(sphereCenter, sphereR, capStart, capEnd, capR, contactDist, cp, cn, sep);
        assert(hit); // distance 0.1 < radius sum 1.0 => overlap
        assert(sep < 0.0);
        // normal should point from origin to sphere center, roughly +X
        assert(std::abs(cn[0] - 1.0) < 1e-9);
    }

    // Case 5: sphere center exactly on the capsule axis (degenerate normal case)
    {
        double sphereCenter[3] = {0.0, 0.0, 0.0};
        double sphereR = 0.5;
        double capStart[3] = {0.0, 0.0, -1.0};
        double capEnd[3]   = {0.0, 0.0,  1.0};
        double capR = 0.5;
        double contactDist = 0.01;
        double cp[3], cn[3], sep;
        bool hit = sphereCapsuleContact(sphereCenter, sphereR, capStart, capEnd, capR, contactDist, cp, cn, sep);
        assert(hit); // center inside capsule
        // normal should be default (1,0,0) because dir is zero at closest point
        assert(std::abs(cn[0] - 1.0) < 1e-9);
        assert(std::abs(cn[1] - 0.0) < 1e-9);
        assert(std::abs(cn[2] - 0.0) < 1e-9);
        // separation should be negative because center is well inside
        assert(sep < 0.0);
    }

    // Case 6: within contact distance but not touching
    {
        double sphereCenter[3] = {0.0, 0.0, 2.9}; // gap of 0.1 to cap at z=2.0, radii sum 1.0 => actual distance 0.9
        double sphereR = 0.4;
        double capStart[3] = {0.0, 0.0, -2.0};
        double capEnd[3]   = {0.0, 0.0,  2.0};
        double capR = 0.6;
        double contactDist = 0.2; // threshold: distance 0.9 < 0.9+0.2 = 1.1? actually 0.9 < 1.0? radiusSum=1.0, gap=0.9, so not touching but within 0.2? compute: distance from center to cap end = 0.9, radiusSum=1.0 => separation = -0.1? Wait: sphere center at z=2.9, cap end at z=2.0, distance=0.9, radiusSum=1.0, so separation = 0.9 - 1.0 = -0.1 (overlap!). Let's adjust: set sphere z=3.5, distance=1.5, radiusSum=1.0 => separation=0.5 (positive, but within contactDist 0.5? Let's pick contactDist=0.6, then contact exists).
        sphereCenter[2] = 3.5;
        double cp2[3], cn2[3], sep2;
        bool hit2 = sphereCapsuleContact(sphereCenter, sphereR, capStart, capEnd, capR, 0.6, cp2, cn2, sep2);
        assert(hit2);
        assert(sep2 > 0.0); // not touching
        assert(sep2 <= 0.6); // but within contact distance
        // normal points along +Z
        assert(std::abs(cn2[2] - 1.0) < 1e-9);
    }

    return 0;
}

#include <cmath>
#include <algorithm>

/**
 * Compute the closest point on a line segment from s to e to a point c,
 * and return the squared distance from c to that closest point.
 * Output parameter param is the normalized segment parameter (0=s, 1=e).
 */
static double distPointSegmentSquared(
    const double s[3], const double e[3], const double c[3], double& param)
{
    const double ap[3] = {c[0] - s[0], c[1] - s[1], c[2] - s[2]};
    const double ab[3] = {e[0] - s[0], e[1] - s[1], e[2] - s[2]};

    double abSq = ab[0]*ab[0] + ab[1]*ab[1] + ab[2]*ab[2];
    double t;
    if (abSq < 1e-12) {
        // degenerate segment: treat as a point at s
        t = 0.0;
    } else {
        double nom = ap[0]*ab[0] + ap[1]*ab[1] + ap[2]*ab[2];
        t = std::clamp(nom / abSq, 0.0, 1.0);
    }
    param = t;

    // closest point p = s + t * ab
    double p[3] = {s[0] + t*ab[0], s[1] + t*ab[1], s[2] + t*ab[2]};
    double dx = c[0] - p[0];
    double dy = c[1] - p[1];
    double dz = c[2] - p[2];
    return dx*dx + dy*dy + dz*dz;
}

/**
 * Determine if a sphere and a capsule intersect (or are within a contact distance).
 * 
 * Parameters:
 *   sphereCenter[3]  - world-space center of the sphere
 *   sphereRadius     - radius of the sphere
 *   capStart[3]      - center of the first hemispherical cap of the capsule
 *   capEnd[3]        - center of the second hemispherical cap of the capsule
 *   capsuleRadius    - radius of the capsule
 *   contactDist      - extra distance threshold for reporting contacts
 * 
 * Outputs (valid only if the function returns true):
 *   contactPoint[3]  - point on the sphere surface along the normal
 *   contactNormal[3] - unit vector pointing from the capsule toward the sphere
 *   separation       - signed distance: negative = overlap, positive = within contactDist
 * 
 * Returns: true if a contact exists (including within the contact distance).
 */
bool sphereCapsuleContact(
    const double sphereCenter[3], double sphereRadius,
    const double capStart[3], const double capEnd[3], double capsuleRadius,
    double contactDist,
    double contactPoint[3], double contactNormal[3], double& separation)
{
    double t;
    double sqDist = distPointSegmentSquared(capStart, capEnd, sphereCenter, t);

    double radiusSum = sphereRadius + capsuleRadius;
    double inflatedSum = radiusSum + contactDist;
    double sqInflatedSum = inflatedSum * inflatedSum;

    if (sqDist > sqInflatedSum) {
        return false;
    }

    // closest point on segment
    double closest[3] = {
        capStart[0] + t * (capEnd[0] - capStart[0]),
        capStart[1] + t * (capEnd[1] - capStart[1]),
        capStart[2] + t * (capEnd[2] - capStart[2])
    };

    double dir[3] = {sphereCenter[0]-closest[0], sphereCenter[1]-closest[1], sphereCenter[2]-closest[2]};
    double dirLenSq = dir[0]*dir[0] + dir[1]*dir[1] + dir[2]*dir[2];

    if (dirLenSq < 1e-12) {
        // sphere center exactly on the capsule axis at the closest point
        contactNormal[0] = 1.0; contactNormal[1] = 0.0; contactNormal[2] = 0.0;
    } else {
        double invLen = 1.0 / std::sqrt(dirLenSq);
        contactNormal[0] = dir[0] * invLen;
        contactNormal[1] = dir[1] * invLen;
        contactNormal[2] = dir[2] * invLen;
    }

    // contact point on sphere surface
    contactPoint[0] = sphereCenter[0] - contactNormal[0] * sphereRadius;
    contactPoint[1] = sphereCenter[1] - contactNormal[1] * sphereRadius;
    contactPoint[2] = sphereCenter[2] - contactNormal[2] * sphereRadius;

    // separation = distance - radiusSum (negative when overlapping)
    separation = std::sqrt(std::max(0.0, sqDist)) - radiusSum;

    return true;
}

// The core algorithm is to compute the closest point on the line segment from `s` to `e` to the sphere center `c`. This is a standard point-to-segment distance problem. Compute vector `ap = c - s` and `ab = e - s`. The parameter `t` along the segment is `clamp(dot(ap, ab) / dot(ab, ab), 0, 1)`, but if `ab` is the zero vector (segment degenerate), then `t` is 0 and the closest point is just `s`. The closest point `p = s + t * ab`. The distance squared from `c` to `p` is computed. The inflated radius sum is `sphereRadius + capsuleRadius + contactDistance`. If the squared distance is less than or equal to the squared inflated sum, a contact exists. The contact normal is `normalize(c - p)`, but if `c - p` is nearly zero (sphere center exactly on the capsule axis at the closest point), we use a fallback like `(1,0,0)`. The contact point is the sphere center minus the sphere radius times the normal (so the point is on the sphere surface). The separation is `sqrt(squareDist) - (sphereRadius + capsuleRadius)`, which is negative for actual overlap, positive for being within contact distance, and zero for touching. The algorithm is O(1) time and O(1) space. Edge cases: degenerate segment handled by checking if `dot(ab,ab)` is zero (use a tolerance); sphere center on the segment axis handled by checking if the squared length of `c - p` is below a tiny epsilon. For numerical robustness, use `std::sqrt` after clamping the squared distance to be non-negative. The contact distance is added so that contacts are reported slightly before actual touching.
