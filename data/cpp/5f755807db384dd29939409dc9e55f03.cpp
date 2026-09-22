/*
Write a C++ function named `countConsecutiveDuplicates` that takes an integer `n` and a string `s` as parameters, where `n` represents the number of characters in `s` (the length of the string). The function should return an integer representing the number of positions `i` (where `1 ≤ i < n`) such that the character at index `i` is equal to the character at index `i-1`. In other words, count how many times a character is immediately followed by the same character. The input string may contain any printable characters (letters, digits, spaces, etc.), and `n` will always match the actual length of `s`. You may assume `n` is a positive integer. The function should be const-correct (take the string by const reference) and should not modify the input.
*/

#include <string>

// Counts the number of adjacent character pairs that are equal.
// n must equal s.length().
int countConsecutiveDuplicates(int n, const std::string& s) {
    int count = 0;
    for (int i = 1; i < n; ++i) {
        if (s[i] == s[i - 1]) {
            ++count;
        }
    }
    return count;
}

#include <cassert>
#include <string>

// Declaration of the solution function (include it here for testing)
int countConsecutiveDuplicates(int n, const std::string& s);

int main() {
    // Test cases
    assert(countConsecutiveDuplicates(1, "a") == 0);
    assert(countConsecutiveDuplicates(3, "aaa") == 2);
    assert(countConsecutiveDuplicates(3, "aba") == 0);
    assert(countConsecutiveDuplicates(5, "aabbc") == 2);
    assert(countConsecutiveDuplicates(7, "hello  ") == 2); // "ll" and "  " (two spaces)
    assert(countConsecutiveDuplicates(4, "1233") == 1);
    assert(countConsecutiveDuplicates(6, "XXXXXX") == 5);
    assert(countConsecutiveDuplicates(2, "ab") == 0);
    assert(countConsecutiveDuplicates(2, "AA") == 1);
    assert(countConsecutiveDuplicates(0, "") == 0); // n=0 as edge case (though spec says positive, test anyway)
    
    return 0;
}

// The algorithm iterates through the string from the second character (index 1) to the end. For each index `i`, we compare `s[i]` with `s[i-1]`. If they are equal, we increment a counter. At the end, the counter is returned.  
// Edge cases:  
// - `n` is 1: the loop never executes (since `i=1` and condition `i<n` is false), so returns 0.  
// - All characters same: e.g., "aaa" → count = 2 (positions 1 and 2 match their predecessor).  
// - No adjacent equal characters: returns 0.  
// - Input may contain spaces or special characters; comparisons are done using `char` equality, which works fine.  
// Time complexity: O(n) because we traverse the string once. Space complexity: O(1) auxiliary space since we only use a counter and loop index.
