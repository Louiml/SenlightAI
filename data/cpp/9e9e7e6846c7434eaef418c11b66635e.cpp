/*
Given a positive whole number, write a C++ function that returns the string `"TWEET"` if the number of digits in the number is less than or equal to 3, and returns `"MUTE"` if the number of digits is greater than 3. The function should accept the number as a `std::string` (which may contain leading zeros, but will always be a non-empty sequence of digits from '0' to '9'). For example, a string like `"0012"` has 4 digits so the result is `"MUTE"`, while `"120"` has 3 digits so the result is `"TWEET"`. The function must be pure, deterministic, and not perform any I/O.
*/

#include <string>

// Return "TWEET" if the digit count is <= 3, otherwise "MUTE".
// The input is a non-empty string of digits only.
std::string classifyTweet(const std::string& number) {
    const std::size_t digitCount = number.size();  // number of digits in the string
    if (digitCount <= 3) {
        return "TWEET";
    } else {
        return "MUTE";
    }
}

#include <cassert>
#include <string>

std::string classifyTweet(const std::string& number);

int main() {
    // Exact limit cases
    assert(classifyTweet("") == "TWEET");       // empty string (not normally given, but safe)
    assert(classifyTweet("1") == "TWEET");      // 1 digit
    assert(classifyTweet("12") == "TWEET");     // 2 digits
    assert(classifyTweet("123") == "TWEET");    // 3 digits (limit)
    assert(classifyTweet("1234") == "MUTE");    // 4 digits
    assert(classifyTweet("12345") == "MUTE");   // many digits

    // Leading zeros count as digits
    assert(classifyTweet("000") == "TWEET");    // 3 digits
    assert(classifyTweet("0000") == "MUTE");    // 4 digits
    assert(classifyTweet("007") == "TWEET");    // 3 digits
    assert(classifyTweet("0100") == "MUTE");    // 4 digits

    // Large number
    assert(classifyTweet("123456789") == "MUTE");

    // Single zero
    assert(classifyTweet("0") == "TWEET");

    return 0;
}

// The task is straightforward: count the number of characters in the input string (which represents the number of digits, since the string contains only digit characters), and compare that count against a threshold of 3. If the count is ≤ 3, return `"TWEET"`; otherwise return `"MUTE"`. Edge cases: empty string is not allowed per spec, but if it were given, its size is 0 which would be ≤ 3, so it would return `"TWEET"` — acceptable anyway. Leading zeros are naturally counted as digits, which matches the requirement. The solution uses `std::string::size()` which returns `size_t` (unsigned), so comparing with integer literal 3 is safe. Time complexity is O(n) where n is the length of the input string (just to compute length, actually O(n) is required to read the string into memory, but `size()` is O(1) after construction; however, we can say the function itself is O(1) as it only calls `size()`. Overall auxiliary space is O(1).
