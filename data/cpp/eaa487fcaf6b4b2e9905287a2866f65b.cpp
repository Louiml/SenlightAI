Write a C++ function `int maxRepeatedPatternFactor(const std::string& s)` that takes a non-empty string `s` consisting of lowercase English letters and returns the maximum integer `k` such that `s` can be written as `t + t + ... + t` (`k` times) for some non-empty string `t`. If no such `k > 1` exists, return `1`. The input string may contain repeated characters and can be up to length 1,000,000, so your function must be efficient. Note that if the entire string is a single character, the answer is `1` unless the string is empty (which won't be tested). The function must handle cases like `"abcabcabc"` (returns `3`) and `"ababab"` (returns `3` or `2`? Actually `"ababab"` can be seen as `("ab")^3` or `("abab")^?` no, but the largest `k` is `3` because `ab` repeated 3 times). For `"aaaa"`, the answer is `4` because `"a"` repeated 4 times. For `"ababa"`, the answer is `1` because no non-trivial repetition exists.
#include <cassert>
#include <string>

// The solution function is declared above.

int main() {
    assert(maxRepeatedPatternFactor("a") == 1);
    assert(maxRepeatedPatternFactor("aa") == 2);
    assert(maxRepeatedPatternFactor("aaa") == 3);
    assert(maxRepeatedPatternFactor("aaaa") == 4);
    assert(maxRepeatedPatternFactor("ab") == 1);
    assert(maxRepeatedPatternFactor("abab") == 2);
    assert(maxRepeatedPatternFactor("abcabcabc") == 3);
    assert(maxRepeatedPatternFactor("ababab") == 3);
    assert(maxRepeatedPatternFactor("ababa") == 1);
    assert(maxRepeatedPatternFactor("xyzxyzxyzxyz") == 4);
    return 0;
}
#include <string>
#include <vector>

// Returns the maximum repetition factor k such that s = t^k for some non-empty t.
// If no such k > 1 exists, returns 1.
int maxRepeatedPatternFactor(const std::string& s) {
    int n = static_cast<int>(s.size());
    if (n == 0) return 0;  // Not expected but safe.

    // Compute KMP prefix function (pi array).
    std::vector<int> pi(n, 0);
    for (int i = 1; i < n; ++i) {
        int j = pi[i - 1];
        while (j > 0 && s[i] != s[j]) {
            j = pi[j - 1];
        }
        if (s[i] == s[j]) {
            ++j;
        }
        pi[i] = j;
    }

    // Length of the smallest candidate period.
    int period = n - pi[n - 1];
    if (n % period == 0) {
        return n / period;
    } else {
        return 1;
    }
}
// The problem is to find the largest number of repetitions of a substring that forms the whole string. This is equivalent to finding the smallest period `p` such that `n % p == 0` and the string consists of `n/p` copies of the prefix of length `p`. The standard KMP prefix-function (or "next" array) gives us the longest proper prefix that is also a suffix at each position. For the whole string, let `len = n - pi[n-1]` be the length of the shortest candidate period. If `n % len == 0`, then the string is exactly `n/len` copies of that prefix, and the maximum `k` is `n/len`. Otherwise, there is no non-trivial repetition, so return `1`. The key edge case is when `pi[n-1]` is zero, then `len = n`, and `n % n == 0` gives `k = 1`, which is correct. Another edge case: for `"ababab"`, `n=6`, `pi[5]=4`, `len=2`, `6%2==0` gives `k=3` (correct). For `"aaaa"`, `n=4`, `pi[3]=3`, `len=1`, `k=4`. For `"ababa"`, `n=5`, `pi[4]=3`, `len=2`, `5%2 != 0` so return 1. Time complexity is O(n) for KMP, space O(n) for the prefix array. The function must avoid allocating large dynamic memory repeatedly; using a vector of ints is fine.
