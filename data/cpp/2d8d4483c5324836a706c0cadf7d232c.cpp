// Write a C++ function `int lcsLength(const std::string& a, const std::string& b)` that computes the length of the Longest Common Subsequence (LCS) between two strings. The strings may contain lowercase and uppercase letters, digits, and spaces, and may be empty. The function must use dynamic programming with memoization, where the memo table is sized dynamically to the input lengths. For any inputs, the function should return the correct LCS length; for example, `lcsLength("abc", "abc")` returns 3, `lcsLength("abc", "def")` returns 0, and `lcsLength("", "xyz")` returns 0. You may not use a global or static memo table; instead, create the table inside the function using `std::vector`. Use `const` references for inputs. The function must be self-contained (no main). Edge cases include one or both strings empty, and strings with repeated characters.
The LCS problem asks for the maximum length of a subsequence that appears in both strings in the same order. A standard recursive formulation is: for indices `i` and `j` into `a` and `b`, if either index reaches the end, the LCS length is 0. If `a[i] == b[j]`, the result is `1 + LCS(i+1, j+1)`. Otherwise, it is `max(LCS(i+1, j), LCS(i, j+1))`. Direct recursion is exponential; using memoization (top-down DP) with a 2D table of size `(n+1) x (m+1)` avoids recomputation. Since the table is sized to actual input lengths, we allocate `std::vector<std::vector<int>> memo(n+1, std::vector<int>(m+1, -1))` and fill it lazily. When `memo[i][j]` is not -1, we return it. This ensures each state is computed once. Edge cases: empty strings yield 0 because base case triggers immediately. Repeated characters are handled because the table stores the answer for each pair of indices. Time complexity is O(n*m) and space complexity is O(n*m) for the memo table; recursion depth is O(n+m) in worst case, but that is acceptable for typical constraints. An alternative iterative DP exists with the same complexity, but the task specifically requires memoization. We must ensure the function is `const`-correct: accepting `const std::string&` and not modifying inputs.
#include <vector>
#include <string>
#include <algorithm>

/*
 * Return the length of the longest common subsequence between strings `a` and `b`.
 * Uses top-down dynamic programming with memoization.
 */
int lcsLength(const std::string& a, const std::string& b) {
    int n = static_cast<int>(a.size());
    int m = static_cast<int>(b.size());
    
    // memo[i][j] = LCS length for a suffix starting at i and b suffix starting at j
    // -1 means not yet computed
    std::vector<std::vector<int>> memo(n + 1, std::vector<int>(m + 1, -1));
    
    // Recursive helper (lambda captures by reference)
    std::function<int(int,int)> lcs_rec = [&](int i, int j) -> int {
        // Base case: if either index is at end, LCS length is 0
        if (i == n || j == m) {
            return 0;
        }
        // Return cached result if available
        if (memo[i][j] != -1) {
            return memo[i][j];
        }
        // If characters match, take 1 plus the result of skipping both
        if (a[i] == b[j]) {
            memo[i][j] = 1 + lcs_rec(i + 1, j + 1);
        } else {
            // Otherwise, take the maximum of skipping one character from either string
            memo[i][j] = std::max(lcs_rec(i + 1, j), lcs_rec(i, j + 1));
        }
        return memo[i][j];
    };
    
    return lcs_rec(0, 0);
}
#include <cassert>
#include <string>
// Include the solution function here or via header

int main() {
    // Basic cases
    assert(lcsLength("abc", "abc") == 3);
    assert(lcsLength("abc", "def") == 0);
    assert(lcsLength("", "xyz") == 0);
    assert(lcsLength("xyz", "") == 0);
    assert(lcsLength("", "") == 0);
    
    // Mixed case and spaces
    assert(lcsLength("a1 b2", "a1 b2") == 6);
    assert(lcsLength("abc", "aBc") == 2);  // 'a' and 'c' match, 'b' != 'B'
    
    // Repeated characters
    assert(lcsLength("aaa", "aa") == 2);
    assert(lcsLength("abab", "baba") == 3);  // e.g., "aba" or "bab"
    
    // Longer strings
    assert(lcsLength("AGGTAB", "GXTXAYB") == 4);  // "GTAB"
    assert(lcsLength("ABCDGH", "AEDFHR") == 3);   // "ADH"
    
    return 0;
}
