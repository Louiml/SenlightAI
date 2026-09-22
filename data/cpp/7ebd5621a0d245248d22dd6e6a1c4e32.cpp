// Write a C++ function named `binaryStringToDecimalRecursive` that takes a C-style string (null-terminated character array) representing a binary number (only characters '0' and '1', possibly empty) and returns its decimal integer value as an `int`. The function must use a recursive approach that processes the string from the most significant digit (leftmost) to the least significant digit (rightmost), tracking the current exponent of 2. Handle an empty string by returning 0, and assume the input string is valid binary with no leading sign, spaces, or non-binary characters. The function should be `const`-correct, taking `const char*` as input.
// The core idea is to recursively traverse the binary string from right to left. At each step, we look at the character at the current index, convert it to an integer (0 or 1), multiply it by 2 raised to the current exponent (which starts at 0 for the rightmost digit and increments by 1 for each step leftward), and add that to the recursive result of processing the remaining characters to the left. The base case is when the index becomes negative (meaning we have processed all characters), returning 0. This is a classic divide-and-conquer recursion where the problem size (number of characters remaining) decreases by 1 each call. The number of recursive calls equals the length of the string `n`, and each call does constant work (a subtraction, a multiplication, a power computation). Thus, time complexity is O(n) for the recursion plus O(n) for the `pow` calls if naive, but since exponent is small (at most string length -1), we can treat `pow` as O(1) for practical int sizes, giving O(n) overall. Space complexity is O(n) for the recursion call stack in the worst case (when the string is very long), plus O(1) auxiliary for variables. Edge cases: empty string returns 0; a single character '0' returns 0; a single character '1' returns 1; long strings might overflow int, but the task assumes valid int-range results; leading zeros are handled naturally because they multiply by 0.
#include <cmath>

// Recursively convert a binary string (const char*) to its decimal int value.
// Processes from rightmost (least significant) to leftmost (most significant).
// Empty string returns 0.
int binaryStringToDecimalRecursive(const char* binary) {
    // Helper recursive function with index and exponent tracking
    // We use a lambda or a separate function to avoid exposing extra parameters
    // But for simplicity, we define a recursive helper inside using a function.
    // Since C++ doesn't allow nested functions directly, we use a static helper.
    // However, the task asks for a free function, so we implement recursion via a helper.
    // We'll write a private static-like recursive function.
    // Actually, we can modify the signature to accept length and exponent, but the
    // task specifies a single function. So we'll create an internal helper.
    // To keep it self-contained, we use a lambda via std::function, but that adds
    // overhead. Better: define a separate recursive helper function (static).
    // Since we only output the solution function, we'll implement internally.
    // But we are allowed to write helper functions inside the solution? The task says
    // "a descriptively named free function that matches the task specification" and
    // "Do not include a main function". So we can write a helper.
    // Let's write a helper recursive function.
    // However, to keep the solution clean, we can implement the recursion directly
    // by passing the index and exponent as default parameters? But C++ doesn't allow
    // default parameters on free functions after 'const char*'? It does, but the
    // signature must match exactly. So we'll write a helper.
    // For simplicity, let's write a recursive function that takes the string,
    // the current index (starting from length-1), and the exponent.
    // We'll declare a static helper in the same translation unit.
    // But since we are only outputting code, we can include the helper.
}

// Actually, to satisfy the requirement, we must output only the free function.
// So we'll implement a classic recursive helper inside the same file.
// But the solution section says "Output code only. Do not include a main function".
// So we can define a helper function before the main requested function.
// Let's do that.

// Recursive helper: processes binary string from position 'index' down to 0,
// with current exponent 'exp'.
static int binaryToDecimalHelper(const char* binary, int index, int exp) {
    if (index < 0) return 0;
    return (binary[index] - '0') * std::pow(2, exp) + binaryToDecimalHelper(binary, index - 1, exp + 1);
}

// Public function: returns decimal value of binary string.
int binaryStringToDecimalRecursive(const char* binary) {
    // Find length by iterating until null terminator.
    int length = 0;
    while (binary[length] != '\0') ++length;
    // Start recursion from the last valid index (length-1) and exponent 0.
    return binaryToDecimalHelper(binary, length - 1, 0);
}
#include <cassert>
#include <cstring>

// Assume the solution function is declared above (we include it here for testing).
// Since we are providing tests separately, we'll include the function definition.

int main() {
    // Test empty string
    assert(binaryStringToDecimalRecursive("") == 0);
    // Test single characters
    assert(binaryStringToDecimalRecursive("0") == 0);
    assert(binaryStringToDecimalRecursive("1") == 1);
    // Test small numbers
    assert(binaryStringToDecimalRecursive("10") == 2);   // binary 10 = 2
    assert(binaryStringToDecimalRecursive("11") == 3);   // binary 11 = 3
    assert(binaryStringToDecimalRecursive("100") == 4);  // binary 100 = 4
    assert(binaryStringToDecimalRecursive("101") == 5);  // binary 101 = 5
    assert(binaryStringToDecimalRecursive("110") == 6);  // binary 110 = 6
    assert(binaryStringToDecimalRecursive("111") == 7);  // binary 111 = 7
    // Test larger numbers
    assert(binaryStringToDecimalRecursive("1000") == 8);
    assert(binaryStringToDecimalRecursive("1001") == 9);
    assert(binaryStringToDecimalRecursive("1010") == 10);
    assert(binaryStringToDecimalRecursive("1111") == 15);
    assert(binaryStringToDecimalRecursive("10000") == 16);
    // Test with leading zeros (should be same as without)
    assert(binaryStringToDecimalRecursive("00101") == 5); // leading zeros ignored
    assert(binaryStringToDecimalRecursive("000") == 0);
    // Test maximum 32-bit integer (binary 1111111111111111111111111111111 but that's too large for int, so use safe: 31 ones is 2^31-1 but might overflow int, so use 30 ones = 1073741823)
    // Let's test 30 ones: binary string of length 30 all '1' => 2^30 - 1 = 1073741823, fits in int.
    std::string thirtyOnes(30, '1');
    assert(binaryStringToDecimalRecursive(thirtyOnes.c_str()) == 1073741823);
    // Test 20 bits: 11111111111111111111 (20 ones) = 1048575
    std::string twentyOnes(20, '1');
    assert(binaryStringToDecimalRecursive(twentyOnes.c_str()) == 1048575);
    return 0;
}
