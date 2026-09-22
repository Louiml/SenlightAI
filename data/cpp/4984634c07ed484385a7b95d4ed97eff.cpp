// Write a C++ function named `multiplyBigNumbers` that takes two non-negative integers represented as strings (each containing only decimal digits, with no leading zeros except the single digit "0") and returns their exact product as a string. The input strings may be up to 100,000 digits long, so the result may be up to 200,000 digits. Your implementation must use a manual big-integer multiplication algorithm (schoolbook or optimized), not built-in arbitrary-precision libraries or converting to native integer types. Handle the edge case where either operand is "0".

// The solution uses the classic schoolbook multiplication algorithm for big integers. Convert each input string into arrays of digits stored in reverse order (least significant digit first) to simplify index handling and carry propagation. Multiply each digit of the first number by each digit of the second number, accumulating the partial products into a result array at the correct positions (i + j for 0-based indices). After all multiplications, normalize the result by carrying over values greater than 9 to the next higher position. Finally, convert the reversed digit array back into a string, stripping any leading zeros that may have resulted from the multiplication (e.g., when either input is "0"). Edge cases include zero inputs, unequal digit lengths, and large carry values. Time complexity is O(n*m) where n and m are the digit counts of the inputs; space complexity is O(n+m) for the result array and output string. The constraints fit within available memory (arrays are dynamically allocated based on input sizes to avoid wasteful large fixed‑size buffers).

#include <string>
#include <vector>
#include <algorithm>

// Multiply two non-negative integers represented as decimal strings.
// Returns the product as a decimal string.
std::string multiplyBigNumbers(const std::string& a, const std::string& b) {
    // Handle the zero case early to simplify logic
    if (a == "0" || b == "0") {
        return "0";
    }

    int n = a.size();
    int m = b.size();

    // Store digits in reverse order (least significant first)
    std::vector<int> numA(n);
    std::vector<int> numB(m);
    for (int i = 0; i < n; ++i) {
        numA[i] = a[n - 1 - i] - '0';
    }
    for (int i = 0; i < m; ++i) {
        numB[i] = b[m - 1 - i] - '0';
    }

    // Result can have at most n + m digits
    std::vector<int> result(n + m, 0);

    // Schoolbook multiplication
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            result[i + j] += numA[i] * numB[j];
        }
    }

    // Normalize: carry over digits > 9
    for (int i = 0; i < n + m - 1; ++i) {
        if (result[i] >= 10) {
            result[i + 1] += result[i] / 10;
            result[i] %= 10;
        }
    }

    // Build the result string from most significant digit to least
    std::string product;
    product.reserve(n + m);

    // Skip leading zeros (at most one possible because we handled zero case)
    int start = n + m - 1;
    while (start > 0 && result[start] == 0) {
        --start;
    }

    for (int i = start; i >= 0; --i) {
        product.push_back(static_cast<char>('0' + result[i]));
    }

    return product;
}

#include <cassert>
#include <string>

// Declaration of the function under test
std::string multiplyBigNumbers(const std::string& a, const std::string& b);

int main() {
    // Basic multiplications
    assert(multiplyBigNumbers("0", "0") == "0");
    assert(multiplyBigNumbers("0", "999") == "0");
    assert(multiplyBigNumbers("1", "1") == "1");
    assert(multiplyBigNumbers("2", "3") == "6");
    assert(multiplyBigNumbers("12", "34") == "408");
    assert(multiplyBigNumbers("123", "456") == "56088");

    // Larger numbers and edge carries
    assert(multiplyBigNumbers("999", "999") == "998001");
    assert(multiplyBigNumbers("1000", "1000") == "1000000");

    // Asymmetric lengths
    assert(multiplyBigNumbers("123456789", "987654321") == "121932631112635269");

    // Very large numbers (both 20 digits)
    std::string bigA = "12345678901234567890";
    std::string bigB = "98765432109876543210";
    assert(multiplyBigNumbers(bigA, bigB) == "1219326311370217952237463801111263526900");

    // Another large multiplication with carries cascade
    assert(multiplyBigNumbers("99999999999999999999", "99999999999999999999") == "9999999999999999999800000000000000000001");

    return 0;
}
