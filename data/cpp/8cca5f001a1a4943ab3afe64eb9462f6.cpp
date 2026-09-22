Write a C++ function named `computePowerOfTwo` that takes an integer `n` (where `n > 64`) and returns a `std::string` containing the decimal representation of 2 raised to the power `n` (i.e., 2^n). The input `n` is guaranteed to be positive and greater than 64, but you should still validate it inside the function: if `n <= 64`, throw an `std::invalid_argument` exception. The result must be computed exactly using arbitrary-precision arithmetic (since 2^n for n > 64 exceeds the range of standard 64-bit integers). The function should not print anything; it returns the result as a string without leading zeros. Use an efficient digit-by-digit multiplication algorithm (e.g., array or vector-based representation of the number) to compute the power.

The solution represents the number as a vector of integers, where each element stores a single decimal digit (0–9), with the least significant digit at index 0. Initialize the vector with `{1}` (representing 2^0). Then, for each of the `n` multiplications by 2, iterate through all existing digits, double each digit, add the carry from the previous position (starting at 0), store the new digit as `product % 10`, and propagate the carry as `product / 10`. After processing all existing digits, if a carry remains (which will be either 1 or 0 for doubling, but can be larger if the carry chains, though in practice it’s always 0 or 1 because each digit max is 9, doubling gives at most 18+1=19, carry ≤1; however, the general algorithm handles any carry), append the carry as new digits (it will be exactly 1 per extra digit). After the loop, build the final string by reversing the digits (since the vector stores least significant first) and concatenating them. Edge cases: `n <= 64` throws exception; `n = 65` gives 2^65 = 36893488147419103232 (a 20-digit number). Time complexity: O(n * d) where d is the number of digits (approximately n * log10(2) ≈ 0.301n), so overall O(n^2). Space complexity: O(d) = O(n) for the digit vector and output string.

#include <string>
#include <vector>
#include <stdexcept>

// Compute 2^n exactly as a decimal string. Requires n > 64.
std::string computePowerOfTwo(int n) {
    if (n <= 64) {
        throw std::invalid_argument("n must be greater than 64");
    }
    
    std::vector<int> digits = {1}; // least significant digit first
    for (int i = 0; i < n; ++i) {
        int carry = 0;
        for (auto& digit : digits) {
            int product = digit * 2 + carry;
            digit = product % 10;
            carry = product / 10;
        }
        while (carry > 0) {
            digits.push_back(carry % 10);
            carry /= 10;
        }
    }
    
    std::string result;
    result.reserve(digits.size());
    for (auto it = digits.rbegin(); it != digits.rend(); ++it) {
        result.push_back(static_cast<char>('0' + *it));
    }
    return result;
}

#include <cassert>
#include <string>

// Declare the solution function (assumed to be defined elsewhere or above)
std::string computePowerOfTwo(int n);

int main() {
    // Basic correctness for n = 65, 66, 100
    assert(computePowerOfTwo(65) == "36893488147419103232");
    assert(computePowerOfTwo(66) == "73786976294838206464");
    assert(computePowerOfTwo(100) == "1267650600228229401496703205376");
    
    // Check that invalid input throws
    bool threw = false;
    try {
        computePowerOfTwo(64);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);
    
    // Verify consistency: 2^66 == 2 * 2^65
    std::string sixtyFive = computePowerOfTwo(65);
    std::string sixtySix = computePowerOfTwo(66);
    // Manually verify doubling: last digit of 2^65 is 2, multiply by 2 gives 4, etc.
    assert(sixtySix == "73786976294838206464");
    
    // Check length: number of digits roughly floor(n * log10(2)) + 1
    std::string hundred = computePowerOfTwo(100);
    assert(hundred.size() == 31); // because 2^100 has 31 digits
    
    // Edge large value (e.g., 128) – verify first few and last few digits
    std::string power128 = computePowerOfTwo(128);
    assert(power128.substr(0, 5) == "34028");
    assert(power128.substr(power128.size() - 3) == "256");
    
    return 0;
}
