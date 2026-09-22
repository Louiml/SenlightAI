/*
Write a C++ function that takes a vector of pairs of integers (each pair represents a point with y and x coordinates) and returns a new vector sorted in descending order of the x-coordinate. If two points have the same x-coordinate, their relative order must remain unchanged (stable sort). The function should not modify the input vector, and it should handle an empty input vector gracefully by returning an empty vector. Use a custom comparator based on the `std::stable_sort` algorithm to ensure stability.
*/
#include <vector>
#include <algorithm>

struct Point {
    int y;
    int x;
};

// Sorts a copy of points in descending order of x-coordinate (stable).
std::vector<Point> sortByXDescending(const std::vector<Point>& points) {
    std::vector<Point> sorted = points; // Copy to avoid modifying input

    // Stable comparator: descending x, ties keep original order due to stable_sort
    auto compareByXDescending = [](const Point& a, const Point& b) {
        return a.x > b.x;
    };

    std::stable_sort(sorted.begin(), sorted.end(), compareByXDescending);

    return sorted;
}
#include <cassert>
#include <vector>
#include <iostream>

int main() {
    // Basic test with distinct x values
    std::vector<Point> input1 = {{0, 3}, {1, 1}, {2, 2}};
    std::vector<Point> result1 = sortByXDescending(input1);
    assert(result1[0].x == 3 && result1[1].x == 2 && result1[2].x == 1);
    assert(result1[0].y == 0 && result1[1].y == 2 && result1[2].y == 1);

    // Test stability with equal x values
    std::vector<Point> input2 = {{5, 2}, {7, 2}, {1, 2}, {3, 1}};
    std::vector<Point> result2 = sortByXDescending(input2);
    assert(result2[0].x == 2 && result2[1].x == 2 && result2[2].x == 2 && result2[3].x == 1);
    // Original order among x=2 points: y=5, y=7, y=1 must be preserved
    assert(result2[0].y == 5 && result2[1].y == 7 && result2[2].y == 1);

    // Test with negative x values
    std::vector<Point> input3 = {{1, -5}, {2, -1}, {3, -10}};
    std::vector<Point> result3 = sortByXDescending(input3);
    assert(result3[0].x == -1 && result3[1].x == -5 && result3[2].x == -10);

    // Test empty input
    std::vector<Point> input4;
    std::vector<Point> result4 = sortByXDescending(input4);
    assert(result4.empty());

    // Confirm input vectors are not modified
    assert(input1[0].x == 3 && input1[1].x == 1 && input1[2].x == 2);
    assert(input2[0].y == 5 && input2[1].y == 7 && input2[2].y == 1);

    // Test with a single point
    std::vector<Point> input5 = {{42, 99}};
    std::vector<Point> result5 = sortByXDescending(input5);
    assert(result5.size() == 1 && result5[0].x == 99 && result5[0].y == 42);

    return 0;
}
// The solution uses the `std::stable_sort` algorithm from the C++ standard library, which guarantees that equal elements retain their original relative order. We define a comparator that compares two `Point` structures by their `x` member in descending order (i.e., return `a.x > b.x`). Since `std::stable_sort` is required, we do not need to worry about preserving order for ties—it is handled automatically. The function should create a copy of the input vector to avoid modifying the caller's data, then sort the copy and return it. For an empty input, the copy is already empty, and sorting it is a no-op. Time complexity is O(n log n) for the sort, with O(n) auxiliary space for the copy. Space complexity is O(n) due to the returned vector. Edge cases include points with equal x (stable order), negative x values, and empty input.
