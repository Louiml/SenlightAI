/*
Write a C++ function `std::vector<std::string> phoneLetterCombinations(const std::string& digits)` that takes a string containing digits from '2' to '9' and returns all possible letter combinations that the number could represent, following the classic telephone keypad mapping (2→"abc", 3→"def", 4→"ghi", 5→"jkl", 6→"mno", 7→"pqrs", 8→"tuv", 9→"wxyz"). The order of the output does not matter, but every combination must be of length equal to the number of digits, and each letter in a combination must correspond to the digit at the same position. If the input string is empty, return an empty vector. The function must be self-contained and avoid global variables.
*/
#include <string>
#include <vector>
#include <unordered_map>

// Return all possible letter combinations for a string of digits '2'-'9'.
std::vector<std::string> phoneLetterCombinations(const std::string& digits) {
    if (digits.empty()) return {};

    static const std::unordered_map<char, std::string> phoneMap = {
        {'2', "abc"}, {'3', "def"}, {'4', "ghi"},
        {'5', "jkl"}, {'6', "mno"}, {'7', "pqrs"},
        {'8', "tuv"}, {'9', "wxyz"}
    };

    std::vector<std::string> result;
    std::string path;

    std::function<void(int)> backtrack = [&](int index) {
        if (index == static_cast<int>(digits.size())) {
            result.push_back(path);
            return;
        }
        for (char c : phoneMap.at(digits[index])) {
            path.push_back(c);
            backtrack(index + 1);
            path.pop_back();
        }
    };

    backtrack(0);
    return result;
}
#include <cassert>
#include <vector>
#include <string>

// The solution function is assumed to be declared above.

int main() {
    // Empty input
    assert(phoneLetterCombinations("").empty());

    // Single digit
    auto single = phoneLetterCombinations("2");
    assert(single.size() == 3);
    assert((single == std::vector<std::string>{"a", "b", "c"}));

    // Two digits from example
    auto two = phoneLetterCombinations("23");
    assert(two.size() == 9);
    assert((two == std::vector<std::string>{"ad", "ae", "af", "bd", "be", "bf", "cd", "ce", "cf"}));

    // Digit with four letters
    auto three = phoneLetterCombinations("79");
    assert(three.size() == 16);
    // Spot-check a few
    assert(std::find(three.begin(), three.end(), "pw") != three.end());
    assert(std::find(three.begin(), three.end(), "sz") != three.end());

    // Three digits
    auto threeDigits = phoneLetterCombinations("234");
    assert(threeDigits.size() == 27);
    // Every string length must be 3
    for (const auto& s : threeDigits) {
        assert(s.size() == 3);
    }

    // Repetitive digits
    auto repeated = phoneLetterCombinations("22");
    assert(repeated.size() == 9);
    // Check that all start with 'a', 'b', or 'c'
    for (const auto& s : repeated) {
        assert(s[0] >= 'a' && s[0] <= 'c');
        assert(s[1] >= 'a' && s[1] <= 'c');
    }

    // Long input: 4^4 = 256 combinations
    auto large = phoneLetterCombinations("2345");
    assert(large.size() == 256);
    for (const auto& s : large) {
        assert(s.size() == 4);
    }

    return 0;
}
// The problem is a classic combinatorial enumeration where each digit provides a fixed set of possible characters. We use a depth-first backtracking approach: maintain a `path` string that builds the current combination, and an index that tracks which digit we are currently processing. Starting from index 0, for each digit we iterate over all its possible letters, append one to `path`, recurse to the next index, and then backtrack by removing the last character. When the index equals the length of the input, we have a complete combination, so we add `path` to the result vector. The base case handles reaching the end of the digits. An important edge case is an empty input string, which should immediately return an empty vector—not a vector containing one empty string. The mapping from digit to letters is stored in a constant unordered_map. Time complexity is O(4^n * n) for result construction since there are at most 4^n combinations, each of length n, but the backtracking itself visits 4^n leaf nodes and does O(n) copying when storing each string; space complexity is O(n) for the recursion stack and the path string, plus O(4^n * n) for the output vector in the worst case.
