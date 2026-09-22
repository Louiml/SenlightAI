Write a C++ function that generates a LaTeX-style table displaying the sequence `f(k)` for `k = 1` to `10`, where `f(1) = 1`, and for `k >= 2`: if `k` is even, `f(k) = f(k/2) + 1`; if `k` is odd, `f(k) = 1 / f(k-1)`. The function must return a `std::string` containing the full LaTeX table code (including `$$`, `\begin{array}`, `\hline`, etc.) exactly as shown in the snippet, but with correctly formatted fractions (using `\frac{a}{b}`) when not an integer, and plain integers otherwise. Do not include any leading/trailing whitespace beyond the necessary LaTeX formatting. The function signature is `std::string latexFractionTable()`.
#include <cassert>
#include <string>

// The solution function is declared above.

int main() {
    std::string table = latexFractionTable();
    // Check that the table contains expected content.
    assert(table.find("$$") == 0);
    assert(table.rfind("$$") == table.size() - 2);
    assert(table.find("\\begin{array}{c|c|c|c|c|c|c|c|c|c|c}") != std::string::npos);
    assert(table.find("k & 1 & 2 & 3 & 4 & 5 & 6 & 7 & 8 & 9 & 10") != std::string::npos);
    // Verify specific values:
    // f(1)=1, f(2)=2, f(3)=1/2, f(4)=3, f(5)=2/3, f(6)=3/2, f(7)=2/3? Let's compute manually:
    // f1=1
    // f2 = f1+1 = 2
    // f3 = 1/f2 = 1/2
    // f4 = f2+1 = 3
    // f5 = 1/f4 = 1/3
    // f6 = f3+1 = 1/2+1 = 3/2
    // f7 = 1/f6 = 2/3
    // f8 = f4+1 = 4
    // f9 = 1/f8 = 1/4
    // f10 = f5+1 = 1/3+1 = 4/3
    assert(table.find("\\frac{1}{2}") != std::string::npos);
    assert(table.find("\\frac{1}{3}") != std::string::npos);
    assert(table.find("\\frac{3}{2}") != std::string::npos);
    assert(table.find("\\frac{2}{3}") != std::string::npos);
    assert(table.find("\\frac{1}{4}") != std::string::npos);
    assert(table.find("\\frac{4}{3}") != std::string::npos);
    // Integers should appear as plain numbers, not fractions.
    assert(table.find("\\frac{1}{1}") == std::string::npos);
    assert(table.find("\\frac{2}{1}") == std::string::npos);
    assert(table.find("\\frac{3}{1}") == std::string::npos);
    assert(table.find("\\frac{4}{1}") == std::string::npos);
    // Check that there are no extra or missing separators.
    // Count occurrences of " & " should be 20 (10 in header row, 10 in f row).
    int count = 0;
    size_t pos = 0;
    while ((pos = table.find(" & ", pos)) != std::string::npos) {
        ++count; ++pos;
    }
    assert(count == 20);
}
#include <string>
#include <vector>
#include <numeric> // for std::gcd (C++17)

struct Fraction {
    long long num;
    long long den;
    Fraction(long long n = 0, long long d = 1) : num(n), den(d) {
        reduce();
    }
    void reduce() {
        if (den < 0) { num = -num; den = -den; }
        long long g = std::gcd(std::abs(num), std::abs(den));
        if (g != 0) { num /= g; den /= g; }
        if (num == 0) den = 1;
    }
    Fraction operator+(const Fraction& other) const {
        return Fraction(num * other.den + other.num * den, den * other.den);
    }
    Fraction operator/(const Fraction& other) const {
        return Fraction(num * other.den, den * other.num);
    }
};

std::string latexFractionTable() {
    const int maxK = 10;
    std::vector<Fraction> f(maxK + 1);
    f[1] = Fraction(1, 1);
    for (int k = 2; k <= maxK; ++k) {
        if (k % 2 == 0) {
            f[k] = f[k / 2] + Fraction(1, 1);
        } else {
            f[k] = Fraction(1, 1) / f[k - 1];
        }
    }

    std::string result = "$$";
    result += "\\begin{array}{c";
    for (int i = 1; i <= maxK; ++i) result += "|c";
    result += "}\n";

    // Header row
    result += "k";
    for (int i = 1; i <= maxK; ++i) result += " & " + std::to_string(i);
    result += " \\\\ \\hline\n";

    // f(k) row
    result += "f(k)";
    for (int i = 1; i <= maxK; ++i) {
        result += " & ";
        if (f[i].num % f[i].den == 0) {
            result += std::to_string(f[i].num / f[i].den);
        } else {
            result += "\\frac{" + std::to_string(f[i].num) + "}{" + std::to_string(f[i].den) + "}";
        }
    }
    result += " \\\\\n";
    result += "\\end{array}\n";
    result += "$$";

    return result;
}
// The core problem is to compute the sequence correctly and then format it into a LaTeX array. The recurrence is:  
// - Base: `f(1) = 1` (represented as fraction 1/1).  
// - For even `k > 1`: `f(k) = f(k/2) + 1`, where the addition is fraction addition: `a/b + 1 = (a + b)/b`.  
// - For odd `k > 1`: `f(k) = 1 / f(k-1)`, where division is fraction division: `1 / (a/b) = b/a`.  
//
// All operations must keep fractions in reduced form (divide numerator and denominator by their greatest common divisor). Edge cases: the denominator of `f(k)` can become 1 (making it an integer), so when formatting, check if numerator % denominator == 0 and print the integer; otherwise print `\frac{a}{b}` without simplification (since the fraction is already reduced). The table structure is fixed: first row is `k` values 1..10, second row is `f(k)` values. The LaTeX code uses `$$` at the start and end, `\begin{array}{c|c|c...|c}` with 10 `|c` columns after the first `c`, a horizontal line after the header row, and ends with `\end{array}` and `$$`. The solution computes the array of fractions iteratively (each term depends only on previous values—either `f[k-1]` for odd, or `f[k/2]` for even), so it runs in O(10) = O(1) time and O(1) space. The only tricky part is string formatting, which is straightforward.
