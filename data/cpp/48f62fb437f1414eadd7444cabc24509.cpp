/*
Given a sequence of 2D waypoints represented as `std::vector<std::pair<double, double>>` (each pair is an (x, y) coordinate) and a current vehicle position as another `std::pair<double, double>`, write a C++ function that computes the index of the next waypoint to target based on a constant lookahead distance. Specifically, the function should return the index of the first waypoint whose Euclidean distance from the current position is greater than the given lookahead distance. If no such waypoint exists (i.e., the vehicle is already within the lookahead distance of the last waypoint), return the index of the last waypoint. If the waypoint list is empty, return -1. The function should also handle the edge case where the lookahead distance is non-positive by returning 0 (the first waypoint) if the list is non-empty.
*/

#include <vector>
#include <cmath>
#include <utility>

// Returns the index of the next waypoint to target based on a constant lookahead distance.
// If the list is empty, returns -1.
// Otherwise, returns the index of the first waypoint whose distance from the current position
// is strictly greater than lookahead_distance. If none qualifies, returns the index of the last waypoint.
// For non-positive lookahead_distance, returns 0 (or -1 if empty).
int findNextWaypoint(const std::vector<std::pair<double, double>>& waypoints,
                     const std::pair<double, double>& current_position,
                     double lookahead_distance) {
    if (waypoints.empty()) {
        return -1;
    }

    const double dx = current_position.first;
    const double dy = current_position.second;

    for (size_t i = 0; i < waypoints.size(); ++i) {
        const double wx = waypoints[i].first;
        const double wy = waypoints[i].second;
        const double dist = std::sqrt((wx - dx) * (wx - dx) + (wy - dy) * (wy - dy));
        if (dist > lookahead_distance) {
            return static_cast<int>(i);
        }
    }

    return static_cast<int>(waypoints.size() - 1);
}

#include <cassert>
#include <vector>
#include <utility>

// Function declaration from solution
int findNextWaypoint(const std::vector<std::pair<double, double>>& waypoints,
                     const std::pair<double, double>& current_position,
                     double lookahead_distance);

int main() {
    // Test 1: Basic case, first waypoint beyond lookahead
    std::vector<std::pair<double, double>> w1 = {{0.0, 0.0}, {3.0, 0.0}, {6.0, 0.0}};
    assert(findNextWaypoint(w1, {0.0, 0.0}, 2.0) == 1); // distance to waypoint 0 is 0, to 1 is 3 > 2

    // Test 2: All waypoints within lookahead, return last index
    assert(findNextWaypoint(w1, {5.0, 0.0}, 10.0) == 2); // distances: 5, 2, 1 all <= 10

    // Test 3: Empty list
    std::vector<std::pair<double, double>> w2;
    assert(findNextWaypoint(w2, {0.0, 0.0}, 1.0) == -1);

    // Test 4: Non-positive lookahead distance, returns first waypoint
    assert(findNextWaypoint(w1, {0.0, 0.0}, 0.0) == 1); // distance to waypoint 0 is 0 not > 0, to 1 is 3 > 0
    assert(findNextWaypoint(w1, {0.0, 0.0}, -5.0) == 1); // same logic

    // Test 5: Exactly equal to lookahead distance, not greater, so continue
    std::vector<std::pair<double, double>> w3 = {{2.0, 0.0}, {2.0, 1.0}};
    assert(findNextWaypoint(w3, {0.0, 0.0}, 2.0) == 1); // distance to waypoint 0 is 2 (not >), to 1 is sqrt(5) ≈ 2.236 > 2

    // Test 6: Single waypoint inside lookahead
    std::vector<std::pair<double, double>> w4 = {{1.0, 1.0}};
    assert(findNextWaypoint(w4, {0.0, 0.0}, 2.0) == 0); // distance sqrt(2) <= 2, return last (index 0)

    // Test 7: Single waypoint outside lookahead
    assert(findNextWaypoint(w4, {0.0, 0.0}, 1.0) == 0); // distance sqrt(2) > 1, return first (also index 0)

    return 0;
}

// The solution iterates through the vector of waypoints, computing the Euclidean distance between the current position and each waypoint using the standard distance formula `sqrt((x2-x1)^2 + (y2-y1)^2)`. For each waypoint index `i`, if the distance is strictly greater than the lookahead distance, the function immediately returns `i` because that is the first waypoint that lies outside the lookahead circle. If the loop finishes without finding such a waypoint, it means all waypoints are within the lookahead distance; in that case, the function returns the index of the last waypoint (`size - 1`). The special case of an empty vector returns -1. For a non-positive lookahead distance, the condition `distance > lookahead` will be true for the first waypoint (since distance is non-negative and > a non-positive threshold), so the function returns 0 appropriately. The time complexity is O(n) where n is the number of waypoints, because we scan the list once. The auxiliary space complexity is O(1) since we only store a few local variables (the distance and the current index).
