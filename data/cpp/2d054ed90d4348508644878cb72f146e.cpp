// Write a C++ function `string longestPalindromeWithSymmetricEdges(const string& s)` that, given a non-empty string `s` of lowercase English letters, returns a longest substring of `s` that can be expressed as `A + P + reverse(A)`, where `P` is a palindrome (including odd-length palindromes centered at one character or even-length palindromes centered between two characters) and `A` is a (possibly empty) prefix of the overall string. In other words, the chosen substring must consist of a palindrome in the middle, and outside that palindrome, the characters (if any) must match the corresponding characters at the very beginning and very end of the original string. More precisely: Let the palindrome be the substring `s[l..r]` (indexing from 0). Let `x = min(l, n-1-r)`. The candidate substring is `s[0..x-1] + s[l..r] + s[n-x..n-1]`. You must return the longest such candidate substring (if multiple have the same maximum length, return any one). The function should not modify the input and must handle strings up to length 200,000 efficiently.
The main algorithm uses Manacher’s algorithm to compute, for every center, the radius of the longest odd-length palindrome (`d1[i]` = number of characters including the center) and the radius of the longest even-length palindrome (`d2[i]` = number of pairs). Manacher runs in O(n) time. After computing `d1` and `d2`, we need to consider every possible palindrome center and determine the maximum symmetric extension possible. For an odd palindrome centered at `i`, the palindrome spans `[i-d1[i]+1, i+d1[i]-1]`. Let `a = i-d1[i]+1` and `b = i+d1[i]-1`. The number of characters available on the left outside the palindrome is `l = a`, and on the right outside is `r = n-1-b`. We can take at most `mn = min(l, r)` characters from both ends, but only if those outer characters match the corresponding prefix and suffix. To quickly determine how many characters from the ends match, precompute the longest common prefix length (`mc`) between the original string and its reverse, i.e., the number of positions `i` from 0 upward such that `s[i] == s[n-1-i]` consecutively. If `mn <= mc`, then extending by `mn` works, and the length of the candidate is `L + 2*mn`, where `L = b-a+1`. If `mn > mc`, we can only extend by `mc` characters (since beyond that the outer characters don't match), but the code in the snippet assumes that the palindrome itself is chosen such that `mn <= mc`; note that the original snippet only checks `mn <= mc`. Actually, for correctness we must allow extending by at most `mc`, not necessarily `mn`. So the correct condition is: the number of usable outer characters is `take = min(mn, mc)`. The candidate length is `L + 2*take`. We iterate over all odd centers and all even centers (where `d2[i] > 0`), and keep track of the maximum length. Once we have the best center and parity, we reconstruct the answer string by taking the palindrome substring `s[a..b]` and prepending/appending the first and last `take` characters. Since we only need the maximum length, we can simply record the best `(length, type, center)` and after the loop build the result. Edge cases: a single character palindrome (length 1) with no symmetric edges is valid (take=0). Even-length palindromes require `d2[i] > 0`. Also note that the palindrome must be entirely within the string, which is guaranteed by the Manacher arrays. Time complexity is O(n) for Manacher and O(n) for scanning, and O(n) space.
#include <string>
#include <vector>
#include <algorithm>

// Return a longest substring of s that is of the form A + P + reverse(A),
// where P is a palindrome (odd or even centered), and A is a prefix of s
// that is also a suffix of s when considered from the ends.
// The substring is defined as: take first k and last k characters of s,
// where k = min(center_left, center_right, longest_common_prefix_of_s_and_reverse(s)),
// surrounding the palindrome s[l..r].
std::string longestPalindromeWithSymmetricEdges(const std::string& s) {
    int n = static_cast<int>(s.size());
    
    // Manacher's algorithm for odd-length palindromes: d1[i] = radius including center
    std::vector<int> d1(n);
    int l = 0, r = -1;
    for (int i = 0; i < n; ++i) {
        int k = (i > r) ? 1 : std::min(d1[l + r - i], r - i + 1);
        while (i + k < n && i - k >= 0 && s[i + k] == s[i - k]) ++k;
        d1[i] = k;
        if (i + k - 1 > r) {
            l = i - k + 1;
            r = i + k - 1;
        }
    }
    
    // Manacher's algorithm for even-length palindromes: d2[i] = radius (number of pairs)
    std::vector<int> d2(n);
    l = 0; r = -1;
    for (int i = 0; i < n; ++i) {
        int k = (i > r) ? 0 : std::min(d2[l + r - i + 1], r - i + 1);
        while (i + k < n && i - k - 1 >= 0 && s[i + k] == s[i - k - 1]) ++k;
        d2[i] = k;
        if (i + k - 1 > r) {
            l = i - k;
            r = i + k - 1;
        }
    }
    
    // Compute longest common prefix between s and its reverse (number of matching outer pairs)
    int mc = 0;
    for (int i = 0, j = n - 1; i < j && s[i] == s[j]; ++i, --j) {
        ++mc;
    }
    
    int best_len = 0;
    int best_type = 1; // 1 for odd, 2 for even
    int best_center = 0;
    
    // Check odd-length palindromes
    for (int i = 0; i < n; ++i) {
        int a = i - d1[i] + 1;
        int b = i + d1[i] - 1;
        int L = b - a + 1;
        int left_avail = a;
        int right_avail = n - 1 - b;
        int take = std::min(std::min(left_avail, right_avail), mc);
        int cand = L + 2 * take;
        if (cand > best_len) {
            best_len = cand;
            best_type = 1;
            best_center = i;
        }
    }
    
    // Check even-length palindromes
    for (int i = 1; i < n; ++i) {
        if (d2[i] == 0) continue;
        int a = i - d2[i];
        int b = i + d2[i] - 1;
        int L = b - a + 1;
        int left_avail = a;
        int right_avail = n - 1 - b;
        int take = std::min(std::min(left_avail, right_avail), mc);
        int cand = L + 2 * take;
        if (cand > best_len) {
            best_len = cand;
            best_type = 2;
            best_center = i;
        }
    }
    
    // Reconstruct the best result
    int a, b, take;
    if (best_type == 1) {
        int i = best_center;
        a = i - d1[i] + 1;
        b = i + d1[i] - 1;
        take = std::min(std::min(a, n - 1 - b), mc);
    } else {
        int i = best_center;
        a = i - d2[i];
        b = i + d2[i] - 1;
        take = std::min(std::min(a, n - 1 - b), mc);
    }
    
    std::string result = s.substr(0, take) + s.substr(a, b - a + 1) + s.substr(n - take);
    return result;
}
#include <cassert>
#include <string>
#include <iostream>

// Include the solution function here or link it.
// For the test, we declare it externally.
std::string longestPalindromeWithSymmetricEdges(const std::string& s);

int main() {
    // Single character
    assert(longestPalindromeWithSymmetricEdges("a") == "a");
    // Two same characters
    assert(longestPalindromeWithSymmetricEdges("aa") == "aa");
    // Two different characters: best is a single character
    assert(longestPalindromeWithSymmetricEdges("ab") == "a"); // or "b", both length 1
    // Palindrome in middle with symmetric edges
    assert(longestPalindromeWithSymmetricEdges("abcba") == "abcba"); // whole string is palindrome
    // Need to extend a palindrome with matching ends: "abacaba" -> whole string palindrome
    assert(longestPalindromeWithSymmetricEdges("abacaba") == "abacaba");
    // "xabayx" -> the middle "aba" can extend by exactly 1 on each side? s[0]='x', s[5]='x', s[1]='a', s[4]='y' so only 1. result "xabax"
    assert(longestPalindromeWithSymmetricEdges("xabayx") == "xabax");
    // "xyzzyxw" -> middle "yzzy" is even palindrome, symmetric edges: s[0]='x', s[6]='w' no match, so only palindrome itself length 4 -> "yzzy"
    assert(longestPalindromeWithSymmetricEdges("xyzzyxw") == "yzzy");
    // "aaaa" -> entire string is palindrome, length 4
    assert(longestPalindromeWithSymmetricEdges("aaaa") == "aaaa");
    // "abacdfgdcaba" -> longest palindrome is "aba" at start? Actually "aba" at positions 0-2 and 10-12, but with symmetric edges? Let's test: The whole string has prefix "abac" and suffix "caba"? Not matching. Best is "aba" length 3 with take=0. Also "aba" at end. So length 3.
    assert(longestPalindromeWithSymmetricEdges("abacdfgdcaba").size() == 3);
    // Long string for performance, but just a simple case
    assert(longestPalindromeWithSymmetricEdges("racecar") == "racecar");
    
    std::cout << "All tests passed!" << std::endl;
    return 0;
}
