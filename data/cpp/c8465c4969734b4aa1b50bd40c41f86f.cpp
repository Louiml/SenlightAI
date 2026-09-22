/*
Write a C++ function named `compareIntegers` that takes two integers `a` and `b` as parameters and returns a string describing their relationship: `"a < b"` if `a` is less than `b`, `"a > b"` if `a` is greater than `b`, or `"a == b"` if they are equal. The function must handle any integer values (including negatives, zero, and large values) and must not read from standard input or write to standard output; it should only return the result string. The caller will handle user interaction. Ensure the function is `const`-correct (parameters passed by value for simple integers) and include necessary headers.
*/
#include <string>

// Compare two integers and return a string describing their relationship.
std::string compareIntegers(const int a, const int b) {
    if (a < b) {
        return "a < b";
    }
    if (a > b) {
        return "a > b";
    }
    return "a == b";
}
#include <cassert>
#include <string>

// Forward declaration of the solution function (not required in test if included in same file, but here for clarity).
std::string compareIntegers(const int a, const int b);

int main() {
    // Test basic less-than, greater-than, and equality.
    assert(compareIntegers(1, 2) == "a < b");
    assert(compareIntegers(5, 3) == "a > b");
    assert(compareIntegers(4, 4) == "a == b");

    // Test negative numbers.
    assert(compareIntegers(-1, 0) == "a < b");
    assert(compareIntegers(-5, -10) == "a > b");
    assert(compareIntegers(-7, -7) == "a == b");

    // Test zero and positive values.
    assert(compareIntegers(0, 100) == "a < b");
    assert(compareIntegers(100, 0) == "a > b");
    assert(compareIntegers(0, 0) == "a == b");

    // Test large integers.
    assert(compareIntegers(2147483647, -2147483647) == "a > b");
    assert(compareIntegers(-2147483647, 2147483647) == "a < b");
    assert(compareIntegers(123456789, 123456789) == "a == b");

    // Test mixed signs and extreme values.
    assert(compareIntegers(-999, -998) == "a < b");
    assert(compareIntegers(-998, -999) == "a > b");
    assert(compareIntegers(42, 43) == "a < b");
    assert(compareIntegers(43, 42) == "a > b");
    assert(compareIntegers(1, 1) == "a == b");

    return 0;
}
// The solution is straightforward: compare the two integer arguments using simple relational operators. Start by checking `a < b`; if true, return `"a < b"`. Otherwise, check `a > b`; if true, return `"a > b"`. If neither condition holds, the only remaining possibility is equality, so return `"a == b"`. No special edge cases exist beyond standard integer behavior; negative numbers, zeros, and large values work without issues. The algorithm runs in constant time \(O(1)\) and uses constant space \(O(1)\) because it only performs a few comparisons and constructs a fixed-size string.
