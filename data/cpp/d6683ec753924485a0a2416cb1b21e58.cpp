Write a C++ function `xorPairSum` that takes a vector of non-negative integers and returns the sum of `(a[i] ^ a[j])` for all pairs `(i, j)` where `i < j`. The function should return the result modulo `10^9 + 7`. The input vector may be empty (in which case return 0), and may contain up to `10^5` elements, each up to `10^9`. The function must be efficient enough to handle large inputs without iterating over all pairs explicitly.
The naive O(n^2) approach is too slow for n up to 10^5. Instead, we compute the contribution of each bit independently across all pairs. For a given bit position `b` (0 to 30, since numbers ≤ 10^9 < 2^30), let `count_one` be the number of elements with that bit set to 1, and `count_zero` be the number with that bit set to 0. For any pair `(i,j)` with `i<j`, the XOR will have this bit set to 1 exactly when one element has the bit 1 and the other has 0. The number of such unordered pairs is `count_one * count_zero`. Each such pair contributes `(1 << b)` to the sum. So we sum over all bits: contribution = `count_one * count_zero * (1 << b)`, taken modulo MOD. Edge cases: empty vector (return 0), all numbers identical (then for each bit either count_one or count_zero is zero, so sum is 0). Also handle modulo multiplication carefully to avoid overflow (use long long). Time complexity: O(n * 31) ≈ O(n), space O(1).
#include <vector>
#include <cstdint>

const long long MOD = 1000000007LL;

// Return sum_{i<j} (a[i] XOR a[j]) modulo 1e9+7.
long long xorPairSum(const std::vector<int>& nums) {
    if (nums.empty()) return 0;

    const int MAX_BIT = 30; // numbers up to 1e9 < 2^30
    long long result = 0;

    for (int bit = 0; bit <= MAX_BIT; ++bit) {
        long long count_one = 0;
        for (int value : nums) {
            if ((value >> bit) & 1) {
                ++count_one;
            }
        }
        long long count_zero = static_cast<long long>(nums.size()) - count_one;
        long long pairs_with_bit_set = count_one * count_zero % MOD;
        long long bit_value = (1LL << bit) % MOD;
        result = (result + pairs_with_bit_set * bit_value) % MOD;
    }

    return result;
}
#include <cassert>
#include <vector>

// The function is declared here (as in the solution above), but for test we include it.
long long xorPairSum(const std::vector<int>& nums);

int main() {
    // Test empty
    assert(xorPairSum({}) == 0);

    // Test single element
    assert(xorPairSum({5}) == 0);

    // Test two elements: 1 XOR 2 = 3
    assert(xorPairSum({1, 2}) == 3);

    // Test three elements: pairs: (1,2)=3, (1,3)=2, (2,3)=1 total=6
    assert(xorPairSum({1, 2, 3}) == 6);

    // Test all identical: XOR of any pair is 0
    assert(xorPairSum({7, 7, 7}) == 0);

    // Test larger values: 10^9 and 0 => XOR = 10^9
    assert(xorPairSum({1000000000, 0}) == 1000000000);

    // Test modulo: 10^5 elements all 1? Actually 1 XOR 1 = 0, so 0.
    // Better test modulo: 2000000000 and 1? XOR = 2000000001 which is < MOD so fine.
    // Test with value near MOD: 1000000006 XOR 0 = 1000000006
    assert(xorPairSum({1000000006, 0}) == 1000000006);

    // Test a mixed set with 4 elements: [1,2,4,8] all distinct powers of 2
    // Pairs: 1^2=3,1^4=5,1^8=9,2^4=6,2^8=10,4^8=12 sum=45
    assert(xorPairSum({1,2,4,8}) == 45);

    // Test with duplicates: [1,1,2] => pairs: (1,1)=0, (1,2)=3 twice => 6
    assert(xorPairSum({1,1,2}) == 6);

    // Test large: all numbers from 0 to 999 (1000 numbers). Sum of all pairwise XOR.
    // Known formula: sum = (sum over bits) count_one*count_zero * 2^bit.
    // We'll just check the function returns something consistent, but for assert we can use a small known case.
    // Instead, test with 0 and 1 only: multiple 0s and 1s.
    // [0,0,1,1]: pairs with xors: (0,0)=0,(0,0)=0,(0,1)=1 four times? Actually 0s indices 0,1 and 1s indices 2,3 => 2*2=4 pairs of (0,1) each XOR=1 => sum=4.
    assert(xorPairSum({0,0,1,1}) == 4);

    return 0;
}
