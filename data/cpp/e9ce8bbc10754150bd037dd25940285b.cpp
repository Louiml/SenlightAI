/*
Write a C++ function `findBestRatio` that takes three integers `A`, `B`, and `L` (where `A` and `B` are positive, `L` is a positive limit, and `A/B` is not necessarily reduced). The function must find the fraction `i/j` (with `1 ≤ i ≤ L` and `1 ≤ j ≤ L`) that is in lowest terms (i.e., `gcd(i,j) == 1`) and is the smallest such fraction that is greater than or equal to `A/B`. If multiple fractions satisfy this, choose the one with the smallest positive difference from `A/B` (measured as `i*B - j*A`). Finally, return a `std::pair<int,int>` containing the chosen `i` (numerator) and `j` (denominator). Assume `A`, `B`, and `L` are all positive integers, and that at least one valid fraction exists within the range (this is guaranteed because `L ≥ 1` and the fraction `1/1` is always available, but ensure your function handles the case where `A/B > 1` properly).
*/
#include <utility>
#include <numeric>

// Find the smallest reduced fraction i/j (1 <= i,j <= L) that is >= A/B.
// Return the pair (i, j).
std::pair<int, int> findBestRatio(int A, int B, int L) {
    int bestNum = 1;  // Default to first valid candidate
    int bestDen = 1;
    int bestDiff = -1;  // Use -1 to indicate uninitialized

    for (int i = 1; i <= L; ++i) {
        for (int j = 1; j <= L; ++j) {
            if (std::gcd(i, j) != 1) continue;  // Must be in lowest terms

            // Check i/j >= A/B using cross-multiplication
            if (i * B >= j * A) {
                int diff = i * B - j * A;
                if (bestDiff == -1 || diff < bestDiff) {
                    bestDiff = diff;
                    bestNum = i;
                    bestDen = j;
                }
            }
        }
    }
    return {bestNum, bestDen};
}
#include <cassert>
#include <utility>

// The solution function is declared above; include it here implicitly.

int main() {
    // Example from the original problem: A=1, B=3, L=5 -> best is 2/5? Let's compute manually:
    // Reduced fractions with denom <=5: 1/1,1/2,1/3,1/4,1/5,2/1,2/3,2/5,3/1,3/2,3/4,3/5,4/1,4/3,4/5,5/1,5/2,5/3,5/4,5/5(gcd=5 not 1)...
    // >= 1/3: smallest is 1/3 itself (diff=0). So expected (1,3).
    assert(findBestRatio(1, 3, 5) == std::make_pair(1, 3));

    // A=2, B=5, L=10. 2/5 = 0.4. Check fractions >=0.4: 1/2 (0.5) diff=0.5*10? Actually i*B - j*A = 1*5-2*2=1; 2/5 is not reduced? gcd(2,5)=1, so that itself is valid and diff=0. So expected (2,5).
    assert(findBestRatio(2, 5, 10) == std::make_pair(2, 5));

    // A=3, B=2, L=3. 3/2=1.5. Fractions with denom<=3 and num<=3, reduced: (1,1)=1 diff=1*2-1*3? Actually 1/1<1.5, not valid. (2,1)=2 diff=2*2-1*3=1; (3,1)=3 diff=3*2-1*3=3; (3,2) not reduced? gcd=1, so 1.5 diff=0. So expected (3,2).
    assert(findBestRatio(3, 2, 3) == std::make_pair(3, 2));

    // A=4, B=1, L=2. 4/1=4. Only fractions with num<=2, den<=2: max is 2/1=2, which is <4, so no valid fraction? But L=2 gives max 2, so condition fails? Actually the problem guarantees at least one valid, but here none is >=4. But our function will just return whatever bestDiff was set to (first valid). First valid when i=1,j=1? 1>=4? false. i=1,j=2? 0.5? false. i=2,j=1? 2>=4? false. i=2,j=2? gcd=2 not 1. So bestDiff remains -1, returns (1,1). This is a limitation; but test assumptions say there is always a valid one. So let's skip this test.

    // Test with larger L to ensure correct when ratio >1 but within range.
    // A=5, B=1, L=5. Need fraction >=5. Only 5/1 itself is >=5, and gcd=1. So expected (5,1).
    assert(findBestRatio(5, 1, 5) == std::make_pair(5, 1));

    // Test with L=1, any A=B gives 1/1 is >=1 if A>=B, else no valid? But we have guaranteed. For A=1,B=1,L=1 => (1,1).
    assert(findBestRatio(1, 1, 1) == std::make_pair(1, 1));

    // Test where A/B is between two reduced fractions, pick the smallest diff.
    // A=1, B=2, L=4. Fractions: 1/2 itself (diff=0) so (1,2).
    assert(findBestRatio(1, 2, 4) == std::make_pair(1, 2));

    // Test where A/B=1/3 but L=2, no 1/3 because denom must be <=2 and gcd(i,j)=1. Fractions: 1/1 (1), 1/2 (0.5), 2/1 (2) all >=1/3? 1/1 diff=1*3-1*1=2, 1/2 diff=1*3-2*1=1, 2/1 diff=2*3-1*1=5. Smallest diff is 1 from (1,2). So expected (1,2).
    assert(findBestRatio(1, 3, 2) == std::make_pair(1, 2));
}
// The brute-force approach enumerates all pairs `(i, j)` with `1 ≤ i ≤ L` and `1 ≤ j ≤ L`. For each pair, check if `gcd(i, j) == 1` to enforce the lowest-terms condition. Then check if `i/j >= A/B` by comparing cross-multiplication: `i*B >= j*A`. Among all valid candidates, we need the smallest fraction that is at least `A/B`, which means minimizing the difference `i*B - j*A` (since `i*B - j*A >= 0`). If multiple pairs yield the same minimum difference, the code as provided picks the first encountered in the double loop (since it uses `>` rather than `>=`), which is acceptable. Edge cases: `A/B` could be greater than 1, but the loop covers values up to `L`; if `A/B` is very large and `L` is small, the smallest fraction >= `A/B` might be `L/L = 1`, but only if `gcd(L,L)=L` which is not 1 unless `L=1`, so we must rely on the enumeration to find a valid pair (e.g., for `L=1`, only `(1,1)` works, which is fine). Time complexity is `O(L^2 log L)` due to the gcd computation inside the nested loops (each gcd is `O(log(min(i,j)))`). Space complexity is `O(1)` ignoring the input and output.
