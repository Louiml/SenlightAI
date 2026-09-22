Given a binary string of length n (containing only characters '0' and '1'), write a C++ function that returns the number of substrings that contain exactly one '1'. The function should accept a `const std::string&` and return an `unsigned long long` value because the result may be large. For example, in the string "101", the substrings with exactly one '1' are "1", "0", "1", "10", "01", and "101" — wait, let's clarify: a substring is contiguous, so for "101", the total substrings are: "1","0","1","10","01","101". Those with exactly one '1' are "1", "0", "1", "10", "01" (the first and last single "1"s each have one '1', the middle "0" has zero, "10" has one, "01" has one, "101" has two). So count = 5. However, the original snippet counts something else: it counts the total number of substrings that contain at least one '1'? Actually the original counts total substrings of all-ones groups? Let's reinterpret the task: The snippet counts the number of substrings that consist entirely of '1's? No, it counts c = number of '1's, then returns c*(c+1)/2 which is the number of substrings formed by a contiguous block of c ones. But the input string may have zeros in between, so that formula is wrong for general binary strings. The intended task from the snippet is likely: Given a binary string, count the number of substrings that contain at least one '1' (or exactly one '1'?). Looking at the code: it counts total ones c, then computes c*(c+1)/2. That equals the number of substrings of a string of length c consisting entirely of '1's. But for arbitrary binary strings, that's not the count. The task should be redefined as: Given a binary string, return the number of substrings that contain only '1's, i.e., substrings that are entirely made up of '1's. For each maximal block of consecutive '1's of length L, it contributes L*(L+1)/2 substrings. Sum over all blocks. The original code incorrectly sums a single block of all ones, but we'll correct it. So the task: Write a function `countAllOnesSubstrings(const std::string& s)` that returns the total number of substrings consisting solely of '1's. For example, "101" has blocks: "1" length 1 -> 1 substring, "1" length 1 -> 1 substring, total 2. For "110011", blocks: "11" length 2 -> 3, and "11" length 2 -> 3, total 6. The result may exceed 32-bit, so use `unsigned long long`. Handle empty string? The problem likely assumes non-empty, but we'll handle empty by returning 0.

// The solution scans the string from left to right, tracking the current length of a consecutive run of '1's. For each character, if it is '1', we increment the current run length and add that run length to the total (because each new '1' at the end of a run creates exactly `run_length` new substrings that end at that position and are all ones). If the character is '0', we reset the current run length to zero. This additive approach sums the contribution of every maximal block: for a block of length L, the contributions are 1+2+...+L = L*(L+1)/2, because each position in the block adds a number equal to its distance from the start of the block. The algorithm runs in O(n) time where n is the length of the string, and uses O(1) auxiliary space (only a few integer variables). Edge cases: empty string returns 0; string with no '1's returns 0; string with all '1's of length n returns n*(n+1)/2; long strings may produce large values, so use `unsigned long long` to avoid overflow (max n up to 10^5 gives result ~5e9, but if n up to 10^6, result ~5e11, still fits in 64-bit; for n up to 10^7, ~5e13, still fine). The function is `const`-correct because it takes a `const std::string&` and does not modify it.

#include <string>

// Count the number of substrings that consist entirely of '1's.
// Each maximal block of consecutive '1's of length L contributes L*(L+1)/2 substrings.
unsigned long long countAllOnesSubstrings(const std::string& s) {
    unsigned long long total = 0;
    unsigned long long currentRun = 0;
    
    for (char ch : s) {
        if (ch == '1') {
            ++currentRun;
            total += currentRun;  // Each new '1' adds all new substrings ending here
        } else {
            currentRun = 0;      // Reset on any non-'1' character
        }
    }
    return total;
}

#include <cassert>
#include <string>

// Declaration of the function being tested (in practice, include the header)
unsigned long long countAllOnesSubstrings(const std::string& s);

int main() {
    assert(countAllOnesSubstrings("") == 0);
    assert(countAllOnesSubstrings("0") == 0);
    assert(countAllOnesSubstrings("1") == 1);
    assert(countAllOnesSubstrings("10") == 1);
    assert(countAllOnesSubstrings("01") == 1);
    assert(countAllOnesSubstrings("11") == 3);
    assert(countAllOnesSubstrings("101") == 2);
    assert(countAllOnesSubstrings("110011") == 6);
    assert(countAllOnesSubstrings("111000111") == 6 + 6); // 3 each block
    assert(countAllOnesSubstrings("010101") == 3);
    return 0;
}
