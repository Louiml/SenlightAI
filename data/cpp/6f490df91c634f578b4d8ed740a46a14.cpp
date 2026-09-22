Write a C++ function `ll countAndPrintModifiedNumbers(ll n)` that takes a single non-negative 64-bit integer `n` and returns a `vector<ll>` containing all distinct positive values obtained by flipping exactly one set bit of `n` from 1 to 0 (i.e., clearing exactly one 1-bit), and also including the original `n` itself. The resulting vector must be sorted in descending order, and must not contain duplicates. For example, if `n = 7` (binary `111`), clearing one bit gives `6`, `5`, `3`, and adding original `7` gives `[7, 6, 5, 3]`. If `n` is a power of two (only one set bit), clearing that bit gives `0`, which is not positive and should be excluded, so the result is just `[n]`. The function must handle `n = 0` (which has no set bits) by returning an empty vector. The function should not print anything; it should just return the vector.

// The key is to iterate over each bit position from the most significant down to the least (bit 63 down to 0) and for each bit that is set in `n`, compute `n ^ (1<<i)` (which effectively clears that bit). Since we iterate from high to low, the original `n` will be larger than any modified value, so we should append `n` last to get descending order naturally if we iterate from high to low and then push `n` at the end. However, we must be careful not to include `0` because the problem says "positive values" — actually the code snippet includes `0` if `n` is a power of two? Let's check: the original code has `if (n&(1ll<<i)) if (n^(1ll<<i)) ans.pb(n^(1ll<<i));` — it only adds if `n^(1ll<<i)` is non-zero (since `if (n^(1ll<<i))` is true when that value is non-zero). So we must replicate that: only include the modified value if it's not zero. Since we iterate from high to low, the modified values are already in descending order, and then we append `n` at the end. However, for `n=0`, there are no set bits, so we return an empty vector. For `n` a power of two, the only modified value is `0`, which is excluded, so we return just `{n}`. Edge case: when `n` is `1`, clearing the only set bit gives `0`, so we return `{1}`. Complexity: O(64) time, O(1) auxiliary space besides the output vector.

#include <vector>
#include <cstdint>

// Returns all distinct positive integers obtained by clearing exactly one set bit of n,
// plus n itself, in descending order. Excludes 0. Returns empty for n=0.
std::vector<long long> getModifiedNumbers(long long n) {
    std::vector<long long> result;
    if (n == 0) return result;
    
    // Iterate from highest bit to lowest to get descending order for modified values.
    for (int i = 63; i >= 0; --i) {
        if (n & (1LL << i)) {
            long long modified = n ^ (1LL << i);
            if (modified != 0) {
                result.push_back(modified);
            }
        }
    }
    // Append original n last (it is largest).
    result.push_back(n);
    return result;
}

#include <cassert>
#include <vector>

// The solution function is already declared above; include it here or assume it's in scope.

int main() {
    // n = 7 (111) -> clear bits: 6(110),5(101),3(011) plus 7
    {
        auto v = getModifiedNumbers(7);
        std::vector<long long> expected = {7, 6, 5, 3};
        assert(v == expected);
    }
    // n = 0 -> empty
    {
        auto v = getModifiedNumbers(0);
        assert(v.empty());
    }
    // n = 1 (power of two) -> only original
    {
        auto v = getModifiedNumbers(1);
        std::vector<long long> expected = {1};
        assert(v == expected);
    }
    // n = 8 (1000) -> clear bit gives 0 (excluded), so just {8}
    {
        auto v = getModifiedNumbers(8);
        std::vector<long long> expected = {8};
        assert(v == expected);
    }
    // n = 15 (1111) -> clear each bit: 14,13,11,7 plus 15
    {
        auto v = getModifiedNumbers(15);
        std::vector<long long> expected = {15, 14, 13, 11, 7};
        assert(v == expected);
    }
    // n = 10 (1010) -> clear bit3(8) -> 2, clear bit1(2) -> 8, plus 10
    {
        auto v = getModifiedNumbers(10);
        std::vector<long long> expected = {10, 8, 2};
        assert(v == expected);
    }
    // Large number to test 64-bit handling: 2^62 + 1 -> clear bit62 gives 1, clear bit0 gives 2^62
    {
        long long n = (1LL << 62) + 1;
        auto v = getModifiedNumbers(n);
        std::vector<long long> expected = {n, (1LL << 62), 1LL};
        assert(v == expected);
    }
    return 0;
}
