// Write a C++ function that, given three 3D points in space (represented as `QVector3D` using the Qt framework), computes the signed distance from a fourth point to the plane defined by the three points. The function must return a `float`. If the three points are collinear or coincident (so that no unique plane exists), return `0.0f`. Assume all inputs are valid `QVector3D` objects; you do not need to handle NaN or infinite values. Your function should not modify the input points.
// The plane through three non-collinear points can be defined by a point on the plane (any of the three) and a normal vector. The normal is the cross product of two edge vectors, e.g., `(p2 - p1) × (p3 - p1)`. The signed distance from a point `q` to this plane is `dot(normal, q - p1) / length(normal)`, but this requires normalizing the normal first. If the normal's length is zero (collinear or coincident points), we return `0.0f`. If the length is nonzero, we normalize and compute the dot product. An alternative is to use `QVector3D::distanceToPlane`, but we must first check that a valid plane exists. The main algorithm: compute edge vectors, compute cross product, check its squared length against a small epsilon (e.g., `1e-12`) to avoid division by zero, then return the normalized dot product. Edge case: if the points are identical, the cross product is zero. If the points are collinear, the cross product is also zero. Time complexity is O(1) since only a few vector operations are performed; space complexity is O(1) as no extra containers are needed.
#include <QVector3D>
#include <cmath>

// Compute signed distance from 'queryPoint' to the plane defined by p1, p2, p3.
// Returns 0.0f if the three points do not define a unique plane (collinear/coincident).
float signedDistanceToPlane(const QVector3D& p1, const QVector3D& p2, const QVector3D& p3, const QVector3D& queryPoint)
{
    const QVector3D edge1 = p2 - p1;
    const QVector3D edge2 = p3 - p1;
    const QVector3D normal = QVector3D::crossProduct(edge1, edge2);

    const float normalSquaredLength = normal.lengthSquared();
    if (normalSquaredLength < 1e-12f) {
        // Degenerate plane: no unique normal.
        return 0.0f;
    }

    const QVector3D normalizedNormal = normal / std::sqrt(normalSquaredLength);
    return QVector3D::dotProduct(normalizedNormal, queryPoint - p1);
}
#include <cassert>
#include <cmath>
#include <QVector3D>

// Include the solution function here (or from a header).

int main()
{
    // Simple plane z = 0, normal (0,0,1)
    QVector3D p1(0, 0, 0);
    QVector3D p2(1, 0, 0);
    QVector3D p3(0, 1, 0);

    // Point above plane
    assert(std::fabs(signedDistanceToPlane(p1, p2, p3, QVector3D(0.5f, 0.5f, 3.0f)) - 3.0f) < 1e-5f);
    // Point on plane
    assert(std::fabs(signedDistanceToPlane(p1, p2, p3, QVector3D(2.0f, 3.0f, 0.0f))) < 1e-5f);
    // Point below plane (negative distance)
    assert(std::fabs(signedDistanceToPlane(p1, p2, p3, QVector3D(0.0f, 0.0f, -2.0f)) + 2.0f) < 1e-5f);

    // Plane x = 1, normal (1,0,0)
    QVector3D q1(1, 0, 0);
    QVector3D q2(1, 4, 0);
    QVector3D q3(1, 0, 5);
    assert(std::fabs(signedDistanceToPlane(q1, q2, q3, QVector3D(4.0f, 0.0f, 0.0f)) - 3.0f) < 1e-5f);
    assert(std::fabs(signedDistanceToPlane(q1, q2, q3, QVector3D(-1.0f, 0.0f, 0.0f)) + 2.0f) < 1e-5f);

    // Degenerate: collinear points
    QVector3D c1(0, 0, 0);
    QVector3D c2(1, 1, 1);
    QVector3D c3(2, 2, 2);
    assert(signedDistanceToPlane(c1, c2, c3, QVector3D(5, 5, 5)) == 0.0f);

    // Degenerate: coincident points
    QVector3D d1(1, 2, 3);
    QVector3D d2(1, 2, 3);
    QVector3D d3(1, 2, 3);
    assert(signedDistanceToPlane(d1, d2, d3, QVector3D(0, 0, 0)) == 0.0f);

    // Signed distance reflects side: flip normal by reordering points
    QVector3D e1(0, 0, 0);
    QVector3D e2(1, 0, 0);
    QVector3D e3(0, 1, 0);
    // Reverse order gives opposite normal
    float d1_val = signedDistanceToPlane(e1, e2, e3, QVector3D(0, 0, 1));
    float d2_val = signedDistanceToPlane(e1, e3, e2, QVector3D(0, 0, 1));
    assert(std::fabs(d1_val - 1.0f) < 1e-5f);
    assert(std::fabs(d2_val + 1.0f) < 1e-5f);
}
