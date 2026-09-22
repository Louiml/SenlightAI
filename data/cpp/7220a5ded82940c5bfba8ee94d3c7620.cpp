// Write a C++ function named `subtractProductAndSum` that takes a single non-negative integer `n` and returns the result of subtracting the sum of its decimal digits from the product of its decimal digits. For example, given `4421`, the digits are 4, 4, 2, 1; the product is 4×4×2×1 = 32 and the sum is 4+4+2+1 = 11, so the function should return 21. The function must handle the edge case where `n` is zero (product should be 0, sum 0, result 0) and where `n` has a single digit (since the product equals the sum, the result will be 0). You may assume the input is always a non-negative integer that fits in a standard `int`. Do not use any loops other than a range-based for loop over the string representation of the number, and do not include a `main` function in your solution (the test section will provide it).
// The solution converts the integer to a string using `std::to_string`, then iterates over each character. Each character is a digit from '0' to '9'; subtracting the character code of '0' yields the integer value of that digit. Initialize `sum` to 0 and `product` to 1 (since multiplying by 1 is the identity). For each digit, add it to `sum` and multiply it into `product`. After the loop, return `product - sum`. Edge cases: for `n = 0`, the string is "0", so the loop runs once with digit 0; sum becomes 0, product becomes 0, and the result is 0. For `n` equal to a single digit like 7, product = 7 and sum = 7, result = 0. The algorithm processes each digit exactly once, so for a number with `d` digits, it runs in O(d) time. Since the number of digits is at most about 10 (for a 32-bit int), this is effectively O(1) time. Auxiliary space is O(d) for the string, which again is O(1) for practical inputs.
#include <string>

// Returns (product of digits) - (sum of digits) for a non-negative integer n.
int subtractProductAndSum(int n) {
    const std::string digits = std::to_string(n);
    int sum = 0;
    int product = 1;
    
    for (const char c : digits) {
        const int digit = c - '0';
        sum += digit;
        product *= digit;
    }
    
    return product - sum;
}
#include <cassert>

// Declaration of the function to test (assume it's defined elsewhere)
int subtractProductAndSum(int n);

int main() {
    assert(subtractProductAndSum(4421) == 21);
    assert(subtractProductAndSum(0) == 0);
    assert(subtractProductAndSum(1) == 0);
    assert(subtractProductAndSum(10) == -1);  // product=0, sum=1
    assert(subtractProductAndSum(234) == 15); // product=24, sum=9
    assert(subtractProductAndSum(1111) == -4); // product=1, sum=4
    assert(subtractProductAndSum(999) == 720); // product=729, sum=27
    assert(subtractProductAndSum(1234) == 14); // product=24, sum=10
    assert(subtractProductAndSum(1000) == -1); // product=0, sum=1
    assert(subtractProductAndSum(7) == 0);
    return 0;
}
