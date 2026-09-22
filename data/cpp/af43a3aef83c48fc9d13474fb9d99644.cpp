/*
Write a C++ function `vector<string> wordBreakAll(const string& s, const vector<string>& wordDict)` that returns every possible sentence formed by inserting spaces into the string `s` such that each segment is a valid word from `wordDict`. The words in the dictionary are unique, and the same dictionary word may be reused multiple times. The sentences must preserve the original character order in `s`, and each sentence’s words should be separated by a single space. If no segmentation exists, return an empty vector. The function must be efficient enough to handle strings up to length 20 and dictionaries with up to 1000 words, and must not mutate its inputs.
*/

#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>

// Returns all possible sentences formed by breaking s into dictionary words.
std::vector<std::string> wordBreakAll(const std::string& s, const std::vector<std::string>& wordDict) {
    std::unordered_set<std::string> dict(wordDict.begin(), wordDict.end());
    std::unordered_map<int, std::vector<std::string>> memo;

    // Helper lambda with memoization.
    std::function<std::vector<std::string>(int)> dfs = [&](int start) -> std::vector<std::string> {
        if (start == static_cast<int>(s.size())) {
            return {""};
        }
        auto it = memo.find(start);
        if (it != memo.end()) {
            return it->second;
        }

        std::vector<std::string> result;
        std::string word;
        for (int end = start; end < static_cast<int>(s.size()); ++end) {
            word.push_back(s[end]);
            if (dict.find(word) == dict.end()) {
                continue;
            }
            std::vector<std::string> rightParts = dfs(end + 1);
            for (const std::string& part : rightParts) {
                if (part.empty()) {
                    result.push_back(word);
                } else {
                    result.push_back(word + " " + part);
                }
            }
        }
        memo[start] = result;
        return result;
    };

    return dfs(0);
}

#include <iostream>
#include <vector>
#include <string>
#include <cassert>

// Assuming the solution function is declared above.
// For test, we explicitly include the function here to keep test self-contained.
// (In practice, the solution would be in a separate header.)
// To keep the answer concise, we assume the function is already available.

int main() {
    // Test 1: Example from the snippet.
    std::string s1 = "catsanddog";
    std::vector<std::string> dict1 = {"cat", "cats", "and", "sand", "dog"};
    std::vector<std::string> res1 = wordBreakAll(s1, dict1);
    std::vector<std::string> expected1 = {"cats and dog", "cat sand dog"};
    assert(res1 == expected1);

    // Test 2: No possible segmentation.
    std::string s2 = "pineapplepenapple";
    std::vector<std::string> dict2 = {"apple", "pen", "pine", "pineapple"};
    std::vector<std::string> res2 = wordBreakAll(s2, dict2);
    assert(res2.empty());

    // Test 3: Single word string.
    std::string s3 = "hello";
    std::vector<std::string> dict3 = {"hello"};
    std::vector<std::string> res3 = wordBreakAll(s3, dict3);
    assert(res3.size() == 1 && res3[0] == "hello");

    // Test 4: Empty string (empty result, not a sentence).
    std::string s4 = "";
    std::vector<std::string> dict4 = {"a"};
    std::vector<std::string> res4 = wordBreakAll(s4, dict4);
    assert(res4.empty());

    // Test 5: Multiple segmentations with repeated words.
    std::string s5 = "aaaa";
    std::vector<std::string> dict5 = {"a", "aa"};
    std::vector<std::string> res5 = wordBreakAll(s5, dict5);
    std::vector<std::string> expected5 = {"a a a a", "a a aa", "a aa a", "aa a a", "aa aa"};
    // Sort both for comparison to avoid order dependence.
    std::sort(res5.begin(), res5.end());
    std::sort(expected5.begin(), expected5.end());
    assert(res5 == expected5);

    // Test 6: Dictionary word appears but not contiguous.
    std::string s6 = "abcd";
    std::vector<std::string> dict6 = {"ab", "bc", "cd"};
    std::vector<std::string> res6 = wordBreakAll(s6, dict6);
    assert(res6.empty());

    // Test 7: Overlapping words.
    std::string s7 = "bcc";
    std::vector<std::string> dict7 = {"bc", "cc", "b", "c"};
    std::vector<std::string> res7 = wordBreakAll(s7, dict7);
    std::vector<std::string> expected7 = {"b cc", "bc c"};
    std::sort(res7.begin(), res7.end());
    std::sort(expected7.begin(), expected7.end());
    assert(res7 == expected7);

    std::cout << "All tests passed." << std::endl;
    return 0;
}

// This is a classic word break problem with reconstruction of all solutions. The approach uses recursive backtracking with memoization. At each position `i` in the string, try every possible subword `s[i..j]`. If that subword exists in the dictionary, recursively find all segmentations of the remainder `s[j+1..]`. For each remainder sentence, prepend the current word (adding a space if the remainder is non-empty). Memoization stores results for each starting index to avoid recomputing overlapping subproblems. Edge cases: an empty remainder produces a single empty string (which acts as a base case to allow the outer word to form a complete sentence). If no word starting at `i` leads to a valid split, the result for `i` is an empty vector. The dictionary is stored in an `unordered_set` for O(1) average lookup. Time complexity: in the worst case, the number of valid sentences is exponential, but the memoization reduces overlapping subproblem recomputation to O(n * L) where L is the number of valid subwords starting at each position, plus the cost of copying result strings, which can be O(total output size). Space complexity is O(n * total output size) for the memo, plus recursion stack of depth up to n.
