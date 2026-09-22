// Write a C++ function `double signedAreaMagnitude(const std::vector<std::pair<double,double>>& vertices)` that takes a vector of 2D points (each represented as a `std::pair<double,double>` where first is x and second is y) defining a simple polygon in either clockwise or counterclockwise order, and returns the area of the polygon as a non-negative double. The polygon may be non-convex and may contain collinear points. Assume the vector contains at least three points and that the polygon is closed (the last point is not necessarily equal to the first, and the function should treat the polygon as closed by connecting the last point back to the first). The function must correctly handle large coordinate values and should not have any numerical overflow issues beyond standard `double` precision. The area should be computed using the shoelace formula.

The core algorithm is the shoelace formula (also known as Gauss's area formula). For vertices \((x_0,y_0), (x_1,y_1), \ldots, (x_{n-1},y_{n-1})\), the signed area is \(\frac{1}{2}\sum_{i=0}^{n-1} (x_i \cdot y_{i+1} - y_i \cdot x_{i+1})\), where index \(i+1\) wraps around to 0 when \(i = n-1\). The result of this sum is positive for counterclockwise orientation and negative for clockwise orientation, so taking the absolute value gives the actual area. Edge cases include collinear vertices (which contribute zero to the sum, so they are harmless) and polygons with many vertices requiring a wide range of sums; using `double` for accumulation is standard. The time complexity is \(O(n)\) because we scan each vertex exactly once. The space complexity is \(O(1)\) auxiliary, aside from the input vector which we do not copy.

#include <vector>
#include <utility>
#include <cmath>
#include <cstddef>

// Compute the area of a simple polygon given by vertices in order.
// Each vertex is a pair (x, y). The polygon is implicitly closed by the last
// vertex connecting back to the first. The function returns a non-negative area.
double polygonArea(const std::vector<std::pair<double,double>>& vertices) {
    if (vertices.size() < 3) {
        return 0.0;
    }
    double sum = 0.0;
    const std::size_t n = vertices.size();
    for (std::size_t i = 0; i < n; ++i) {
        const std::size_t j = (i + 1) % n;
        sum += vertices[i].first * vertices[j].second
             - vertices[i].second * vertices[j].first;
    }
    return std::fabs(sum) / 2.0;
}

#include <cassert>
#include <cmath>
#include <vector>
#include <utility>

// Include or declare the polygonArea function here

int main() {
    // Square with area 1 (counterclockwise)
    std::vector<std::pair<double,double>> square = {{0.0,0.0},{1.0,0.0},{1.0,1.0},{0.0,1.0}};
    assert(std::fabs(polygonArea(square) - 1.0) < 1e-9);

    // Same square but clockwise ordering should give same area
    std::vector<std::pair<double,double>> squareCW = {{0.0,0.0},{0.0,1.0},{1.0,1.0},{1.0,0.0}};
    assert(std::fabs(polygonArea(squareCW) - 1.0) < 1e-9);

    // Triangle with area 0.5
    std::vector<std::pair<double,double>> tri = {{0.0,0.0},{2.0,0.0},{0.0,1.0}};
    assert(std::fabs(polygonArea(tri) - 1.0) < 1e-9);

    // Rectangle 2x3 area 6
    std::vector<std::pair<double,double>> rect = {{0.0,0.0},{2.0,0.0},{2.0,3.0},{0.0,3.0}};
    assert(std::fabs(polygonArea(rect) - 6.0) < 1e-9);

    // Non-convex "L" shape with area 3 (area of 2x2 square minus 1x1 cutout)
    std::vector<std::pair<double,double>> Lshape = {{0.0,0.0},{2.0,0.0},{2.0,1.0},{1.0,1.0},{1.0,2.0},{0.0,2.0}};
    assert(std::fabs(polygonArea(Lshape) - 3.0) < 1e-9);

    // Collinear points (degenerate triangle) area 0
    std::vector<std::pair<double,double>> line = {{0.0,0.0},{1.0,1.0},{2.0,2.0}};
    assert(std::fabs(polygonArea(line) - 0.0) < 1e-9);

    // Large coordinates
    std::vector<std::pair<double,double>> big = {{1e9,0.0},{2e9,0.0},{2e9,1e9},{1e9,1e9}};
    assert(std::fabs(polygonArea(big) - 1e18) < 1e9); // 1e18 area, allow tolerance

    // Zero-area sliver (identical points) 
    std::vector<std::pair<double,double>> zero = {{0.0,0.0},{0.0,0.0},{0.0,0.0}};
    assert(std::fabs(polygonArea(zero) - 0.0) < 1e-12);

    return 0;
}
