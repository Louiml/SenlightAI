/*
Write a C++ function named `printArrayStatistics` that takes a `const std::vector<int>&` as input and returns a `std::string` containing the sum of all elements, the smallest element, and the largest element, formatted as `"sum=X, min=Y, max=Z"` (with no extra spaces, and in that exact order). The input vector may be empty, may contain negative numbers, duplicates, and up to 10^6 elements. Your function must handle an empty vector gracefully by returning `"sum=0, min=0, max=0"` for that case. Ensure the function is `const`-correct and uses appropriate standard library algorithms where suitable.
*/
#include <string>
#include <vector>

// Returns a formatted string with sum, min, and max of the input vector.
// For an empty input, returns "sum=0, min=0, max=0".
std::string printArrayStatistics(const std::vector<int>& arr) {
    if (arr.empty()) {
        return "sum=0, min=0, max=0";
    }

    int sum = 0;
    int min_val = arr[0];
    int max_val = arr[0];

    for (int value : arr) {
        sum += value;
        if (value < min_val) min_val = value;
        if (value > max_val) max_val = value;
    }

    return "sum=" + std::to_string(sum) + ", min=" + std::to_string(min_val) + ", max=" + std::to_string(max_val);
}
#include <cassert>
#include <vector>
#include <string>

// Declaration of the function under test (or include the solution header)
std::string printArrayStatistics(const std::vector<int>& arr);

int main() {
    // Empty vector
    assert(printArrayStatistics({}) == "sum=0, min=0, max=0");

    // Simple positive case
    assert(printArrayStatistics({1, 2, 3, 4}) == "sum=10, min=1, max=4");

    // Negative numbers
    assert(printArrayStatistics({-5, -1, -10}) == "sum=-16, min=-10, max=-1");

    // Single element
    assert(printArrayStatistics({7}) == "sum=7, min=7, max=7");

    // Duplicates and zeros
    assert(printArrayStatistics({0, 0, -2, 3}) == "sum=1, min=-2, max=3");

    // Large values
    assert(printArrayStatistics({1000000, -1000000, 5}) == "sum=5, min=-1000000, max=1000000");

    // Mixed large and negative
    assert(printArrayStatistics({-100, 200, -300}) == "sum=-200, min=-300, max=200");
}
// The solution iterates over the vector once to compute the sum, and simultaneously track the minimum and maximum. We initialize `sum` to `0`, and for empty vectors we immediately return the fallback string. For non-empty vectors, we initialize `min` and `max` to the first element to avoid issues with `INT_MAX`/`INT_MIN` extremes, then update them as we traverse. Using `std::accumulate` for the sum and `std::min_element` / `std::max_element` would require three passes; a single pass is more efficient and simpler. Edge cases: empty vector (return defaults), single element (min==max==element), all negative values, duplicate values (no special handling needed). Time complexity is O(n) for a single pass, space complexity is O(1) beyond the input and output string.
