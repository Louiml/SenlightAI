Write a C++ function named `minimumEditDistance` that takes two strings `s1` and `s2` as input and returns the minimum number of single-character operations (insertion, deletion, or substitution) required to transform `s1` into `s2`. The function must use a memoized recursive approach with a 2D dynamic programming table, following the same logic as the given snippet: if one string is exhausted, the cost is the length of the remaining part of the other string; if characters match, recurse diagonally; otherwise, take 1 plus the minimum of three recursive calls (deletion from `s1`, substitution, and insertion into `s1`). The solution must handle empty strings, strings with repeated characters, and lengths up to 5000, using a global memoization table initialized to -1. The function should be `const`-correct and avoid unnecessary copies of strings.

#include <assert.h>
#include <bits/stdc++.h>
using namespace std;

// Assume minimumEditDistance is declared above

int main() {
    assert(minimumEditDistance("abc", "abc") == 0);
    assert(minimumEditDistance("", "") == 0);
    assert(minimumEditDistance("a", "") == 1);
    assert(minimumEditDistance("", "abc") == 3);
    assert(minimumEditDistance("horse", "ros") == 3);
    assert(minimumEditDistance("intention", "execution") == 5);
    assert(minimumEditDistance("kitten", "sitting") == 3);
    assert(minimumEditDistance("flaw", "lawn") == 2);
    assert(minimumEditDistance("distance", "difference") == 5);
    assert(minimumEditDistance("abcde", "edcba") == 4);
    return 0;
}

#include <bits/stdc++.h>
using namespace std;

long long dp[5001][5001]; // Memoization table, initialized to -1

// Recursive helper with memoization
long long editDistanceHelper(int i, int j, const string& s1, const string& s2) {
    // Base case: one string exhausted
    if (i == s1.size() && j == s2.size()) return 0;
    if (i == s1.size() || j == s2.size()) {
        return max(s1.size() - i, s2.size() - j);
    }
    if (dp[i][j] != -1) return dp[i][j];

    long long ans;
    if (s1[i] == s2[j]) {
        ans = editDistanceHelper(i + 1, j + 1, s1, s2);
    } else {
        ans = 1 + min(editDistanceHelper(i + 1, j, s1, s2),
                 min(editDistanceHelper(i + 1, j + 1, s1, s2),
                     editDistanceHelper(i, j + 1, s1, s2)));
    }
    return dp[i][j] = ans;
}

// Public function that initializes memoization and calls the helper
long long minimumEditDistance(const string& s1, const string& s2) {
    memset(dp, -1, sizeof(dp));
    return editDistanceHelper(0, 0, s1, s2);
}

// The problem is the classic edit distance (Levenshtein distance) problem, solved via recursion with memoization. The recursive state is defined by indices `(i, j)` representing positions in `s1` and `s2`. The base case occurs when either index reaches the end of its string: if `i == s1.size()` and `j == s2.size()`, cost is 0; if only one is exhausted, we must delete all remaining characters from the longer string, so the cost is `max(s1.size() - i, s2.size() - j)`. For the recursive step, if `s1[i] == s2[j]`, no operation is needed and we move both indices forward. Otherwise, we consider three possible operations: deletion (move `i` forward, cost 1), substitution (move both `i` and `j` forward, cost 1), and insertion (move `j` forward, cost 1). We take the minimum of these three. To avoid recomputing overlapping subproblems, we store results in a 2D array `dp[i][j]`, initialized to -1, and return the cached value if available. Edge cases include empty strings (where the answer is the length of the other string), strings of length 1, and fully identical strings (cost 0). Time complexity is O(n*m) where n = s1 length and m = s2 length, due to each pair of indices being computed at most once. Space complexity is O(n*m) for the memoization table, plus O(n*m) for recursion stack in the worst case (though recursion depth is at most n+m). The provided snippet uses `int` as 64-bit via `#define int long long`, but for constraints up to 5000, 32-bit int is sufficient; still, we keep `long long` for safety. A key improvement is passing strings by const reference to avoid copying.
