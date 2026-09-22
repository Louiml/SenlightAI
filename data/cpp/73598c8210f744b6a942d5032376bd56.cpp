// Write a C++ function named `sortNamesAlphabetically` that takes an array of C-style strings (pointers to `char`) and its size, and sorts the array in ascending lexicographical order using the `std::sort` algorithm with a custom comparator. The function must perform the sorting in place and must not return a value. The comparator should use `strcmp` to compare two strings and return `true` if the first string is lexicographically less than the second. The function must be `const`-correct: it should not modify the strings themselves, only the array of pointers. Assume the input array may contain any number of names, including zero, and that the strings are null-terminated. The function should be reusable for any array of C-strings, not just a fixed-size list.

The main idea is to use the standard library algorithm `std::sort` on a range defined by the first element and the element just past the last (`names + size`). The custom comparator `alpha_comp` must be passed as the third argument to `std::sort`. The comparator takes two `const char*` parameters (or `char*` but `const` correctness suggests using `const char*` to avoid modifying the string literals, though in practice `std::sort` will pass pointers to `char*` elements, so the comparator must accept `const char*` or `char*`; using `const char*` is safer and compiles because `char*` converts to `const char*`). Inside the comparator, `strcmp(s1, s2) < 0` returns whether `s1` comes before `s2`. Edge cases include an empty array (size 0) and a single-element array, where `std::sort` does nothing. The algorithm runs in `O(n log n)` time on average due to `std::sort` (introsort) and uses `O(log n)` auxiliary stack space for recursion, not counting the input array itself. The strings themselves are not copied or modified; only the pointers in the array are rearranged.

#include <algorithm>
#include <cstring>

// Sorts an array of C-strings alphabetically in ascending order.
// The array 'names' is modified in place; the strings themselves are not changed.
void sortNamesAlphabetically(char* names[], size_t size) {
    std::sort(names, names + size, [](const char* a, const char* b) {
        return std::strcmp(a, b) < 0;
    });
}

#include <cassert>
#include <cstring>
#include <iostream>

// Assume the solution function is defined above.
void sortNamesAlphabetically(char* names[], size_t size);

int main() {
    // Test 1: General case
    char* names1[] = { (char*)"Serge", (char*)"Tata", (char*)"Elena",
                       (char*)"Dmitr", (char*)"Miha", (char*)"Vlad" };
    sortNamesAlphabetically(names1, 6);
    assert(std::strcmp(names1[0], "Dmitr") == 0);
    assert(std::strcmp(names1[1], "Elena") == 0);
    assert(std::strcmp(names1[2], "Miha") == 0);
    assert(std::strcmp(names1[3], "Serge") == 0);
    assert(std::strcmp(names1[4], "Tata") == 0);
    assert(std::strcmp(names1[5], "Vlad") == 0);

    // Test 2: Already sorted
    char* names2[] = { (char*)"a", (char*)"b", (char*)"c" };
    sortNamesAlphabetically(names2, 3);
    assert(std::strcmp(names2[0], "a") == 0);
    assert(std::strcmp(names2[1], "b") == 0);
    assert(std::strcmp(names2[2], "c") == 0);

    // Test 3: Reverse order
    char* names3[] = { (char*)"c", (char*)"b", (char*)"a" };
    sortNamesAlphabetically(names3, 3);
    assert(std::strcmp(names3[0], "a") == 0);
    assert(std::strcmp(names3[1], "b") == 0);
    assert(std::strcmp(names3[2], "c") == 0);

    // Test 4: Duplicate values
    char* names4[] = { (char*)"x", (char*)"x", (char*)"x" };
    sortNamesAlphabetically(names4, 3);
    assert(std::strcmp(names4[0], "x") == 0);
    assert(std::strcmp(names4[1], "x") == 0);
    assert(std::strcmp(names4[2], "x") == 0);

    // Test 5: Single element
    char* names5[] = { (char*)"single" };
    sortNamesAlphabetically(names5, 1);
    assert(std::strcmp(names5[0], "single") == 0);

    // Test 6: Empty array (size 0)
    char** names6 = nullptr;
    sortNamesAlphabetically(names6, 0); // Should not crash

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
