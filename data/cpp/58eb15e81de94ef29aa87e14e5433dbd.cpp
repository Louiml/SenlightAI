Write a C++ function named `reverseCharacters` that takes a reference to a `std::vector<char>` and reverses the order of the elements in place. The function must modify the original vector directly (not return a copy) and should handle vectors of any size, including empty ones. The reversal must be performed using a two-pointer (left/right) approach with manual swapping, without relying on the standard library’s `std::reverse`. The function should be `const`-correct in the sense that it does not modify any external state, and it must preserve the original vector’s capacity (no reallocations needed beyond what’s required). The task requires only the free function implementation; do not include `main` or any test harness in the solution.

#include <cassert>
#include <vector>

// Function declaration (implemented separately per task)
void reverseCharacters(std::vector<char>& s);

int main() {
    // Test 1: Reverses a typical string
    std::vector<char> v1 = {'h', 'e', 'l', 'l', 'o'};
    reverseCharacters(v1);
    assert(v1 == std::vector<char>({'o', 'l', 'l', 'e', 'h'}));

    // Test 2: Single element remains unchanged
    std::vector<char> v2 = {'x'};
    reverseCharacters(v2);
    assert(v2 == std::vector<char>({'x'}));

    // Test 3: Empty vector is safe
    std::vector<char> v3 = {};
    reverseCharacters(v3);
    assert(v3.empty());

    // Test 4: Even-length vector
    std::vector<char> v4 = {'a', 'b', 'c', 'd'};
    reverseCharacters(v4);
    assert(v4 == std::vector<char>({'d', 'c', 'b', 'a'}));

    // Test 5: Vector with repeated characters
    std::vector<char> v5 = {'a', 'a', 'b', 'b'};
    reverseCharacters(v5);
    assert(v5 == std::vector<char>({'b', 'b', 'a', 'a'}));

    // Test 6: Larger vector
    std::vector<char> v6 = {'1', '2', '3', '4', '5', '6'};
    reverseCharacters(v6);
    assert(v6 == std::vector<char>({'6', '5', '4', '3', '2', '1'}));

    // Test 7: Two-element vector
    std::vector<char> v7 = {'p', 'q'};
    reverseCharacters(v7);
    assert(v7 == std::vector<char>({'q', 'p'}));

    // Test 8: All same characters
    std::vector<char> v8 = {'z', 'z', 'z'};
    reverseCharacters(v8);
    assert(v8 == std::vector<char>({'z', 'z', 'z'}));

    return 0;
}

#include <vector>

// Reverse the order of elements in the given vector of characters in place.
void reverseCharacters(std::vector<char>& s) {
    int left = 0;
    int right = static_cast<int>(s.size()) - 1;

    while (left < right) {
        // Swap characters at left and right indices
        char temp = s[left];
        s[left] = s[right];
        s[right] = temp;

        ++left;
        --right;
    }
}

// The approach uses two indices: `left` starting at index 0 and `right` starting at index `s.size() - 1`. While `left < right`, the characters at these positions are swapped using a temporary `char` variable (or `std::swap` if desired, but manual swap is fine). Then `left` increments by 1 and `right` decrements by 1. The loop stops when `left` meets or crosses `right`, which correctly reverses all elements. Edge cases: an empty vector (size 0) or a single-element vector (size 1) cause the loop condition to be false immediately, so no swaps occur—the function does nothing, which is correct. For an even-sized vector, all pairs are swapped; for an odd-sized vector, the middle element stays in place. Time complexity is O(n/2) = O(n), where n is the size of the vector, because each element is moved at most once. Space complexity is O(1) auxiliary, as only a temporary variable for swapping is used; swapping in place modifies the input vector directly with no extra storage.
