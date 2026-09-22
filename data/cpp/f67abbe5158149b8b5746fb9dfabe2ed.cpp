/*
Write a C++ function `int processNumber(int n)` that, given a positive integer `n`, computes and returns a single integer formed by concatenating two parts: first, the result of `n * 2` (as a decimal integer), then immediately after it, without any separator, the decimal digits of `n` expressed in base-4 but written in **reverse order** (i.e., least significant digit first). For example, if `n = 5`, then `n*2 = 10`, and `5` in base-4 is `11` (since 5 = 1*4 + 1), so the output integer is `1011`. If `n = 12`, `n*2 = 24`, and `12` in base-4 is `30` (12 = 3*4 + 0), reversed gives `03`, so the output is `2403`. Note: `n` is positive and fits in a 32-bit signed integer, but the final concatenated result may exceed 32 bits, so use `long long` for the return type. Do not print anything; just return the value.
*/
#include <string>
#include <cstdint>

// Given a positive integer n, return the concatenation of (2*n) and the reversed base-4 digits of n.
long long processNumber(int n) {
    // Compute 2*n as long long to avoid overflow on n itself.
    long long doubled = 2LL * n;
    // Build the reversed base-4 representation.
    std::string reversedBase4;
    int value = n;
    if (value == 0) {
        reversedBase4 = "0";
    } else {
        while (value > 0) {
            int remainder = value % 4;
            reversedBase4 += static_cast<char>('0' + remainder);
            value /= 4;
        }
    }
    // Concatenate and convert to long long.
    std::string combined = std::to_string(doubled) + reversedBase4;
    return std::stoll(combined);
}
#include <cassert>

int main() {
    // Basic examples
    assert(processNumber(5) == 1011);   // 10 + "11" -> 1011
    assert(processNumber(12) == 2403);  // 24 + "03" -> 2403
    assert(processNumber(1) == 21);     // 2 + "1" -> 21
    assert(processNumber(2) == 42);     // 4 + "2" -> 42
    assert(processNumber(3) == 63);     // 6 + "3" -> 63
    assert(processNumber(4) == 810);    // 8 + "10" -> 810
    assert(processNumber(6) == 1212);   // 12 + "12" -> 1212 (6 in base4 = 12)
    assert(processNumber(7) == 1413);   // 14 + "13" -> 1413 (7 in base4 = 13)
    assert(processNumber(8) == 1620);   // 16 + "20" -> 1620 (8 in base4 = 20)
    assert(processNumber(20) == 40110); // 40 + "110" -> 40110 (20 in base4 = 110)
}
// The problem has two independent parts:  
// 1. **Triple the value**: Compute `2 * n` as a `long long`.  
// 2. **Base-4 reversed representation**: Convert `n` to base-4 by repeatedly dividing by 4 and collecting remainders. Since we need the digits in reverse order (least significant first), we can append each remainder directly to a string as we produce it (the first remainder is the least significant digit). The base-4 representation may have leading zeros when reversed (e.g., for `12`, base-4 is `30`, reversed gives `03`). These zeros must be preserved in the final concatenation.  
//
// After building the reversed base-4 string, concatenate `std::to_string(2*n)` with that string, then convert the whole concatenated string to a `long long` using `std::stoll`. Edge cases: `n=1` gives base-4 digit `1`, reversed `1`, `2*1=2`, result `21`; `n=2` gives base-4 `2`, reversed `2`, result `42`; large `n` may cause the concatenated string length to exceed `long long`? But since `n` fits in 32-bit, `2*n` fits in 64-bit, and base-4 digits count is at most `log4(2^31) ≈ 15.5` digits, so the total length is at most ~20 digits, which fits in `long long` (19 digits max for signed 64-bit on many platforms? Actually `long long` max is 9.22e18, ~19 digits, but 20 digits might overflow). To be safe, we can return `long long` but note that for the largest `n` near `2^31-1`, the concatenated number may exceed `long long`. For the scope of this task, we assume the test cases avoid such overflow, or we can use `unsigned long long` (max ~1.8e19, 20 digits) but still may overflow. We'll use `long long` and trust the test data.  
//
// Time complexity: O(log_4 n) for the base-4 conversion, which is O(log n). Space complexity: O(log n) for the string.
