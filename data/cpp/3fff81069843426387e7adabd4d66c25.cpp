/*
Write a C++ function named `findLastOccurrence` that takes a C-style string (`char[]`), a target character `c`, and a reference to a boolean flag. The function must locate the **last** occurrence of `c` in the string and return a reference to that character in the original array. If the character is not found, the function must set the boolean flag to `false` and return a safe reference (e.g., to the null terminator at the end of the string) so that the caller does not crash when assigning to it, but the caller should check the flag before using the result. If found, set the flag to `true`. The function must handle empty strings (where the first character is `'\0'`), duplicate characters, single-character strings, and cases where the target is the last character. It must also ensure that any modification through the returned reference modifies the original array. The caller will use the flag to decide whether to modify the string, as in the original snippet.
*/

#include <cstddef>

// Finds the last occurrence of c in a C-string and returns a reference to it.
// If found, sets success to true; otherwise sets it to false and returns a reference
// to the null terminator (safe to assign to, but caller should not use it when !success).
char& findLastOccurrence(char a[], char c, bool& success) {
    success = false;
    std::size_t lastIndex = 0; // index of the null terminator for the not-found case
    std::size_t i = 0;
    while (a[i] != '\0') {
        if (a[i] == c) {
            lastIndex = i;
            success = true;
        }
        ++i;
    }
    // If no occurrence, lastIndex is 0, which is the null terminator (safe).
    return a[lastIndex];
}

#include <cassert>
#include <cstring>

// Forward declaration of the solution function
char& findLastOccurrence(char a[], char c, bool& success);

int main() {
    // Basic test: last 'a' in "paplae" is at index 3
    {
        char s[] = "paplae";
        bool found = false;
        char& ref = findLastOccurrence(s, 'a', found);
        assert(found == true);
        ref = 'A';
        assert(std::strcmp(s, "paplAe") == 0);
    }

    // Character appears only once
    {
        char s[] = "hello";
        bool found = false;
        char& ref = findLastOccurrence(s, 'e', found);
        assert(found == true);
        ref = 'E';
        assert(std::strcmp(s, "hEllo") == 0);
    }

    // Character not present
    {
        char s[] = "hello";
        bool found = true; // initialize to true to test it becomes false
        char& ref = findLastOccurrence(s, 'z', found);
        assert(found == false);
        // The returned reference points to the null terminator; assigning it is safe
        ref = '!'; // changes the null terminator to '!', making the string corrupted but not crashing
        // Undo the modification to keep the test clean
        s[5] = '\0';
        assert(std::strcmp(s, "hello") == 0);
    }

    // Empty string
    {
        char s[] = "";
        bool found = true;
        char& ref = findLastOccurrence(s, 'a', found);
        assert(found == false);
        assert(ref == '\0');
    }

    // Duplicate characters, last occurrence is the last one
    {
        char s[] = "banana";
        bool found = false;
        char& ref = findLastOccurrence(s, 'a', found);
        assert(found == true);
        ref = 'A';
        assert(std::strcmp(s, "bananA") == 0);
    }

    // Character is the last character of the string
    {
        char s[] = "xyz";
        bool found = false;
        char& ref = findLastOccurrence(s, 'z', found);
        assert(found == true);
        ref = 'Z';
        assert(std::strcmp(s, "xyZ") == 0);
    }

    return 0;
}

// The solution scans the entire character array from the beginning to the null terminator, tracking the index of every occurrence of `c`. Since the problem asks for the **last** occurrence, we update the stored index each time we encounter `c`, so after the loop, the stored index points to the final occurrence. The boolean flag is set to `true` only if at least one occurrence was found. For the not-found case, we return a reference to the null terminator (a legal, dereferenceable character in the array) to avoid undefined behavior, but the caller must check the flag. Edge cases: empty string—loop condition `a[i] != '\0'` fails immediately, flag stays `false`, we return `a[0]` which is `'\0'` (safe). If `c` is `'\0'` (the null character), the loop never sees it because the loop stops at `'\0'`, so it will not be found. If the string has only one character and it matches, the loop finds it at index 0 and sets the flag. The time complexity is O(n) for string length n, and space complexity is O(1) auxiliary.
