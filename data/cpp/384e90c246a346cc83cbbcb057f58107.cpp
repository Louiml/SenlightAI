// Write a C++ function named `maximumOfThree` that takes three integer parameters and returns the maximum value among them. The function must handle ties correctly: when two or more numbers share the maximum value, it should still return that maximum value. The function must not read from standard input or write to standard output—it must be purely computational. Additionally, the function must be marked `const`-correct and must not modify its inputs. After implementing the function, validate it with a set of assertions covering distinct values, duplicate values, negative numbers, and equal values.
// The solution uses a straightforward comparison-based approach. Start by assuming the first number is the maximum, then compare it with the second and third numbers, updating the maximum whenever a larger value is found. This works because the maximum of a set of numbers is simply the largest among them, regardless of duplicates. Edge cases include all three numbers equal (the function returns that value), two numbers equal and larger than the third (returns the duplicate larger value), and negative numbers (comparison operators handle ordering correctly). Time complexity is O(1) with at most two comparisons, and space complexity is O(1) since only a single local variable is used. No special handling is required for ties because the algorithm naturally returns the correct maximum.
#include <algorithm> // for std::max

// Return the maximum of three integers.
// The function does not modify its inputs and is const-correct.
int maximumOfThree(const int a, const int b, const int c) {
    return std::max(a, std::max(b, c));
}
#include <cassert>

// Global main function to test the solution function.
int main() {
    assert(maximumOfThree(1, 2, 3) == 3);
    assert(maximumOfThree(10, 5, 7) == 10);
    assert(maximumOfThree(-1, -5, -3) == -1);
    assert(maximumOfThree(4, 4, 4) == 4);
    assert(maximumOfThree(7, 7, 2) == 7);
    assert(maximumOfThree(1, 9, 9) == 9);
    assert(maximumOfThree(0, 0, 0) == 0);
    assert(maximumOfThree(-100, 100, -50) == 100);
    assert(maximumOfThree(5, 3, 5) == 5);
    assert(maximumOfThree(-1, 0, -2) == 0);
    return 0;
}
