// Write a C++ function that takes an integer `k` and a vector of non-negative integers, and returns a boolean indicating whether the product of all elements in the vector is divisible by `k`. The product may be extremely large, so it must not be computed directly. Instead, determine divisibility by performing modular multiplication using unsigned 64-bit integers. The function should handle the case where the vector is empty (product of an empty set is 1), and also handle the case where `k` is 1 (every product is divisible by 1). The function signature should be: `bool isProductDivisible(const std::vector<long long>& numbers, long long k)`.

#include <cassert>
#include <vector>

// Function declaration (already defined above)
bool isProductDivisible(const std::vector<long long>&, long long);

int main() {
    // Test 1: Simple case where product is divisible
    assert(isProductDivisible({2, 3, 4}, 12) == true);
    // Test 2: Product not divisible
    assert(isProductDivisible({2, 3, 4}, 5) == false);
    // Test 3: k = 1, always divisible
    assert(isProductDivisible({7, 11, 13}, 1) == true);
    // Test 4: Empty vector, product = 1, divisible only if k=1
    assert(isProductDivisible({}, 1) == true);
    assert(isProductDivisible({}, 5) == false);
    // Test 5: Large numbers, product modulo works correctly
    assert(isProductDivisible({1000000000LL, 1000000000LL}, 1000000000LL) == true);
    // Test 6: One number equals k
    assert(isProductDivisible({10}, 10) == true);
    // Test 7: One number less than k
    assert(isProductDivisible({9}, 10) == false);
    // Test 8: Zero in the list makes product zero (divisible by any positive k)
    assert(isProductDivisible({0, 5, 7}, 3) == true);
    // Test 9: Multiple numbers producing remainder 1
    assert(isProductDivisible({7, 7, 7}, 13) == false);
    // Test 10: Large k to check overflow handling
    assert(isProductDivisible({123456789LL, 987654321LL}, 1000000007LL) == false);
    return 0;
}

#include <vector>
#include <cstdint>

// Returns true if the product of all numbers in 'numbers' is divisible by 'k'.
// The product may be huge, so we compute modulo 'k' step by step.
bool isProductDivisible(const std::vector<long long>& numbers, long long k) {
    // k must be positive
    if (k <= 0) return false; // invalid input, but treat as false

    // Initially, the product of an empty set is 1.
    // 1 % k = 1 if k > 1, and 0 if k == 1.
    unsigned long long remainder = 1ULL % static_cast<unsigned long long>(k);

    for (const long long num : numbers) {
        // Since numbers are non-negative, cast to unsigned long long safely.
        unsigned long long term = static_cast<unsigned long long>(num) % static_cast<unsigned long long>(k);
        // Use __int128 to avoid overflow in multiplication.
        __int128 product = static_cast<__int128>(remainder) * term;
        remainder = static_cast<unsigned long long>(product % static_cast<unsigned long long>(k));
    }

    return remainder == 0;
}

// The key idea is to compute the product modulo `k` without overflowing. Since the product can be huge, we reduce each multiplication modulo `k` immediately. We initialize `remainder = 1 % k`. For each number `a` in the vector, we compute `remainder = (remainder * (a % k)) % k`. Because `a` is non-negative and `k` is positive, `a % k` is safe. After processing all numbers, if `remainder == 0`, then the product is divisible by `k`, otherwise not. Edge cases: if `k == 1`, then `1 % 1 == 0` and the result is always true (since any product is divisible by 1). If the vector is empty, the product is 1, so the result is true only when `k == 1` (since `1 % 1 == 0`). To avoid overflow in `remainder * (a % k)`, we use `unsigned long long` for the multiplication (since the product of two numbers each less than `k` could be up to `(k-1)^2`, which for `k` up to about 4e9 fits in 64-bit unsigned, but for larger `k` we must use `__int128` or careful mod multiplication; but for typical constraints, using `unsigned long long` and assuming `k` fits in 64-bit signed, the intermediate product could overflow if `k` is large. To be safe, we can use `__int128` for the multiplication, which is supported by GCC and Clang). Time complexity is O(n) where n is the size of the vector. Space complexity is O(1) auxiliary.
