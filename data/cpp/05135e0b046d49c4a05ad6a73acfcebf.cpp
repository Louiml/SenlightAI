// Write a C++ function `largestConcatenation` that takes a vector of non-negative integer strings and returns a single string formed by concatenating all the elements in an order that produces the largest possible numeric result. For example, given `{"3", "30", "34", "5", "9"}`, the correct order is `"9 5 34 3 30"` (without spaces) yielding `"9534330"`, which is larger than any other arrangement. The input strings may have leading zeros (e.g., `"0"`, `"00"`, `"007"`), and the result should be returned as a string (not an integer) because it can be extremely large. If the vector is empty, return an empty string.

The key observation is that the problem reduces to sorting the strings using a custom comparator. For two strings `a` and `b`, we compare the concatenations `a+b` and `b+a`. If `a+b` is lexicographically greater than `b+a`, then `a` should appear before `b` in the final arrangement. This works because for any two adjacent elements in an optimal ordering, swapping them would produce a smaller or equal concatenation, and the comparator ensures that the sorted order yields the globally maximum concatenation due to transitivity and the fact that the comparator defines a strict weak ordering (it is asymmetric and transitive). After sorting with this comparator, simply concatenate all strings in order to obtain the result. Edge cases: if all strings are `"0"` (or contain only zeros), the result should be a single `"0"` rather than a long string of zeros—so after concatenation, strip leading zeros and if the result becomes empty, return `"0"`. Time complexity is `O(n log n * L)` where `L` is the average length of the strings (since each comparison creates two concatenated strings of length up to `2L`), and space complexity is `O(L)` for the temporary strings during comparison, plus `O(n)` for the output string (ignoring the input vector).

#include <string>
#include <vector>
#include <algorithm>

// Return the largest possible concatenated number from a vector of non-negative integer strings.
std::string largestConcatenation(const std::vector<std::string>& nums) {
    if (nums.empty()) return "";
    
    // Copy to mutable vector for sorting (since the function takes const reference)
    std::vector<std::string> sorted = nums;
    
    // Custom comparator: check which order produces a lexicographically larger concatenation
    std::sort(sorted.begin(), sorted.end(), [](const std::string& a, const std::string& b) {
        // Compare a+b vs b+a without modifying a and b (use local copies)
        std::string ab = a + b;
        std::string ba = b + a;
        return ab > ba; // descending order
    });
    
    // Concatenate all strings
    std::string result;
    for (const auto& s : sorted) {
        result += s;
    }
    
    // Handle the edge case where all numbers are zero (e.g., "0", "00", "000")
    size_t start = result.find_first_not_of('0');
    if (start == std::string::npos) {
        return "0";
    }
    return result.substr(start);
}

#include <cassert>
#include <string>
#include <vector>

// Function prototype for testing (the solution function is defined above)
std::string largestConcatenation(const std::vector<std::string>& nums);

int main() {
    // Basic test cases
    assert(largestConcatenation({"3", "30", "34", "5", "9"}) == "9534330");
    assert(largestConcatenation({"1", "2", "3"}) == "321");
    assert(largestConcatenation({"10", "2"}) == "210");
    assert(largestConcatenation({"0"}) == "0");
    assert(largestConcatenation({"0", "0"}) == "0");
    assert(largestConcatenation({"0", "00", "000"}) == "0");
    assert(largestConcatenation({"9", "90", "9"}) == "9990");
    assert(largestConcatenation({"121", "12"}) == "12121");
    assert(largestConcatenation({"824", "8247"}) == "8248247");
    assert(largestConcatenation({"123", "1234"}) == "1234123");
    assert(largestConcatenation({}) == "");
    assert(largestConcatenation({"54", "546"}) == "54654");
    assert(largestConcatenation({"1", "1", "1"}) == "111");
    return 0;
}
