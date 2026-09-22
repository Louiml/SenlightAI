Implement a C++ function that simulates a simplified version of the Box2D island position-correction solver for a single revolute joint connecting two rigid bodies. Given the initial positions, orientations, masses, inertias, and a target anchor point, the function must perform `N` iterations of Full Nonlinear Gauss-Seidel position correction, re-computing the Jacobian (radius vectors), effective mass, and position error each iteration, and return the final corrected positions and orientations as a struct. The solver should handle two dynamic bodies, support a position bias factor (Baumgarte-like stabilization), and clamp corrections to prevent overshoot. The input uses `float` for all quantities, and the function must be `const`-correct with no external dependencies beyond the standard library.
// The core algorithm is inspired by the Full NGS position solver described in the Box2D comments. For each iteration, we compute the joint anchor points in world space using the current positions and orientations: `pA = cA + R(aA) * rA_local` and `pB = cB + R(aB) * rB_local`, where `R(theta)` is the 2D rotation matrix. The position error is the difference `C = pB - pA`. The Jacobian is `J = [-I, -skew(rA), I, skew(rB)]` for each body's linear and angular components. The effective mass is `K = invMassA * I + invIA * skew(rA) * skew(rA)^T + invMassB * I + invIB * skew(rB) * skew(rB)^T`, which must be inverted (a 2x2 symmetric matrix). The pseudo-velocity correction is `lambda = -bias * K^-1 * C`, where `bias` is a fraction (e.g., 0.2). Apply the correction to positions and orientations: `cA -= invMassA * lambda`, `aA -= invIA * cross(rA, lambda)`, and similarly for B with opposite signs. After `N` iterations, return the final state. Edge cases include avoiding division by zero when effective mass is singular (e.g., zero inertia), and clamping large corrections to a maximum translation per iteration to prevent instability. Time complexity is `O(N)` iterations, each with constant-time matrix operations, so `O(N)` overall; space is `O(1)`.
#include <cmath>
#include <array>

struct BodyState {
    float x; // position x
    float y; // position y
    float angle; // orientation in radians
};

struct JointSolverResult {
    BodyState bodyA;
    BodyState bodyB;
};

struct JointPositionSolverInput {
    BodyState bodyA;
    BodyState bodyB;
    float invMassA;
    float invIA;
    float invMassB;
    float invIB;
    float localAnchorAX; // relative to body A center of mass
    float localAnchorAY;
    float localAnchorBX; // relative to body B center of mass
    float localAnchorBY;
    int iterations; // number of NGS iterations
    float bias; // position correction bias factor in [0,1]
};

// Helper: 2D cross product of vector and scalar (returns vector)
inline std::array<float,2> crossVecScalar(const std::array<float,2>& v, float s) {
    return {s * v[1], -s * v[0]};
}

// Helper: scalar cross product of two vectors
inline float crossVecVec(const std::array<float,2>& a, const std::array<float,2>& b) {
    return a[0]*b[1] - a[1]*b[0];
}

// Helper: rotate a local vector by angle
inline std::array<float,2> rotate(float cosA, float sinA, const std::array<float,2>& v) {
    return {cosA*v[0] - sinA*v[1], sinA*v[0] + cosA*v[1]};
}

// Solve position constraints for a single revolute joint using Full NGS.
JointSolverResult solveRevoluteJointPosition(const JointPositionSolverInput& input) {
    // Initialize state from input
    BodyState stateA = input.bodyA;
    BodyState stateB = input.bodyB;

    // Bias factor clamped to [0,1]
    float bias = std::fmax(0.0f, std::fmin(1.0f, input.bias));

    for (int iter = 0; iter < input.iterations; ++iter) {
        // Current orientations
        float cosA = std::cos(stateA.angle);
        float sinA = std::sin(stateA.angle);
        float cosB = std::cos(stateB.angle);
        float sinB = std::sin(stateB.angle);

        // World-space anchor points
        std::array<float,2> rA = rotate(cosA, sinA, {input.localAnchorAX, input.localAnchorAY});
        std::array<float,2> rB = rotate(cosB, sinB, {input.localAnchorBX, input.localAnchorBY});
        std::array<float,2> pA = {stateA.x + rA[0], stateA.y + rA[1]};
        std::array<float,2> pB = {stateB.x + rB[0], stateB.y + rB[1]};

        // Position error C = pB - pA
        std::array<float,2> C = {pB[0] - pA[0], pB[1] - pA[1]};

        // If error is tiny, skip
        if (C[0]*C[0] + C[1]*C[1] < 1e-12f) continue;

        // Effective mass K = invMassA*I + invIA*skew(rA)*skew(rA)^T + invMassB*I + invIB*skew(rB)*skew(rB)^T
        // skew(v) = [0 -v_y; v_x 0], so skew(v)*skew(v)^T = [v_y^2, -v_x*v_y; -v_x*v_y, v_x^2]
        float K11 = input.invMassA + input.invIA * rA[1]*rA[1] + input.invMassB + input.invIB * rB[1]*rB[1];
        float K12 = -input.invIA * rA[0]*rA[1] - input.invIB * rB[0]*rB[1];
        float K22 = input.invMassA + input.invIA * rA[0]*rA[0] + input.invMassB + input.invIB * rB[0]*rB[0];

        // Invert 2x2 symmetric matrix
        float det = K11*K22 - K12*K12;
        if (std::fabs(det) < 1e-12f) break; // singular, avoid division
        float invDet = 1.0f / det;
        float invK11 = K22 * invDet;
        float invK12 = -K12 * invDet;
        float invK22 = K11 * invDet;

        // Compute lambda = -bias * invK * C
        std::array<float,2> lambda;
        lambda[0] = -bias * (invK11 * C[0] + invK12 * C[1]);
        lambda[1] = -bias * (invK12 * C[0] + invK22 * C[1]);

        // Clamp lambda magnitude to a maximum (e.g., 1.0 per iteration) to avoid overshoot
        float lambdaMagSq = lambda[0]*lambda[0] + lambda[1]*lambda[1];
        const float maxLambda = 1.0f;
        if (lambdaMagSq > maxLambda*maxLambda) {
            float scale = maxLambda / std::sqrt(lambdaMagSq);
            lambda[0] *= scale;
            lambda[1] *= scale;
        }

        // Apply corrections: body A moves opposite to lambda, body B moves with lambda
        // Linear: cA -= invMassA * lambda; cB += invMassB * lambda
        stateA.x -= input.invMassA * lambda[0];
        stateA.y -= input.invMassA * lambda[1];
        stateB.x += input.invMassB * lambda[0];
        stateB.y += input.invMassB * lambda[1];

        // Angular: aA -= invIA * cross(rA, lambda); aB += invIB * cross(rB, lambda)
        // cross(r, lambda) = r_x * lambda_y - r_y * lambda_x
        float crossA = crossVecVec(rA, lambda);
        float crossB = crossVecVec(rB, lambda);
        stateA.angle -= input.invIA * crossA;
        stateB.angle += input.invIB * crossB;
    }

    JointSolverResult result;
    result.bodyA = stateA;
    result.bodyB = stateB;
    return result;
}
#include <cassert>
#include <cmath>

// Include the solution function here (in a real test, this would be linked separately)
// For this standalone test, copy the solution code above into the same file.

int main() {
    // Test 1: Two bodies with no offset, should not move if no error
    {
        JointPositionSolverInput input;
        input.bodyA = {0.0f, 0.0f, 0.0f};
        input.bodyB = {0.0f, 0.0f, 0.0f};
        input.invMassA = 1.0f;
        input.invIA = 1.0f;
        input.invMassB = 1.0f;
        input.invIB = 1.0f;
        input.localAnchorAX = 0.0f;
        input.localAnchorAY = 0.0f;
        input.localAnchorBX = 0.0f;
        input.localAnchorBY = 0.0f;
        input.iterations = 1;
        input.bias = 0.2f;

        auto result = solveRevoluteJointPosition(input);
        assert(std::fabs(result.bodyA.x - 0.0f) < 1e-5f);
        assert(std::fabs(result.bodyA.y - 0.0f) < 1e-5f);
        assert(std::fabs(result.bodyB.x - 0.0f) < 1e-5f);
        assert(std::fabs(result.bodyB.y - 0.0f) < 1e-5f);
        assert(std::fabs(result.bodyA.angle - 0.0f) < 1e-5f);
        assert(std::fabs(result.bodyB.angle - 0.0f) < 1e-5f);
    }

    // Test 2: One body offset in x, should pull them together
    {
        JointPositionSolverInput input;
        input.bodyA = {0.0f, 0.0f, 0.0f};
        input.bodyB = {1.0f, 0.0f, 0.0f};
        input.invMassA = 1.0f;
        input.invIA = 1.0f;
        input.invMassB = 1.0f;
        input.invIB = 1.0f;
        input.localAnchorAX = 0.0f;
        input.localAnchorAY = 0.0f;
        input.localAnchorBX = 0.0f;
        input.localAnchorBY = 0.0f;
        input.iterations = 100;
        input.bias = 1.0f; // full correction

        auto result = solveRevoluteJointPosition(input);
        // After many iterations with bias=1, should converge to identical positions
        assert(std::fabs(result.bodyA.x - result.bodyB.x) < 1e-4f);
        assert(std::fabs(result.bodyA.y - result.bodyB.y) < 1e-4f);
    }

    // Test 3: Angular offset with local anchors
    {
        JointPositionSolverInput input;
        input.bodyA = {0.0f, 0.0f, 0.0f};
        input.bodyB = {0.0f, 0.0f, 1.0f}; // B rotated 1 rad
        input.invMassA = 1.0f;
        input.invIA = 1.0f;
        input.invMassB = 1.0f;
        input.invIB = 1.0f;
        input.localAnchorAX = 0.0f;
        input.localAnchorAY = 0.0f;
        input.localAnchorBX = 1.0f;
        input.localAnchorBY = 0.0f; // anchor at (1,0) local to B
        input.iterations = 100;
        input.bias = 1.0f;

        auto result = solveRevoluteJointPosition(input);
        // World anchor of B: rotate (1,0) by 1 rad => (cos1, sin1) ~ (0.54, 0.84)
        // After correction, B's world anchor should be at (0,0) approximately
        float cosB = std::cos(result.bodyB.angle);
        float sinB = std::sin(result.bodyB.angle);
        float worldAnchorBX = result.bodyB.x + cosB * input.localAnchorBX - sinB * input.localAnchorBY;
        float worldAnchorBY = result.bodyB.y + sinB * input.localAnchorBX + cosB * input.localAnchorBY;
        assert(std::fabs(worldAnchorBX - 0.0f) < 0.01f);
        assert(std::fabs(worldAnchorBY - 0.0f) < 0.01f);
    }

    // Test 4: Bias factor reduces correction magnitude
    {
        JointPositionSolverInput input;
        input.bodyA = {0.0f, 0.0f, 0.0f};
        input.bodyB = {1.0f, 0.0f, 0.0f};
        input.invMassA = 1.0f;
        input.invIA = 1.0f;
        input.invMassB = 1.0f;
        input.invIB = 1.0f;
        input.localAnchorAX = 0.0f;
        input.localAnchorAY = 0.0f;
        input.localAnchorBX = 0.0f;
        input.localAnchorBY = 0.0f;
        input.iterations = 1;
        input.bias = 0.2f;

        auto result = solveRevoluteJointPosition(input);
        // After one iteration, error reduced but not eliminated
        float errorAfter = std::fabs(result.bodyA.x - result.bodyB.x);
        assert(errorAfter < 1.0f - 0.1f); // reduced from 1.0
        assert(errorAfter > 0.0f); // still positive error
    }

    // Test 5: Stability with heavy body (small invMass)
    {
        JointPositionSolverInput input;
        input.bodyA = {0.0f, 0.0f, 0.0f};
        input.bodyB = {2.0f, 0.0f, 0.0f};
        input.invMassA = 0.01f; // heavy
        input.invIA = 0.01f;
        input.invMassB = 1.0f;
        input.invIB = 1.0f;
        input.localAnchorAX = 0.0f;
        input.localAnchorAY = 0.0f;
        input.localAnchorBX = 0.0f;
        input.localAnchorBY = 0.0f;
        input.iterations = 100;
        input.bias = 0.2f;

        auto result = solveRevoluteJointPosition(input);
        // Heavy body should move less; the light body moves more
        assert(std::fabs(result.bodyA.x) < std::fabs(result.bodyB.x - 2.0f));
        // Both should move toward each other
        assert(result.bodyA.x > 0.0f); // heavy moves right
        assert(result.bodyB.x < 2.0f); // light moves left
    }

    return 0;
}
