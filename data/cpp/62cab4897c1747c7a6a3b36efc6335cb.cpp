// Write a C++ function `polynomialDivision` that takes two vectors of `long long` representing polynomial coefficients in ascending order of degree (i.e., index 0 is the constant term, index n is the coefficient of x^n). Given polynomial C(x) and A(x) where A(x) is not the zero polynomial and the degree of C is at least the degree of A, the function must return the quotient polynomial B(x) such that C(x) = A(x) * B(x) exactly (no remainder). For this task, assume the input guarantees an exact division (no remainder) and that C and A are non-empty vectors with leading coefficients non-zero. The function should compute the quotient via the standard polynomial long division algorithm from highest degree down.

#include <cassert>
#include <vector>

// The solution function is assumed to be defined above or included here for completeness.
// (In a real test you'd include the solution header/file.)

int main() {
    // Example: C = 2x^2 + 3x + 1, A = x + 1 => B = 2x + 1
    std::vector<long long> c1 = {1, 3, 2};
    std::vector<long long> a1 = {1, 1};
    std::vector<long long> b1 = polynomialDivision(c1, a1);
    assert(b1.size() == 2);
    assert(b1[0] == 1);
    assert(b1[1] == 2);

    // Example: C = x^3 + 2x^2 + x, A = x => B = x^2 + 2x + 1
    std::vector<long long> c2 = {0, 1, 2, 1};
    std::vector<long long> a2 = {0, 1};
    std::vector<long long> b2 = polynomialDivision(c2, a2);
    assert(b2.size() == 3);
    assert(b2[0] == 1);
    assert(b2[1] == 2);
    assert(b2[2] == 1);

    // Example: C = 6x^4 + 5x^3 + 4x^2 + 3x + 2, A = 2x^2 + 1 => B = 3x^2 + (5/2)x + ... 
    // But to keep integer exact, use C = (3x^2 - x + 2) * (2x + 1) = 6x^3 - 2x^2 + 4x + 3x^2 - x + 2 = 6x^3 + x^2 + 3x + 2
    // So A = 2x+1, B = 3x^2 - x + 2 => C = 6x^3 + x^2 + 3x + 2
    std::vector<long long> c3 = {2, 3, 1, 6};
    std::vector<long long> a3 = {1, 2};
    std::vector<long long> b3 = polynomialDivision(c3, a3);
    assert(b3.size() == 3);
    assert(b3[0] == 2);
    assert(b3[1] == -1);
    assert(b3[2] == 3);

    // Constant divisor: C = 4x^2 + 2x + 6, A = 2 => B = 2x^2 + x + 3
    std::vector<long long> c4 = {6, 2, 4};
    std::vector<long long> a4 = {2};
    std::vector<long long> b4 = polynomialDivision(c4, a4);
    assert(b4.size() == 3);
    assert(b4[0] == 3);
    assert(b4[1] == 1);
    assert(b4[2] == 2);

    // Same degree: C = 3x + 3, A = x + 1 => B = 3
    std::vector<long long> c5 = {3, 3};
    std::vector<long long> a5 = {1, 1};
    std::vector<long long> b5 = polynomialDivision(c5, a5);
    assert(b5.size() == 1);
    assert(b5[0] == 3);
}

#include <vector>

// Divide polynomial C by polynomial A (both in ascending coefficient order) and return quotient B.
// Assumes exact division and non-zero leading coefficients.
std::vector<long long> polynomialDivision(const std::vector<long long>& c, const std::vector<long long>& a) {
    int n = static_cast<int>(a.size()) - 1; // degree of A
    int m = static_cast<int>(c.size()) - n - 1; // degree of B
    std::vector<long long> b(m + 1, 0);
    std::vector<long long> temp_c = c; // mutable copy of dividend

    for (int i = m; i >= 0; --i) {
        // Leading coefficient of current remainder at degree i+n
        b[i] = temp_c[i + n] / a[n];
        // Subtract b[i] * A * x^i from temp_c
        for (int j = 0; j <= n; ++j) {
            temp_c[i + j] -= b[i] * a[j];
        }
    }

    return b;
}

// The algorithm is a direct implementation of polynomial long division. Let `n = A.size() - 1` be the degree of A, and `m = C.size() - n - 1` be the degree of B. We create a mutable copy `temp_c` of C to track the remaining dividend after each step. Iterate `i` from `m` down to 0 (highest degree term of B first). At each step, the leading coefficient of the remaining dividend is at index `i+n`, and we divide it by the leading coefficient of A (`a[n]`) to get `b[i]`. Then we subtract `b[i] * A` shifted by `i` from `temp_c` (i.e., for each `j` in `[0, n]`, subtract `b[i]*a[j]` from `temp_c[i+j]`). After processing all degrees from `m` down to 0, the remainder should be zero (by problem constraint), and `b` is the quotient. 
//
// Edge cases: If A has degree 0 (i.e., A is a constant), then `n=0`, and the loop simplifies to dividing each element of C by that constant. If C has the same degree as A, then `m=0` and we perform one division step. If C's leading coefficient is not divisible by A's leading coefficient in integer arithmetic, the problem constraint says exact division, but in practice `long long` division truncates; the test cases avoid this. Time complexity is O(m * n) where m is degree of B and n is degree of A, because for each of the (m+1) terms we loop over (n+1) coefficients. Space complexity is O(m+1) for the output and O(|C|) for the temporary copy.
