Write a C++ function named `swapWithoutTemp` that takes two integer references and swaps their values without using a temporary variable, using only arithmetic operations (addition and subtraction). The function must correctly handle negative numbers, zero, and large values that do not cause integer overflow. Additionally, write a separate traversal function named `swapAllPairs` that takes a `std::vector<int>&` and swaps every adjacent pair of elements: indices (0,1), (2,3), etc., using `swapWithoutTemp` internally. If the vector has an odd number of elements, the last element remains unchanged. The traversal function must return `void` and modify the vector in place. Ensure both functions are `const`-correct for any read-only parameters.

// The core idea is to swap two integers without a temporary by using addition and subtraction. The classic swap is: `a = a - b; b = a + b; a = b - a;`. However, this works only if no overflow occurs. Since the problem restricts to cases that do not cause overflow, we can assume safe arithmetic for the given test vectors (values within reasonable integer range). Edge cases: if both values are zero, or one is zero, the arithmetic works fine. If the two values are very large and of opposite signs, no overflow occurs because the intermediate `a - b` might exceed bounds? Actually, to be safe, we could use `a = a + b; b = a - b; a = a - b;` but that has similar overflow risks. We'll stick with the given snippet's version but note the assumption. The traversal loops from index 0 to `size()-2` with step 2, calling `swapWithoutTemp` for each pair. Time complexity is O(n) for the traversal, and O(1) per swap. Space complexity is O(1) auxiliary.

#include <vector>

// Swap two integers without using a temporary variable, using arithmetic operations.
// Assumes no integer overflow occurs during the arithmetic.
void swapWithoutTemp(int& a, int& b) {
    a = a - b;
    b = a + b;
    a = b - a;
}

// Swap every adjacent pair of elements in the vector.
// For odd-length vectors, the last element remains unchanged.
void swapAllPairs(std::vector<int>& values) {
    const std::size_t size = values.size();
    for (std::size_t i = 0; i + 1 < size; i += 2) {
        swapWithoutTemp(values[i], values[i + 1]);
    }
}

#include <cassert>
#include <vector>

// The solution functions are assumed to be defined above.
// Test cases for swapWithoutTemp and swapAllPairs.
int main() {
    // Test swapWithoutTemp directly
    int a = 5, b = 3;
    swapWithoutTemp(a, b);
    assert(a == 3 && b == 5);

    int c = -7, d = 2;
    swapWithoutTemp(c, d);
    assert(c == 2 && d == -7);

    int e = 0, f = 0;
    swapWithoutTemp(e, f);
    assert(e == 0 && f == 0);

    int g = 100, h = -100;
    swapWithoutTemp(g, h);
    assert(g == -100 && h == 100);

    // Test swapAllPairs
    std::vector<int> v1 = {1, 2, 3, 4};
    swapAllPairs(v1);
    assert((v1 == std::vector<int>{2, 1, 4, 3}));

    std::vector<int> v2 = {5, 6, 7};
    swapAllPairs(v2);
    assert((v2 == std::vector<int>{6, 5, 7}));

    std::vector<int> v3 = {10};
    swapAllPairs(v3);
    assert((v3 == std::vector<int>{10}));

    std::vector<int> v4 = {};
    swapAllPairs(v4);
    assert(v4.empty());

    std::vector<int> v5 = {-1, 0, 1, 2, 3};
    swapAllPairs(v5);
    assert((v5 == std::vector<int>{0, -1, 2, 1, 3}));

    std::vector<int> v6 = {42, -42};
    swapAllPairs(v6);
    assert((v6 == std::vector<int>{-42, 42}));

    return 0;
}
