Write a C++ function named `removeFirstOccurrence` that takes a C-style character array (a null-terminated string) and a character `x`, and removes the first occurrence of `x` from the string in-place. The function should modify the original array so that the resulting string has the first matching character deleted, with all subsequent characters shifted left by one position and the array still properly null-terminated. Handle edge cases: if the character does not appear in the string, or if the string is empty, the function should leave the array unchanged. The function signature should be `void removeFirstOccurrence(char str[], char x);` and it should not use any additional arrays or standard library string functions (like `strchr`, `strlen`, `memmove`). The function must be robust for any valid input char array sized at least 100, but your implementation should not assume a fixed size; it should rely only on the null terminator.
The solution involves first determining the length of the string by iterating until the null terminator `'\0'` is found, counting characters along the way. Then, search for the first index `i` where `str[i] == x`. If found, shift all characters from index `i+1` to the end of the string (including the null terminator) one position to the left, effectively overwriting the matched character. This is done by looping from `i` to `length-1` and assigning `str[j] = str[j+1]`. After the loop, the null terminator is automatically moved one step left, so the string ends correctly. If the character is not found, no operation is performed. Edge cases include: an empty string (length 0) where the loop does nothing and no match is possible; a single-character string where the matched character becomes the null terminator after shifting; multiple occurrences where only the first is removed, leaving later ones intact. Time complexity is O(n) for measuring length plus O(n) for the shift in the worst case, yielding O(n) overall, where n is the string length. Space complexity is O(1) because we only use a few integer variables.
// Removes the first occurrence of character x from the C-string str, in-place.
// Leaves the array unchanged if x is not found or the string is empty.
void removeFirstOccurrence(char str[], char x) {
    // Calculate the length of the string; length is 0 if str[0] == '\0'
    int length = 0;
    while (str[length] != '\0') {
        ++length;
    }

    // Find the index of the first occurrence of x
    int index = -1;
    for (int i = 0; i < length; ++i) {
        if (str[i] == x) {
            index = i;
            break;
        }
    }

    // If x was found, shift all characters after it left by one
    if (index != -1) {
        for (int j = index; j < length; ++j) {
            str[j] = str[j + 1];
        }
        // No need to add null terminator manually because str[length] was '\0'
        // and it is now copied to str[length-1], and the loop stops at j = length-1
        // (since we access str[j+1] up to str[length] which is null).
    }
}
int main() {
    // Test 1: Basic removal from a multi-character string
    char s1[] = "hello";
    removeFirstOccurrence(s1, 'l');
    assert(strcmp(s1, "helo") == 0);

    // Test 2: Character appears multiple times, only first removed
    char s2[] = "banana";
    removeFirstOccurrence(s2, 'a');
    assert(strcmp(s2, "bnana") == 0);

    // Test 3: Character not present, string unchanged
    char s3[] = "test";
    removeFirstOccurrence(s3, 'z');
    assert(strcmp(s3, "test") == 0);

    // Test 4: Empty string, no change
    char s4[] = "";
    removeFirstOccurrence(s4, 'a');
    assert(strcmp(s4, "") == 0);

    // Test 5: Single character string, removal leaves empty string
    char s5[] = "x";
    removeFirstOccurrence(s5, 'x');
    assert(strcmp(s5, "") == 0);

    // Test 6: Character is first in string
    char s6[] = "abc";
    removeFirstOccurrence(s6, 'a');
    assert(strcmp(s6, "bc") == 0);

    // Test 7: Character is last in string
    char s7[] = "abcd";
    removeFirstOccurrence(s7, 'd');
    assert(strcmp(s7, "abc") == 0);

    // Test 8: String with spaces and special characters
    char s8[] = "hello world";
    removeFirstOccurrence(s8, ' ');
    assert(strcmp(s8, "helloworld") == 0);

    // Test 9: Repeated removal calls on same array (first call removes first 'a', second removes next)
    char s9[] = "aabc";
    removeFirstOccurrence(s9, 'a');
    removeFirstOccurrence(s9, 'a');
    assert(strcmp(s9, "bc") == 0);

    // Test 10: Character is not found but string contains null-like characters (not applicable, but ensure no crash)
    char s10[] = "no";
    removeFirstOccurrence(s10, 'n');
    removeFirstOccurrence(s10, 'o');
    assert(strcmp(s10, "") == 0);
}
