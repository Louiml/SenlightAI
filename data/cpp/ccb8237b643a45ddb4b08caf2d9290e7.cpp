// Write a C++ function named `reversedListWithoutCrash` that takes a `std::list<char>` as input and returns a new `std::list<char>` containing the characters in reverse order of the input. The function must avoid undefined behavior by not dereferencing invalid iterators. Specifically, it must not access or print elements beyond the list’s bounds. The returned list should preserve the original characters exactly, just reversed. The function should work for empty lists, single-element lists, and lists of any size. It must not modify the input list, and it should allocate only the necessary memory for the output. The function signature is: `std::list<char> reversedListWithoutCrash(const std::list<char>& input);`

#include <cassert>
#include <list>

// Declaration of the function under test.
std::list<char> reversedListWithoutCrash(const std::list<char>& input);

int main() {
    // Test empty list.
    std::list<char> empty;
    assert(reversedListWithoutCrash(empty).empty());

    // Test single element.
    std::list<char> single = {'a'};
    assert(reversedListWithoutCrash(single) == std::list<char>({'a'}));

    // Test normal list.
    std::list<char> input = {'a', 'b', 'c', 'd'};
    std::list<char> expected = {'d', 'c', 'b', 'a'};
    assert(reversedListWithoutCrash(input) == expected);

    // Test with repeated characters.
    std::list<char> repeats = {'x', 'y', 'x', 'z'};
    std::list<char> expected_repeats = {'z', 'x', 'y', 'x'};
    assert(reversedListWithoutCrash(repeats) == expected_repeats);

    // Test that input is not modified.
    std::list<char> original = input;
    reversedListWithoutCrash(input);
    assert(input == original);

    // Test large list (e.g., 1000 elements).
    std::list<char> large;
    for (int i = 0; i < 1000; ++i) {
        large.push_back(static_cast<char>('a' + (i % 26)));
    }
    std::list<char> reversed_large = reversedListWithoutCrash(large);
    // Verify first and last elements are swapped.
    assert(reversed_large.front() == large.back());
    assert(reversed_large.back() == large.front());
    // Verify size.
    assert(reversed_large.size() == large.size());

    return 0;
}

#include <list>
#include <cstddef>

// Returns a new list containing the characters of 'input' in reverse order.
// Does not modify 'input'. Handles empty lists gracefully.
std::list<char> reversedListWithoutCrash(const std::list<char>& input) {
    std::list<char> result;
    // Iterate from the beginning to the end of the input list.
    for (std::list<char>::const_iterator it = input.begin(); it != input.end(); ++it) {
        // Insert each character at the front of the result list to reverse order.
        result.push_front(*it);
    }
    return result;
}

// The task is to reverse a `std::list<char>` safely. The key challenge is that iterators in lists are not random-access, and incrementing past `end()` is undefined behavior. A straightforward approach is to iterate from the front of the input list and insert each character at the front of the output list. Since inserting at the front of a `std::list` is O(1), this builds the reversed list in a single pass. For an empty input, the function simply returns an empty list. For a single element, it returns a list with that same element. Edge cases include an input with repeated characters—these are handled naturally, as they are copied in order. We must not use `p++` on an iterator that might be `end()`, nor dereference `end()`. The algorithm uses `O(n)` time (one traversal) and `O(n)` auxiliary space for the output list (since each character is copied). The input list is not modified, and we use `const` reference to avoid copies. For efficiency and safety, we use a `const_iterator` for traversal. Alternatively, we could use `std::reverse` on a copy, but that requires copying first and then reversing in place, which is also O(n) time and O(n) space. The chosen approach directly builds the reversed list.
