// Write a standalone C++ function `reverseAndFormat` that takes an array of integers and its size, and returns a `std::string` containing the elements of the array in reverse order, separated by single spaces, with no trailing space. The function must handle arrays of any non-negative size (including empty arrays, for which it should return an empty string), and must not modify the original array. The solution should use `const` for the input array and avoid any global state or extra output side effects.

#include <cassert>
#include <string>
#include <vector>

int main() {
    assert(reverseAndFormat({}) == "");
    assert(reverseAndFormat({42}) == "42");
    assert(reverseAndFormat({1, 2, 3}) == "3 2 1");
    assert(reverseAndFormat({-1, 0, 5, 100}) == "100 5 0 -1");
    assert(reverseAndFormat({7, 7, 7}) == "7 7 7");
    assert(reverseAndFormat({1, -2, 3, -4, 5}) == "5 -4 3 -2 1");
    return 0;
}

#include <string>
#include <vector>

// Returns a string with the elements of arr in reverse order, space-separated.
// For an empty array, returns an empty string.
std::string reverseAndFormat(const std::vector<int>& arr) {
    std::string result;
    int n = arr.size();
    for (int i = n - 1; i >= 0; --i) {
        if (!result.empty()) {
            result += ' ';
        }
        result += std::to_string(arr[i]);
    }
    return result;
}

// The main algorithm is straightforward: iterate from the last index (`n-1`) down to the first index (`0`), appending each element to a string. To avoid a trailing space, append elements with a space separator only between consecutive elements, not after the last one. This can be done by checking if the current position is not the first element being appended, or by building the result and then removing the final space. Edge cases include an empty array (return empty string) and a single-element array (return just that number as a string). Time complexity is O(n) because each element is visited once, and each string concatenation is amortized O(1) length of the accumulated string (or O(n) total if using repeated `+=` which is fine). Space complexity is O(n) for the returned string, plus O(1) auxiliary space.
