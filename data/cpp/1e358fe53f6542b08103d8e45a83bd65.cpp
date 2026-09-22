In the context of navigation mesh generation for video game AI, obstacles (such as fences or low walls) between two adjacent nav areas must be traversable. You are tasked with implementing a C++ function that analyzes a simplified one-dimensional model of such obstacles. Given a vector of `Segment` structs, each representing a horizontal line segment with a start coordinate, an end coordinate, and a height above some baseline, and given a threshold `stepHeight`, write a function `countTraversableObstacles` that returns the number of segments whose height is strictly greater than `stepHeight` (i.e., obstacles that a character cannot step over). For each such segment, the function must also compute and store the minimum and maximum height among all traversable obstacles in a separately provided output vector. If a segment's height is exactly equal to `stepHeight` or lower, it is considered traversable and is not counted. The input vector may be empty, in which case the function returns 0 and leaves the output vector empty. The segments are not necessarily sorted, and each segment has a unique `id` field. The output should be a vector of pairs (id, height) for every non-traversable segment, sorted by ascending id.
// The solution is straightforward: iterate over each segment in the input vector once. For each segment, compare its height against the threshold `stepHeight`. If the height is strictly greater than `stepHeight`, increment a counter and append a pair containing the segment's id and height to a result vector. After processing all segments, sort the result vector by id in ascending order (since segments are not guaranteed to be sorted). Edge cases include an empty input vector (returns 0 and empty output), segments with height exactly equal to the threshold (not counted), and the fact that each segment has a unique id so sorting avoids ambiguity. The time complexity is O(n log n) due to the sorting step, where n is the number of segments, and the auxiliary space complexity is O(k) for the output vector, where k is the number of non-traversable segments (at most n). If the input were guaranteed sorted by id, the sort could be omitted, but for a robust standalone function, sorting ensures correctness regardless of input order.
#include <vector>
#include <algorithm>
#include <utility>

// Represents a horizontal line segment with id, start, end, and height.
struct Segment {
    int id;
    double start;
    double end;
    double height;
};

// Counts segments with height > stepHeight and returns their ids and heights sorted by id.
int countTraversableObstacles(const std::vector<Segment>& segments, double stepHeight, std::vector<std::pair<int, double>>& nonTraversable) {
    // Clear any pre-existing data in the output vector.
    nonTraversable.clear();
    
    // First pass: collect all segments that exceed the step height.
    std::vector<std::pair<int, double>> temp;
    for (const auto& seg : segments) {
        if (seg.height > stepHeight) {
            temp.emplace_back(seg.id, seg.height);
        }
    }
    
    // Sort by id in ascending order.
    std::sort(temp.begin(), temp.end(), [](const auto& lhs, const auto& rhs) {
        return lhs.first < rhs.first;
    });
    
    // Move the sorted result into the output vector.
    nonTraversable = std::move(temp);
    return static_cast<int>(nonTraversable.size());
}
#include <cassert>
#include <vector>
#include <utility>

// Segment struct and solution function declaration (assumed available).
// (Include the struct and function from the Solution section here for test compilation.)

int main() {
    // Test 1: Empty input.
    std::vector<Segment> empty;
    std::vector<std::pair<int, double>> result;
    assert(countTraversableObstacles(empty, 1.0, result) == 0);
    assert(result.empty());

    // Test 2: All traversable (height less than or equal to threshold).
    std::vector<Segment> flat = {{1, 0.0, 5.0, 1.0}, {2, 5.0, 10.0, 0.5}, {3, 10.0, 15.0, 1.0}};
    assert(countTraversableObstacles(flat, 1.0, result) == 0);
    assert(result.empty());

    // Test 3: All non-traversable, unsorted input.
    std::vector<Segment> high = {{4, 0.0, 2.0, 2.0}, {1, 2.0, 4.0, 3.0}, {3, 4.0, 6.0, 2.5}};
    assert(countTraversableObstacles(high, 1.0, result) == 3);
    assert(result.size() == 3);
    assert(result[0] == std::make_pair(1, 3.0));
    assert(result[1] == std::make_pair(3, 2.5));
    assert(result[2] == std::make_pair(4, 2.0));

    // Test 4: Mixed heights, threshold boundary.
    std::vector<Segment> mixed = {{2, 0.0, 1.0, 2.0}, {1, 1.0, 2.0, 1.0}, {3, 2.0, 3.0, 1.5}};
    assert(countTraversableObstacles(mixed, 1.0, result) == 2);
    assert(result.size() == 2);
    assert(result[0] == std::make_pair(2, 2.0));
    assert(result[1] == std::make_pair(3, 1.5));

    // Test 5: Duplicate heights and ids are unique.
    std::vector<Segment> dup = {{5, 0.0, 1.0, 3.0}, {6, 1.0, 2.0, 3.0}, {7, 2.0, 3.0, 2.0}};
    assert(countTraversableObstacles(dup, 2.0, result) == 2);
    assert(result[0] == std::make_pair(5, 3.0));
    assert(result[1] == std::make_pair(6, 3.0));

    return 0;
}
