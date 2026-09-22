/*
Write a C++ function that, given an array `I` of `N` integers (each in `[0, 255]`) and a count `N`, finds the parameters `(S, A, C)` each in the range `[0, 15]` that minimize the Shannon entropy of the transformed sequence `O_i = (I[i] + R(i, S, A, C)) % 256`, where `R(0, S, A, C) = S` and `R(i, S, A, C) = (A * R(i-1, S, A, C) + C) % 256` for `i >= 1`. The entropy is computed from the frequency distribution of the transformed values over the 256 possible byte values. If multiple parameter sets yield the same minimal entropy (within a tolerance of `1e-8`), return the lexicographically smallest set in the order `(S, A, C)`. The function should return a `std::tuple<int,int,int>` or similar structure containing `(res_S, res_A, res_C)`. The input array is 1-indexed as in the original snippet; your function should accept a zero-indexed vector (convert internally). Assume `N >= 1` and each `I[i]` is already in `[0,255]`.
*/

#include <vector>
#include <cmath>
#include <tuple>
#include <cstring>

// Given a zero-indexed vector of bytes (values 0-255) of length N,
// find (S,A,C) in [0,15]^3 minimizing the entropy of the transformed sequence.
// Returns a tuple (S,A,C) with lexicographically smallest on ties.
std::tuple<int,int,int> minimizeEntropy(const std::vector<int>& I) {
    int N = (int)I.size();
    const double EPS = 1e-8;
    const int MAX_RANGE = 256;
    
    double min_H = 1e100;
    int best_S = 0, best_A = 0, best_C = 0;
    
    // Precompute frequencies for each parameter set on the fly.
    int freq[256];
    
    for (int S = 0; S <= 15; ++S) {
        for (int A = 0; A <= 15; ++A) {
            for (int C = 0; C <= 15; ++C) {
                std::memset(freq, 0, sizeof(freq));
                
                int R = S; // R(1)
                for (int i = 0; i < N; ++i) {
                    int O = (I[i] + R) % MAX_RANGE;
                    freq[O]++;
                    if (i < N - 1) {
                        R = (A * R + C) % MAX_RANGE;
                    }
                }
                
                double entropy = 0.0;
                for (int k = 0; k < MAX_RANGE; ++k) {
                    if (freq[k] == 0) continue;
                    double p = (double)freq[k] / (double)N;
                    entropy -= p * std::log(p);
                }
                
                if (entropy < min_H - EPS) {
                    min_H = entropy;
                    best_S = S;
                    best_A = A;
                    best_C = C;
                }
            }
        }
    }
    
    return std::make_tuple(best_S, best_A, best_C);
}

#include <cassert>
#include <vector>
#include <tuple>

int main() {
    // Test 1: All zeros, N=3. Any transformation? R(1)=S. O=(0+S)%256. 
    // Frequencies: all same value, entropy 0. Lexicographically smallest (0,0,0) works.
    std::vector<int> I1 = {0,0,0};
    auto r1 = minimizeEntropy(I1);
    assert(r1 == std::make_tuple(0,0,0));

    // Test 2: Single value 255. Entropy 0 always. Smallest tuple.
    std::vector<int> I2 = {255};
    auto r2 = minimizeEntropy(I2);
    assert(r2 == std::make_tuple(0,0,0));

    // Test 3: Two distinct values 0 and 1. Original entropy = -2*(0.5*log0.5) = log2 ≈ 0.693.
    // If we pick S=0,A=0,C=0, R=0 always, O = [0,1], same entropy.
    // But maybe some combination reduces it? For N=2, entropy min is 0 if both become same? 
    // Try S=1: R(1)=1, O(1)=1. Then R(2)=1, O(2)=2. Still two distinct. So not 0.
    // Any (S,A,C) that makes both outputs equal? O1=0+S, O2=1+R(2) with R(2)=(A*S+C)%256.
    // Need S%256 == (1+(A*S+C)%256)%256 -> S == 1 + (A*S+C)%256 mod 256.
    // Since all small, hard. Let's just check that returned tuple satisfies constraints.
    std::vector<int> I3 = {0,1};
    auto r3 = minimizeEntropy(I3);
    assert(std::get<0>(r3) >= 0 && std::get<0>(r3) <= 15);
    assert(std::get<1>(r3) >= 0 && std::get<1>(r3) <= 15);
    assert(std::get<2>(r3) >= 0 && std::get<2>(r3) <= 15);

    // Test 4: A simple case where known optimal exists. 
    // I = [0, 256 - S]? Let's test I = [0, 255] with N=2. 
    // If we pick S=1: R(1)=1, O1=1. R(2)=(A*1+C)%256. Choose A=1,C=0 => R(2)=1, O2=(255+1)%256=0. So O=[1,0] two distinct.
    // Pick S=0,A=0,C=1: R(1)=0, O1=0. R(2)=1, O2=0. So O=[0,0] entropy 0! 
    // Verify: R(1)=S=0, O1=0+0=0. R(2)=(0*0+1)%256=1, O2=255+1=256%256=0. Yes.
    std::vector<int> I4 = {0,255};
    auto r4 = minimizeEntropy(I4);
    // There might be other combos giving 0, but lexicographically smallest should be (0,0,1)? Let's check.
    // Note that S=0,A=0,C=1 gives R(2)=1, O2=0, so entropy 0. Is there a smaller tuple also giving 0? 
    // (0,0,0) gives O1=0, R(2)=0, O2=255 -> two distinct, entropy >0. 
    // (0,0,1) is smallest with C=1. So assert equals (0,0,1).
    assert(r4 == std::make_tuple(0,0,1));

    // Test 5: Constant value repeated, N=5, I = {42,42,42,42,42}.
    std::vector<int> I5 = {42,42,42,42,42};
    auto r5 = minimizeEntropy(I5);
    assert(r5 == std::make_tuple(0,0,0));

    // Test 6: All distinct values 0..255 (N=256). Entropy max ~log(256)=5.545.
    // We can't easily predict min, but verify output is within range.
    std::vector<int> I6;
    for (int i = 0; i < 256; ++i) I6.push_back(i);
    auto r6 = minimizeEntropy(I6);
    assert(std::get<0>(r6) >= 0 && std::get<0>(r6) <= 15);
    assert(std::get<1>(r6) >= 0 && std::get<1>(r6) <= 15);
    assert(std::get<2>(r6) >= 0 && std::get<2>(r6) <= 15);

    // Test 7: N=1 with I[0]=128.
    std::vector<int> I7 = {128};
    auto r7 = minimizeEntropy(I7);
    assert(r7 == std::make_tuple(0,0,0));

    // Test 8: Verify that for I=[1,1,1,1] and N=4, entropy 0; (0,0,0) works.
    std::vector<int> I8 = {1,1,1,1};
    auto r8 = minimizeEntropy(I8);
    assert(r8 == std::make_tuple(0,0,0));

    // Test 9: A case where (0,0,1) is optimal similar to test 4 but with N=3: I={0,255,0}.
    // With S=0,A=0,C=1: R1=0,O1=0; R2=1,O2=0; R3=(0*1+1)%256=1,O3=1. So O=[0,0,1] entropy >0.
    // Maybe (0,0,0) gives O=[0,255,0] also >0. Both non-zero, so we just check bounds.
    std::vector<int> I9 = {0,255,0};
    auto r9 = minimizeEntropy(I9);
    assert(std::get<0>(r9) >= 0 && std::get<0>(r9) <= 15);

    return 0;
}

// The problem requires brute-force search over all 16^3 = 4096 parameter combinations. For each combination, we compute the transformed sequence by iterating through the input and updating a recurrence. The recurrence `R(i)` can be computed incrementally: start with `R = S` for `i=1`, then for each subsequent `i`, update `R = (A * R + C) % 256`. For each transformed value `O_i = (I[i] + R) % 256`, we increment a frequency counter of size 256. After processing all N elements, compute entropy as `-Σ (freq[k]/N) * log(freq[k]/N)` for nonzero frequencies, using natural log (the result is the same regardless of log base; here we use `log` which is natural log). We track the minimum entropy; when a new minimum is found that is strictly less than the current best by more than `1e-8`, we update the best parameters. Since we iterate S, A, C in increasing order, the first encountered minimum will be lexicographically smallest, so no tie-breaking is needed beyond strict inequality. Edge cases: N=1, all identical values (entropy 0), and when no transformation reduces entropy (still output a valid set). Time complexity: O(4096 * N) because for each of 4096 combos we scan N elements and a constant 256 for entropy. Space complexity: O(256) for frequency counts plus O(N) for the input vector (if passed by reference, O(1) extra). The recurrence uses only a few scalars, so effectively O(1) auxiliary beyond the input.
