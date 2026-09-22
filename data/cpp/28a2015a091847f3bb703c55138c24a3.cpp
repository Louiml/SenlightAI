/*
Write a C++ function `generateNonEmptySubsequences` that takes a non-empty string `str` and returns a vector of strings containing all non-empty subsequences of `str`, in the order produced by the recursive subset-generation algorithm (i.e., for each character, first exclude it, then include it). The empty subsequence must be omitted. The input will contain only lowercase English letters, with length at least 1 and at most 10. The returned vector should preserve the exact recursive ordering described by the pattern: for a string of length `n`, the output order corresponds to a binary counter where each step chooses "exclude" (0) before "include" (1) at each position, evaluated from leftmost character to rightmost. For example, for `"abc"`, the order should be `["c", "b", "bc", "a", "ac", "ab", "abc"]` (derived by processing positions left-to-right: for each index, first recurse excluding it, then include it). Do not include any `main` function; provide only the free function.
*/

#include <string>
#include <vector>

// Generate all non-empty subsequences of str, in recursive exclude-before-include order.
std::vector<std::string> generateNonEmptySubsequences(const std::string& str) {
    std::vector<std::string> result;
    std::string current;
    // Recursive helper: process index i and accumulate into 'current'
    // Since we cannot define a lambda with capture inside the function without C++11,
    // we use a private static helper function (or a lambda; here we use a helper).
    // For clarity, we implement a recursive lambda (C++11+) or a nested function.
    // The standard approach is a local function via std::function or a helper.
    // We'll use a std::function for a self-contained solution.
    // Alternatively, define a separate static function outside. Since the task requires
    // a free function, we'll include a helper as a static function inside the same file.
    // But the task says "free function" only; we can define a helper as a static free function.
    // To keep it simple, we implement recursion using a lambda (requires C++11).
    // But the solution must be self-contained; lambda is fine.
    // We'll write a lambda that captures result, current, str.
    // However, the function must be const-correct: str is const reference.
    // We'll call a recursive lambda.
    // To avoid including <functional>, we can use a small local struct or a recursive function
    // declared as a lambda with auto self. This requires C++14 or later.
    // Since the task does not specify C++ version, we'll use a helper static function outside.
    // But the instruction says "free function" only. We can define a private helper as a static
    // free function in the same file. We'll do that.
    // Define a private static helper.
    // Actually, we can just implement the recursion inside the function using a lambda with
    // std::function, but that adds <functional>. To keep minimal, we'll use a nested function
    // by defining a local struct with an operator() that is recursive.
    // For brevity, we'll use a recursive lambda with auto (C++14). That's acceptable.
    // To be safe across compilers, we'll define a static helper function below.
    // But the solution output must be a single function? The task says "free function" and
    // "with a descriptively named free function" – it may include helper functions as long as
    // they are free. We'll define a static helper.
    // Let's implement directly with a helper function outside.
    // For readability, we'll include the helper as a static free function.
    // We'll write it inside the solution code block.
    // Since the solution code block is the only code, we can have two functions.
    // The task says "free function" – it can have helpers.
    // We'll do:
    // static void solveHelper(const std::string&, std::string&, int, std::vector<std::string>&);
    // Then the main function calls it.
    // To match the original snippet, we'll replicate the logic.
    // We'll write a helper with the same signature as the snippet's solve.
    // But note: original uses pass-by-value output, but we can use reference for efficiency.
    // We'll keep it simple.
    // We'll implement the solution using a local lambda with std::function to avoid extra function.
    // Let's use std::function for clarity.
    // But we must include <functional>. That's fine.
    // Actually, we can just write a recursive lambda without std::function using C++14's auto.
    // We'll do that.
    // We'll also use const correctness: str is const reference, output is passed by reference.
    // Let's write a lambda that captures &result, &current, and str.
    // We'll call it with index 0.
    // Use auto self = [&](int idx, std::string out) -> void { ... } - but that copies.
    // To avoid copies, we can use a reference to the current string and push/pop.
    // We'll do that.
    // Implementation:
    // std::function<void(int)> dfs = [&](int index) {
    //     if (index >= str.size()) {
    //         if (!current.empty()) result.push_back(current);
    //         return;
    //     }
    //     // exclude
    //     dfs(index+1);
    //     // include
    //     current.push_back(str[index]);
    //     dfs(index+1);
    //     current.pop_back();
    // };
    // dfs(0);
    // return result;
    // This is clean. We'll use std::function.
    // Include <functional> and <string> and <vector>.
    // However, the problem statement says "free function" – we can have a free function that uses a lambda.
    // The solution must be self-contained with necessary headers.
    // We'll provide that.

    // To avoid copy overhead, we'll use a helper lambda that modifies a shared 'current' string.
    std::vector<std::string> ans;
    std::string output;
    // Use std::function for recursive lambda (works in C++11).
    std::function<void(int)> dfs = [&](int index) {
        if (index >= static_cast<int>(str.size())) {
            if (!output.empty()) {
                ans.push_back(output);
            }
            return;
        }
        // Exclude current character
        dfs(index + 1);
        // Include current character
        output.push_back(str[index]);
        dfs(index + 1);
        output.pop_back(); // backtrack
    };
    dfs(0);
    return ans;
}

#include <cassert>
#include <string>
#include <vector>

// Solution function is assumed to be declared above.
// For test, we include the function here (but in real scenario it's separate).
// We'll just write the test with the function defined above.

int main() {
    // Test 1: Example "abc"
    std::vector<std::string> r1 = generateNonEmptySubsequences("abc");
    std::vector<std::string> e1 = {"c", "b", "bc", "a", "ac", "ab", "abc"};
    assert(r1 == e1);

    // Test 2: Single character
    std::vector<std::string> r2 = generateNonEmptySubsequences("z");
    assert(r2.size() == 1 && r2[0] == "z");

    // Test 3: Two characters "ab": order should be ["b","a","ab"]
    std::vector<std::string> r3 = generateNonEmptySubsequences("ab");
    std::vector<std::string> e3 = {"b", "a", "ab"};
    assert(r3 == e3);

    // Test 4: Empty subsequence omitted for "a"
    std::vector<std::string> r4 = generateNonEmptySubsequences("a");
    assert(r4 == std::vector<std::string>{"a"});

    // Test 5: Repeated characters "aa": distinct positions => order ["a","a","aa"] but first 'a' is from index1 exclude index0, second 'a' from include index0 exclude index1, then "aa". Actually recursion: exclude0 exclude1 => empty skip; include1 => "a"; include0 exclude1 => "a"; include0 include1 => "aa". So order ["a","a","aa"]. 
    std::vector<std::string> r5 = generateNonEmptySubsequences("aa");
    std::vector<std::string> e5 = {"a", "a", "aa"};
    assert(r5 == e5);

    // Test 6: All characters same but length 3 "aaa": order should be 7 subsequences: 
    // exclude0: exclude1 exclude2 => empty skip; include2 => "a"; include1 exclude2 => "a"; include1 include2 => "aa"; 
    // include0: exclude1 exclude2 => "a"; include1 exclude2 => "aa"; include1 include2 => "aaa". 
    // So order: ["a","a","aa","a","aa","aaa"]? Wait careful: Let's enumerate.
    // Recursion tree (0=exclude, 1=include) for indices 0,1,2.
    // All leaves: 000 (skip), 001 (include index2 -> "a"), 010 (include index1 -> "a"), 011 (include1&2 -> "aa"), 100 (include0 -> "a"), 101 (include0&2 -> "aa"), 110 (include0&1 -> "aa"), 111 (all -> "aaa").
    // Order by DFS: 000, 001, 010, 011, 100, 101, 110, 111.
    // So non-empty: "a", "a", "aa", "a", "aa", "aa", "aaa". 
    std::vector<std::string> r6 = generateNonEmptySubsequences("aaa");
    std::vector<std::string> e6 = {"a", "a", "aa", "a", "aa", "aa", "aaa"};
    assert(r6 == e6);

    // Test 7: Length 0?? Task says non-empty, but if called with empty, should return empty? Not needed.
    // Test 8: Check size for "abcd": 2^4-1 = 15
    std::vector<std::string> r8 = generateNonEmptySubsequences("abcd");
    assert(r8.size() == 15);

    // Test 9: Check first element of "xyz" should be "z"
    std::vector<std::string> r9 = generateNonEmptySubsequences("xyz");
    assert(r9[0] == "z");

    // Test 10: Check last element of "xy" should be "xy"
    std::vector<std::string> r10 = generateNonEmptySubsequences("xy");
    assert(r10.back() == "xy");

    // All assertions passed
}

// To compile, need to include the solution function definition above. For this test block, we assume it's defined.

// The solution uses a recursive depth-first search (DFS) over indices of the input string. At each index, we consider two branches: first, exclude the current character and recurse into the next index; second, include the current character (append to a temporary output string) and recurse into the next index. The base case occurs when the index reaches the string length; at that point, if the accumulated `output` is non-empty, we push it into the result vector. The recursion naturally produces the order described: because we always recurse for exclusion before inclusion, all subsequences that exclude the first character appear before those that include it, and the same logic applies recursively for subsequent characters. Edge cases include a single-character string (returns one element) and a string with repeated characters (duplicates are treated as distinct subsequences based on positions). Time complexity is \(O(2^n \cdot n)\) because there are \(2^n\) subsequences (excluding empty) and each constructed string can have length up to `n`; copying strings into the result costs \(O(n)\) per subsequence. Space complexity is \(O(n)\) for the recursion stack and the temporary output string, plus \(O(2^n \cdot n)\) for the result vector itself.
