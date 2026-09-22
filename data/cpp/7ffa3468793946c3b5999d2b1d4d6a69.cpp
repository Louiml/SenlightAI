// Write a C++ function `reverseAndPrint` that takes a vector of integers and returns a string containing the original integers reversed in order, each followed by exactly one space, with no trailing space at the end. The function must handle empty input by returning an empty string, and must work correctly when the vector contains negative numbers, zeros, or repeated values. Do not modify the input vector – the function should take the vector by constant reference. The function should be reusable and not depend on any global state or console input/output.
The solution is straightforward: iterate over the vector from the last element to the first, appending each integer converted to a string plus a space. To avoid a trailing space, either build the string with spaces and then trim the last character (if non-empty), or check if it's the last element being processed and skip adding the space in that case. Edge cases include: an empty vector (return empty string), a single element (return that number without any space), and negative numbers (standard `std::to_string` handles the minus sign). Time complexity is \(O(n)\) where \(n\) is the size of the vector, since we visit each element exactly once. Space complexity is \(O(n)\) for the resulting string, with no extra auxiliary data structures. Using `const vector<int>&` ensures we don't accidentally modify the input, and `std::to_string` converts each integer safely.
#include <string>
#include <vector>

// Return a string containing the integers in reverse order, separated by single spaces,
// with no trailing space. Returns an empty string for an empty vector.
std::string reverseAndPrint(const std::vector<int>& numbers) {
    std::string result;
    for (auto it = numbers.rbegin(); it != numbers.rend(); ++it) {
        result += std::to_string(*it);
        if (it + 1 != numbers.rend()) {
            result += ' ';
        }
    }
    return result;
}
#include <cassert>
#include <vector>
#include <string>

// (Solution function is assumed to be included above or in the same translation unit)

int main() {
    // Empty vector
    assert(reverseAndPrint({}) == "");
    
    // Single element
    assert(reverseAndPrint({5}) == "5");
    
    // Normal order
    assert(reverseAndPrint({1, 2, 3}) == "3 2 1");
    
    // Negative numbers and zeros
    assert(reverseAndPrint({-1, 0, -2}) == "-2 0 -1");
    
    // Duplicates
    assert(reverseAndPrint({7, 7, 7}) == "7 7 7");
    
    // Large vector with mixed values
    assert(reverseAndPrint({10, -20, 30, 0, -40}) == "-40 0 30 -20 10");
    
    // Check that input vector is not modified (const reference)
    std::vector<int> input = {1, 2, 3};
    reverseAndPrint(input);
    assert(input == std::vector<int>({1, 2, 3}));
    
    return 0;
}
