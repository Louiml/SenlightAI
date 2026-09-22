Write a C++ function named `reverseCharacterVector` that takes a `std::vector<char>` by reference and reverses the order of its elements in place. The function must not use any standard library algorithm functions such as `std::reverse`, and must operate directly on the vector using indexing and swapping. The vector may be empty, contain a single character, or contain an even or odd number of elements. For example, given `{'a','b','c','d'}`, after the call the vector should become `{'d','c','b','a'}`. The function should return `void` and handle all cases without assumptions about the vector’s size.

The approach is straightforward: use two indices—one starting at the beginning of the vector and the other at the end—and swap the characters at these positions while the start index is strictly less than the end index. Continue moving the start index forward and the end index backward until they meet or cross. For a vector of size `n`, the number of swaps is `n/2` (integer division). Edge cases: an empty vector or a vector with one element requires no swaps, which is naturally handled because the loop condition fails immediately. The algorithm runs in O(n) time because each element is accessed a constant number of times, and it uses O(1) auxiliary space since no extra data structures are created. The function modifies the vector in place, so no copy is made.

#include <vector>

// Reverses the order of elements in the given vector in place.
// The function swaps elements symmetrically from the ends toward the center.
void reverseCharacterVector(std::vector<char>& s) {
    int left = 0;
    int right = static_cast<int>(s.size()) - 1;

    while (left < right) {
        char temp = s[left];        // swap using a manual temporary variable
        s[left] = s[right];
        s[right] = temp;

        ++left;
        --right;
    }
}

#include <cassert>
#include <vector>

int main() {
    {
        std::vector<char> s = {'a', 'b', 'c', 'd'};
        reverseCharacterVector(s);
        assert(s == std::vector<char>({'d', 'c', 'b', 'a'}));
    }
    {
        std::vector<char> s = {'x'};
        reverseCharacterVector(s);
        assert(s == std::vector<char>({'x'}));
    }
    {
        std::vector<char> s = {};
        reverseCharacterVector(s);
        assert(s.empty());
    }
    {
        std::vector<char> s = {'h', 'e', 'l', 'l', 'o'};
        reverseCharacterVector(s);
        assert(s == std::vector<char>({'o', 'l', 'l', 'e', 'h'}));
    }
    {
        std::vector<char> s = {'1', '2', '3', '4', '5', '6'};
        reverseCharacterVector(s);
        assert(s == std::vector<char>({'6', '5', '4', '3', '2', '1'}));
    }
    {
        std::vector<char> s = {'a', 'b', 'c', 'c', 'b', 'a'};
        reverseCharacterVector(s);
        assert(s == std::vector<char>({'a', 'b', 'c', 'c', 'b', 'a'}));  // palindrome
    }
    return 0;
}
