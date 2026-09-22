Write a C++ function that takes a non-empty vector of characters and reverses it in-place. The input vector represents a sequence of characters (which could be letters, digits, symbols, or spaces). The function must modify the original vector so that its elements are in reverse order, preserving all characters exactly as they appear (no case conversion, no removal of spaces). The function should handle vectors of any size from 1 upward, including vectors with duplicate characters. For example, `{'a','b','c'}` becomes `{'c','b','a'}`, and `{'x','y'}` becomes `{'y','x'}`. The function signature must use a reference to a `std::vector<char>` to allow in-place modification, and it must not return anything.
// The standard approach is to use a two-pointer technique. We place one pointer at the start (index `0`) and another at the end (index `size()-1`). While the start pointer is less than or equal to the end pointer, we swap the characters at those positions, then move the start pointer forward and the end pointer backward. This repeats until the pointers cross, which occurs after roughly half the size iterations. Edge cases: for a vector of size 1, the start pointer equals the end pointer, and swapping an element with itself is a no-op but harmless; the loop runs once and then exits. For an empty vector, the problem states non-empty, but if it occurred, `size()-1` would underflow; hence we only consider non-empty inputs. The loop condition uses `<=` to handle odd-length vectors where the middle element stays in place. Time complexity is O(n) because each element is swapped once (n/2 swaps). Auxiliary space is O(1) because we use only a few integer indices and a temporary variable for swapping.
#include <vector>
#include <utility> // for std::swap

// Reverse the contents of the given character vector in-place.
void reverseVector(std::vector<char>& s) {
    int left = 0;
    int right = static_cast<int>(s.size()) - 1;
    while (left <= right) {
        std::swap(s[left], s[right]);
        ++left;
        --right;
    }
}
#include <cassert>
#include <vector>

// Function declaration (assumed from solution)
void reverseVector(std::vector<char>& s);

int main() {
    // Test 1: Odd length
    std::vector<char> v1 = {'a', 'b', 'c'};
    reverseVector(v1);
    assert((v1 == std::vector<char>{'c', 'b', 'a'}));

    // Test 2: Even length
    std::vector<char> v2 = {'x', 'y'};
    reverseVector(v2);
    assert((v2 == std::vector<char>{'y', 'x'}));

    // Test 3: Single element
    std::vector<char> v3 = {'z'};
    reverseVector(v3);
    assert((v3 == std::vector<char>{'z'}));

    // Test 4: Duplicate characters
    std::vector<char> v4 = {'m', 'm', 'm'};
    reverseVector(v4);
    assert((v4 == std::vector<char>{'m', 'm', 'm'}));

    // Test 5: Already reversed (palindrome)
    std::vector<char> v5 = {'r', 'a', 'c', 'e', 'c', 'a', 'r'};
    reverseVector(v5);
    assert((v5 == std::vector<char>{'r', 'a', 'c', 'e', 'c', 'a', 'r'}));

    // Test 6: Larger set with spaces and symbols
    std::vector<char> v6 = {' ', '1', 'A', '@'};
    reverseVector(v6);
    assert((v6 == std::vector<char>{'@', 'A', '1', ' '}));

    // Test 7: Five elements with distinct characters
    std::vector<char> v7 = {'h', 'e', 'l', 'l', 'o'};
    reverseVector(v7);
    assert((v7 == std::vector<char>{'o', 'l', 'l', 'e', 'h'}));

    // Test 8: Two identical characters
    std::vector<char> v8 = {'p', 'p'};
    reverseVector(v8);
    assert((v8 == std::vector<char>{'p', 'p'}));
}
