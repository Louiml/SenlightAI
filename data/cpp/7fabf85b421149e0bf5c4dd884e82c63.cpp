// Given a non-empty string `num` consisting only of digits (0-9), write a C++ function that returns the largest odd number that can be formed by taking a prefix of `num` (i.e., a substring starting at index 0 and ending at some index). If no odd prefix exists, return an empty string. The input may contain leading zeros, and the output must preserve those leading zeros if they are part of a valid prefix. For example, for `"4206"` the result is `""`, and for `"52"` (note: 5 and 52 are both odd, but 5 is the largest odd prefix? Actually, "52" is even, "5" is odd, so return "5"). The function should handle lengths up to 10^5 efficiently, and must not use any extra space proportional to the input size (only O(1) auxiliary).
#include <cassert>
#include <string>

// Declaration for testing (include the solution above or link it)
std::string largestOddNumber(const std::string& num);

int main() {
    // Basic cases
    assert(largestOddNumber("52") == "5");
    assert(largestOddNumber("4206") == "");
    assert(largestOddNumber("35427") == "35427"); // entire number is odd
    assert(largestOddNumber("1234") == "123"); // last odd digit at index 2
    
    // Edge cases: leading zeros and single digit
    assert(largestOddNumber("001") == "001");
    assert(largestOddNumber("8") == "");
    assert(largestOddNumber("9") == "9");
    
    // Mixed cases where longest prefix is not the whole number
    assert(largestOddNumber("2310") == "231"); // '1' at index 2
    assert(largestOddNumber("24680") == ""); // all even
    assert(largestOddNumber("13579") == "13579"); // all odd
    
    // Large-like string (just testing correctness, not performance)
    assert(largestOddNumber("1000000000000000000000000000001") == "1000000000000000000000000000001"); // ends with 1
    
    return 0;
}
#include <string>

// Return the largest odd prefix of the given digit string. If no odd prefix exists, return an empty string.
std::string largestOddNumber(const std::string& num) {
    // Scan from the rightmost character to the left.
    for (int i = static_cast<int>(num.size()) - 1; i >= 0; --i) {
        // Check if the digit is odd (1,3,5,7,9).
        if ((num[i] - '0') % 2 != 0) {
            // The prefix ending at i is the longest odd prefix.
            return num.substr(0, i + 1);
        }
    }
    // No odd digit found.
    return "";
}
// The key observation is that an odd number must end in an odd digit (1,3,5,7,9). Since any prefix ending at index `i` is `num.substr(0, i+1)`, we only need to find the rightmost odd digit in the string. If we scan from the end (`i = n-1` down to `0`), the first time we encounter a digit whose numeric value (`num[i] - '0'`) is odd, the prefix ending at that index is guaranteed to be the largest odd prefix (because any longer prefix ending after that would include an even last digit, making it even, and any shorter prefix would be numerically smaller or equal? Actually, consider "102": rightmost odd is at index 0 (digit '1'), prefix "1" is odd; "10" is even, "102" is even. So "1" is the only odd prefix. Consider "5232": rightmost odd is at index 0 (digit '5'), prefix "5", "52", "523", "5232" — only "5" is odd. So scanning from the end ensures we pick the longest odd prefix? Wait, "123" has rightmost odd at index 2 (digit '3'), prefix "123" is odd. That is the longest prefix. But if we have "5310", rightmost odd is at index 1 (digit '3'), prefix "53" is odd, "5" is also odd but shorter. The largest odd number is "53" (since "53" > "5"). So the longest odd prefix is indeed the one ending at the rightmost odd digit. Because if there is a longer prefix that ends with an even digit, that prefix is even, so not valid. Any valid odd prefix must end with an odd digit. The longest such prefix is the one ending at the last (rightmost) odd digit in the string. Therefore, scanning from the end until we find an odd digit, return `num.substr(0, i+1)`. If none found, return empty string. Edge cases: empty input? The task says non-empty, but if allowed, return empty. Leading zeros: e.g., "001" – rightmost odd at index 2, prefix "001" is odd (value 1), so return "001". Time complexity O(n) because one scan from the end, space O(1) for the function itself (the returned substring uses O(k) where k is the length of the prefix, but that is part of the output, not auxiliary). Auxiliary space is O(1).
