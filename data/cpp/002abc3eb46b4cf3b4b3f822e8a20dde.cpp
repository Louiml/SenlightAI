Write a C++ function named `printPermutations` that takes a non-empty string and prints every distinct permutation of its characters to standard output, one per line. The function must generate permutations in lexicographic (dictionary) order, and it must handle strings containing repeated characters by printing each distinct permutation only once (no duplicates). The function should return `void` and accept the input string as a `const std::string&`. For clarity, you may use a helper function to perform the recursive backtracking, but the main public function must be `printPermutations`. The implementation should be self-contained, include necessary headers, and avoid global mutable state. For example, given `"AAB"`, the output should be `AAB`, `ABA`, `BAA` (in that order), not three `AAB` and three `ABA` and three `BAA` as a naive recursion would print.

#include <cassert>
#include <sstream>
#include <iostream>

// Forward declaration of the function under test.
void printPermutations(const std::string& str);

// Helper to capture output from printPermutations.
std::string captureOutput(const std::string& input) {
    std::ostringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    printPermutations(input);
    std::cout.rdbuf(old);
    return buffer.str();
}

int main() {
    // Test 1: simple three distinct characters, lexicographic order.
    assert(captureOutput("BAC") == "ABC\nACB\nBAC\nBCA\nCAB\nCBA\n");

    // Test 2: repeated characters, no duplicates.
    assert(captureOutput("AAB") == "AAB\nABA\nBAA\n");

    // Test 3: all identical characters, only one permutation.
    assert(captureOutput("ZZZ") == "ZZZ\n");

    // Test 4: mixed with duplicates and order.
    assert(captureOutput("BCB") == "BBC\nBCB\nCBB\n");

    // Test 5: single character.
    assert(captureOutput("X") == "X\n");

    // Test 6: longer input with duplicates, check a small prefix.
    std::string out = captureOutput("ABA");
    assert(out == "AAB\nABA\nBAA\n");

    std::cout << "All tests passed!" << std::endl;
    return 0;
}

#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

// Helper: recursively generates unique permutations in lexicographic order.
void generate(const std::string& s, std::string& current, std::vector<bool>& used, int depth) {
    if (depth == s.length()) {
        std::cout << current << '\n';
        return;
    }
    for (int i = 0; i < s.length(); ++i) {
        // Skip if already used
        if (used[i]) continue;
        // Skip duplicates: if the same char appears earlier and that position is unused,
        // choosing this one would produce a duplicate ordering.
        if (i > 0 && s[i] == s[i - 1] && !used[i - 1]) continue;
        used[i] = true;
        current.push_back(s[i]);
        generate(s, current, used, depth + 1);
        current.pop_back();
        used[i] = false;
    }
}

// Prints every distinct permutation of the input string (non-empty) in lexicographic order.
void printPermutations(const std::string& str) {
    if (str.empty()) return; // Guard against empty input, though spec says non-empty.
    std::string sorted = str;
    std::sort(sorted.begin(), sorted.end());
    std::string current;
    std::vector<bool> used(sorted.length(), false);
    generate(sorted, current, used, 0);
}

// The core idea is to generate all unique permutations using a recursive backtracking approach where, at each step, we choose a character from the remaining set to place next, but we skip characters that are identical to a previously chosen character at the same recursion depth in order to avoid duplicates. To achieve lexicographic order, we first sort the input string so that characters are processed in increasing order. In the recursion, we maintain a `bool` vector (`used`) to mark which original positions have already been placed in the current prefix, and we also enforce a rule: when iterating over positions, if the current character equals the previous character in the sorted string and the previous position hasn't been used yet, we skip it—this ensures that identical characters are only picked in their natural sorted order among themselves. After choosing a character, we recurse with the reduced problem. The base case is when the prefix length equals the string length, at which point we print the prefix. Edge cases: an empty string is technically not allowed per the task spec, but we can guard by printing nothing; strings with all identical characters produce only one permutation. Time complexity is O(n! · n) because there are n! permutations (or fewer with duplicates) and each permutation costs O(n) to build and print. Space complexity is O(n) for the recursion stack and the `used` vector, plus O(n) for the prefix string.
