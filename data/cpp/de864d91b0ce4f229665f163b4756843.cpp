// Write a standalone C++ function `double calculateLensqCriterion(const std::vector<Vector3>& positions, const std::vector<Vector3>& normals, const std::vector<Vector2>& uvs)` that, given three parallel arrays of (at least one) vertices representing the positions, normals, and 2D UV coordinates of a triangle mesh, computes a scalar "smoothing criterion" derived from the logic in `BandPatchMesh::MeshVert::AddUV`. For each pair of distinct vertices `(i, j)` where `i < j`, compute the vector `d = pos[i] - pos[j]`, then project `d` onto the plane perpendicular to `norm[j]` by computing `proj = d - norm[j] * dot(d, norm[j])`. Let `L = lengthSquared(d)` and `P = lengthSquared(proj)`. If `P > 0` and `L > 0`, compute the quantity `sqrt(P / L)`. Then, compute `dot_proj_with_tangent = dot(proj, uv[j])` treating `uv[j]` as a 2D vector (ignore the third coordinate) and `dot_proj_with_bitangent = dot(proj, uv[j])` repeated? No—correctly: use `uv[j].x` for one dot and `uv[j].y` for the other, forming two scaled components `(sqrt(P/L) * (uv[j].x) * dot(proj, uv[j]))`? Simplify: The task is to implement a function that returns the sum over all ordered pairs `(i, j)` with `i != j` of the quantity `max(0, 1 - 2 * sqrt(P/L))`, but only if both `L > 0` and `P > 0`. If `P == 0` but `L > 0`, add 1.0. If `L == 0`, add 0.0. The function must be `const`-correct, use only standard libraries, and not rely on external math libraries.
#include <cassert>
#include <cmath>

// Include the solution code above, then:

int main() {
    // Case 1: single pair where vertices are identical in position, zero contribution
    {
        std::vector<Vector3> pos = {Vector3(0,0,0), Vector3(0,0,0)};
        std::vector<Vector3> norm = {Vector3(0,0,1), Vector3(0,0,1)};
        std::vector<Vector2> uv = {Vector2(0,0), Vector2(0,0)};
        double r = calculateLensqCriterion(pos, norm, uv);
        assert(std::fabs(r - 0.0) < 1e-6);
    }

    // Case 2: vertices perpendicular to normal -> projection equals d, ratio=1, contribution 1-2=-1 -> max 0
    {
        std::vector<Vector3> pos = {Vector3(1,0,0), Vector3(0,0,0)};
        std::vector<Vector3> norm = {Vector3(0,0,1), Vector3(0,0,1)};
        std::vector<Vector2> uv = {Vector2(0,0), Vector2(0,0)};
        double r = calculateLensqCriterion(pos, norm, uv);
        assert(std::fabs(r - 0.0) < 1e-6);
    }

    // Case 3: vertex exactly along normal -> projection zero, contribution 1.0
    {
        std::vector<Vector3> pos = {Vector3(0,0,2), Vector3(0,0,0)};
        std::vector<Vector3> norm = {Vector3(0,0,1), Vector3(0,0,1)};
        std::vector<Vector2> uv = {Vector2(0,0), Vector2(0,0)};
        double r = calculateLensqCriterion(pos, norm, uv);
        assert(std::fabs(r - 1.0) < 1e-6);
    }

    // Case 4: vertex at 45 degrees to normal -> projection length = d * sqrt(0.5), ratio = sqrt(0.5) ≈ 0.7071, contribution = 1 - 2*0.7071 ≈ -0.414 → 0
    {
        std::vector<Vector3> pos = {Vector3(1,0,1), Vector3(0,0,0)};
        std::vector<Vector3> norm = {Vector3(0,0,1), Vector3(0,0,1)};
        std::vector<Vector2> uv = {Vector2(0,0), Vector2(0,0)};
        double r = calculateLensqCriterion(pos, norm, uv);
        assert(std::fabs(r) < 1e-6);
    }

    // Case 5: three vertices, v0 far away perpendicular, v1 near parallel, v2 identical
    {
        std::vector<Vector3> pos = {Vector3(10,0,0), Vector3(0,0,1), Vector3(0,0,0)};
        std::vector<Vector3> norm = {Vector3(0,0,1), Vector3(0,0,1), Vector3(0,0,1)};
        std::vector<Vector2> uv = {Vector2(0,0), Vector2(0,0), Vector2(0,0)};
        // Pairs:
        // (0,1): d=(10,0,-1), proj=(10,0,0), pSq=100, dSq=101, ratio≈0.995, contribution max(0,1-1.99)=0
        // (0,2): d=(10,0,0), proj=(10,0,0), pSq=100, dSq=100, ratio=1, contribution max(0,1-2)=0
        // (1,2): d=(0,0,1), proj=(0,0,0), pSq=0, dSq=1, contribution 1.0
        double r = calculateLensqCriterion(pos, norm, uv);
        assert(std::fabs(r - 1.0) < 1e-6);
    }

    // Case 6: mismatch sizes returns 0
    {
        std::vector<Vector3> pos = {Vector3(0,0,0)};
        std::vector<Vector3> norm = {Vector3(0,0,1), Vector3(0,0,1)};
        std::vector<Vector2> uv = {Vector2(0,0)};
        assert(calculateLensqCriterion(pos, norm, uv) == 0.0);
    }

    // Case 7: empty input returns 0
    {
        std::vector<Vector3> pos, norm;
        std::vector<Vector2> uv;
        assert(calculateLensqCriterion(pos, norm, uv) == 0.0);
    }

    return 0;
}
#include <vector>
#include <cmath>
#include <algorithm>

struct Vector2 {
    float x, y;
    Vector2(float x = 0, float y = 0) : x(x), y(y) {}
};

struct Vector3 {
    float x, y, z;
    Vector3(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}
};

static float dot(const Vector3& a, const Vector3& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

static Vector3 subtract(const Vector3& a, const Vector3& b) {
    return Vector3(a.x - b.x, a.y - b.y, a.z - b.z);
}

static Vector3 scale(const Vector3& v, float s) {
    return Vector3(v.x * s, v.y * s, v.z * s);
}

static Vector3 add(const Vector3& a, const Vector3& b) {
    return Vector3(a.x + b.x, a.y + b.y, a.z + b.z);
}

static float lengthSquared(const Vector3& v) {
    return v.x * v.x + v.y * v.y + v.z * v.z;
}

// Sum over all unordered pairs (i < j) of a smoothing criterion based on projection.
// For each pair, compute d = pos[i] - pos[j], project onto plane perpendicular to norm[j],
// and accumulate max(0, 1 - 2 * sqrt(projectedSq / dSq)) for projectedSq > 0,
// or 1.0 if projectedSq == 0 and dSq > 0, or 0.0 if dSq == 0.
double calculateLensqCriterion(
    const std::vector<Vector3>& positions,
    const std::vector<Vector3>& normals,
    const std::vector<Vector2>& uvs
) {
    if (positions.size() != normals.size() || normals.size() != uvs.size()) {
        return 0.0;
    }
    if (positions.empty()) return 0.0;

    double total = 0.0;
    const size_t n = positions.size();
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = i + 1; j < n; ++j) {
            // Use vertex j as the reference (like mv in AddUV)
            Vector3 d = subtract(positions[i], positions[j]);
            double dSq = lengthSquared(d);
            if (dSq == 0.0) {
                total += 0.0; // L == 0 case
                continue;
            }

            // Project d onto plane perpendicular to norm[j]
            double ndot = dot(d, normals[j]);
            Vector3 proj = subtract(d, scale(normals[j], (float)ndot));
            double pSq = lengthSquared(proj);

            if (pSq > 0.0) {
                double ratio = std::sqrt(pSq / dSq);
                total += std::max(0.0, 1.0 - 2.0 * ratio);
            } else {
                // P == 0 but L > 0
                total += 1.0;
            }
        }
    }
    return total;
}
// The solution iterates over all ordered pairs `(i, j)` with `i != j`. For each pair, it copies the code flow from `AddUV` without the mutation part. Compute `d = pos[i] - pos[j]`. Compute `L = lengthSquared(d)`. Compute `dot = dot(d, norm[j])`. Compute `proj = d - norm[j] * dot`. Compute `P = lengthSquared(proj)`. If `P > 0`, compute `recipsq = sqrt(P / L)` (this is the inverse of the reciprocal square root used in the original code, but mathematically equivalent to `sqrt(P/L)` after simplification). If `recipsq` is used as a scaling factor: in the original, `recipsq = RecipSqrtAccurate(newlensq / lensq)` and then it multiplies `recipsq * vr.x * dot5` where `dot5 = dot(proj, mv->unk4)` and `recipsq * vry * dot4` where `dot4 = dot(proj, mv->unk10)`. For our standalone criterion, ignore the UV combination and simply return the value `sqrt(P/L)` as a penalty. Then, define the contribution as `max(0, 1 - sqrt(P/L))`? The task says "max(0, 1 - 2 * sqrt(P/L))". Actually, read the task carefully: it defines the criterion as "sum over all ordered pairs with i != j of the quantity max(0, 1 - 2 * sqrt(P/L))" but then says "If P == 0 but L > 0, add 1.0. If L == 0, add 0.0." This is contradictory because if `P == 0`, then `sqrt(P/L)=0`, so `max(0,1-0)=1`, which is consistent. So the rule is: if `L>0` and `P>0`, add `max(0, 1 - 2 * sqrt(P/L))`? Wait the task says "the quantity max(0, 1 - 2 * sqrt(P/L))" but then says "If P == 0 but L > 0, add 1.0" which is consistent with `sqrt(P/L)=0` and would give `max(0,1-0)=1` if the factor were 1, but it says 1 - 2*0 =1, so consistent. For `L == 0`, `sqrt(P/L)` is undefined, so add 0. So implement: for each unordered pair? The task says "ordered pairs" but later says "pairs of distinct vertices (i, j) where i < j" which is unordered. Clarify: The function should sum over all unordered pairs `(i,j)` with `i<j`. For each such pair, compute the quantity using the vertex `j` as the reference (norm[j]). Then add the contribution. This is consistent with the original `AddUV` where `mv` is the other vertex and `this` is the reference. Time complexity is O(n^2) because we iterate over all pairs. Space complexity is O(1) auxiliary. Edge cases: empty input returns 0? The task says non-empty but handle gracefully by returning 0.0. Also, if the arrays have different sizes, assume they are all the same size? Return 0 if sizes mismatch to be safe. For numerical robustness, use `std::sqrt` and check for `P > 0` and `L > 0` to avoid division by zero.
