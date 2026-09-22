Write a C++ function that takes three integers as input and returns a string containing the smallest and largest of the three values, formatted as "min max" with a single space separating them. The function must handle negative numbers, duplicate values, and zero correctly. The three input values are passed as parameters, and the function returns the result as a `std::string`. Edge cases include all values being equal, all values being negative, and mixed positive/negative values.

// The solution is straightforward: initialize the minimum and maximum to the first value, then compare each subsequent value against both. Since there are exactly three values, we can use nested comparisons or `std::min` and `std::max` with initializer lists. The algorithm does not require sorting—only two comparisons per value to update both extremes. Edge cases: duplicates do not affect the result (e.g., 5, 5, 5 yields "5 5"), and negatives work naturally with comparisons. Time complexity is O(1) since the input size is fixed at three; space complexity is O(1) aside from the returned string.

#include<string>
#include<algorithm>

// Return a string containing the smallest and largest of three integers,
// formatted as "min max".
std::string minMaxOfThree(int a, int b, int c) {
    const int minimum = std::min({a, b, c});
    const int maximum = std::max({a, b, c});
    return std::to_string(minimum) + " " + std::to_string(maximum);
}

#include<assert.h>
#include<string>

// Declare the function (or include the header where it is defined)
std::string minMaxOfThree(int a, int b, int c);

int main() {
    // All equal
    assert(minMaxOfThree(5, 5, 5) == "5 5");
    // Negative and positive mix
    assert(minMaxOfThree(-3, 7, 0) == "-3 7");
    // Negative values only
    assert(minMaxOfThree(-1, -10, -100) == "-100 -1");
    // Order variations
    assert(minMaxOfThree(2, 1, 3) == "1 3");
    assert(minMaxOfThree(9, 4, 7) == "4 9");
    // Duplicate min or max
    assert(minMaxOfThree(4, 4, 9) == "4 9");
    assert(minMaxOfThree(1, 8, 8) == "1 8");
    // Zero as a value
    assert(minMaxOfThree(0, -5, 2) == "-5 2");
    // Large and small extremes
    assert(minMaxOfThree(2147483647, -2147483647, 0) == "-2147483647 2147483647");
}
