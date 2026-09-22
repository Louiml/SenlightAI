Write a C++ function named `createClosingChain` that takes a vector of 2D points (using double-precision x and y coordinates) representing the vertices of a polygon and returns a new vector of points that forms a closed chain by explicitly appending the first vertex at the end of the list. The function must also validate the input: reject inputs with fewer than 3 vertices (return an empty vector if invalid) and reject inputs where any two consecutive vertices are too close together (specifically, if the squared distance between consecutive vertices is less than or equal to `1e-12`). If all checks pass, the returned vector should have length `count + 1` where the last element equals the first element. The function must be `const`-correct, take the input vector by `const std::vector<Point>&`, and not modify the input.
#include <cassert>
#include <vector>

// Assume the solution function is defined above (or include it here).

int main() {
    // Valid triangle
    std::vector<Point> tri = {{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}};
    auto closedTri = createClosingChain(tri);
    assert(closedTri.size() == 4);
    assert(closedTri[0].x == 0.0 && closedTri[0].y == 0.0);
    assert(closedTri[1].x == 1.0 && closedTri[1].y == 0.0);
    assert(closedTri[2].x == 0.0 && closedTri[2].y == 1.0);
    assert(closedTri[3].x == closedTri[0].x && closedTri[3].y == closedTri[0].y);

    // Valid square
    std::vector<Point> square = {{0.0, 0.0}, {2.0, 0.0}, {2.0, 2.0}, {0.0, 2.0}};
    auto closedSquare = createClosingChain(square);
    assert(closedSquare.size() == 5);
    assert(closedSquare[4].x == 0.0 && closedSquare[4].y == 0.0);

    // Too few vertices
    std::vector<Point> tooFew = {{0.0, 0.0}, {1.0, 1.0}};
    assert(createClosingChain(tooFew).empty());

    // Empty input
    std::vector<Point> empty;
    assert(createClosingChain(empty).empty());

    // Duplicate consecutive vertices
    std::vector<Point> dup = {{0.0, 0.0}, {0.0, 0.0}, {1.0, 1.0}};
    assert(createClosingChain(dup).empty());

    // Very close consecutive vertices (squared distance <= 1e-12)
    std::vector<Point> close = {{0.0, 0.0}, {1e-6, 0.0}, {1.0, 1.0}};
    assert(createClosingChain(close).empty());

    // Exactly at the threshold? Use distance = sqrt(1e-12) = 1e-6, squared = 1e-12 => should reject
    std::vector<Point> threshold = {{0.0, 0.0}, {1e-6, 0.0}, {2.0, 0.0}};
    assert(createClosingChain(threshold).empty());

    // Valid with negative coordinates
    std::vector<Point> neg = {{-5.0, -3.0}, {2.0, -1.0}, {4.0, 5.0}};
    auto closedNeg = createClosingChain(neg);
    assert(closedNeg.size() == 4);
    assert(closedNeg[3].x == -5.0 && closedNeg[3].y == -3.0);

    return 0;
}
#include <vector>
#include <cmath>

struct Point {
    double x;
    double y;
};

// Returns a closed chain by appending the first vertex to the end.
// Returns an empty vector if input has fewer than 3 vertices or if any
// two consecutive vertices are too close (squared distance <= 1e-12).
std::vector<Point> createClosingChain(const std::vector<Point>& vertices) {
    const std::size_t n = vertices.size();
    if (n < 3) {
        return {};
    }

    constexpr double kMinSquaredDist = 1e-12;
    for (std::size_t i = 0; i + 1 < n; ++i) {
        double dx = vertices[i + 1].x - vertices[i].x;
        double dy = vertices[i + 1].y - vertices[i].y;
        if (dx * dx + dy * dy <= kMinSquaredDist) {
            return {};
        }
    }

    std::vector<Point> closed;
    closed.reserve(n + 1);
    closed.insert(closed.end(), vertices.begin(), vertices.end());
    closed.push_back(vertices[0]);
    return closed;
}
// The solution is straightforward: first check that the input size is at least 3; if not, return an empty vector. Then iterate through consecutive pairs (including the last and first vertex for a closed chain? Actually no—the problem specifies checking consecutive vertices in the given order only, not wrapping around, because the chain is initially open. The closure is only for the output representation, not for validation.) For each pair `(i, i+1)` from 0 to n-2, compute the squared Euclidean distance `dx*dx + dy*dy` and if it is `<= 1e-12`, return an empty vector. After validation, create a new vector of size `n+1`, copy all original vertices, and set the last element to the first element. Time complexity is O(n) because we iterate through vertices once and copy them once. Space complexity is O(n) for the output vector (plus O(1) auxiliary). Edge cases: n=3 is the minimum valid size; if n=0,1,2 return empty; if two consecutive points are identical or extremely close, return empty. Also handle potential overflow in squared distance by using double arithmetic, which is fine for normal inputs.
