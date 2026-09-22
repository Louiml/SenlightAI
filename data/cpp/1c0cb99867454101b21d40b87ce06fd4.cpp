Write a C++ function named `largestConcatenatedNumber` that takes a non-empty vector of non-negative integers and returns the largest number that can be formed by concatenating all the integers in any order. The result should be returned as a string (not a numeric type) to avoid overflow, and duplicate integers are allowed. If the largest possible number is zero (i.e., all inputs are zero), the function must return the string `"0"`. The order of concatenation should be determined solely by comparing pairs of numbers as strings: `a+b` vs `b+a`, where `a+b` denotes concatenation. The function must be self-contained, properly `const`-correct, and use only the standard library. Example: for input `{3, 30, 34, 5, 9}`, the output should be `"9534330"`; for `{0, 0}` the output should be `"0"`.

// The key insight is that to form the largest number, we need to sort the integers as strings in a custom order. The comparator `comp(a, b)` first converts two integers to strings `a` and `b`, then checks whether concatenating `a+b` is lexicographically greater than `b+a`. If `a+b > b+a`, then `a` should appear before `b` in the final concatenation. This custom sort ensures that the overall concatenation is maximized. After sorting, if the first string is `"0"`, then every string is `"0"` (since no negative numbers and all are non-negative), so the answer must be `"0"` to avoid returning a string like `"000"`. Otherwise, we simply concatenate all strings in the sorted order. Edge cases: all zeros, a single element, and numbers with different lengths (e.g., `3` vs `30`). The time complexity is `O(n log n)` due to sorting, where `n` is the number of integers, and each comparison takes `O(L)` where `L` is the total character length of two numbers (effectively constant for typical inputs). Auxiliary space is `O(n)` to store the string representations.

#include <string>
#include <vector>
#include <algorithm>
#include <functional>

// Helper comparator for sorting strings in a way that maximizes concatenation.
static bool largestConcatComparator(const std::string& a, const std::string& b) {
    return (a + b) > (b + a);
}

// Given a non-empty vector of non-negative integers, return the largest
// number formed by concatenating all integers in any order, as a string.
std::string largestConcatenatedNumber(const std::vector<int>& nums) {
    // Convert all integers to strings.
    std::vector<std::string> snums;
    snums.reserve(nums.size());
    for (int n : nums) {
        snums.push_back(std::to_string(n));
    }

    // Sort using the custom comparator.
    std::sort(snums.begin(), snums.end(), largestConcatComparator);

    // If the largest first element is "0", then all are "0".
    if (snums.front() == "0") {
        return "0";
    }

    // Concatenate all strings.
    std::string result;
    for (const std::string& s : snums) {
        result += s;
    }
    return result;
}

#include <cassert>
#include <string>
#include <vector>

// The solution function is declared above; include it here or place before main.
// For completeness, the function definition is assumed to be included above.
int main() {
    // Basic example from classic problem.
    assert(largestConcatenatedNumber({3, 30, 34, 5, 9}) == "9534330");

    // Single element.
    assert(largestConcatenatedNumber({123}) == "123");

    // All zeros.
    assert(largestConcatenatedNumber({0, 0, 0}) == "0");

    // Leading zero not allowed, but zeros among others.
    assert(largestConcatenatedNumber({0, 1, 2}) == "210");

    // Duplicate numbers.
    assert(largestConcatenatedNumber({10, 2, 10}) == "21010");

    // Large numbers that would overflow a numeric type.
    assert(largestConcatenatedNumber({1000000000, 999999999}) == "9999999991000000000");

    // Already in correct order.
    assert(largestConcatenatedNumber({9, 8, 7}) == "987");

    // Reverse order.
    assert(largestConcatenatedNumber({7, 8, 9}) == "987");

    // Mixed lengths and zeros.
    assert(largestConcatenatedNumber({0, 0, 1}) == "100");
}
