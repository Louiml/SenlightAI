/*
Given a string `expression` that always contains exactly one `'+'` character and consists only of digits (so it represents a valid sum of two non-negative integers, e.g., `"247+38"`), write a C++ function `std::string minimizeResult(const std::string& expression)` that inserts exactly one pair of parentheses `'('` and `')'` into the string such that the resulting arithmetic expression evaluates to the **minimum possible value**. The parentheses must be placed so that the `'+'` remains inside them and at least one digit appears before the opening parenthesis and at least one digit after the closing parenthesis (so the form is `leftPart(leftMiddle+rightMiddle)rightPart`, where `leftPart`, `leftMiddle`, `rightMiddle`, and `rightPart` are non‑empty strings of digits, except that `leftPart` and `rightPart` may be empty strings – if empty, they contribute a factor of `1` to the multiplication). The expression is evaluated with standard operator precedence: parentheses first, then addition inside them, then multiplication of the three factors (leftPart × (leftMiddle+rightMiddle) × rightPart). Return the string with the single pair of parentheses inserted that yields the smallest result. If multiple placements yield the same minimum, return the one that appears first when scanning the possible placements in increasing order of the opening parenthesis index, and for equal opening index, increasing order of the closing parenthesis index (i.e., the first found in the double loop described below). The original order of digits and `'+'` must be preserved; only one pair of parentheses is added.
*/

#include <string>
#include <climits>
#include <cstdlib>

// Insert one pair of parentheses into "expression" (which is a sum of two
// non-negative integer strings) to minimize the value of leftPart *
// (leftMiddle + rightMiddle) * rightPart, where empty leftPart/rightPart
// contribute factor 1. Returns the string with the parentheses inserted.
std::string minimizeResult(const std::string& expression) {
    const std::size_t plusPos = expression.find('+');
    const std::size_t N = expression.size();
    int bestValue = INT_MAX;
    std::string bestResult;

    // The opening parenthesis can be placed at index 'a' (0 <= a < plusPos)
    for (std::size_t a = 0; a < plusPos; ++a) {
        // The left factor: empty (a == 0) is treated as 1
        const int leftFactor = (a == 0) ? 1 : std::stoi(expression.substr(0, a));
        const int leftMiddle = std::stoi(expression.substr(a, plusPos - a));

        // The closing parenthesis can be placed at index 'b' (plusPos+2 <= b <= N)
        for (std::size_t b = plusPos + 2; b <= N; ++b) {
            // The right factor: empty (b == N) is treated as 1
            const int rightFactor = (b == N) ? 1 : std::stoi(expression.substr(b));
            const int rightMiddle = std::stoi(expression.substr(plusPos + 1, b - plusPos - 1));

            const int current = leftFactor * (leftMiddle + rightMiddle) * rightFactor;
            if (current < bestValue) {
                bestValue = current;
                bestResult = expression.substr(0, a) + '(' +
                             expression.substr(a, plusPos - a) + '+' +
                             expression.substr(plusPos + 1, b - plusPos - 1) + ')' +
                             expression.substr(b);
            }
        }
    }
    return bestResult;
}

#include <cassert>
#include <string>

// The solution function is declared above (not repeated here).
int main() {
    // Basic case: 247+38, smallest is 2(47+3)8 → 2*(50)*8 = 800
    assert(minimizeResult("247+38") == "2(47+3)8");
    // Single digit each side: 1+1, only one possibility (1+1) → 1*(2)*1=2
    assert(minimizeResult("1+1") == "(1+1)");
    // Larger left side: 999+1, try (99+9)1? Actually best is 9(99+1) → 9*100=900
    assert(minimizeResult("999+1") == "9(99+1)");
    // Larger right side: 1+999, best is 1(1+99)9 → 1*100*9 = 900
    assert(minimizeResult("1+999") == "1(1+99)9");
    // All same digits: 111+111, best is 1(11+11)1 → 1*22*1 = 22
    assert(minimizeResult("111+111") == "1(11+11)1");
    // Zero involved: 0+123, best is (0+1)23 → 1*23 = 23
    assert(minimizeResult("0+123") == "(0+1)23");
    // Multiple digits, check tie-breaking (first found): 123+45, best is 1(23+4)5 → 1*27*5 = 135
    assert(minimizeResult("123+45") == "1(23+4)5");
    // Edge: long equal halves
    assert(minimizeResult("10+10") == "1(0+1)0");
    assert(minimizeResult("5+5") == "(5+5)");
    // More complex: 1234+5678, best is 1(234+5)678 → 1*239*678 = 162042
    assert(minimizeResult("1234+5678") == "1(234+5)678");
    return 0;
}

// We need to consider every feasible split of the string into four parts: `A` (before the opening parenthesis), `B` (from the opening parenthesis to just before `'+'`), `C` (from just after `'+'` to just before the closing parenthesis), and `D` (after the closing parenthesis). The original string has the form `DIGITS '+' DIGITS`. Let `p` be the index of `'+'`. The opening parenthesis must be placed at some index `a` where `0 ≤ a < p`, and the closing parenthesis at some index `b` where `p+2 ≤ b ≤ N` (where `N` is the length). Then `A = expression.substr(0,a)`, `B = expression.substr(a, p-a)`, `C = expression.substr(p+1, b-p-1)`, `D = expression.substr(b)`. The value is `valA * (valB + valC) * valD`, where empty `A` or `D` are treated as `1`. We iterate over all valid `(a,b)` pairs and compute the value using `stoi` on the substrings. Keep track of the best value and the corresponding constructed string. Edge cases: `A` or `D` can be empty (treated as factor 1); `B` and `C` are always non‑empty because `a < p` and `b > p+1`. The number of pairs is at most `(p) * (N-p-1)`, which for a length up to ~10 is small (max ~25). Time complexity: O(N^2) per test with substring conversion, where N is string length (≤ 10 in typical constraints). Space complexity: O(N) for the result string.
