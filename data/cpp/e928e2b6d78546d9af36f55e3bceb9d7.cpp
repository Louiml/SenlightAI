Write a C++ function named `printWithCommas` that accepts a `const std::vector<int>&` and returns a single `std::string` containing all the integers separated by " , " (comma, space, comma is not needed; use ", " exactly). The output string should start with the first element and end with the last element, with no leading or trailing whitespace. If the vector is empty, return an empty string. The function must not modify the input vector and must use a range-based for loop or an index-based loop internally. The output format for the vector `{0,1,2}` should be `"0, 1, 2"`.
The solution iterates over each element of the input vector, converting each integer to a string (using `std::to_string`) and appending it to a result string. For all elements except the first, prepend a comma and a space (", ") before the number to achieve the desired separator. The main edge case is an empty vector, which must return an empty string without any separator. Also handle a single-element vector, which should just return that element as a string with no separator. The time complexity is O(n) where n is the number of elements, because each element is visited once and conversion/appending is linear in the number of digits (but overall dominated by n). The space complexity is O(n) due to the output string storing all characters.
#include <string>
#include <vector>

// Returns a string containing all integers in the vector, separated by ", ".
// Returns an empty string if the input vector is empty.
std::string printWithCommas(const std::vector<int>& numbers) {
    std::string result;
    bool first = true;
    for (const int& number : numbers) {
        if (!first) {
            result += ", ";
        }
        result += std::to_string(number);
        first = false;
    }
    return result;
}
#include <cassert>
#include <string>
#include <vector>

// Assume printWithCommas is defined above or included here.

int main() {
    std::vector<int> empty = {};
    assert(printWithCommas(empty) == "");

    std::vector<int> single = {42};
    assert(printWithCommas(single) == "42");

    std::vector<int> small = {0, 1, 2};
    assert(printWithCommas(small) == "0, 1, 2");

    std::vector<int> negative = {-5, -10, -1};
    assert(printWithCommas(negative) == "-5, -10, -1");

    std::vector<int> mixed = {1, -2, 3, 0};
    assert(printWithCommas(mixed) == "1, -2, 3, 0");

    std::vector<int> large = {1000, 2000, 3000};
    assert(printWithCommas(large) == "1000, 2000, 3000");

    std::vector<int> duplicates = {7, 7, 7};
    assert(printWithCommas(duplicates) == "7, 7, 7");
}
