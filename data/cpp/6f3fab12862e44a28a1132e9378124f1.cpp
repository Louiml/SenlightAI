// Write a C++ function named `avoid_obstacles` that takes a vector of 2D points (as `std::pair<double,double>`), a vector of elliptical obstacles described by center coordinates (h, k) and semi-axes (rx, ry) stored in a simple struct `Ellipse`, and returns a new vector of waypoints where any waypoint that lies strictly inside an obstacle ellipse (i.e., normalized squared distance < 1.0) is replaced by the nearest point on the horizontal line through that waypoint that is just outside the obstacle, specifically by checking integer multiples of a fixed step size (10.0) to the left first, then to the right, and choosing the first valid point (not inside any obstacle) in each direction; if both directions yield valid points, select the one that results in the shorter total path length from the previous accepted waypoint (with the starting waypoint always accepted as-is). The function must preserve the order of waypoints, operate on `const` references for inputs where possible, and handle the edge case where a waypoint is inside multiple obstacles by considering the union of all obstacles. The function should use Euclidean distance and assume `rx` and `ry` are positive (if `ry` < 0.5, clamp it to 0.5). Provide complete code with necessary includes and a `struct Ellipse` definition.

#include <cassert>
#include <cmath>

int main() {
    // Test 1: No obstacles, waypoints pass through
    std::vector<Ellipse> no_obs;
    std::vector<std::pair<double,double>> wps1 = {{0.0, 0.0}, {10.0, 10.0}, {20.0, 20.0}};
    auto res1 = avoid_obstacles(wps1, no_obs);
    assert(res1.size() == 3);
    assert(res1[0] == wps1[0]);
    assert(res1[1] == wps1[1]);
    assert(res1[2] == wps1[2]);

    // Test 2: Waypoint inside an ellipse, no previous offset issue
    std::vector<Ellipse> obs2 = {{10.0, 10.0, 5.0, 4.0}}; // center (10,10), rx=5, ry=4
    std::vector<std::pair<double,double>> wps2 = {{0.0, 0.0}, {10.0, 10.0}}; // second is exactly center, inside
    auto res2 = avoid_obstacles(wps2, obs2);
    assert(res2.size() == 2);
    // First waypoint accepted as-is
    assert(res2[0] == std::make_pair(0.0, 0.0));
    // The second must be replaced; check it's not inside obstacle
    assert(!is_collision(res2[1], obs2));
    // It should be on the same y=10 line, either left or right of center
    assert(std::fabs(res2[1].second - 10.0) < 1e-9);
    // The closest free point on the line y=10 at multiples of 10 from x=10: left gives x=0, right gives x=20
    // Distance from prev (0,0) to (0,10)=10, to (20,10)=sqrt(500)~22.36, so left chosen
    assert(std::fabs(res2[1].first - 0.0) < 1e-9);

    // Test 3: Two waypoints, second inside obstacle, previous point far on right
    std::vector<std::pair<double,double>> wps3 = {{30.0, 10.0}, {10.0, 10.0}};
    auto res3 = avoid_obstacles(wps3, obs2);
    assert(res3.size() == 2);
    assert(res3[0] == std::make_pair(30.0, 10.0));
    // Prev is (30,10), candidates (0,10) and (20,10); distance to (20,10)=10, to (0,10)=30, so right chosen
    assert(std::fabs(res3[1].first - 20.0) < 1e-9);
    assert(std::fabs(res3[1].second - 10.0) < 1e-9);

    // Test 4: Edge case ry < 0.5 should be clamped to 0.5
    std::vector<Ellipse> obs4 = {{0.0, 0.0, 10.0, 0.2}}; // very flat ellipse
    std::vector<std::pair<double,double>> wps4 = {{0.0, 0.0}, {1.0, 0.0}};
    auto res4 = avoid_obstacles(wps4, obs4);
    assert(res4.size() == 2);
    // First waypoint at center collides but accepted; second (1,0) is inside? calc=(1^2/100)+(0/0.25)=0.01 <1, so inside
    // Candidates: left from (1,0): x=-9? Actually step=10, k=1 gives x=-9, not inside? calc=81/100=0.81<1, so need k=2: x=-19, calc=361/100=3.61>1 free
    // Right: k=1 gives x=11, calc=121/100=1.21>1 free
    // Prev is (0,0), distances to (-19,0)=19, to (11,0)=11 -> right chosen
    assert(std::fabs(res4[1].first - 11.0) < 1e-9);

    // Test 5: Multiple obstacles, waypoint inside both (union)
    std::vector<Ellipse> obs5 = {{0.0, 0.0, 5.0, 5.0}, {3.0, 0.0, 5.0, 5.0}};
    std::vector<std::pair<double,double>> wps5 = {{100.0, 100.0}, {1.0, 0.0}};
    auto res5 = avoid_obstacles(wps5, obs5);
    assert(res5.size() == 2);
    assert(res5[0] == std::make_pair(100.0, 100.0));
    // (1,0) is inside both? For obs1: 1/25=0.04 <1; obs2: (1-3)^2/25=4/25=0.16<1 -> inside
    // Candidates: left from (1,0): k=1 gives -9, check obs1: 81/25=3.24>1, obs2: 144/25=5.76>1 free; right gives 11, also free
    // Distances from prev (100,100) to (-9,0) huge, to (11,0) also huge; left slightly closer? sqrt(109^2+100^2) vs sqrt(89^2+100^2) -> right is closer
    assert(std::fabs(res5[1].first - 11.0) < 1e-9);

    // Test 6: Empty input
    std::vector<std::pair<double,double>> wps6;
    auto res6 = avoid_obstacles(wps6, obs2);
    assert(res6.empty());

    return 0;
}

#include <vector>
#include <utility>
#include <cmath>
#include <limits>

struct Ellipse {
    double h;  // center x
    double k;  // center y
    double rx; // semi-axis x
    double ry; // semi-axis y
};

// Check if a point is inside any ellipse (strictly inside, calc < 1.0)
bool is_collision(const std::pair<double,double>& p, const std::vector<Ellipse>& obstacles) {
    double x = p.first;
    double y = p.second;
    for (const auto& obs : obstacles) {
        double ry = obs.ry;
        if (ry < 0.5) ry = 0.5; // clamp as specified
        double calc = (x - obs.h) * (x - obs.h) / (obs.rx * obs.rx) +
                      (y - obs.k) * (y - obs.k) / (ry * ry);
        if (calc < 1.0) return true;
    }
    return false;
}

// Find first free point along horizontal line from given point, direction sign (-1 left, +1 right)
std::pair<double,double> find_free_offset(const std::pair<double,double>& p,
                                          int direction,
                                          const std::vector<Ellipse>& obstacles,
                                          double step = 10.0) {
    int k = 1;
    while (true) {
        std::pair<double,double> candidate(p.first + direction * step * k, p.second);
        if (!is_collision(candidate, obstacles)) {
            return candidate;
        }
        ++k;
    }
}

// Main function: replace waypoints that collide with obstacles
std::vector<std::pair<double,double>> avoid_obstacles(
    const std::vector<std::pair<double,double>>& waypoints,
    const std::vector<Ellipse>& obstacles) {
    if (waypoints.empty()) return {};

    std::vector<std::pair<double,double>> result;
    // Always accept the first waypoint as-is
    result.push_back(waypoints[0]);

    for (size_t i = 1; i < waypoints.size(); ++i) {
        const auto& wp = waypoints[i];
        if (!is_collision(wp, obstacles)) {
            result.push_back(wp);
            continue;
        }

        // Find left and right free candidates
        auto left_candidate = find_free_offset(wp, -1, obstacles);
        auto right_candidate = find_free_offset(wp, +1, obstacles);

        // Choose the one closer to the previous accepted waypoint
        const auto& prev = result.back();
        double dist_left = std::hypot(left_candidate.first - prev.first,
                                       left_candidate.second - prev.second);
        double dist_right = std::hypot(right_candidate.first - prev.first,
                                        right_candidate.second - prev.second);

        if (dist_left <= dist_right) {
            result.push_back(left_candidate);
        } else {
            result.push_back(right_candidate);
        }
    }
    return result;
}

// The solution processes waypoints sequentially. For each waypoint in the input, first check if it lies inside any obstacle using the ellipse equation: `((x-h)^2 / rx^2) + ((y-k)^2 / ry^2) < 1.0`. If it is collision-free, accept it as is. If it collides, generate candidate replacement points along the horizontal line y = constant (same y as the original waypoint). For the left direction, start with offset = 10.0, then multiply by increasing integer k = 1,2,3,... and check if the point (x - 10*k, y) is collision-free. Stop at the first k where it's free. Do the same for the right direction (x + 10*k, y). If both directions yield at least one free point, choose the one that minimizes the total distance from the previously accepted waypoint (the last element of the output vector) to that candidate, plus the accumulated path distance so far. However, since the path distance is monotonic, we simply compare the direct Euclidean distances from the previous accepted point to each candidate and pick the smaller. If only one direction yields a free point, use that. If both directions fail (which should not happen for finite obstacles, but handle defensively), fall back to the original point. Additionally, we need to handle the starting waypoint specially: it is always accepted as the first element of the output, regardless of collision, because there is no previous point to compare. Complexity: For each waypoint, in the worst case we may scan many multiples of 10 to find a free point; but for well-separated obstacles this is typically O(1). Checking collision against all obstacles is O(M) where M is the number of obstacles. So overall time is O(N * M * L) where L is the average number of steps checked, and space is O(N) for the output vector (plus O(1) extra).
