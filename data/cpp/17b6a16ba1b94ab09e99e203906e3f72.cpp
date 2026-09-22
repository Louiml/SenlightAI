/*
Write a C++ function named `checkRange` that takes an integer value and returns a boolean indicating whether the integer lies within the inclusive range [10, 20]. The function should return `true` if the value is between 10 and 20 inclusive, and `false` otherwise. The function must be `const`-correct (the integer parameter is passed by value, so mark it `const` inside the parameter list) and should not print anything to the console—the caller is responsible for output. This function should be usable in a simple program that reads an integer from standard input and prints "yes" if the integer is in range, otherwise "no".
*/
#include <cstdbool> // for bool type, though not strictly required in C++

// Returns true if value lies within the inclusive range [10, 20].
bool checkRange(const int value) {
    return (value >= 10 && value <= 20);
}
#include <cassert>

int main() {
    // Boundary values: exactly 10 and 20 should return true
    assert(checkRange(10) == true);
    assert(checkRange(20) == true);

    // Interior values
    assert(checkRange(15) == true);
    assert(checkRange(12) == true);

    // Values just outside range
    assert(checkRange(9) == false);
    assert(checkRange(21) == false);

    // Negative and large values
    assert(checkRange(-5) == false);
    assert(checkRange(100) == false);
    assert(checkRange(0) == false);

    return 0;
}
// The solution is straightforward: compare the input integer against the lower bound (10) and upper bound (20). The condition `value >= 10 && value <= 20` captures inclusivity on both ends. Edge cases include exactly the boundary values (10 and 20) which must return `true`, and values immediately outside (e.g., 9 and 21) which must return `false`. No special handling is needed for negative numbers or large values—the comparison works for any `int`. Time complexity is O(1) as it performs a fixed number of comparisons; space complexity is O(1) as it uses no additional data structures.
