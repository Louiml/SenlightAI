Write a standalone C++ function `void removeConsecutiveDuplicates(char* input)` that modifies a null-terminated character array in-place by removing all consecutive duplicate characters, keeping only the first occurrence of each run. The function must handle a `nullptr` pointer and an empty string gracefully. For example, `"aaabbbccc"` becomes `"abc"`, `"aabbaa"` becomes `"aba"` (because the second `a` run is separate), and `"a"` remains `"a"`. The function must not allocate new memory, must not use the standard library string or vector classes, and must operate directly on the original character array. The signature and behavior must match the given code snippet exactly.

// The core algorithm uses two indices: a read index `i` and a write index `j`. Both start at 0. We iterate through the entire string using `i`. For each character at position `i`, we check if it is either the first character (`i==0`) or different from the previous character `input[i-1]`. If so, we copy it to the write position `input[j]` and increment `j`. This effectively shifts the non-duplicate characters forward in the array. At the end, we place the null terminator at position `j`. The algorithm handles edge cases: `nullptr` and empty string both trigger an early return without modification. A string with all unique characters will simply copy itself back. A string with all identical characters will result in a single character. The time complexity is O(n) because each character is visited once by `i` and each character is written at most once by `j`. The space complexity is O(1) because only two integer variables are used in addition to the input array itself.

#include <cstddef> // for nullptr

/**
 * Removes consecutive duplicate characters from a null-terminated char array in-place.
 * 
 * @param input Pointer to a mutable null-terminated character array.
 *              If nullptr or empty, the function does nothing.
 */
void removeConsecutiveDuplicates(char* input) {
    if (input == nullptr || *input == '\0') {
        return;
    }
    int readIndex = 0; // scanning the original string
    int writeIndex = 0; // position to write the next unique character
    
    while (input[readIndex] != '\0') {
        // Keep a character if it is the first one or differs from its predecessor
        if (readIndex == 0 || input[readIndex] != input[readIndex - 1]) {
            input[writeIndex] = input[readIndex];
            ++writeIndex;
        }
        ++readIndex;
    }
    // Terminate the modified string
    input[writeIndex] = '\0';
}

#include <cassert>
#include <cstring>

// The solution function is declared above (include it here in practice).
void removeConsecutiveDuplicates(char* input);

int main() {
    // Test 1: General case with mixed runs
    char test1[] = "aaabbbccc";
    removeConsecutiveDuplicates(test1);
    assert(std::strcmp(test1, "abc") == 0);

    // Test 2: Non-adjacent duplicates are kept
    char test2[] = "aabbaa";
    removeConsecutiveDuplicates(test2);
    assert(std::strcmp(test2, "aba") == 0);

    // Test 3: Single character
    char test3[] = "a";
    removeConsecutiveDuplicates(test3);
    assert(std::strcmp(test3, "a") == 0);

    // Test 4: All identical characters
    char test4[] = "zzzzz";
    removeConsecutiveDuplicates(test4);
    assert(std::strcmp(test4, "z") == 0);

    // Test 5: Already no duplicates
    char test5[] = "abc";
    removeConsecutiveDuplicates(test5);
    assert(std::strcmp(test5, "abc") == 0);

    // Test 6: Empty string
    char test6[] = "";
    removeConsecutiveDuplicates(test6);
    assert(std::strcmp(test6, "") == 0);

    // Test 7: Nullptr input (should not crash; no assertion needed)
    removeConsecutiveDuplicates(nullptr);

    // Test 8: Duplicates at end of string
    char test8[] = "abcdddd";
    removeConsecutiveDuplicates(test8);
    assert(std::strcmp(test8, "abcd") == 0);

    // Test 9: Duplicates at start and end
    char test9[] = "xxxabcxxx";
    removeConsecutiveDuplicates(test9);
    assert(std::strcmp(test9, "xabcx") == 0);

    // Test 10: Mixed symbols and digits
    char test10[] = "1122334455";
    removeConsecutiveDuplicates(test10);
    assert(std::strcmp(test10, "12345") == 0);

    return 0;
}
