// Write a C++ function that computes, for each query (n, k), the number of pairs (i, j) with 0 ≤ i ≤ n, 0 ≤ j ≤ min(i, k) such that C(i, j) is divisible by a given modulus m. The function takes three parameters: the number of queries q, the modulus m, and a vector of pairs of integers representing the queries (n_i, k_i). It must return a vector of long long integers, one per query, representing the counts. All n_i and k_i are between 0 and 2000 inclusive, and m is between 2 and 10^9. The function should precompute all necessary data up to the maximum n across all queries in a single pass, then answer each query in O(1) after preprocessing. The final answer for each query must be accurate even when m is not prime, as long as m ≥ 2.
// The problem is a classic combination of Pascal’s triangle (Yang Hui triangle) for modular binomial coefficients and a 2D prefix sum to count zero residues. Build a table `C[i][j]` for 0 ≤ i ≤ 2000, 0 ≤ j ≤ i, where `C[i][j] = C(i,j) mod m`. Compute iteratively: `C[i][0]=1` for all i, and for j>0: `C[i][j] = (C[i-1][j] + C[i-1][j-1]) % m`. Simultaneously, build a prefix sum table `pref[i][j]` where `pref[i][j]` = number of zeros among C(a,b) for all 0 ≤ a ≤ i and 0 ≤ b ≤ min(a, j). This can be computed using the recurrence: `pref[i][j] = pref[i][j-1] + pref[i-1][j] - pref[i-1][j-1] + (C[i][j]==0 ? 1 : 0)`, with careful handling of boundaries. For each row i, after filling all j from 0 to i, set `pref[i][i+1] = pref[i][i]` so that queries with k > i (which is allowed because k_i can be larger than n_i) are answered correctly by indexing with `j = min(k, i)`. To answer a query (n,k), if k > n, we only consider j up to n, so return `pref[n][min(k,n)]`. Edge cases: n=0 or k=0, where the only binomial is C(0,0)=1, which is divisible by m only if m=1 (but m≥2), so count is 0. Preprocessing is O(2000^2) = 4 million operations, and each query is O(1), so total time is O(2000^2 + q). Space is O(2000^2) for the two tables, which is about 8 MB per table if using int, but using long long for counts is safer (up to ~2 million per query, fits in int but long long is fine). The modulus may be up to 1e9, and intermediate sums may exceed int, so use `long long` for the C table and prefix sums to avoid overflow, and cast to long long when adding.
#include <vector>
#include <algorithm>

// Precompute Pascal's triangle modulo m and 2D prefix sums of zero entries.
// Returns vector of answers for each query in 'queries'.
std::vector<long long> countDivisibleBinomials(
    int q, 
    int m, 
    const std::vector<std::pair<int, int>>& queries) {
    
    const int maxN = 2000;
    // C[i][j] = C(i,j) mod m, for 0 <= i <= maxN, 0 <= j <= i
    // pref[i][j] = number of zeros among C(a,b) for 0 <= a <= i, 0 <= b <= min(a,j)
    // Use long long for safety.
    std::vector<std::vector<long long>> C(maxN + 1, std::vector<long long>(maxN + 2, 0));
    std::vector<std::vector<long long>> pref(maxN + 1, std::vector<long long>(maxN + 2, 0));
    
    // Initialize row 0 and row 1 of C
    C[0][0] = 1 % m;  // only if m > 1, this is 1
    if (maxN >= 1) {
        C[1][0] = 1 % m;
        C[1][1] = 1 % m;
    }
    // Build Pascal's triangle and prefix sums
    for (int i = 0; i <= maxN; ++i) {
        // For each row, we need to fill columns 0..i, and also pref up to i+1
        // Set C[i][0] = 1 if i>0 (already done for i=0,1)
        if (i >= 2) {
            C[i][0] = 1 % m;
        }
        // We'll compute C[i][j] for j=1..i, and simultaneously pref
        for (int j = 0; j <= i; ++j) {
            if (j == 0) {
                // C[i][0] already set
                // pref[i][0] = pref[i-1][0] + (C[i][0]==0)
                if (i > 0) {
                    pref[i][0] = pref[i-1][0] + (C[i][0] == 0 ? 1 : 0);
                } else {
                    pref[0][0] = (C[0][0] == 0 ? 1 : 0);  // C(0,0)=1, so 0
                }
            } else {
                // Compute C[i][j] using Pascal's rule
                if (j == i) {
                    C[i][j] = 1 % m;  // C(i,i)=1
                } else {
                    // i >= 2 and j between 1 and i-1
                    C[i][j] = (C[i-1][j] + C[i-1][j-1]) % m;
                }
                // Update pref[i][j] using 2D prefix recurrence
                // pref[i][j] = pref[i][j-1] + pref[i-1][j] - pref[i-1][j-1] + (C[i][j]==0)
                pref[i][j] = pref[i][j-1] + pref[i-1][j] - pref[i-1][j-1] + (C[i][j] == 0 ? 1 : 0);
            }
        }
        // Set pref[i][i+1] = pref[i][i] for queries with k > i
        pref[i][i+1] = pref[i][i];
    }
    
    // Answer queries
    std::vector<long long> answers;
    answers.reserve(q);
    for (const auto& query : queries) {
        int n = query.first;
        int k = query.second;
        // k can be > n, but we only have up to column n
        int col = std::min(k, n);
        answers.push_back(pref[n][col]);
    }
    return answers;
}
#include <cassert>
#include <vector>
#include <utility>

// The solution function is declared above (or included via header).

int main() {
    // Test 1: basic small case, m=2
    {
        int q = 3;
        int m = 2;
        std::vector<std::pair<int,int>> queries = {{2,2}, {3,1}, {0,0}};
        auto ans = countDivisibleBinomials(q, m, queries);
        // C(0,0)=1, C(1,0)=1, C(1,1)=1, C(2,0)=1, C(2,1)=2 mod2=0, C(2,2)=1
        // For (2,2): zeros among rows 0..2, cols up to min(row,2): C(2,1)=0 -> 1
        // For (3,1): rows 0..3, cols up to min(row,1): zeros: C(2,1)=0, C(3,1)=3 mod2=1? no zero, so only 1? Wait C(3,1)=3 mod2=1, so total zeros = 1.
        // For (0,0): only C(0,0)=1, no zero -> 0
        assert(ans[0] == 1);
        assert(ans[1] == 1);
        assert(ans[2] == 0);
    }
    // Test 2: m=3, simple
    {
        int q = 2;
        int m = 3;
        std::vector<std::pair<int,int>> queries = {{3,2}, {4,4}};
        auto ans = countDivisibleBinomials(q, m, queries);
        // Compute manually:
        // Row0: 1
        // Row1: 1,1
        // Row2: 1,2,1 (mod3: 1,2,1)
        // Row3: 1,3,3,1 (mod3: 1,0,0,1) -> zeros at (3,1) and (3,2)
        // Row4: 1,4,6,4,1 (mod3: 1,1,0,1,1) -> zero at (4,2) only
        // Query (3,2): rows 0..3, cols up to min(row,2):
        //   Row0: col0:1 (not zero)
        //   Row1: col0,1: 1,1
        //   Row2: col0,1,2: 1,2,1
        //   Row3: col0,1,2: 1,0,0 -> two zeros
        // Total zeros = 2
        // Query (4,4): rows 0..4, cols up to min(row,4) which is all cols:
        //   Row3: zeros at (3,1),(3,2) =2
        //   Row4: zero at (4,2) =1
        // Total = 3
        assert(ans[0] == 2);
        assert(ans[1] == 3);
    }
    // Test 3: k > n case
    {
        int q = 1;
        int m = 5;
        std::vector<std::pair<int,int>> queries = {{2,10}};
        auto ans = countDivisibleBinomials(q, m, queries);
        // Rows 0..2: 
        // Row0: 1
        // Row1: 1,1
        // Row2: 1,2,1 (mod5: 1,2,1) no zeros
        // Since k>n, we consider up to col=n=2, no zeros -> 0
        assert(ans[0] == 0);
    }
    // Test 4: larger random check with known value
    {
        int q = 1;
        int m = 2;
        std::vector<std::pair<int,int>> queries = {{4,4}};
        auto ans = countDivisibleBinomials(q, m, queries);
        // Pascal rows modulo 2:
        // Row0:1
        // Row1:1,1
        // Row2:1,0,1 -> zero at (2,1)
        // Row3:1,1,1,1 -> all 1 mod2
        // Row4:1,0,0,0,1 -> zeros at (4,1),(4,2),(4,3) -> 3 zeros
        // Total zeros: row2 has 1, row4 has 3 => 4
        assert(ans[0] == 4);
    }
    // Test 5: m=100, no zeros except maybe large numbers (but note C(2000,1000) > 100, may be zero mod 100)
    {
        int q = 2;
        int m = 100;
        std::vector<std::pair<int,int>> queries = {{1,1}, {2,2}};
        auto ans = countDivisibleBinomials(q, m, queries);
        // Row0:1
        // Row1:1,1
        // Row2:1,2,1 -> none divisible by 100
        // Query (1,1): zeros? none -> 0
        // Query (2,2): zeros? none -> 0
        assert(ans[0] == 0);
        assert(ans[1] == 0);
    }
    return 0;
}
