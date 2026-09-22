Write a C++ function `addBinary` that takes two non-empty strings representing binary numbers (containing only '0' and '1', with no leading zeros except for "0" itself) and returns their sum as a binary string. The binary numbers may have different lengths, including very large lengths (up to 10^4 digits), so you cannot convert them to built-in integer types. The function should perform the addition digit by digit using carry propagation, and the result must also contain only '0' and '1' with no leading zeros (except for the result "0"). You may modify the input strings inside the function, but the original arguments should not be modified from the caller's perspective (i.e., pass by `const` reference or copy them internally). Your solution must handle all edge cases, including unequal lengths, carries that propagate beyond the most significant digit, and the case where both inputs are "0".
The main algorithm is a manual binary addition using string manipulation. First, identify which input string is longer (if equal, either works) and make a copy of it to store the result. Reverse both strings so that we can process from the least significant bit (rightmost) to the most significant bit. Initialize a carry variable to 0. Iterate through each position up to the length of the longer string. For positions where the shorter string has a digit, add that digit's numeric value (obtained by subtracting '0') to the current longer digit. Then add the carry, compute the new digit as `(current_sum) % 2` converted back to a character, and update the carry as `(current_sum) / 2`. After processing all digits of the longer string, if a carry remains, append additional digits by repeatedly taking carry modulo 2 and dividing carry by 2 until carry becomes 0 (this handles cases like "1" + "1" = "10"). Finally, reverse the result string back to normal order and return it. Important edge cases: unequal lengths (the shorter string is exhausted, so we only add carry to remaining digits of the longer string), no leading zeros in the result (by using the copy of the longer string, which already has no leading zeros, and if the result ends up with a leading zero only when both inputs are "0"), and extremely long inputs (we avoid integer overflow by never converting to numeric types). Time complexity is O(max(n, m) + number of extra carry digits), which is O(max(n, m)) overall. Space complexity is O(max(n, m)) for the result string and any copies, plus O(1) auxiliary for variables.
#include <string>
#include <algorithm>

// Add two binary strings and return their sum as a binary string.
// The inputs are passed by const reference, and the function works
// on copies internally to avoid modifying the original arguments.
std::string addBinary(const std::string& a, const std::string& b) {
    // Copy the longer and shorter strings.
    std::string longer = (a.size() >= b.size()) ? a : b;
    std::string shorter = (a.size() >= b.size()) ? b : a;

    // Reverse both strings to process least significant bits first.
    std::reverse(longer.begin(), longer.end());
    std::reverse(shorter.begin(), shorter.end());

    // Add the shorter digits directly into the longer string.
    for (size_t i = 0; i < shorter.size(); ++i) {
        longer[i] += shorter[i] - '0'; // Convert char to int, add, store as char (may become '2' etc.)
    }

    // Propagate carry through all digits of the longer string.
    char carry = 0;
    size_t idx = 0;
    const size_t length = longer.size();
    while (idx < length) {
        longer[idx] += carry;          // Add carry to current digit
        carry = (longer[idx] - '0') / 2;   // New carry (0 or 1 since binary)
        longer[idx] = (longer[idx] - '0') % 2 + '0'; // Set digit to 0 or 1
        ++idx;
    }

    // If there is still a carry, append additional bits.
    while (carry > 0) {
        longer.push_back(static_cast<char>('0' + (carry % 2)));
        carry = static_cast<char>(carry / 2);
    }

    // Reverse back to normal order.
    std::reverse(longer.begin(), longer.end());

    return longer;
}
#include <cassert>
#include <string>

// The function under test (declaration only, actual code above in solution).
std::string addBinary(const std::string& a, const std::string& b);

int main() {
    // Basic cases
    assert(addBinary("0", "0") == "0");
    assert(addBinary("1", "0") == "1");
    assert(addBinary("0", "1") == "1");
    assert(addBinary("1", "1") == "10");
    assert(addBinary("10", "11") == "101");
    assert(addBinary("111", "1") == "1000");

    // Unequal lengths
    assert(addBinary("1101", "111") == "10100");
    assert(addBinary("1010", "1011") == "10101");

    // Larger numbers with leading no issue
    assert(addBinary("111111", "111111") == "1111110");
    assert(addBinary("10101010101010101010", "11001100110011001100") == "101111101111101111110");

    // Very long input (simulate large length)
    std::string longA(10000, '1');
    std::string longB(10000, '1');
    std::string result = addBinary(longA, longB);
    // Expected: 10000 zeros followed by '1' at the front : "1" + 10000 zeros
    assert(result.size() == 10001);
    assert(result[0] == '1');
    for (size_t i = 1; i < result.size(); ++i) {
        assert(result[i] == '0');
    }

    return 0;
}
