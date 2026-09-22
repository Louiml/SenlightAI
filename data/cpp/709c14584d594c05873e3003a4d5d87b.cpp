// Write a C++ function `largestDivisibleNumber` that takes a vector of integers (each either 0 or 5) and returns the largest number (as a string) that can be formed using all or some of these digits, such that the number is divisible by both 90 and 5. The number must be a valid non-negative integer without leading zeros, except the number 0 itself. If it is impossible to form any such number, return "-1". The function should handle empty vectors gracefully, returning "-1". Note: a number divisible by 90 must end in 0 and have a digit sum divisible by 9 (since 90 = 9*10). Since the digits are only 0 and 5, the digit sum is 5 * (count of 5s used), so the count of 5s used must be a multiple of 9. To maximize the number, use as many digits as possible, preferring 5s (which are larger than 0s) in the most significant positions, but respecting the multiple-of-9 constraint on the count of 5s. If any 0 exists, the number can end with 0 to satisfy divisibility by 10; otherwise, no number is possible.

The key insight is that a number divisible by 90 must be divisible by both 9 and 10. Divisible by 10 means the last digit must be 0, so at least one 0 is required. Divisible by 9 means the sum of digits must be a multiple of 9. Since all nonzero digits are 5, the sum is 5 * (number of 5s used). To make this sum a multiple of 9, the count of 5s used must be a multiple of 9, because 5 and 9 are coprime. Therefore, from the available 5s, we can use at most `floor(c5 / 9) * 9` fives. To maximize the numeric value, we put all the fives first (most significant) and then all zeros (least significant). If there are no zeros, return "-1" because no number can be divisible by 10. If after trimming, the count of fives is zero, then we can only form the number 0 (since we have at least one zero). The algorithm simply counts zeros and fives, computes the maximum usable fives as `(c5 / 9) * 9` (integer division), and then checks conditions. Time complexity is O(n) for counting, and O(n) for constructing the output string (since we output up to n digits). Space complexity is O(n) for the output string, but auxiliary space is O(1) besides that.

#include <string>
#include <vector>

// Returns the largest number (as a string) divisible by 90 that can be formed
// from the given digits (each must be 0 or 5). Returns "-1" if impossible.
std::string largestDivisibleNumber(const std::vector<int>& digits) {
    int countZero = 0;
    int countFive = 0;

    for (int d : digits) {
        if (d == 0) {
            ++countZero;
        } else if (d == 5) {
            ++countFive;
        }
        // Ignore any invalid digit (though problem guarantees 0 or 5)
    }

    // Must have at least one zero to be divisible by 10.
    if (countZero == 0) {
        return "-1";
    }

    // The number of 5s used must be a multiple of 9.
    int usableFives = (countFive / 9) * 9;

    // If no fives can be used, the only valid number is 0.
    if (usableFives == 0) {
        return "0";
    }

    // Build the result: all fives first, then all zeros.
    std::string result;
    result.reserve(usableFives + countZero);
    result.append(usableFives, '5');
    result.append(countZero, '0');
    return result;
}

#include <cassert>
#include <vector>
#include <string>

// Forward declaration of the function under test.
std::string largestDivisibleNumber(const std::vector<int>& digits);

int main() {
    // Basic cases
    assert(largestDivisibleNumber({5, 5, 5, 5, 5, 5, 5, 5, 5, 0}) == "5555555550");
    assert(largestDivisibleNumber({0}) == "0");
    assert(largestDivisibleNumber({5}) == "-1");
    assert(largestDivisibleNumber({5, 5, 5, 5, 5, 5, 5, 5, 0}) == "0"); // only 8 fives, not multiple of 9
    assert(largestDivisibleNumber({}) == "-1");

    // Larger mixed case
    std::vector<int> manyFives(18, 5);
    manyFives.push_back(0);
    std::string expected(18, '5');
    expected.push_back('0');
    assert(largestDivisibleNumber(manyFives) == expected);

    // Extra zeros don't change the value, just append more zeros
    std::vector<int> withExtraZeros = {5,5,5,5,5,5,5,5,5,0,0,0};
    assert(largestDivisibleNumber(withExtraZeros) == "555555555000");

    // All zeros
    assert(largestDivisibleNumber({0,0,0}) == "0");

    // More than enough fives but not a multiple of 9 exactly
    std::vector<int> tenFives = {5,5,5,5,5,5,5,5,5,5,0};
    assert(largestDivisibleNumber(tenFives) == "5555555550"); // uses 9 fives

    return 0;
}
