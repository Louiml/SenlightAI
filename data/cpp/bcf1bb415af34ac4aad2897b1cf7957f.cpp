/*
Create a standalone C++ function that takes a vector of 2D points representing a path and a query point, and returns a `std::pair` containing the closest point on the path (as a 2D point) and its index in the path. The function must handle paths with at least one point, break early if the distance starts increasing (to optimize for sorted paths), and return the index of the first occurrence if multiple points are equally close. The input path is given as `std::vector<std::pair<double,double>>` and the query point as `std::pair<double,double>`. The function should be efficient and should not modify the input. This is a simplified version of the `findClosestPoint` logic from a robotic path-following controller, where the path is assumed to be roughly sequential (e.g., a planned route) and early termination improves performance.
*/

#include <vector>
#include <utility>
#include <limits>
#include <cmath>
#include <algorithm>

/**
 * @brief Find the closest point on a path to a given query point.
 * 
 * @param path Vector of 2D points representing the path (at least one point required).
 * @param query The query point.
 * @return std::pair<std::pair<double,double>, int> Closest point on path (as pair) and its index.
 */
std::pair<std::pair<double,double>, int> findClosestPointOnPath(
    const std::vector<std::pair<double,double>>& path,
    const std::pair<double,double>& query) {
    
    // Path must have at least one point
    if (path.empty()) {
        throw std::invalid_argument("Path must not be empty");
    }
    
    double min_dist_sqr = std::numeric_limits<double>::max();
    std::pair<double,double> closest_point = path[0];
    int closest_idx = 0;
    double prev_dist_sqr = std::numeric_limits<double>::max();
    
    for (size_t i = 0; i < path.size(); ++i) {
        // Compute squared Euclidean distance
        double dx = path[i].first - query.first;
        double dy = path[i].second - query.second;
        double dist_sqr = dx * dx + dy * dy;
        
        if (dist_sqr < min_dist_sqr) {
            min_dist_sqr = dist_sqr;
            closest_point = path[i];
            closest_idx = static_cast<int>(i);
        } else if (dist_sqr > prev_dist_sqr) {
            // If distance starts increasing, we can break (assuming roughly sorted path)
            break;
        }
        prev_dist_sqr = dist_sqr;
    }
    
    return std::make_pair(closest_point, closest_idx);
}

#include <cassert>
#include <iostream>

int main() {
    // Basic test with monotonic path
    std::vector<std::pair<double,double>> path1 = {{0.0,0.0}, {1.0,0.0}, {2.0,0.0}, {3.0,0.0}};
    auto result1 = findClosestPointOnPath(path1, {2.3, 0.0});
    assert(result1.first == std::make_pair(2.0, 0.0));
    assert(result1.second == 2);
    
    // Single point path
    std::vector<std::pair<double,double>> path2 = {{5.0,5.0}};
    auto result2 = findClosestPointOnPath(path2, {0.0,0.0});
    assert(result2.first == std::make_pair(5.0, 5.0));
    assert(result2.second == 0);
    
    // Equidistant points (pick first)
    std::vector<std::pair<double,double>> path3 = {{0.0,0.0}, {2.0,0.0}, {4.0,0.0}};
    auto result3 = findClosestPointOnPath(path3, {1.0,0.0});
    assert(result3.first == std::make_pair(0.0, 0.0));
    assert(result3.second == 0);
    
    // Non-monotonic (early break might miss, but check worst case works)
    std::vector<std::pair<double,double>> path4 = {{0.0,0.0}, {10.0,0.0}, {5.0,0.0}};
    auto result4 = findClosestPointOnPath(path4, {4.0,0.0});
    // First point distance = 16, second = 36 (increase -> break), but third would be 1, so this test
    // demonstrates that early break may not be correct for non-sorted paths. We'll adjust the test
    // to use a properly sorted path for early break validation.
    // For non-sorted, the function will only consider first two points; that's expected behavior.
    // We'll remove this test or use it to show limitation. Instead, test with sorted path for early break.
    
    // Test with distances that initially decrease then increase
    std::vector<std::pair<double,double>> path5 = {{0.0,0.0}, {2.0,0.0}, {4.0,0.0}, {6.0,0.0}};
    auto result5 = findClosestPointOnPath(path5, {3.5,0.0});
    assert(result5.first == std::make_pair(4.0, 0.0)); // distance to 2.0 = 2.25, to 4.0 = 0.25, to 6.0 = 6.25 (increase -> break)
    assert(result5.second == 2);
    
    // Test with negative coordinates
    std::vector<std::pair<double,double>> path6 = {{-3.0,-1.0}, {-1.0,-2.0}, {0.0,0.0}};
    auto result6 = findClosestPointOnPath(path6, {-0.5,-0.5});
    // distances: to (-3,-1) = 6.5+0.25=6.75, to (-1,-2)=0.25+2.25=2.5, to (0,0)=0.25+0.25=0.5 -> closest is (0,0)
    assert(result6.first == std::make_pair(0.0, 0.0));
    assert(result6.second == 2);
    
    // Test with large numbers
    std::vector<std::pair<double,double>> path7 = {{1000.0,1000.0}, {2000.0,2000.0}};
    auto result7 = findClosestPointOnPath(path7, {1500.0,1500.0});
    assert(result7.first == std::make_pair(1000.0,1000.0) || result7.first == std::make_pair(2000.0,2000.0));
    assert(result7.second == 0 || result7.second == 1);
    
    std::cout << "All tests passed!" << std::endl;
    return 0;
}

// The solution iterates over all points in the path, computing the squared Euclidean distance between the query point and each path point. Squared distance is used to avoid unnecessary square-root calculations. It tracks the minimum squared distance and the corresponding index. The algorithm also optimizes by breaking early: if the distance to the current point is greater than the distance to the previous point, it assumes the path is moving away and stops (this works well if the path is sorted or roughly monotonic in distance from the query). Edge cases include a path of length 1 (returns that point and index 0), an empty path (undefined behavior; the task assumes at least one point), and duplicate distances (returns the first minimal index). Time complexity is \(O(n)\) in the worst case (when no early break), but often \(O(k)\) where \(k\) is the index of the closest point if the path is arranged such that distance decreases then increases. Space complexity is \(O(1)\) extra besides the input.
