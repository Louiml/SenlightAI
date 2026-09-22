// Write a C++ function named `square_digits` that takes a non-negative integer as input and returns an integer formed by concatenating the squares of each digit of the input number. For example, given `9119`, the function should return `811181` because `9²=81`, `1²=1`, `1²=1`, `9²=81`. The input will be a non-negative integer (including zero), and the output must also be a standard integer. Assume the resulting concatenated number will fit within the range of a 32-bit signed integer. Do not use any external libraries beyond the standard C++ library.
The solution processes the input number digit by digit. Convert the integer to a string using `std::to_string` so that each digit can be accessed individually. For each character in the string, convert it back to an integer digit by subtracting the ASCII value of `'0'` (or using `digit - '0'` for clarity). Compute the square of that digit (using simple multiplication, not `pow`, to avoid floating-point issues) and append the result as a string to an accumulating result string. After processing all digits, convert the accumulated string back to an integer using `std::stoi` and return it. Edge cases: the input might be `0`, which yields `"0"` after squaring, and the result is `0`. Also, if any digit square contains multiple digits (e.g., `9` → `81`), the concatenation naturally handles multi-digit results, so the result string may be longer than the original string. If the result string begins with leading zeros (which can happen if a digit is 0 or 1, e.g., `101` → `101` → wait `1²=1`, `0²=0`, `1²=1` → `101`; but if input `10` → `1` and `0` → `"10"`), `stoi` will correctly parse it, discarding leading zeros only if they appear at the beginning of the string, but the string built by appending squares cannot start with a zero unless the first digit is 0 (input like `05` is not valid as an integer; input `0` gives `"0"`). The time complexity is O(d) where d is the number of digits in the input, and space complexity is O(d) for the intermediate strings.
#include <string>

// Return an integer formed by concatenating the squares of each digit of input.
int square_digits(int num) {
    std::string num_str = std::to_string(num);
    std::string result_str;
    
    for (char ch : num_str) {
        int digit = ch - '0';
        int square = digit * digit;
        result_str += std::to_string(square);
    }
    
    return std::stoi(result_str);
}
#include <cassert>

int main() {
    assert(square_digits(9119) == 811181);
    assert(square_digits(0) == 0);
    assert(square_digits(1) == 1);
    assert(square_digits(9) == 81);
    assert(square_digits(12) == 14);
    assert(square_digits(123) == 149);
    assert(square_digits(100) == 100);
    assert(square_digits(222) == 444);
    assert(square_digits(888) == 646464);
    assert(square_digits(321) == 941);
    return 0;
}
