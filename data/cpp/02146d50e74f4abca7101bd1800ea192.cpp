// Write a C++ function that takes a vector of strings where each string represents a non-negative integer, along with three integer parameters `k`, `s`, and `l`. For each string in the vector, extract a substring starting at position `s` (0-based) with length `l`, convert that substring to an integer, and include that integer in the result vector only if it is strictly greater than `k`. The function should return a new vector<int> containing those qualifying integers in the same order as they appear in the input. You may assume every string is long enough to safely extract the specified substring, and the substring consists only of digit characters (so conversion is always valid). The function must not modify the input vector.

The solution iterates through each string in the input vector. For each string, it extracts the relevant portion using `substr(s, l)` and converts it to an integer with `std::stoi`. Since the substring is guaranteed to be numeric and within bounds, no error handling for conversion or out-of-range is necessary. After conversion, a simple comparison `num > k` determines whether to append the value to the result. The algorithm processes each element exactly once, so the time complexity is O(n * l) where n is the number of strings and l is the fixed substring length (conversion cost is proportional to l). The space complexity is O(m) where m is the number of qualifying integers stored in the result vector, plus O(1) auxiliary space for loop variables and the temporary integer. Edge cases include: if no string yields an integer greater than `k`, the function returns an empty vector; if `l` is 0, `stoi("")` is undefined behavior, but the task guarantees valid substrings; duplicate values are stored separately because each string is handled independently. Constant correctness is maintained by taking the input vector by const reference.

#include <string>
#include <vector>

// Extract substrings from strings, convert to int, and return those > k.
std::vector<int> extractAndFilter(const std::vector<std::string>& intStrs,
                                  int k, int s, int l) {
    std::vector<int> result;
    
    for (const std::string& str : intStrs) {
        int value = std::stoi(str.substr(s, l));
        if (value > k) {
            result.push_back(value);
        }
    }
    
    return result;
}

#include <cassert>
#include <vector>
#include <string>

// Function declaration (prototype) for testing.
std::vector<int> extractAndFilter(const std::vector<std::string>& intStrs,
                                  int k, int s, int l);

int main() {
    // Basic case: filter values > 5
    std::vector<std::string> data1 = {"012345", "123456", "234567"};
    std::vector<int> result1 = extractAndFilter(data1, 5, 1, 3);
    assert((result1 == std::vector<int>{123, 234, 345}));

    // No qualifying values
    std::vector<std::string> data2 = {"111", "222", "333"};
    std::vector<int> result2 = extractAndFilter(data2, 500, 0, 3);
    assert(result2.empty());

    // Some qualify, some don't
    std::vector<std::string> data3 = {"100", "200", "300"};
    std::vector<int> result3 = extractAndFilter(data3, 150, 0, 3);
    assert((result3 == std::vector<int>{200, 300}));

    // Edge: k is 0 and all values positive
    std::vector<std::string> data4 = {"0001", "0002"};
    std::vector<int> result4 = extractAndFilter(data4, 0, 0, 4);
    assert((result4 == std::vector<int>{1, 2}));

    // Edge: substring starts at beginning, full length
    std::vector<std::string> data5 = {"42", "7", "99"};
    std::vector<int> result5 = extractAndFilter(data5, 50, 0, 2);
    assert((result5 == std::vector<int>{99}));

    // Edge: l equals 1, single digit extraction
    std::vector<std::string> data6 = {"abc1", "xyz9"};
    std::vector<int> result6 = extractAndFilter(data6, 5, 3, 1);
    assert((result6 == std::vector<int>{9}));

    // All values qualify, check order preserved
    std::vector<std::string> data7 = {"001", "002", "003"};
    std::vector<int> result7 = extractAndFilter(data7, 0, 0, 3);
    assert((result7 == std::vector<int>{1, 2, 3}));

    // Larger s with trailing characters
    std::vector<std::string> data8 = {"start123end", "start456end"};
    std::vector<int> result8 = extractAndFilter(data8, 200, 5, 3);
    assert((result8 == std::vector<int>{456}));

    // Mixed lengths but correct substring extraction
    std::vector<std::string> data9 = {"ab12", "cd34", "ef56"};
    std::vector<int> result9 = extractAndFilter(data9, 30, 2, 2);
    assert((result9 == std::vector<int>{34, 56}));

    // Ensure input vector is not modified (implicitly checked by const ref)
    std::vector<std::string> original = {"100", "200"};
    std::vector<std::string> copy = original;
    extractAndFilter(original, 150, 0, 3);
    assert(original == copy);

    return 0;
}
