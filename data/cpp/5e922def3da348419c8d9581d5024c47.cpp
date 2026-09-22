Write a C++ function that takes a string `num` representing a non-negative integer with no leading zeros (except the number "0" itself) and returns the largest odd-numbered substring that can be formed by taking a prefix of the original string. In other words, find the longest prefix of `num` whose numeric value is odd. If no odd prefix exists, return an empty string. The input string will contain only digits ('0'–'9') and will have length between 1 and 100,000. The output should be a string representing that odd number (with no leading zeros, except "0" itself for the number zero; however, since zero is even, it will never be returned). Your function must handle very large inputs efficiently.

The key observation is that a number is odd if and only if its last digit is odd (1, 3, 5, 7, or 9). Since we are looking for the largest odd prefix, we should scan the string from right to left and find the rightmost position where the digit at that position is odd. The prefix ending at that position is the longest prefix whose last digit is odd, and therefore the largest odd number that can be formed by removing trailing digits. If no odd digit exists at all, then no prefix is odd, and the result is an empty string. This greedy approach works because any prefix ending before the rightmost odd digit would be shorter, and any prefix extending beyond that rightmost odd digit would end with an even digit and therefore be even. Edge cases: (1) a single-digit odd number like "7" returns "7"; (2) a single-digit even number like "2" returns ""; (3) a string with only even digits like "2468" returns ""; (4) a very long string with an odd digit at the end returns the entire string; (5) a string like "420" where the rightmost odd digit is at position 0 ("4" is even, "2" is even, "0" is even) returns "". Time complexity is O(n) for scanning the string once, and space complexity is O(n) for the returned substring copy (though we could return using `substr`, which copies the prefix).

#include <string>

// Given a string representing a non-negative integer, return the longest
// prefix that is an odd number. If no such prefix exists, return an empty string.
std::string largestOddPrefix(const std::string& num) {
    // Scan from the end to find the rightmost odd digit.
    for (int i = static_cast<int>(num.size()) - 1; i >= 0; --i) {
        // Check if the digit is odd using its ASCII value.
        if ((num[i] - '0') % 2 != 0) {
            // Return the prefix from start to this position inclusive.
            return num.substr(0, i + 1);
        }
    }
    // No odd digit found.
    return "";
}

#include <cassert>
#include <string>

// Function declaration (included for completeness; actual definition above).
std::string largestOddPrefix(const std::string& num);

int main() {
    // Basic odd and even cases.
    assert(largestOddPrefix("52") == "5");
    assert(largestOddPrefix("4206") == "");
    assert(largestOddPrefix("35427") == "35427");
    
    // Single digit cases.
    assert(largestOddPrefix("7") == "7");
    assert(largestOddPrefix("2") == "");
    
    // Large number with odd at the end.
    assert(largestOddPrefix("123456789") == "123456789");
    
    // Large number with even at the end but odd earlier.
    assert(largestOddPrefix("123456780") == "1234567");
    
    // Only even digits.
    assert(largestOddPrefix("24680") == "");
    
    // Zero handling (zero is even, so empty).
    assert(largestOddPrefix("0") == "");
    
    // Leading zeros in middle are fine (input has no leading zeros except "0" itself).
    assert(largestOddPrefix("101") == "101");
    
    // Long string where rightmost odd is at the end after many evens.
    assert(largestOddPrefix("222222222222222222223") == "222222222222222222223");
    
    return 0;
}
