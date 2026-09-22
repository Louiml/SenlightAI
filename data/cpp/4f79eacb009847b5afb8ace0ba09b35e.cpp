Write a standalone C++ function named `sumInitializerList` that accepts a `std::initializer_list<int>` and returns the sum of all its elements as an `int`. The function must be `const`-correct (i.e., it should not modify the list or any external state), must handle empty initializer lists gracefully by returning `0`, and must not rely on any global variables or external input. The function will be used by a caller that passes brace-enclosed lists of varying sizes, including negative numbers and duplicates, and must produce the correct arithmetic sum.
The solution is straightforward: iterate over every element in the `std::initializer_list<int>` using a range-based `for` loop, accumulating each value into a local `int` accumulator initialized to `0`. Because `std::initializer_list` is a lightweight view over a temporary array, no dynamic allocation is needed, and the iteration is linear. Edge cases: an empty list yields `0` naturally because the accumulator starts at `0` and the loop body never executes. Negative numbers and duplicates are handled automatically by integer addition. The function is marked `const` by taking the initializer list by value (which is cheap and standard practice) and using only local variables. Time complexity is \(O(n)\) where \(n\) is the number of elements, and space complexity is \(O(1)\) auxiliary (the initializer list itself already occupies its storage). Potential overflow is not considered per the task scope; the function uses plain `int` arithmetic.
#include <initializer_list>

// Return the sum of all integers in the given initializer_list.
int sumInitializerList(std::initializer_list<int> values) {
    int total = 0;
    for (int value : values) {
        total += value;
    }
    return total;
}
#include <cassert>

// Function declaration (for clarity; normally the solution header would be included)
int sumInitializerList(std::initializer_list<int> values);

int main() {
    // Basic positive numbers
    assert(sumInitializerList({1, 2, 3, 4, 5}) == 15);
    // Negative numbers and duplicates
    assert(sumInitializerList({-1, -2, -3}) == -6);
    assert(sumInitializerList({5, 5, 5}) == 15);
    // Mixed signs
    assert(sumInitializerList({10, -2, 7, 0}) == 15);
    // Single element
    assert(sumInitializerList({42}) == 42);
    // Empty list
    assert(sumInitializerList({}) == 0);
    // Larger list
    assert(sumInitializerList({1, 1, 1, 1, 1, 1, 1, 1, 1, 1}) == 10);
    return 0;
}
