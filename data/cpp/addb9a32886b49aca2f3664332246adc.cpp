Write a standalone C++ function `countSharedOccurrences` that accepts a vector of strings (representing logical formulas as raw text) and returns an unordered_map<string, int> where each key is a string token that appears in at least one formula, and each value is the number of formulas that contain that token at least once. Tokens are defined as maximal sequences of alphanumeric characters (letters and digits) separated by any non-alphanumeric characters (spaces, punctuation, operators, etc.). The function must treat tokens case-sensitively. If a token appears multiple times in the same formula, it should be counted only once for that formula. The input vector may be empty, in which case an empty map is returned. The function must be `const`-correct (i.e., it should not modify the input). Assume the input strings contain only ASCII characters.

The core task is to count, for each unique token, the number of distinct formulas that contain that token. The algorithm proceeds as follows:

1. **Tokenization**: For each formula string `s` in the input vector, scan the string character by character. Build a temporary token string by accumulating consecutive alphanumeric characters. When a non-alphanumeric character or the end of the string is encountered, if the accumulated token is non-empty, process it: insert it into a `std::set<std::string>` (or a `std::unordered_set`) that tracks all tokens found in this particular formula. This set ensures that duplicate tokens within a single formula are counted only once. Then clear the temporary token buffer.

2. **Global counting**: After processing a formula and having its set of unique tokens, iterate over the set and increment the count in the global `std::unordered_map<std::string, int>` for each token. This map accumulates the number of formulas that contain each token.

3. **Edge cases**: 
   - Empty input vector: return an empty map.
   - Empty formula string: no tokens, nothing to add.
   - Formulas with only non-alphanumeric characters: nothing to add.
   - Punctuation like `_`, `-`, parentheses, whitespace, etc., are treated as separators; they do not appear in tokens.
   - Case-sensitivity: `Token` and `token` are distinct keys.

4. **Time complexity**: Let `F` be the number of formulas and `L` be the average length of a formula. The total number of characters scanned is `O(F * L)`. Inserting into a set for each formula costs `O(k log k)` where `k` is the number of unique tokens in that formula, and then iterating that set costs `O(k)` for the global map update. Overall, time complexity is `O(F * L + total_unique_tokens_across_formulas * average_set_operations)`, which is effectively `O(F * L)` in practice since `k` is bounded by `L` and the log factor is small. The auxiliary space is `O(F * L)` in the worst case if every formula has a distinct set of tokens, plus the output map size.

- **Space complexity**: The temporary set for each formula uses `O(k)` space, and the global map uses `O(U)` where `U` is the total number of unique tokens across all formulas. Overall `O(U + max_k)`.

#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>

// Count, for each distinct alphanumeric token, the number of strings in the
// input vector that contain that token at least once. Tokens are maximal
// sequences of alphanumeric characters (letters and digits), case-sensitive,
// and separated by any non-alphanumeric character. Duplicates within a single
// string are counted once. Returns an empty map for an empty input.
std::unordered_map<std::string, int> countSharedOccurrences(const std::vector<std::string>& formulas) {
    std::unordered_map<std::string, int> result;

    for (const std::string& formula : formulas) {
        std::unordered_set<std::string> unique_tokens;
        std::string current_token;

        for (char ch : formula) {
            if (std::isalnum(static_cast<unsigned char>(ch))) {
                current_token.push_back(ch);
            } else {
                if (!current_token.empty()) {
                    unique_tokens.insert(current_token);
                    current_token.clear();
                }
            }
        }

        // Handle token at the end of the string (if any).
        if (!current_token.empty()) {
            unique_tokens.insert(current_token);
        }

        // Update global counts for this formula's unique tokens.
        for (const std::string& token : unique_tokens) {
            ++result[token];
        }
    }

    return result;
}

#include <cassert>
#include <vector>
#include <string>
#include <unordered_map>

// The solution function is already defined above; include it here manually for the test.
// (In a real test harness, include the header/source that contains the function.)
#include <cctype>

int main() {
    // Basic case with multiple formulas
    std::vector<std::string> formulas1 = {"x + y", "x * z", "y - x"};
    std::unordered_map<std::string, int> res1 = countSharedOccurrences(formulas1);
    assert(res1.size() == 3);
    assert(res1["x"] == 3);
    assert(res1["y"] == 2);
    assert(res1["z"] == 1);

    // Duplicate tokens in a single formula should be counted once
    std::vector<std::string> formulas2 = {"a a a", "b a"};
    std::unordered_map<std::string, int> res2 = countSharedOccurrences(formulas2);
    assert(res2.size() == 2);
    assert(res2["a"] == 2);
    assert(res2["b"] == 1);

    // Empty input returns empty map
    std::vector<std::string> formulas3;
    std::unordered_map<std::string, int> res3 = countSharedOccurrences(formulas3);
    assert(res3.empty());

    // Case sensitivity and punctuation as separators
    std::vector<std::string> formulas4 = {"Hello, world!", "hello_world", "Hello World"};
    std::unordered_map<std::string, int> res4 = countSharedOccurrences(formulas4);
    assert(res4["Hello"] == 2);
    assert(res4["world"] == 2);
    assert(res4["hello"] == 1);
    assert(res4["World"] == 1);

    // Formulas with only non-alphanumeric characters
    std::vector<std::string> formulas5 = {"!!!", "???", "..."};
    std::unordered_map<std::string, int> res5 = countSharedOccurrences(formulas5);
    assert(res5.empty());

    // Empty string in the middle and edge with token at end without separator
    std::vector<std::string> formulas6 = {"abc", "", "def123"};
    std::unordered_map<std::string, int> res6 = countSharedOccurrences(formulas6);
    assert(res6.size() == 2);
    assert(res6["abc"] == 1);
    assert(res6["def123"] == 1);

    // Mixed separators and digits
    std::vector<std::string> formulas7 = {"1+2=3", "2*3=6", "3/1"};
    std::unordered_map<std::string, int> res7 = countSharedOccurrences(formulas7);
    assert(res7["1"] == 2); // appears in first and third
    assert(res7["2"] == 2); // first and second
    assert(res7["3"] == 3); // all three
    assert(res7["6"] == 1);
    assert(res7["0"] == 0); // not present

    return 0;
}
