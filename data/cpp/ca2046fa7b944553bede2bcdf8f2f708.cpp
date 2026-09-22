// Write a C++ function that takes a vector of city coordinates (two parallel vectors of doubles for x and y), a vector of integers representing a tour (a permutation of city indices 0..n-1), and returns the total Euclidean tour length, assuming the tour starts at the first city in the given path, visits each subsequent city in order, and then returns from the last city back to the first. The function must be `const`-correct and handle the case where the path vector is empty by returning 0.0. Use the same distance formula as in the snippet: for each consecutive pair (including the wrap-around), compute `sqrt(dx*dx + dy*dy)` where `dx` is the difference in x-coordinates and `dy` is the difference in y-coordinates. Do not modify the input vectors; treat them as read-only.
The solution iterates over each index `i` in the path vector (size `n`). For each iteration, we compute the starting city as `path[i]` and the ending city as `path[(i+1) % n]` (using modulo to wrap the last index back to 0). We fetch the x and y coordinates for both cities from the input coordinate vectors, compute the differences, and accumulate the Euclidean distance using `std::hypot` or manual `sqrt`. Edge cases: if the path vector is empty, the loop never runs and the sum stays 0.0, which we return. We must ensure we do not access coordinates out of bounds, but the problem statement assumes valid indices; still, we take the path size as the loop bound. Time complexity is O(n) where n is the number of cities, and space complexity is O(1) besides the input storage.
#include <vector>
#include <cmath>

// Compute the total Euclidean tour length for a given path of city indices.
// The path is cyclic: it starts at path[0], goes through path[1]...path[n-1],
// then returns from path[n-1] back to path[0].
// If the path is empty, returns 0.0.
double compute_tour_length(const std::vector<double>& xs,
                           const std::vector<double>& ys,
                           const std::vector<int>& path) {
    double total = 0.0;
    const size_t n = path.size();
    if (n == 0) return total;

    for (size_t i = 0; i < n; ++i) {
        int start = path[i];
        int end = path[(i + 1) % n];
        double dx = xs[start] - xs[end];
        double dy = ys[start] - ys[end];
        total += std::sqrt(dx * dx + dy * dy);
    }
    return total;
}
#include <cassert>
#include <cmath>
#include <vector>

// Declaration of the solution function for this test file.
double compute_tour_length(const std::vector<double>& xs,
                           const std::vector<double>& ys,
                           const std::vector<int>& path);

int main() {
    // Test 1: Simple square tour (0,0) -> (1,0) -> (1,1) -> (0,1) -> back to (0,0)
    std::vector<double> xs = {0.0, 1.0, 1.0, 0.0};
    std::vector<double> ys = {0.0, 0.0, 1.0, 1.0};
    std::vector<int> path = {0, 1, 2, 3};
    double expected = 4.0; // 1 + 1 + 1 + 1
    assert(std::fabs(compute_tour_length(xs, ys, path) - expected) < 1e-9);

    // Test 2: Same square but different starting point and order
    path = {2, 3, 0, 1};
    assert(std::fabs(compute_tour_length(xs, ys, path) - expected) < 1e-9);

    // Test 3: Single city tour length is zero (from city to itself)
    path = {1};
    assert(std::fabs(compute_tour_length(xs, ys, path) - 0.0) < 1e-9);

    // Test 4: Empty path returns zero
    path = {};
    assert(std::fabs(compute_tour_length(xs, ys, path) - 0.0) < 1e-9);

    // Test 5: Two cities with known distance (3-4-5 triangle: distance 5)
    std::vector<double> xs2 = {0.0, 3.0};
    std::vector<double> ys2 = {0.0, 4.0};
    path = {0, 1};
    expected = 5.0 * 2; // go from 0 to 1 (5) and back from 1 to 0 (5)
    assert(std::fabs(compute_tour_length(xs2, ys2, path) - expected) < 1e-9);

    // Test 6: Negative coordinates and ordering
    std::vector<double> xs3 = {-2.0, 2.0, -2.0, 2.0};
    std::vector<double> ys3 = {-2.0, -2.0, 2.0, 2.0};
    path = {0, 1, 3, 2};
    expected = 4 + 4 + 4 + 4; // each horizontal/vertical segment length 4
    assert(std::fabs(compute_tour_length(xs3, ys3, path) - expected) < 1e-9);

    return 0;
}
