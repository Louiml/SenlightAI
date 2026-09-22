// Write a C++ function that takes a non-empty string `S` consisting only of lowercase English letters (e.g., `"abc"`) and returns a vector of strings containing all possible permutations where a single space may optionally be inserted between any adjacent characters, preserving the original character order. Each valid result is a string with the same number of letters as `S` and 0 to `|S|-1` spaces inserted between adjacent letters. The output order must be exactly: for each position from left to right, the recursive generation should first produce the branch **with** a space after the current character, then the branch **without** a space, down to the last character. The function signature is `std::vector<std::string> insertSpaces(const std::string& S);`. Note that the input string length is at least 1, and the total number of output strings is exactly \(2^{|S|-1}\) (for example, `"abc"` produces `"a b c"`, `"a bc"`, `"ab c"`, `"abc"` in that order). The solution must not use any global or static variables, and it must be self-contained (no external libraries other than standard C++).

#include <cassert>
#include <vector>
#include <string>

int main() {
    // Single character
    std::vector<std::string> r1 = insertSpaces("a");
    assert(r1.size() == 1);
    assert(r1[0] == "a");

    // Two characters
    std::vector<std::string> r2 = insertSpaces("ab");
    assert(r2.size() == 2);
    assert(r2[0] == "a b");
    assert(r2[1] == "ab");

    // Three characters with exact order
    std::vector<std::string> r3 = insertSpaces("abc");
    assert(r3.size() == 4);
    assert(r3[0] == "a b c");
    assert(r3[1] == "a bc");
    assert(r3[2] == "ab c");
    assert(r3[3] == "abc");

    // Four characters, check size and first/last
    std::vector<std::string> r4 = insertSpaces("wxyz");
    assert(r4.size() == 8);
    assert(r4[0] == "w x y z");
    assert(r4[7] == "wxyz");

    // Repeated letters still produce all combinations
    std::vector<std::string> r5 = insertSpaces("aa");
    assert(r5.size() == 2);
    assert(r5[0] == "a a");
    assert(r5[1] == "aa");

    // Empty string returns empty vector
    std::vector<std::string> r6 = insertSpaces("");
    assert(r6.empty());

    // Longer string, verify count power of two
    std::vector<std::string> r7 = insertSpaces("abcdef");
    assert(r7.size() == 32);
}

#include <vector>
#include <string>

// Generate all strings formed by optionally inserting a space between adjacent characters of S.
std::vector<std::string> insertSpaces(const std::string& S) {
    std::vector<std::string> results;
    
    // Recursive helper: current index, current built string.
    std::function<void(size_t, std::string)> dfs = [&](size_t idx, std::string current) {
        if (idx + 1 == S.size()) {
            // Last character, append and store.
            current += S[idx];
            results.push_back(current);
            return;
        }
        // Branch 1: insert space after current character.
        dfs(idx + 1, current + S[idx] + " ");
        // Branch 2: no space after current character.
        dfs(idx + 1, current + S[idx]);
    };
    
    if (S.empty()) {
        return results;
    }
    dfs(0, "");
    return results;
}

// The problem is a classic recursion over positions. Starting at index 0, we build a temporary string character by character. At each index `i` (except the last), we have two choices: either append the current character followed by a space, or append only the current character, then recurse into the next position. When we reach the last index, we append that final character and store the completed temporary string. This yields every possible combination of spaces between the letters. The order requirement is satisfied by recursing first with the space branch, then without. Edge cases include a single-character input, which simply returns that character with no spaces; also the empty string is not allowed by the task, but the function can handle it gracefully by returning an empty vector. Time complexity is \(O(2^{n-1} \cdot n)\) because there are \(2^{n-1}\) results and each result has length up to \(2n-1\) (since each letter plus possibly a space). Space complexity is \(O(2^{n-1} \cdot n)\) for the output, plus \(O(n)\) recursion depth.
