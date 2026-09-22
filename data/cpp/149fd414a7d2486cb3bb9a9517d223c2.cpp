Write a C++ function `analyzeAdaptiveFaceScalarStatistics` that takes a vector of pairs, where each pair contains a numeric grid node location (as a `double`) and a scalar face value (as a `double`), along with a cell width `dx` (a positive `double`). The function should return a `std::tuple<double, double, double, double>` containing, in this order: the average of all face values, the absolute maximum of all face values, the computed divergence approximation across each adjacent pair of faces (i.e., for consecutive nodes in the input vector, compute `(value_right - value_left) / dx` and take the maximum absolute divergence among all such adjacent pairs), and the estimated center velocity of the first cell (i.e., the average of the first two face values). The input vector is guaranteed to have at least two elements, representing face locations and values in increasing order of location. If the vector length is odd, ignore the last element for the average and divergence calculations. For divergence, only consider pairs where the index of the left face is even (0-based), simulating face pairs of cells in an adaptive binary tree. Return `NaN` (std::nan("")) for the center velocity if there are not at least two valid elements for it.
#include <cassert>
#include <cmath>
#include <tuple>
#include <vector>

int main() {
    // 1. Basic case with 4 faces (even count)
    std::vector<std::pair<double,double>> faces1 = {{0.0, 1.0}, {1.0, 2.0}, {2.0, 1.5}, {3.0, 0.5}};
    auto r1 = analyzeAdaptiveFaceScalarStatistics(faces1, 1.0);
    assert(std::get<0>(r1) == 1.25);            // (1+2+1.5+0.5)/4
    assert(std::get<1>(r1) == 2.0);             // max abs
    assert(std::get<2>(r1) == 1.0);             // max(|(2-1)/1|, |(0.5-1.5)/1|) = 1
    assert(std::get<3>(r1) == 1.5);             // (1+2)/2

    // 2. Odd count: ignore last
    std::vector<std::pair<double,double>> faces2 = {{0.0, 3.0}, {1.0, 7.0}, {2.0, 5.0}, {3.0, -1.0}, {4.0, 100.0}};
    auto r2 = analyzeAdaptiveFaceScalarStatistics(faces2, 0.5);
    assert(std::get<0>(r2) == 3.5);             // (3+7+5-1)/4
    assert(std::get<1>(r2) == 7.0);             // max abs among valid
    assert(std::get<2>(r2) == 8.0);             // max(|(7-3)/0.5|, |(-1-5)/0.5|) = 8
    assert(std::get<3>(r2) == 5.0);             // (3+7)/2

    // 3. Exactly two faces
    std::vector<std::pair<double,double>> faces3 = {{1.0, 4.0}, {2.0, -2.0}};
    auto r3 = analyzeAdaptiveFaceScalarStatistics(faces3, 2.0);
    assert(std::get<0>(r3) == 1.0);             // (4-2)/2
    assert(std::get<1>(r3) == 4.0);
    assert(std::get<2>(r3) == 3.0);             // |(-2-4)/2|
    assert(std::get<3>(r3) == 1.0);             // (4-2)/2

    // 4. Negative values and zero divergence
    std::vector<std::pair<double,double>> faces4 = {{0.0, -5.0}, {1.0, -5.0}, {2.0, 3.0}, {3.0, 3.0}};
    auto r4 = analyzeAdaptiveFaceScalarStatistics(faces4, 1.0);
    assert(std::get<0>(r4) == -1.0);            // (-5-5+3+3)/4
    assert(std::get<1>(r4) == 5.0);
    assert(std::get<2>(r4) == 0.0);             // both pairs have zero change
    assert(std::get<3>(r4) == -5.0);

    // 5. Invalid dx
    auto r5 = analyzeAdaptiveFaceScalarStatistics({{0.0,1.0},{1.0,2.0}}, 0.0);
    assert(std::isnan(std::get<0>(r5)));
    assert(std::isnan(std::get<1>(r5)));
    assert(std::isnan(std::get<2>(r5)));
    assert(std::isnan(std::get<3>(r5)));

    // 6. Even count but only 2 after ignoring? Not possible; but check n=3 (odd -> valid count=2)
    // Already tested in case 2.

    // 7. Large values
    std::vector<std::pair<double,double>> faces7 = {{0.0, 1e6}, {1.0, -1e6}, {2.0, 1e6}, {3.0, -1e6}};
    auto r7 = analyzeAdaptiveFaceScalarStatistics(faces7, 1e6);
    assert(std::get<0>(r7) == 0.0);
    assert(std::get<1>(r7) == 1e6);
    assert(std::get<2>(r7) == 2.0);  // |(-1e6-1e6)/1e6| = 2
    assert(std::get<3>(r7) == 0.0);
}
#include <tuple>
#include <vector>
#include <cmath>
#include <algorithm>
#include <limits>

/**
 * Analyze a sequence of face scalar values in an adaptive binary tree.
 *
 * @param faces   Vector of pairs (location, value). Locations are strictly increasing.
 * @param dx      Cell width (positive).
 * @return Tuple (average_abs, max_abs, max_divergence_abs, center_velocity_first_cell)
 */
std::tuple<double, double, double, double> analyzeAdaptiveFaceScalarStatistics(
    const std::vector<std::pair<double, double>>& faces,
    double dx)
{
    const std::size_t n = faces.size();
    // Deduplicate to invalid input
    if (n < 2 || dx <= 0.0) {
        return {std::nan(""), std::nan(""), std::nan(""), std::nan("")};
    }

    // Determine number of valid faces (ignore last if odd)
    const std::size_t valid_count = (n % 2 == 0) ? n : n - 1;
    if (valid_count < 2) {
        return {std::nan(""), std::nan(""), std::nan(""), std::nan("")};
    }

    // Average and max absolute over valid faces
    double sum = 0.0;
    double max_abs = 0.0;
    for (std::size_t i = 0; i < valid_count; ++i) {
        double val = faces[i].second;
        sum += val;
        max_abs = std::max(max_abs, std::abs(val));
    }
    double average = sum / static_cast<double>(valid_count);

    // Max divergence over pairs (even index left, odd index right)
    double max_divergence = 0.0;
    for (std::size_t i = 0; i + 1 < valid_count; i += 2) {
        double left  = faces[i].second;
        double right = faces[i + 1].second;
        double div = (right - left) / dx;
        max_divergence = std::max(max_divergence, std::abs(div));
    }

    // Center velocity of first cell
    double center_velocity = 0.5 * (faces[0].second + faces[1].second);

    return {average, max_abs, max_divergence, center_velocity};
}
// The solution iterates once over the vector to compute a running sum and maximum absolute value for all valid face values (ignoring the last element if the size is odd). Separately, for each even index `i` from 0 to size-2 (stepping by 2), we compute the divergence using the pair `(values[i], values[i+1])` and track the maximum absolute divergence. The center velocity is simply the average of the first two elements (indices 0 and 1) if they exist. Edge cases: if the vector has fewer than 2 elements, return `nan` for center velocity; if vector length is exactly 2, both average and divergence use those two elements. Complexity is O(n) time and O(1) auxiliary space. No allocation beyond the returned tuple.
