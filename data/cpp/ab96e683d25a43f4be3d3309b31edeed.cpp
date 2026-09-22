Write a C++ function `beautifulPermutation(int n)` that returns a string containing a permutation of integers from 1 to n such that no two adjacent numbers differ by exactly 1. If no such permutation exists for the given n, return the string `"NO SOLUTION"`. The function must handle any integer n from 1 to 10^6, and the output string should have numbers separated by single spaces with no trailing space. For example, for n=4, a valid output is `"2 4 1 3"` (differences are 2, 3, 2 — none equal 1), while for n=2 or n=3, no such permutation exists.

The key observation is that for n ≥ 4, a valid permutation can always be constructed by first outputting all even numbers in increasing order, then all odd numbers in increasing order. This works because even numbers differ by at least 2 among themselves, odd numbers differ by at least 2 among themselves, and the only boundary (between the last even and the first odd, or between the last odd and the first even if reversed) also differs by at least 2 when n ≥ 4. For n=1, the permutation is trivially valid (single number, no adjacent pair). For n=2 and n=3, no valid permutation exists because the only permutations of {1,2} and {1,2,3} always contain adjacent numbers differing by 1. Edge cases: n=1 returns "1"; n=2 and n=3 return "NO SOLUTION"; n=4 returns "2 4 1 3"; n=5 returns "2 4 1 3 5" (differences 2,3,2,2). Time complexity is O(n) because we iterate over n numbers to build the string. Space complexity is O(n) for the resulting string, and O(1) auxiliary beyond that.

#include <string>

// Returns a permutation of 1..n such that no adjacent numbers differ by exactly 1.
// Returns "NO SOLUTION" if no such permutation exists (n == 2 or n == 3).
std::string beautifulPermutation(int n) {
    if (n == 2 || n == 3) {
        return "NO SOLUTION";
    }
    if (n == 1) {
        return "1";
    }

    std::string result;

    // Append all even numbers first
    for (int i = 2; i <= n; i += 2) {
        if (!result.empty()) {
            result += " ";
        }
        result += std::to_string(i);
    }

    // Append all odd numbers next
    for (int i = 1; i <= n; i += 2) {
        if (!result.empty()) {
            result += " ";
        }
        result += std::to_string(i);
    }

    return result;
}

#include <cassert>
#include <string>

// Forward declaration of the function under test (simulates including the solution)
std::string beautifulPermutation(int n);

int main() {
    // n=1: single element, trivially valid
    assert(beautifulPermutation(1) == "1");

    // n=2,3: no valid permutation exists
    assert(beautifulPermutation(2) == "NO SOLUTION");
    assert(beautifulPermutation(3) == "NO SOLUTION");

    // n=4: known valid output
    assert(beautifulPermutation(4) == "2 4 1 3");

    // n=5: even-then-odd works
    assert(beautifulPermutation(5) == "2 4 1 3 5");

    // n=6: even-then-odd works
    assert(beautifulPermutation(6) == "2 4 6 1 3 5");

    // n=10: verify no adjacent difference is 1 by parsing the string
    std::string p = beautifulPermutation(10);
    int prev = 0;
    bool valid = true;
    int pos = 0;
    while (pos < (int)p.size()) {
        int nextPos = p.find(' ', pos);
        if (nextPos == std::string::npos) nextPos = p.size();
        int num = std::stoi(p.substr(pos, nextPos - pos));
        if (prev != 0 && std::abs(num - prev) == 1) {
            valid = false;
            break;
        }
        prev = num;
        pos = nextPos + 1;
    }
    assert(valid);

    // n=100: ensure output length makes sense (contains all numbers 1..n)
    std::string big = beautifulPermutation(100);
    int count = 1;
    for (char c : big) if (c == ' ') count++;
    assert(count == 100);

    return 0;
}
