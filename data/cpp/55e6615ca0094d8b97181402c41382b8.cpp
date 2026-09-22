// Write a C++ function `countWaysToConstruct` that takes a target string and a vector of word fragments (strings), and returns the number of distinct ways the target string can be formed by concatenating fragments from the vector. Fragments may be reused any number of times, and order matters (so "ab" + "c" is different from "a" + "bc"). The function must handle cases where no construction is possible (return 0) and must be efficient for repeated subproblems. The input strings consist only of lowercase English letters, and the target string may be up to 1000 characters long, with up to 50 fragments each up to 20 characters.
// The problem is a classic counting version of the "canConstruct" / "word break" problem. We can solve it using memoized recursion (top-down dynamic programming). Define a recursive function `solve(remaining)` that returns the number of ways to form `remaining` from the fragments. Base case: if `remaining` is empty, return 1 (one way to construct the empty string). For each fragment `w`, if `remaining` starts with `w`, then the number of ways for that branch is `solve(remaining.substr(w.length()))`. Sum over all matching fragments. To avoid recomputation, memoize results for each distinct substring. A subtlety: a result of 0 is a valid memoized value, so we cannot use a map where a missing key defaults to 0 (that would cause infinite recursion). Instead, use a map from string to `int` (with sentinel -1 for "not computed"), or use an unordered_map with a sentinel. The given snippet uses `memo[targetstr] > 0` and `== -1` to distinguish uncomputed from computed-zero. Edge cases: empty target string returns 1; fragments longer than remaining are skipped; duplicate fragments in the vector are allowed and each counts as a separate way (though they produce identical paths, they are considered distinct choices). Time complexity: Let `T` be the target length, `F` be the number of fragments, and `L` be the average fragment length. The recursion explores at most `T` distinct substrings (each suffix of the target), and for each we try `F` fragments, each requiring an `O(L)` prefix check. So total time is `O(T * F * L)` in the worst case, and space is `O(T)` for the memo map plus recursion stack depth `O(T)`.
#include <string>
#include <vector>
#include <unordered_map>

// Count the number of distinct ways to build target by concatenating fragments from wordBank.
// Fragments can be reused, and order matters.
int countWaysToConstruct(const std::string& target, const std::vector<std::string>& wordBank) {
    // Memoization: map from remaining substring to number of ways.
    // Use a sentinel -1 to indicate "not computed yet" because 0 is a valid result.
    static std::unordered_map<std::string, int> memo;
    
    // Helper recursive lambda (C++14 style, but we can use a separate function for clarity).
    // Since we cannot define a lambda that calls itself easily without capturing, we use a helper function.
    struct Solver {
        const std::vector<std::string>& words;
        std::unordered_map<std::string, int>& memo;
        
        int solve(const std::string& remaining) {
            auto it = memo.find(remaining);
            if (it != memo.end()) return it->second;
            
            if (remaining.empty()) {
                memo[remaining] = 1;
                return 1;
            }
            
            int total = 0;
            for (const auto& w : words) {
                if (w.size() > remaining.size()) continue;
                if (remaining.compare(0, w.size(), w) == 0) {
                    total += solve(remaining.substr(w.size()));
                }
            }
            
            memo[remaining] = total;
            return total;
        }
    };
    
    // Clear memo for each call (since static memo would leak between calls).
    // Better: use a non-static local memo passed to the solver.
    std::unordered_map<std::string, int> localMemo;
    Solver solver{wordBank, localMemo};
    return solver.solve(target);
}
#include <cassert>
#include <string>
#include <vector>

// The function is declared above; here we test it.
int main() {
    // Test 1: From the snippet, "purple" with fragments purp, p, ur, le, purpl -> 2 ways.
    std::vector<std::string> frags1 = {"purp", "p", "ur", "le", "purpl"};
    assert(countWaysToConstruct("purple", frags1) == 2);
    
    // Test 2: "abcdef" with fragments ab, abc, cd, def, abcd -> 1 way.
    std::vector<std::string> frags2 = {"ab", "abc", "cd", "def", "abcd"};
    assert(countWaysToConstruct("abcdef", frags2) == 1);
    
    // Test 3: "skateboard" with fragments bo, rd, ate, t, ska, sk, boar -> 0 ways.
    std::vector<std::string> frags3 = {"bo", "rd", "ate", "t", "ska", "sk", "boar"};
    assert(countWaysToConstruct("skateboard", frags3) == 0);
    
    // Test 4: "enterpotentpot" with fragments a, p, ent, enter, ot, o, t -> 4 ways.
    std::vector<std::string> frags4 = {"a", "p", "ent", "enter", "ot", "o", "t"};
    assert(countWaysToConstruct("enterpotentpot", frags4) == 4);
    
    // Test 5: Empty target string -> 1 way (using zero fragments).
    std::vector<std::string> frags5 = {"a", "b"};
    assert(countWaysToConstruct("", frags5) == 1);
    
    // Test 6: Single character target, matching fragment.
    std::vector<std::string> frags6 = {"x"};
    assert(countWaysToConstruct("x", frags6) == 1);
    
    // Test 7: Single character target, no matching fragment.
    std::vector<std::string> frags7 = {"y"};
    assert(countWaysToConstruct("x", frags7) == 0);
    
    // Test 8: Multiple identical fragments count separately.
    std::vector<std::string> frags8 = {"aa", "aa", "a"};
    // target "aaa": ways: a+a+a (1 way using each "a"), aa+a (using first aa), aa+a (using second aa), a+aa (using first aa), a+aa (using second aa) -> That would be 5, but careful: "a" appears once. Actually fragments: "aa" twice and "a" once. Ways to form "aaa": 
    // - a a a (using the "a" fragment once, then? we need three a's, only one "a" fragment, so no)
    // Wait, with fragments "aa", "aa", "a", how to make "aaa"? 
    // "aa" + "a" (using first "aa" and the "a") -> 2 choices for which "aa" to use, so 2 ways.
    // "a" + "aa" (using first "aa" and the "a") -> 2 ways.
    // total 4 ways. Let's compute: target "aaa" length 3, fragments: "aa" (size2), "aa" (size2), "a" (size1). 
    // Starting with "a" (the only "a"): remaining "aa" -> then "aa" (two ways) -> 2 ways.
    // Starting with "aa" (first): remaining "a" -> then "a" (one way) -> 1 way.
    // Starting with "aa" (second): remaining "a" -> then "a" (one way) -> 1 way.
    // Total 4 ways. So assert 4.
    assert(countWaysToConstruct("aaa", frags8) == 4);
    
    // Test 9: Long target with no solution, but ensure no stack overflow.
    std::vector<std::string> frags9 = {"a"};
    std::string longTarget(1000, 'b');
    assert(countWaysToConstruct(longTarget, frags9) == 0);
    
    return 0;
}
