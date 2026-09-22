/*
Given a non-empty string consisting of uppercase English letters, write a C++ function `generateSubsequences` that returns a `std::vector<std::string>` containing all possible subsequences of the input string, in the exact order produced by a recursive depth-first exploration that always first excludes the current character, then includes it. The input string contains at least one character and at most 10 characters. The returned vector must preserve this specific ordering (i.e., for "ABC", the order must be "", "C", "B", "BC", "A", "AC", "AB", "ABC"). The function should not print anything; it must return a vector of strings.
*/

#include <string>
#include <vector>

// Generate all subsequences of the input string in the exact order produced by
// a recursive exploration that excludes the current character before including it.
// The order is: first all subsequences without the first character, then all with it.
std::vector<std::string> generateSubsequences(const std::string& input) {
    std::vector<std::string> result;
    
    // Helper lambda for recursive generation.
    // s: remaining substring to process.
    // ans: accumulated subsequence so far.
    // out: reference to the result vector.
    auto explore = [&](const std::string& s, const std::string& ans, auto&& self) -> void {
        if (s.empty()) {
            result.push_back(ans);
            return;
        }
        char current = s[0];
        std::string rest = s.substr(1);
        
        // Exclude current character.
        self(rest, ans, self);
        // Include current character.
        self(rest, ans + current, self);
    };
    
    explore(input, "", explore);
    return result;
}

#include <cassert>
#include <string>
#include <vector>

// Assume generateSubsequences is defined above (or included here).

int main() {
    // Test 1: Example from the original snippet with "ABC".
    std::vector<std::string> result1 = generateSubsequences("ABC");
    std::vector<std::string> expected1 = {"", "C", "B", "BC", "A", "AC", "AB", "ABC"};
    assert(result1 == expected1);
    
    // Test 2: Single character.
    std::vector<std::string> result2 = generateSubsequences("X");
    std::vector<std::string> expected2 = {"", "X"};
    assert(result2 == expected2);
    
    // Test 3: Two characters, order should be "", "B", "A", "AB".
    std::vector<std::string> result3 = generateSubsequences("AB");
    std::vector<std::string> expected3 = {"", "B", "A", "AB"};
    assert(result3 == expected3);
    
    // Test 4: Duplicate letters are distinct positions, so "AA" gives 4 subsequences.
    std::vector<std::string> result4 = generateSubsequences("AA");
    // Order: "", "A" (second), "A" (first), "AA".
    std::vector<std::string> expected4 = {"", "A", "A", "AA"};
    assert(result4 == expected4);
    
    // Test 5: Length 4, verify count and first and last entries.
    std::vector<std::string> result5 = generateSubsequences("ABCD");
    assert(result5.size() == 16);
    assert(result5.front() == "");
    assert(result5.back() == "ABCD");
    
    // Test 6: Verify the sequence is exactly the recursive exclude-then-include order for "Z".
    std::vector<std::string> result6 = generateSubsequences("Z");
    assert(result6[0] == "" && result6[1] == "Z");
    
    // Test 7: Check that each subsequence is indeed a subsequence (basic sanity).
    std::vector<std::string> result7 = generateSubsequences("PQ");
    for (const auto& sub : result7) {
        // Check that sub is a subsequence of "PQ" by simple comparison.
        size_t pos = 0;
        bool ok = true;
        for (char c : sub) {
            while (pos < 2 && "PQ"[pos] != c) ++pos;
            if (pos == 2) { ok = false; break; }
            ++pos;
        }
        assert(ok);
    }
    
    return 0;
}

// The solution uses recursion to explore the subset of subsequences. At each step, we process the first character of the current substring. We make two recursive calls: first, a call that does **not** include the current character (i.e., the answer string is passed unchanged), and second, a call that **includes** the current character (i.e., the current character is appended to the answer string). This mirrors the original snippet's logic, but instead of printing the result when the substring becomes empty, we push the accumulated answer string into a vector that is passed by reference. The base case occurs when the input substring is empty; at that point, the current answer (which represents one complete subsequence) is appended to the output vector. Since the recursion always goes "exclude" before "include", the order of the subsequences exactly matches the original snippet's print order. Edge cases: an empty input is not allowed per the task, but if it were, the function would return a single empty string; duplicate letters are treated as distinct positions, so "AA" produces four subsequences. Time complexity: there are \(2^n\) subsequences, each of length up to \(n\), so generating and copying them takes \(O(n \cdot 2^n)\) time. Space complexity is \(O(n)\) for the recursion stack (depth at most \(n\)) plus \(O(n \cdot 2^n)\) for the output vector.
