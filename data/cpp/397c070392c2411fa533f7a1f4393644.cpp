Write a standalone C++ function named `mergeBeaconSets` that takes two `std::vector<Point>` arguments, each representing a set of 3D beacon coordinates (where `Point` is a struct with `int x, y, z` and equality operators), and returns a single `std::vector<Point>` containing the union of both sets, with duplicate points removed. The function must preserve the relative order of first appearance: all points from the first set appear first (in their original order, skipping duplicates already inserted), then all points from the second set that are not already present appear next (in their original order). Use `std::find` for membership checking. The function must be `const`-correct and return by value. Assume `Point` is defined in a separate header or above the function.

The approach is to iterate over each input vector and insert points into a result vector only if they are not already present. For the first set, we iterate from index 0 to end, and for each point, check if it already exists in the result vector using `std::find`. If not found, we push it back. Then we do the same for the second set. This ensures that duplicates are removed globally (across both sets) and that the order of first appearance is maintained: first occurrences from set A, then new occurrences from set B. The algorithm is straightforward. Edge cases include: empty input vectors (which yield an empty result), identical points across both sets (only first occurrence retained), and duplicate points within a single set (only the first occurrence retained). Time complexity is \(O(n^2)\) in the worst case because `std::find` on a vector is linear, and we perform it for each of the \(n\) points in the union; more precisely, for each point we search the growing result vector, giving \(O(|A|^2 + |B| \cdot |A| + |B|^2)\) in worst case, but typically we denote it as \(O(k^2)\) where \(k = |A|+|B|\). Space complexity is \(O(k)\) for the result vector, plus the internal storage of the input vectors (which are passed by const reference, so no extra copy).

#include <vector>
#include <algorithm>

// Assume Point is defined with public int x, y, z and operator==.
struct Point {
    int x, y, z;
    bool operator==(const Point& other) const {
        return x == other.x && y == other.y && z == other.z;
    }
};

// Return the union of two beacon sets, preserving first-occurrence order.
std::vector<Point> mergeBeaconSets(const std::vector<Point>& a, const std::vector<Point>& b) {
    std::vector<Point> result;
    result.reserve(a.size() + b.size());

    // Add points from first set, skipping duplicates.
    for (const auto& p : a) {
        if (std::find(result.begin(), result.end(), p) == result.end()) {
            result.push_back(p);
        }
    }

    // Add points from second set that are not already present.
    for (const auto& p : b) {
        if (std::find(result.begin(), result.end(), p) == result.end()) {
            result.push_back(p);
        }
    }

    return result;
}

#include <cassert>
#include <vector>

int main() {
    // Test case 1: disjoint sets
    std::vector<Point> a1 = {{0,0,0}, {1,1,1}};
    std::vector<Point> b1 = {{2,2,2}, {3,3,3}};
    auto r1 = mergeBeaconSets(a1, b1);
    assert(r1.size() == 4);
    assert(r1[0] == Point{0,0,0});
    assert(r1[1] == Point{1,1,1});
    assert(r1[2] == Point{2,2,2});
    assert(r1[3] == Point{3,3,3});

    // Test case 2: overlapping sets, duplicates removed
    std::vector<Point> a2 = {{0,0,0}, {1,1,1}, {2,2,2}};
    std::vector<Point> b2 = {{1,1,1}, {2,2,2}, {3,3,3}};
    auto r2 = mergeBeaconSets(a2, b2);
    assert(r2.size() == 4);
    assert(r2[0] == Point{0,0,0});
    assert(r2[1] == Point{1,1,1});
    assert(r2[2] == Point{2,2,2});
    assert(r2[3] == Point{3,3,3});

    // Test case 3: identical sets
    std::vector<Point> a3 = {{1,1,1}};
    std::vector<Point> b3 = {{1,1,1}};
    auto r3 = mergeBeaconSets(a3, b3);
    assert(r3.size() == 1);
    assert(r3[0] == Point{1,1,1});

    // Test case 4: empty first set
    std::vector<Point> a4;
    std::vector<Point> b4 = {{5,5,5}};
    auto r4 = mergeBeaconSets(a4, b4);
    assert(r4.size() == 1);
    assert(r4[0] == Point{5,5,5});

    // Test case 5: empty both
    std::vector<Point> a5;
    std::vector<Point> b5;
    auto r5 = mergeBeaconSets(a5, b5);
    assert(r5.empty());

    // Test case 6: duplicate within first set only
    std::vector<Point> a6 = {{1,1,1}, {1,1,1}};
    std::vector<Point> b6 = {{2,2,2}};
    auto r6 = mergeBeaconSets(a6, b6);
    assert(r6.size() == 2);
    assert(r6[0] == Point{1,1,1});
    assert(r6[1] == Point{2,2,2});

    // Test case 7: second set has duplicates of first and internal duplicates
    std::vector<Point> a7 = {{0,0,0}};
    std::vector<Point> b7 = {{0,0,0}, {0,0,0}, {1,2,3}};
    auto r7 = mergeBeaconSets(a7, b7);
    assert(r7.size() == 2);
    assert(r7[0] == Point{0,0,0});
    assert(r7[1] == Point{1,2,3});

    return 0;
}
