Write a C++ function named `decimalToBinary` that accepts a single non-negative integer `n` and returns an integer representing its binary equivalent using the division-by-2 method. The function must handle `n = 0` correctly (returning `0`), and for any positive `n`, it must produce the binary representation without leading zeros. The function should be `const`-correct where applicable and must not rely on any external libraries beyond the standard C++ headers. The return value must be an integer that visually represents the binary digits (e.g., for input `5`, return `101`). Assume the input is within the range where the binary representation fits into a standard `int` (i.e., `n <= 2^31 - 1` for 32-bit ints, but practical limits apply since the binary representation as an integer with 1s and 0s will overflow for large inputs—you only need to handle `n` such that the binary digit count is at most 9 or 10 digits, e.g., `n <= 1023`, to ensure no overflow of the returned integer). The function must be self-contained and not use `main` or any other entry-point.
// The core algorithm is the standard repeated division by 2 method: while `n > 0`, compute the remainder (`bit = n % 2`), which is the least significant binary digit. Build the result integer by appending this bit to the front of the current binary representation. This is done by keeping a multiplier `place` that starts at 1 and multiplied by 10 for each new bit, then adding `bit * place` to the running result. After extracting the bit, set `n = n / 2` to process the next higher bit. For `n = 0`, the loop does not execute, so we must explicitly return `0` (the binary representation of 0 is `0`). Edge cases include `n = 1` (returns `1`), and small numbers like `2` (returns `10`). Since the returned value is an integer, we cannot represent binary strings longer than about 9-10 digits without overflow; thus, we restrict the input to `n <= 1023` (binary up to `1111111111`). Time complexity is `O(log₂ n)` because each iteration halves `n`. Space complexity is `O(1)` as only a few integer variables are used.
#include <cmath>   // for pow (though not strictly needed, kept for clarity)

// Convert a non-negative integer n (0 <= n <= 1023) to its binary representation
// as an integer (e.g., 5 -> 101, 0 -> 0). Uses the division-by-2 method.
int decimalToBinary(const int n) {
    if (n == 0) {
        return 0;
    }

    int result = 0;
    int place = 1;  // 10^0, 10^1, 10^2, ...
    int value = n;

    while (value > 0) {
        const int bit = value % 2;
        result += bit * place;
        place *= 10;
        value /= 2;
    }

    return result;
}
#include <cassert>

int main() {
    // Basic conversions
    assert(decimalToBinary(0) == 0);
    assert(decimalToBinary(1) == 1);
    assert(decimalToBinary(2) == 10);
    assert(decimalToBinary(3) == 11);
    assert(decimalToBinary(4) == 100);
    assert(decimalToBinary(5) == 101);
    assert(decimalToBinary(10) == 1010);
    assert(decimalToBinary(15) == 1111);
    assert(decimalToBinary(100) == 1100100);
    assert(decimalToBinary(1023) == 1111111111);
}
