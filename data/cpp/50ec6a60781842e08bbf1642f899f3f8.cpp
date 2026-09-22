Write a C++ function `findClosestLine` that takes a collection of `Line` objects (`Lines`) and a reference position `pos` (of type `millimeter_t`), and returns a const iterator to the line whose `pos` field is closest in absolute distance to the given position. If the collection is empty, return `lines.end()`. The `Line` type must be a simple struct with at least an `id` (e.g., `uint32_t`) and a `pos` field of type `millimeter_t` (which is a floating-point alias). The function must handle duplicate distances by choosing the first encountered line (i.e., the one with the smallest index). The solution must be self-contained, not rely on any external libraries beyond standard C++ headers, and use proper `const` correctness for the input container and iterator.
#include <cassert>

int main() {
    // Test with empty collection
    Lines empty;
    assert(findClosestLine(empty, 0.0) == empty.end());

    // Test with a single line
    Lines one = {{1, 10.0}};
    assert(findClosestLine(one, 12.0) == one.begin());
    assert(findClosestLine(one, 10.0) == one.begin());

    // Test with multiple lines, typical case
    Lines lines = {{1, 0.0}, {2, 5.0}, {3, 10.0}, {4, 20.0}};
    auto it = findClosestLine(lines, 7.0);
    assert(it->id == 2); // distance to 5.0 is 2.0, to 10.0 is 3.0

    it = findClosestLine(lines, 12.0);
    assert(it->id == 3); // distance to 10.0 is 2.0, to 20.0 is 8.0

    // Test with equal distances, first one wins
    Lines equal = {{1, 0.0}, {2, 10.0}};
    it = findClosestLine(equal, 5.0); // both distances 5.0
    assert(it->id == 1);

    // Test with negative positions
    Lines neg = {{1, -10.0}, {2, -5.0}, {3, 0.0}};
    it = findClosestLine(neg, -4.0);
    assert(it->id == 2); // distance 1.0, -10 gives 6.0, 0 gives 4.0

    // Test with same position multiple times
    Lines dup = {{1, 3.0}, {2, 3.0}, {3, 8.0}};
    it = findClosestLine(dup, 3.0);
    assert(it->id == 1);

    // Test large values
    Lines large = {{1, 1000.0}, {2, -500.0}, {3, 250.0}};
    it = findClosestLine(large, 260.0);
    assert(it->id == 3); // distance 10.0

    it = findClosestLine(large, -490.0);
    assert(it->id == 2); // distance 10.0

    return 0;
}
#include <cmath>
#include <cstdint>
#include <limits>
#include <vector>

using millimeter_t = double;

struct Line {
    uint32_t id;
    millimeter_t pos;
};

using Lines = std::vector<Line>;

// Returns iterator to the line whose pos is closest to the given position.
Lines::const_iterator findClosestLine(const Lines& lines, millimeter_t pos) {
    Lines::const_iterator closest = lines.end();
    millimeter_t minDist = std::numeric_limits<millimeter_t>::infinity();

    for (Lines::const_iterator it = lines.begin(); it != lines.end(); ++it) {
        const millimeter_t d = std::fabs(it->pos - pos);
        if (d < minDist) {
            minDist = d;
            closest = it;
        }
    }

    return closest;
}
// The algorithm is a straightforward linear scan over the container. Initialize a variable `minDist` to positive infinity (or a very large value) and an iterator `closest` to `lines.end()`. For each element in the container, compute `abs(it->pos - pos)` and if this distance is strictly less than the current minimum, update the minimum and set `closest` to the current iterator. Because we use strict `<`, the first line with a given minimal distance is chosen. After the loop, return `closest`. Edge cases: empty container returns `end()`. If all distances are equal, the first line is returned. Time complexity is \(O(n)\) where \(n\) is the number of lines, and space complexity is \(O(1)\) auxiliary. The use of `micro::numeric_limits<millimeter_t>::infinity()` is replaced here with `std::numeric_limits<millimeter_t>::infinity()` or a large constant to avoid dependency on the `micro` namespace. The type `millimeter_t` is a floating-point alias (e.g., `float`), so `abs` should use `std::fabs` or `std::abs` with `<cmath>`. Since `millimeter_t` might be a user-defined type in the original snippet, we standardize it as an alias for `float` to keep the task self-contained.
