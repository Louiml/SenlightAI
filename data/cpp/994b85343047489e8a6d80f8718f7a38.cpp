/*
Write a C++ function that takes an integer `n` (where `n >= 1`) and returns a string containing all integers from 1 to `n` arranged so that no two consecutive numbers in the sequence have adjacent values (i.e., the absolute difference between any two consecutive numbers in the output must not be 1). The output should list even numbers first (in increasing order), followed by odd numbers (in increasing order), and be space-separated with no trailing space. For `n == 2` or `n == 3`, no such arrangement exists; in those cases, return the string `"NO SOLUTION"`. For `n == 1`, the arrangement `"1"` is trivially valid.
*/

#include <string>

// Returns a permutation of 1..n where consecutive differences are never 1.
// If impossible, returns "NO SOLUTION".
std::string beautifulPermutation(int n) {
    if (n == 2 || n == 3) {
        return "NO SOLUTION";
    }
    
    std::string result;
    
    // Append all even numbers first
    for (int i = 2; i <= n; i += 2) {
        if (!result.empty()) {
            result += ' ';
        }
        result += std::to_string(i);
    }
    
    // Append all odd numbers next
    for (int i = 1; i <= n; i += 2) {
        if (!result.empty()) {
            result += ' ';
        }
        result += std::to_string(i);
    }
    
    return result;
}

#include <cassert>
#include <string>

// Assume beautifulPermutation is declared above or included.

int main() {
    assert(beautifulPermutation(1) == "1");
    assert(beautifulPermutation(2) == "NO SOLUTION");
    assert(beautifulPermutation(3) == "NO SOLUTION");
    assert(beautifulPermutation(4) == "2 4 1 3");
    assert(beautifulPermutation(5) == "2 4 1 3 5");
    assert(beautifulPermutation(6) == "2 4 6 1 3 5");
    // Verify no adjacent difference equals 1 for n=7
    std::string s7 = beautifulPermutation(7); // "2 4 6 1 3 5 7"
    assert(s7 == "2 4 6 1 3 5 7");
    // For n=10, check that key boundary values differ by at least 2
    assert(beautifulPermutation(10) == "2 4 6 8 10 1 3 5 7 9");
    return 0;
}

// The key observation is that if we list all even numbers from 1 to `n` in increasing order, then all odd numbers in increasing order, the only possible adjacent conflicts occur at the boundary between the last even and the first odd (or vice versa). For example, with `n = 4`, evens are 2, 4 and odds are 1, 3 → sequence 2,4,1,3: differences are 2,3,2 — all fine. For `n = 5`: evens 2,4, odds 1,3,5 → 2,4,1,3,5: differences 2,3,2,2 — fine. The only problematic cases are small `n` where the evens and odds are interleaved too tightly: for `n = 2` (evens: 2, odds: 1 → diff=1) and for `n = 3` (evens: 2, odds: 1,3 → diff between 2 and 1 is 1, also between 1 and 3 is 2 but 2 and 1 conflict). For `n >= 4`, this even-then-odd pattern always works because the largest even ≤ n and the smallest odd ≥ 1 differ by at least 2 (e.g., n=4: last even=4, first odd=1 → diff=3; n=5: last even=4, first odd=1 → diff=3; n=6: last even=6, first odd=1 → diff=5). Edge cases: `n=1` returns `"1"` (only one number, no consecutive pairs). Time complexity is O(n) because we iterate twice over the range; space complexity is O(n) for the output string.
