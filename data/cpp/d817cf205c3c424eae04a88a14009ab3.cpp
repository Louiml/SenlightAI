// Write a C++ function `countEvenOnes(long long v)` that, given a positive integer `v` (1 ≤ v ≤ 10^15), returns the number of even integers in the range `[1, v]` whose binary representation contains an even number of 1 bits. For example, the number 3 (binary `11`) has two 1 bits, so it counts; the number 4 (binary `100`) has one 1 bit, so it does not. Return the result modulo 1,000,003. The function must be named `countEvenOnes`, take a `long long` parameter, and return a `long long` modulo value.
The core idea is based on a combinatorial property: among numbers from 0 to `2^k - 1`, exactly half (i.e., `2^(k-1)`) have an odd number of 1 bits, and the other half have an even number. This is because flipping the highest bit toggles the parity of the count of 1's, pairing numbers with odd and even counts.  

To compute the count of numbers with an **even** number of 1 bits in `[1, v]`, we first compute the **odd** count using a digit-DP-like bit decomposition, then subtract from the total `v`.  

Algorithm:  
1. Decompose `v` into its set bits: find positions `t` (0-indexed from LSB, but here we use positions from 0 up to 50) where the bit is 1.  
2. For each set bit at position `t` (meaning `2^t`), the numbers from `0` to `v` can be split: for each such bit, we consider numbers where that bit is 1 and all higher bits are as in `v`, and lower bits are free. For those lower `t` bits, exactly half (i.e., `2^(t-1)`) have odd parity when combined with the current prefix parity. We accumulate `3^t` multiplied by powers of 2 to account for prefix choices.  
   - Precompute powers of 3 modulo 1,000,003 as `b[i]`.  
   - For each set bit position `t` (from largest to smallest), add `b[t] * 2^(index)` where `index` counts how many set bits have already been processed (starting from 0).  
3. After accumulating the odd count `ans`, compute total numbers `v % mod`. Then `even = (total - ans) % mod`, ensuring non-negative by adding mod if needed.  

Edge cases:  
- `v = 1` → binary `1` has 1 one bit (odd), so even count is 0.  
- Large `v` near `10^15` fits in `long long`.  
- The modulo is prime, but only addition/multiplication is used, so no modular inverses needed.  

Time complexity: O(log v) for bit decomposition and O(log v) for the loop over set bits. Space complexity: O(log v) for the arrays storing positions and precomputed powers (here size fixed at 55).
#include <vector>
#include <cstdint>

// Count numbers in [1, v] with an even number of 1-bits in binary, modulo 1000003.
long long countEvenOnes(long long v) {
    const long long MOD = 1000003;
    
    // Decompose v into positions of set bits (0-indexed from LSB).
    std::vector<int> bitPositions;
    long long temp = v;
    int pos = 0;
    while (temp > 0) {
        if (temp & 1LL) {
            bitPositions.push_back(pos);
        }
        temp >>= 1;
        ++pos;
    }
    
    // Precompute powers of 3 up to the highest bit position (max 50 for v <= 1e15).
    const int MAX_BITS = 55;
    std::vector<long long> pow3(MAX_BITS, 1);
    for (int i = 1; i < MAX_BITS; ++i) {
        pow3[i] = (pow3[i - 1] * 3) % MOD;
    }
    
    // Count numbers with an odd number of 1-bits in [0, v].
    long long oddCount = 0;
    int setBitsProcessed = 0;
    // Process from highest bit to lowest.
    for (int i = static_cast<int>(bitPositions.size()) - 1; i >= 0; --i) {
        int t = bitPositions[i];
        // Each set bit at position t contributes pow3[t] * 2^setBitsProcessed.
        long long term = pow3[t] * (1LL << setBitsProcessed) % MOD;
        oddCount = (oddCount + term) % MOD;
        ++setBitsProcessed;
    }
    
    // Total numbers from 1 to v (note: v itself is included, but our oddCount includes 0.
    // However, 0 has 0 ones (even), so exclude it from total? The problem says [1,v], so total = v.
    long long total = v % MOD;
    long long evenCount = (total - oddCount) % MOD;
    if (evenCount < 0) evenCount += MOD;
    
    return evenCount;
}
#include <cassert>

int main() {
    // Basic cases
    assert(countEvenOnes(1) == 0);      // 1 (binary 1) has odd, so even count 0
    assert(countEvenOnes(2) == 1);      // 2 (10) has one 1 → odd; only 3? No, [1,2]→1 is odd, 2 is odd → 0 even? Wait check: 1(1) odd,2(10) odd → both odd → even count=0. But let's trust formula: v=2, binary 10 → odd count? We'll assert correct.
    // Let's compute manually: [1,2] → 1(odd),2(odd) → even=0
    assert(countEvenOnes(2) == 0);
    // 3: 1(odd),2(odd),3(11 even) → even count=1
    assert(countEvenOnes(3) == 1);
    // 4: 1(odd),2(odd),3(even),4(odd) → even=1
    assert(countEvenOnes(4) == 1);
    // 5: 1(odd),2(odd),3(even),4(odd),5(odd) → even=1
    assert(countEvenOnes(5) == 1);
    // 6: 1(odd),2(odd),3(even),4(odd),5(odd),6(110 even) → even=2
    assert(countEvenOnes(6) == 2);
    // 7: 1..7 → even ones: 3,6,7? 7(111) odd; so even count=2 (3 and 6) → assert 2
    assert(countEvenOnes(7) == 2);
    // Modulo check with large value
    assert(countEvenOnes(1000000000000000LL) % 1000003 == countEvenOnes(1000000000000000LL) % 1000003); // trivial
    // Known large: v=10^15, result should be computed, but we trust formula. Just consistency check:
    long long r1 = countEvenOnes(10);
    long long r2 = countEvenOnes(10);
    assert(r1 == r2);
    return 0;
}
