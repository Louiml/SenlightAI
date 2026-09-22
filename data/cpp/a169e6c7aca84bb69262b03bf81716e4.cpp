Write a C++ function `sortPointsByCoordinates` that takes an array of 2D integer points (each point has an `x` and a `y` coordinate) and its length `n`, sorts the points in ascending order first by the `x` coordinate, and if two points have the same `x`, then by the `y` coordinate, and returns the sorted points in a new dynamically allocated array. The function must handle edge cases such as `n = 0` (return a null pointer or an empty array) and duplicate points. You may assume all coordinates are within the range of a 32-bit signed integer. Do not modify the input array. The function signature should be: `Point* sortPointsByCoordinates(const Point* points, int n);` where `Point` is a struct with two `int` members `x` and `y`. Provide a self-contained implementation with proper memory management, and ensure the returned array is allocated with `new` (the caller is responsible for `delete[]`).
// The solution involves creating a copy of the input array and then applying the standard `std::sort` algorithm with a custom comparator that orders by `x` first and only if `x` values are equal, orders by `y`. The comparator must be a free function or a lambda that takes two `Point` objects by value or const reference. Important edge cases: (1) `n = 0` — return a null pointer or an empty array (here we return `nullptr` for simplicity, but the test must handle it); (2) duplicate points — the sort must be stable, but `std::sort` is not guaranteed stable; however, since duplicates are identical, their relative order doesn't matter, so no issue. (3) Large `n` up to 100,000 — `std::sort` runs in \(O(n \log n)\) time on average and uses \(O(\log n)\) stack space for recursion, plus we need \(O(n)\) extra space for the copy. The input array is not modified, and we return a newly allocated array of size `n`. If `n = 0`, we return `nullptr`. The implementation must include `#include <algorithm>` and `#include <cstddef>` (or `<cstdlib>` for `nullptr`). The function should be `const`-correct: the input pointer is `const Point*`, and the function does not modify the input.
#include <algorithm>
#include <cstddef>

struct Point {
    int x;
    int y;
};

// Sort an array of points by x, then by y.
// Returns a newly allocated sorted array of size n; caller must delete[].
// If n == 0, returns nullptr.
Point* sortPointsByCoordinates(const Point* points, int n) {
    if (n <= 0) {
        return nullptr;
    }

    // Make a copy of the input array to sort
    Point* sorted = new Point[n];
    for (int i = 0; i < n; ++i) {
        sorted[i] = points[i];
    }

    // Custom comparator: sort by x, then by y when x equal
    std::sort(sorted, sorted + n, [](const Point& a, const Point& b) {
        if (a.x != b.x) {
            return a.x < b.x;
        }
        return a.y < b.y;
    });

    return sorted;
}
#include <cassert>
#include <cstddef>

// Assume struct Point and the function are defined above

int main() {
    // Test 1: Basic sorting
    Point p1[] = {{3, 2}, {1, 5}, {2, 1}, {1, 3}};
    Point* result1 = sortPointsByCoordinates(p1, 4);
    assert(result1[0].x == 1 && result1[0].y == 3);
    assert(result1[1].x == 1 && result1[1].y == 5);
    assert(result1[2].x == 2 && result1[2].y == 1);
    assert(result1[3].x == 3 && result1[3].y == 2);
    delete[] result1;

    // Test 2: Single element
    Point p2[] = {{42, -7}};
    Point* result2 = sortPointsByCoordinates(p2, 1);
    assert(result2[0].x == 42 && result2[0].y == -7);
    delete[] result2;

    // Test 3: Duplicates and negative coordinates
    Point p3[] = {{0, 0}, {-1, 2}, {-1, 1}, {0, 0}, {-1, 2}};
    Point* result3 = sortPointsByCoordinates(p3, 5);
    assert(result3[0].x == -1 && result3[0].y == 1);
    assert(result3[1].x == -1 && result3[1].y == 2);
    assert(result3[2].x == -1 && result3[2].y == 2);
    assert(result3[3].x == 0 && result3[3].y == 0);
    assert(result3[4].x == 0 && result3[4].y == 0);
    delete[] result3;

    // Test 4: Already sorted
    Point p4[] = {{1, 1}, {2, 0}, {2, 3}};
    Point* result4 = sortPointsByCoordinates(p4, 3);
    assert(result4[0].x == 1 && result4[0].y == 1);
    assert(result4[1].x == 2 && result4[1].y == 0);
    assert(result4[2].x == 2 && result4[2].y == 3);
    delete[] result4;

    // Test 5: Empty input
    Point* result5 = sortPointsByCoordinates(nullptr, 0);
    assert(result5 == nullptr); // no allocation, nullptr returned

    // Test 6: Large n (just to ensure no crash) - but for assert, check a few
    const int N = 1000;
    Point* large = new Point[N];
    for (int i = 0; i < N; ++i) {
        large[i].x = (i * 37) % 50;
        large[i].y = (i * 19) % 50;
    }
    Point* sortedLarge = sortPointsByCoordinates(large, N);
    for (int i = 1; i < N; ++i) {
        assert(!(sortedLarge[i].x < sortedLarge[i-1].x));
        if (sortedLarge[i].x == sortedLarge[i-1].x) {
            assert(sortedLarge[i].y >= sortedLarge[i-1].y);
        }
    }
    delete[] large;
    delete[] sortedLarge;

    // Test 7: Input array is not modified
    Point original[] = {{5, 1}, {3, 4}, {5, 0}};
    Point copyBefore[] = {{5, 1}, {3, 4}, {5, 0}};
    Point* result7 = sortPointsByCoordinates(original, 3);
    for (int i = 0; i < 3; ++i) {
        assert(original[i].x == copyBefore[i].x && original[i].y == copyBefore[i].y);
    }
    assert(result7[0].x == 3 && result7[0].y == 4);
    assert(result7[1].x == 5 && result7[1].y == 0);
    assert(result7[2].x == 5 && result7[2].y == 1);
    delete[] result7;

    return 0;
}
