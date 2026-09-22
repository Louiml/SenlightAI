Write a C++ function that takes a vector of non-negative integers and returns the largest number that can be formed by concatenating all the integers in any order. The result should be returned as a string (since it may exceed the range of standard integer types). If the largest number is zero (i.e., all integers are zero), return the string `"0"`. The function must handle vectors containing zeros, duplicate values, and numbers up to several digits each. The input vector is guaranteed to be non-empty.
The core idea is to sort the numbers using a custom comparator that decides which of two numbers should appear first in the final concatenation to yield a larger combined number. For two numbers `a` and `b`, we compare the two possible concatenations: `to_string(a) + to_string(b)` versus `to_string(b) + to_string(a)`. If the former is lexicographically (and numerically, since both have the same length) greater, then `a` should come before `b`. This comparator is not a strict weak ordering in the traditional numeric sense, but it is transitive for the purpose of forming the largest concatenation, making it valid for `std::sort`. After sorting, if the first element is `0`, then all elements are zero, and the result is simply `"0"`. Otherwise, we concatenate all the numbers in sorted order. Edge cases include vectors with a single zero, vectors with multiple zeros, and vectors where leading zeros would appear if not handled (though since all inputs are non-negative, zeros only matter when they are the sole elements). The time complexity is O(n log n) due to sorting, where n is the number of integers, plus O(k) for concatenation where k is the total number of digits. The space complexity is O(k) for the output string and the temporary strings created during comparisons.
#include <string>
#include <vector>
#include <algorithm>

// Compares two integers by which concatenation yields a larger number.
bool compareForLargestConcat(int a, int b) {
    std::string sa = std::to_string(a);
    std::string sb = std::to_string(b);
    return sa + sb > sb + sa;
}

// Returns the largest number formed by concatenating all non-negative integers.
std::string largestConcatenatedNumber(const std::vector<int>& nums) {
    if (nums.empty()) {
        return "";
    }

    // Copy to allow sorting without modifying the original vector.
    std::vector<int> sortedNums = nums;
    std::sort(sortedNums.begin(), sortedNums.end(), compareForLargestConcat);

    // If the largest number after sorting is 0, all numbers are zero.
    if (sortedNums[0] == 0) {
        return "0";
    }

    std::string result;
    for (int num : sortedNums) {
        result += std::to_string(num);
    }
    return result;
}
#include <cassert>
#include <vector>
#include <string>

// The function declaration is assumed to be available from the solution above.
// The following main() runs tests.

int main() {
    // Basic cases
    assert(largestConcatenatedNumber({10, 2}) == "210");
    assert(largestConcatenatedNumber({3, 30, 34, 5, 9}) == "9534330");
    assert(largestConcatenatedNumber({1}) == "1");
    assert(largestConcatenatedNumber({0, 0}) == "0");

    // Edge cases with zeros mixed
    assert(largestConcatenatedNumber({0, 1}) == "10");
    assert(largestConcatenatedNumber({0, 0, 1}) == "100");
    assert(largestConcatenatedNumber({0}) == "0");

    // Duplicates and larger numbers
    assert(largestConcatenatedNumber({121, 12}) == "12121");
    assert(largestConcatenatedNumber({128, 12}) == "12812");
    assert(largestConcatenatedNumber({824, 938, 1399, 5607, 6973, 5703, 9609, 4398, 8247}) == "9609938824824769735703560743981399");

    // All identical numbers
    assert(largestConcatenatedNumber({9, 9, 9}) == "999");
    assert(largestConcatenatedNumber({1, 1, 1}) == "111");

    return 0;
}
