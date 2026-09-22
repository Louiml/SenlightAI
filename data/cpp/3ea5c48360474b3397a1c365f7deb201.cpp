// Write a C++ function `sumOfRepeatedDigits` that takes a string of exactly three digits (digits '0'–'9', no spaces or other characters) and returns the sum formed by repeating the hundreds, tens, and ones digits separately as three-digit numbers: hundreds digit repeated three times, tens digit repeated three times, and ones digit repeated three times, then summing those three numbers. For example, for input "123", the three numbers are 111, 222, and 333, and the sum is 666. The function should handle leading zeros correctly (e.g., "012" gives 0*111 + 1*111 + 2*111 = 333). Assume input is always exactly three digits, but you may validate this with an `assert` if desired. Provide the function only, not a `main` wrapper.

The solution extracts each character from the string, converts it to an integer by subtracting `'0'` (since ASCII digits are consecutive). For each digit `d`, the repeated three-digit number is `d * 111` because `ddd` = 100d + 10d + d = 111d. Summing the three gives `(a + b + c) * 111`. This is O(1) time and O(1) auxiliary space. Edge cases: zero digits contribute zero; leading zeros are handled naturally since subtraction of `'0'` works regardless of digit value; input length is guaranteed by the task, but an assert can be added for robustness (though not required). No loops needed, just direct indexing and arithmetic.

#include <string>
#include <cassert>

// Given a string of exactly three digits (e.g., "123"), return the sum of
// three numbers formed by repeating each digit three times: aaa + bbb + ccc.
int sumOfRepeatedDigits(const std::string& digits) {
    // Validate input length; assumes caller provides exactly 3 characters.
    assert(digits.size() == 3);

    // Convert each character to its integer value.
    const int a = digits[0] - '0';
    const int b = digits[1] - '0';
    const int c = digits[2] - '0';

    // Each repeated number is digit * 111; sum them.
    return (a + b + c) * 111;
}

#include <cassert>

int main() {
    assert(sumOfRepeatedDigits("123") == 666); // 111 + 222 + 333
    assert(sumOfRepeatedDigits("000") == 0);
    assert(sumOfRepeatedDigits("999") == 2997); // 999 + 999 + 999
    assert(sumOfRepeatedDigits("010") == 111); // 0 + 111 + 0
    assert(sumOfRepeatedDigits("505") == 1110); // 555 + 0 + 555
    assert(sumOfRepeatedDigits("111") == 333);
    assert(sumOfRepeatedDigits("987") == 2664); // 999 + 888 + 777
    assert(sumOfRepeatedDigits("100") == 111);
    assert(sumOfRepeatedDigits("001") == 111);
    assert(sumOfRepeatedDigits("321") == 666); // symmetric property
    return 0;
}
