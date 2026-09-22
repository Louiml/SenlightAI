// Write a C++ function `largestNumberFromArray` that takes a non-empty vector of non-negative integers and returns a string representing the largest possible number formed by concatenating all the integers in some order. The function must handle leading zeros by stripping them from the final result (if the result becomes an empty string or all zeros, return "0"). For example, given `[3, 30, 34, 5, 9]`, the largest number is `"9534330"`; for `[0,0]`, the result must be `"0"`, not `"00"`.
The main idea is to sort the integers using a custom comparator that compares two numbers by their concatenated forms: `a` should come before `b` if `to_string(a)+to_string(b) > to_string(b)+to_string(a)`. This ensures the largest possible concatenation appears first. After sorting, concatenate all numbers into a single string. The critical edge case is when the input contains only zeros (e.g., `[0,0]`), which would produce `"00"`; the standard practice is to strip leading zeros, so we remove leading `'0'` characters while the result length is greater than 1, leaving `"0"` if all are zeros. The custom comparator uses string concatenation, so each comparison costs \(O(L)\) where \(L\) is the total length of the concatenated strings per comparison (approximately twice the average digit length). Sorting takes \(O(n \log n)\) comparisons, so overall time complexity is \(O(n L \log n)\), where \(L\) is the average digit count. Space complexity is \(O(n)\) for the sorted vector and the output string (excluding the internal sorting stack). The comparator must be strict weak ordering; using `>` in the comparator with `sort` is incorrect (should return `true` when `a` should precede `b`), but the provided snippet works because it returns `true` when `a`'s concatenation is larger, which is correct. However, we will implement a safe `static bool cmp` that returns `a+b > b+a` and ensure it is a strict weak ordering by using `<` semantics internally with a positive–negative check, but the direct `>` is acceptable for distinct values. To be safe, we compare both directions to avoid equality ties (e.g., `[1,1]`), but equality does not matter for sorting.
#include <string>
#include <vector>
#include <algorithm>

// Custom comparator: return true if a should appear before b
static bool concatenationOrder(int a, int b) {
    std::string left = std::to_string(a) + std::to_string(b);
    std::string right = std::to_string(b) + std::to_string(a);
    return left > right;
}

// Return the largest number formed by concatenating all integers in any order.
std::string largestNumberFromArray(const std::vector<int>& nums) {
    std::vector<int> sorted = nums; // copy to allow sorting without modifying input
    std::sort(sorted.begin(), sorted.end(), concatenationOrder);

    std::string result;
    for (int num : sorted) {
        result += std::to_string(num);
    }

    // Strip leading zeros, but keep at least one digit (if all zeros, result becomes "0")
    size_t firstNonZero = 0;
    while (firstNonZero < result.length() - 1 && result[firstNonZero] == '0') {
        ++firstNonZero;
    }
    return result.substr(firstNonZero);
}
#include <cassert>
#include <string>
#include <vector>

int main() {
    // Basic test from problem statement
    assert(largestNumberFromArray({3, 30, 34, 5, 9}) == "9534330");
    // All zeros produce "0", not "00"
    assert(largestNumberFromArray({0, 0}) == "0");
    // Single zero
    assert(largestNumberFromArray({0}) == "0");
    // Single non-zero
    assert(largestNumberFromArray({5}) == "5");
    // Already sorted correctly
    assert(largestNumberFromArray({9, 8, 7}) == "987");
    // Edge with repeated numbers
    assert(largestNumberFromArray({1, 1, 1}) == "111");
    // Mixed where order matters due to prefix
    assert(largestNumberFromArray({10, 1}) == "110");
    // Larger case
    assert(largestNumberFromArray({824, 938, 1399, 5607, 6973, 5703, 9609, 4398, 8247}) == "9609938824824769735703560743981399");
    // All same leading digit but different lengths
    assert(largestNumberFromArray({3, 33, 333}) == "333333");
    return 0;
}
