// Write a standalone C++ function that, given three integer arguments `n`, `m`, and `p` along with two vectors of integers `a` (length `n`) and `b` (length `m`), returns the sum of the indices of the first elements in each vector that are not divisible by `p`. Specifically, find the smallest index `i` (0-based) such that `a[i] % p != 0`, and the smallest index `j` such that `b[j] % p != 0`. If all elements in a vector are divisible by `p`, treat its first non-divisible index as the vector’s length. Return `i + j`. The function must be named `firstNonDivisibleSum` and accept parameters in the order `(const std::vector<long long>& a, const std::vector<long long>& b, long long p)`. It must be `const`-correct, handle empty vectors, and work for negative numbers (where `%` in C++ may be negative, so check `value % p != 0` regardless of sign). The solution should be efficient and avoid unnecessary copies.

// The core idea is to scan each vector from the beginning and stop at the first element that is not divisible by `p`. A number is divisible by `p` if and only if `value % p == 0` (this works for negative values in C++: e.g., `-6 % 3 == 0`, `-5 % 3 == -2 ≠ 0`). If no such element exists, the "first non-divisible index" is the size of the vector. We sum the two indices. Edge cases: empty vector → returns `0 + otherIndex`; both empty → returns `0`; all elements divisible by `p` → returns `n + m`. The algorithm runs in \(O(n + m)\) time in the worst case (if every element is divisible) and \(O(1)\) auxiliary space, aside from the input vectors. We must ensure we use `long long` to avoid overflow if indices are large, though in practice sizes are `size_t`. Note the original code reads `n,m,p` as `long long` but then uses them as array sizes, which is fine; our function uses standard vectors.

#include <vector>

// Returns the sum of the first indices in a and b where elements are not divisible by p.
// If all elements in a vector are divisible, returns that vector's size as its index.
long long firstNonDivisibleSum(const std::vector<long long>& a, const std::vector<long long>& b, long long p) {
    long long indexA = static_cast<long long>(a.size());
    for (std::size_t i = 0; i < a.size(); ++i) {
        if (a[i] % p != 0) {
            indexA = static_cast<long long>(i);
            break;
        }
    }

    long long indexB = static_cast<long long>(b.size());
    for (std::size_t i = 0; i < b.size(); ++i) {
        if (b[i] % p != 0) {
            indexB = static_cast<long long>(i);
            break;
        }
    }

    return indexA + indexB;
}

#include <cassert>
#include <vector>

// Declaration (assumed from solution)
long long firstNonDivisibleSum(const std::vector<long long>& a, const std::vector<long long>& b, long long p);

int main() {
    // Basic case: first non-divisible at index 2 and 1 → sum 3
    assert(firstNonDivisibleSum({2,4,3,8}, {6,5,10}, 2) == 3);
    // Negative numbers: -3 is divisible by 3, -5 is not → index 2 in second vector? Let's check
    // a = {-6,-9,-15} all divisible by 3 → index = 3; b = {-3, -5, 7} → first non-divisible at index 1 → sum = 4
    assert(firstNonDivisibleSum({-6,-9,-15}, {-3,-5,7}, 3) == 4);
    // All divisible in both → sum of sizes: 3+4=7
    assert(firstNonDivisibleSum({4,8,12}, {10,20,30,40}, 2) == 7);
    // Empty first, non-empty second → first index 0, second index 0 → sum 1? Wait empty a size 0, b first at 0 → 0+0=0? No b[0] not divisible → index 0 → sum=0
    assert(firstNonDivisibleSum({}, {1,2}, 2) == 0);
    // Empty both → sum 0
    assert(firstNonDivisibleSum({}, {}, 5) == 0);
    // First non-divisible at 0 in both → sum 0
    assert(firstNonDivisibleSum({1,2},{3,4},5) == 0);
    // Larger test: a size 5 all divisible, b size 3 all divisible → 8
    assert(firstNonDivisibleSum({6,12,18,24,30}, {9,15,21}, 3) == 8);
    return 0;
}
