// Write a C++ function `std::vector<int> subsetByNumber(int n, int64_t k)` that, given the size `n` of a subset (a vector of `n` bits, each either 0 or 1, representing presence of the corresponding element), and a non‑negative integer `k` between `0` and `2^n - 1` inclusive, returns the lexicographically ordered (in terms of binary reflective Gray‑code? No, actually this task is about standard binary order) binary representation of `k` as a vector of length `n` with the most significant bit first (i.e., the leftmost element corresponds to the highest power of 2). The representation should be exactly the binary expansion of `k` padded on the left with zeros to length `n`. If `k` is negative or `k > 2^n - 1` (with the special case `n == 63` where the total number of subsets is `INT64_MAX` and any non‑negative `k` is allowed), return an empty vector. The function must handle `n == 0` correctly: for `n == 0`, only `k == 0` is valid, and the returned vector is empty (length zero). This problem is inspired by the `object_by_number` method of a subset combinatorics class: it reverses the mapping from a subset object (a bit vector) to its ordinal number.

The solution interprets `k` as a binary number. The standard way to convert an integer to its binary representation is to repeatedly take `k % 2` and push that bit, then divide `k` by 2. This yields bits from least significant to most significant. After processing all bits, we have a vector of length equal to the number of bits in `k`. To obtain the final vector of length `n` with most significant bit first, we need to pad with zeros on the *left* (i.e., prepend zeros) until the vector size reaches `n`, and then reverse the order to get most‑significant‑first. Edge cases:  
- If `k == 0`, the loop produces an empty vector; we then pad with `n` zeros and reverse (reversing an empty or all‑zero vector is fine).  
- If `n == 0`, the only valid `k` is `0`. For `k == 0`, the loop yields an empty vector; we pad with `0` zeros (so it remains empty) and reversing an empty vector yields empty.  
- If `n >= 63`, the maximum number of subsets is `INT64_MAX` (which is `2^63 - 1`). For `n == 63`, `2^63` overflows `int64_t`, so the condition must check: if `n < 63` and `k > (1LL << n) - 1`, invalid; if `n >= 63`, any `k >= 0` is valid because the maximum representable `k` is `INT64_MAX`, and that equals `2^63 - 1`; but `1LL << 63` is undefined behavior, so we guard. The number of bits needed for `k` is at most 63, so the vector length before padding is at most 63, and after padding to `n` (if `n` is huge, but in practice `n` is bounded by 63 for valid input, because `n` larger than 63 would exceed `INT64_MAX` capacity, but the function doesn't restrict `n`, it just pads: for very large `n`, we would produce a vector of length `n` with many leading zeros, which is allowed). Time complexity is O(number of bits of `k` + `n`) = O(n) for typical `n ≤ 63`, and space is O(n). Edge case: if `k` is negative or out of range, return empty vector.

#include <cstdint>
#include <vector>
#include <algorithm>

// Return binary representation of k as a vector of length n, most significant bit first.
// Return empty vector if k is invalid for the given n.
std::vector<int> subsetByNumber(int n, int64_t k) {
    // Validate k range
    if (k < 0) return {};
    if (n < 63 && k > ((1LL << n) - 1)) return {};
    // For n >= 63, any k >= 0 is valid (since k is int64_t, max is INT64_MAX)
    
    // Handle n == 0: only k == 0 is valid, and output is empty vector
    if (n == 0) {
        return (k == 0) ? std::vector<int>() : std::vector<int>();
    }
    
    // Build binary representation from least significant bit
    std::vector<int> bits;
    int64_t value = k;
    while (value > 0) {
        bits.push_back(static_cast<int>(value % 2));
        value /= 2;
    }
    // If k == 0, bits is empty, which is fine
    
    // Pad with zeros on the left (most significant side) to reach length n
    int pad = n - static_cast<int>(bits.size());
    for (int i = 0; i < pad; ++i) {
        bits.push_back(0);
    }
    
    // Reverse to get most significant first
    std::reverse(bits.begin(), bits.end());
    
    return bits;
}

#include <cassert>
#include <vector>
#include <cstdint>

// Assume subsetByNumber is defined as above

int main() {
    // Basic cases
    assert(subsetByNumber(3, 0) == std::vector<int>({0,0,0}));
    assert(subsetByNumber(3, 1) == std::vector<int>({0,0,1}));
    assert(subsetByNumber(3, 2) == std::vector<int>({0,1,0}));
    assert(subsetByNumber(3, 3) == std::vector<int>({0,1,1}));
    assert(subsetByNumber(3, 4) == std::vector<int>({1,0,0}));
    assert(subsetByNumber(3, 5) == std::vector<int>({1,0,1}));
    assert(subsetByNumber(3, 6) == std::vector<int>({1,1,0}));
    assert(subsetByNumber(3, 7) == std::vector<int>({1,1,1}));
    
    // Edge case n=0
    assert(subsetByNumber(0, 0).empty());
    assert(subsetByNumber(0, 1).empty());  // invalid because k>0 for n=0
    
    // Negative k
    assert(subsetByNumber(5, -1).empty());
    
    // Out of range
    assert(subsetByNumber(5, 32).empty()); // 2^5 = 32, max k=31
    assert(subsetByNumber(5, 31) == std::vector<int>({1,1,1,1,1}));
    
    // n=63, large k
    assert(subsetByNumber(63, INT64_MAX).size() == 63);
    assert(subsetByNumber(63, INT64_MAX).front() == 1);  // MSB is 1
    assert(subsetByNumber(63, 0).front() == 0);
    
    // Padding with leading zeros for small k
    assert(subsetByNumber(6, 1) == std::vector<int>({0,0,0,0,0,1}));
    assert(subsetByNumber(10, 1023) == std::vector<int>({0,1,1,1,1,1,1,1,1,1}));
    assert(subsetByNumber(10, 1024).empty()); // 2^10=1024, max k=1023
    
    return 0;
}
