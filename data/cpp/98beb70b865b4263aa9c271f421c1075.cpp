Write a C++ function named `isSparseNumber` that takes a positive integer `n` as input and returns a boolean value indicating whether the binary representation of `n` is **sparse**. A number is considered sparse if and only if its binary representation does **not** contain two consecutive `1` bits. For example, `5` (binary `101`) is sparse, but `3` (binary `11`) is not sparse, and `6` (binary `110`) is also not sparse because the last two bits are both `1`. The function must operate on an integer greater than or equal to 1, and it should treat 0 as sparse (since its binary representation is `"0"` with no consecutive ones). You must implement the conversion from decimal to binary manually (do not use `std::bitset` or other built-in formatting functions) and then scan the resulting binary string for consecutive `1`s. The function should have the signature `bool isSparseNumber(int n);` and include proper `const` correctness where applicable.

// The solution approach is straightforward: convert the given integer `n` into its binary representation as a string, then check for any occurrence of two adjacent `1` characters. The conversion is done by repeatedly dividing `n` by 2 and recording whether each remainder is 0 or 1, building the string from least significant bit to most significant bit (or using a reverse at the end). For the edge case where `n` is 0, the loop would not run and the binary string would be empty; we handle this by explicitly returning `true` because 0 has no consecutive ones. The main algorithm iterates through the binary string from index 1 onward (starting at the second character) and checks if the current character and the previous character are both `'1'`; if so, immediately return `false`. If the loop completes without finding any consecutive ones, return `true`. The time complexity is `O(log n)` because the number of bits in the binary representation of `n` is proportional to the number of times `n` is halved. The auxiliary space complexity is also `O(log n)` for storing the binary string. The algorithm handles all positive integers correctly, including large values, and correctly identifies sparse versus non-sparse numbers. Note that `INT_MAX` (with binary `111...1` of 31 ones) is not sparse, and powers of two like `8` (binary `1000`) are sparse.

#include <string>

// Returns true if the binary representation of n has no consecutive 1s.
// n is assumed to be a non-negative integer.
bool isSparseNumber(int n) {
    // Edge case: 0 has binary "0", which has no consecutive ones.
    if (n == 0) {
        return true;
    }

    // Build binary representation as a string (least significant bit first).
    std::string binary = "";
    int value = n;
    while (value > 0) {
        binary += (value % 2 == 0) ? '0' : '1';
        value /= 2;
    }

    // Reverse to get the correct order (most significant bit first).
    // We could also scan from the end, but reversing makes the check natural.
    std::string bin = "";
    for (int i = binary.size() - 1; i >= 0; --i) {
        bin += binary[i];
    }

    // Check for consecutive '1's.
    for (size_t i = 1; i < bin.size(); ++i) {
        if (bin[i] == '1' && bin[i - 1] == '1') {
            return false;
        }
    }
    return true;
}

#include <cassert>

// Forward declaration of the solution function (if not defined above; here it is defined above).
bool isSparseNumber(int n);

int main() {
    // Basic positive cases
    assert(isSparseNumber(0) == true);   // binary "0"
    assert(isSparseNumber(1) == true);   // binary "1"
    assert(isSparseNumber(2) == true);   // binary "10"
    assert(isSparseNumber(5) == true);   // binary "101"
    assert(isSparseNumber(8) == true);   // binary "1000"
    assert(isSparseNumber(10) == true);  // binary "1010"

    // Non-sparse numbers
    assert(isSparseNumber(3) == false);  // binary "11"
    assert(isSparseNumber(6) == false);  // binary "110"
    assert(isSparseNumber(7) == false);  // binary "111"
    assert(isSparseNumber(27) == false); // binary "11011"

    // Larger sparse numbers
    assert(isSparseNumber(21) == true);  // binary "10101"
    assert(isSparseNumber(40) == true);  // binary "101000"

    // Large non-sparse number (max int representation has consecutive ones)
    assert(isSparseNumber(2147483647) == false); // binary has many consecutive ones

    return 0;
}
