Write a C++ function named `convertAndSum` that takes two strings, `num1` and `num2`, each representing a non-negative integer with no leading zeros (except the number "0" itself), and returns a string representing the sum of the two integers. The function must not use `std::stoi`, `std::stol`, `std::istringstream`, or any standard library conversion functions; instead, you must manually parse each digit using character arithmetic and manually convert the resulting sum back to a string. Assume the input strings contain only digits and are non-empty.

#include <cassert>
#include <string>

// Assume the function is declared above or included here.
int main() {
    assert(convertAndSum("0", "0") == "0");
    assert(convertAndSum("1", "2") == "3");
    assert(convertAndSum("10", "20") == "30");
    assert(convertAndSum("99", "1") == "100");
    assert(convertAndSum("123", "456") == "579");
    assert(convertAndSum("9999", "1") == "10000");
    assert(convertAndSum("5", "5") == "10");
    assert(convertAndSum("42", "0") == "42");
    assert(convertAndSum("0", "7") == "7");
    assert(convertAndSum("100", "200") == "300");
    return 0;
}

#include <string>
#include <algorithm>

// Manually convert a non-negative integer string to int.
int stringToInt(const std::string& s) {
    int n = 0;
    for (char ch : s) {
        n = n * 10 + (ch - '0');
    }
    return n;
}

// Manually convert a non-negative int to a string.
std::string intToString(int n) {
    if (n == 0) return "0";
    std::string result;
    while (n > 0) {
        result += static_cast<char>('0' + n % 10);
        n /= 10;
    }
    std::reverse(result.begin(), result.end());
    return result;
}

// Return the string representation of the sum of two digit strings.
std::string convertAndSum(const std::string& num1, const std::string& num2) {
    int a = stringToInt(num1);
    int b = stringToInt(num2);
    int sum = a + b;
    return intToString(sum);
}

// The solution requires two helper operations: converting a digit string to an integer and converting an integer back to a string, both done manually. For string-to-int, iterate over each character, multiply the current result by 10 and add the digit value (`s[i] - '0'`). For int-to-string, repeatedly extract the last digit using `% 10`, append the corresponding character, then divide by 10, and finally reverse the collected string. Edge cases include the number "0" (which yields an empty string if the loop condition is `while (n > 0)`, so handle it by returning "0" when the integer is zero), and potential overflow if inputs are large—but within the typical `int` range, this is fine. The main algorithm simply parses both strings, adds the integers, and converts the result back to a string. Time complexity is O(len(num1) + len(num2) + len(result)), space complexity is O(len(result)) for the returned string, plus O(1) auxiliary space.
