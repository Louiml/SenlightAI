// Write a C++ function `std::string getHappyString(int n, int k)` that returns the k-th lexicographically smallest string of length `n` consisting only of the characters `'a'`, `'b'`, and `'c'`, with the constraint that no two adjacent characters are the same. If fewer than `k` such strings exist, return an empty string. The function must be efficient and not rely on generating all strings when possible, though generating all valid strings and then selecting the k-th is acceptable for `n` up to 10. Implement the function with proper `const` correctness where applicable, and ensure it is self-contained with all necessary `#include` directives.

// The problem asks for the k-th lexicographically smallest "happy string" of a given length `n`, where a happy string uses only `'a'`, `'b'`, `'c'` and has no equal adjacent characters. The straightforward approach is to use backtracking to generate all valid strings of length `n` in lexicographic order (since we iterate characters from `'a'` to `'c'`), store them in a vector, and then return the element at index `k-1` if it exists, otherwise return an empty string. The backtracking function recursively builds a string, at each step trying `'a'`, `'b'`, `'c'` in order, skipping a character if it equals the current last character of the partially built string. When the length reaches `n`, the string is appended to the result vector. The base case for the recursion is when the current string length equals `n`. Edge cases include `n <= 0` (though typical constraints have `n >= 1`), `k` larger than the total number of valid strings (which is `3 * 2^(n-1)` because the first character has 3 choices and each subsequent has 2 choices), and the empty result when `k` is out of range. The time complexity is `O(3 * 2^(n-1) * n)` because we generate all valid strings, each of length `n`, and the number of valid strings is exponential. Since the problem constraints often allow `n` up to 10, this is acceptable. The space complexity is `O(3 * 2^(n-1) * n)` for storing the generated strings, plus `O(n)` for the recursion stack.

#include <string>
#include <vector>

// Return the k-th lexicographically smallest happy string of length n,
// or an empty string if fewer than k such strings exist.
std::string getHappyString(int n, int k) {
    std::vector<std::string> happyStrings;
    std::string current;
    
    // Backtracking function to generate all happy strings of length n.
    // It builds strings in lexicographic order because we try 'a', 'b', 'c' in order.
    auto generate = [&](auto&& self, std::string& current) -> void {
        if (current.length() == static_cast<std::size_t>(n)) {
            happyStrings.push_back(current);
            return;
        }
        for (char ch = 'a'; ch <= 'c'; ++ch) {
            // Skip if the last character equals the current character.
            if (!current.empty() && current.back() == ch) {
                continue;
            }
            current.push_back(ch);
            self(self, current);
            current.pop_back();
        }
    };
    
    generate(generate, current);
    
    // If k is out of range, return empty string.
    if (k > static_cast<int>(happyStrings.size())) {
        return "";
    }
    return happyStrings[k - 1];
}

#include <cassert>
#include <string>

// Function declaration from the solution (placed here for test compilation).
std::string getHappyString(int n, int k);

int main() {
    // Basic n=1 cases: all three single characters are happy.
    assert(getHappyString(1, 1) == "a");
    assert(getHappyString(1, 2) == "b");
    assert(getHappyString(1, 3) == "c");
    assert(getHappyString(1, 4) == "");

    // n=2: valid strings are "ab","ac","ba","bc","ca","cb" (lexicographic order).
    assert(getHappyString(2, 1) == "ab");
    assert(getHappyString(2, 2) == "ac");
    assert(getHappyString(2, 3) == "ba");
    assert(getHappyString(2, 6) == "cb");
    assert(getHappyString(2, 7) == "");

    // n=3: first few should be "aba","abc","aca","acb","bab",...
    assert(getHappyString(3, 1) == "aba");
    assert(getHappyString(3, 2) == "abc");
    assert(getHappyString(3, 3) == "aca");
    assert(getHappyString(3, 4) == "acb");
    assert(getHappyString(3, 5) == "bab");

    // Large k value beyond total count returns empty.
    // Total number for n=3 is 3 * 2^(2) = 12.
    assert(getHappyString(3, 12) == "cbc");
    assert(getHappyString(3, 13) == "");

    // n=4: total count is 3 * 2^3 = 24.
    assert(getHappyString(4, 24) == "cbcb");
    assert(getHappyString(4, 25) == "");

    return 0;
}
