/*
Write a C++ function `spanStatistics` that takes a vector of integers and returns a `std::pair<int, int>` containing the shortest span (minimum difference between any two distinct elements after sorting) and the longest span (difference between the maximum and minimum values). The function must throw a custom exception `NotEnoughNumbers` if the vector contains fewer than 2 elements. The function must handle duplicate values correctly (e.g., if there are duplicates, the shortest span should be 0), and must work for negative numbers. The function signature should be: `std::pair<int, int> spanStatistics(const std::vector<int>& values);` and you must also define the exception class `NotEnoughNumbers` derived from `std::exception`.
*/
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <utility>

// Custom exception for insufficient numbers.
class NotEnoughNumbers : public std::exception {
public:
    const char* what() const noexcept override {
        return "At least 2 numbers are required to compute spans.";
    }
};

// Returns {shortestSpan, longestSpan} for a vector of integers.
// Throws NotEnoughNumbers if the vector has fewer than 2 elements.
std::pair<int, int> spanStatistics(const std::vector<int>& values) {
    if (values.size() < 2) {
        throw NotEnoughNumbers();
    }

    // Copy to allow sorting without modifying the input.
    std::vector<int> sorted = values;
    std::sort(sorted.begin(), sorted.end());

    // Longest span: max - min.
    int longest = sorted.back() - sorted.front();

    // Shortest span: minimum difference between consecutive sorted elements.
    int shortest = sorted[1] - sorted[0];
    for (std::size_t i = 2; i < sorted.size(); ++i) {
        int diff = sorted[i] - sorted[i - 1];
        if (diff < shortest) {
            shortest = diff;
        }
    }

    return {shortest, longest};
}
#include <cassert>
#include <vector>
#include <utility>

int main() {
    // Basic case
    std::vector<int> v1 = {5, 1, 9, 3};
    auto r1 = spanStatistics(v1);
    assert(r1.first == 2);  // 3-1=2, 5-3=2 (shortest)
    assert(r1.second == 8); // 9-1=8

    // Negative numbers
    std::vector<int> v2 = {-3, -1, -7};
    auto r2 = spanStatistics(v2);
    assert(r2.first == 2);  // -1 - (-3) = 2
    assert(r2.second == 6); // -1 - (-7) = 6

    // Duplicates produce zero shortest span
    std::vector<int> v3 = {4, 1, 4, 7};
    auto r3 = spanStatistics(v3);
    assert(r3.first == 0);  // 4-4=0
    assert(r3.second == 6); // 7-1=6

    // Two elements only
    std::vector<int> v4 = {10, 20};
    auto r4 = spanStatistics(v4);
    assert(r4.first == 10);
    assert(r4.second == 10);

    // Unsorted input is handled correctly
    std::vector<int> v5 = {8, 1, 5, 3};
    auto r5 = spanStatistics(v5);
    assert(r5.first == 2);  // 5-3=2
    assert(r5.second == 7); // 8-1=7

    // Exception for too few elements
    std::vector<int> v6 = {42};
    bool threw = false;
    try {
        spanStatistics(v6);
    } catch (const NotEnoughNumbers&) {
        threw = true;
    }
    assert(threw);

    // Exception for empty vector
    std::vector<int> v7;
    threw = false;
    try {
        spanStatistics(v7);
    } catch (const NotEnoughNumbers&) {
        threw = true;
    }
    assert(threw);

    // Large range
    std::vector<int> v8 = {-100, 0, 100};
    auto r8 = spanStatistics(v8);
    assert(r8.first == 100); // 0-(-100)=100, 100-0=100
    assert(r8.second == 200); // 100-(-100)=200

    return 0;
}
// The solution involves copying the input vector to allow sorting without modifying the caller's data. Sort the copy in ascending order using `std::sort`. For the shortest span, iterate through the sorted vector and compute the difference between each consecutive pair, tracking the minimum absolute difference. Since the vector is sorted, consecutive differences are non-negative, and the minimum is the shortest span. If duplicates exist, consecutive equal values yield a difference of 0, which is the smallest possible. For the longest span, simply subtract the first element (minimum) from the last element (maximum) of the sorted vector. Edge cases: if the vector has fewer than 2 elements, throw the custom exception. The time complexity is O(n log n) due to sorting, and space complexity is O(n) for the copy. The exception class must override `what()` to return a descriptive message.
