Write a C++ function `int substringBeautySum(const std::string& s)` that takes a non-empty string consisting of lowercase English letters and returns the sum of the "beauty" of all substrings. The beauty of a substring is defined as the difference between the maximum frequency and the minimum frequency of any character appearing in that substring. The function should consider all possible substrings (contiguous sequences) of the input string, including single-character substrings (where beauty is 0 because max and min are equal). The input string may contain repeated characters, and the function must handle the full range of lengths from 1 up to very large strings efficiently within the constraints of the algorithm.
The straightforward approach is to enumerate every starting index `i` and every ending index `j` (forming substring `s[i..j]`), and for each substring compute the character frequencies, then the max and min frequencies. Using an `unordered_map` to store frequencies for each substring, and iterating over the map to find max and min, the total time complexity is O(n³) in the worst case (n² substrings, each requiring O(n) work for frequency updates and map scanning if map size grows). However, a more efficient method is to fix the start index `i` and extend `j` incrementally, updating the frequency map in constant time per extension. Still, to compute max and min each time, we can either scan the map (which has at most 26 entries since alphabet is lowercase) — that gives O(n² * 26) = O(n²) time, which is acceptable for typical constraints (n up to ~500 in LeetCode). Space complexity is O(1) because the map holds at most 26 keys. The edge cases include single-character substrings (beauty = 0) and substrings where all characters have the same frequency (beauty = 0). The algorithm correctly handles these because max and min will be equal. The initial max is set to 0 and min to a large number (or the first encountered frequency) to handle empty maps, but since substrings are non-empty, we always have at least one character. It is important to reset min frequency properly; using INT_MAX and then comparing with actual frequencies works because the map is not empty.
#include <string>
#include <unordered_map>
#include <algorithm>
#include <climits>

// Returns the sum of beauties (max frequency - min frequency) over all substrings.
int substringBeautySum(const std::string& s) {
    int total = 0;
    const int n = static_cast<int>(s.length());

    // Iterate over all possible starting positions.
    for (int i = 0; i < n; ++i) {
        std::unordered_map<char, int> freq;
        // Extend the substring ending at j.
        for (int j = i; j < n; ++j) {
            ++freq[s[j]];

            int maxFreq = 0;
            int minFreq = INT_MAX;

            // The alphabet is lowercase letters, so map size is at most 26.
            for (const auto& entry : freq) {
                maxFreq = std::max(maxFreq, entry.second);
                minFreq = std::min(minFreq, entry.second);
            }

            total += (maxFreq - minFreq);
        }
    }

    return total;
}
#include <cassert>
#include <string>

// The solution function declaration is assumed to be in scope from the previous section.
int main() {
    // Single character: substring "a" has beauty 0.
    assert(substringBeautySum("a") == 0);

    // Two different characters: substrings "a" (0), "b" (0), "ab" (max=1, min=1, diff=0) => total 0.
    assert(substringBeautySum("ab") == 0);

    // "aab": substrings "a"(0), "a"(0), "b"(0), "aa"(0), "ab"(0), "aab"(freq a=2,b=1, diff=1) => total 1.
    assert(substringBeautySum("aab") == 1);

    // "abc": all substrings have at most 1 of each char, so every beauty is 0.
    assert(substringBeautySum("abc") == 0);

    // "aabb": let's manually compute key ones: "aa" (0), "ab" (0), "aabb" (max=2,min=2, diff=0), actually all substrings have balanced counts after extension? Let's compute quickly.
    // All substrings: "a"(0),"a"(0),"b"(0),"b"(0), "aa"(0), "ab"(0), "bb"(0), "aab"(a=2,b=1,diff=1), "abb"(a=1,b=2,diff=1), "aabb"(2,2,diff=0) => total 1+1=2.
    assert(substringBeautySum("aabb") == 2);

    // "aba": substrings "a"(0),"b"(0),"a"(0),"ab"(0),"ba"(0),"aba"(a=2,b=1,diff=1) => total 1.
    assert(substringBeautySum("aba") == 1);

    // "abcabc": all substrings of length <=3 have diff 0, longer ones? Let's check "abca" (a=2, b=1, c=1, diff=1), "abcab" (a=2,b=2,c=1,diff=1), "abcabc" (all 2, diff=0). So there are multiple contributing. Hard to manually compute, but we can test known LeetCode result: beautySum("abcabc") = 12? Let's verify via quick reasoning: Actually we just trust the algorithm. For safety, test small known values.
    assert(substringBeautySum("aba") == 1);
    assert(substringBeautySum("aa") == 0); // substring "aa" has max=2, min=2, diff=0.

    // Test with repeated pattern "aaab": "aaab" has counts a=3,b=1 diff=2, substrings "aaa" diff=0, "aab" diff=1? Actually "aab" a=2,b=1 diff=1. Let's compute total: single chars all 0, pairs: "aa"0, "aa"0, "ab"0, triples: "aaa"0, "aab"1, "aaab"2 => total 3.
    assert(substringBeautySum("aaab") == 3);

    return 0;
}
