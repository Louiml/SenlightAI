// Write a C++ function named `sumEvenArrayElements` that takes a `const std::vector<int>&` and returns an `int` representing the sum of all even-valued elements in the vector. The function must handle vectors of any size (including empty), and only even numbers (divisible by 2) contribute to the sum. Negative even numbers (e.g., -4) must also be included. Do not modify the input vector; use `const` reference. The function should be self-contained with necessary headers (`<vector>` and `<numeric>` if desired, but a simple loop is fine).
The main algorithm is straightforward: iterate through each element of the vector using a range-based `for` loop, check if the element is even using the condition `value % 2 == 0`. If it is, add it to an accumulating sum initialized to 0. Edge cases: an empty vector returns 0 (since there are no elements to sum). The check `% 2 == 0` works for negative even numbers because in C++ the remainder of a negative number divided by 2 is either 0 (if even) or -1 (if odd), so `value % 2 == 0` correctly identifies negative evens. The time complexity is O(n) where n is the number of elements, and space complexity is O(1) aside from the input vector itself.
#include <vector>

// Sum all even integers in the given vector.
// Returns 0 for an empty vector.
int sumEvenArrayElements(const std::vector<int>& numbers) {
    int total = 0;
    for (int value : numbers) {
        if (value % 2 == 0) {
            total += value;
        }
    }
    return total;
}
#include <cassert>
#include <vector>

// Declaration of the function under test (in a real project, include the header)
int sumEvenArrayElements(const std::vector<int>& numbers);

int main() {
    // Empty vector -> sum is 0
    assert(sumEvenArrayElements({}) == 0);

    // All even positive numbers
    assert(sumEvenArrayElements({2, 4, 6, 8}) == 20);

    // Mixed odd and even
    assert(sumEvenArrayElements({1, 2, 3, 4, 5, 6}) == 12);

    // Negative even numbers included
    assert(sumEvenArrayElements({-4, -2, 1, 3}) == -6);

    // Single element: odd -> 0, even -> that value
    assert(sumEvenArrayElements({7}) == 0);
    assert(sumEvenArrayElements({10}) == 10);

    // All odd
    assert(sumEvenArrayElements({1, 3, 5, 7}) == 0);

    // Duplicates and zeros
    assert(sumEvenArrayElements({0, 0, 2, 2, 3}) == 4);

    return 0;
}
