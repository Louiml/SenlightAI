// Write a C++ function that takes a `std::list<int>` containing the numbers to be summed and returns the sum as an `int`. The function must handle an empty list gracefully by returning 0. You may assume all input values are valid integers (no non-numeric input or overflow concerns), but the function itself should be robust to being called with any `std::list<int>` including one with negative numbers, duplicates, or a single element. The task is to implement the summation logic as a standalone free function that can be reused, with proper `const` correctness (the parameter should be a `const std::list<int>&` to avoid copying and to signal that the list is not modified). Do not write a `main` function in the solution; only the function is required.

// The main algorithm is a straightforward iteration over all elements of the list, accumulating each value into a running total. Since the list is passed by `const` reference, we can safely iterate without modifying it. Start the total at 0; if the list is empty, the loop body never executes and 0 is returned, which is the correct neutral element for addition. For each element, simply add its value to the total. Edge cases include: an empty list (returns 0), a list with one element (returns that element), lists with negative values (handled naturally by addition), and lists with duplicate values (summed normally). Time complexity is \(O(n)\) where \(n\) is the number of elements in the list, because we visit each element exactly once. Space complexity is \(O(1)\) auxiliary, as we only use a single integer variable for the total, plus the loop iterator overhead which is constant.

#include <list>

// Computes and returns the sum of all integers in the list.
// Returns 0 if the list is empty.
int sumList(const std::list<int>& numbers) {
    int total = 0;
    for (int value : numbers) {
        total += value;
    }
    return total;
}

#include <cassert>
#include <list>

// Solution function declaration (must match the provided implementation)
int sumList(const std::list<int>& numbers);

int main() {
    std::list<int> empty;
    assert(sumList(empty) == 0);

    std::list<int> one = {42};
    assert(sumList(one) == 42);

    std::list<int> positives = {1, 2, 3, 4, 5};
    assert(sumList(positives) == 15);

    std::list<int> negatives = {-1, -2, -3};
    assert(sumList(negatives) == -6);

    std::list<int> mixed = {-5, 10, -2, 7};
    assert(sumList(mixed) == 10);

    std::list<int> duplicates = {7, 7, 7, 7};
    assert(sumList(duplicates) == 28);

    std::list<int> withZero = {0, 0, 0};
    assert(sumList(withZero) == 0);

    std::list<int> large = {100, -100, 50, -50, 1};
    assert(sumList(large) == 1);
}
