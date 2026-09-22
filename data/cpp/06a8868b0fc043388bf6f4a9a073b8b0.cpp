// Write a C++ function named `reconstructLogin` that takes a target login string and a vector of password strings, and returns a `std::vector<std::string>` containing a sequence of passwords whose concatenation exactly equals the target login string. If multiple valid sequences exist, return any one. If no valid sequence exists, return a vector containing exactly one element: the string `"WRONG PASSWORD"`. The passwords can be used multiple times, and the order of the returned passwords must follow the order in which they appear in the login string. The function must handle cases where the target login is empty (in that case, return an empty vector), where passwords are empty strings (skip them, as they contribute nothing), and where a valid concatenation requires backtracking (e.g., a prefix of the login matches a password, but leads to a dead end, while a different shorter or longer password would succeed). The function should not modify the input parameters and should use efficient memoization to avoid exponential time on repeated subproblems.

// The problem is a classic word-break with memoization. Define a recursive helper `solve(start)` that attempts to build a valid suffix from position `start` in the login string. At each call, iterate through all passwords. For each password that matches the substring starting at `start` (i.e., `login.compare(start, pass.size(), pass) == 0`), recursively solve from `start + pass.size()`. If that subproblem returns a non-empty vector that does not contain `"WRONG PASSWORD"` as its only element, then we found a valid sequence: prepend the current password to that sub-solution and return it. If no password leads to a valid sequence, return `{"WRONG PASSWORD"}`. To optimize, use a memoization table (e.g., a vector of `vector<string>` or a status vector) to store results for each starting index, avoiding recomputation. Important edge cases: (1) an empty login returns empty vector immediately; (2) empty passwords should be ignored because they don't advance the index and could cause infinite recursion; (3) if a password is longer than the remaining login, it can't match; (4) if multiple solutions exist, the first one found is fine; (5) the recursion must not revisit a state with the same start index using the same logic without memoization, otherwise exponential blowup. Time complexity: \(O(N \cdot M \cdot L)\) where N is login length, M is number of passwords, and L is average password length (for string comparison). Space complexity: \(O(N \cdot \text{average solution size})\) in the worst case for memoization, but in practice we can store only a boolean or the best solution.

#include <vector>
#include <string>
#include <unordered_map>

// Returns a sequence of passwords that concatenates to the login.
// If impossible, returns {"WRONG PASSWORD"}.
std::vector<std::string> reconstructLogin(const std::string& login,
                                          const std::vector<std::string>& passwords) {
    std::unordered_map<int, std::vector<std::string>> memo;
    
    // Helper lambda for recursion with memoization.
    std::function<std::vector<std::string>(int)> solve = [&](int start) -> std::vector<std::string> {
        if (start == static_cast<int>(login.size())) {
            return {}; // Success: empty sequence.
        }
        auto it = memo.find(start);
        if (it != memo.end()) {
            return it->second;
        }
        
        for (const auto& p : passwords) {
            if (p.empty()) continue; // Skip empty passwords.
            if (static_cast<int>(p.size()) > static_cast<int>(login.size()) - start) continue;
            // Check if p matches login[start:start+p.size()]
            if (login.compare(start, p.size(), p) == 0) {
                auto sub = solve(start + static_cast<int>(p.size()));
                if (!sub.empty() && sub.front() == "WRONG PASSWORD") {
                    continue; // This path fails.
                }
                // Found valid sequence: prepend p to sub.
                std::vector<std::string> result = {p};
                result.insert(result.end(), sub.begin(), sub.end());
                memo[start] = result;
                return result;
            }
        }
        // No valid password at this position.
        memo[start] = {"WRONG PASSWORD"};
        return memo[start];
    };
    
    return solve(0);
}

#include <cassert>
#include <vector>
#include <string>
#include <iostream>

// Include the solution function here (or link it).
// For brevity, assume the function above is defined.

int main() {
    // Test 1: Simple concatenation
    std::vector<std::string> passwords1 = {"ab", "cd"};
    assert(reconstructLogin("abcd", passwords1) == std::vector<std::string>({"ab", "cd"}));
    
    // Test 2: Multiple solutions, return any valid one
    std::vector<std::string> passwords2 = {"a", "aa", "aaa"};
    auto res2 = reconstructLogin("aaa", passwords2);
    // Validate that concatenation equals "aaa" and not "WRONG PASSWORD"
    std::string concat2;
    for (const auto& s : res2) concat2 += s;
    assert(concat2 == "aaa");
    
    // Test 3: Impossible case
    std::vector<std::string> passwords3 = {"a", "b"};
    assert(reconstructLogin("c", passwords3) == std::vector<std::string>({"WRONG PASSWORD"}));
    
    // Test 4: Empty login returns empty vector
    std::vector<std::string> passwords4 = {"x"};
    assert(reconstructLogin("", passwords4).empty());
    
    // Test 5: Overlapping/backtracking needed (e.g., "aab" with ["a","ab"] -> fails but ["aa","b"] works)
    std::vector<std::string> passwords5 = {"a", "aa", "b"};
    auto res5 = reconstructLogin("aab", passwords5);
    std::string concat5;
    for (const auto& s : res5) concat5 += s;
    assert(concat5 == "aab");
    
    // Test 6: Empty passwords are ignored (should not cause infinite loop)
    std::vector<std::string> passwords6 = {"", "x"};
    assert(reconstructLogin("x", passwords6) == std::vector<std::string>({"x"}));
    
    // Test 7: Password longer than remaining login
    std::vector<std::string> passwords7 = {"abc"};
    assert(reconstructLogin("ab", passwords7) == std::vector<std::string>({"WRONG PASSWORD"}));
    
    // Test 8: Use same password multiple times
    std::vector<std::string> passwords8 = {"ab"};
    assert(reconstructLogin("abab", passwords8) == std::vector<std::string>({"ab", "ab"}));
    
    // Test 9: No passwords available
    std::vector<std::string> passwords9;
    assert(reconstructLogin("a", passwords9) == std::vector<std::string>({"WRONG PASSWORD"}));
    
    // Test 10: Larger test with many options
    std::vector<std::string> passwords10 = {"leet", "code", "le", "et"};
    auto res10 = reconstructLogin("leetcode", passwords10);
    std::string concat10;
    for (const auto& s : res10) concat10 += s;
    assert(concat10 == "leetcode");
    
    std::cout << "All tests passed." << std::endl;
    return 0;
}
