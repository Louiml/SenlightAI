Write a C++ function that, given an integer `n` followed by exactly `n` integers read from standard input, determines whether the sequence is "EASY" or "HARD". The sequence is considered "HARD" if at any point while processing the numbers in order, the bitwise OR of all numbers seen so far (including the current one) is non-zero. However, the twist is that the OR operation is performed cumulatively: each new number is OR-ed with the running total. If the running total ever becomes non-zero, the function should immediately return a string `"HARD"`; otherwise, after processing all numbers, return `"EASY"`. The input will always be valid: `n` is a positive integer (≥1), and the next `n` integers are provided. The function must read from `cin` and return the appropriate string.

#include <iostream>
#include <string>
#include <cassert>

// Function declaration (assumed defined elsewhere)
std::string determineDifficulty(int n);

int main() {
    // Helper to simulate input via string buffer is not trivial with cin.
    // Instead, we test the function by temporarily redirecting cin.
    // These tests require manual input setup; for demonstration we will
    // use a sequence of redirections in separate scopes.
    
    {
        std::istringstream input("3 0 0 0");
        std::cin.rdbuf(input.rdbuf());
        assert(determineDifficulty(3) == "EASY");
    }
    {
        std::istringstream input("3 0 5 0");
        std::cin.rdbuf(input.rdbuf());
        assert(determineDifficulty(3) == "HARD");
    }
    {
        std::istringstream input("1 0");
        std::cin.rdbuf(input.rdbuf());
        assert(determineDifficulty(1) == "EASY");
    }
    {
        std::istringstream input("1 -7");
        std::cin.rdbuf(input.rdbuf());
        assert(determineDifficulty(1) == "HARD");
    }
    {
        std::istringstream input("4 0 0 0 0");
        std::cin.rdbuf(input.rdbuf());
        assert(determineDifficulty(4) == "EASY");
    }
    {
        std::istringstream input("2 0 3");
        std::cin.rdbuf(input.rdbuf());
        assert(determineDifficulty(2) == "HARD");
    }
    {
        std::istringstream input("5 0 0 1 0 0");
        std::cin.rdbuf(input.rdbuf());
        assert(determineDifficulty(5) == "HARD");
    }
    {
        std::istringstream input("3 1 0 0");
        std::cin.rdbuf(input.rdbuf());
        assert(determineDifficulty(3) == "HARD");
    }
    
    std::cout << "All tests passed!" << std::endl;
    return 0;
}

#include <iostream>
#include <string>

// Reads n integers from standard input and returns "HARD" if the cumulative
// bitwise OR of all numbers seen so far becomes non-zero at any point.
// Otherwise returns "EASY".
std::string determineDifficulty(int n) {
    int running_or = 0;
    for (int i = 0; i < n; ++i) {
        int current;
        std::cin >> current;
        running_or |= current;
        if (running_or != 0) {
            return "HARD";
        }
    }
    return "EASY";
}

// The core algorithm is straightforward: iterate through the `n` numbers while maintaining a running OR value. For each number, update the running OR by `running |= current`. After updating, check if the running OR is non-zero. If it is, return `"HARD"` immediately—because the problem states “if op is non-zero at any point, print HARD”. Importantly, the OR of any non-zero number with anything is non-zero, so if the first number is non-zero, the function immediately returns HARD. If all numbers are zero, the running OR remains zero forever, so after processing all n numbers we return `"EASY"`. 
//
// Edge cases: (1) `n=1` with value 0 → EASY; value non-zero → HARD. (2) A sequence like `0 0 0` → EASY. (3) A sequence like `0 5 0` → HARD because after reading 5 the OR becomes 5 (non-zero). (4) The OR operation is idempotent, so mixing zeros and non-zeros: the moment a non-zero appears, the result is HARD. No special handling is needed for negative numbers because bitwise OR works on signed integers, but any non-zero value (positive or negative) will produce a non-zero OR. Time complexity is O(n) because we process each number once. Space complexity is O(1) because we only store the running OR and the current integer.
