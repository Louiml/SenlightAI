Write a C++ function `reverse_words_in_place(char* input)` that takes a null-terminated C-string containing words separated by single or multiple spaces, and reverses the order of the words in-place without using any additional string or array storage. The function should modify the original character array so that the words appear in reverse order while preserving the original spacing pattern between words (e.g., if there were two spaces between words in the input, there should still be two spaces between those same words in the output). The input may have leading or trailing spaces, and may be empty or contain only spaces. You may use standard library functions like `strlen` but must not allocate dynamic memory or create temporary strings.

The core algorithm uses the classic two-step reversal technique: first reverse the entire string, then reverse each individual word within the reversed string.  
1. **Find the length** of the string using `strlen` (or manual loop).  
2. **Reverse the whole string** from index 0 to length-1, so the words appear in reverse order but each word is also reversed internally.  
3. **Scan through the reversed string** and identify each word by its boundaries (spaces or the end-of-string). For each word, reverse its characters back to normal order.  
Important edge cases:  
- **Empty string or only spaces**: Reversing a string of length 0 is a no‑op; the word scan loop must handle the end-of-string properly without accessing out of bounds.  
- **Multiple consecutive spaces**: After reversing the whole string, any sequence of spaces remains in the same order, but they separate the words. The scan must detect each word start and end correctly, skipping groups of spaces.  
- **Leading/trailing spaces**: These are reversed and may appear at the end/start, but the scan handles them as spaces and does not reverse anything for them.  
Time complexity is \(O(n)\), since each character is visited a constant number of times (once during whole reversal, once during scanning, and once per word reversal). Space complexity is \(O(1)\) because all operations are in‑place.

#include <cstddef>  // for size_t

// Reverses a segment of the character array from index i to j (inclusive)
void reverse_segment(char* input, size_t i, size_t j) {
    while (i < j) {
        char temp = input[i];
        input[i] = input[j];
        input[j] = temp;
        ++i;
        --j;
    }
}

// Reverses the order of words in the input C-string in-place,
// preserving the spacing pattern between words.
void reverse_words_in_place(char* input) {
    // Handle empty or null input
    if (input == nullptr || input[0] == '\0') {
        return;  // Nothing to do
    }

    // Find the length of the string (excluding null terminator)
    size_t length = 0;
    while (input[length] != '\0') {
        ++length;
    }

    // Step 1: Reverse the entire string
    reverse_segment(input, 0, length - 1);

    // Step 2: Reverse each individual word back to normal order
    size_t start = 0;
    size_t i = 0;
    while (i <= length) {
        // Check if we reached a space or the end of the string
        if (i == length || input[i] == ' ') {
            // Check if there is a non-empty word between 'start' and 'i-1'
            if (start < i) {
                // Reverse the word from start to i-1
                reverse_segment(input, start, i - 1);
            }
            // Skip all consecutive spaces after this position
            while (i < length && input[i] == ' ') {
                ++i;
            }
            // Set start for the next word (or the next space block)
            start = i;
        } else {
            // Not a space, move to next character
            ++i;
        }
    }
}

#include <cassert>
#include <cstring>

int main() {
    // Test 1: Basic reversal with single spaces
    char str1[] = "i like coding";
    reverse_words_in_place(str1);
    assert(strcmp(str1, "coding like i") == 0);

    // Test 2: Multiple spaces between words
    char str2[] = "hello   world  test";
    reverse_words_in_place(str2);
    assert(strcmp(str2, "test  world   hello") == 0);

    // Test 3: Single word, should remain unchanged
    char str3[] = "single";
    reverse_words_in_place(str3);
    assert(strcmp(str3, "single") == 0);

    // Test 4: Empty string
    char str4[] = "";
    reverse_words_in_place(str4);
    assert(strcmp(str4, "") == 0);

    // Test 5: Leading and trailing spaces
    char str5[] = "  leading and trailing  ";
    reverse_words_in_place(str5);
    assert(strcmp(str5, "  trailing and leading  ") == 0);

    // Test 6: Only spaces
    char str6[] = "   ";
    reverse_words_in_place(str6);
    assert(strcmp(str6, "   ") == 0);

    // Test 7: Two words with three spaces in between
    char str7[] = "ab   cd";
    reverse_words_in_place(str7);
    assert(strcmp(str7, "cd   ab") == 0);

    // Test 8: Null pointer handling (should not crash)
    char* str8 = nullptr;
    reverse_words_in_place(str8);

    // Test 9: Words with different lengths
    char str9[] = "a bb ccc dddd";
    reverse_words_in_place(str9);
    assert(strcmp(str9, "dddd ccc bb a") == 0);

    // Test 10: Already reversed input
    char str10[] = "backwards is this";
    reverse_words_in_place(str10);
    assert(strcmp(str10, "this is backwards") == 0);

    return 0;
}
