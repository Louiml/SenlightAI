Write a C++ function named `reverseLinesSkipFirst` that takes a null-terminated C-string containing multiple lines separated by newline characters, reverses each individual line in place, and returns the number of lines that were processed. The function must only reverse lines after the first line — the first line in the input must be left unchanged (do not reverse it, but it still counts as a processed line if it ends with a newline). The input string may contain trailing newlines, and the function must handle both Windows-style (`\r\n`) and Unix-style (`\n`) line endings, normalizing the reversal so that `\r` is treated as part of the line content. The function should modify the original string and return the total count of lines (including the first) that appear in the input. For example, given the input `"first\nsecond\nthird"`, the function should transform it to `"first\ncondes\nriht"` and return 3. If the input is empty, return 0. If there is no newline at all, return 1 and do not modify the string (since only the first line exists). The function must be robust against strings with up to 10000 characters.

#include <cassert>
#include <cstring>

int main() {
    // Test 1: Standard Unix line endings, multiple lines.
    char str1[] = "first\nsecond\nthird";
    assert(reverseLinesSkipFirst(str1) == 3);
    assert(std::strcmp(str1, "first\ncondes\nriht") == 0);

    // Test 2: Windows line endings.
    char str2[] = "one\r\ntwo\r\nthree\r\n";
    assert(reverseLinesSkipFirst(str2) == 4); // The trailing \n creates an empty last line.
    assert(std::strcmp(str2, "one\r\nowt\r\neerht\r\n") == 0);

    // Test 3: Single line without newline.
    char str3[] = "hello";
    assert(reverseLinesSkipFirst(str3) == 1);
    assert(std::strcmp(str3, "hello") == 0);

    // Test 4: Empty string.
    char str4[] = "";
    assert(reverseLinesSkipFirst(str4) == 0);
    assert(std::strcmp(str4, "") == 0);

    // Test 5: Consecutive newlines (empty middle lines).
    char str5[] = "abc\n\ndef\n";
    assert(reverseLinesSkipFirst(str5) == 4); // abc, empty, fed, empty after trailing newline.
    assert(std::strcmp(str5, "abc\n\nfed\n") == 0);

    // Test 6: Only newlines (first line empty, second empty)
    char str6[] = "\n";
    assert(reverseLinesSkipFirst(str6) == 2); // empty first line, empty second
    assert(std::strcmp(str6, "\n") == 0);

    // Test 7: First line has no newline, but second exists (should not happen if first has no newline? Actually first line ends at first newline)
    char str7[] = "x\nyz";
    assert(reverseLinesSkipFirst(str7) == 2);
    assert(std::strcmp(str7, "x\nzy") == 0);

    // Test 8: String with spaces and punctuation.
    char str8[] = "keep\nrace car\n";
    assert(reverseLinesSkipFirst(str8) == 3); // keep, race car, empty
    assert(std::strcmp(str8, "keep\nrac ecar\n") == 0);

    // Test 9: Carriage return only (no newline).
    char str9[] = "a\rb";
    assert(reverseLinesSkipFirst(str9) == 1);
    assert(std::strcmp(str9, "a\rb") == 0);

    // Test 10: Long string with many lines to ensure no overflow.
    char str10[10000];
    memset(str10, 0, sizeof(str10));
    char* ptr = str10;
    const char* first = "line0";
    strcpy(ptr, first);
    ptr += strlen(first);
    for (int i = 1; i < 100; ++i) {
        *ptr++ = '\n';
        char buf[16];
        sprintf(buf, "line%d", i);
        strcpy(ptr, buf);
        ptr += strlen(buf);
    }
    *ptr = '\0';
    int count = reverseLinesSkipFirst(str10);
    assert(count == 100);
    // Verify first unchanged.
    assert(strncmp(str10, "line0\n", 6) == 0);
    // Verify last line reversed: "line99" -> "99enil"
    assert(strcmp(strrchr(str10, '\n') + 1, "99enil") == 0);
}

#include <cstddef>

// Reverses each line after the first in a multi-line C-string.
// Returns the total number of lines (including the first) present in the input.
// The first line is left unchanged. Handles '\r\n' and '\n' line endings.
// Modifies the input string in place.
int reverseLinesSkipFirst(char* str) {
    if (str == nullptr || str[0] == '\0') {
        return 0;
    }

    int lineCount = 0;
    char* lineStart = str;
    char* p = str;

    // Scan through the string to find each line.
    while (*p != '\0') {
        if (*p == '\n') {
            // We have found a complete line ending with '\n'.
            lineCount++;
            // If this is the first line, do nothing; otherwise reverse the line content.
            if (lineCount > 1) {
                // Reverse characters from lineStart to p (exclusive), treating '\r' as content.
                char* left = lineStart;
                char* right = p - 1;
                while (left < right) {
                    char temp = *left;
                    *left = *right;
                    *right = temp;
                    left++;
                    right--;
                }
            }
            // Move to the next line after the newline.
            p++;
            lineStart = p;
            continue;
        }
        p++;
    }

    // Handle the last line after the final newline (or the whole string if no newline).
    if (lineStart != p) {
        lineCount++;
        // If this is not the first line, reverse it.
        if (lineCount > 1) {
            char* left = lineStart;
            char* right = p - 1;
            while (left < right) {
                char temp = *left;
                *left = *right;
                *right = temp;
                left++;
                right--;
            }
        }
    } else if (lineCount == 0) {
        // The string is entirely empty (though we already returned 0 for empty string).
        return 0;
    } else {
        // The string ended with a newline, so an empty last line exists.
        lineCount++;
    }

    return lineCount;
}

// The solution processes the string in a single pass, identifying each line by scanning for `\n` characters. A line is defined as a segment from the current position up to (but not including) the next `\n`, or to the end of the string if no `\n` is found. We need to track whether we are processing the first line: if so, we do not reverse its content, but we still count it as a line. For subsequent lines, we reverse the characters between the line start and the newline separator (or end of string). To handle `\r` correctly, we treat `\r` as a regular character — when reversing, the `\r` will move to the front of the line if it was originally at the end, which matches typical behavior for line-ending normalization. Edge cases include: empty string (return 0), a single line without newline (return 1, no change), multiple lines with trailing newline (count includes the empty line? Actually if the string ends with `\n`, the last "line" is empty — we count that as a line, but reversing an empty line does nothing), and consecutive newlines (empty lines). The algorithm uses a pointer to traverse the string, counting newlines encountered to determine line boundaries. For each line after the first, we reverse the substring in place using a two-pointer swap. Time complexity is O(n) where n is the length of the string, because each character is visited at most twice (once during scanning, once during reversal). Space complexity is O(1) auxiliary, as we modify the string in place and use only a few integer/pointer variables.
