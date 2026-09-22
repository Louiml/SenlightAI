// Write a C++ function `vector<int> catalanProduct(unsigned int n)` that, given an integer \(n\) (with \(2 \le n \le 100\)), returns the decimal value of the product \(C_n \times n!\) where \(C_n\) is the \(n\)-th Catalan number, \(C_n = \frac{1}{n+1}\binom{2n}{n}\). The result will not exceed 200 decimal digits, so you must use a big-integer representation. Specifically, return a `vector<int>` where each element stores exactly 4 decimal digits (base 10000), with the most significant digits at index 0 and the least significant group at the end (e.g., the number 123456789 would be represented as `{1, 2345, 6789}`). Implement the big-integer operations (multiplication by a small integer and division by a small integer) as helper functions within your solution. Ensure the function correctly computes the value for all \(n\) up to 100, and note that the product is always a positive integer (no leading zeros in the vector, except the trivial case where the number is 0, which will not occur here).

The key insight is that the Catalan number \(C_n = \frac{1}{n+1}\binom{2n}{n}\) can be computed iteratively using the recurrence \(C_1 = 1\), and for \(i \ge 2\): \(C_i = C_{i-1} \times \frac{4i-2}{i+1}\). We maintain the Catalan number as a big integer in base 10000, multiplying by the numerator and dividing by the denominator at each step (both small integers). After computing \(C_n\), we multiply the result by \(n!\) by repeatedly multiplying by integers from 2 to \(n\). The big-integer representation uses a fixed-size array of length 500, but we only need about 50 elements for 200 digits (since 500*4 = 2000 digits, which is more than enough). To return the result as a `vector<int>` without leading zeros, after all operations we trim leading zero entries from the front. The multiplication and division algorithms are straightforward: multiplication processes from the least significant end carrying, and division processes from the most significant end carrying the remainder. Time complexity is \(O(L \cdot n)\) where \(L\) is the number of 4-digit groups (at most 500 in the fixed array, but trimmed to about 50 for output), so total operations are small. Space complexity is \(O(L)\) for the big integer plus \(O(1)\) for auxiliary variables.

#include <vector>
#include <algorithm>

// Multiply a big integer represented in base 10000 by a small integer b (b >= 0)
void bigMultiply(std::vector<int>& num, int b) {
    if (b == 0) {
        std::fill(num.begin(), num.end(), 0);
        return;
    }
    int carry = 0;
    for (int i = (int)num.size() - 1; i >= 0; --i) {
        int cur = num[i] * b + carry;
        num[i] = cur % 10000;
        carry = cur / 10000;
    }
}

// Divide a big integer represented in base 10000 by a small integer b (b > 0)
void bigDivide(std::vector<int>& num, int b) {
    int rem = 0;
    for (int i = 0; i < (int)num.size(); ++i) {
        long long cur = rem * 10000LL + num[i];
        num[i] = (int)(cur / b);
        rem = (int)(cur % b);
    }
}

// Trim leading zeros from the big integer representation
void trimLeadingZeros(std::vector<int>& num) {
    int firstNonZero = 0;
    while (firstNonZero < (int)num.size() && num[firstNonZero] == 0) {
        ++firstNonZero;
    }
    if (firstNonZero == (int)num.size()) {
        num.assign(1, 0); // all zeros -> represent as {0}
    } else {
        num.erase(num.begin(), num.begin() + firstNonZero);
    }
}

// Return C_n * n! as a base-10000 vector<int>, most significant at index 0
std::vector<int> catalanProduct(unsigned int n) {
    const int MAX_GROUPS = 100; // enough for 200 digits (50 groups) plus safety
    std::vector<int> catalan(MAX_GROUPS, 0);
    catalan[MAX_GROUPS - 1] = 1; // C_1 = 1

    // Compute C_n using recurrence for i = 2..n
    for (unsigned int i = 2; i <= n; ++i) {
        bigMultiply(catalan, 4 * i - 2);
        bigDivide(catalan, i + 1);
    }

    // Multiply by n! = 2 * 3 * ... * n
    for (unsigned int j = 2; j <= n; ++j) {
        bigMultiply(catalan, j);
    }

    // Trim leading zeros and return
    trimLeadingZeros(catalan);
    return catalan;
}

#include <cassert>
#include <vector>
#include <string>
#include <sstream>

// Helper to convert big integer vector to a decimal string for comparison
std::string bigToString(const std::vector<int>& num) {
    std::ostringstream oss;
    oss << num[0];
    for (size_t i = 1; i < num.size(); ++i) {
        oss.width(4);
        oss.fill('0');
        oss << num[i];
    }
    return oss.str();
}

int main() {
    // Known values: C_2 = 2, C_3 = 5, C_4 = 14, etc.
    // C_2 * 2! = 2*2 = 4
    assert(bigToString(catalanProduct(2)) == "4");
    // C_3 * 3! = 5*6 = 30
    assert(bigToString(catalanProduct(3)) == "30");
    // C_4 * 4! = 14*24 = 336
    assert(bigToString(catalanProduct(4)) == "336");
    // C_5 * 5! = 42*120 = 5040
    assert(bigToString(catalanProduct(5)) == "5040");
    // C_6 * 6! = 132*720 = 95040
    assert(bigToString(catalanProduct(6)) == "95040");
    // C_7 * 7! = 429*5040 = 2162160
    assert(bigToString(catalanProduct(7)) == "2162160");
    // C_10 * 10! = 16796 * 3628800 = 60949324800
    assert(bigToString(catalanProduct(10)) == "60949324800");
    // Edge case: n=2 is the minimum given in the task
    assert(catalanProduct(2).size() == 1 && catalanProduct(2)[0] == 4);
    // Large n, just check the length is correct (C_100 * 100! has about 200 digits)
    assert(bigToString(catalanProduct(100)).size() >= 190 && bigToString(catalanProduct(100)).size() <= 210);
    return 0;
}
