// Write a C++ function named `computeUnionRect` that takes a vector of rectangles (each represented by four integers: x, y, width, height) and returns the smallest axis-aligned rectangle that contains all input rectangles. If the input vector is empty, return a zero-sized rectangle at (0,0). The function should handle rectangles that overlap, touch, or are disjoint, and correctly compute the bounding box using the same logic as the `resultRoi` function from the provided snippet. The input rectangles may have negative coordinates. The function signature should be: `cv::Rect computeUnionRect(const std::vector<cv::Rect>& rects)` — but since we are not using OpenCV, define your own simple `Rect` struct with `int x, y, width, height` and appropriate constructors. The function must be `const`-correct and not modify the input.

// To compute the union (bounding box) of rectangles, we need to find the minimum top-left corner and maximum bottom-right corner across all rectangles. Initialize the top-left to the largest possible integer values and the bottom-right to the smallest possible integer values. Iterate through each rectangle: update the minimum x and y from each rectangle’s top-left, and update the maximum x and y from each rectangle’s bottom-right (x+width, y+height). After processing all rectangles, if the vector was empty, return a default rectangle (0,0,0,0). Otherwise, construct the result rectangle with the computed top-left and the width/height being the difference between the max bottom-right and min top-left. Edge cases include negative coordinates (handled naturally by the min/max comparisons), rectangles that touch (bounding box has zero area in one dimension but still a valid rectangle), and overlapping rectangles (the union is simply the outer bounds). Time complexity is O(n) where n is the number of rectangles; space complexity is O(1) auxiliary.

#include <vector>
#include <algorithm>
#include <limits>

// Simple rectangle structure with top-left corner and dimensions.
struct Rect {
    int x, y, width, height;
    Rect() : x(0), y(0), width(0), height(0) {}
    Rect(int x_, int y_, int w_, int h_) : x(x_), y(y_), width(w_), height(h_) {}
};

// Computes the smallest axis-aligned rectangle that contains all input rectangles.
// Returns a zero-sized rectangle at (0,0) if the input vector is empty.
Rect computeUnionRect(const std::vector<Rect>& rects) {
    if (rects.empty()) {
        return Rect(0, 0, 0, 0);
    }

    int min_x = std::numeric_limits<int>::max();
    int min_y = std::numeric_limits<int>::max();
    int max_x = std::numeric_limits<int>::min();
    int max_y = std::numeric_limits<int>::min();

    for (const Rect& r : rects) {
        min_x = std::min(min_x, r.x);
        min_y = std::min(min_y, r.y);
        max_x = std::max(max_x, r.x + r.width);
        max_y = std::max(max_y, r.y + r.height);
    }

    return Rect(min_x, min_y, max_x - min_x, max_y - min_y);
}

#include <cassert>

int main() {
    // Single rectangle
    Rect r1(2, 3, 10, 20);
    Rect u1 = computeUnionRect({r1});
    assert(u1.x == 2 && u1.y == 3 && u1.width == 10 && u1.height == 20);

    // Overlapping rectangles
    Rect r2(0, 0, 5, 5);
    Rect r3(3, 3, 5, 5);
    Rect u2 = computeUnionRect({r2, r3});
    assert(u2.x == 0 && u2.y == 0 && u2.width == 8 && u2.height == 8);

    // Disjoint rectangles
    Rect r4(-5, -5, 2, 2);
    Rect r5(10, 10, 3, 3);
    Rect u3 = computeUnionRect({r4, r5});
    assert(u3.x == -5 && u3.y == -5 && u3.width == 18 && u3.height == 18);

    // Touching rectangles (edge-to-edge)
    Rect r6(0, 0, 4, 4);
    Rect r7(4, 0, 4, 4);
    Rect u4 = computeUnionRect({r6, r7});
    assert(u4.x == 0 && u4.y == 0 && u4.width == 8 && u4.height == 4);

    // Empty input
    Rect u5 = computeUnionRect({});
    assert(u5.x == 0 && u5.y == 0 && u5.width == 0 && u5.height == 0);

    // Negative coordinates
    Rect r8(-10, -10, 1, 1);
    Rect r9(-1, -1, 2, 2);
    Rect u6 = computeUnionRect({r8, r9});
    assert(u6.x == -10 && u6.y == -10 && u6.width == 11 && u6.height == 11);

    // Large number of rectangles
    std::vector<Rect> many;
    for (int i = 0; i < 1000; ++i) {
        many.emplace_back(i, i, 1, 1);
    }
    Rect u7 = computeUnionRect(many);
    assert(u7.x == 0 && u7.y == 0 && u7.width == 1000 && u7.height == 1000);

    // Rectangle with zero width/height (degenerate)
    Rect r10(5, 5, 0, 10);
    Rect u8 = computeUnionRect({r10});
    assert(u8.x == 5 && u8.y == 5 && u8.width == 0 && u8.height == 10);

    return 0;
}
