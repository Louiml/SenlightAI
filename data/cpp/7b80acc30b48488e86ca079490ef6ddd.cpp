// Write a standalone C++ function that computes the absolute poses of a joint hierarchy given a vector of relative poses and a parent-index list. The input is a vector of `glm::quat` rotations and `glm::vec3` translations (one per joint), plus a `std::vector<int>` where `parent[i]` gives the parent index of joint `i` (or `-1` for the root). The function must return a `std::vector<AnimPose>` where each entry is the pose in world/absolute space computed as `absolutePoses[i] = absolutePoses[parent[i]] * relativePoses[i]`, with the root’s absolute pose equal to its relative pose. Assume the chain is a tree (no cycles) and that parent indices are smaller than child indices (guarantees a valid top-down traversal). The function should handle at least one root joint and correctly propagate rotations and translations using quaternion–vector multiplication (rotation applies to translation offset, then translation is added). Do not use external libraries beyond GLM.

// The core algorithm is a simple forward pass over all joints in increasing index order. Because parent indices are guaranteed to be smaller than child indices, iterating from `i = 0` to `n-1` ensures that when we process joint `i`, its parent’s absolute pose (if any) has already been computed. For each joint:
// - If `parent[i] == -1`, set `abs[i] = rel[i]`.
// - Else, compute `abs[i] = abs[parent[i]] * rel[i]`, where multiplication of two `AnimPose` objects is defined as concatenation: the resulting rotation is `abs[parent].rot * rel.rot`, and the resulting translation is `abs[parent].trans + abs[parent].rot * rel.trans` (using GLM’s quaternion–vector multiplication).  
// Edge cases: (1) Empty input → return an empty vector; (2) single joint with root (parent = -1) → its absolute pose is its relative pose; (3) a joint with a valid rotation and zero translation still propagates correctly due to quaternion multiplication. Time complexity is O(n) because each joint is processed exactly once. Space complexity is O(n) for the output vector. The approach is robust to any tree structure and does not require recursion or a topological sort because of the parent-index ordering assumption.

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <vector>

// Minimal pose structure with rotation and translation.
struct AnimPose {
    glm::quat rot = glm::quat(1.0f, 0.0f, 0.0f, 0.0f); // identity quaternion
    glm::vec3 trans = glm::vec3(0.0f);

    // Concatenation of two poses: apply `other` after `this`.
    AnimPose operator*(const AnimPose& other) const {
        return AnimPose{ rot * other.rot, trans + rot * other.trans };
    }
};

// Compute absolute poses from a list of relative poses and a parent-index array.
// parent[i] is the index of the parent joint, or -1 for the root.
std::vector<AnimPose> computeAbsolutePoses(const std::vector<AnimPose>& relativePoses,
                                           const std::vector<int>& parentIndices) {
    const size_t n = relativePoses.size();
    if (n == 0 || n != parentIndices.size()) {
        return {}; // invalid input
    }

    std::vector<AnimPose> absolutePoses(n);
    for (size_t i = 0; i < n; ++i) {
        if (parentIndices[i] < 0) {
            // Root joint: absolute pose equals its relative pose.
            absolutePoses[i] = relativePoses[i];
        } else {
            // Non-root joint: absolute pose = parent's absolute pose * relative pose.
            const int parent = parentIndices[i];
            // Parent index must be smaller than child index (guaranteed by problem).
            absolutePoses[i] = absolutePoses[parent] * relativePoses[i];
        }
    }
    return absolutePoses;
}

#include <cassert>
#include <cmath>

// Helper to compare two quaternions approximately.
bool quatEquals(const glm::quat& a, const glm::quat& b, float eps = 1e-5f) {
    return glm::length(a - b) < eps || glm::length(a + b) < eps; // account for sign
}

// Helper to compare two vectors approximately.
bool vecEquals(const glm::vec3& a, const glm::vec3& b, float eps = 1e-5f) {
    return glm::length(a - b) < eps;
}

// Helper to compare two AnimPose approximately.
bool poseEquals(const AnimPose& a, const AnimPose& b, float eps = 1e-5f) {
    return quatEquals(a.rot, b.rot, eps) && vecEquals(a.trans, b.trans, eps);
}

int main() {
    // Test 1: Empty input
    {
        std::vector<AnimPose> rels;
        std::vector<int> parents;
        auto abs = computeAbsolutePoses(rels, parents);
        assert(abs.empty());
    }

    // Test 2: Single root joint
    {
        std::vector<AnimPose> rels = { AnimPose{ glm::quat(0.5f, 0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 2.0f, 3.0f) } };
        std::vector<int> parents = { -1 };
        auto abs = computeAbsolutePoses(rels, parents);
        assert(abs.size() == 1);
        assert(poseEquals(abs[0], rels[0]));
    }

    // Test 3: Two-joint chain, translation propagation
    {
        // Root at origin with identity rotation.
        AnimPose root{ glm::quat(1.0f, 0.0f, 0.0f, 0.0f), glm::vec3(0.0f) };
        // Child offset by (1,0,0) in root space.
        AnimPose child{ glm::quat(1.0f, 0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f) };
        std::vector<AnimPose> rels = { root, child };
        std::vector<int> parents = { -1, 0 };
        auto abs = computeAbsolutePoses(rels, parents);
        assert(abs.size() == 2);
        assert(poseEquals(abs[0], root));
        assert(poseEquals(abs[1], AnimPose{ glm::quat(1.0f, 0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f) }));
    }

    // Test 4: Two-joint chain with rotation propagation
    {
        // Root rotated 90° around Z.
        glm::quat rotZ90(cosf(3.14159265f / 4.0f), 0.0f, 0.0f, sinf(3.14159265f / 4.0f));
        AnimPose root{ rotZ90, glm::vec3(0.0f) };
        // Child relative translation (1,0,0) should become (0,1,0) in parent space.
        AnimPose child{ glm::quat(1.0f, 0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f) };
        std::vector<AnimPose> rels = { root, child };
        std::vector<int> parents = { -1, 0 };
        auto abs = computeAbsolutePoses(rels, parents);
        assert(abs.size() == 2);
        // Child absolute rotation = root rotation * identity = root rotation.
        assert(quatEquals(abs[1].rot, rotZ90));
        // Child absolute translation = (0,0,0) + rotZ90 * (1,0,0) = (0,1,0)
        glm::vec3 expectedTrans = rotZ90 * glm::vec3(1.0f, 0.0f, 0.0f);
        assert(vecEquals(abs[1].trans, expectedTrans));
    }

    // Test 5: Three-joint chain with translation offsets
    {
        AnimPose a{ glm::quat(1.0f, 0.0f, 0.0f, 0.0f), glm::vec3(0.0f) };
        AnimPose b{ glm::quat(1.0f, 0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f) };
        AnimPose c{ glm::quat(1.0f, 0.0f, 0.0f, 0.0f), glm::vec3(2.0f, 3.0f, 0.0f) };
        std::vector<AnimPose> rels = { a, b, c };
        std::vector<int> parents = { -1, 0, 1 };
        auto abs = computeAbsolutePoses(rels, parents);
        assert(abs.size() == 3);
        assert(poseEquals(abs[0], a));
        assert(poseEquals(abs[1], AnimPose{ glm::quat(1.0f, 0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f) }));
        // c absolute = (1,0,0) + (2,3,0) = (3,3,0)
        assert(poseEquals(abs[2], AnimPose{ glm::quat(1.0f, 0.0f, 0.0f, 0.0f), glm::vec3(3.0f, 3.0f, 0.0f) }));
    }

    // Test 6: Mixed tree (two children under root)
    {
        AnimPose root{ glm::quat(1.0f, 0.0f, 0.0f, 0.0f), glm::vec3(0.0f) };
        AnimPose child1{ glm::quat(1.0f, 0.0f, 0.0f, 0.0f), glm::vec3(5.0f, 0.0f, 0.0f) };
        AnimPose child2{ glm::quat(1.0f, 0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 5.0f, 0.0f) };
        std::vector<AnimPose> rels = { root, child1, child2 };
        std::vector<int> parents = { -1, 0, 0 };
        auto abs = computeAbsolutePoses(rels, parents);
        assert(abs.size() == 3);
        assert(poseEquals(abs[0], root));
        assert(poseEquals(abs[1], AnimPose{ glm::quat(1.0f, 0.0f, 0.0f, 0.0f), glm::vec3(5.0f, 0.0f, 0.0f) }));
        assert(poseEquals(abs[2], AnimPose{ glm::quat(1.0f, 0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 5.0f, 0.0f) }));
    }

    // Test 7: Invalid input sizes
    {
        std::vector<AnimPose> rels = { AnimPose{} };
        std::vector<int> parents = { -1, 0 }; // mismatch
        auto abs = computeAbsolutePoses(rels, parents);
        assert(abs.empty());
    }

    return 0;
}
