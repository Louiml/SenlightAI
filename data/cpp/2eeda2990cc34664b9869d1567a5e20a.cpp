Write a standalone C++ function named `computeDivisionResults` that takes two integer parameters `a` and `b` (where `b` is guaranteed non-zero), and returns a `std::string` containing four lines, each line showing the result of integer division and floating-point division with both integer and floating-point operands, exactly mirroring the order and formatting shown below. For inputs `a=5` and `b=2`, the output must be:
```
2
2.5
2.5
2.5
```
Each result line must be followed by a newline. The function must handle positive and negative integers correctly, and must use standard division operators (not casts or custom arithmetic) to produce the results. The function must be `const`-correct and should not modify its inputs.

#include <cassert>
#include <string>

// Function declaration (solution above)
std::string computeDivisionResults(int a, int b);

int main() {
    // Basic positive case.
    assert(computeDivisionResults(5, 2) == "2\n2.5\n2.5\n2.5\n");

    // Negative numerator: integer division truncates toward zero.
    assert(computeDivisionResults(-5, 2) == "-2\n-2.5\n-2.5\n-2.5\n");

    // Negative denominator: floating-point results are positive if signs differ.
    assert(computeDivisionResults(5, -2) == "-2\n-2.5\n-2.5\n-2.5\n");

    // Both negative: integer division truncates toward zero, so -5/-2 = 2.
    assert(computeDivisionResults(-5, -2) == "2\n2.5\n2.5\n2.5\n");

    // Numerator zero.
    assert(computeDivisionResults(0, 3) == "0\n0\n0\n0\n");

    // Larger integer division with no fractional part.
    assert(computeDivisionResults(10, 2) == "5\n5\n5\n5\n");

    // Division where integer result differs from floating (e.g., 1/3).
    assert(computeDivisionResults(1, 3) == "0\n0.333333\n0.333333\n0.333333\n");

    // Check that 7/4 gives integer 1, float 1.75.
    assert(computeDivisionResults(7, 4) == "1\n1.75\n1.75\n1.75\n");

    return 0;
}

#include <string>
#include <sstream>

// Compute and return the four division results as a multi-line string.
// The output lines are: integer division, double/int, int/double, double/double.
// Precondition: b != 0.
std::string computeDivisionResults(int a, int b) {
    std::ostringstream output;

    // Integer division truncates toward zero.
    output << a / b << '\n';

    // Floating-point division with promoted numerator.
    output << static_cast<double>(a) / b << '\n';

    // Floating-point division with promoted denominator.
    output << a / static_cast<double>(b) << '\n';

    // Explicit double division.
    output << static_cast<double>(a) / static_cast<double>(b) << '\n';

    return output.str();
}

// The solution is straightforward: compute four division results and format them into a single string. The first division uses integer division `a / b`, which truncates toward zero in C++ (e.g., `5/2` gives `2`, `-5/2` gives `-2`). The second division uses a floating-point numerator: `(double)a / b`, which promotes `b` to `double` and yields a fractional result. The third division uses a floating-point denominator: `a / (double)b`, which yields the same result as the second (since both operands are promoted to `double`). The fourth uses `(double)a / (double)b` explicitly. For edge cases: if `b` is negative, integer division still truncates toward zero, but floating-point results are correct. If `a` or `b` is zero, the function must still work (division by zero is undefined behavior, but the task guarantees `b != 0`; if `a == 0`, all results are `0` or `0.0`). The output format must match exactly: each number followed by a newline, with no extra spaces. Time complexity is O(1), space complexity O(1) (excluding the returned string).
