/*
Write a C++ function `string findExtremeIds(const vector<string>& ids, const vector<double>& xCoords, const vector<double>& yCoords)` that takes two equal-length vectors: one of student IDs (each a non-empty string of at most 4 characters) and two vectors of their corresponding x and y coordinates. The function must return a single string containing the ID of the student who is nearest to the origin (0,0) followed by a space and then the ID of the student who is farthest from the origin. If two students are equally close/equally far, keep the first one encountered in the input order. The distance is Euclidean, computed as `sqrt(x*x + y*y)`. Assume the input vectors are non‑empty and of equal length. The returned string must have exactly one space between the two IDs (format: `"minID maxID"`). You must not modify the input vectors.
*/

#include <string>
#include <vector>
#include <limits>

// Returns "minID maxID" where minID is the ID of the point closest to the origin,
// and maxID is the ID of the point farthest. Ties keep the first occurrence.
std::string findExtremeIds(const std::vector<std::string>& ids,
                           const std::vector<double>& xCoords,
                           const std::vector<double>& yCoords) {
    const std::size_t n = ids.size();
    double minDistSq = std::numeric_limits<double>::max();
    double maxDistSq = 0.0;
    std::string minId, maxId;

    for (std::size_t i = 0; i < n; ++i) {
        double distSq = xCoords[i] * xCoords[i] + yCoords[i] * yCoords[i];

        if (distSq < minDistSq) {
            minDistSq = distSq;
            minId = ids[i];
        }

        if (distSq > maxDistSq) {
            maxDistSq = distSq;
            maxId = ids[i];
        }
    }

    return minId + " " + maxId;
}

#include <cassert>
#include <string>
#include <vector>

// free function declaration (implementation above)
std::string findExtremeIds(const std::vector<std::string>& ids,
                           const std::vector<double>& xCoords,
                           const std::vector<double>& yCoords);

int main() {
    // Basic case: distinct points
    std::vector<std::string> ids1 = {"A", "B", "C"};
    std::vector<double> x1 = {1.0, -2.0, 0.0};
    std::vector<double> y1 = {1.0, 3.0, -1.0};
    assert(findExtremeIds(ids1, x1, y1) == "C B"); // C dist sqrt(1), B dist sqrt(13)

    // Tie for closest: first occurrence kept
    std::vector<std::string> ids2 = {"X", "Y", "Z"};
    std::vector<double> x2 = {1.0, -1.0, 5.0};
    std::vector<double> y2 = {0.0, 0.0, 0.0};
    assert(findExtremeIds(ids2, x2, y2) == "X Z"); // X and Y both dist 1, X first

    // Tie for farthest: first occurrence kept
    std::vector<std::string> ids3 = {"P", "Q", "R"};
    std::vector<double> x3 = {0.0, 3.0, -3.0};
    std::vector<double> y3 = {0.0, 4.0, -4.0};
    assert(findExtremeIds(ids3, x3, y3) == "P Q"); // P dist 0, Q and R dist 5, Q first

    // Single element
    std::vector<std::string> ids4 = {"Only"};
    std::vector<double> x4 = {2.5};
    std::vector<double> y4 = {3.5};
    assert(findExtremeIds(ids4, x4, y4) == "Only Only");

    // All same distance
    std::vector<std::string> ids5 = {"A", "B", "C"};
    std::vector<double> x5 = {1.0, -1.0, 0.0};
    std::vector<double> y5 = {1.0, -1.0, 1.41421356237};
    // distances all ~1.414, so min and max are both first element
    assert(findExtremeIds(ids5, x5, y5) == "A A");

    // Negative and zero coordinates
    std::vector<std::string> ids6 = {"n1", "z", "p"};
    std::vector<double> x6 = {-3.0, 0.0, 2.0};
    std::vector<double> y6 = {4.0, 0.0, 2.0};
    // n1 dist 5, z dist 0, p dist sqrt(8)~2.828
    assert(findExtremeIds(ids6, x6, y6) == "z n1");

    return 0;
}

// The solution iterates once through all indices, computing the squared distance (or actual distance) from the origin for each point. To avoid floating-point precision issues when comparing equal distances, it can compare `x*x + y*y` directly since the square root is a monotonic function. Initialize `minDistSq` to a very large number (e.g., `DBL_MAX` or `numeric_limits<double>::max()`) and `maxDistSq` to a very small number (e.g., `0` or negative). For each index `i`, compute `distSq = xCoords[i]*xCoords[i] + yCoords[i]*yCoords[i]`. If `distSq < minDistSq`, update `minDistSq` and `minId`; if `distSq > maxDistSq`, update `maxDistSq` and `maxId`. Using strict `<` and `>` ensures that ties keep the first occurrence. The function returns `minId + " " + maxId`. Time complexity is O(n) where n is the number of students. Space complexity is O(1) auxiliary, not counting input vectors or the returned string.
