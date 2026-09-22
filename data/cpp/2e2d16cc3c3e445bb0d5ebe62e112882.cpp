Write a C++ function that takes two binary strings (consisting only of characters '0' and '1') and returns their sum as a binary string (also containing only '0' and '1'). The function must correctly handle inputs of different lengths, including cases where one string is empty, and must properly manage any carry that propagates beyond the most significant bit. Do not use any built-in binary conversion utilities; implement the addition manually by processing the strings from the least significant bit (rightmost) to the most significant bit (leftmost). The resulting binary string should have no leading zeros, except when the result is exactly "0".
The standard algorithm mirrors manual binary addition. Since binary addition proceeds from the least significant bit, reverse both input strings so we can iterate from index 0 (now representing the rightmost bits). Maintain a `carry` variable initialized to 0. For each position up to the length of the shorter string, sum the two bits plus the carry. Since each bit is a character '0' or '1', convert to integer by subtracting '0'. If the sum is 2 or 3, the resulting bit is the sum modulo 2 (i.e., subtract 2 if >=2) and the carry becomes 1; otherwise the bit is the sum and carry becomes 0. After processing the common length, handle the remaining digits of the longer string similarly, adding only the carry to each remaining bit. Finally, if a carry of 1 remains after processing all digits, append an extra '1'. Reverse the result back to normal order and return it. Edge cases include: one string being empty (treat as carry-only addition, but the problem typically guarantees non-empty strings; still handle robustly), inputs like "0" and "0" returning "0", and inputs like "111" and "1" producing "1000". Time complexity is O(max(n,m)) where n and m are the lengths of the inputs, and space complexity is O(max(n,m)) for the reversed and result strings.
#include <string>
#include <algorithm>

// Returns the sum of two binary strings (e.g., "11" + "1" -> "100").
// Assumes inputs contain only '0' and '1' characters.
std::string addBinaryStrings(const std::string& a, const std::string& b) {
    // Reverse both strings to process from least significant bit.
    std::string aRev = a;
    std::string bRev = b;
    std::reverse(aRev.begin(), aRev.end());
    std::reverse(bRev.begin(), bRev.end());

    std::string resultRev;
    int i = 0;
    int carry = 0;

    // Process common length.
    while (i < aRev.size() && i < bRev.size()) {
        int bitSum = (aRev[i] - '0') + (bRev[i] - '0') + carry;
        char bit = (bitSum % 2) + '0';
        resultRev.push_back(bit);
        carry = bitSum / 2;
        ++i;
    }

    // Process remaining digits of a if longer.
    while (i < aRev.size()) {
        int bitSum = (aRev[i] - '0') + carry;
        char bit = (bitSum % 2) + '0';
        resultRev.push_back(bit);
        carry = bitSum / 2;
        ++i;
    }

    // Process remaining digits of b if longer.
    while (i < bRev.size()) {
        int bitSum = (bRev[i] - '0') + carry;
        char bit = (bitSum % 2) + '0';
        resultRev.push_back(bit);
        carry = bitSum / 2;
        ++i;
    }

    // If final carry remains, append '1'.
    if (carry) {
        resultRev.push_back('1');
    }

    // Reverse to get final result.
    std::reverse(resultRev.begin(), resultRev.end());
    return resultRev;
}
#include <cassert>
#include <string>

// The solution function is declared above; here we test it.
int main() {
    // Basic cases
    assert(addBinaryStrings("11", "1") == "100");
    assert(addBinaryStrings("0", "0") == "0");
    assert(addBinaryStrings("1", "0") == "1");
    assert(addBinaryStrings("0", "1") == "1");
    
    // Different lengths
    assert(addBinaryStrings("1010", "101") == "1111");
    assert(addBinaryStrings("111", "1") == "1000");
    assert(addBinaryStrings("1", "111") == "1000");
    
    // Longer carry propagation
    assert(addBinaryStrings("1111", "1") == "10000");
    assert(addBinaryStrings("10101", "01010") == "11111"); // Note leading zeros in input are allowed
    assert(addBinaryStrings("110", "110") == "1100");
    
    // Large simple cases
    assert(addBinaryStrings("1000000000", "1000000000") == "10000000000");
    assert(addBinaryStrings("11111111", "00000001") == "100000000");
    
    return 0;
}
