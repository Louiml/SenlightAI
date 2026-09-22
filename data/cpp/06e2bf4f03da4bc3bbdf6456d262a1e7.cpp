Write a C++ function `compareFractions` that takes a string representing two fractions in the format `"a/b, c/d"` (where `a`, `b`, `c`, and `d` are positive integers, and the string contains exactly one comma followed by a space separating the two fractions). The function must return a string containing the larger fraction in its original `"numerator/denominator"` format. If the two fractions are equal, return the string `"equal"`. The function should handle whitespace variations (e.g., spaces before/after the numbers or comma) and must not rely on direct floating-point division to avoid precision issues; instead, compare using cross-multiplication (`a * d` versus `c * b`).

#include <cassert>

int main() {
    // Basic cases
    assert(compareFractions("1/2, 3/4") == "3/4");
    assert(compareFractions("3/4, 1/2") == "3/4");
    assert(compareFractions("1/2, 2/4") == "equal");

    // Equal fractions with different representations
    assert(compareFractions("2/3, 4/6") == "equal");
    assert(compareFractions("5/7, 10/14") == "equal");

    // Larger numerators/denominators
    assert(compareFractions("100/200, 50/100") == "equal");
    assert(compareFractions("7/8, 6/7") == "7/8");
    assert(compareFractions("6/7, 7/8") == "7/8");

    // Whitespace variations
    assert(compareFractions("  1/2 , 3/4  ") == "3/4");
    assert(compareFractions("1/2,3/4") == "3/4");

    // Fractions where first is larger
    assert(compareFractions("9/10, 1/10") == "9/10");
    assert(compareFractions("1/3, 2/3") == "2/3");

    return 0;
}

#include <string>
#include <sstream>

// Compare two fractions given as "a/b, c/d" and return the larger one in its original form,
// or "equal" if they are numerically equal. Uses cross-multiplication to avoid floating-point errors.
std::string compareFractions(const std::string& input) {
    int a, b, c, d;
    char slash1, comma, slash2;

    std::istringstream stream(input);
    stream >> a >> slash1 >> b >> comma >> c >> slash2 >> d;

    // Cross-multiply to compare fractions without division.
    int left = a * d;
    int right = c * b;

    if (left > right) {
        return std::to_string(a) + "/" + std::to_string(b);
    } else if (left < right) {
        return std::to_string(c) + "/" + std::to_string(d);
    } else {
        return "equal";
    }
}

// The solution reads the four integers from the input string using a `std::stringstream`, ignoring the slash `/`, comma `,`, and any whitespace using a dummy `char`. Once `a`, `b`, `c`, and `d` are extracted, compare the cross-products: `lhs = a * d` and `rhs = c * b`. If `lhs > rhs`, then `a/b` is larger, so return `to_string(a) + "/" + to_string(b)`. If `lhs < rhs`, then `c/d` is larger, so return `to_string(c) + "/" + to_string(d)`. Otherwise, the fractions are equal, so return `"equal"`. Edge cases include divisions by zero (though the problem statement guarantees positive integers, so no special handling is needed), and inputs with varying whitespace which the stream extraction handles naturally. Time complexity is O(1) because only a fixed number of integers are parsed, and space complexity is O(1) aside from the returned string.
