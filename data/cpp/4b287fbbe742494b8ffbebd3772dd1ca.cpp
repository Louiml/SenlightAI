Write a standalone C++ function named `discreteFrechetDistance` that takes two vectors of 2D points (each point represented as a `vector<double>` with exactly two coordinates) and computes the discrete Fréchet distance between the two polygonal curves. The function must implement the classic dynamic programming algorithm: define `L[i][j]` as the minimum over all couplings of the first `i+1` points of curve1 and the first `j+1` points of curve2, where `L[i][j]` is the maximum of the Euclidean distance between point `i` of curve1 and point `j` of curve2 and the minimum of `L[i-1][j]`, `L[i][j-1]`, and `L[i-1][j-1]`. Base cases: `L[0][0]` equals the Euclidean distance between the first points; for the first row and first column, use the maximum of the current distance and the predecessor's value. The function must be `const`-correct, take the input vectors by `const` reference, and return a `double`. It must handle curves of any positive length, including curves with a single point. Use `std::sqrt`, `std::max`, `std::min`, and a 2D `std::vector<double>` for the DP table to avoid stack overflow for large inputs.
// The discrete Fréchet distance measures the similarity between two polygonal curves by considering a monotone coupling (a sequence of pairs of indices that starts at (0,0) and ends at (n-1,m-1), moving only right, down, or diagonal). The DP recurrence is derived from the “dog leash” interpretation: at each step, the leash length is the maximum of the current point-pair distance and the minimal leash length needed to reach the current pair from a valid predecessor. The algorithm initializes `L[0][0]` as the Euclidean distance between the first points. For the first row (`i=0`, `j>0`), the only predecessor is `L[0][j-1]`, so `L[0][j] = max(distance(0,j), L[0][j-1])`. Similarly for the first column (`j=0`, `i>0`). For interior cells, the value is `max(distance(i,j), min(L[i-1][j], L[i][j-1], L[i-1][j-1]))`. The final answer is `L[n-1][m-1]`. Edge cases include curves of length 1 (then the answer is simply the distance between those two points), and identical or overlapping curves (the distance will be the maximum deviation). The time complexity is `O(n·m)` because each cell is computed once, and space complexity is `O(n·m)` for the DP table. The algorithm assumes both vectors are non-empty and each inner vector has exactly two coordinates; no validation is needed per the standard problem definition.
#include <vector>
#include <cmath>
#include <algorithm>

// Compute the Euclidean distance between two 2D points.
double pointDistance(const std::vector<double>& p1, const std::vector<double>& p2) {
    double dx = p1[0] - p2[0];
    double dy = p1[1] - p2[1];
    return std::sqrt(dx * dx + dy * dy);
}

// Compute the discrete Fréchet distance between two polygonal curves.
// Each curve is a vector of 2D points, each point a vector<double> of length 2.
double discreteFrechetDistance(const std::vector<std::vector<double>>& curve1,
                               const std::vector<std::vector<double>>& curve2) {
    const int n = static_cast<int>(curve1.size());
    const int m = static_cast<int>(curve2.size());

    // DP table: L[i][j] stores the minimal leash length up to point i of curve1 and j of curve2.
    std::vector<std::vector<double>> L(n, std::vector<double>(m, 0.0));

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            double dist = pointDistance(curve1[i], curve2[j]);

            if (i == 0 && j == 0) {
                L[i][j] = dist;
            } else if (i == 0) {
                L[i][j] = std::max(dist, L[i][j - 1]);
            } else if (j == 0) {
                L[i][j] = std::max(dist, L[i - 1][j]);
            } else {
                double min_prev = std::min(std::min(L[i - 1][j], L[i][j - 1]), L[i - 1][j - 1]);
                L[i][j] = std::max(dist, min_prev);
            }
        }
    }

    return L[n - 1][m - 1];
}
#include <cassert>
#include <cmath>
#include <vector>

// Function declaration (implementation above)
double discreteFrechetDistance(const std::vector<std::vector<double>>& curve1,
                               const std::vector<std::vector<double>>& curve2);

int main() {
    // Test 1: Identical single-point curves -> distance 0
    std::vector<std::vector<double>> c1a = {{0.0, 0.0}};
    std::vector<std::vector<double>> c2a = {{0.0, 0.0}};
    assert(std::fabs(discreteFrechetDistance(c1a, c2a) - 0.0) < 1e-9);

    // Test 2: Single points at distance 5
    std::vector<std::vector<double>> c1b = {{0.0, 0.0}};
    std::vector<std::vector<double>> c2b = {{3.0, 4.0}};
    assert(std::fabs(discreteFrechetDistance(c1b, c2b) - 5.0) < 1e-9);

    // Test 3: Two identical straight lines from (0,0) to (1,1) -> distance 0
    std::vector<std::vector<double>> c1c = {{0.0, 0.0}, {1.0, 1.0}};
    std::vector<std::vector<double>> c2c = {{0.0, 0.0}, {1.0, 1.0}};
    assert(std::fabs(discreteFrechetDistance(c1c, c2c) - 0.0) < 1e-9);

    // Test 4: Parallel lines offset by 2 in y-direction -> distance 2
    std::vector<std::vector<double>> c1d = {{0.0, 0.0}, {1.0, 0.0}};
    std::vector<std::vector<double>> c2d = {{0.0, 2.0}, {1.0, 2.0}};
    assert(std::fabs(discreteFrechetDistance(c1d, c2d) - 2.0) < 1e-9);

    // Test 5: Known example from literature, curves of length 3 and 4
    // Curve1: (0,0), (1,1), (2,0) ; Curve2: (0,0), (1,0), (2,0), (3,0)
    // Minimal leash required: max(0, distance(1,1)-(1,0)=1, distance(2,0)-(2,0)=0, distance(2,0)-(3,0)=1) -> 1
    std::vector<std::vector<double>> c1e = {{0.0, 0.0}, {1.0, 1.0}, {2.0, 0.0}};
    std::vector<std::vector<double>> c2e = {{0.0, 0.0}, {1.0, 0.0}, {2.0, 0.0}, {3.0, 0.0}};
    assert(std::fabs(discreteFrechetDistance(c1e, c2e) - 1.0) < 1e-9);

    // Test 6: Symmetric case (swap arguments) should give same result
    assert(std::fabs(discreteFrechetDistance(c2e, c1e) - 1.0) < 1e-9);

    // Test 7: Large separation in a corner, check that max of all distances is not enough
    // Curve1: (0,0) and (100,0) ; Curve2: (50,1) and (51,1). The optimal coupling forces a big jump.
    std::vector<std::vector<double>> c1f = {{0.0, 0.0}, {100.0, 0.0}};
    std::vector<std::vector<double>> c2f = {{50.0, 1.0}, {51.0, 1.0}};
    // Possible couplings: (0,0)-(0,0) dist ~50.01, then (1,0)-(1,1) dist ~49.01; or diagonal etc.
    // The minimal maximum is about 50.01
    double result_f = discreteFrechetDistance(c1f, c2f);
    assert(result_f > 50.0 && result_f < 50.1);

    // Test 8: Empty curves not allowed by specification, but if both single point at same location, distance 0
    std::vector<std::vector<double>> c1g = {{7.0, 8.0}};
    std::vector<std::vector<double>> c2g = {{7.0, 8.0}};
    assert(std::fabs(discreteFrechetDistance(c1g, c2g) - 0.0) < 1e-9);

    return 0;
}
