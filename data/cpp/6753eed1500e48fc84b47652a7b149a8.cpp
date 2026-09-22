/*
Write a standalone C++ function named `collisionRepulsion` that takes a target label represented by a 2D position (as a pair of floats) and a half-size (as a pair of floats), plus a vector of other labels each with an id, 2D position, and half-size. The function must return a 2D vector (as a `std::pair<float,float>`) representing the sum of normalized repulsion directions from all other labels whose axis-aligned bounding boxes (AABBs) overlap the target’s AABB. Overlap occurs if the absolute differences in both x and y coordinates are strictly less than the sum of the respective half-sizes. Skip the label with the same id as the target. If no collisions occur, return (0,0). The input vector may be empty, may contain duplicate positions, or may contain labels with identical ids (but only one with the target’s id). Use `std::vector` and `std::pair` from the standard library.
*/
#include <vector>
#include <utility>
#include <cmath>

// Compute the sum of normalized repulsion directions from colliding labels.
// Target is defined by (targetPosX, targetPosY) and (targetHalfX, targetHalfY).
// Each label in others has fields: id, posX, posY, halfX, halfY.
// Returns a pair (fx, fy) representing the total repulsion force.
std::pair<float, float> collisionRepulsion(
    float targetPosX, float targetPosY, float targetHalfX, float targetHalfY,
    int targetId,
    const std::vector<std::tuple<int, float, float, float, float>>& others) {
    
    float totalX = 0.0f;
    float totalY = 0.0f;
    
    for (const auto& other : others) {
        int otherId = std::get<0>(other);
        float otherPosX = std::get<1>(other);
        float otherPosY = std::get<2>(other);
        float otherHalfX = std::get<3>(other);
        float otherHalfY = std::get<4>(other);
        
        if (otherId == targetId) continue;
        
        // AABB overlap test (strict inequality)
        bool overlapX = std::abs(targetPosX - otherPosX) < (targetHalfX + otherHalfX);
        bool overlapY = std::abs(targetPosY - otherPosY) < (targetHalfY + otherHalfY);
        
        if (!overlapX || !overlapY) continue;
        
        // Direction from other to target
        float dx = targetPosX - otherPosX;
        float dy = targetPosY - otherPosY;
        float norm = std::sqrt(dx * dx + dy * dy);
        
        if (norm > 0.0f) {
            totalX += dx / norm;
            totalY += dy / norm;
        } else {
            // Degenerate case: exact same center, use arbitrary normalized direction
            totalX += 1.0f;
            totalY += 0.0f;
        }
    }
    
    return {totalX, totalY};
}
#include <cassert>
#include <tuple>
#include <vector>

// The solution function is declared above; include the definition here or in a header.

int main() {
    // Test 1: No collisions, single other far away
    {
        std::vector<std::tuple<int, float, float, float, float>> others = {{1, 10.0f, 10.0f, 1.0f, 1.0f}};
        auto result = collisionRepulsion(0.0f, 0.0f, 1.0f, 1.0f, 0, others);
        assert(result.first == 0.0f && result.second == 0.0f);
    }
    
    // Test 2: One collision directly to the right (other at +2, halfWidths sum = 2, strict < fails because |0-2| = 2 not < 2)
    // Use distance 1.9 to cause overlap
    {
        std::vector<std::tuple<int, float, float, float, float>> others = {{1, 1.9f, 0.0f, 1.0f, 1.0f}};
        auto result = collisionRepulsion(0.0f, 0.0f, 1.0f, 1.0f, 0, others);
        // dx = -1.9, dy = 0; normalized = (-1, 0)
        assert(std::abs(result.first - (-1.0f)) < 1e-5f);
        assert(std::abs(result.second - 0.0f) < 1e-5f);
    }
    
    // Test 3: Two collisions, one left one right, symmetric -> sum cancels
    {
        std::vector<std::tuple<int, float, float, float, float>> others = {
            {1, -1.9f, 0.0f, 1.0f, 1.0f},
            {2, 1.9f, 0.0f, 1.0f, 1.0f}
        };
        auto result = collisionRepulsion(0.0f, 0.0f, 1.0f, 1.0f, 0, others);
        assert(std::abs(result.first) < 1e-5f);
        assert(std::abs(result.second) < 1e-5f);
    }
    
    // Test 4: Skipping same id
    {
        std::vector<std::tuple<int, float, float, float, float>> others = {
            {0, 0.0f, 0.0f, 1.0f, 1.0f},  // same id, should be ignored
            {1, 1.9f, 0.0f, 1.0f, 1.0f}
        };
        auto result = collisionRepulsion(0.0f, 0.0f, 1.0f, 1.0f, 0, others);
        // Only collision with id=1
        assert(std::abs(result.first - (-1.0f)) < 1e-5f);
        assert(std::abs(result.second) < 1e-5f);
    }
    
    // Test 5: Exact same center, should contribute (1,0)
    {
        std::vector<std::tuple<int, float, float, float, float>> others = {
            {1, 0.0f, 0.0f, 1.0f, 1.0f}
        };
        auto result = collisionRepulsion(0.0f, 0.0f, 1.0f, 1.0f, 0, others);
        assert(result.first == 1.0f && result.second == 0.0f);
    }
    
    // Test 6: Empty vector
    {
        std::vector<std::tuple<int, float, float, float, float>> others;
        auto result = collisionRepulsion(0.0f, 0.0f, 1.0f, 1.0f, 0, others);
        assert(result.first == 0.0f && result.second == 0.0f);
    }
    
    // Test 7: Overlap in X only (no Y overlap) -> no collision
    {
        std::vector<std::tuple<int, float, float, float, float>> others = {
            {1, 1.9f, 3.0f, 1.0f, 1.0f}  // y difference = 3 >= 2, no overlap
        };
        auto result = collisionRepulsion(0.0f, 0.0f, 1.0f, 1.0f, 0, others);
        assert(result.first == 0.0f && result.second == 0.0f);
    }
    
    // Test 8: Diagonal collision, normalized direction
    {
        // Other at (1,1), both half sizes 1 -> overlap in both axes, dx=-1, dy=-1, norm = sqrt(2)
        std::vector<std::tuple<int, float, float, float, float>> others = {
            {1, 1.0f, 1.0f, 1.0f, 1.0f}
        };
        auto result = collisionRepulsion(0.0f, 0.0f, 1.0f, 1.0f, 0, others);
        float expectedX = -1.0f / std::sqrt(2.0f);
        float expectedY = -1.0f / std::sqrt(2.0f);
        assert(std::abs(result.first - expectedX) < 1e-5f);
        assert(std::abs(result.second - expectedY) < 1e-5f);
    }
    
    return 0;
}
// The algorithm iterates over every other label in the vector, skipping any with the same id as the target. For each candidate, compute the AABB overlap test: for each axis independently, check if the absolute difference between the target’s center and the candidate’s center is less than the sum of the corresponding half-sizes (strict `<` to match typical AABB overlap, since touching edges do not count as collision). If both axes pass, compute the direction vector from the candidate to the target (target position minus candidate position), normalize it (divide by its Euclidean norm), and add it to an accumulator. Edge cases: if the two centers are exactly equal, the difference vector is zero, and normalization would divide by zero; in that case, contribute a fixed arbitrary unit vector, e.g., (1,0), to avoid undefined behavior. After processing all candidates, return the accumulated vector. The time complexity is O(n) where n is the number of labels, since each is checked once. Space complexity is O(1) aside from temporary variables and the output.
