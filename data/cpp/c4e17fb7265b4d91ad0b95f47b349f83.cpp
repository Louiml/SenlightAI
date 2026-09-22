// Write a standalone C++ function named `bigIntegerFactorial` that takes a single non-negative integer `n` (as a normal `int`) and returns the exact value of `n!` as a `std::string` decimal representation. The input may be as large as 1000, so the result can have over 2500 digits and must be computed using arbitrary-precision arithmetic. You must implement your own simple big integer class or use a digit-array approach (e.g., storing digits in reverse order in a `std::vector<int>` or `char` array) to handle multiplication by a small integer. The function should return the factorial as a string with no leading zeros (except for the special case `0!` which is `"1"`). You may not use external libraries like GMP. The function must be efficient enough to handle `n=1000` within a typical time limit (a few seconds) and should not use recursion (use an iterative multiplication loop). The solution must be self-contained, not relying on a `main` function in the provided code, and must pass the provided test assertions.

The core problem is computing the factorial of a large integer `n` up to 1000, where the result exceeds the capacity of standard 64-bit integers. The straightforward approach is to use custom big-integer multiplication: maintain the current factorial as a sequence of decimal digits (stored in reverse order for easy left-to-right multiplication with carry). Initialize the result to `"1"` (i.e., a digit vector `{1}`). Then, for each integer `i` from 2 to `n`, multiply the current digit vector by `i` using a simple grade-school multiplication: for each digit position, compute `digit * i + carry`, store `result % 10` in the current digit, and carry `result / 10` to the next position. After processing all digits, if a carry remains, append its digits (also in reverse order) to the vector. After the loop, reverse the vector to obtain the final decimal string. Important edge cases: `n=0` returns `"1"` (by definition of `0!`), and `n=1` returns `"1"`. No negative inputs are expected; if given, the function can either return `"1"` (for n <= 0) or handle gracefully. For large `n`, the number of multiplications is `n-1`, and each multiplication processes the current number of digits (which grows linearly with `n`), so the total time complexity is `O(n^2)` in the number of digits, equivalently `O(n^2 log n)` if considering the integer size, but since the digit length is `O(log(n!)) = O(n log n)`, the total digit operations are `O(n^2 log n)` worst-case. For `n=1000`, this is easily manageable. Space complexity is `O(number of digits)` which is `O(n log n)`. The function must handle the carry correctly and avoid any integer overflow on `int` by keeping the multiplication values within safe range (each `digit * i + carry` is at most `9*1000 + 9` which fits in a 32-bit int). Ensure the final string has no leading zeros; the only case with a leading zero would be `n=0` which we handle explicitly.

#include <string>
#include <vector>
#include <algorithm>

// Compute n! for non-negative integer n and return it as a decimal string.
std::string bigIntegerFactorial(int n) {
    // By definition, 0! = 1! = 1.
    if (n <= 1) {
        return "1";
    }

    // Store digits in reverse order (least significant first).
    std::vector<int> digits = {1};

    for (int factor = 2; factor <= n; ++factor) {
        int carry = 0;
        for (size_t i = 0; i < digits.size(); ++i) {
            int product = digits[i] * factor + carry;
            digits[i] = product % 10;
            carry = product / 10;
        }
        // Append remaining carry digits.
        while (carry > 0) {
            digits.push_back(carry % 10);
            carry /= 10;
        }
    }

    // Reverse to get normal decimal order.
    std::string result;
    result.reserve(digits.size());
    for (auto it = digits.rbegin(); it != digits.rend(); ++it) {
        result.push_back(static_cast<char>('0' + *it));
    }
    return result;
}

int main() {
    // Basic small factorials.
    assert(bigIntegerFactorial(0) == "1");
    assert(bigIntegerFactorial(1) == "1");
    assert(bigIntegerFactorial(2) == "2");
    assert(bigIntegerFactorial(3) == "6");
    assert(bigIntegerFactorial(5) == "120");

    // Larger values with known results.
    assert(bigIntegerFactorial(10) == "3628800");
    assert(bigIntegerFactorial(20) == "2432902008176640000");
    assert(bigIntegerFactorial(25) == "15511210043330985984000000");

    // Test with a large n to ensure no overflow and correct length.
    std::string fact100 = bigIntegerFactorial(100);
    assert(fact100.size() == 158); // 100! has 158 digits.
    assert(fact100.substr(0, 2) == "93"); // Known first two digits of 100!.

    // Test consistency: n! = n * (n-1)! for random small n.
    for (int n = 2; n <= 15; ++n) {
        // Compare with multiplication using a simple big-int? Instead, just check digit length roughly.
        // But to avoid overcomplication, just check known values for n=10 and n=20 above.
    }

    // Another known value: 1000! has 2568 digits (exactness not checked but length is stable).
    std::string fact1000 = bigIntegerFactorial(1000);
    assert(fact1000.size() == 2568);
    // Ensure it starts with '4' (first digit of 1000!).
    assert(fact1000[0] == '4');
}
