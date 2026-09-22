Write a C++ function `long long bitwiseSubarraySum(const std::vector<int>& A)` that takes a non-empty array of non-negative integers and returns the sum of the bitwise AND of every contiguous subarray, modulo 998244353. For example, for `[1,2,3]`, the subarrays are [1]=1, [2]=2, [3]=3, [1,2]=0, [2,3]=2, [1,2,3]=0, so the sum is 1+2+3+0+2+0 = 8. The function must be self-contained (no global state) and must not use any library beyond standard C++ headers. The solution should be efficient for up to N=200,000 elements, where each element is in the range [0, 1,000,000,000]. The modulo constant is given as 998244353 (a prime). The function must handle duplicate values, zeros, and large values correctly.

#include <cassert>
#include <vector>
#include <iostream>

// Brute force for verification (not part of the solution function)
long long bruteForce(const std::vector<int>& A) {
    constexpr long long MOD = 998244353LL;
    int n = (int)A.size();
    long long total = 0;
    for (int i = 0; i < n; ++i) {
        int current = A[i];
        for (int j = i; j < n; ++j) {
            current &= A[j];
            total = (total + current) % MOD;
        }
    }
    return total;
}

int main() {
    // Test 1: basic example
    assert(sumOfBitwiseAnd({1, 2, 3}) == 8);
    // Test 2: all zeros
    assert(sumOfBitwiseAnd({0, 0, 0}) == 0);
    // Test 3: single element
    assert(sumOfBitwiseAnd({5}) == 5);
    // Test 4: all same nonzero
    assert(sumOfBitwiseAnd({7, 7, 7}) == 7 * 6); // subarrays: 7 +7+7+7+7+7 = 42
    // Test 5: mixed with zeros
    assert(sumOfBitwiseAnd({1, 0, 1}) == 1 + 0 + 1 + 0 + 0 + 0); // [1],[0],[1],[1,0]=0,[0,1]=0,[1,0,1]=0 => 2
    // Test 6: large value
    assert(sumOfBitwiseAnd({1024}) == 1024);
    // Test 7: random small arrays compared to brute force
    std::vector<std::vector<int>> tests = {
        {3, 5, 6, 2},
        {12, 4, 8, 16},
        {1, 1, 2, 2, 3, 3},
        {0, 0, 1, 0, 1},
        {1000000000, 999999999, 123456789, 987654321}
    };
    for (const auto& arr : tests) {
        assert(sumOfBitwiseAnd(arr) == bruteForce(arr));
    }
    // Test 8: all bits set pattern
    std::vector<int> big(200, (1<<30)-1); // 200 elements of all ones
    // Expected: sum over all subarrays of AND = (1<<30)-1 times number of subarrays
    long long count = (long long)200 * 201 / 2;
    assert(sumOfBitwiseAnd(big) == ((((1LL<<30)-1) % 998244353LL) * (count % 998244353LL)) % 998244353LL);
    return 0;
}

#include <vector>
#include <cstdint>

// Compute the sum of bitwise AND of all contiguous subarrays modulo 998244353.
// A must be non-empty and contain non-negative integers < 2^30.
long long sumOfBitwiseAnd(const std::vector<int>& A) {
    constexpr long long MOD = 998244353LL;
    const int n = static_cast<int>(A.size());
    long long result = 0;

    // Iterate over each bit position (0..29)
    for (int bit = 0; bit < 30; ++bit) {
        long long run = 0;  // length of current suffix where all elements have this bit set
        long long bitValue = 1LL << bit;
        for (int i = 0; i < n; ++i) {
            if (A[i] & (1 << bit)) {
                ++run;
            } else {
                run = 0;
            }
            result = (result + (run % MOD) * bitValue) % MOD;
        }
    }
    return result;
}

// A direct approach enumerating all subarrays is O(N^2) and too slow for N up to 200,000. Instead, we use a bitwise contribution technique. For each bit position `j` (0 to 29, since the maximum value is less than 2^30), we need to count how many subarrays have the j-th bit set in their AND result. The AND of a subarray has bit j set if and only if all elements in that subarray have bit j set. So for each bit, we process the array from left to right, maintaining a count `count[j]` that represents the total number of starting positions of subarrays that end at the current index and all have bit j set? Actually, a more efficient method is the incremental formula: when we consider element at index `i` (0-based), all subarrays that end at `i`. Let `totalEndedAtI = i+1`. For bit j:
// - If A[i] has bit j set, then the bit j remains set in any subarray that ended at `i-1` and had bit j set, plus the single-element subarray [i] itself. So the number of subarrays ending at i with bit j set is `previousCount + 1`, but we also can add the contribution of subarrays that end at i to the total result. However the provided snippet uses a clever formula: it tracks `count[j]` which is the sum over all subarrays ending before i of the number of starting positions that form a subarray with bit j set? Let's derive: For bit j, consider all subarrays ending at index k (k from 0 to i). For each such subarray, if all elements have bit j set, we add 2^j to result. The number of such subarrays ending at k is exactly the length of the longest contiguous run ending at k where all elements have bit j set? Actually, for subarray [l..k] to have bit j set, all A[l]..A[k] must have bit j set. So if A[k] has bit j set, and let `runLength` = number of consecutive elements ending at k that all have bit j set. Then the number of subarrays ending at k with bit j set is `runLength`. The total contribution to result over all subarrays is sum over all k of (runLength at k) * 2^j. The snippet's `count[j]` is essentially the accumulated sum of `runLength` over all previous indices? Looking at the code: when A[i] has bit j set, it does `result += ((N-i)*(i+1) - count[j]) * 2^j` and then `count[j] += N-i`. That's not directly what I described. Let's reinterpret: The code processes from i=0 to N-1 (left to right). For each i, it considers all subarrays that start at i and end somewhere? Actually `N-i` is the number of subarrays that start at i. `i+1` is the number of subarrays that end at i. Their product is the total number of subarrays that either start at i or end at i? Wait, `(N-i)*(i+1)` counts each subarray [l..r] exactly once? Because for each subarray, it has a starting index l and ending index r. For a given i, `(N-i)*(i+1)` counts all subarrays that either start at <=i and end at >=i? That is the number of subarrays that include position i. Indeed, for a fixed i, there are (i+1) choices for l (0..i) and (N-i) choices for r (i..N-1), giving subarrays that include i. So each subarray is counted exactly (r-l+1) times? Actually, a subarray [l..r] includes positions l..r, so it is counted for each i in that range, which is (r-l+1) times. The total sum over all i of (N-i)*(i+1) is sum over all subarrays of their length. That is the sum of lengths of all subarrays. For AND, we want sum over subarrays of AND value. For each bit, the AND value of a subarray either has that bit set (contributes 2^j) or not. So total sum = sum over bits 2^j * (number of subarrays where bit j is set). Now, `count[j]` in the code is maintained as: when processing i, count[j] accumulates something like `N - i` for each previous index where A[i] had bit set? Let's examine: If A[i] has bit j set, the code does `result += ((N-i)*(i+1) - count[j]) * 2^j`. That adds 2^j times (total number of subarrays that include i) minus count[j]. That suggests that for bit j, at index i where bit is set, we add 2^j for every subarray that includes i but does NOT have bit j set? Because total subarrays including i is (N-i)*(i+1). If we subtract count[j] (which presumably counts subarrays including previous indices that already have bit set), we get subarrays including i where bit j is NOT set? That seems odd. Actually, the correct approach is: For each bit j, we can count how many subarrays do NOT have bit j set (i.e., at least one zero). Then subtract from total subarrays. But the code is different. Let's test on simple example: A=[1,2,3], N=3. bits up to 30. Process bit 0 (1<<0=1). For i=0, A=1 (bit0 set). N-i=3, i+1=1, product=3. count[0]=0 initially. Result += (3 - 0)*1 =3. Then count[0] += N-i=3 -> count[0]=3. i=1, A=2 (bit0 not set, since 2 binary 10). The else branch: result += count[0]*1 = 3. So now result=6. No update to count. i=2, A=3 (bit0 set: 3 has bit0=1). N-i=1, i+1=3, product=3. count[0]=3. result += (3-3)*1=0. count[0]+=1 ->4. After loop, result from bit0 is 6. Similarly bit1 (2): i=0 A=1 (bit1 not set) -> result += count[1]*2 = 0. i=1 A=2 (bit1 set) -> (N-i=2, i+1=2 product=4) - count[1](0) =4, *2=8, result=6+8=14, count[1]+=2 ->2. i=2 A=3 (bit1 set) -> (N-i=1, i+1=3 product=3)-count[1](2)=1, *2=2, result=16, count[1]+=1 ->3. For bit2 (4) none. Final result=16, but expected is 8. So the code produces 16, which is double? Actually, the original snippet might compute something else – it's not the sum of AND, but maybe sum of XOR? Let's check: XOR of subarrays for [1,2,3] is: [1]=1, [2]=2, [3]=3, [1,2]=3, [2,3]=1, [1,2,3]=0, sum=10. Not 16. So the snippet is something else probably. But my task is to create an independent task inspired by it, not necessarily reproduce the snippet's exact behavior. Since the snippet is a bitwise contribution pattern, I will create a classic problem: sum of bitwise AND of all subarrays. I need to provide a correct solution. The correct method uses `count[j]` as the number of subarrays ending at current index where all elements have bit j set. Actually, let's derive: For each bit j, we want number of subarrays where all elements have bit j set. We can maintain `run` = length of suffix ending at current index where all elements have bit j set. Then add `run` to answer for each index (since that many subarrays ending here have bit set). That is O(N * 30). For each i, if A[i] has bit j set, `run++`, else `run=0`. Then `ans += run * (1<<j)`. That sums correctly. But the snippet uses a different formula that seems to compute sum of bitwise XOR? Actually, it computes sum over i of (number of subarrays including i that have an odd number of set bits? No). Let’s not worry. I will design a task for sum of bitwise AND. I'll provide a clean solution. Edge cases: zeros break runs. Negative numbers not allowed. Modulo applies. Time O(30N), space O(1) extra. I'll write function `long long sumOfBitwiseAnd(std::vector<int> A)`. For the test, I'll include several array examples and compare with brute force for small arrays, and also test large simple patterns.
