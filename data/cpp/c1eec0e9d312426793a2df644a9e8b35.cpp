// Write a C++ function `std::string triangleType(const std::vector<int>& nums)` that takes a vector of exactly three positive integers representing side lengths and returns a string indicating the triangle type: `"equilateral"` if all three sides are equal, `"isosceles"` if exactly two sides are equal, `"scalene"` if all sides are distinct, and `"none"` if the three lengths do not satisfy the triangle inequality (i.e., the sum of any two sides must be strictly greater than the third side for a valid triangle). The function must handle edge cases such as invalid side lengths (zero or negative), though the problem guarantees positive integers; if the input size is not exactly 3, the function should return `"invalid"`. Provide a standalone implementation with appropriate `const` correctness and no `main` function.
// The solution first checks that the input vector has exactly 3 elements; if not, return `"invalid"`. Then, it sorts the three sides (or uses a direct check) to apply the triangle inequality: for a valid triangle, the sum of the two smaller sides must be strictly greater than the largest side. Alternatively, one can compute the total sum and verify that `(total - side) > side` for each side, which is equivalent. After confirming triangle validity, classify the triangle by counting distinct side values using a set or by direct comparisons. If all three are equal → `"equilateral"`; if exactly two are equal → `"isosceles"`; if all distinct → `"scalene"`. Edge cases to consider: degenerate triangles where the sum equals the largest side (e.g., `{1,2,3}`) must be `"none"`, and vectors with invalid length (not 3) must be `"invalid"`. Time complexity is O(1) since the vector size is fixed to 3, and space complexity is O(1) (or O(1) extra if using a set of max size 3).
#include <string>
#include <vector>
#include <algorithm>

// Determines the type of triangle from three side lengths.
// Returns "invalid" if nums.size() != 3, "none" if sides cannot form a triangle,
// otherwise "equilateral", "isosceles", or "scalene".
std::string triangleType(const std::vector<int>& nums) {
    if (nums.size() != 3) {
        return "invalid";
    }

    // Check triangle inequality: sum of any two sides > third side.
    int total = nums[0] + nums[1] + nums[2];
    for (int side : nums) {
        if (total - side <= side) {
            return "none";
        }
    }

    // Count distinct sides.
    if (nums[0] == nums[1] && nums[1] == nums[2]) {
        return "equilateral";
    } else if (nums[0] == nums[1] || nums[0] == nums[2] || nums[1] == nums[2]) {
        return "isosceles";
    } else {
        return "scalene";
    }
}
#include <cassert>
#include <vector>
#include <string>

int main() {
    // Equilateral
    assert(triangleType({3, 3, 3}) == "equilateral");
    // Isosceles
    assert(triangleType({5, 5, 3}) == "isosceles");
    // Scalene
    assert(triangleType({3, 4, 5}) == "scalene");
    // Not a triangle (sum of two smaller equals largest)
    assert(triangleType({1, 2, 3}) == "none");
    // Not a triangle (sum of two smaller less than largest)
    assert(triangleType({1, 2, 4}) == "none");
    // Invalid size
    assert(triangleType({1, 2}) == "invalid");
    assert(triangleType({1, 2, 3, 4}) == "invalid");
    // Edge case: very large sides but still valid scalene
    assert(triangleType({1000000, 999999, 1}) == "scalene"); // sum 1000000+999999=1999999 > 1
    // Edge: two equal sides, triangle inequality satisfied
    assert(triangleType({2, 2, 3}) == "isosceles");
    // Edge: zero or negative side (not expected but function still handles)
    assert(triangleType({0, 1, 2}) == "none"); // 0+1 <= 2
    return 0;
}
