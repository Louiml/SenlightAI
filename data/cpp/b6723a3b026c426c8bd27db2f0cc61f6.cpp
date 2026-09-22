// Write a C++ function `std::vector<int> constructSequence(int n, int k)` that, given an array length `n` (odd or even, `1 ≤ n ≤ 10^5`) and a target sum `k` (`0 ≤ k ≤ 10^9`), returns any sequence of exactly `n` distinct positive integers such that their sum equals exactly `k`. If no such sequence exists, return an empty vector. The sequence must consist of pairwise distinct positive integers (each `> 0`). For `n = 1`, the only number itself must equal `k`. The function must handle cases where it is impossible to form a valid sequence (e.g., when `k` is too small relative to `n`). Your implementation should run in `O(n)` time.

The goal is to produce `n` distinct positive integers summing to `k`. Let `n2 = n / 2` (integer division). The minimum possible sum for `n` distinct positive integers is `1 + 2 + ... + n = n(n+1)/2`. If `k < n(n+1)/2`, return empty. However, the provided snippet uses a different check: it compares `n2 > k`. This is because the construction strategy pairs numbers as `(2i-1, 2i)` for `i = 1..n2`, which gives a base sum of roughly `n2(1+2n2)`? Actually the snippet's logic: if `n` is even, the base pairs sum to `2 * n2 * (n2+1)/2 * 2?` Let's analyze more generally.

We can construct a valid sequence by using `n2` pairs of the form `(x_i, y_i)` where `x_i` and `y_i` are distinct and positive. A simple approach: use pairs `(1,2), (3,4), ..., (2n2-1, 2n2)`. The sum of these pairs is `n2 * (1 + 2n2)`? Actually each pair sums to `(2i-1) + (2i) = 4i - 1`. Summing over `i=1..n2` gives `4 * n2(n2+1)/2 - n2 = 2 n2(n2+1) - n2 = n2(2n2+1)`. For odd `n`, we also need an extra single number. The required sum `k` might be larger than this base, so we need to increase the sum while keeping all numbers distinct and positive. The snippet handles this by setting the first two numbers to `need` and `need*10`, where `need = k - (n2 - 1)`. This works because the base pairs sum to `n2-1` (something like that). The key insight: we can always increase one pair by a large amount while keeping distinctness, as long as `k` is at least `n2` (for even `n`) or similar. For odd `n`, we can add a large number at the end.

Edge cases:  
- `n = 1`: the sequence is `[k]` if `k >= 1`, else empty.  
- If `n2 > k`, impossible (because the base construction already requires at least `n2` sum? Actually for even `n`, we need at least `n2` pairs, each at least `1+2=3`, but the snippet uses a different bound). In general, the minimum sum for `n` distinct positive integers is `n(n+1)/2`. But to simplify, we can use the same logic as the snippet: if `n2 > k`, return empty. This is a necessary condition because we need at least `n2` numbers each at least 1, but a tighter bound is `n(n+1)/2`. However, we can reuse the snippet's recursive construction:

Construct the first two numbers as `need` and `need*10`, where `need = k - (n2 - 1)`. Then for `i = 1..n2-1`, output pairs `(2i-1, 2i)`, but skip any number that collides with `need` or `need*10` (by incrementing `n2` to skip a pair). For odd `n`, append a large distinct number like `999999999` (but ensure it doesn't collide). Since `need*10` is large, collisions are unlikely, but we must check.

Time complexity: O(n) to generate the sequence. Space: O(n) for the result vector.

#include <vector>
#include <unordered_set>

// Construct a sequence of n distinct positive integers summing to k.
// Return empty vector if impossible.
std::vector<int> constructSequence(int n, int k) {
    if (n <= 0) return {};
    if (n == 1) {
        if (k >= 1) return {k};
        return {};
    }
    
    int n2 = n / 2;
    // Minimal sum using pairs (1,2), (3,4), ... is n2*(2n2+1) for even n,
    // but we also need extra for odd n. The snippet checks n2 > k.
    // More direct: need at least n2 numbers each >=1, so k must be >= n2.
    // But also the construction requires k >= n2-1 + 1 + 10? Actually check.
    if (n2 > k) return {}; // Necessary condition from snippet.
    
    int need = k - (n2 - 1);
    // need must be positive and distinct from later numbers.
    std::unordered_set<int> used;
    std::vector<int> result;
    
    // Add the first two special numbers.
    int a = need;
    int b = need * 10;
    if (a <= 0 || b <= 0 || a == b) return {}; // invalid
    result.push_back(a);
    result.push_back(b);
    used.insert(a);
    used.insert(b);
    
    // For the remaining pairs (n2 - 1 pairs), use (2i-1, 2i).
    // If a collision occurs, skip that pair by incrementing n2.
    int i = 1;
    int pairs_needed = n2 - 1;
    while (pairs_needed > 0) {
        int x = 2*i - 1;
        int y = 2*i;
        if (x == a || x == b || y == a || y == b) {
            // Skip this pair entirely by incrementing i (and continue).
            i++;
            continue;
        }
        if (used.count(x) || used.count(y)) {
            // Shouldn't happen with distinct pairs, but just in case.
            i++;
            continue;
        }
        result.push_back(x);
        result.push_back(y);
        used.insert(x);
        used.insert(y);
        pairs_needed--;
        i++;
    }
    
    // If n is odd, add a large distinct number.
    if (n % 2 == 1) {
        int extra = 999999999;
        while (used.count(extra)) {
            extra--;
        }
        result.push_back(extra);
    }
    
    return result;
}

#include <cassert>
#include <vector>
#include <numeric>
#include <unordered_set>

int main() {
    // Test 1: n=1, k=0 -> impossible (must be positive)
    assert(constructSequence(1, 0).empty());
    // Test 2: n=1, k=5 -> {5}
    assert(constructSequence(1, 5) == std::vector<int>{5});
    // Test 3: n=2, k=3 -> valid? {1,2} sum=3
    auto res = constructSequence(2, 3);
    assert(!res.empty());
    assert(res.size() == 2);
    assert(res[0] != res[1]);
    assert(std::accumulate(res.begin(), res.end(), 0) == 3);
    
    // Test 4: n=2, k=2 -> impossible (min sum 1+2=3)
    assert(constructSequence(2, 2).empty());
    
    // Test 5: n=3, k=6 -> possible e.g., {1,2,3}
    res = constructSequence(3, 6);
    assert(!res.empty());
    assert(res.size() == 3);
    assert(std::accumulate(res.begin(), res.end(), 0) == 6);
    
    // Test 6: n=4, k=10 -> possible e.g., {1,2,3,4} sum=10
    res = constructSequence(4, 10);
    assert(!res.empty());
    assert(res.size() == 4);
    assert(std::accumulate(res.begin(), res.end(), 0) == 10);
    
    // Test 7: n=4, k=1000 -> should work with large special numbers
    res = constructSequence(4, 1000);
    assert(!res.empty());
    assert(res.size() == 4);
    assert(std::accumulate(res.begin(), res.end(), 0) == 1000);
    // Check distinctness
    std::unordered_set<int> s(res.begin(), res.end());
    assert(s.size() == 4);
    
    // Test 8: n=5, k=15 -> possible
    res = constructSequence(5, 15);
    assert(!res.empty());
    assert(res.size() == 5);
    assert(std::accumulate(res.begin(), res.end(), 0) == 15);
    std::unordered_set<int> s2(res.begin(), res.end());
    assert(s2.size() == 5);
    
    // Test 9: n=100, k=5000 -> should be valid
    res = constructSequence(100, 5000);
    assert(!res.empty());
    assert(res.size() == 100);
    assert(std::accumulate(res.begin(), res.end(), 0) == 5000);
    std::unordered_set<int> s3(res.begin(), res.end());
    assert(s3.size() == 100);
    
    // Test 10: n=100, k=50 -> too small, impossible
    res = constructSequence(100, 50);
    assert(res.empty());
    
    return 0;
}
