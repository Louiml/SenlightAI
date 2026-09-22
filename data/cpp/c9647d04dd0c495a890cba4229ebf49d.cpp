/*
Write a C++ function named `findFarthestPoint` that takes an immutable reference to a `std::vector<btVector3>` (where `btVector3` is a simple 3D vector type with `x`, `y`, `z` public members and a `dot` method) and an immutable reference to a direction vector `dir`. The function must return a `btVector3` representing the point in the cloud that maximizes the dot product with `dir`. If the cloud is empty, return a zero vector `btVector3(0,0,0)`. If all dot products are equal (e.g., a single point or symmetric cloud), return the first point that achieves the maximum. The direction vector is not guaranteed to be normalized, and its length can be zero; in that case, treat it as the positive x-axis direction `(1,0,0)`. The function should use a linear scan to compare dot products and must not modify the input vectors.
*/
#include <vector>
#include <limits>

// Minimal 3D vector type with public members and a dot product method.
struct btVector3 {
    double x, y, z;
    
    btVector3(double x = 0.0, double y = 0.0, double z = 0.0)
        : x(x), y(y), z(z) {}
    
    double dot(const btVector3& other) const {
        return x * other.x + y * other.y + z * other.z;
    }
    
    double lengthSquared() const {
        return dot(*this);
    }
};

/**
 * Find the point in the cloud that maximizes the dot product with the given direction.
 * If the direction is zero, use (1,0,0). Return zero vector if the cloud is empty.
 */
btVector3 findFarthestPoint(const std::vector<btVector3>& cloud, const btVector3& dir) {
    // Use a default direction if the given direction has negligible length.
    btVector3 effectiveDir = dir;
    if (dir.lengthSquared() < 1e-12) {
        effectiveDir = btVector3(1.0, 0.0, 0.0);
    }
    
    int bestIndex = -1;
    double bestDot = -std::numeric_limits<double>::infinity();
    
    for (int i = 0; i < static_cast<int>(cloud.size()); ++i) {
        double currentDot = cloud[i].dot(effectiveDir);
        // Strict greater to keep the first occurrence in case of ties.
        if (currentDot > bestDot) {
            bestDot = currentDot;
            bestIndex = i;
        }
    }
    
    if (bestIndex == -1) {
        return btVector3(0.0, 0.0, 0.0);
    }
    return cloud[bestIndex];
}
#include <cassert>

int main() {
    // Test 1: Normal case with a clear maximum.
    std::vector<btVector3> cloud1 = {btVector3(1,0,0), btVector3(2,3,4), btVector3(-1,5,2)};
    btVector3 dir1(0,1,0);
    btVector3 result1 = findFarthestPoint(cloud1, dir1);
    assert(result1.x == -1 && result1.y == 5 && result1.z == 2); // Max dot = 5
    
    // Test 2: Empty cloud returns zero vector.
    std::vector<btVector3> cloud2;
    btVector3 result2 = findFarthestPoint(cloud2, btVector3(1,1,1));
    assert(result2.x == 0 && result2.y == 0 && result2.z == 0);
    
    // Test 3: Zero direction defaults to positive x-axis.
    std::vector<btVector3> cloud3 = {btVector3(0,10,0), btVector3(3,0,0), btVector3(-2,1,1)};
    btVector3 result3 = findFarthestPoint(cloud3, btVector3(0,0,0));
    assert(result3.x == 3 && result3.y == 0 && result3.z == 0); // Max along x = 3
    
    // Test 4: Single point returns that point.
    std::vector<btVector3> cloud4 = {btVector3(4,-2,9)};
    btVector3 result4 = findFarthestPoint(cloud4, btVector3(0.5,-1,2));
    assert(result4.x == 4 && result4.y == -2 && result4.z == 9);
    
    // Test 5: All identical dot products returns first point.
    std::vector<btVector3> cloud5 = {btVector3(1,1,0), btVector3(2,2,0), btVector3(-3,-3,0)};
    btVector3 dir5 = btVector3(1,1,0);
    btVector3 result5 = findFarthestPoint(cloud5, dir5);
    // All dots = 2,4,-6? Actually 1*1+1*1=2, 2*1+2*1=4, -3+-3=-6. So max is (2,2,0).
    assert(result5.x == 2 && result5.y == 2 && result5.z == 0);
    
    // Test 6: Ties in dot product (symmetric points) returns first one.
    std::vector<btVector3> cloud6 = {btVector3(0,1,0), btVector3(0,1,0), btVector3(0,2,0)};
    btVector3 dir6 = btVector3(0,1,0);
    btVector3 result6 = findFarthestPoint(cloud6, dir6);
    assert(result6.x == 0 && result6.y == 2 && result6.z == 0); // Only unique max is (0,2,0)
    
    // Test 7: Negative direction works correctly.
    std::vector<btVector3> cloud7 = {btVector3(5,5,5), btVector3(-1,-1,-1), btVector3(0,0,0)};
    btVector3 dir7 = btVector3(-1,-1,-1);
    btVector3 result7 = findFarthestPoint(cloud7, dir7);
    // Dots: -15, 3, 0 -> max is -1,-1,-1
    assert(result7.x == -1 && result7.y == -1 && result7.z == -1);
    
    // Test 8: Large values and normalization independence.
    std::vector<btVector3> cloud8 = {btVector3(100,0,0), btVector3(0,1e6,0), btVector3(0,0,500)};
    btVector3 dir8 = btVector3(0.0, 1e-6, 0.0);
    btVector3 result8 = findFarthestPoint(cloud8, dir8);
    // Scaling direction does not change index; max is (0,1e6,0)
    assert(result8.x == 0 && result8.y == 1e6 && result8.z == 0);
    
    return 0;
}
// The solution iterates over all points once, computing the dot product of each with the (possibly defaulted) direction vector. If the direction's squared length is below a tiny epsilon (e.g., `1e-12`), replace it with `(1,0,0)` to avoid numerical issues when normalizing (though normalization is not required for dot product comparison since scaling uniformly does not change the maximizing index). We maintain the best dot product so far and the index of the corresponding point. Initialize the best dot product to negative infinity (e.g., `-1e30`) so that the first point always becomes the current best. For each point, compute `dot = p.x*dir.x + p.y*dir.y + p.z*dir.z`. If `dot > bestDot` (strictly greater to preserve first-occurrence tie-breaking), update bestDot and bestIndex. After the loop, if bestIndex remains -1 (empty input), return the zero vector; otherwise return the point at bestIndex. Time complexity is O(N) for N points, and space complexity is O(1) beyond the input. Edge cases: empty vector, zero-length direction, a single point, and all points yielding identical dot products (handled by strict comparison).
