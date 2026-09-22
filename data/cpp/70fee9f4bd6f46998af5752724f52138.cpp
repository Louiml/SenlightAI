Write a C++ function named `compressRanges` that takes a non-empty string `s` containing positive integers separated by commas (e.g., `"3,1,2,4,7,8,10"`) and returns a string representing the sorted integers as compressed ranges using the format: consecutive numbers separated by exactly one increment (i.e., values that form a strictly increasing sequence with step 1) are written as `"first-last"`; non-consecutive numbers are written individually. All groups must be separated by commas, with no trailing comma or extra spaces. Duplicate numbers are ignored (each value appears only once in the result). The output must be in ascending order. For example, input `"3,1,2,4,7,8,10"` yields `"1-4,7-8,10"`. Input `"5,5,6"` yields `"5-6"`. Input `"1"` yields `"1"`. The function must be robust to leading/trailing commas or spaces. The input contains at least one integer, and all integers are positive ( ≥ 1).
#include <cassert>
#include <string>

int main() {
    // Single number
    assert(compressRanges("1") == "1");
    // Consecutive range
    assert(compressRanges("1,2,3,4") == "1-4");
    // Mixed ranges and singles
    assert(compressRanges("3,1,2,4,7,8,10") == "1-4,7-8,10");
    // Duplicates collapsed
    assert(compressRanges("5,5,6,6,7") == "5-7");
    // Leading/trailing commas and spaces
    assert(compressRanges(" ,2, 4,5, 9 , ") == "2,4-5,9");
    // Large gap between groups
    assert(compressRanges("1,100,101,200") == "1,100-101,200");
    // All consecutive long run
    assert(compressRanges("10,12,11,9") == "9-12");
    // Single element after duplicates
    assert(compressRanges("42,42,42") == "42");
    // Non-consecutive singles
    assert(compressRanges("3,1,2") == "1-3");
    // Two separate pairs
    assert(compressRanges("1,2,4,5") == "1-2,4-5");
    return 0;
}
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

// Compress sorted unique positive integers into ranges of consecutive numbers.
// Input: comma-separated string of positive integers (may contain duplicates).
// Output: comma-separated ranges, e.g., "1-4,7-8,10".
std::string compressRanges(const std::string& s) {
    std::vector<int> nums;
    std::stringstream ss(s);
    int num;
    char comma;
    while (ss >> num) {
        nums.push_back(num);
        ss >> comma; // skip comma or fail at end
    }

    std::sort(nums.begin(), nums.end());
    nums.erase(std::unique(nums.begin(), nums.end()), nums.end());

    std::string result;
    size_t start = 0; // index of beginning of current run
    for (size_t i = 0; i < nums.size(); ++i) {
        // If at last element or next is not consecutive, close the group
        if (i == nums.size() - 1 || nums[i + 1] != nums[i] + 1) {
            if (!result.empty()) result += ",";
            if (start == i) {
                result += std::to_string(nums[i]);
            } else {
                result += std::to_string(nums[start]) + "-" + std::to_string(nums[i]);
            }
            start = i + 1;
        }
    }
    return result;
}
// The solution needs to parse comma-separated integers from the input string, remove duplicates, sort them, and then group consecutive integers where each next value equals the previous plus one. First, use a `std::stringstream` to extract integers and skip commas (by reading a character after each integer). Store them in a `std::vector<int>`, then sort and erase duplicates using `std::unique`. After that, iterate through the unique sorted vector, tracking the start index of the current run. A run continues as long as `nums[i+1] == nums[i] + 1`. When the run breaks, if the run length is at least 2 (i.e., start index differs from current index), output `"start-current"`; otherwise output just `"start"`. Append a comma after each group except the last. Edge cases: single element, all duplicates, long consecutive runs, and input with extra commas/spaces (parsing ignores non-numeric characters). Time complexity is O(n log n) due to sorting, where n is the number of integers. Space complexity is O(n) for the vector. The function returns a std::string and must be `const`-correct.
