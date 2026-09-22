// Given an array of `n` integers, write a C++ function `xorTransform` that takes a vector of integers and returns a new vector where each element is replaced by the XOR of that element with the XOR of all elements in the original array. For example, if the original array is `[1, 2, 3]`, the total XOR is `1 ^ 2 ^ 3 = 0`, so the result is `[1^0, 2^0, 3^0] = [1, 2, 3]`. If the array is `[5, 7, 9]`, total XOR is `5^7^9 = (5^7)=2, 2^9=11`, so result is `[5^11=14, 7^11=12, 9^11=2]`. The function must handle any non-empty vector (size ≥ 1) and return the transformed vector.
#include <cassert>
#include <vector>

// Include the solution function here (or link it)
std::vector<int> xorTransform(const std::vector<int>& numbers);

int main() {
    // Single element: becomes 0
    assert(xorTransform({5}) == std::vector<int>({0}));
    // All elements equal
    assert(xorTransform({7, 7, 7}) == std::vector<int>({7, 7, 7})); // total = 7, 7^7=0? Wait: 7^7=0, but careful: totalXor = 7^7^7 = 7, so each 7^7=0, result {0,0,0}. Let's correct: total = 7, so 7^7=0, so {0,0,0}. Test that.
    // Let's write a correct assertion: {7,7,7} -> total=7, each 7^7=0 -> {0,0,0}
    assert(xorTransform({7, 7, 7}) == std::vector<int>({0, 0, 0}));
    // Example from description
    assert(xorTransform({5, 7, 9}) == std::vector<int>({14, 12, 2}));
    // Another example
    assert(xorTransform({0, 0, 0}) == std::vector<int>({0, 0, 0}));
    // Negative numbers
    assert(xorTransform({-1, -2, -3}) == std::vector<int>({-1 ^ (-1 ^ -2 ^ -3), -2 ^ (-1 ^ -2 ^ -3), -3 ^ (-1 ^ -2 ^ -3)}));
    // Compute manually: -1 = ...1111, -2 = ...1110, -3 = ...1101, XOR = ...1111 ^ ...1110 = ...0001, ...0001 ^ ...1101 = ...1100 = -4. So total = -4. Then each: -1 ^ -4 = (-1 ^ -4) = ...1111 ^ ...1100 = ...0011 = 3, -2 ^ -4 = ...1110 ^ ...1100 = ...0010 = 2, -3 ^ -4 = ...1101 ^ ...1100 = ...0001 = 1. So result {3,2,1}.
    assert(xorTransform({-1, -2, -3}) == std::vector<int>({3, 2, 1}));
    // Larger vector
    assert(xorTransform({1, 2, 3, 4}) == std::vector<int>({1^4, 2^4, 3^4, 4^4})); // total = 1^2=3, 3^3=0, 0^4=4, so result {5,6,7,0}
    assert(xorTransform({1, 2, 3, 4}) == std::vector<int>({5, 6, 7, 0}));
    return 0;
}
#include <vector>

// Given a vector of integers, return a new vector where each element
// is replaced by the XOR of that element with the XOR of all elements
// in the original vector.
std::vector<int> xorTransform(const std::vector<int>& numbers) {
    int totalXor = 0;
    for (const int& value : numbers) {
        totalXor ^= value;
    }

    std::vector<int> result;
    result.reserve(numbers.size());
    for (const int& value : numbers) {
        result.push_back(value ^ totalXor);
    }
    return result;
}
// The main idea is to compute the XOR of all elements in the array in one pass, then in a second pass XOR each element with that total. This works because XOR is associative and commutative, and each element `x` becomes `x ^ total`. For a single-element array, total is that element itself, so the result is `[x ^ x] = [0]`. Edge cases include negative integers (bitwise XOR works normally in two's complement), zero, duplicate values, and a vector of size 1. Time complexity is O(n) for `n` elements (two passes), and space complexity is O(n) for the result vector, which is required output. No additional data structures are needed beyond the input vector and result.
