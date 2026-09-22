// You are given an even number of strings, each consisting only of lowercase English letters. XOR all characters at the same position across all strings (e.g., for position 0, XOR the first character of every string together; for position 1, XOR the second characters, etc.), and also XOR the lengths of all strings together. If the XOR of all lengths is zero, return the string "-1". Otherwise, return a string whose length equals that XOR-of-lengths value, built by taking the XOR character results for positions 0 through that length-1. Note that some strings may be shorter than others; treat missing positions as contributing no character (i.e., effectively a neutral value that does not affect the XOR at that position, which can be implemented by only XORing characters that exist). The input always contains at least two strings, and the number of strings is even. You must implement a function that takes a `std::vector<std::string>` and returns a `std::string` per this rule.
The core idea is to maintain an array (or vector) of character XOR accumulators for each position, indexed from 0 upward. Initially, all accumulator values are zero (or the null character `'\0'`). For each string, iterate over its characters and XOR each character into the accumulator at the same index. Simultaneously, maintain a separate cumulative XOR of all string lengths. After processing all strings, if the length-XOR is zero, return "-1". Otherwise, the result length is exactly that length-XOR value. Build the result by taking the first `resLen` characters from the accumulator array, where each is the XOR of all characters that appeared at that position across all strings. Important edge cases: if a string is shorter, positions beyond its length simply don’t contribute (which is fine because XOR with `'\0'` is the identity). Since the number of strings is even, the length-XOR can be any positive integer, but if it’s zero (which happens when the XOR of all lengths equals 0), the specification says to return "-1". Also, note that in the original code, the accumulator array is of fixed size 20, but here we can use a dynamic vector sized to the maximum string length to avoid overflow. Time complexity is O(total number of characters across all strings), and space complexity is O(max string length) for the accumulator, plus O(1) extra for the length XOR.
#include <string>
#include <vector>

// Given a vector of strings, XOR characters at same positions and XOR lengths.
// Returns "-1" if XOR of lengths is 0, otherwise builds the result string.
std::string xorStrings(const std::vector<std::string>& strings) {
    int maxLen = 0;
    int lenXor = 0;
    for (const auto& s : strings) {
        lenXor ^= static_cast<int>(s.size());
        if (static_cast<int>(s.size()) > maxLen) {
            maxLen = static_cast<int>(s.size());
        }
    }

    if (lenXor == 0) {
        return "-1";
    }

    // Accumulator for each position, initialized to 0 (null char).
    std::string xorAccum(maxLen, '\0');
    for (const auto& s : strings) {
        for (int i = 0; i < static_cast<int>(s.size()); ++i) {
            xorAccum[i] ^= s[i];
        }
    }

    // Build result of length lenXor.
    return xorAccum.substr(0, lenXor);
}
#include <cassert>
#include <string>
#include <vector>

// Include the solution function here (or link it).
std::string xorStrings(const std::vector<std::string>& strings);

int main() {
    // Basic even number of strings.
    assert(xorStrings({"ab", "cd"}) == 
           std::string(1, 'a' ^ 'c') + std::string(1, 'b' ^ 'd'));
    // XOR of lengths: 2^2 = 0 -> -1.
    assert(xorStrings({"aa", "bb"}) == "-1");
    // Different lengths: 3^1 = 2, result length 2.
    std::string s1 = "abc";
    std::string s2 = "x";
    int lenXor = 3 ^ 1;
    std::string expected(lenXor, '\0');
    expected[0] = 'a' ^ 'x';
    expected[1] = 'b';
    assert(xorStrings({s1, s2}) == expected);
    // Even more strings with mixed lengths: lengths 2,3,1,2 -> XOR=2^3^1^2=2.
    std::vector<std::string> v = {"ab", "cde", "f", "gh"};
    int lx = 2 ^ 3 ^ 1 ^ 2; // 2
    std::string exp2(lx, '\0');
    exp2[0] = 'a' ^ 'c' ^ 'f' ^ 'g';
    exp2[1] = 'b' ^ 'd' ^ 'h';
    assert(xorStrings(v) == exp2);
    // Single-character strings (length 1 each, even count)
    std::vector<std::string> v2 = {"p", "q", "r", "s"};
    int lx2 = 1 ^ 1 ^ 1 ^ 1; // 0 -> -1
    assert(xorStrings(v2) == "-1");
    // Two strings with same length but not zero length xor (e.g., 2 and 4 => 6)
    std::vector<std::string> v3 = {"ab", "cdef"};
    int lx3 = 2 ^ 4; // 6
    std::string exp3(lx3, '\0');
    exp3[0] = 'a' ^ 'c';
    exp3[1] = 'b' ^ 'd';
    exp3[2] = 'e';
    exp3[3] = 'f';
    exp3[4] = '\0';
    exp3[5] = '\0';
    assert(xorStrings(v3) == exp3);
    // Long strings to ensure no overflow.
    std::string long1(100, 'a');
    std::string long2(100, 'a');
    assert(xorStrings({long1, long2}) == "-1"); // 100^100=0
    std::string long3(100, 'a');
    std::string long4(101, 'b');
    int lx4 = 100 ^ 101; // 1
    std::string exp4(lx4, '\0');
    exp4[0] = 'a' ^ 'b';
    assert(xorStrings({long3, long4}) == exp4);
    return 0;
}
