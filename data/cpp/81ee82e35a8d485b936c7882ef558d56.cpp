Write a standalone C++ function named `integerToRoman` that converts a positive integer in the range 1 to 3999 into its Roman numeral representation as a `std::string`. The function must handle all standard Roman numeral rules, including subtractive notation (e.g., 4 as "IV", 9 as "IX", 40 as "XL", 90 as "XC", 400 as "CD", 900 as "CM"). The input is guaranteed to be a valid integer within the given range, so no error handling is required. The function should be efficient, use no dynamic memory allocation beyond the returned string, and be implemented without using any lookup tables or precomputed strings for the entire numeral set—only the seven basic symbols (`I`, `V`, `X`, `L`, `C`, `D`, `M`) and their values (1, 5, 10, 50, 100, 500, 1000) may be used in the algorithm.
#include <cassert>
#include <string>

// The solution function is declared above.
int main() {
    assert(integerToRoman(1) == "I");
    assert(integerToRoman(4) == "IV");
    assert(integerToRoman(9) == "IX");
    assert(integerToRoman(40) == "XL");
    assert(integerToRoman(90) == "XC");
    assert(integerToRoman(400) == "CD");
    assert(integerToRoman(900) == "CM");
    assert(integerToRoman(1994) == "MCMXCIV");
    assert(integerToRoman(2025) == "MMXXV");
    assert(integerToRoman(3999) == "MMMCMXCIX");
}
#include <string>
#include <vector>

// Converts a positive integer (1 to 3999) to its Roman numeral representation.
std::string integerToRoman(int num) {
    const std::vector<char> roman = {'M', 'D', 'C', 'L', 'X', 'V', 'I'};
    const std::vector<int> value = {1000, 500, 100, 50, 10, 5, 1};
    
    std::string result;
    
    // Process each decimal place from thousands down to ones.
    for (size_t n = 0; n < roman.size(); n += 2) {
        int digit = num / value[n];
        
        if (digit < 4) {
            result.append(digit, roman[n]);
        } else if (digit == 4) {
            result.push_back(roman[n]);
            result.push_back(roman[n - 1]);
        } else if (digit > 4 && digit < 9) {
            result.push_back(roman[n - 1]);
            result.append(digit - 5, roman[n]);
        } else if (digit == 9) {
            result.push_back(roman[n]);
            result.push_back(roman[n - 2]);
        }
        
        num %= value[n];
    }
    
    return result;
}
// The solution processes each decimal place (thousands, hundreds, tens, ones) from highest to lowest, using a pattern based on the digit at that place. For each place, the base symbol (`roman[n]`) and two higher symbols are used: the half-value symbol (`roman[n-1]`) and the next-base symbol (`roman[n-2]`). The digit `x` at that place is extracted by integer division by the place value (1000, 100, 10, 1). Then the following cases are applied: if `x < 4`, append `x` copies of the base symbol; if `x == 4`, append base symbol followed by half-value symbol; if `5 ≤ x ≤ 8`, append half-value symbol followed by `(x-5)` base symbols; if `x == 9`, append base symbol followed by next-base symbol. After processing, the remainder is moved to the next lower place. This approach exactly matches the given snippet's logic, but we generalize it to handle all four places by iterating over the symbol/value arrays with step 2, starting from `n=0` (thousands) down to `n=6` (ones). Edge cases include digits 4 and 9 at any place, and the value 3999 which produces `MMMCMXCIX`. Time complexity is \(O(1)\) because the loop runs at most 4 iterations regardless of input, and each iteration appends at most 4 characters. Space complexity is \(O(1)\) auxiliary, plus the output string which is at most 15 characters (for 3888, which is `MMMDCCCLXXXVIII`).
