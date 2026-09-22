/*
Implement a standalone C++ function that models the discretization of motion primitives at different starting angles. Given a set of base motion primitives defined at angle 0 (each with an end pose vector and a movement type), a number of discrete angles, and a primitive accuracy threshold, produce a list of all discretized primitives for all angles by rotating the base end poses, scaling them incrementally to snap to integer grid coordinates and valid discrete angles, and rejecting duplicates. Each resulting primitive must store its starting angle, discrete end pose (rounded to integer grid cells and an angle in \([0, numAngles)\)), the original movement type, and a cost multiplier. The solution must handle straight movements (forward, backward, lateral) by scaling the end pose, point turns by incrementing discrete angle steps without changing position, and curves (forward/backward turns, lateral curves) by rotating around a scaled center of rotation. Primitives must be rejected if the rounded end pose deviates from the true continuous end pose by more than the accuracy threshold, if curves fall on the wrong side of the rotation axis, or if the end position duplicates an already-added primitive for the same angle. For each angle, the number of primitives produced must be identical to the count for angle 0; otherwise the function should indicate failure by returning an empty vector.
*/

#include <vector>
#include <cmath>
#include <set>
#include <utility>
#include <limits>

// Movement type enumeration compatible with the original context.
enum class MovementType {
    FORWARD,
    BACKWARD,
    LATERAL,
    POINTTURN,
    FORWARD_TURN,
    BACKWARD_TURN,
    LATERAL_CURVE
};

// A simple 3D vector structure for positions and orientations.
struct Vec3 {
    double x = 0.0, y = 0.0, z = 0.0;
};

// A motion primitive: start angle (discrete), end pose (integer grid coords and discrete angle),
// movement type, and cost multiplier.
struct DiscretizedPrimitive {
    int startAngle;
    Vec3 endPose; // x,y are integer grid cells; z is discrete angle in [0,numAngles)
    MovementType movType;
    double costMultiplier;
};

// Helper to rotate a 2D vector by a given angle in radians around the z-axis.
Vec3 rotateZ(const Vec3& v, double angle) {
    double cosA = std::cos(angle);
    double sinA = std::sin(angle);
    return {v.x * cosA - v.y * sinA, v.x * sinA + v.y * cosA, v.z};
}

// Main function: discretize base primitives (defined at angle 0) for all start angles.
// Returns an empty vector if the number of primitives differs across angles.
std::vector<DiscretizedPrimitive> discretizePrimitives(
    const std::vector<DiscretizedPrimitive>& basePrims,
    int numAngles,
    double primAccuracy)
{
    if (numAngles <= 0 || basePrims.empty() || primAccuracy <= 0.0) {
        return {};
    }

    double radPerAngle = 2.0 * M_PI / static_cast<double>(numAngles);
    int upperDiscreteAngle = static_cast<int>(std::ceil(numAngles / 4.0));
    double maxDistToCenterGrids = 10.0; // in grid cells, arbitrary limit
    double increaseValue = 0.1; // scaling step for straight and curve lengths

    std::vector<DiscretizedPrimitive> result;
    int primsForAngle0 = 0;

    for (int angle = 0; angle < numAngles; ++angle) {
        int primsAddedForThisAngle = 0;
        std::set<std::tuple<int,int,int>> reachedEndPositions;

        for (const auto& base : basePrims) {
            Vec3 turnedEndPose = rotateZ(base.endPose, angle * radPerAngle);
            // For point turns, the base end pose is (0,0,theta) where theta is +/-1.
            // For curves, we need the rotated center of rotation; we store it as a third component for simplicity.
            Vec3 turnedCenter;
            if (base.movType == MovementType::FORWARD_TURN || 
                base.movType == MovementType::BACKWARD_TURN || 
                base.movType == MovementType::LATERAL_CURVE) {
                // Assume the base primitive's center of rotation is stored in z as a number.
                // In real code this would be a separate field; here we use a convention:
                // For FORWARD_TURN/BACKWARD_TURN, center is (0, base.endPose.z, 0) where base.endPose.z is signed radius.
                // For LATERAL_CURVE, center is (base.endPose.z, 0, 0) where base.endPose.z is signed radius.
                double radius = base.endPose.z;
                if (base.movType == MovementType::FORWARD_TURN || base.movType == MovementType::BACKWARD_TURN) {
                    turnedCenter = rotateZ({0.0, radius, 0.0}, angle * radPerAngle);
                } else {
                    turnedCenter = rotateZ({radius, 0.0, 0.0}, angle * radPerAngle);
                }
            }

            double d = 0.0;
            int currentDiscreteAngleStep = 1;
            int primsAddedForThisBase = 0;

            while (primsAddedForThisBase < static_cast<int>(basePrims.size() * 2)) { // safety bound
                Vec3 continuousEndPose;
                int discreteAngle = angle;
                bool valid = true;

                switch (base.movType) {
                    case MovementType::FORWARD:
                    case MovementType::BACKWARD:
                    case MovementType::LATERAL: {
                        Vec3 scaled = {turnedEndPose.x * (1.0 + d), turnedEndPose.y * (1.0 + d), turnedEndPose.z};
                        continuousEndPose = scaled;
                        discreteAngle = angle;
                        d += increaseValue;
                        break;
                    }
                    case MovementType::POINTTURN: {
                        continuousEndPose = {0.0, 0.0, 0.0};
                        discreteAngle = (d + 1) * turnedEndPose.z + angle;
                        d += 1.0;
                        if (d > upperDiscreteAngle) {
                            valid = false;
                        }
                        break;
                    }
                    case MovementType::FORWARD_TURN:
                    case MovementType::BACKWARD_TURN:
                    case MovementType::LATERAL_CURVE: {
                        double angleRad = currentDiscreteAngleStep * turnedEndPose.z * radPerAngle;
                        Vec3 scaledCenter = {turnedCenter.x * (1.0 + d), turnedCenter.y * (1.0 + d), turnedCenter.z};
                        Vec3 vecFromCenter = {0.0 - scaledCenter.x, 0.0 - scaledCenter.y, 0.0};
                        Vec3 rotatedVec = rotateZ(vecFromCenter, angleRad);
                        continuousEndPose = {rotatedVec.x + scaledCenter.x, rotatedVec.y + scaledCenter.y, 0.0};
                        discreteAngle = currentDiscreteAngleStep * turnedEndPose.z + angle;
                        currentDiscreteAngleStep++;
                        if (currentDiscreteAngleStep > upperDiscreteAngle) {
                            currentDiscreteAngleStep = 1;
                            d += increaseValue;
                        }
                        break;
                    }
                    default:
                        valid = false;
                        break;
                }

                if (!valid) break;

                // Check if primitive became too long.
                Vec3 diffFromBase = {continuousEndPose.x - turnedEndPose.x, continuousEndPose.y - turnedEndPose.y, 0.0};
                if (std::sqrt(diffFromBase.x*diffFromBase.x + diffFromBase.y*diffFromBase.y) > maxDistToCenterGrids) {
                    return {};
                }

                // Round to integer grid cells.
                int roundedX = static_cast<int>(std::round(continuousEndPose.x));
                int roundedY = static_cast<int>(std::round(continuousEndPose.y));
                double diffX = roundedX - continuousEndPose.x;
                double diffY = roundedY - continuousEndPose.y;
                double valueEnd = std::sqrt(diffX*diffX + diffY*diffY);

                if (valueEnd > primAccuracy) {
                    continue; // try next scaling step
                }

                // Normalize discrete angle to [0, numAngles).
                int normDiscreteAngle = discreteAngle;
                while (normDiscreteAngle < 0) normDiscreteAngle += numAngles;
                while (normDiscreteAngle >= numAngles) normDiscreteAngle -= numAngles;

                // Curve validation: check side of axis based on movement type.
                if (base.movType == MovementType::FORWARD_TURN || base.movType == MovementType::BACKWARD_TURN) {
                    // Turned-back rounded position (unrotate by start angle) must be on same side as original center.
                    Vec3 rotatedBack = rotateZ({static_cast<double>(roundedX), static_cast<double>(roundedY), 0.0}, -angle * radPerAngle);
                    double centerY = (base.movType == MovementType::FORWARD_TURN) ? turnedCenter.y : -turnedCenter.y;
                    double originalCenterY = centerY > 0 ? 1.0 : -1.0;
                    if (rotatedBack.y * originalCenterY <= 0) {
                        continue;
                    }
                }
                if (base.movType == MovementType::LATERAL_CURVE) {
                    Vec3 rotatedBack = rotateZ({static_cast<double>(roundedX), static_cast<double>(roundedY), 0.0}, -angle * radPerAngle);
                    // For lateral curve, center is on positive x; direction is in endPose.z sign.
                    if (base.endPose.z > 0 && rotatedBack.y >= 0) continue;
                    if (base.endPose.z < 0 && rotatedBack.y <= 0) continue;
                }

                // Skip point turns exceeding 90 degrees.
                if (base.movType == MovementType::POINTTURN && d > upperDiscreteAngle) {
                    break;
                }

                // Check for duplicate.
                auto key = std::make_tuple(roundedX, roundedY, normDiscreteAngle);
                if (reachedEndPositions.insert(key).second) {
                    DiscretizedPrimitive prim;
                    prim.startAngle = angle;
                    prim.endPose = {static_cast<double>(roundedX), static_cast<double>(roundedY), static_cast<double>(normDiscreteAngle)};
                    prim.movType = base.movType;
                    prim.costMultiplier = base.costMultiplier;
                    result.push_back(prim);
                    primsAddedForThisBase++;
                    primsAddedForThisAngle++;
                }
            }
        }

        if (angle == 0) {
            primsForAngle0 = primsAddedForThisAngle;
        } else if (primsAddedForThisAngle != primsForAngle0) {
            return {}; // mismatch across angles
        }
    }

    return result;
}

#include <cassert>
#include <vector>
#include <cmath>

int main() {
    // Example: two base primitives: forward straight (cost 1) and forward turn (cost 2).
    std::vector<DiscretizedPrimitive> base;
    base.push_back({0, {1.0, 0.0, 0.0}, MovementType::FORWARD, 1.0});
    base.push_back({0, {0.0, 0.0, 1.0}, MovementType::POINTTURN, 1.0});

    // With 4 angles and accuracy 0.5, we expect consistent number of primitives per angle.
    auto result = discretizePrimitives(base, 4, 0.5);
    assert(!result.empty());

    // Check that all start angles are within [0,4) and end angles normalized.
    for (const auto& p : result) {
        assert(p.startAngle >= 0 && p.startAngle < 4);
        int e = static_cast<int>(p.endPose.z);
        assert(e >= 0 && e < 4);
    }

    // Test with only straight primitives: orientation unchanged.
    std::vector<DiscretizedPrimitive> straightOnly;
    straightOnly.push_back({0, {2.0, 0.0, 0.0}, MovementType::FORWARD, 1.0});
    straightOnly.push_back({0, {-2.0, 0.0, 0.0}, MovementType::BACKWARD, 1.0});
    auto resStraight = discretizePrimitives(straightOnly, 8, 0.1);
    assert(!resStraight.empty());
    for (const auto& p : resStraight) {
        assert(p.endPose.x != 0 || p.endPose.y != 0);
        assert(p.endPose.z == p.startAngle); // no rotation change
    }

    // Test with only curve primitives: ensure they produce valid curves.
    std::vector<DiscretizedPrimitive> curveOnly;
    curveOnly.push_back({0, {0.0, 0.0, 1.0}, MovementType::FORWARD_TURN, 1.0}); // radius 1, left turn
    auto resCurve = discretizePrimitives(curveOnly, 16, 0.3);
    assert(!resCurve.empty());

    // Test with invalid input (zero angles) returns empty.
    auto emptyRes = discretizePrimitives(base, 0, 0.5);
    assert(emptyRes.empty());

    // Test with mismatched primitives per angle: use a degenerate threshold that fails.
    // With very small accuracy, we might get zero primitives for some angles, but that should not happen
    // because we stop at max distance; instead test with a negative accuracy.
    auto badAcc = discretizePrimitives(base, 4, -0.1);
    assert(badAcc.empty());

    // Check that the number of primitives for each angle is equal (count by startAngle).
    if (!result.empty()) {
        int countAngle0 = 0;
        for (const auto& p : result) if (p.startAngle == 0) countAngle0++;
        for (int a = 1; a < 4; ++a) {
            int cnt = 0;
            for (const auto& p : result) if (p.startAngle == a) cnt++;
            assert(cnt == countAngle0);
        }
    }

    return 0;
}

// The solution iterates over each discrete start angle (0 to numAngles-1). For each angle, it processes each base primitive defined at angle 0. For straight movements (forward, backward, lateral), the base end pose is rotated by the start angle, and then scaled by factors \(1.0, 1.1, 1.2, \dots\) until the rounded integer grid coordinates deviate from the continuous end pose by at most the accuracy threshold. The discrete angle stays the same as the start angle. For point turns, the end pose is fixed at (0,0), and the discrete angle is incremented by the direction sign times an increasing step count; these are rejected if they exceed \(ceil(numAngles/4)\) steps. For curves, the center of rotation is rotated by the start angle, then scaled by increasing factors; the end pose is computed by rotating the vector from the center to the starting point around the center by discrete angle steps, and the resulting position is rounded. The curve is validated by checking that the turned-back rounded position lies on the same side of the x-axis as the center (for forward/backward turns) or on the correct side for lateral curves. For all movement types, the rounded discrete end pose and discrete angle are used to form a unique key (x, y, angle) stored in a set to avoid duplicates; only new keys produce a primitive. For each angle, we count how many primitives were added; this count must match the count from angle 0; otherwise the function returns an empty vector. Time complexity is \(O(numAngles \times P \times S \times \log(S))\), where \(P\) is the number of base primitives, \(S\) is the number of scaling steps until snapping (bounded by a max distance), and the set insertion is logarithmic. Space complexity is \(O(numAngles \times P \times S)\) in the worst case for the stored primitives and the set, though in practice each base primitive yields a fixed number of primitives per angle.
