Write a C++ function named `totalManhattanDistance` that accepts as its parameters a positive integer `n` and a sequence of `n` pairs of integer coordinates (`x`, `y`), and returns the total Manhattan distance along the path that visits the points in the given order. The Manhattan distance between two points `(x1, y1)` and `(x2, y2)` is `|x1 - x2| + |y1 - y2|`. The function should read the coordinates from standard input in the following format: first an integer `n`, then `n` lines, each containing two integers `x` and `y` separated by a space. The function must return the total distance as an `int`. Assume `n >= 1` and all coordinates fit in the range of `int`.

#include <cassert>
#include <iostream>
#include <sstream>

// The solution function is declared above; we need to redirect stdin for tests.
// To keep this self-contained, we'll manually simulate input via a helper.
// But the task requires direct calls, so we re-implement a testable version.
// For the test, we'll copy the function but feed it via a stringstream.
int testableTotal(const std::string& input) {
    std::istringstream iss(input);
    int n;
    iss >> n;

    int prevX, prevY;
    iss >> prevX >> prevY;

    int res = 0;
    for (int i = 1; i < n; ++i) {
        int x, y;
        iss >> x >> y;
        res += std::abs(prevX - x) + std::abs(prevY - y);
        prevX = x; prevY = y;
    }
    return res;
}

int main() {
    assert(testableTotal("1\n0 0\n") == 0);
    assert(testableTotal("2\n0 0\n3 4\n") == 7);
    assert(testableTotal("3\n1 1\n4 5\n1 1\n") == 12);
    assert(testableTotal("2\n-1 -1\n2 2\n") == 6);
    assert(testableTotal("4\n10 10\n10 10\n10 10\n10 10\n") == 0);
    assert(testableTotal("5\n0 0\n1 0\n1 1\n0 1\n0 0\n") == 4);
    std::cout << "All tests passed!\n";
    return 0;
}

#include <cstdlib> // for std::abs

// Computes the total Manhattan distance along a path of n points.
// Reads from stdin: first an int n, then n pairs of integers (x y).
// Returns the sum of |x[i+1]-x[i]| + |y[i+1]-y[i]| over i = 0..n-2.
int totalManhattanDistance() {
    int n;
    std::cin >> n;

    int prevX, prevY;
    std::cin >> prevX >> prevY; // first point

    int totalDistance = 0;
    for (int i = 1; i < n; ++i) {
        int currX, currY;
        std::cin >> currX >> currY;
        totalDistance += std::abs(prevX - currX) + std::abs(prevY - currY);
        prevX = currX;
        prevY = currY;
    }
    return totalDistance;
}

// The solution requires summing the Manhattan distance between each consecutive pair of points. The algorithm reads the first point and stores its coordinates, then iterates over the remaining `n - 1` points, computing the absolute differences in x and y with the previous point, adding them to the total, and updating the previous point to the current one. The main edge case is when `n == 1`, where the loop body never executes, and the total remains `0`, which is correct because the path starts and ends at the same point (only one point, no travel). Another edge case is having points that repeat or have negative coordinates; the absolute value handles negatives properly. Time complexity is `O(n)` because we process each point exactly once, and space complexity is `O(1)` because we only store two points at a time (previous and current) and the running total.
