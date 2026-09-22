// Write a C++ function `largestNumber` that takes a non-empty vector of non-negative integers and returns the largest number that can be formed by arranging the integers in any order and concatenating their decimal representations. The result may be very large, so it must be returned as a string. Leading zeros in the final result are not allowed (e.g., if all numbers are zero, return `"0"`). The input vector may contain duplicate values, and the numbers can range from `0` to at most 1e9.

// The key insight is that the ordering of the numbers must maximize the concatenated string, which is not the same as sorting numerically or lexicographically. The correct custom comparator compares two strings `a` and `b` by checking which concatenation is lexicographically larger: `a + b` vs `b + a`. If `a + b > b + a`, then `a` should come before `b`. This comparator is transitive and yields the optimal order for forming the largest number. After sorting all numbers as strings using this comparator, a special case must be handled: if the first string after sorting is `"0"`, then the entire result is all zeros, and the correct output is simply `"0"` (to avoid returning `"000..."`). Otherwise, concatenate all strings in sorted order. The time complexity is `O(n log n * m)` where `n` is the number of integers and `m` is the average number of digits per number (because each comparison involves concatenating and comparing strings of length up to `2m`). The space complexity is `O(n * m)` for storing the strings and the result.

#include <string>
#include <vector>
#include <algorithm>

// Returns the largest number formed by concatenating the decimal representations of a vector of non-negative integers.
std::string largestNumber(std::vector<int>& nums) {
    std::vector<std::string> strs;
    strs.reserve(nums.size());

    // Convert all integers to strings.
    for (int num : nums) {
        strs.push_back(std::to_string(num));
    }

    // Custom comparator: sorts by which concatenation is lexicographically larger.
    std::sort(strs.begin(), strs.end(), [](const std::string& a, const std::string& b) {
        return a + b > b + a;
    });

    // If the largest number is "0", then the entire result is all zeros.
    if (strs[0] == "0") {
        return "0";
    }

    // Concatenate all sorted strings.
    std::string result;
    for (const std::string& s : strs) {
        result += s;
    }
    return result;
}

#include <cassert>

int main() {
    std::vector<int> nums1 = {3, 30, 34, 5, 9};
    assert(largestNumber(nums1) == "9534330");

    std::vector<int> nums2 = {10, 2};
    assert(largestNumber(nums2) == "210");

    std::vector<int> nums3 = {0, 0, 0};
    assert(largestNumber(nums3) == "0");

    std::vector<int> nums4 = {1};
    assert(largestNumber(nums4) == "1");

    std::vector<int> nums5 = {999, 99, 9};
    assert(largestNumber(nums5) == "999999");

    std::vector<int> nums6 = {121, 12};
    assert(largestNumber(nums6) == "12121");

    std::vector<int> nums7 = {0, 1, 2};
    assert(largestNumber(nums7) == "210");

    std::vector<int> nums8 = {830, 8308};
    assert(largestNumber(nums8) == "8308830");

    return 0;
}
