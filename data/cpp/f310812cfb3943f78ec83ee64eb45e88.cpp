Write a C++ function `int countQuadruples(const std::vector<int>& nums)` that takes a vector of distinct positive integers and returns the number of quadruples `(a, b, c, d)` such that `a * b = c * d`, where `a < b < c < d` and all four indices are distinct. The input vector may be empty or contain fewer than four elements, in which case the result is 0. The function must handle up to 10^4 elements efficiently (do not use brute force over all quadruples). The order of `a` and `b` within a pair and `c` and `d` within a pair does not matter; a quadruple is counted once if it satisfies the condition. For example, given `nums = {2, 3, 4, 6}`, the pairs `(2, 6)` and `(3, 4)` both have product 12, so there is exactly one quadruple: `(2, 3, 4, 6)` — the function returns 1. Note that all integers are distinct, so no product from the same index appears more than once within a pair. The function must not modify the input vector and must use constant extra space beyond the hash map.

// The key observation is that for each pair of indices `(i, j)` with `i < j`, we compute their product `p = nums[i] * nums[j]`. If the same product `p` occurs among `k` different pairs, then each unordered pair of these `k` pairs forms a valid quadruple satisfying `a*b = c*d`, provided the four indices are distinct. However, because all numbers are distinct, two different pairs with the same product automatically have four distinct indices (if they shared an index, the other element would be equal due to the product, contradicting distinctness). Thus, for each product value, if it occurs `k` times, the number of quadruples contributed is `C(k, 2) = k*(k-1)/2`. This is because each choice of two distinct pairs gives one quadruple. The brute-force double loop over all pairs (`i < j`) runs in O(n^2) time, which is acceptable for n up to 10^4 (10^8 operations may be borderline but typically fine in optimized C++ with a good hash map). For each product, we count how many times it appears. The total number of quadruples is the sum over all products of `k*(k-1)/2`. Edge cases: empty vector, size < 4, or no repeated products all return 0. Time complexity is O(n^2) due to the pair enumeration, and space complexity is O(n^2) in the worst case for the hash map storing up to n*(n-1)/2 distinct products, but typically less. However, we can avoid storing all pairs by using a counting technique: we still need to count occurrences per product, so the same space bound applies. The algorithm is straightforward: iterate all pairs, increment a counter for each product in an unordered_map, then sum `count*(count-1)/2` over all entries. Since all numbers are distinct and positive, no overflow occurs for products up to 10^4 * 10^4 = 10^8, which fits in a 32-bit int, but using `long long` for safety is recommended for the final answer.

#include <vector>
#include <unordered_map>

// Count quadruples (a,b,c,d) with distinct indices such that a*b = c*d.
// Assumes input contains distinct positive integers.
// Returns the total number of such quadruples.
long long countQuadruples(const std::vector<int>& nums) {
    const int n = static_cast<int>(nums.size());
    if (n < 4) return 0;  // Not enough elements for a quadruple.

    std::unordered_map<long long, int> productCount;
    productCount.reserve(static_cast<size_t>(n) * (n - 1) / 2);

    // Count how many pairs have each product.
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            long long prod = static_cast<long long>(nums[i]) * nums[j];
            ++productCount[prod];
        }
    }

    // For each product appearing k times, it contributes C(k,2) quadruples.
    long long total = 0;
    for (const auto& entry : productCount) {
        long long k = entry.second;
        total += k * (k - 1) / 2;
    }
    return total;
}

#include <cassert>
#include <vector>

// The solution function declaration (as above) is assumed available.
long long countQuadruples(const std::vector<int>& nums);

int main() {
    // Basic case: one quadruple from pairs (2,6) and (3,4)
    assert(countQuadruples({2, 3, 4, 6}) == 1);

    // Empty vector
    assert(countQuadruples({}) == 0);

    // Fewer than 4 elements
    assert(countQuadruples({1, 2, 3}) == 0);

    // Two different products each giving one quadruple
    // (1,12)=(3,4) and (2,12)=(3,8) share product 12? Actually check:
    // nums = {1,2,3,4,6,8,12}
    // pairs: (1,12)=12, (2,6)=12, (3,4)=12 -> k=3 => C(3,2)=3 quadruples
    // pairs: (2,8)=16, (4,4)? no distinct indices, (1,16)? not in list.
    // Only product 12 appears 3 times -> total 3.
    assert(countQuadruples({1, 2, 3, 4, 6, 8, 12}) == 3);

    // No repeated products
    assert(countQuadruples({1, 2, 3, 5, 7}) == 0);

    // Larger example: nums = {1,2,3,4,6,8,12,24}
    // Product 24: (1,24),(2,12),(3,8),(4,6) -> k=4 => C(4,2)=6
    // Product 12: (1,12),(2,6),(3,4) -> k=3 => C(3,2)=3
    // Total 9
    assert(countQuadruples({1, 2, 3, 4, 6, 8, 12, 24}) == 9);

    // Product with many pairs: {1,2,4,8,16,32,64}
    // Product 64 appears: (1,64),(2,32),(4,16) -> 3 pairs => 3 quadruples
    // Product 128 appears: (2,64),(4,32),(8,16) -> 3 pairs => 3 quadruples
    // No other repeats. Total 6.
    assert(countQuadruples({1, 2, 4, 8, 16, 32, 64}) == 6);

    // Duplicate products across different pairs but overlapping indices? Not possible with distinct numbers.
    // Ensure large input doesn't crash (just compile-time sanity, not a full stress test)
    std::vector<int> big(1000);
    for (int i = 0; i < 1000; ++i) big[i] = i + 1;
    // Just call to ensure no runtime error (will be O(n^2)=1e6 pairs, fine)
    (void)countQuadruples(big);

    return 0;
}
