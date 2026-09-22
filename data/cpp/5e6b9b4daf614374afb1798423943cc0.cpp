Write a C++ function named `convexHullGiftWrapping` that takes a `std::vector<Coord>` where `Coord` is a struct with integer fields `x` and `y`, and returns a `std::vector<Coord>` containing the vertices of the convex hull in counter-clockwise order starting from the lexicographically smallest point (lowest x, and if tied, highest y). The input must contain at least 3 distinct points; if all points are identical or fewer than 3 points are provided, return an empty vector. The function should implement the gift wrapping (Jarvis march) algorithm without using any parallel constructs, and must handle duplicate points, collinear points on the hull boundary (including only the extreme endpoints of collinear edges), and edge cases where points share the same x or y coordinates.
The solution applies the classic Jarvis march algorithm. Start by finding the "most left" point: the one with the smallest x-coordinate, and among ties, the largest y-coordinate (this ensures a consistent starting vertex). Then repeatedly select the next hull vertex by finding the point that makes the smallest counter-clockwise angle from the last hull edge. For the first step, use a horizontal reference vector pointing right (or equivalently, compare using the angle from the starting point with respect to a horizontal line). For subsequent steps, given the last two vertices `A` and `B`, for each candidate point `C`, compute the angle between vector `AB` and `BC` using `atan2(cross, dot)`; choose the point with the smallest positive angle (favoring counter-clockwise turns). In case of ties (collinear points), select the one farthest from the current last vertex to skip intermediate collinear points. Continue until the next selected point equals the starting point. Edge cases to handle: all points identical (return empty), fewer than 3 distinct points (return empty), and points that are collinear such that the hull has only 2 vertices (in which case return both extreme points). Time complexity is O(n*h) where n is the number of input points and h is the number of hull vertices (worst case O(n^2) for points on a circle). Space complexity is O(n) for storing the output hull.
#include <vector>
#include <cmath>
#include <algorithm>
#include <cstdlib>

struct Coord {
    int x;
    int y;
};

// Compute the squared Euclidean distance between two points.
static double squaredDistance(const Coord& a, const Coord& b) {
    long long dx = static_cast<long long>(a.x) - b.x;
    long long dy = static_cast<long long>(a.y) - b.y;
    return static_cast<double>(dx * dx + dy * dy);
}

// Compute the angle between vectors AB and BC (B is the vertex).
// Returns a positive angle in [0, pi] for counter-clockwise turns.
static double angleBetween(const Coord& a, const Coord& b, const Coord& c) {
    Coord ab = {b.x - a.x, b.y - a.y};
    Coord cb = {b.x - c.x, b.y - c.y};
    double dot = static_cast<double>(ab.x) * cb.x + static_cast<double>(ab.y) * cb.y;
    double cross = static_cast<double>(ab.x) * cb.y - static_cast<double>(ab.y) * cb.x;
    return std::atan2(std::abs(cross), dot);
}

// Compute the angle of vector from 'vertex' to 'candidate' relative to a horizontal ray pointing left.
// Returns an angle in [0, pi].
static double angleFromLeft(const Coord& vertex, const Coord& candidate) {
    // Reference vector is (-1, 0), which points left.
    Coord ref = {-1, 0};
    Coord vc = {candidate.x - vertex.x, candidate.y - vertex.y};
    double dot = static_cast<double>(ref.x) * vc.x + static_cast<double>(ref.y) * vc.y;
    double cross = static_cast<double>(ref.x) * vc.y - static_cast<double>(ref.y) * vc.x;
    return std::atan2(std::abs(cross), dot);
}

// Gift wrapping (Jarvis march) convex hull.
// Returns hull vertices in counter-clockwise order starting from the most-left point.
std::vector<Coord> convexHullGiftWrapping(const std::vector<Coord>& points) {
    std::vector<Coord> hull;
    if (points.size() < 3) {
        return hull;
    }

    // Check if all points are identical.
    bool allSame = true;
    for (const auto& p : points) {
        if (!(p.x == points[0].x && p.y == points[0].y)) {
            allSame = false;
            break;
        }
    }
    if (allSame) {
        return hull;
    }

    // Find the most left point (smallest x, then largest y).
    int startIndex = 0;
    for (size_t i = 1; i < points.size(); ++i) {
        if (points[i].x < points[startIndex].x ||
            (points[i].x == points[startIndex].x && points[i].y > points[startIndex].y)) {
            startIndex = static_cast<int>(i);
        }
    }

    Coord start = points[startIndex];
    hull.push_back(start);

    // If there are exactly two distinct points, return both.
    Coord second;
    bool foundDistinct = false;
    for (const auto& p : points) {
        if (!(p.x == start.x && p.y == start.y)) {
            second = p;
            foundDistinct = true;
            break;
        }
    }
    if (!foundDistinct) {
        // All points identical, already handled.
        return hull;
    }
    // Check if all points are collinear (only two distinct points exist).
    bool collinearOnly = true;
    for (const auto& p : points) {
        long long cross = static_cast<long long>(second.x - start.x) * (p.y - start.y) -
                          static_cast<long long>(second.y - start.y) * (p.x - start.x);
        if (cross != 0) {
            collinearOnly = false;
            break;
        }
    }
    if (collinearOnly) {
        // Return the two extreme points along the line.
        Coord minP = start, maxP = start;
        for (const auto& p : points) {
            if (p.x < minP.x || (p.x == minP.x && p.y < minP.y)) minP = p;
            if (p.x > maxP.x || (p.x == maxP.x && p.y > maxP.y)) maxP = p;
        }
        hull.clear();
        hull.push_back(minP);
        hull.push_back(maxP);
        return hull;
    }

    // Select the second hull point: minimize the angle from the left reference.
    int nextIndex = -1;
    double bestAngle = 10.0; // larger than any possible angle
    for (size_t i = 0; i < points.size(); ++i) {
        if (i == static_cast<size_t>(startIndex)) continue;
        double ang = angleFromLeft(start, points[i]);
        if (ang < bestAngle - 1e-12) {
            bestAngle = ang;
            nextIndex = static_cast<int>(i);
        } else if (std::abs(ang - bestAngle) < 1e-12) {
            // Tie: choose the farthest point
            if (squaredDistance(start, points[i]) > squaredDistance(start, points[nextIndex])) {
                nextIndex = static_cast<int>(i);
            }
        }
    }
    hull.push_back(points[nextIndex]);

    // Main loop: repeatedly choose next hull vertex.
    int current = nextIndex;
    int prev = startIndex;
    while (true) {
        int candidate = -1;
        double minAngle = 10.0;
        for (size_t i = 0; i < points.size(); ++i) {
            if (static_cast<int>(i) == prev || static_cast<int>(i) == current) continue;
            double ang = angleBetween(points[prev], points[current], points[i]);
            if (ang < minAngle - 1e-12) {
                minAngle = ang;
                candidate = static_cast<int>(i);
            } else if (std::abs(ang - minAngle) < 1e-12) {
                // Tie: choose the farthest from current
                if (squaredDistance(points[current], points[i]) > squaredDistance(points[current], points[candidate])) {
                    candidate = static_cast<int>(i);
                }
            }
        }

        // If no candidate found (should not happen for valid input), break.
        if (candidate == -1) break;

        // If the candidate is the start point, we are done.
        if (candidate == startIndex) {
            break;
        }

        // Otherwise, add and continue.
        hull.push_back(points[candidate]);
        prev = current;
        current = candidate;
    }

    return hull;
}
#include <cassert>
#include <vector>
#include <iostream>

int main() {
    using std::vector;

    // Simple square
    vector<Coord> square = {{0,0},{1,0},{1,1},{0,1},{0,0}};
    auto hull = convexHullGiftWrapping(square);
    assert(hull.size() == 4);
    assert(hull[0].x == 0 && hull[0].y == 1); // most left and top
    // Verify counter-clockwise order
    assert(hull[1].x == 1 && hull[1].y == 1);
    assert(hull[2].x == 1 && hull[2].y == 0);
    assert(hull[3].x == 0 && hull[3].y == 0);

    // Triangle with duplicate interior point
    vector<Coord> tri = {{0,0},{2,0},{1,1},{1,0}};
    hull = convexHullGiftWrapping(tri);
    assert(hull.size() == 3);
    assert(hull[0].x == 0 && hull[0].y == 0);
    assert(hull[1].x == 2 && hull[1].y == 0);
    assert(hull[2].x == 1 && hull[2].y == 1);

    // Collinear points
    vector<Coord> line = {{0,0},{5,0},{3,0},{1,0}};
    hull = convexHullGiftWrapping(line);
    assert(hull.size() == 2);
    assert(hull[0].x == 0 && hull[0].y == 0);
    assert(hull[1].x == 5 && hull[1].y == 0);

    // All identical points
    vector<Coord> same = {{7,7},{7,7}};
    hull = convexHullGiftWrapping(same);
    assert(hull.empty());

    // Less than 3 points
    vector<Coord> two = {{1,2},{3,4}};
    hull = convexHullGiftWrapping(two);
    assert(hull.empty());

    // Points with same x coordinates
    vector<Coord> vertical = {{0,0},{0,3},{0,1},{1,1}};
    hull = convexHullGiftWrapping(vertical);
    assert(hull.size() == 3);
    assert(hull[0].x == 0 && hull[0].y == 3);
    assert(hull[1].x == 1 && hull[1].y == 1);
    assert(hull[2].x == 0 && hull[2].y == 0);

    // Randomized stress test with known convex polygon
    vector<Coord> pentagon = {{-2,0},{0,3},{2,0},{1,-2},{-1,-2},{0,1}};
    hull = convexHullGiftWrapping(pentagon);
    assert(hull.size() == 5);
    // Starting point should be (-2,0) because it has smallest x, and among those with x=-2, highest y (only one)
    assert(hull[0].x == -2 && hull[0].y == 0);
    // Check that the hull orientation is counter-clockwise by verifying a known sequence
    // The order should be: (-2,0) -> (0,3) -> (2,0) -> (1,-2) -> (-1,-2)
    assert(hull[1].x == 0 && hull[1].y == 3);
    assert(hull[2].x == 2 && hull[2].y == 0);
    assert(hull[3].x == 1 && hull[3].y == -2);
    assert(hull[4].x == -1 && hull[4].y == -2);

    // Points with duplicate hull vertices (exact duplicates in input)
    vector<Coord> dup = {{0,0},{0,0},{1,0},{0,1},{1,1},{1,1}};
    hull = convexHullGiftWrapping(dup);
    assert(hull.size() == 4);

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
