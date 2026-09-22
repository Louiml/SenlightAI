// Write a C++ function `int frequencyOfPoint(const std::vector<std::pair<int, int>>& points, int x, int y)` that counts how many times the point `(x, y)` appears in a vector of 2D integer points. The input vector may contain duplicate points, and the function must handle negative coordinates and large coordinate values safely. Use an `unordered_map` with a custom hash and equality operator for a `point` struct (defined as `x` and `y` fields) to achieve efficient counting. The function should return the count as an integer. Do not modify the input vector.
The solution creates a custom `point` struct with `x` and `y` as `int` members, and defines both an equality operator and a hash function (e.g., `hash = x * 37 + y`). The main algorithm iterates through the vector of pairs, constructs a `point` for each, and increments the count in an `unordered_map<point, int, hasher>`. After populating the map, it looks up the target `(x, y)` and returns the stored count, or 0 if the key is absent. Edge cases: empty vector (returns 0), negative coordinates (hash and equality must handle signed ints correctly), and duplicate points (each occurrence increments the count). Time complexity is O(N) for building the map plus O(1) average for lookup, so total O(N). Space complexity is O(N) in the worst case for storing all unique points.
#include <vector>
#include <unordered_map>

struct point {
    int x, y;

    bool operator==(const point& other) const {
        return x == other.x && y == other.y;
    }
};

struct PointHasher {
    size_t operator()(const point& p) const {
        // A simple but effective hash; handles negative coordinates fine.
        return static_cast<size_t>(p.x * 37 + p.y);
    }
};

// Count occurrences of the point (x, y) in the given vector of points.
// The vector is passed by const reference to avoid copying and ensure no modification.
int frequencyOfPoint(const std::vector<std::pair<int, int>>& points, int x, int y) {
    std::unordered_map<point, int, PointHasher> freq;

    // Populate the map with counts.
    for (const auto& pair : points) {
        point p{pair.first, pair.second};
        freq[p]++;
    }

    // Lookup the target point; if not found, operator[] would insert it, so use find.
    auto it = freq.find(point{x, y});
    if (it != freq.end()) {
        return it->second;
    }
    return 0;
}
#include <cassert>
#include <vector>
#include <utility>

int main() {
    // Basic test with duplicates
    std::vector<std::pair<int, int>> points = {{1,2},{3,4},{1,2},{5,6},{1,2}};
    assert(frequencyOfPoint(points, 1, 2) == 3);
    assert(frequencyOfPoint(points, 3, 4) == 1);
    assert(frequencyOfPoint(points, 7, 8) == 0);

    // Empty vector
    std::vector<std::pair<int, int>> empty;
    assert(frequencyOfPoint(empty, 0, 0) == 0);

    // Negative coordinates
    std::vector<std::pair<int, int>> neg = {{-1,-2},{-1,-2},{0,0},{-1,-2}};
    assert(frequencyOfPoint(neg, -1, -2) == 3);
    assert(frequencyOfPoint(neg, 0, 0) == 1);

    // Large coordinate values
    std::vector<std::pair<int, int>> large = {{100000,200000},{100000,200000},{ -100000,-200000}};
    assert(frequencyOfPoint(large, 100000, 200000) == 2);
    assert(frequencyOfPoint(large, -100000, -200000) == 1);

    // Single element
    std::vector<std::pair<int, int>> single = {{4,5}};
    assert(frequencyOfPoint(single, 4, 5) == 1);
    assert(frequencyOfPoint(single, 4, 6) == 0);

    // All same point
    std::vector<std::pair<int, int>> allSame = {{0,0},{0,0},{0,0}};
    assert(frequencyOfPoint(allSame, 0, 0) == 3);

    return 0;
}
