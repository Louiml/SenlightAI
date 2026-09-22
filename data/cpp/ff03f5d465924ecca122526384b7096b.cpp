Write a standalone C++ function that simulates the core logic of the given graphics code but without any GUI dependencies. The function should take integer coordinates for two circle centers and a shared radius, then compute and return a string that represents a simplified "frame" of the scene: for each circle, compute and output its symmetric points (the 8-fold symmetry points as in the `plot` function) in a deterministic, sorted order, and for the line, output the rasterized line points (Bresenham-like) sorted by x then y. The output string should list all points from both circles and the line, each formatted as `(x,y)`, separated by spaces, with circles colored "C" and line points colored "L". The function signature: `std::string generateScene(int xc1, int yc1, int xc2, int yc2, int radius)`. All inputs are positive integers, radius > 0, and the centers are distinct. The line should be drawn from the first center to the second center using the given algorithm (assume slope is gentle, i.e., |dx| >= |dy|, and x2 > x1). If the line segment is invalid (x2 <= x1), return an empty string.
#include <cassert>
#include <string>
#include <sstream>
#include <set>

// Helper to count occurrences of a substring
int countOccurrences(const std::string& haystack, const std::string& needle) {
    int count = 0;
    size_t pos = 0;
    while ((pos = haystack.find(needle, pos)) != std::string::npos) {
        count++;
        pos += needle.length();
    }
    return count;
}

int main() {
    // Test basic circle with radius 1 at (0,0), line from (0,0) to (2,0)
    std::string scene1 = generateScene(0, 0, 2, 0, 1);
    // Expect circle points: 8 points for (0,1) and 8 for (1,0) but duplicate (0,1) and (1,0) overlap? Actually (0,1),(1,0),( -1,0),(0,-1) and symmetric duplicates - total 8*2=16? Let's just verify some key points exist
    assert(countOccurrences(scene1, "(0,0)L") >= 1); // line passes through origin? Actually line from (0,0) to (2,0) includes (0,0),(1,0),(2,0)
    assert(countOccurrences(scene1, "(1,0)L") >= 1);
    assert(countOccurrences(scene1, "(2,0)L") >= 1);
    // Circle at (0,0) radius 1: points (0,1),(1,0),( -1,0),(0,-1) and their symmetric counterparts
    assert(countOccurrences(scene1, "(0,1)C") >= 1);
    assert(countOccurrences(scene1, "(1,0)C") >= 1);
    assert(countOccurrences(scene1, "(-1,0)C") >= 1);
    assert(countOccurrences(scene1, "(0,-1)C") >= 1);

    // Test invalid line direction
    assert(generateScene(5, 0, 3, 0, 2) == "");

    // Test line with slope 1 (gentle because dx=2, dy=2, but dx>=dy)
    // Centers (0,0) and (2,2), radius 1
    std::string scene2 = generateScene(0, 0, 2, 2, 1);
    assert(countOccurrences(scene2, "(0,0)L") >= 1);
    assert(countOccurrences(scene2, "(1,1)L") >= 1);
    assert(countOccurrences(scene2, "(2,2)L") >= 1);

    // Test symmetry: For a circle radius 2, the eight symmetric points should appear equal times
    // Use a circle at (10,10) radius 2, plus line to (12,10) (dx=2, dy=0)
    std::string scene3 = generateScene(10, 10, 12, 10, 2);
    // Point (12,10) should appear as C (from circle symmetric? Actually (10+2,10+0) is (12,10) from circle) and also as L
    assert(countOccurrences(scene3, "(12,10)C") >= 1);
    assert(countOccurrences(scene3, "(12,10)L") >= 1);

    // Check that output is sorted: extract x,y from first and last tokens
    // First token should have smallest x, last should have largest x
    std::string scene4 = generateScene(0, 0, 5, 0, 1);
    size_t firstSpace = scene4.find(' ');
    std::string firstToken = scene4.substr(0, firstSpace);
    std::string lastToken = scene4.substr(scene4.rfind(' ') + 1);
    // Parse first and last x values
    int firstX = std::stoi(firstToken.substr(1, firstToken.find(',') - 1));
    int lastX = std::stoi(lastToken.substr(1, lastToken.find(',') - 1));
    assert(firstX <= lastX);

    // Test radius 0 should produce only center point? But radius>0 per spec, but we can test radius=1 with repeated centers? Not needed.

    // Test many points: no crash and reasonable size
    std::string scene5 = generateScene(0, 0, 10, 0, 5);
    // For each circle, roughly 8*radius = 8*5=40 points each, but some duplicates due to symmetry? Actually for radius 5, x from 0 to 5 inclusive, each gives 8 points but some overlap at axes, so unique points are about 4*radius? Not critical; just check it's non-empty
    assert(!scene5.empty());

    return 0;
}
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <cmath>

// Generate a textual representation of two circles and a line between their centers.
// For each circle, compute the 8-fold symmetric boundary points using the midpoint circle algorithm.
// For the line, use Bresenham's algorithm for gentle slopes (|dx| >= |dy|) and x2 > x1.
// Returns a string of space-separated tokens like "(x,y)C" for circle points and "(x,y)L" for line points.
// If the line is not from left to right (x2 <= x1), returns an empty string.
std::string generateScene(int xc1, int yc1, int xc2, int yc2, int radius) {
    // Validate line direction
    if (xc2 <= xc1) {
        return "";
    }

    std::vector<std::pair<std::pair<int,int>, char>> points;

    // Helper lambda to add a circle point with the given offset
    auto addCirclePoint = [&points](int x, int y, int xc, int yc) {
        points.push_back({{xc + x, yc + y}, 'C'});
        points.push_back({{xc + y, yc + x}, 'C'});
        points.push_back({{xc - x, yc + y}, 'C'});
        points.push_back({{xc - y, yc + x}, 'C'});
        points.push_back({{xc + x, yc - y}, 'C'});
        points.push_back({{xc + y, yc - x}, 'C'});
        points.push_back({{xc - x, yc - y}, 'C'});
        points.push_back({{xc - y, yc - x}, 'C'});
    };

    // Generate points for both circles using midpoint algorithm (x <= y)
    for (int centerIdx = 0; centerIdx < 2; ++centerIdx) {
        int xc = (centerIdx == 0) ? xc1 : xc2;
        int yc = (centerIdx == 0) ? yc1 : yc2;
        int x = 0;
        int y = radius;
        int p = 1 - radius;

        while (x <= y) {
            addCirclePoint(x, y, xc, yc);
            if (p < 0) {
                x++;
                p = p + 2 * x + 1;
            } else {
                x++;
                y--;
                p = p + 2 * (x - y) + 1;
            }
        }
    }

    // Generate line points using a simplified Bresenham for gentle slope
    int dx = xc2 - xc1;
    int dy = yc2 - yc1;
    int x = xc1;
    int y = yc1;
    int s = 2 * dy - dx; // using integer arithmetic as in original code

    while (x <= xc2) {
        points.push_back({{x, y}, 'L'});
        if (x == xc2) break;
        if (s < 0) {
            x++;
            s = s + 2 * dy;
        } else {
            x++;
            y++;
            s = s + 2 * (dy - dx);
        }
    }

    // Sort points lexicographically by x, then y, then color
    std::sort(points.begin(), points.end(), [](const auto& a, const auto& b) {
        if (a.first.first != b.first.first) return a.first.first < b.first.first;
        if (a.first.second != b.first.second) return a.first.second < b.first.second;
        return a.second < b.second;
    });

    // Build output string
    std::ostringstream oss;
    for (size_t i = 0; i < points.size(); ++i) {
        if (i > 0) oss << " ";
        oss << "(" << points[i].first.first << "," << points[i].first.second << ")" << points[i].second;
    }
    return oss.str();
}
// The task extracts the geometric output from the graphics code into a purely computational problem. For each circle, we generate all 8 symmetric points for each x from 0 to y (inclusive) where (x,y) lies on the circle using the midpoint circle algorithm, but we must include the starting point (0, radius) and iterate until x <= y, not just x < y, to capture all points. The `plot` function produces coordinates relative to the center; we add the center coordinates. For the line, we implement Bresenham's algorithm for the gentle slope (|dx| >= |dy|) with the condition x2 > x1; if dx is negative or zero, we return empty. We collect all points into a vector of pairs, tagging each with a color identifier 'C' for circle points and 'L' for line points. Then we sort the vector lexicographically by (x, y, color) to ensure deterministic output. We need to handle duplicate points (e.g., if circles overlap or line passes through circle points) – we keep duplicates since the spec doesn't say to remove them. Finally, format each point as `(x,y)` and append the color as a suffix like `(x,y)C` for circles and `(x,y)L` for lines, separated by spaces. Complexity: circle generation is O(radius) per circle, line is O(|dx|) where |dx| is horizontal distance. Sorting all points takes O(m log m) where m is total points (about 8*radius*2 + line length). Space is O(m). Edge cases: radius=1 produces few points; centers such that circles overlap produce duplicated coordinate pairs; line slope exactly 1 works with the given algorithm.
