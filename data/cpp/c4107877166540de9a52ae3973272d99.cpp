Write a C++ function named `displayOrderedPairs` that takes a non-empty `std::vector<int>` by const-reference and prints every possible ordered pair `(x, y)` where `x` and `y` are elements of the vector (including cases where `x` and `y` are the same element). Each pair must be printed on its own line in the format `x,y` (no spaces). The function must not modify the input vector and must not return any value. The output must be deterministic, with pairs generated in row-major order: for each `i` from 0 to n-1, print all `j` values from 0 to n-1 (i.e., all pairs `(array[i], array[0]), (array[i], array[1]), ..., (array[i], array[n-1])`). The function should use `cout` for output and must be declared with proper `const` correctness. Do not include a `main` function in your solution; only provide the free function and necessary includes.
// The task is a direct application of nested loops over the vector's indices. The main algorithm iterates with an outer loop over each element as the first component of the pair, and an inner loop over all elements as the second component. For every combination of `i` and `j`, the function prints `array[i]` followed by a comma and `array[j]`, then a newline. This naturally produces all `n^2` ordered pairs, including duplicates such as `(a,a)` and repeated values. Edge cases include a vector with a single element (so only one pair is printed, `x,x`) and vectors with duplicate values (which are simply printed multiple times, as expected). Since the vector is passed by const reference, it is not copied, and no modifications occur. Time complexity is \(O(n^2)\) because the nested loops execute exactly `n^2` iterations. Space complexity is \(O(1)\) because only loop counters and temporary references are used, and no extra containers are allocated beyond the input vector itself.
#include <iostream>
#include <vector>

// Print every ordered pair (x, y) from the vector, one per line as "x,y".
void displayOrderedPairs(const std::vector<int>& array) {
    for (std::size_t i = 0; i < array.size(); ++i) {
        for (std::size_t j = 0; j < array.size(); ++j) {
            std::cout << array[i] << "," << array[j] << '\n';
        }
    }
}
#include <cassert>
#include <sstream>
#include <vector>

// Forward declaration of the function (assumes the solution is included above).
void displayOrderedPairs(const std::vector<int>& array);

// Helper to capture stdout from a function call as a string.
std::string captureOutput(const std::vector<int>& input) {
    std::ostringstream buffer;
    std::streambuf* oldCout = std::cout.rdbuf(buffer.rdbuf());
    displayOrderedPairs(input);
    std::cout.rdbuf(oldCout);
    return buffer.str();
}

int main() {
    // Single element: only one ordered pair.
    assert(captureOutput({7}) == "7,7\n");

    // Two distinct elements: all 2x2 pairs.
    assert(captureOutput({1, 2}) == "1,1\n1,2\n2,1\n2,2\n");

    // Three elements with duplicates: all 3x3 pairs including repeats.
    assert(captureOutput({1, 1, 2}) ==
           "1,1\n1,1\n1,2\n1,1\n1,1\n1,2\n2,1\n2,1\n2,2\n");

    // Larger example: verify first and last lines only, but full length.
    std::vector<int> test = {1, 2, 3, 4};
    std::string out = captureOutput(test);
    assert(out == "1,1\n1,2\n1,3\n1,4\n2,1\n2,2\n2,3\n2,4\n3,1\n3,2\n3,3\n3,4\n4,1\n4,2\n4,3\n4,4\n");

    // Empty vector is disallowed by spec, but testing a non-empty vector with repeated values.
    assert(captureOutput({5, 5}) == "5,5\n5,5\n5,5\n5,5\n");

    return 0;
}
