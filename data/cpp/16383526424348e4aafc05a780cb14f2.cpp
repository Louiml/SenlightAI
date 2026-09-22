Write a C++ function `long calculateSpan(const std::vector<int>& numbers)` that, given a vector of at least two integers, returns the difference between the largest and smallest values in the vector (the "longest span"). The function must handle vectors with duplicate values and negative numbers correctly. If the input vector contains fewer than two elements, the function should throw a `std::invalid_argument` exception. The function must not modify the input vector and should operate in O(n) time and O(1) auxiliary space.
The solution needs to find both the maximum and minimum elements in the vector in a single pass, then return their difference as a `long`. Important edge cases include: a vector with exactly two elements (the span is simply the absolute difference between them), vectors with all identical values (span = 0), and vectors containing negative numbers (the difference might overflow `int`, hence the use of `long`). The algorithm iterates through the vector once, updating the running maximum and minimum. For \(n\) elements, this takes \(O(n)\) time and uses \(O(1)\) extra space. The function is marked `const` with respect to the input (passed by `const` reference), and the throw condition is checked at the beginning to avoid any unnecessary work.
#include <vector>
#include <stdexcept>
#include <algorithm>

// Compute the difference between the largest and smallest values in a vector.
// Throws std::invalid_argument if the vector has fewer than two elements.
long calculateSpan(const std::vector<int>& numbers) {
    if (numbers.size() < 2) {
        throw std::invalid_argument("At least two numbers required");
    }

    int minVal = numbers[0];
    int maxVal = numbers[0];

    for (size_t i = 1; i < numbers.size(); ++i) {
        minVal = std::min(minVal, numbers[i]);
        maxVal = std::max(maxVal, numbers[i]);
    }

    return static_cast<long>(maxVal) - static_cast<long>(minVal);
}
#include <cassert>
#include <vector>
#include <stdexcept>

int main() {
    // Basic case
    assert(calculateSpan({1, 2, 3, 4}) == 3);
    // Negative numbers
    assert(calculateSpan({-5, -1, -10}) == 9);
    // Duplicates
    assert(calculateSpan({3, 3, 3}) == 0);
    // Exactly two elements
    assert(calculateSpan({7, 7}) == 0);
    assert(calculateSpan({100, 1}) == 99);
    // Unsorted
    assert(calculateSpan({10, -2, 8, 0}) == 12);
    // Larger values to test long conversion
    assert(calculateSpan({2000000000, -2000000000}) == 4000000000L);

    // Exception test
    bool thrown = false;
    try {
        calculateSpan({42});
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try {
        calculateSpan({});
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    assert(thrown);

    return 0;
}
