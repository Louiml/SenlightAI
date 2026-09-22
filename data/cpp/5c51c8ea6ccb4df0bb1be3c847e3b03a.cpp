// Write a C++ function that finds all triples of three-digit integers (a, 2a, 3a) such that each of the nine digits from 1 to 9 appears exactly once across the three numbers. For example, 192, 384, 576 is a valid triple. The function should return a vector of strings, where each string contains the three numbers separated by spaces in the format "a 2a 3a", sorted in ascending order of `a`. The input is none; the output is determined solely by the constraint. This is a classic combinatorial enumeration problem where you must iterate over possible values of `a` in a specific range, extract digits carefully (using integer division and modulo), and check for digit uniqueness.

#include <cassert>
#include <vector>
#include <string>

int main() {
    // The expected known solutions (a, 2a, 3a) with digits 1-9 each once.
    std::vector<std::string> expected = {
        "192 384 576",
        "219 438 657",
        "273 546 819",
        "327 654 981"
    };
    
    std::vector<std::string> actual = findDigitPermutationTriples();
    
    // Check the number of solutions.
    assert(actual.size() == 4);
    
    // Check each expected solution is present and in order.
    for (size_t i = 0; i < expected.size(); ++i) {
        assert(actual[i] == expected[i]);
    }
    
    // Additional checks: verify each tuple's numbers are in correct ratio.
    for (const std::string& line : actual) {
        int a, b, c;
        sscanf(line.c_str(), "%d %d %d", &a, &b, &c);
        assert(b == 2 * a);
        assert(c == 3 * a);
        assert(a >= 123 && a <= 329);
    }
    
    return 0;
}

#include <vector>
#include <string>
#include <algorithm>
#include <array>

// Generate all triples (a, 2a, 3a) whose nine digits are a permutation of 1-9.
std::vector<std::string> findDigitPermutationTriples() {
    std::vector<std::string> result;
    for (int a = 123; a <= 329; ++a) {
        const int b = 2 * a;
        const int c = 3 * a;
        
        std::array<int, 9> digits = {
            a % 10, (a / 10) % 10, a / 100,
            b % 10, (b / 10) % 10, b / 100,
            c % 10, (c / 10) % 10, c / 100
        };
        
        std::sort(digits.begin(), digits.end());
        
        bool isPermutation = true;
        for (int i = 0; i < 9; ++i) {
            if (digits[i] != i + 1) {
                isPermutation = false;
                break;
            }
        }
        
        if (isPermutation) {
            result.push_back(std::to_string(a) + " " +
                             std::to_string(b) + " " +
                             std::to_string(c));
        }
    }
    return result;
}

// The key observation is that the smallest possible first number is 123 (since 1,2,3 are the smallest digits) and the largest is 329 (because 3*329 = 987, the largest three-digit number without repeating digits and without zero). For each candidate `a` in this range, compute `b = 2*a` and `c = 3*a`. Since `a` starts at 123, `b` is between 246 and 658, and `c` is between 369 and 987, all are three-digit numbers. For each, extract each digit (units, tens, hundreds) using modulo and integer division. Place the nine digits into an array, sort it, and check that the sorted array equals {1,2,3,4,5,6,7,8,9}. This ensures all nine digits appear exactly once. Edge cases: numbers can contain a zero digit (e.g., 204, 408, 612 would be invalid because digit 0 appears) — the sorted check will catch any zero because the required set has no zero. Also, ensure no digit repeats — sorting and comparing handles duplicates. Time complexity is O(207 * 9 log 9) ≈ O(1) since the loop is bounded by 207 iterations and sorting 9 elements is constant-time; space is O(1). The result should be collected in ascending order of `a`, which the loop naturally produces. Finally, format each triple as a string with spaces.
