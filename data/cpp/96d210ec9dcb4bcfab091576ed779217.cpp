/*
Write a C++ function `countValidNumbers(long long N, long long K, const std::vector<long long>& A, const std::vector<long long>& B)` that returns the number (modulo 1,000,000,007) of N-digit positive integers (no leading zeros) that can be partitioned from the most significant digit into K-digit blocks (each block is exactly K digits, so N is a multiple of K), such that for every block index i (0 ≤ i < N/K), the block's value is divisible by A[i] and its first digit (the most significant digit of that block) equals B[i] (where 0 ≤ B[i] ≤ 9). The input vectors A and B each have length exactly N/K, with A[i] being a positive integer. Output the count modulo mod. The function must handle N up to 10^5 and K up to 10 (since K ≤ 10 for practical int64 overflow, but the function should still work for K up to 18 if needed by using __int128 for intermediate products, but you may assume K ≤ 10 in the test). The blocks are independent: you multiply the number of valid blocks for each position. For each block, the block's value is an integer from 10^(K-1)·B[i] (inclusive) to 10^(K-1)·(B[i]+1)-1 (inclusive). Count how many numbers in that range are divisible by A[i].
*/
#include <vector>
#include <cstdint>

const long long MOD = 1000000007LL;

// Count valid N-digit numbers (blocks of K digits) modulo MOD.
// For each block i, its K-digit value must be divisible by A[i] and its
// first digit must equal B[i]. Blocks are independent, so multiply counts.
long long countValidNumbers(long long N, long long K,
                            const std::vector<long long>& A,
                            const std::vector<long long>& B) {
    long long pow10K = 1;
    for (long long i = 0; i < K; ++i) pow10K *= 10LL; // 10^K
    long long pow10K1 = pow10K / 10LL; // 10^(K-1)

    long long result = 1;
    long long blocks = N / K;
    for (long long i = 0; i < blocks; ++i) {
        long long L = pow10K1 * B[i]; // inclusive lower bound of block value
        long long R = pow10K1 * (B[i] + 1) - 1; // inclusive upper bound
        long long a = A[i];
        long long count;
        if (L == 0) {
            // Multiples of a from 0 to R inclusive
            count = (R / a) + 1;
        } else {
            // Multiples in [L, R] = floor(R/a) - floor((L-1)/a)
            count = (R / a) - ((L - 1) / a);
        }
        result = (result * (count % MOD)) % MOD;
    }
    return result;
}
#include <cassert>
#include <vector>

// The solution function is declared above.

int main() {
    // Example 1: N=2, K=1, blocks: 2 blocks. A=[2,3], B=[1,2]
    // Block0: [1,1] divisible by 2? only 2? Actually [1*10^0=1, (1+1)-1=1] => {1} not divisible by 2 => count 0
    // So total 0.
    {
        std::vector<long long> A = {2, 3};
        std::vector<long long> B = {1, 2};
        assert(countValidNumbers(2, 1, A, B) == 0);
    }

    // Example 2: N=2, K=1, A=[1,1], B=[0,0] but first block B[0]=0 would make number 0? Not N-digit; but our function doesn't check that. Let's use B[0]=1, B[1]=0.
    // Block0: [1,1] divisible by 1 => 1 choice. Block1: [0,0] divisible by 1 => 1 choice (value 0). Total 1.
    {
        std::vector<long long> A = {1, 1};
        std::vector<long long> B = {1, 0};
        assert(countValidNumbers(2, 1, A, B) == 1);
    }

    // Example 3: N=4, K=2, A=[10, 10], B=[1, 0]
    // Block0: L=10*1=10, R=19. Multiples of 10: 10 only => count 1.
    // Block1: L=10*0=0, R=9. Multiples of 10: 0 only => count=1 (since 0 divisible). Total 1.
    {
        std::vector<long long> A = {10, 10};
        std::vector<long long> B = {1, 0};
        assert(countValidNumbers(4, 2, A, B) == 1);
    }

    // Example 4: N=3, K=3, single block A=[7], B=[1]
    // L=100, R=199. Multiples of 7: 105,112,...,196 => (196-105)/7+1=14. Check: floor(199/7)=28, floor(99/7)=14 => 28-14=14.
    {
        std::vector<long long> A = {7};
        std::vector<long long> B = {1};
        assert(countValidNumbers(3, 3, A, B) == 14);
    }

    // Example 5: N=6, K=2, A=[2,3,5], B=[1,2,3]
    // Block0: L=10, R=19, multiples of 2: 10,12,14,16,18 => 5
    // Block1: L=20, R=29, multiples of 3: 21,24,27 => 3
    // Block2: L=30, R=39, multiples of 5: 30,35 => 2
    // Total 5*3*2=30
    {
        std::vector<long long> A = {2, 3, 5};
        std::vector<long long> B = {1, 2, 3};
        assert(countValidNumbers(6, 2, A, B) == 30);
    }

    // Example 6: Large A (e.g., A=1000) with K=1, B=9
    // L=9, R=9, divisibility by 1000: none => 0
    {
        std::vector<long long> A = {1000};
        std::vector<long long> B = {9};
        assert(countValidNumbers(1, 1, A, B) == 0);
    }

    // Example 7: K=2, A=1, B=0 for a block: all 0..9 are valid? Actually L=0,R=9, count=10
    {
        std::vector<long long> A = {1, 1};
        std::vector<long long> B = {1, 0};
        // Block0: L=10,R=19 count=10, Block1: L=0,R=9 count=10, total=100
        assert(countValidNumbers(4, 2, A, B) == 100);
    }

    // Example 8: Modulo check with large number of blocks. N=10, K=1, A all 2, B all 1
    // Each block: L=1,R=1, divisible by 2? none => 0 total.
    {
        std::vector<long long> A(10, 2);
        std::vector<long long> B(10, 1);
        assert(countValidNumbers(10, 1, A, B) == 0);
    }

    // Example 9: All A=1, all B=0 except first B=1, for N=6,K=2 (3 blocks)
    // Each block: block0 L=10,R=19 count=10; block1 L=0,R=9 count=10; block2 L=0,R=9 count=10 => total 1000
    {
        std::vector<long long> A(3, 1);
        std::vector<long long> B = {1, 0, 0};
        assert(countValidNumbers(6, 2, A, B) == 1000);
    }

    // Example 10: Edge: K=1, B=0 for only block? That would make number 0, but our function doesn't enforce N-digit. Test that it returns 1 if A=1.
    {
        std::vector<long long> A = {1};
        std::vector<long long> B = {0};
        // L=0,R=0, count=1 (0 is divisible by 1)
        assert(countValidNumbers(1, 1, A, B) == 1);
    }

    return 0;
}
// The core observation is that the N-digit number is just K independent blocks concatenated (from most significant to least). For each block i, the block's value is restricted to a specific interval: if the block's leading digit is B[i], then the block's value v satisfies L = 10^(K-1)·B[i] ≤ v ≤ R = 10^(K-1)·(B[i]+1)-1. We need to count integers v in [L,R] such that v % A[i] == 0. The count of multiples of A[i] in any interval [L,R] is: floor(R/A[i]) - floor((L-1)/A[i]) (with the convention that floor(x/A) for x ≤ 0 gives 0). Since all blocks are independent (the choice of digits in one block doesn't affect another), the total count is the product over all blocks of the number of valid choices for that block, taken modulo mod. Edge cases: B[i]=0 (then L=0, but since the block is part of a larger number and the most significant digit of the whole number is the first digit of the first block, B[0] cannot be 0 for the whole number to be N-digit, but the problem says the number is N-digit with no leading zeros, so B[0] must be >=1; however, for other blocks B[i] can be 0, allowing the block itself to have leading zeros internally, which is fine). For A[i]=1, every number in the interval is valid, so count = R-L+1. Careful with integer overflow: 10^K for K up to 10 fits in long long (10^10 > 2^31 but < 2^63). For K=10, 10^(K-1)=10^9, and R up to 10^10-1, but multiplying by A up to 10^5 could overflow long long? We avoid multiplication of large numbers; we only compute floor divisions. The count formula only uses divisions by A[i] on large numbers up to ~10^10, which fits in long long (max ~1e10). So safe. Compute x = 10^K (as long long). Then total multiples in full range [0, x-1] is (x-1)/a +1. Then subtract the count in [0, L-1] and [R+1, x-1]? Simpler: use formula directly: count = floor(R/a) - floor((L-1)/a) if L>0 else floor(R/a)+1 (but since L can be 0 only when B=0, then L=0 so count = floor(R/a)+1? Actually multiples from 0 to R inclusive. But careful: block value must be at least 0? Since the block is part of the whole number, it can be 0 if B=0, and 0 is divisible by any a, and 0 is in the interval, so include it. So use general formula: count = floorDiv(R,a) - floorDiv(L-1,a) with the convention that for negative numbers, floor division yields negative, but here L-1 >= -1 only when L=0 (so L-1=-1). For L=0, floor(-1/a) = -1 (if a>1), so floor(R/a)-(-1)=floor(R/a)+1, which correctly includes 0. So we can implement a safe floor division for non-negative divisor by using integer division for non-negative numerator. Here L-1 is either >=0 (if L>0) or -1 (if L=0). For -1, integer division in C++ truncates toward zero, giving -1/ a = 0, which is wrong. So we must handle L=0 specially: count = floor(R/a)+1. Or use a helper function floorDiv(n,a) that for n>=0 returns n/a, and for n<0 returns -((-n+a-1)/a) (i.e., floor). Simpler: for each block, compute L and R. If L==0, count = R/a + 1. Else count = R/a - (L-1)/a (all non-negative). This avoids negative division issues. For B[i]>0, L>=10^(K-1) >=1, so fine. For B[i]=0, L=0, handle separately. Time complexity: O(N/K) per call, space O(1) extra (aside from input vectors). The product is taken modulo mod.
