// Given two strings `a` and `b` of equal length `n`, each consisting of digits (`'0'`–`'9'`) and question marks (`'?'`), write a C++ function that replaces every `'?'` in both strings with a single digit such that the resulting two numbers have the smallest possible absolute difference. If multiple pairs achieve the same minimal difference, choose the one with the smaller first number; if still tied, choose the smaller second number. The function must return a `std::pair<std::string, std::string>` containing the fully resolved strings (with no `'?'` remaining), preserving leading zeros. All inputs are guaranteed to have at least one valid resolution.

// The problem can be solved by iterating over each position `i` (0-indexed) as the first position where the two final numbers may differ. At positions before `i`, both numbers must be identical, so we set each pair of corresponding characters to a common resolved digit using a helper that handles `?` combinations. At position `i`, we try all ordered digit pairs `(va, vb)` with `va != vb` (unless `i` is the last position, where we also allow `va == vb` because no later digits can affect the comparison; actually for `i == n-1` we can also allow equality, but the general loop includes it naturally). For all positions after `i`, we greedily set the smaller-number side to its maximum possible digit (if `?` → `9`, otherwise fixed) and the larger-number side to its minimum possible digit (if `?` → `0`, otherwise fixed) to minimize the difference. If any position before or at `i` cannot be resolved consistently, that candidate is skipped. For each valid candidate pair, we compute the absolute difference and maintain the best `(A, B)` pair according to the tie-breaking rule: smaller absolute difference first, then smaller `A`, then smaller `B`. Since both strings have length up to 18 (typical constraints in similar competitive problems), the number of candidates is at most `n * 10 * 10` = O(n · 100), and each candidate requires O(n) work to build the resulting numbers. Therefore time complexity is O(n² · 100) ≈ O(n²) and space is O(n) for the strings. Edge cases include all `?` strings, equal fixed digits, and cases where the first differing position is forced to be equal (only when `n=1`).

#include <string>
#include <utility>
#include <algorithm>
#include <cstdlib>

// Resolve two strings of digits and '?' into numbers with minimal absolute difference.
// Returns a pair of strings with all '?' replaced by digits, preserving leading zeros.
std::pair<std::string, std::string> minimizeDifference(const std::string& a, const std::string& b) {
    const int n = static_cast<int>(a.size());
    const long long INF = 1e18;

    // Helper: resolve a single position where both sides must be equal.
    auto matchEqual = [](char x, char y) -> char {
        if (x == '?' && y == '?') return '0';
        if (x == '?') return y;
        if (y == '?') return x;
        if (x == y) return x;
        return '-'; // conflict
    };

    // Positive infinity for comparison.
    auto evaluate = [](long long A, long long B) {
        return std::make_tuple(std::llabs(A - B), A, B);
    };

    std::pair<std::string, std::string> best;
    long long bestDiff = INF, bestA = INF, bestB = INF;

    // Try each position i as the first where the two numbers differ.
    for (int i = 0; i < n; ++i) {
        for (int va = 0; va <= 9; ++va) {
            for (int vb = 0; vb <= 9; ++vb) {
                // If we are not at the last position, va and vb must be different.
                if (va == vb && i != n - 1) continue;

                // Build numbers digit by digit.
                long long A = 0, B = 0;
                bool ok = true;
                for (int j = 0; j < n; ++j) {
                    if (j < i) {
                        // Both sides must be identical here.
                        char m = matchEqual(a[j], b[j]);
                        if (m == '-') { ok = false; break; }
                        A = 10 * A + (m - '0');
                        B = 10 * B + (m - '0');
                    } else if (j == i) {
                        // Set the chosen different digits.
                        if (a[j] != '?' && a[j] - '0' != va) { ok = false; break; }
                        if (b[j] != '?' && b[j] - '0' != vb) { ok = false; break; }
                        A = 10 * A + va;
                        B = 10 * B + vb;
                    } else {
                        // After the differing position, minimize the difference.
                        if (va < vb) {
                            // A is smaller, so maximize A's digits, minimize B's.
                            if (a[j] == '?') A = 10 * A + 9; else A = 10 * A + (a[j] - '0');
                            if (b[j] == '?') B = 10 * B + 0; else B = 10 * B + (b[j] - '0');
                        } else {
                            // B is smaller, so maximize B's digits, minimize A's.
                            if (a[j] == '?') A = 10 * A + 0; else A = 10 * A + (a[j] - '0');
                            if (b[j] == '?') B = 10 * B + 9; else B = 10 * B + (b[j] - '0');
                        }
                    }
                }
                if (!ok) continue;

                // Compare with current best.
                auto cur = evaluate(A, B);
                auto bestCur = evaluate(bestA, bestB);
                if (cur < bestCur) {
                    bestA = A;
                    bestB = B;
                    bestDiff = std::llabs(A - B);
                }
            }
        }
    }

    // Convert numbers back to strings with leading zeros.
    auto to_string_with_zeros = [n](long long val) {
        std::string s = std::to_string(val);
        while (static_cast<int>(s.size()) < n) s = "0" + s;
        return s;
    };

    best.first = to_string_with_zeros(bestA);
    best.second = to_string_with_zeros(bestB);
    return best;
}

#include <cassert>
#include <string>
#include <utility>

std::pair<std::string, std::string> minimizeDifference(const std::string& a, const std::string& b);

int main() {
    assert(minimizeDifference("?", "?") == std::make_pair("0", "0"));
    assert(minimizeDifference("1?", "?2") == std::make_pair("10", "12"));
    assert(minimizeDifference("?5", "3?") == std::make_pair("05", "39"));
    assert(minimizeDifference("?0?", "?9?") == std::make_pair("007", "090"));
    assert(minimizeDifference("123", "123") == std::make_pair("123", "123"));
    assert(minimizeDifference("???", "???") == std::make_pair("000", "000"));
    assert(minimizeDifference("9?9", "8?8") == std::make_pair("909", "808"));
    assert(minimizeDifference("?1?", "?0?") == std::make_pair("010", "009"));
    assert(minimizeDifference("?1?", "?2?") == std::make_pair("010", "120"));
    assert(minimizeDifference("?999", "?000") == std::make_pair("0999", "1000"));
    return 0;
}
