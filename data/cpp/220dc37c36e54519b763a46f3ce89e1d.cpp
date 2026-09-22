Write a C++ function `rectangleStats` that takes two positive integers representing the side lengths of a rectangle (width and height) and returns a string containing the area and the perimeter, separated by a single space, in that order. For example, given `3` and `4`, the function should return `"12 14"`. The input values are guaranteed to be positive integers within the range `1` to `100000`, so no overflow or negative-number handling is required. The function must not read from standard input or write to standard output; it should purely compute and return the result.

// The problem is straightforward arithmetic. The area of a rectangle is `width * height`, and the perimeter is `2 * (width + height)`. Since the inputs are positive and bounded by 100000, the maximum area is `100000 * 100000 = 10000000000` which fits in a 64-bit integer (`long long`), and the maximum perimeter is `2 * (100000 + 100000) = 400000`, also safely within `int`. To be robust, we can use `long long` for calculations. The function constructs the output string by first computing both values, then converting them to strings and concatenating with a space. Edge cases: minimal input `1,1` yields `"1 4"`; equal sides (square) still work; no other special cases exist because inputs are guaranteed positive. Time complexity is O(1), space complexity is O(1) for the returned string length (which is constant, at most a few characters).

#include <string>
#include <cstdint>

// Compute the area and perimeter of a rectangle.
// Returns a string formatted as "area perimeter".
std::string rectangleStats(int width, int height) {
    std::int64_t area = static_cast<std::int64_t>(width) * height;
    std::int64_t perimeter = 2LL * (width + height);
    return std::to_string(area) + " " + std::to_string(perimeter);
}

#include <cassert>
#include <string>

// Declaration of the function under test.
std::string rectangleStats(int width, int height);

int main() {
    assert(rectangleStats(3, 4) == "12 14");
    assert(rectangleStats(1, 1) == "1 4");
    assert(rectangleStats(5, 5) == "25 20");
    assert(rectangleStats(100000, 100000) == "10000000000 400000");
    assert(rectangleStats(1, 100000) == "100000 200002");
    assert(rectangleStats(2, 3) == "6 10");
    assert(rectangleStats(7, 8) == "56 30");
    return 0;
}
