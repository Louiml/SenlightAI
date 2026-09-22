// Write a C++ function named `reverseIntegerSequence` that takes a non-empty sequence of integers (provided one per line via standard input, with the first input line being the count `n`) and returns a `std::vector<int>` containing the integers in reverse order of their appearance. The function must read exactly `n` integers from standard input, where `n` is given first. Handle the edge case where `n` is 0 by returning an empty vector. Do not print anything to standard output; instead, return the reversed vector so the caller can process it further.

// The task requires reading a count `n` from standard input, then reading `n` integer values. Since the input is read from a stream (e.g., `std::cin`), we cannot know the size in advance without reading the first value. The main algorithm is straightforward: read `n`, if `n` is 0, return an empty vector; otherwise, allocate a vector of size `n`, read each integer into it sequentially, then reverse the vector using `std::reverse` or by manually swapping elements from both ends. Edge cases include `n=0` (return empty), negative `n` (should be treated as invalid input; we can handle by returning empty or ignoring, but per common practice, we can assume `n` is non-negative, but defensive coding could reject negative). Time complexity is O(n) for reading and O(n) for reversing, giving overall O(n). Space complexity is O(n) for storing the vector. The function should not read extra whitespace beyond the integers; standard `>>` extraction automatically skips whitespace.

#include <vector>
#include <iostream>
#include <algorithm>

// Reads an integer count n from standard input, then reads n integers,
// and returns a vector containing those integers in reverse order.
std::vector<int> reverseIntegerSequence() {
    int n;
    std::cin >> n;
    if (n <= 0) {
        return {};
    }

    std::vector<int> values(n);
    for (int i = 0; i < n; ++i) {
        std::cin >> values[i];
    }

    std::reverse(values.begin(), values.end());
    return values;
}

#include <cassert>
#include <iostream>
#include <sstream>

// The solution function is already included from the previous section.
// To test directly, we need to redirect std::cin to a stringstream.

int main() {
    // Test 1: Normal reverse
    {
        std::istringstream input("5 1 2 3 4 5");
        std::cin.rdbuf(input.rdbuf());
        std::vector<int> result = reverseIntegerSequence();
        assert((result == std::vector<int>{5, 4, 3, 2, 1}));
    }
    // Test 2: Single element
    {
        std::istringstream input("1 42");
        std::cin.rdbuf(input.rdbuf());
        std::vector<int> result = reverseIntegerSequence();
        assert((result == std::vector<int>{42}));
    }
    // Test 3: Zero elements
    {
        std::istringstream input("0");
        std::cin.rdbuf(input.rdbuf());
        std::vector<int> result = reverseIntegerSequence();
        assert(result.empty());
    }
    // Test 4: Negative numbers and duplicates
    {
        std::istringstream input("6 -3 7 -3 0 9 -3");
        std::cin.rdbuf(input.rdbuf());
        std::vector<int> result = reverseIntegerSequence();
        assert((result == std::vector<int>{-3, 9, 0, -3, 7, -3}));
    }
    // Test 5: All negative numbers
    {
        std::istringstream input("3 -1 -2 -3");
        std::cin.rdbuf(input.rdbuf());
        std::vector<int> result = reverseIntegerSequence();
        assert((result == std::vector<int>{-3, -2, -1}));
    }
    // Test 6: Large sequence (10 elements)
    {
        std::istringstream input("10 10 9 8 7 6 5 4 3 2 1");
        std::cin.rdbuf(input.rdbuf());
        std::vector<int> result = reverseIntegerSequence();
        assert((result == std::vector<int>{1, 2, 3, 4, 5, 6, 7, 8, 9, 10}));
    }
    // Test 7: Reversing already reversed sequence
    {
        std::istringstream input("3 3 2 1");
        std::cin.rdbuf(input.rdbuf());
        std::vector<int> result = reverseIntegerSequence();
        assert((result == std::vector<int>{1, 2, 3}));
    }
    // Test 8: Negative count (should return empty)
    {
        std::istringstream input("-5");
        std::cin.rdbuf(input.rdbuf());
        std::vector<int> result = reverseIntegerSequence();
        assert(result.empty());
    }

    return 0;
}
