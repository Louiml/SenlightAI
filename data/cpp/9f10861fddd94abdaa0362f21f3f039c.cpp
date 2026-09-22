// Write a C++ function named `sortCharactersInPlace` that takes a mutable string or character array by reference/pointer and sorts all its characters in ascending ASCII order. The function must handle both `std::string` inputs and C-style character arrays (including those with null terminators) and return the sorted result as a `std::string` (for array input, copy the sorted characters into a returned string). The input may contain spaces, digits, uppercase/lowercase letters, and punctuation; sorting must follow the natural ASCII ordering (e.g., space < digits < uppercase < lowercase). The function should not modify the original input if it is passed as `const std::string&`, but for char arrays it may modify the array in place and also return the sorted string. If the input is empty, return an empty string. Ensure no memory leaks and that null terminators in char arrays are handled correctly.

#include <cassert>
#include <string>
#include <cstring>

// Declaration of the tested functions (provided above in the solution).
std::string sortCharactersInPlace(std::string str);
std::string sortCharactersInPlace(char* arr);

int main() {
    // Test with std::string: lowercase and spaces
    assert(sortCharactersInPlace(std::string("bca")) == "abc");
    assert(sortCharactersInPlace(std::string("Life is beautiful")) == "  Laebefiiilstu");
    // Test with std::string: empty
    assert(sortCharactersInPlace(std::string("")) == "");
    // Test with std::string: single character
    assert(sortCharactersInPlace(std::string("Z")) == "Z");

    // Test with char array: normal
    char arr1[] = "programming";
    std::string result1 = sortCharactersInPlace(arr1);
    assert(result1 == "aggimmnoprr");
    assert(std::string(arr1) == "aggimmnoprr"); // original array is also sorted

    // Test with char array: containing digits and uppercase
    char arr2[] = "cBa32";
    std::string result2 = sortCharactersInPlace(arr2);
    assert(result2 == "23Bac");
    assert(std::string(arr2) == "23Bac");

    // Test with char array: empty string
    char arr3[] = "";
    std::string result3 = sortCharactersInPlace(arr3);
    assert(result3 == "");
    assert(std::string(arr3) == "");

    // Test with char array: null pointer
    char* nullPtr = nullptr;
    std::string result4 = sortCharactersInPlace(nullPtr);
    assert(result4 == "");

    return 0;
}

#include <string>
#include <algorithm>
#include <cstring>

// Sorts the characters of a std::string in ascending ASCII order and returns the sorted string.
std::string sortCharactersInPlace(std::string str) {
    std::sort(str.begin(), str.end());
    return str;
}

// Sorts a C-style character array in place (null-terminated) and returns the sorted string.
// The function also modifies the original array to contain the sorted characters.
std::string sortCharactersInPlace(char* arr) {
    if (!arr) return "";
    std::size_t len = std::strlen(arr);
    std::sort(arr, arr + len);
    return std::string(arr, len);
}

// The solution relies on the standard library `std::sort` with a range defined by iterators or pointers. For a `std::string`, we can directly sort `str.begin()` to `str.end()`. For a C-style char array, we compute its length using `strlen` (assuming it is null-terminated), then sort the range from the array's start to `start + length`. The original array can be modified in place, and we can also construct a `std::string` from the sorted characters to return. Key edge cases: empty string (sort handles fine, returns empty), single character (trivial), char array with no null terminator (undefined behavior — but the problem implies valid null-terminated arrays), and handling of spaces and punctuation (all sorted by ASCII value). The time complexity is O(n log n) due to `std::sort`, where n is the number of characters. Space complexity is O(1) auxiliary (excluding the input storage and the returned string copy), because `std::sort` is in-place. For char arrays, `strlen` runs in O(n).
