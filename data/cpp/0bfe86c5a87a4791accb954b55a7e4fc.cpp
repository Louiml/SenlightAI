/*
Write a C++ function named `templateArrayMinMax` that accepts a reference to a `std::vector<int>` and returns a `std::pair<int,int>` where the first element is the minimum value and the second element is the maximum value found in the vector. The function must handle an empty vector by throwing a `std::invalid_argument` exception with the message `"empty vector"`. The function must be `const`-correct, meaning it should not modify the input vector, and it should work for vectors containing any number of elements, including negative numbers, zero, and duplicates. The function must be efficient, processing the vector in a single pass without using any standard library algorithms like `std::min_element` or `std::max_element` (implement the logic manually). After writing the function, provide test code that asserts correct behavior for several cases: a non-empty vector with distinct values, a vector with all equal values, a vector with negative numbers, a vector with a single element, and an empty vector (which should throw an exception).
*/

#include <vector>
#include <utility>
#include <stdexcept>

// Returns {min, max} of the elements in the vector.
// Throws std::invalid_argument if the vector is empty.
std::pair<int, int> templateArrayMinMax(const std::vector<int>& data) {
    if (data.empty()) {
        throw std::invalid_argument("empty vector");
    }
    
    int current_min = data[0];
    int current_max = data[0];
    
    for (std::size_t i = 1; i < data.size(); ++i) {
        if (data[i] < current_min) {
            current_min = data[i];
        }
        if (data[i] > current_max) {
            current_max = data[i];
        }
    }
    
    return {current_min, current_max};
}

#include <cassert>
#include <vector>
#include <utility>
#include <stdexcept>

int main() {
    // Normal case with distinct values
    std::vector<int> v1 = {3, 1, 4, 1, 5, 9, 2, 6};
    auto result1 = templateArrayMinMax(v1);
    assert(result1.first == 1 && result1.second == 9);

    // All equal values
    std::vector<int> v2 = {7, 7, 7, 7};
    auto result2 = templateArrayMinMax(v2);
    assert(result2.first == 7 && result2.second == 7);

    // Negative numbers
    std::vector<int> v3 = {-5, -2, -8, -1};
    auto result3 = templateArrayMinMax(v3);
    assert(result3.first == -8 && result3.second == -1);

    // Single element
    std::vector<int> v4 = {42};
    auto result4 = templateArrayMinMax(v4);
    assert(result4.first == 42 && result4.second == 42);

    // Mixed positive and negative with zero
    std::vector<int> v5 = {0, -10, 10, 5, -3};
    auto result5 = templateArrayMinMax(v5);
    assert(result5.first == -10 && result5.second == 10);

    // Empty vector should throw
    std::vector<int> v6;
    bool threw = false;
    try {
        templateArrayMinMax(v6);
    } catch (const std::invalid_argument& e) {
        threw = true;
        assert(std::string(e.what()) == "empty vector");
    }
    assert(threw);

    return 0;
}

// The solution involves iterating through the vector once, maintaining two variables: `current_min` and `current_max`. Initialize both to the first element of the vector (if the vector is non-empty). For each subsequent element, compare and update both variables accordingly. Edge cases to consider: an empty vector must throw an exception before any iteration; a vector with a single element should return that element as both min and max; duplicate values do not require special handling. Time complexity is `O(n)` where `n` is the number of elements, as we traverse the vector exactly once. Space complexity is `O(1)` for the two temporary variables and the pair returned, excluding the input vector itself.
