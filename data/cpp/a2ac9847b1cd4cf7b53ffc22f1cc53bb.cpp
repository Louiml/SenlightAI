Write a C++ function named `reverseVectorDescription` that takes a `const std::vector<int>&` and returns a `std::string` containing the elements of the vector in reverse order, separated by a single space, with a single leading space before the first element (so that the output format matches the snippet's prefix "myvector backwards:" followed by the reversed elements). If the vector is empty, return an empty string. The function must not modify the input vector.

The task is straightforward: traverse the vector from the last element to the first using reverse iterators (`crbegin()` and `crend()`), and build a string by appending each integer preceded by a space. The key steps: if the vector is empty, return immediately with an empty string to avoid leading spaces. Otherwise, loop over `auto rit = v.crbegin(); rit != v.crend(); ++rit` and append `" " + std::to_string(*rit)` to the result. This ensures exactly one leading space and no trailing spaces. Edge cases: empty vector returns ""; a vector with one element returns " " + that element. Time complexity is O(n) for n elements (each integer converted to string and appended), and space complexity is O(n) for the result string, plus O(1) auxiliary.

#include <string>
#include <vector>

// Return a string of the vector's elements in reverse order, each preceded by a space.
// For example, input {1,2,3} returns " 3 2 1". Empty input returns "".
std::string reverseVectorDescription(const std::vector<int>& v) {
    if (v.empty()) {
        return "";
    }

    std::string result;
    for (auto rit = v.crbegin(); rit != v.crend(); ++rit) {
        result += " " + std::to_string(*rit);
    }
    return result;
}

#include <cassert>
#include <string>
#include <vector>

// (Assume the solution function is declared above.)

int main() {
    // Basic case with multiple elements
    std::vector<int> v1 = {1, 2, 3, 4, 5};
    assert(reverseVectorDescription(v1) == " 5 4 3 2 1");

    // Single element
    std::vector<int> v2 = {42};
    assert(reverseVectorDescription(v2) == " 42");

    // Empty vector
    std::vector<int> v3;
    assert(reverseVectorDescription(v3) == "");

    // Negative numbers and duplicates
    std::vector<int> v4 = {-1, 0, -1, 3, 3};
    assert(reverseVectorDescription(v4) == " 3 3 -1 0 -1");

    // Large numbers
    std::vector<int> v5 = {1000000, 2000000, 3000000};
    assert(reverseVectorDescription(v5) == " 3000000 2000000 1000000");

    // Vector with two elements in ascending order
    std::vector<int> v6 = {7, 8};
    assert(reverseVectorDescription(v6) == " 8 7");

    // Vector with two equal elements
    std::vector<int> v7 = {5, 5};
    assert(reverseVectorDescription(v7) == " 5 5");

    // Vector with negative single element
    std::vector<int> v8 = {-99};
    assert(reverseVectorDescription(v8) == " -99");

    // Verify input is not modified
    std::vector<int> v9 = {10, 20, 30};
    reverseVectorDescription(v9);
    assert(v9 == std::vector<int>({10, 20, 30}));
}
