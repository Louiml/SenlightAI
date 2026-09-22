Given a string `s` consisting of lowercase English letters and two positive integers `n` and `m`, write a C++ function `std::string constructAnswer(int n, int m, const std::string& s)` that simulates the following process: first, count the frequency of each letter from `'a'` to `'z'` in `s`. Then, for each of the `m` rounds (rounds numbered `1` to `m`), find the smallest index `lst` (0‑based, where `0` corresponds to `'a'`) such that the count of that letter is zero, but also limit `lst` to not exceed `n/m` (integer division). Let `lst` be the minimum of that smallest zero‑count index and `n/m`. Then, for every index `i` from `0` to `lst-1` inclusive, decrement the count of letter `i` by one. Append the character `('a' + lst)` to the result string. After all `m` rounds, return the built string. The total length of `s` is exactly `n`. The function must handle cases where some letters appear zero times initially, and where `n` is not necessarily a multiple of `m`. The process assumes the counts never become negative because the input guarantees it is always possible to decrement the required number of letters (i.e., for each round, for every `i < lst`, the current count of letter `i` is at least 1). The function should be deterministic and efficient for `n, m` up to 200,000.

#include <cassert>
#include <string>

// Include the solution function declaration here (or copy the definition above)
std::string constructAnswer(int n, int m, const std::string& s);

int main() {
    // Example 1: n=5, m=2, s="ababa" -> counts a:3, b:2, others:0. limit=2.
    // Round1: first_zero=2 (c), lst=min(2,2)=2, dec a,b -> cnt a:2,b:1, char 'c'
    // Round2: first_zero=2 (still c? actually a:2>0,b:1>0,c:0), lst=2, dec a,b -> cnt a:1,b:0, char 'c'
    // result "cc"
    assert(constructAnswer(5, 2, "ababa") == "cc");

    // Example 2: n=3, m=3, s="abc" -> counts a,b,c each 1, others:0. limit=1.
    // Round1: first_zero=3 (d), lst=min(3,1)=1, dec a -> cnt a:0, char 'b'
    // Round2: first_zero=0 (a), lst=min(0,1)=0, no dec, char 'a'
    // Round3: first_zero=0, lst=0, char 'a'
    // result "baa"
    assert(constructAnswer(3, 3, "abc") == "baa");

    // Example 3: n=4, m=2, s="aabb" -> counts a:2,b:2, others:0. limit=2.
    // Round1: first_zero=2, lst=2, dec a,b -> cnt a:1,b:1, char 'c'
    // Round2: first_zero=2, lst=2, dec a,b -> cnt a:0,b:0, char 'c'
    // result "cc"
    assert(constructAnswer(4, 2, "aabb") == "cc");

    // Example 4: n=1, m=1, s="z" -> counts z:1, others:0. limit=1.
    // Round1: first_zero=0 (a), lst=min(0,1)=0, no dec, char 'a'
    // result "a"
    assert(constructAnswer(1, 1, "z") == "a");

    // Example 5: n=6, m=2, s="abcdef" -> counts a..f each 1, others:0. limit=3.
    // Round1: first_zero=6 (g), lst=min(6,3)=3, dec a,b,c -> cnt: a,b,c become 0, char 'd'
    // Round2: first_zero=0 (a), lst=min(0,3)=0, char 'a'
    // result "da"
    assert(constructAnswer(6, 2, "abcdef") == "da");

    // Example 6: n=10, m=5, s="aaaaaaaaaa" -> counts a:10, others:0. limit=2.
    // Each round: first_zero=1 (b), lst=min(1,2)=1, dec a -> cnt a decreases by 1 each round, char 'b'
    // After 5 rounds: result "bbbbb"
    assert(constructAnswer(10, 5, "aaaaaaaaaa") == "bbbbb");

    // Example 7: n=2, m=2, s="aa" -> counts a:2, others:0. limit=1.
    // Round1: first_zero=1, lst=1, dec a -> cnt a:1, char 'b'
    // Round2: first_zero=1, lst=1, dec a -> cnt a:0, char 'b'
    // result "bb"
    assert(constructAnswer(2, 2, "aa") == "bb");

    // Example 8: n=26, m=1, s="abcdefghijklmnopqrstuvwxyz" -> all counts 1. limit=26.
    // Round1: first_zero=26, lst=min(26,26)=26, dec all 26 letters -> each count becomes 0, char 'z'+1? 'a'+26 is outside lowercase, but 'a'+26 = '{' (char 123) but that's not a lowercase letter. However the problem says the output character is 'a'+lst, so it may be non-lowercase. That's fine.
    // The function will push_back('a'+26) which is '{', but for test we can just check length and that it's not empty.
    std::string r8 = constructAnswer(26, 1, "abcdefghijklmnopqrstuvwxyz");
    assert(r8.size() == 1 && r8[0] == 'a' + 26);

    // Example 9: n=3, m=1, s="abc" -> counts a,b,c each 1, others:0. limit=3.
    // first_zero=3, lst=min(3,3)=3, dec a,b,c -> all become 0, char 'a'+3='d'
    assert(constructAnswer(3, 1, "abc") == "d");

    // Example 10: n=0? But n is positive, skip. Also test n multiple of m with no zero until late.
    // n=4, m=4, s="abcd" -> counts each 1. limit=1.
    // Round1: first_zero=4, lst=1, dec a -> cnt a:0, char 'b'
    // Round2: first_zero=0, lst=0, char 'a'
    // Round3: first_zero=0, lst=0, char 'a'
    // Round4: first_zero=0, lst=0, char 'a' => "baaa"
    assert(constructAnswer(4, 4, "abcd") == "baaa");

    return 0;
}

#include <string>
#include <vector>
#include <algorithm>

std::string constructAnswer(int n, int m, const std::string& s) {
    std::vector<int> cnt(26, 0);
    for (char c : s) {
        cnt[c - 'a']++;
    }

    std::string result;
    result.reserve(m);

    int limit = n / m;

    for (int round = 0; round < m; ++round) {
        // Find the smallest index with zero count
        int first_zero = 0;
        while (first_zero < 26 && cnt[first_zero] > 0) {
            ++first_zero;
        }
        // If all 26 letters have positive counts, first_zero would be 26,
        // but we need to cap it.
        int lst = std::min(first_zero, limit);

        // Decrement counts for indices 0 to lst-1 (inclusive)
        for (int i = 0; i < lst; ++i) {
            cnt[i]--;
        }

        result.push_back(static_cast<char>('a' + lst));
    }

    return result;
}

// The core idea is to maintain a frequency array `cnt[26]` initialized from the input string `s`. For each of the `m` rounds, we need to determine the smallest letter index `j` such that `cnt[j] == 0` (i.e., that letter is completely exhausted). However, we also have the constraint that `lst` cannot exceed `limit = n / m` (integer division, where `n` and `m` are the given total length and number of rounds). So we compute `candidate1` as the first index from 0 upward where `cnt[index] == 0`, and `candidate2 = limit`. Then `lst = min(candidate1, candidate2)`. After determining `lst`, we decrement `cnt[i]` for every `i` from 0 to `lst-1` (inclusive). This decrement is always safe because the problem guarantees that the counts are sufficient. Then append the character `('a' + lst)` to the result. The algorithm runs in `O(26 * m + n)` time because each round scans up to 26 letters to find the first zero, and initialization scans the string of length `n`. The space complexity is `O(26)` for the frequency array plus the output string of length `m`. Edge cases: if `n/m` is 0, then `lst` becomes 0 because the first zero is at index 0 (unless all letters have positive counts, but if all have positive counts, then the first zero might be beyond 25 and `candidate1` should be 26, but then `min(26, 0)=0`, which is correct because you cannot select a letter beyond the limit). The process never accesses out-of-bounds because `lst` is at most `limit` and at most 26. Also, `lst` is guaranteed to be at least 1 for the first round if `n >= m` and the string contains enough distinct letters? Actually, the guarantee is that the decrement is always possible, so we trust the input. The function should be implemented in a standalone manner without a `main` function.
