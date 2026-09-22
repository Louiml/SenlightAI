Implement a C++ function `findPatternOccurrences` that takes a text string and a pattern string as parameters and returns a `std::vector<int>` containing all the starting indices (0-based) where the pattern appears as a substring in the text. The function must use the Rabin-Karp string matching algorithm with a rolling hash. Assume input strings contain only printable ASCII characters (no spaces are necessary to handle specially, but the function should work with any string). If the pattern is longer than the text, return an empty vector. The function must properly handle overlapping matches (e.g., pattern "AA" in text "AAA" should return indices 0 and 1).

#include <cassert>
#include <vector>
#include <string>

// The solution function is assumed to be defined above.
// test harness
int main() {
    // Basic match
    std::vector<int> res = findPatternOccurrences("GEEKS FOR GEEKS", "GEEK");
    assert(res.size() == 2 && res[0] == 0 && res[1] == 8);

    // No match
    res = findPatternOccurrences("hello world", "xyz");
    assert(res.empty());

    // Overlapping matches
    res = findPatternOccurrences("AAA", "AA");
    assert(res.size() == 2 && res[0] == 0 && res[1] == 1);

    // Pattern longer than text
    res = findPatternOccurrences("abc", "abcd");
    assert(res.empty());

    // Empty pattern
    res = findPatternOccurrences("abc", "");
    assert(res.empty());

    // Single character pattern
    res = findPatternOccurrences("banana", "a");
    assert(res.size() == 3 && res[0] == 1 && res[1] == 3 && res[2] == 5);

    // Entire text matches
    res = findPatternOccurrences("test", "test");
    assert(res.size() == 1 && res[0] == 0);

    // Case sensitivity
    res = findPatternOccurrences("AbcA", "A");
    assert(res.size() == 2 && res[0] == 0 && res[1] == 3);

    // Multiple occurrences with no overlap
    res = findPatternOccurrences("ababab", "ab");
    assert(res.size() == 3 && res[0] == 0 && res[1] == 2 && res[2] == 4);

    // Text with spaces
    res = findPatternOccurrences("a a a", "a a");
    assert(res.size() == 1 && res[0] == 0);

    // Pattern appears at the end
    res = findPatternOccurrences("hello", "lo");
    assert(res.size() == 1 && res[0] == 3);

    return 0;
}

#include <string>
#include <vector>

// Find all starting indices of 'pat' in 'txt' using Rabin-Karp rolling hash.
std::vector<int> findPatternOccurrences(const std::string& txt, const std::string& pat) {
    std::vector<int> result;
    const int N = static_cast<int>(txt.size());
    const int M = static_cast<int>(pat.size());
    if (M == 0 || M > N) return result;

    const int d = 256;
    const int q = 101;

    // Precompute h = d^(M-1) % q
    int h = 1;
    for (int i = 1; i < M; ++i) {
        h = (h * d) % q;
    }

    // Compute initial hash for pattern and first text window
    int p = 0, t = 0;
    for (int i = 0; i < M; ++i) {
        p = (p * d + static_cast<unsigned char>(pat[i])) % q;
        t = (t * d + static_cast<unsigned char>(txt[i])) % q;
    }

    // Slide the window
    for (int i = 0; i <= N - M; ++i) {
        // Check hash match
        if (p == t) {
            bool match = true;
            for (int j = 0; j < M; ++j) {
                if (txt[i + j] != pat[j]) {
                    match = false;
                    break;
                }
            }
            if (match) {
                result.push_back(i);
            }
        }
        // Compute next hash (rolling)
        if (i < N - M) {
            t = (d * (t - static_cast<unsigned char>(txt[i]) * h) + static_cast<unsigned char>(txt[i + M])) % q;
            if (t < 0) t += q;
        }
    }
    return result;
}

// The Rabin-Karp algorithm uses hashing to efficiently find pattern matches. First, compute an initial hash for the pattern and the first window of the text (of length equal to pattern length) using a base (typically 256) and a prime modulus (e.g., 101). Precompute the value of `d^(M-1) % q` for rolling the hash. Then slide the window across the text: at each position, compare the hash values; if they match, verify character by character to avoid false positives (hash collisions). If match is confirmed, record the starting index. To roll the hash for the next window, subtract the contribution of the outgoing character and add the incoming character, using modular arithmetic to keep values positive. Edge cases: empty pattern — return empty vector (or handle as no match). Pattern longer than text — return empty vector. Overlapping matches are naturally handled because we slide by one position each time. Time complexity is O(N) average case for hashing plus O(M*K) worst-case for the verifications where K is the number of hash collisions; worst-case total O(N*M) if all hashes match, but average O(N+M). Space complexity is O(1) auxiliary, ignoring the output vector.
