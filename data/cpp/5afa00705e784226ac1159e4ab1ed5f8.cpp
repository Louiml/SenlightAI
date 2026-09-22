Write a C++ function that takes two integers `a` and `b` and returns a string describing the relationship: if `a > b`, return `"a is the largest"`; if `a < b`, return `"b is the largest"`; if they are equal, return `"a and b are equal"`. The function must handle negative numbers, zero, and large integers correctly (use `int`). The output must exactly match the specified strings, with no extra formatting or numbers. The function should be free of side effects and `const`-correct for its inputs.

// The solution is a straightforward conditional comparison. Read two integers (the function receives them as parameters). Compare using `>` and `<`. If neither is true, they are equal by the trichotomy law for integers. Edge cases: both zero (equal), one negative and one positive (straightforward comparison), equal negative values (equal case). No special overflow concerns because we only compare values already stored in `int`. Time complexity is O(1) with constant number of comparisons. Space complexity is O(1) for the function itself; the returned string uses additional memory proportional to its length, but that is constant for fixed output strings.

#include <string>

// Determine the relationship between two integers and return a descriptive string.
// Returns "a is the largest" if a > b, "b is the largest" if a < b,
// and "a and b are equal" otherwise.
std::string compareIntegers(int a, int b) {
    if (a > b) {
        return "a is the largest";
    } else if (a < b) {
        return "b is the largest";
    } else {
        return "a and b are equal";
    }
}

#include <cassert>
#include <string>

// declaration from the solution
std::string compareIntegers(int a, int b);

int main() {
    // basic cases
    assert(compareIntegers(5, 3) == "a is the largest");
    assert(compareIntegers(3, 5) == "b is the largest");
    assert(compareIntegers(3, 3) == "a and b are equal");

    // zero and negative numbers
    assert(compareIntegers(0, -1) == "a is the largest");
    assert(compareIntegers(-1, 0) == "b is the largest");
    assert(compareIntegers(0, 0) == "a and b are equal");

    // equal negative numbers
    assert(compareIntegers(-7, -7) == "a and b are equal");

    // large values
    assert(compareIntegers(2147483647, -2147483648) == "a is the largest");
    assert(compareIntegers(-2147483648, 2147483647) == "b is the largest");

    // boundary equality
    assert(compareIntegers(-2147483648, -2147483648) == "a and b are equal");
    assert(compareIntegers(2147483647, 2147483647) == "a and b are equal");

    return 0;
}
