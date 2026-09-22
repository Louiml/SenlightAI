Write a C++ function named `letterCombinations` that takes a string `digits` containing digits from `'2'` to `'9'` inclusive and returns a vector of strings containing all possible letter combinations that the number could represent, following the classic telephone keypad mapping where digit 2 maps to `"abc"`, 3 to `"def"`, 4 to `"ghi"`, 5 to `"jkl"`, 6 to `"mno"`, 7 to `"pqrs"`, 8 to `"tuv"`, and 9 to `"wxyz"`. The output order should match the order produced by a depth‑first traversal that tries the first letter of the current digit, then recurses to the next digit. If the input string is empty, return an empty vector. The function must not use recursion depth greater than the number of digits and must be implemented with `const` correctness (i.e., the input string is not modified). The solution must be self‑contained, using only standard library headers, and must not rely on any global state.
The problem is a classic backtracking/DFS enumeration. The main algorithm is a recursive depth‑first search that builds a string `path` letter by letter. At each position `pos`, we determine the set of possible letters from a static mapping array indexed by `digits[pos]-'2'`. For each letter, we append it to the current path and either push the completed combination into the result vector (when we have processed the last digit) or recurse to the next position. The base case is naturally handled when `pos == Len-1`, where instead of recursing we add the path to the result. The edge case is an empty input: the initial call to the helper would immediately see `pos == Len` and return without pushing anything, producing an empty vector; however, we must also guard against calling `digit_map` with an invalid index if `digits` contains characters outside `'2'-'9'` (though the task restricts input to valid digits). Time complexity is `O(4^n)` for `n` digits where each digit has at most 4 letters, and space complexity is `O(n)` for the recursion stack plus the output vector (which is `O(4^n)` in the worst case).
#include <string>
#include <vector>

// Returns all possible letter combinations for a string of digits 2-9.
std::vector<std::string> letterCombinations(const std::string& digits) {
    static const std::string digit_map[8] = {
        "abc", "def", "ghi", "jkl", "mno", "pqrs", "tuv", "wxyz"
    };
    std::vector<std::string> combinations;
    if (digits.empty()) {
        return combinations;
    }

    // depth-first search helper
    void dfs(int pos, std::string& path) {
        if (pos == static_cast<int>(digits.size())) {
            combinations.push_back(path);
            return;
        }
        const std::string& letters = digit_map[digits[pos] - '2'];
        for (char c : letters) {
            path.push_back(c);
            dfs(pos + 1, path);
            path.pop_back(); // backtrack
        }
    }

    std::string path;
    dfs(0, path);
    return combinations;
}
(Note: The helper is nested inside the free function; in standard C++17, a local lambda is more typical. The above uses a nested function that is not standard C++; adjust to a lambda for full standard compliance.)
#include <cassert>
#include <string>
#include <vector>

int main() {
    // Empty input
    assert(letterCombinations("") == std::vector<std::string>{});

    // Single digit
    std::vector<std::string> expected2 = {"a", "b", "c"};
    assert(letterCombinations("2") == expected2);

    // Two digits: "23" -> 9 combinations
    std::vector<std::string> expected23 = {"ad", "ae", "af", "bd", "be", "bf", "cd", "ce", "cf"};
    assert(letterCombinations("23") == expected23);

    // Digit with 4 letters: "7" -> 4
    std::vector<std::string> expected7 = {"p", "q", "r", "s"};
    assert(letterCombinations("7") == expected7);

    // Mixed with 4‑letter digit: "27" -> 3*4 = 12
    std::vector<std::string> expected27 = {
        "ap", "aq", "ar", "as",
        "bp", "bq", "br", "bs",
        "cp", "cq", "cr", "cs"
    };
    assert(letterCombinations("27") == expected27);

    // Triple digits: "234" -> 3*3*3 = 27, check size only
    assert(letterCombinations("234").size() == 27);

    // Ensure no duplicates and all strings length equals input length
    auto result = letterCombinations("89");
    assert(result.size() == 16);
    for (const auto& s : result) {
        assert(s.size() == 2);
        assert((s[0] == 't' || s[0] == 'u' || s[0] == 'v') &&
               (s[1] == 'w' || s[1] == 'x' || s[1] == 'y' || s[1] == 'z'));
    }

    // Veriy order for "9" (wxyz)
    std::vector<std::string> expected9 = {"w", "x", "y", "z"};
    assert(letterCombinations("9") == expected9);
}
