// Write a C++ function named `secondLargestUnique` that accepts a vector of integers and returns the second largest distinct value in the vector. If the vector contains fewer than two distinct values, the function should throw a `std::runtime_error` with the message `"NOT FOUND"`. The function must handle duplicates correctly by considering only unique elements. The solution should use appropriate standard library containers and follow modern C++ best practices, including `const` correctness.
The problem reduces to finding the second largest element in a set of unique integers. Because duplicates must be ignored, we can insert all elements into a `std::set<int>`, which automatically stores only unique values in sorted order. After insertion, if the set size is less than 2, there is no second-largest distinct value, so throw an exception. Otherwise, the largest element is the last element of the set; the second largest is the element just before it. We can access it using a reverse iterator or by decrementing an iterator from `end()`. The algorithm processes each of the `n` input elements exactly once; set insertion is `O(log n)` on average, so total time complexity is `O(n log n)`. Space complexity is `O(n)` for storing the unique elements in the set. Important edge cases include an empty vector, a vector with all identical values, and a vector with exactly two distinct values, all of which must throw unless there is at least two distinct values.
#include <set>
#include <stdexcept>
#include <vector>

// Return the second largest distinct integer in the vector.
// Throw std::runtime_error if fewer than two distinct values exist.
int secondLargestUnique(const std::vector<int>& values) {
    std::set<int> uniqueValues;

    // Insert all elements; duplicates are automatically ignored.
    for (int value : values) {
        uniqueValues.insert(value);
    }

    // Need at least two distinct values.
    if (uniqueValues.size() < 2) {
        throw std::runtime_error("NOT FOUND");
    }

    // Second largest is the element just before the largest.
    auto it = uniqueValues.end();
    --it;  // Now points to the largest.
    --it;  // Move to the second largest.
    return *it;
}
#include <cassert>
#include <stdexcept>
#include <vector>

int secondLargestUnique(const std::vector<int>& values); // Declaration

int main() {
    // Basic case with distinct values
    assert(secondLargestUnique({1, 2, 3, 4, 5}) == 4);

    // Duplicates ignored
    assert(secondLargestUnique({5, 5, 3, 3, 4, 4, 1}) == 4);

    // Negative and positive values
    assert(secondLargestUnique({-10, -5, -1, 0, 2}) == -1);

    // Unsorted input
    assert(secondLargestUnique({9, 1, 8, 2, 7}) == 8);

    // Exactly two distinct values
    assert(secondLargestUnique({3, 3, 7, 7}) == 3);

    // All equal values -> should throw
    bool threw = false;
    try {
        secondLargestUnique({4, 4, 4});
    } catch (const std::runtime_error& e) {
        threw = true;
        assert(std::string(e.what()) == "NOT FOUND");
    }
    assert(threw);

    // Single element -> should throw
    threw = false;
    try {
        secondLargestUnique({42});
    } catch (const std::runtime_error&) {
        threw = true;
    }
    assert(threw);

    // Empty vector -> should throw
    threw = false;
    try {
        secondLargestUnique({});
    } catch (const std::runtime_error&) {
        threw = true;
    }
    assert(threw);

    return 0;
}
