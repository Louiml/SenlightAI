Write a C++ function named `canRedistributeUniformly` that takes a vector of strings, where each string consists only of lowercase English letters, and returns a boolean indicating whether it is possible to redistribute all characters among the strings so that every string ends up with the exact same number of characters (including zero-length strings if present). The redistribution is unrestricted: you may move any individual character from one string to another, but you cannot change characters. Return `true` if the total count of each distinct character across all strings is divisible evenly by the number of strings; otherwise return `false`. You must not modify the input vector.
#include <cassert>
#include <string>
#include <vector>

// The solution function is declared above (or included from header).
// Tests for canRedistributeUniformly.
int main() {
    // Basic case: all counts divisible by 2 -> true
    std::vector<std::string> words1 = {"ab", "aabb"};
    assert(canRedistributeUniformly(words1) == true);

    // One character not divisible -> false
    std::vector<std::string> words2 = {"a", "b"};
    assert(canRedistributeUniformly(words2) == false);

    // Single string always true
    std::vector<std::string> words3 = {"abc"};
    assert(canRedistributeUniformly(words3) == true);

    // Empty strings among many: still true if all counts divisible
    std::vector<std::string> words4 = {"", "aa", "aa"};
    assert(canRedistributeUniformly(words4) == true);

    // Three strings, total 'a' count = 7 -> not divisible by 3
    std::vector<std::string> words5 = {"aaaa", "aa", "a"};
    assert(canRedistributeUniformly(words5) == false);

    // Zero strings (edge case) -> true
    std::vector<std::string> words6 = {};
    assert(canRedistributeUniformly(words6) == true);

    // Mixed characters, all divisible by 4
    std::vector<std::string> words7 = {"abc", "abc", "abc", "abc"};
    assert(canRedistributeUniformly(words7) == true);

    // Duplicate weird distribution but still divisible
    std::vector<std::string> words8 = {"zz", "z", "zzz"};
    // Total 'z' = 6, divisible by 3 -> true
    assert(canRedistributeUniformly(words8) == true);

    // Non-divisible for one char in a larger set
    std::vector<std::string> words9 = {"xy", "xy", "x"};
    // 'x' count = 3 (not divisible by 3? Actually 3%3==0), wait check: x appears in "xy","xy","x": 1+1+1=3, divisible; y count=2 not divisible by 3 -> false
    assert(canRedistributeUniformly(words9) == false);

    return 0;
}
#include <string>
#include <vector>
#include <unordered_map>

// Returns true if all characters in the words can be redistributed so that
// every string has the same total number of characters.
bool canRedistributeUniformly(const std::vector<std::string>& words) {
    const int n = static_cast<int>(words.size());
    if (n == 0) {
        return true; // Vacuously true for no strings
    }

    std::unordered_map<char, int> charFreq;
    for (const std::string& word : words) {
        for (char c : word) {
            ++charFreq[c];
        }
    }

    for (const auto& entry : charFreq) {
        if (entry.second % n != 0) {
            return false;
        }
    }
    return true;
}
// The core observation is that since characters can be freely moved between strings, the only constraint is that for each distinct character, its total frequency must be divisible by the number of strings. If any character's total count is not a multiple of `n` (where `n` is the number of strings), then it is impossible to distribute that character equally among all strings. Conversely, if every character's total count is divisible by `n`, a valid redistribution always exists because we can simply give each string `freq[ch] / n` copies of that character. Edge cases: (1) an empty vector (n = 0) — the problem likely assumes at least one string, but we can define behavior: if n == 0, return `true` vacuously or handle it explicitly; since the original snippet uses modulo with n, it assumes n > 0. We'll assume n > 0 per problem convention. (2) Strings can be empty, so the total frequency of some characters may be zero, and zero is divisible by any n, so no issue. (3) The logic only cares about characters that appear; we count all characters in all strings. Time complexity: O(total number of characters across all strings) to build the frequency map. Space complexity: O(1) in terms of distinct characters (bounded by 26 if alphabet is fixed, but generically O(k) where k is distinct characters). We use an `unordered_map<char, int>` for frequency.
