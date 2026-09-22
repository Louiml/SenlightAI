Write a C++ function named `advanceAndReturn` that takes a single character by reference and increments it (e.g., `'a'` becomes `'b'`, `'z'` becomes `'{'` in ASCII). The function should return the same character reference so that it can be chained or used in expressions. Then, in a separate test, use this function on an array of characters to demonstrate that the original array is permanently modified. The task focuses on correct use of references, understanding that modifying a reference modifies the original object, and returning a reference from a function is valid when the referenced object outlives the function call.
The core of the task is to implement a function that increments a character and returns a reference to that same character. The function signature is `char& advanceAndReturn(char& c)`. Inside, we do `++c;` to modify the original character (since `c` is a reference), then `return c;`. Because we return a reference, the caller can use the result in further expressions, but more importantly, the modification persists after the function returns. For the array test, we iterate over each element, call `advanceAndReturn(a[i])`, and observe that after the loop, every element of the array has been incremented by one. Edge cases include characters at the end of the ASCII range (e.g., `'\x7F'`), where incrementing may produce a non-printable character, but this is acceptable because we are not printing the result during the function; we just modify it. Time complexity is O(n) for n characters in the array, and space complexity is O(1) extra space.
#include <cstddef>

// Increments the given character by one and returns a reference to it.
// The caller can use the returned reference for further operations.
char& advanceAndReturn(char& c) {
    ++c;
    return c;
}
#include <cassert>

char& advanceAndReturn(char& c); // Declared for use, defined above.

int main() {
    // Test single character modification.
    char ch = 'x';
    advanceAndReturn(ch);
    assert(ch == 'y');

    // Test that the returned reference points to the same variable.
    char another = 'M';
    char& result = advanceAndReturn(another);
    assert(result == 'N');
    assert(&result == &another); // Same address.

    // Test chaining using the returned reference.
    char chain = 'a';
    advanceAndReturn(chain) = 'z';
    assert(chain == 'z');

    // Test on an array: every element must be incremented by 1.
    char arr[] = {'a', 'b', 'c', 'd'};
    const std::size_t len = sizeof(arr) / sizeof(arr[0]);
    for (std::size_t i = 0; i < len; ++i) {
        advanceAndReturn(arr[i]);
    }
    assert(arr[0] == 'b');
    assert(arr[1] == 'c');
    assert(arr[2] == 'd');
    assert(arr[3] == 'e');

    // Test edge case: incrementing 'z' yields '{' in ASCII.
    char zed = 'z';
    advanceAndReturn(zed);
    assert(zed == '{');

    return 0;
}
