Write a C++ function that takes a vector of integers and rearranges it in-place so that all odd numbers appear before all even numbers, preserving the original relative order of elements within each parity group as much as possible. The function should return a copy of the rearranged vector. The input vector may contain negative numbers, zeros, duplicates, and any size from 1 to 10^5. The function must run in O(n) time and use O(n) auxiliary space.
The simplest approach is to use a stability-preserving partition: create two result vectors, one for odd numbers and one for even numbers, iterate through the input once, appending each element to the appropriate vector based on `x % 2 != 0`. Since `x % 2` is nonzero for negative odd numbers in C++, this correctly handles negative values (e.g., -3 % 2 == -1, which is nonzero). Zero is even, so `0 % 2 == 0` correctly places it in the even group. After the single pass, concatenate the odd vector and the even vector, preserving relative order within each group. This is O(n) time and O(n) auxiliary space. Edge cases include: all odd, all even, single element, duplicates, negative values, and zeros. The modulo operation with negative numbers requires care—using `x % 2 != 0` (not `x % 2 == 1`) is correct.
#include <vector>
#include <cstdint>

// Rearrange the vector so all odd numbers come before even numbers,
// preserving relative order within each parity group. Returns a new vector.
std::vector<int> oddsBeforeEvens(const std::vector<int>& input) {
    std::vector<int> odds;
    odds.reserve(input.size() / 2 + 1);
    std::vector<int> evens;
    evens.reserve(input.size() / 2 + 1);
    
    for (const int x : input) {
        if (x % 2 != 0) {
            odds.push_back(x);
        } else {
            evens.push_back(x);
        }
    }
    
    odds.insert(odds.end(), evens.begin(), evens.end());
    return odds;
}
#include <cassert>
#include <vector>

// Declare the function (in real usage, include the header or definition above)
std::vector<int> oddsBeforeEvens(const std::vector<int>& input);

int main() {
    // Basic mixed case with positive numbers
    assert((oddsBeforeEvens({1, 2, 3, 4, 5}) == std::vector<int>{1, 3, 5, 2, 4}));
    
    // All odds
    assert((oddsBeforeEvens({3, 5, 7}) == std::vector<int>{3, 5, 7}));
    
    // All evens
    assert((oddsBeforeEvens({2, 4, 6}) == std::vector<int>{2, 4, 6}));
    
    // Negative odd numbers
    assert((oddsBeforeEvens({-3, 2, -1, 4}) == std::vector<int>{-3, -1, 2, 4}));
    
    // Zero is even
    assert((oddsBeforeEvens({0, 1, 2}) == std::vector<int>{1, 0, 2}));
    
    // Duplicate values
    assert((oddsBeforeEvens({2, 1, 2, 1}) == std::vector<int>{1, 1, 2, 2}));
    
    // Single element
    assert((oddsBeforeEvens({42}) == std::vector<int>{42}));
    assert((oddsBeforeEvens({43}) == std::vector<int>{43}));
    
    // Large test: alternating 1 and 2 for 10 elements
    std::vector<int> large;
    for (int i = 0; i < 10; ++i) {
        large.push_back(i % 2 == 0 ? 1 : 2);
    }
    std::vector<int> expected;
    for (int i = 0; i < 5; ++i) expected.push_back(1);
    for (int i = 0; i < 5; ++i) expected.push_back(2);
    assert((oddsBeforeEvens(large) == expected));
    
    return 0;
}
