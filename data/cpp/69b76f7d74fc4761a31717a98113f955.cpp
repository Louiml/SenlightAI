// Write a standalone C++ function `occurrencesOfPattern` that takes two null-terminated character arrays (C‑strings) — a pattern `pattern` and a text `text` — and returns a `std::pair<int, std::vector<int>>`. The first element of the pair is the total number of non‑overlapping? *No*, the problem is *overlapping* occurrences (as in the given KMP code, where each match is reported by its starting index `i−len`, and overlapping matches are counted). The second element is a vector containing the **first up to 1000** starting indices (0‑based) of all occurrences, in increasing order. For example, pattern `"aa"` in text `"aaaa"` should return count=3 and indices `{0,1,2}`. The function must be efficient for patterns and texts up to 2,000,000 characters. The indices in the output vector must be 0‑based, unlike the original code which used 1‑based.
Use the KMP (Knuth–Morris–Pratt) algorithm. First compute the longest proper prefix‑suffix (LPS) array for the pattern. The pattern and text are given as C‑strings; we need their lengths using `strlen`. Because the input sizes can be up to 2,000,000, we must avoid O(n²) algorithms; KMP runs in O(n+m) time. Build the LPS array with 1‑based indexing internally (like the snippet) or adapt to 0‑based; either works. Then scan the text with the pattern, maintaining `len` equal to the length of the longest prefix of the pattern that is a suffix of the text processed so far. When `len` equals the pattern length, we found an occurrence at starting index `i - len` (where `i` is the 1‑based position in text). Increment the total count and, if the solution vector size is less than 1000, push the 0‑based index (which is `i - len - 1`). Edge cases: empty pattern? The problem implies non‑empty pattern (original code assumes non‑empty). If pattern length is zero, we could return count=0 and empty vector (define behavior). Overlapping matches are handled naturally by the KMP fallback after a full match. Time complexity O(N+M), space O(N) for LPS.
#include <vector>
#include <cstring>

// Returns {total_count, up_to_1000_start_indices_0_based}
std::pair<long long, std::vector<int>> occurrencesOfPattern(const char* pattern, const char* text) {
    int N = (int)std::strlen(pattern);
    int M = (int)std::strlen(text);
    
    std::pair<long long, std::vector<int>> result;
    result.first = 0;
    
    if (N == 0) {
        // No occurrences if pattern is empty, return empty vector
        return result;
    }
    
    // LPS array (1‑based indexing to reduce index shifting)
    std::vector<int> lps(N + 1, 0);
    int len = 0;
    for (int i = 2; i <= N; ++i) {
        while (len != 0 && pattern[len] != pattern[i - 1]) {
            len = lps[len];
        }
        if (pattern[len] == pattern[i - 1]) {
            ++len;
        }
        lps[i] = len;
    }
    
    // Scan text
    len = 0;
    for (int i = 1; i <= M; ++i) {
        while (len != 0 && pattern[len] != text[i - 1]) {
            len = lps[len];
        }
        if (pattern[len] == text[i - 1]) {
            ++len;
        }
        if (len == N) {
            // Found a match starting at 0‑based index (i - len - 1)
            result.first++;
            if ((int)result.second.size() < 1000) {
                result.second.push_back(i - len - 1);
            }
            // Fallback to allow overlapping matches
            len = lps[len];
        }
    }
    
    return result;
}
#include <cassert>
#include <vector>
#include <utility>

int main() {
    // Basic case
    auto r1 = occurrencesOfPattern("a", "banana");
    assert(r1.first == 3);
    assert(r1.second == std::vector<int>({1,3,5}));
    
    // Overlapping matches
    auto r2 = occurrencesOfPattern("aa", "aaaa");
    assert(r2.first == 3);
    assert(r2.second == std::vector<int>({0,1,2}));
    
    // No matches
    auto r3 = occurrencesOfPattern("xyz", "abcabc");
    assert(r3.first == 0);
    assert(r3.second.empty());
    
    // Pattern longer than text
    auto r4 = occurrencesOfPattern("hello", "hi");
    assert(r4.first == 0);
    assert(r4.second.empty());
    
    // Exact match
    auto r5 = occurrencesOfPattern("abc", "abc");
    assert(r5.first == 1);
    assert(r5.second == std::vector<int>({0}));
    
    // Pattern at end
    auto r6 = occurrencesOfPattern("cd", "abcd");
    assert(r6.first == 1);
    assert(r6.second == std::vector<int>({2}));
    
    // Single character, multiple occurrences
    auto r7 = occurrencesOfPattern("b", "bbbb");
    assert(r7.first == 4);
    assert(r7.second == std::vector<int>({0,1,2,3}));
    
    // Pattern that is prefix‑suffix tricky
    auto r8 = occurrencesOfPattern("abab", "abababab");
    assert(r8.first == 3);
    assert(r8.second == std::vector<int>({0,2,4}));
    
    // Ensure vector size is capped at 1000
    const char* largeText = "a";
    // simulate 1001 'a's in text? We'll build string
    std::string text(1001, 'a');
    auto r9 = occurrencesOfPattern("a", text.c_str());
    assert(r9.first == 1001);
    assert(r9.second.size() == 1000);
    assert(r9.second[0] == 0);
    assert(r9.second[999] == 999);
    
    // Empty pattern test (defined behavior)
    auto r10 = occurrencesOfPattern("", "abc");
    assert(r10.first == 0);
    assert(r10.second.empty());
}
