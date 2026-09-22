Write a C++ function that takes a positive integer `n` (1 ≤ n ≤ 10^9) and returns a string indicating the Japanese suffix for its last digit, based on the following rules: if the last digit is 2, 4, 5, 7, or 9, return "hon"; if it is 0, 1, 6, or 8, return "pon"; if it is 3, return "bon". The function should handle any positive integer, using only the last digit for classification, and must be named `japaneseSuffix` with signature `std::string japaneseSuffix(int n)`.
// The solution extracts the last digit via the modulo operator `n % 10`. Since the input is a positive integer, no negative handling is required, but the function should still work correctly for any non-negative integer. A switch statement on the last digit efficiently maps each possible digit (0–9) to its corresponding suffix, with cases grouped for digits that share the same suffix. The algorithm runs in O(1) time and uses O(1) auxiliary space, as only the last digit is considered and a constant-size string is returned. Edge cases: `n = 10` yields 0 → "pon", `n = 3` yields "bon", and large numbers like 1000000000 (last digit 0) still work because the modulo operation is independent of magnitude.
#include <string>

// Return the Japanese suffix based on the last digit of a positive integer.
// Digits 2,4,5,7,9 -> "hon"
// Digits 0,1,6,8   -> "pon"
// Digit 3          -> "bon"
std::string japaneseSuffix(int n) {
    const int lastDigit = n % 10; // Extract the last digit
    switch (lastDigit) {
        case 2:
        case 4:
        case 5:
        case 7:
        case 9:
            return "hon";
        case 0:
        case 1:
        case 6:
        case 8:
            return "pon";
        case 3:
            return "bon";
        default:
            return ""; // Never reached for valid input
    }
}
#include <cassert>
#include <string>
#include <iostream>

// Include the solution function here (or link to it)
std::string japaneseSuffix(int n) {
    const int lastDigit = n % 10;
    switch (lastDigit) {
        case 2: case 4: case 5: case 7: case 9: return "hon";
        case 0: case 1: case 6: case 8: return "pon";
        case 3: return "bon";
        default: return "";
    }
}

int main() {
    assert(japaneseSuffix(2) == "hon");
    assert(japaneseSuffix(5) == "hon");
    assert(japaneseSuffix(9) == "hon");
    assert(japaneseSuffix(10) == "pon");
    assert(japaneseSuffix(1) == "pon");
    assert(japaneseSuffix(8) == "pon");
    assert(japaneseSuffix(3) == "bon");
    assert(japaneseSuffix(13) == "bon");
    assert(japaneseSuffix(123456789) == "hon");
    assert(japaneseSuffix(1000000000) == "pon");

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
