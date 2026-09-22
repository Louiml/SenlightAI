// Write a C++ function named `letterCombinations` that takes a string `digits` containing digits from '2' to '9' (inclusive) and returns a vector of all possible letter combinations that the number could represent, following the classic telephone keypad mapping (2: abc, 3: def, 4: ghi, 5: jkl, 6: mno, 7: pqrs, 8: tuv, 9: wxyz). The result vector should be populated in lexicographical order (by the order of letters on each key) and may be empty if the input string is empty. The function must be a free function (not a class method) and should handle inputs of any length from 0 to 10, with each character guaranteed to be a valid digit from '2' to '9' — no need to validate or handle '0', '1', '*', or '#'.

// The problem is a classic backtracking/DFS enumeration. The main idea is to build combinations character by character using recursion: at each step, we pick the next digit, get its corresponding letter string, and for each letter in that string, append it to a current partial combination, then recurse to the next digit. When the length of the partial combination equals the length of the input digits, we have a complete combination, which we store. After exploring one branch, we remove the last character (backtrack) to try the next letter.
//
// Edge cases:  
// - Empty input: return an empty vector (no combinations).  
// - Single digit: return the letters of that digit as individual strings (e.g., "2" → ["a","b","c"]).  
// - Duplicate letters within a key are not possible (already unique).  
// - No invalid characters are passed, but if they were, they would map to empty strings, causing no recursion — which is safe.
//
// Time complexity: For \(n\) digits, the total number of combinations is the product of the number of letters per digit. In the worst case where all digits are 7 or 9 (4 letters each), the count is \(4^n\). Each combination costs \(O(n)\) to copy into the result, so worst-case time is \(O(n \cdot 4^n)\). Space complexity: The recursion depth is \(O(n)\), and the result vector stores all \(P\) combinations each of length \(n\), so \(O(n \cdot P)\) auxiliary space, plus the current combination string of size \(n\).

#include <vector>
#include <string>

// Return all possible letter combinations for a given digit string (2-9 only).
std::vector<std::string> letterCombinations(const std::string& digits) {
    std::vector<std::string> result;
    if (digits.empty()) {
        return result;
    }

    // Mapping from digit to its corresponding letters.
    const std::vector<std::string> letterMap = {
        "",     // 0
        "",     // 1
        "abc",  // 2
        "def",  // 3
        "ghi",  // 4
        "jkl",  // 5
        "mno",  // 6
        "pqrs", // 7
        "tuv",  // 8
        "wxyz"  // 9
    };

    std::string combination;
    // Recursive lambda to perform backtracking.
    std::function<void(int)> backtrack = [&](int index) {
        if (index == static_cast<int>(digits.size())) {
            result.push_back(combination);
            return;
        }
        int digit = digits[index] - '0';
        const std::string& letters = letterMap[digit];
        for (char letter : letters) {
            combination.push_back(letter);
            backtrack(index + 1);
            combination.pop_back();
        }
    };

    backtrack(0);
    return result;
}

#include <cassert>
#include <vector>
#include <string>

// (The solution function is assumed to be defined above.)

int main() {
    // Empty input returns empty vector.
    assert(letterCombinations("") == std::vector<std::string>{});

    // Single digit 2: "abc"
    assert(letterCombinations("2") == std::vector<std::string>({"a", "b", "c"}));

    // Single digit 7: "pqrs"
    assert(letterCombinations("7") == std::vector<std::string>({"p", "q", "r", "s"}));

    // Two digits "23": all combinations in lexicographical order.
    std::vector<std::string> expected23 = {
        "ad", "ae", "af", "bd", "be", "bf", "cd", "ce", "cf"
    };
    assert(letterCombinations("23") == expected23);

    // Input "79": 4*4=16 combinations, verify first and last.
    auto combos79 = letterCombinations("79");
    assert(combos79.size() == 16);
    assert(combos79.front() == "pw");
    assert(combos79.back() == "sz");

    // Input "2" repeated 3 times: "abc"^3 = 27 combos, check size and first.
    auto combos222 = letterCombinations("222");
    assert(combos222.size() == 27);
    assert(combos222.front() == "aaa");
    assert(combos222.back() == "ccc");

    // Input "234" has 3*3*3 = 27 combos, check a few known ones.
    auto combos234 = letterCombinations("234");
    assert(combos234.size() == 27);
    assert(combos234[0] == "adg");
    assert(combos234[13] == "bfj"); // middle element roughly
    assert(combos234[26] == "cfi");

    // Input "9" returns "wxyz" letters.
    assert(letterCombinations("9") == std::vector<std::string>({"w", "x", "y", "z"}));

    return 0;
}
