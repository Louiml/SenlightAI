Write a C++ function `std::string addPolynomials(const std::vector<int>& poly1, const std::vector<int>& poly2)` that takes two vectors representing polynomial coefficients in order from the highest-degree term down to the constant term (e.g., index 0 is the coefficient of `x^(n-1)`, index n-1 is the constant). The vectors may have different lengths; treat missing coefficients as zero (so the effective degree is max(len1, len2)-1). The function must return a string representation of the sum polynomial, formatted exactly like the original snippet's output: for each term from highest degree down to constant, output the term as `coef*x^degree`, where `coef` is the absolute value of the coefficient. Prefix each term (except the first) with `" + "` if the coefficient is positive, or `" - "` if negative. The first term must not have a leading sign; it is just the absolute value followed by `x^degree`. If all coefficients sum to zero (e.g., both vectors are empty or all sums are zero), return `"0"`. Coefficients are integers, and the degree index starts at 0 for the constant term.
#include <cassert>
#include <string>
#include <vector>

// Include the solution function here (for completeness in a real test, but for the exercise it's separate).
// For this test block, assume the function is already defined above.

int main() {
    // Single-term polynomials: 2x^2 vs 3x^1 -> sum: 2x^2 + 3x^1
    assert(addPolynomials({2, 0}, {0, 3}) == "2x^2 + 3x^1");

    // Same length, mixed signs: 1x^1 + 2x^0 vs -1x^1 + 0x^0 -> 0x^1 + 2x^0 -> 2x^0
    assert(addPolynomials({1, 2}, {-1, 0}) == "2x^0");

    // Different lengths: 1x^2 + 0x^1 + 1x^0 vs 2x^1 -> 1x^2 + 2x^1 + 1x^0
    assert(addPolynomials({1, 0, 1}, {2}) == "1x^2 + 2x^1 + 1x^0");

    // Both vectors empty → zero polynomial
    assert(addPolynomials({}, {}) == "0");

    // All coefficients cancel out: 1x^0 vs -1x^0 → 0
    assert(addPolynomials({1}, {-1}) == "0");

    // Leading negative coefficient in the highest term: -1x^2 + 1x^0
    assert(addPolynomials({-1, 0, 1}, {}) == "1x^2 + 1x^0"); // Note: first term absolute, no leading minus

    // More complex: -2x^3 + 3x^2 - 1x^0
    assert(addPolynomials({-2, 3, 0, -1}, {}) == "2x^3 + 3x^2 - 1x^0");

    // Two-term with negative middle: 1x^2 - 2x^1 + 1x^0
    assert(addPolynomials({1, -2, 1}, {0, 0, 0}) == "1x^2 - 2x^1 + 1x^0");

    // Zero from one side: 0x^1 + 5x^0
    assert(addPolynomials({0, 5}, {}) == "5x^0");

    return 0;
}
#include <string>
#include <vector>
#include <cstdlib>

// Return a formatted sum of two polynomials given by coefficient vectors.
// Vectors are ordered from highest degree to constant term (index 0 = x^(len-1)).
// Missing coefficients are treated as zero.
std::string addPolynomials(const std::vector<int>& poly1, const std::vector<int>& poly2) {
    const size_t maxLen = poly1.size() > poly2.size() ? poly1.size() : poly2.size();
    std::string result;
    bool firstTerm = true;
    bool anyNonZero = false;

    for (size_t i = 0; i < maxLen; ++i) {
        const int a = (i < poly1.size()) ? poly1[i] : 0;
        const int b = (i < poly2.size()) ? poly2[i] : 0;
        const int sum = a + b;
        if (sum == 0) continue;

        anyNonZero = true;
        if (!firstTerm) {
            result += (sum > 0) ? " + " : " - ";
        }
        result += std::to_string(std::abs(sum));
        result += "x^" + std::to_string(maxLen - 1 - i);
        firstTerm = false;
    }

    if (!anyNonZero) {
        return "0";
    }
    return result;
}
// The approach is to first determine the maximum length of the two input vectors, say `L = max(len1, len2)`. Since the vectors store terms from high degree to low, the term at index `i` has degree `L - 1 - i`. We iterate `i` from 0 to `L-1`, computing `sum = (i < len1 ? poly1[i] : 0) + (i < len2 ? poly2[i] : 0)`. Build the string piecewise: for each `sum` that is not zero (but careful: we must handle the case where all sums are zero — then return "0"), append the absolute value and the monomial. For the first non-zero term, do not prepend a sign; for subsequent non-zero terms, prepend `" + "` if sum > 0, else `" - "`. Edge cases: both input vectors may be empty, or all sums zero → return "0". Also, the first term could be negative; then we output just the absolute value without a leading minus (as the original code does), but subsequent terms get the proper sign. Time complexity is O(L) where L is the max length, and space complexity is O(1) auxiliary besides the output string, which is O(L) in size due to the number of terms.
