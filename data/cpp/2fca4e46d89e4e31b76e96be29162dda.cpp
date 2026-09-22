// Given a vector of 2D points represented by pairs of `double` coordinates, write a C++ function that returns a new vector containing only those points that lie inside or on the boundary of a specified axis-aligned rectangular window. The window is defined by its lower-left corner `(x_min, y_min)` and upper-right corner `(x_max, y_max)`, inclusive on all boundaries. The input vector is not modified, and the output vector preserves the original order of the selected points. Assume the window is valid (i.e., `x_min <= x_max` and `y_min <= y_max`), and coordinates are finite. Do not rely on any external libraries; implement a straightforward scan. The function must be named `filterPointsInWindow`, and it must be `const`-correct by taking the input as `const std::vector<std::pair<double, double>>&` and returning a vector by value.
#include <cassert>
#include <vector>
#include <utility>

// The solution function (declared above) is assumed to be available.

int main() {
    using Point = std::pair<double, double>;

    // Empty input
    std::vector<Point> empty;
    assert(filterPointsInWindow(empty, 0.0, 0.0, 10.0, 10.0).empty());

    // All points inside
    std::vector<Point> pts = {{1.0, 2.0}, {3.0, 4.0}, {5.0, 6.0}};
    auto all = filterPointsInWindow(pts, 0.0, 0.0, 7.0, 7.0);
    assert(all == pts);

    // Boundary inclusive
    std::vector<Point> boundary = {{0.0, 0.0}, {10.0, 10.0}, {10.0, 0.0}, {0.0, 10.0}};
    auto bound = filterPointsInWindow(boundary, 0.0, 0.0, 10.0, 10.0);
    assert(bound.size() == 4);

    // Mixed points, preserve order
    std::vector<Point> mixed = {{10.0, 10.0}, {1.0, 1.0}, {20.0, 2.0}, {1.0, 9.0}, {2.0, 2.0}};
    auto filtered = filterPointsInWindow(mixed, 0.0, 0.0, 5.0, 5.0);
    std::vector<Point> expected = {{1.0, 1.0}, {2.0, 2.0}};
    assert(filtered == expected);

    // Negative coordinates
    std::vector<Point> neg = {{-3.0, -2.0}, {-1.0, -1.0}, {0.0, 0.0}, {1.0, 1.0}};
    auto negFiltered = filterPointsInWindow(neg, -2.0, -2.0, 0.0, 0.0);
    std::vector<Point> negExpected = {{-1.0, -1.0}, {0.0, 0.0}};
    assert(negFiltered == negExpected);

    // Duplicates
    std::vector<Point> dup = {{2.0, 2.0}, {2.0, 2.0}, {9.0, 9.0}};
    auto dupFiltered = filterPointsInWindow(dup, 0.0, 0.0, 3.0, 3.0);
    assert(dupFiltered.size() == 2);

    return 0;
}
#include <vector>
#include <utility>

// Returns a vector of points from the input that lie inside or on the boundary
// of the axis-aligned rectangle defined by (x_min, y_min) - (x_max, y_max).
std::vector<std::pair<double, double>> filterPointsInWindow(
    const std::vector<std::pair<double, double>>& points,
    double x_min, double y_min, double x_max, double y_max)
{
    std::vector<std::pair<double, double>> result;
    for (const auto& p : points) {
        double x = p.first;
        double y = p.second;
        if (x >= x_min && x <= x_max && y >= y_min && y <= y_max) {
            result.push_back(p);
        }
    }
    return result;
}
// The solution is a simple linear scan over all input points. For each point `(x, y)`, we check whether it satisfies all four inequalities: `x >= x_min`, `x <= x_max`, `y >= y_min`, and `y <= y_max`. If all conditions hold, the point is appended to the result vector. Because boundaries are inclusive, we use `>=` and `<=` comparisons. Edge cases include empty input (returns an empty vector), points exactly on the boundary (must be included), and duplicate points (all duplicates that satisfy the condition are included). The algorithm runs in `O(n)` time, where `n` is the number of input points, and uses `O(k)` additional space for the output, where `k` is the number of selected points. No sorting or preprocessing is needed, and the order of points is preserved naturally by scanning left-to-right.
