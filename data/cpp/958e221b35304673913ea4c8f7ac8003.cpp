Given a string `nums` consisting only of digits and a length of at least 1, write a C++ function that returns the largest possible substring of length 3 that is composed of the same digit (e.g., "999", "444", "000"). If no such three consecutive identical digits exist, return an empty string. The function must be case-sensitive (only digits), preserve leading zeros in the returned substring (e.g., "000" is a valid result), and handle strings shorter than 3 gracefully by returning an empty string.
The problem can be solved by direct search: iterate through the input string and check every consecutive triplet of characters. Since the valid substrings are limited to the 10 possible triplets from "999" down to "000", we can check each such triplet in order of decreasing numeric value and return the first one found as a substring. Alternatively, a more efficient single-pass scan can check if `nums[i] == nums[i+1] == nums[i+2]`; because we want the *largest* digit substring, we should update the result whenever a valid triplet is found and its digit is greater than the current result's first character. This avoids generating all 10 triplet strings and directly gives the largest. Edge cases: string length < 3 returns "", no valid triplet returns "", and "000" is a valid return (empty string must not be returned). Time complexity is O(n) for a single pass, with O(1) auxiliary space. The straightforward approach scanning all possible triplets and comparing to a precomputed list is O(10n) which simplifies to O(n) and is also acceptable.
#include <string>

// Returns the largest substring of length 3 made of the same digit,
// or an empty string if no such substring exists.
std::string largestGoodInteger(const std::string& nums) {
    if (nums.size() < 3) {
        return "";
    }
    std::string result = "";
    for (std::size_t i = 0; i + 2 < nums.size(); ++i) {
        if (nums[i] == nums[i+1] && nums[i+1] == nums[i+2]) {
            // Since we iterate left to right, we need to keep the
            // largest digit substring. If current digit is greater
            // than the first char of the existing result, replace it.
            if (result.empty() || nums[i] > result[0]) {
                result = nums.substr(i, 3);
            }
        }
    }
    return result;
}
#include <cassert>
#include <string>

// Declaration of the tested function (already provided above)
std::string largestGoodInteger(const std::string& nums);

int main() {
    assert(largestGoodInteger("6777133339") == "777");
    assert(largestGoodInteger("2300019") == "000");
    assert(largestGoodInteger("42352338") == "");
    assert(largestGoodInteger("111") == "111");
    assert(largestGoodInteger("123") == "");
    assert(largestGoodInteger("999999") == "999");
    assert(largestGoodInteger("000000") == "000");
    assert(largestGoodInteger("0123456789") == "");
    assert(largestGoodInteger("122333444455555") == "555");
    assert(largestGoodInteger("9876543210") == "");
    return 0;
}
