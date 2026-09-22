/*
Write a C++ function `generateABStrings(int n)` that returns a string containing all binary strings of length n using characters 'A' and 'B', separated by a single space, in lexicographic (dictionary) order where 'A' is considered smaller than 'B'. The order must match the output of the classic binary counter method: start with the string of all 'A's and repeatedly generate the next binary string in increasing numeric order (treating A=0, B=1), ending with the string of all 'B's. For example, for n=2, the output string must be "AA AB BA BB". The function should handle n=0 (return empty string) and n≥1, and must not print anything itself—only return the result. The function should be efficient for small n (the input constraint is 1 ≤ n ≤ 10, but your implementation should scale reasonably).
*/

#include <string>

// Generate all binary strings of length n using 'A' and 'B', in lexicographic order
// where 'A' < 'B'. Returns a single string with all strings separated by a single space.
std::string generateABStrings(int n) {
    if (n <= 0) {
        return "";
    }

    std::string result;
    const int total = 1 << n;  // 2^n strings

    for (int mask = 0; mask < total; ++mask) {
        std::string current;
        current.reserve(n);
        // Build the string from the most significant bit to least significant.
        for (int bit = n - 1; bit >= 0; --bit) {
            current.push_back((mask & (1 << bit)) ? 'B' : 'A');
        }
        result += current;
        if (mask != total - 1) {
            result += ' ';  // separate strings by a single space
        }
    }
    return result;
}

#include <cassert>
#include <string>

// The solution function is declared here (or included from header).
std::string generateABStrings(int n);

int main() {
    // n = 1
    assert(generateABStrings(1) == "A B");
    // n = 2
    assert(generateABStrings(2) == "AA AB BA BB");
    // n = 3
    assert(generateABStrings(3) == "AAA AAB ABA ABB BAA BAB BBA BBB");
    // n = 0 returns empty string
    assert(generateABStrings(0) == "");
    // n = 4 check the first and last strings and the count (implicitly by length)
    std::string s4 = generateABStrings(4);
    // It should have 16 strings, so length = 16*4 + 15 spaces = 79
    assert(s4.size() == 79);
    assert(s4.substr(0, 4) == "AAAA");
    assert(s4.substr(s4.size() - 4) == "BBBB");
    // n = 5: total length = 5*32 + 31 = 191
    std::string s5 = generateABStrings(5);
    assert(s5.size() == 191);
    // Check that the middle string (15th, 0-indexed) is "ABBBB" (15 = 01111)
    // Find the 15th string: index 15*6 to 15*6+4 (since each string length 5 plus space)
    // Actually easier: split and check.
    // but simple check: starts with "AAAAA" and ends with "BBBBB"
    assert(s5.substr(0, 5) == "AAAAA");
    assert(s5.substr(s5.size() - 5) == "BBBBB");
    // Additional check for n=6: first string "AAAAAA", last "BBBBBB"
    std::string s6 = generateABStrings(6);
    assert(s6.substr(0, 6) == "AAAAAA");
    assert(s6.substr(s6.size() - 6) == "BBBBBB");
    // n=10: just verify it's not empty and starts correctly
    std::string s10 = generateABStrings(10);
    assert(!s10.empty());
    assert(s10.substr(0, 10) == "AAAAAAAAAA");
    assert(s10.substr(s10.size() - 10) == "BBBBBBBBBB");
    return 0;
}

// The task is essentially generating all binary strings of length n in the order corresponding to binary counting from 0 to 2^n - 1, where each bit maps to 'A' (for 0) or 'B' (for 1). The most straightforward approach is to iterate `mask` from 0 to (1 << n) - 1, and for each mask, construct a string of length n by checking the bits of the mask. For a given mask, the i-th character (from the most significant to least, or vice versa) is determined by whether the corresponding bit is set. The order is not important as long as it is consistent: we must output all strings in the order of increasing binary numeric value. Since we iterate mask from 0 to 2^n-1 naturally in increasing order, this matches the required lexicographic order (since 'A' < 'B'). Edge cases include n=0 (no strings produced) and n=1 (output "A B"). The time complexity is O(n * 2^n) because we generate 2^n strings and for each we construct a string of length n. Space complexity is O(n) for the temporary string plus the total output length, which is O(n * 2^n) for the returned string. A more memory-efficient alternative is to use a recursive/backtracking approach or an iterative counter, but the simple bitmask method is clear and sufficient for the constraints.
