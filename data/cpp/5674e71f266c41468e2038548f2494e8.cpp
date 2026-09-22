Write a C++ function named `canFormPalindromeSubstrings` that takes a non-empty string `s` and a positive integer `k`, and returns a Boolean value indicating whether it is possible to partition (not necessarily contiguous) all characters of `s` into exactly `k` non-empty palindrome strings. The function must solve the problem using only the frequency of characters and without constructing actual palindromes. The input may contain lowercase English letters only. A valid solution exists if the number of characters that appear an odd number of times is no greater than `k`. This works because each palindrome can contain at most one character with an odd frequency (the center), so odd-count characters must fit within the `k` available palindromes. If `s` has fewer characters than `k`, the task is impossible because each palindrome needs at least one character. The function must be self-contained, efficient for strings up to 10^5 characters, and use constant auxiliary space.
#include <cassert>
#include <string>

// Function declaration from the solution (included directly for test clarity).
bool canFormPalindromeSubstrings(const std::string& s, int k);

int main() {
    // Basic single character cases
    assert(canFormPalindromeSubstrings("a", 1) == true);
    assert(canFormPalindromeSubstrings("a", 2) == false); // not enough characters

    // Even frequency string can be one palindrome
    assert(canFormPalindromeSubstrings("aabb", 1) == true);
    assert(canFormPalindromeSubstrings("aabb", 2) == true); // can split into two palindromes

    // Odd frequency characters limit k
    assert(canFormPalindromeSubstrings("abc", 1) == false); // three odd counts need at least 3
    assert(canFormPalindromeSubstrings("abc", 3) == true);

    // Mixed case: two odd counts
    assert(canFormPalindromeSubstrings("aab", 2) == true); // 'a' twice, 'b' once -> k=2 ok
    assert(canFormPalindromeSubstrings("aab", 1) == false);

    // k larger than string length
    assert(canFormPalindromeSubstrings("ab", 3) == false);

    // Long string with many pairs and one odd
    assert(canFormPalindromeSubstrings("aaabbbcccddd", 3) == true); // 'a','b','c','d' all odd -> 4 odd counts, k=3 false
    assert(canFormPalindromeSubstrings("aaabbbcccddd", 4) == true);

    // All same character, always possible with k>=1
    assert(canFormPalindromeSubstrings("aaaa", 2) == true);
    assert(canFormPalindromeSubstrings("aaaa", 1) == true);

    return 0;
}
#include <string>
#include <vector>

// Returns true if all characters of s can be rearranged into exactly k non-empty palindromes.
// Uses the fact that each palindrome can have at most one character with odd frequency.
bool canFormPalindromeSubstrings(const std::string& s, int k) {
    if (static_cast<int>(s.size()) < k) {
        return false;
    }

    std::vector<int> frequency(26, 0);
    for (char ch : s) {
        frequency[ch - 'a']++;
    }

    int oddCount = 0;
    for (int count : frequency) {
        if (count % 2 != 0) {
            oddCount++;
        }
    }

    return oddCount <= k;
}
// The core insight is that a palindrome, by definition, has at most one character with an odd frequency (the middle character when the length is odd; zero odd-frequency characters when the length is even). Therefore, if we know how many distinct characters in `s` appear an odd number of times, say `oddCount`, then to construct `k` palindromes using all characters, we need at least `oddCount` palindromes to accommodate those odd characters as centers. The remaining characters (all even counts) can be distributed arbitrarily among the palindromes without affecting parity. Thus, a necessary and sufficient condition is `oddCount <= k`. Additionally, since each palindrome must contain at least one character, if `s.length() < k`, it’s impossible. Edge cases: `k` may be larger than the string length (return false), `k` may equal the string length (true if each character can be its own palindrome, which requires `oddCount <= k`; note `oddCount <= length` always, so it works), and strings with all even frequencies (e.g., "aabb") allow `k` ≥ 0 but since `k` is positive, if `k` is 1, it’s possible because the whole string can be rearranged into a palindrome. Time complexity is O(n + 26) = O(n) for counting, and O(1) auxiliary space for the frequency array. No special handling for `k` being 0 is needed because the problem states `k` is positive.
