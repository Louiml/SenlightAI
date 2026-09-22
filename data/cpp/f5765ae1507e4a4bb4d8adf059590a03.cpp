// Write a C++ function named `applyIntegerUpdate` that accepts a single integer parameter by value, applies the following transformation to it: if the integer is positive, return its square; if the integer is zero, return −1; if the integer is negative, return its absolute value (i.e., multiply by −1). The function must not modify the original argument (since it is passed by value), and must be `const`‑correct (mark the parameter as `const int`). Additionally, write a second free function named `applySequenceUpdate` that takes a `const std::vector<int>&` and returns a new vector where each element is transformed by `applyIntegerUpdate`, preserving the original order. The task requires you to implement both functions with appropriate return types, handle all edge cases (including empty input vector), and ensure that no side effects occur. The solution must be self‑contained, include all necessary headers, and provide clear commentary.
The core algorithm for `applyIntegerUpdate` is a straightforward branching: check if `a > 0` → return `a * a`; else if `a == 0` → return `-1`; else (i.e., `a < 0`) → return `a * -1` (or `-a`). Since the parameter is passed by value and marked `const`, there is no risk of modification. For `applySequenceUpdate`, iterate over each element of the input vector, call `applyIntegerUpdate` on it, and append the result to a new vector. Edge cases: (1) When the input vector is empty, return an empty vector; (2) When elements are zero or negative, ensure correct branch selection; (3) Squaring a positive integer can overflow if the value is too large, but typical test values are within `int` range—still, we can note that using `long long` internally would be safer, but the specification asks for `int` return, so we assume inputs are within a safe range. Time complexity is O(n) for the vector version and O(1) for the single‑value version. Space complexity is O(n) for the returned vector and O(1) for the single‑value version.
#include <vector>

// Transform a single integer according to the rules:
// positive -> square, zero -> -1, negative -> absolute value.
int applyIntegerUpdate(const int a) {
    if (a > 0) {
        return a * a;
    } else if (a == 0) {
        return -1;
    } else {
        return -a;  // absolute value of a negative number
    }
}

// Apply the transformation to each element of the input vector.
std::vector<int> applySequenceUpdate(const std::vector<int>& nums) {
    std::vector<int> result;
    result.reserve(nums.size());  // optional: avoid reallocations
    for (const int value : nums) {
        result.push_back(applyIntegerUpdate(value));
    }
    return result;
}
#include <cassert>
#include <vector>

// The functions are assumed to be defined above (included from the solution). 
// For completeness, we redeclare them here:
int applyIntegerUpdate(const int a);
std::vector<int> applySequenceUpdate(const std::vector<int>& nums);

int main() {
    // Test applyIntegerUpdate
    assert(applyIntegerUpdate(5) == 25);      // positive → square
    assert(applyIntegerUpdate(-5) == 5);      // negative → absolute value
    assert(applyIntegerUpdate(0) == -1);      // zero → -1
    assert(applyIntegerUpdate(1) == 1);       // positive square of 1
    assert(applyIntegerUpdate(-1) == 1);      // absolute value

    // Test applySequenceUpdate
    std::vector<int> empty;
    assert(applySequenceUpdate(empty).empty());  // empty input → empty output

    std::vector<int> input1 = {3, 0, -4, 2, -1};
    std::vector<int> expected1 = {9, -1, 4, 4, 1};
    assert(applySequenceUpdate(input1) == expected1);

    std::vector<int> input2 = {-2, 0, 0, 7};
    std::vector<int> expected2 = {2, -1, -1, 49};
    assert(applySequenceUpdate(input2) == expected2);

    return 0;
}
