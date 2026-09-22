// Write a C++ function named `charArrayToString` that takes a C-style character array (i.e., a pointer to `char`) and its length `n` as parameters, and returns a `std::string` containing exactly the first `n` characters from the array, in the same order. The array may contain any printable ASCII characters, including spaces, digits, and punctuation, but no null terminators are assumed. The function should work correctly for `n = 0` (returning an empty string) and for arrays with repeated or special characters. You must not use any standard library function that converts a character array directly to a string (e.g., `std::string(arr)`), but you may use `std::string` member functions. The function should be `const`-correct where appropriate and not modify the input array.

#include <cassert>
#include <string>

// Declare the function (since it is defined in the solution above)
std::string charArrayToString(const char* arr, int n);

int main() {
    // Basic case with letters
    char arr1[] = {'h', 'e', 'l', 'l', 'o'};
    assert(charArrayToString(arr1, 5) == "hello");

    // Zero length returns empty string
    assert(charArrayToString(arr1, 0) == "");

    // Spaces and punctuation
    char arr2[] = {'a', ' ', 'b', '!', 'c'};
    assert(charArrayToString(arr2, 5) == "a b!c");

    // Partial reading (only first 3 of 5)
    assert(charArrayToString(arr1, 3) == "hel");

    // Repeated characters
    char arr3[] = {'X', 'X', 'X'};
    assert(charArrayToString(arr3, 3) == "XXX");

    // Digits and special symbols
    char arr4[] = {'1', '2', '#', '9'};
    assert(charArrayToString(arr4, 4) == "12#9");

    // Large n but no null terminator involved
    char arr5[] = {'z', 'y', 'x', 'w', 'v'};
    assert(charArrayToString(arr5, 5) == "zyxwv");

    // Non-printable? Not tested here, but handling is identical.

    return 0;
}

#include <string>

// Convert the first n characters of a C-style array into a std::string.
// The input array is not modified and may contain any characters, including spaces.
// n must be non-negative and not exceed the actual array length.
std::string charArrayToString(const char* arr, int n) {
    std::string result;
    result.reserve(n > 0 ? static_cast<size_t>(n) : 0); // optional: preallocate for efficiency
    for (int i = 0; i < n; ++i) {
        result += arr[i];
    }
    return result;
}

// The solution is straightforward: iterate over the first `n` elements of the character array and append each character to a `std::string` using the `+=` operator. Since we are not assuming a null terminator, we must use the provided length `n` to know how many characters to process. Edge cases include `n = 0` (loop does nothing, returns empty string), and large `n` where the array might contain spaces or non-alphanumeric characters—these are handled correctly because we treat each element as a `char`. The time complexity is O(n) because we visit each character once. The space complexity is O(n) for the returned string, plus O(1) auxiliary space for the loop variable and temporary storage. No modifications are made to the input, so we can declare the array parameter as `const char*`.
