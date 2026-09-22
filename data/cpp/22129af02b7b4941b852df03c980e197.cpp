Write a C++ function that takes a non-empty vector of integers by const reference and returns a string describing the first element, the last element, and whether the vector is "palindromic" (reads the same forward and backward). The output string must be formatted exactly as: `First: X, Last: Y, Palindrome: YES/NO`. If the vector has only one element, it is considered trivially palindromic. You must handle vectors of any size ≥ 1, including negative numbers, without modifying the input.
#include <cassert>
#include <vector>
#include <string>

// Declare the function (or include the solution header).
std::string describeVector(const std::vector<int>& numbers);

int main() {
    // Single element: trivially palindromic
    assert(describeVector({5}) == "First: 5, Last: 5, Palindrome: YES");
    
    // Simple even-length palindromic vector
    assert(describeVector({1, 2, 2, 1}) == "First: 1, Last: 1, Palindrome: YES");
    
    // Odd-length palindromic with negative numbers
    assert(describeVector({-3, 0, -3}) == "First: -3, Last: -3, Palindrome: YES");
    
    // Non-palindromic vector
    assert(describeVector({1, 2, 3, 4}) == "First: 1, Last: 4, Palindrome: NO");
    
    // Non-palindromic with matching first/last but mismatch in middle
    assert(describeVector({7, 8, 9, 7}) == "First: 7, Last: 7, Palindrome: NO");
    
    // All same values, larger size
    assert(describeVector({2, 2, 2, 2, 2}) == "First: 2, Last: 2, Palindrome: YES");
    
    // Negative numbers and mismatch at ends
    assert(describeVector({-1, 5, 3}) == "First: -1, Last: 3, Palindrome: NO");
    
    // Even-length palindromic with large numbers
    assert(describeVector({100, 200, 200, 100}) == "First: 100, Last: 100, Palindrome: YES");
    
    return 0;
}
#include <string>
#include <vector>

// Returns a string describing first, last, and palindrome status.
std::string describeVector(const std::vector<int>& numbers) {
    // Edge case: empty vector is not allowed per spec, but handle defensively.
    if (numbers.empty()) {
        return "First: N/A, Last: N/A, Palindrome: NO";
    }
    
    int first = numbers.front();
    int last = numbers.back();
    
    bool palindromic = true;
    size_t left = 0;
    size_t right = numbers.size() - 1;
    
    while (left < right) {
        if (numbers[left] != numbers[right]) {
            palindromic = false;
            break;
        }
        ++left;
        --right;
    }
    
    std::string result = "First: " + std::to_string(first) + ", Last: " + std::to_string(last) + ", Palindrome: ";
    result += (palindromic ? "YES" : "NO");
    return result;
}
// The solution requires reading the first and last elements directly from the vector using `front()` and `back()` (or index 0 and size-1). To check palindromicity, iterate from both ends toward the center using two indices, comparing elements; stop early if a mismatch is found. Edge case: a vector of size 1 is always palindromic, and the first and last elements are the same value. The algorithm runs in O(n) time in the worst case (palindromic or when mismatch is at the center) but can terminate early on the first mismatch, giving O(1) best case. Space complexity is O(1) auxialiary, plus O(1) for the returned string (which has constant length). The function should be `const` correct by taking the vector as `const std::vector<int>&`, and internally use `size_t` for indices to avoid signed/unsigned warnings.
