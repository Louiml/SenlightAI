// Write a C++ function named `stringToInteger` that takes a constant reference to a `std::string` and returns an `int`. The function must manually convert the string to an integer, mimicking the behaviour of `std::stoi` but with stricter rules: it should skip leading whitespace (spaces only, not tabs/newlines), optionally handle a single leading `+` or `-` sign, and then parse consecutive digit characters until a non-digit is encountered. If the parsed value exceeds the 32-bit signed integer range, return `INT_MAX` for positive overflow and `INT_MIN` for negative overflow. If the string contains no digits after optional sign (or is empty after trimming), return `0`. The function must not use any standard parsing helpers like `std::stoi`, `std::atoi`, or string streams — only manual character inspection and arithmetic.

// The algorithm processes the string in three sequential phases: trimming, sign detection, and digit accumulation with overflow checking. First, advance an index `i` while the character is a space. Then, check if `s[i]` is `+` or `-`; if so, set a `sign` variable to `1` or `-1` and increment `i`. In the digit accumulation loop, for each character, first verify it is a digit using `std::isdigit` (cast to unsigned char to avoid undefined behaviour). If it is not a digit, break and return the current accumulated answer. Compute the digit value as `(s[i] - '0') * sign`. Before updating the answer, perform overflow checks: for positive sign, if `ans > INT_MAX/10` or (`ans == INT_MAX/10` and the new digit `(s[i]-'0')` is greater than `INT_MAX % 10`), return `INT_MAX`. For negative sign, since `ans` is negative and digits are negative, check if `ans < INT_MIN/10` or (`ans == INT_MIN/10` and the digit `(s[i]-'0')` (positive digit value) is less than `-(INT_MIN % 10)` — but here we use the negative digit: if `ans == INT_MIN/10` and `digit < (INT_MIN % 10)` because `INT_MIN % 10` is negative (e.g., -8). Then update `ans = ans * 10 + digit` using a `long long` multiplication to avoid intermediate overflow. Edge cases: empty string or only spaces returns 0; string like `"words"` returns 0; string like `"+-12"` returns 0 because after seeing `+`, the next char is `-` which is not a digit, so the loop never accumulates. Time complexity is O(n) where n is the length of input, and space complexity is O(1).

#include <string>
#include <climits>
#include <cctype>

// Convert a string to an integer with overflow clamping and manual parsing.
int stringToInteger(const std::string& s) {
    int i = 0;
    
    // Trim leading spaces (only ' ', not tabs etc.)
    while (i < static_cast<int>(s.size()) && s[i] == ' ') {
        ++i;
    }
    
    // Parse optional sign
    int sign = 1;
    if (i < static_cast<int>(s.size()) && (s[i] == '+' || s[i] == '-')) {
        if (s[i] == '-') {
            sign = -1;
        }
        ++i;
    }
    
    int ans = 0;
    while (i < static_cast<int>(s.size())) {
        // Stop if the current character is not a digit
        if (!std::isdigit(static_cast<unsigned char>(s[i]))) {
            break;
        }
        
        int digit_value = s[i] - '0';
        int dig = digit_value * sign;
        
        // Check for overflow before multiplying
        if (sign == 1 && (ans > INT_MAX / 10 || (ans == INT_MAX / 10 && digit_value > INT_MAX % 10))) {
            return INT_MAX;
        }
        if (sign == -1 && (ans < INT_MIN / 10 || (ans == INT_MIN / 10 && dig < INT_MIN % 10))) {
            return INT_MIN;
        }
        
        ans = static_cast<int>(static_cast<long long>(ans) * 10) + dig;
        ++i;
    }
    
    return ans;
}

#include <cassert>
#include <climits>
#include <string>

// The function signature from the solution
int stringToInteger(const std::string& s);

int main() {
    // Basic cases
    assert(stringToInteger("42") == 42);
    assert(stringToInteger("-42") == -42);
    assert(stringToInteger("+42") == 42);
    
    // Leading spaces
    assert(stringToInteger("   42") == 42);
    assert(stringToInteger("   -42") == -42);
    
    // Trailing non-digit stops parsing
    assert(stringToInteger("4193 with words") == 4193);
    assert(stringToInteger("-91283472332") == INT_MIN); // overflow negative
    assert(stringToInteger("2147483648") == INT_MAX);   // overflow positive
    
    // Edge cases
    assert(stringToInteger("") == 0);
    assert(stringToInteger("   ") == 0);
    assert(stringToInteger("words") == 0);
    assert(stringToInteger("+-12") == 0);
    assert(stringToInteger("+5-2") == 5);
    
    // Maximum and minimum valid values
    assert(stringToInteger("2147483647") == INT_MAX);
    assert(stringToInteger("-2147483648") == INT_MIN);
    
    return 0;
}
