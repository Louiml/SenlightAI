Write a C++ function `repeatCharacters(const std::string& s, int k)` that takes a string `s` consisting of lowercase English letters and a positive integer `k`. The function should check whether every character in `s` occurs a number of times that is divisible by `k`. If every character’s frequency is divisible by `k`, return a string that is formed by taking one copy of each character (in sorted order, each repeated exactly `(frequency / k)` times) and then concatenating that base block `k` times to form the final result. If any character’s frequency is not divisible by `k`, return the string `"-1"`. If the input string is empty, return an empty string. Ensure the result is sorted lexicographically and that `k` is at least 1.
The solution sorts the input string to make it easy to count character frequencies and to build the answer in sorted order. After sorting, we can iterate through the string and count occurrences of each distinct character, storing the characters in the order they first appear (which, due to sorting, is already alphabetical). For each character, if its total frequency modulo `k` is not zero, the whole problem is impossible, so we return `"-1"`. Otherwise, we divide the frequency by `k` and append that many copies of the character to a base string. After processing all characters, we have a base block representing one “unit” of the final string. The final answer is the base block repeated `k` times. Edge cases include an empty input (return empty string), a single character with frequency divisible by `k`, and all frequencies already divisible but maybe `k` being larger than any frequency (e.g., frequency 1 and `k=2`), which correctly yields `-1` because 1 % 2 != 0. The algorithm runs in O(n log n) due to sorting (where n is the string length) and uses O(n) auxiliary space for the map and result string. Counting and building the result is O(n) after sorting.
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>

// Returns a string where each distinct character of s appears in sorted order,
// repeated (count / k) times per block, and the whole block repeated k times.
// Returns "-1" if any character count is not divisible by k.
std::string repeatCharacters(const std::string& s, int k) {
    if (s.empty()) return "";
    if (k <= 0) return "-1"; // defensive, but problem states k >= 1

    std::string sorted_s = s;
    std::sort(sorted_s.begin(), sorted_s.end());

    std::vector<char> distinct;
    std::unordered_map<char, int> freq;
    for (char c : sorted_s) {
        if (freq[c] == 0) distinct.push_back(c);
        freq[c]++;
    }

    std::string base;
    for (char c : distinct) {
        if (freq[c] % k != 0) return "-1";
        int reps = freq[c] / k;
        base.append(reps, c);
    }

    std::string result;
    result.reserve(base.size() * k);
    for (int i = 0; i < k; ++i) result += base;
    return result;
}
#include <cassert>
#include <string>

// The solution function is declared above. Include it before this test.

int main() {
    // Basic valid case
    assert(repeatCharacters("aabbcc", 2) == "abcabc");
    // All frequencies divisible by 1
    assert(repeatCharacters("abc", 1) == "abc");
    // Single character, frequency divisible
    assert(repeatCharacters("aaaa", 4) == "a");
    // Frequency not divisible
    assert(repeatCharacters("abc", 2) == "-1");
    // Empty string
    assert(repeatCharacters("", 3) == "");
    // Duplicates and k=1 gives sorted original
    assert(repeatCharacters("bcaacb", 1) == "aabbcc");
    // k larger than any frequency
    assert(repeatCharacters("aabb", 3) == "-1");
    // Mixed cases: some divisible, some not
    assert(repeatCharacters("aab", 2) == "-1");
    // Large repetition: base length 2, k=3
    assert(repeatCharacters("aabbcc", 3) == "-1"); // 2 % 3 != 0
    // Valid with multiple same letters
    assert(repeatCharacters("aaabbb", 3) == "ababab");
}
