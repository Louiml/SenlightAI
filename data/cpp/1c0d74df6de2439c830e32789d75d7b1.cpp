Write a C++ function named `minimumMinutes` that takes a string `s` consisting only of lowercase characters 'a', 'b', and 'c', and an integer `k` (0 ≤ k ≤ s.length()). The function must return the minimum number of minutes required to collect at least `k` occurrences of each character by taking characters from either end of the string. In one minute, you can take exactly one character from either the left end or the right end of the current remaining string. If it is impossible to collect `k` of each character (i.e., the total count of any character in the entire string is less than `k`), return -1. Otherwise, return the minimum total number of characters taken.

#include <cassert>

int main() {
    // Example from the prompt.
    assert(minimumMinutes("aabaaaacaabc", 2) == 5);
    // Need more than total available.
    assert(minimumMinutes("a", 2) == -1);
    assert(minimumMinutes("abc", 1) == 3);
    // k=0 requires no characters.
    assert(minimumMinutes("abc", 0) == 0);
    // All same character.
    assert(minimumMinutes("aaa", 2) == 2);
    assert(minimumMinutes("aaa", 3) == 3);
    // Mixed, only take from one side if enough.
    assert(minimumMinutes("abccba", 1) == 3); // e.g., take "abc" from left and right? Actually minimum is 3: take "aba"? Let's verify: take left a, left b, right a => ends up with "ccb"? Better: take "abc" from left (3) gives a,b,c; right side unaffected. So 3 works.
    // All characters needed.
    assert(minimumMinutes("cba", 1) == 3);
    // Single character insufficient.
    assert(minimumMinutes("b", 1) == -1);
    // Long string with redundant middle.
    assert(minimumMinutes("aaabbbccc", 1) == 3); // take "abc" from ends? Actually take left a, left a, left a? No, need b and c. Take left a, left a, left a? That gives 3 a's but no b or c. Need at least one of each. Minimum is 3: take "abc" by taking left a, left b? Not adjacent. Actually take left a, left b? No. Take left "a" (1), left "a" (2), left "a" (3) – no b or c. Better: take left "a" (1) then right "c" (2) then right "b" (3) – that takes a,c,b from ends, leaving middle "aabbcc"? Wait string is "aaabbbccc". Take left a, left a, left a? No. Exact minimum: take left a (1), left a (2), left a (3)? No b,c. Take left a (1), right c (2), right c (3) – gives two c, one a, no b. Minimum is 3? Let's check: take left a, left a, right c => have a,a,c – no b. Take left a, left b? No, b is not at end. Actually the string: indices 0:a,1:a,2:a,3:b,4:b,5:b,6:c,7:c,8:c. Take left a (1), left a (2), left a (3) – no b,c. Take right c (1), right c (2), left a (3) – have a,c,c – no b. Need b. Take left a, left a, then right c, then right c? That's 4. But can take left a, then right c, then right b? Right b is at index 5, but after taking right c (index 8) and right c (index 7) and right b (index 6?) Actually to get b from right, index 6 is c, index 5 is b. Take right c (1), right c (2), right b (3) – that's three, gives c,c,b – no a. Take left a (1), right c (2), right b (3) – gives a,c,b – that's exactly one of each! Yes, that's 3. So answer 3.
    assert(minimumMinutes("aaabbbccc", 1) == 3);
    // All characters same, need more than available.
    assert(minimumMinutes("aaa", 4) == -1);
    return 0;
}

#include <string>
#include <algorithm>

// Returns the minimum number of characters that must be taken from either end
// to collect at least k of each 'a', 'b', and 'c'. Returns -1 if impossible.
int minimumMinutes(const std::string& s, int k) {
    const int n = static_cast<int>(s.size());
    int total_a = 0, total_b = 0, total_c = 0;
    for (char ch : s) {
        if (ch == 'a') ++total_a;
        else if (ch == 'b') ++total_b;
        else ++total_c;
    }
    if (total_a < k || total_b < k || total_c < k) {
        return -1;
    }
    if (k == 0) {
        return 0;
    }

    // Counts inside the current middle substring (window).
    int cnt_a = 0, cnt_b = 0, cnt_c = 0;
    int max_window = 0;
    int i = 0;

    for (int j = 0; j < n; ++j) {
        // Add s[j] to the middle substring.
        if (s[j] == 'a') ++cnt_a;
        else if (s[j] == 'b') ++cnt_b;
        else ++cnt_c;

        // Shrink from left while the outside counts would drop below k.
        while (i <= j &&
               (total_a - cnt_a < k || total_b - cnt_b < k || total_c - cnt_c < k)) {
            if (s[i] == 'a') --cnt_a;
            else if (s[i] == 'b') --cnt_b;
            else --cnt_c;
            ++i;
        }

        max_window = std::max(max_window, j - i + 1);
    }

    return n - max_window;
}

// The direct approach of trying all possible combinations of taking from left and right is exponential. Instead, observe that the characters you take from the ends form a contiguous block that remains **after** you remove a contiguous middle substring. Specifically, if we remove a contiguous substring from the original string (the part we do *not* take), then all characters outside that substring are exactly the ones taken from the ends. Therefore, to minimize the number of taken characters, we want to maximize the length of the contiguous middle substring that we leave untouched, subject to the condition that the counts of 'a', 'b', and 'c' **outside** that substring are each at least `k`. Equivalently, the counts inside the removed substring must be at most (total_a - k), (total_b - k), (total_c - k). We use a sliding window (two pointers) over the string to find the longest contiguous substring whose character counts do not exceed these upper bounds. For each right pointer `j`, we decrement counts for `s[j]` (simulating that the substring is removed). If any count falls below the required surplus (i.e., outside substring would have fewer than `k`), we move the left pointer `i` forward, incrementing counts back, until all counts are acceptable again. The maximum valid window length is `maxi`. The answer is `n - maxi`, because all characters outside the chosen middle substring must be taken. Edge cases: if any total count is less than `k`, return -1. If `k == 0`, the answer is 0 because no characters need to be taken (the maximum window can be the entire string, and `n - n = 0`). Time complexity is O(n) because each pointer moves at most n times. Space complexity is O(1) beyond the input string.
