Write a C++ function named `computeUPCEANCheckDigit` that takes a string of digits (without the check digit) representing the body of a UPC-A or EAN-13 barcode and returns the standard UPC/EAN check digit as an integer. The check digit algorithm works as follows: starting from the rightmost digit of the input and moving left, sum the digits in the odd positions (1st, 3rd, 5th, ... from the right) and multiply that sum by 3, then add the sum of the digits in the even positions (2nd, 4th, 6th, ... from the right). The check digit is `(10 - (total_sum % 10)) % 10`. The input string will contain only digits (0-9) and will have at least one digit, but may be of any length. Return the check digit as an integer. If the input string contains any non-digit character, return -1 to indicate an error.

The solution computes the check digit by iterating over the input string from right to left, tracking position parity. We use a zero-based index `i` starting at 0 for the rightmost character. For a character at position `i` (0-based from right), if `i` is even, it's an "odd position" (1st, 3rd, ...) and contributes to the sum that gets multiplied by 3; if `i` is odd, it's an "even position" and contributes directly to the total sum. The algorithm accumulates two sums: `sumOdd` (for positions that get tripled) and `sumEven` (for positions that don't). For each character, we validate that it's a digit; if not, return -1 immediately. At the end, compute `total = sumOdd * 3 + sumEven`, then the check digit is `(10 - (total % 10)) % 10`. The modulus operation ensures that when total is a multiple of 10, the check digit is 0. Edge cases: empty string is not allowed per spec (but if given, it would return -1 because the loop wouldn't run and then we'd need to handle—we'll return -1 for empty to be safe). For a single-digit input, the rightmost is position 1 (odd), so sumOdd gets that digit, sumEven stays 0. Time complexity is O(n) where n is the length of the input string, and space complexity is O(1) auxiliary (only a few integer variables).

#include <string>

/**
 * Computes the standard UPC/EAN check digit for a given string of digits.
 * The input must contain only digits (0-9) and have at least one character.
 * Returns the check digit (0-9) on success, or -1 if the input is invalid
 * (contains non-digit characters or is empty).
 */
int computeUPCEANCheckDigit(const std::string& digits) {
    if (digits.empty()) {
        return -1;
    }
    
    int sumOdd = 0;  // Sum of digits in positions 1,3,5,... from the right (0-based even indices from right)
    int sumEven = 0; // Sum of digits in positions 2,4,6,... from the right (0-based odd indices from right)
    
    // Iterate from rightmost character to leftmost
    int len = digits.size();
    for (int i = 0; i < len; ++i) {
        char c = digits[len - 1 - i];
        if (c < '0' || c > '9') {
            return -1; // Invalid non-digit character
        }
        int digit = c - '0';
        if (i % 2 == 0) {
            sumOdd += digit; // Position 1,3,5,... (0-based even from right)
        } else {
            sumEven += digit; // Position 2,4,6,... (0-based odd from right)
        }
    }
    
    int total = sumOdd * 3 + sumEven;
    return (10 - (total % 10)) % 10;
}

#include <cassert>

int main() {
    // Test with known UPC-A check digit examples
    // UPC-A "03600029145" has check digit 2 (real example)
    assert(computeUPCEANCheckDigit("03600029145") == 2);
    
    // EAN-13 test: body "400638133393" has check digit 1
    assert(computeUPCEANCheckDigit("400638133393") == 1);
    
    // Single digit: check digit of "0" is 0, of "5" is 5
    assert(computeUPCEANCheckDigit("0") == 0);
    assert(computeUPCEANCheckDigit("5") == 5);
    
    // Two digits: "12" → sumOdd=2 (rightmost), sumEven=1; total = 2*3+1=7; check = (10-7)%10=3
    assert(computeUPCEANCheckDigit("12") == 3);
    
    // Digits that produce check digit 0: "10" → sumOdd=0, sumEven=1; total=1; check=(10-1)%10=9? Wait recalc: "10" rightmost 0 (odd), 1 (even): sumOdd=0, sumEven=1; total=0*3+1=1; check=(10-1)%10=9. For 0 check use "50" → rightmost 0 (odd), 5 (even): sumOdd=0, sumEven=5; total=5; check=(10-5)%10=5. Actually for 0: try "11" → rightmost 1(odd), 1(even): sumOdd=1, sumEven=1; total=3+1=4; check=(10-4)%10=6. Let's find one: "100" → rightmost 0(odd), 0(even) from right second, 1(odd) from right third? Wait positions from right: rightmost index 0=odd, index1=even, index2=odd. For "100": index0='0' odd sumOdd+=0; index1='0' even sumEven+=0; index2='1' odd sumOdd+=1; sumOdd=1, sumEven=0; total=3; check=(10-3)%10=7. Try "30" → rightmost 0(odd), 3(even): sumOdd=0, sumEven=3; total=3; check=7. Try "90" → sumOdd=0, sumEven=9; total=9; check=1. Try "00" → sumOdd=0, sumEven=0; total=0; check=0.
    assert(computeUPCEANCheckDigit("00") == 0);
    
    // Invalid input: non-digit character
    assert(computeUPCEANCheckDigit("12a3") == -1);
    
    // Empty string (invalid per spec, but we return -1)
    assert(computeUPCEANCheckDigit("") == -1);
    
    // Longer test: EAN-13 body "590123412345" has check digit 7 (standard example)
    assert(computeUPCEANCheckDigit("590123412345") == 7);
    
    return 0;
}
