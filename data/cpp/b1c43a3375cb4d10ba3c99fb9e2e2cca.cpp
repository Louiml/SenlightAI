/*
Write a standalone C++ function that takes a 2D axis-aligned bounding box (defined by lower and upper corner points) and a 2D ray segment (defined by a start point, an end point indicating direction, and a maximum fraction parameter between 0 and 1) as input. The function must determine whether the ray segment intersects the box. If it does not intersect, return `false`. If it does intersect, return `true` and set the output fraction (a value between 0 and `maxFraction`) where the ray first enters the box, and the outward-facing normal of the box face that is hit (the normal should point in the direction opposite to the ray's travel for that axis). The ray is parameterized as `p(t) = p1 + t * (p2 - p1)` for `0 <= t <= maxFraction`. The function should handle cases where the ray starts inside the box (in which case `tmin = 0` is not considered a valid hit; the function should return `false` because the condition `tmin < 0` is checked), degenerate rays (where `p1 == p2`), and rays parallel to an axis that lie entirely outside the box. Assume the box is valid (lower bound is strictly less than upper bound for each axis). Use a custom small `Vec2` struct with `x` and `y` members and basic operations (`-`, `+`, `*` scalar, `[]` accessor, `dot`, `abs`, `Normalize`) for clarity.
*/
#include <cmath>
#include <algorithm>
#include <limits>

struct Vec2 {
    float x, y;

    Vec2() : x(0.0f), y(0.0f) {}
    Vec2(float px, float py) : x(px), y(py) {}

    float& operator[](int index) {
        return (index == 0) ? x : y;
    }

    const float& operator[](int index) const {
        return (index == 0) ? x : y;
    }

    Vec2 operator+(const Vec2& other) const { return Vec2(x + other.x, y + other.y); }
    Vec2 operator-(const Vec2& other) const { return Vec2(x - other.x, y - other.y); }
    Vec2 operator*(float scalar) const { return Vec2(x * scalar, y * scalar); }

    void Set(float px, float py) { x = px; y = py; }
};

inline float Dot(const Vec2& a, const Vec2& b) {
    return a.x * b.x + a.y * b.y;
}

inline Vec2 AbsVec(const Vec2& v) {
    return Vec2(std::fabs(v.x), std::fabs(v.y));
}

struct RayCastInput {
    Vec2 p1;
    Vec2 p2;
    float maxFraction;
};

struct RayCastOutput {
    float fraction;
    Vec2 normal;
};

struct AABB {
    Vec2 lowerBound;
    Vec2 upperBound;
};

// Performs a ray cast against an axis-aligned bounding box.
// Returns true if the ray segment (p1 to p1 + (p2-p1)*maxFraction) intersects.
bool RayCastAABB(const AABB& box, const RayCastInput& input, RayCastOutput* output) {
    const float epsilon = 1e-6f;
    const float maxFloat = std::numeric_limits<float>::max();

    float tmin = -maxFloat;
    float tmax = maxFloat;

    Vec2 p = input.p1;
    Vec2 d = input.p2 - input.p1;
    Vec2 absD = AbsVec(d);

    Vec2 normal;
    normal.Set(0.0f, 0.0f);

    for (int i = 0; i < 2; ++i) {
        if (absD[i] < epsilon) {
            // Ray is parallel to this slab.
            if (p[i] < box.lowerBound[i] || box.upperBound[i] < p[i]) {
                return false;
            }
        } else {
            float inv_d = 1.0f / d[i];
            float t1 = (box.lowerBound[i] - p[i]) * inv_d;
            float t2 = (box.upperBound[i] - p[i]) * inv_d;

            // Sign of the normal for this slab.
            float s = -1.0f;
            if (t1 > t2) {
                std::swap(t1, t2);
                s = 1.0f;
            }

            // Update the minimum entry time.
            if (t1 > tmin) {
                normal.Set(0.0f, 0.0f);
                normal[i] = s;
                tmin = t1;
            }

            // Update the maximum exit time.
            tmax = std::min(tmax, t2);

            if (tmin > tmax) {
                return false;
            }
        }
    }

    // Ray starts inside the box or behind the entry plane.
    if (tmin < 0.0f || input.maxFraction < tmin) {
        return false;
    }

    output->fraction = tmin;
    output->normal = normal;
    return true;
}
#include <cassert>
#include <cmath>

// Include the Vec2, RayCastInput, RayCastOutput, AABB, and RayCastAABB definitions here.

int main() {
    // Box from (0,0) to (2,2)
    AABB box;
    box.lowerBound = Vec2(0.0f, 0.0f);
    box.upperBound = Vec2(2.0f, 2.0f);

    // Ray from (-1,1) to (3,1) (horizontal, hits left face at x=0)
    RayCastInput input1;
    input1.p1 = Vec2(-1.0f, 1.0f);
    input1.p2 = Vec2(3.0f, 1.0f);
    input1.maxFraction = 1.0f;

    RayCastOutput output1;
    assert(RayCastAABB(box, input1, &output1) == true);
    assert(std::fabs(output1.fraction - 0.25f) < 1e-5f);
    assert(std::fabs(output1.normal.x - (-1.0f)) < 1e-5f);
    assert(std::fabs(output1.normal.y - 0.0f) < 1e-5f);

    // Ray from (3,1) to (-1,1) (moving right-to-left, hits right face at x=2)
    RayCastInput input2;
    input2.p1 = Vec2(3.0f, 1.0f);
    input2.p2 = Vec2(-1.0f, 1.0f);
    input2.maxFraction = 1.0f;

    RayCastOutput output2;
    assert(RayCastAABB(box, input2, &output2) == true);
    assert(std::fabs(output2.fraction - 0.25f) < 1e-5f);
    assert(std::fabs(output2.normal.x - 1.0f) < 1e-5f);
    assert(std::fabs(output2.normal.y - 0.0f) < 1e-5f);

    // Ray that misses the box entirely (above it)
    RayCastInput input3;
    input3.p1 = Vec2(0.5f, 3.0f);
    input3.p2 = Vec2(1.5f, 4.0f);
    input3.maxFraction = 1.0f;

    RayCastOutput output3;
    assert(RayCastAABB(box, input3, &output3) == false);

    // Ray that starts inside the box (should return false per spec)
    RayCastInput input4;
    input4.p1 = Vec2(1.0f, 1.0f);
    input4.p2 = Vec2(3.0f, 1.0f);
    input4.maxFraction = 1.0f;

    RayCastOutput output4;
    assert(RayCastAABB(box, input4, &output4) == false);

    // Ray that is parallel to X axis and lies outside (y=3)
    RayCastInput input5;
    input5.p1 = Vec2(-10.0f, 3.0f);
    input5.p2 = Vec2(10.0f, 3.0f);
    input5.maxFraction = 1.0f;

    RayCastOutput output5;
    assert(RayCastAABB(box, input5, &output5) == false);

    // Degenerate ray (zero length) starting outside the box
    RayCastInput input6;
    input6.p1 = Vec2(3.0f, 3.0f);
    input6.p2 = Vec2(3.0f, 3.0f);
    input6.maxFraction = 1.0f;

    RayCastOutput output6;
    assert(RayCastAABB(box, input6, &output6) == false);

    // Ray that hits the top face (from below, moving up)
    RayCastInput input7;
    input7.p1 = Vec2(1.0f, -1.0f);
    input7.p2 = Vec2(1.0f, 3.0f);
    input7.maxFraction = 1.0f;

    RayCastOutput output7;
    assert(RayCastAABB(box, input7, &output7) == true);
    assert(std::fabs(output7.fraction - 0.25f) < 1e-5f);
    assert(std::fabs(output7.normal.x - 0.0f) < 1e-5f);
    assert(std::fabs(output7.normal.y - (-1.0f)) < 1e-5f);

    // Short ray that does not reach the box
    RayCastInput input8;
    input8.p1 = Vec2(-1.0f, 1.0f);
    input8.p2 = Vec2(-0.5f, 1.0f);
    input8.maxFraction = 1.0f;

    RayCastOutput output8;
    assert(RayCastAABB(box, input8, &output8) == false);

    return 0;
}
// The algorithm is based on the slab method for ray-AABB intersection. For each axis (x and y), compute the entry and exit parameters `t1` and `t2` where the ray crosses the two slab boundaries. If the ray is parallel to an axis (`absD < epsilon`), check whether the starting point lies within that slab; if not, there is no intersection. Otherwise, compute `t1` and `t2` using the inverse of the direction. Swap them so `t1` is the nearer entry and `t2` the farther exit, and track which face was hit (positive or negative direction). Maintain the global `tmin` (maximum of all entry parameters) and `tmax` (minimum of all exit parameters). If at any point `tmin > tmax`, the ray misses the box. After processing both axes, if `tmin < 0` (ray starts inside or behind the box) or `tmin > maxFraction`, the ray does not properly intersect within the allowed segment. Otherwise, set the output fraction to `tmin` and the normal to the stored normal corresponding to the face that produced that `tmin`. Edge cases include: (1) ray starting exactly on a boundary at `t=0` – handled by the `tmin < 0` check (since `tmin` will be 0, which is not less than 0, so it returns true), (2) parallel rays that are inside the slab for that axis but miss on another – handled by the global `tmin/tmax` check, (3) degenerate ray `p1 == p2` – this causes `absD` to be zero for both axes, so the algorithm will only return true if the start point is inside the box, but then `tmin` will be 0 and `tmax` infinite, and the final check `tmin < 0` is false, so it returns true with `fraction=0`? Actually careful: for a degenerate ray, `tmin` remains -max, `tmax` remains +max initially, and for each axis if the point is inside the slab, nothing changes. So after the loop, `tmin` is still -maxFloat, which is less than 0, so the function returns false. That is correct for a degenerate ray that doesn't move. The time complexity is O(1) since it processes a fixed number of axes (2) and uses constant space.
