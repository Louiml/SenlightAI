// Write a C++ function `countWalks(long long n, int k)` that returns the number of walks of exactly `n` steps on a directed graph with `2k + 2` vertices (labeled `0` to `2k+1`). The graph is defined as follows: for every `0 ≤ i ≤ k` and every `0 ≤ j ≤ i`, there is an edge from vertex `i` to vertex `j`, and an edge from vertex `i` to vertex `j + k + 1` weighted by `2^(i-j)` (mod `1e9+7`). Additionally, for every `0 ≤ i ≤ k`, there is an edge from vertex `i + k + 1` to vertex `i`. Finally, there is a self‑loop on vertex `2k+1` and an edge from vertex `2k+1` to vertex `k`. The answer must be computed modulo `1e9+7`, considering that the number of edges is allowed to be huge (n up to `1e18`). Return the sum of the number of walks from vertex `2k+1` to vertex `i` for all `0 ≤ i ≤ k+1` after exactly `n` steps.

This is a classic matrix exponentiation problem. The graph has `size = 2k + 2` vertices, and transitions are represented by an adjacency matrix `T` where `T[a][b]` is the number of ways (and the weight for weighted edges) to go from `a` to `b` in one step. We build `T`:
- For `0 ≤ i, j ≤ i`, set `T[i][j] = C(i,j)` (binomial coefficient) and `T[i][j+k+1] = C(i,j) * 2^(i-j) mod mod`.
- For `0 ≤ i ≤ k`, set `T[i+k+1][i] = 1`.
- Set `T[2k+1][2k+1] = 1` and `T[2k+1][k] = 1`.

Then `T^n` (after exponentiation) gives the number of walks of length `n` between every pair of vertices. The answer is the sum of row `2k+1` for columns `0` through `k+1` inclusive (vertices `0..k` and the extra vertex `k+1`? Actually `k+1` is not a separate vertex; Adjointly, the original snippet uses `size = (k+1)*2 + 1`, meaning vertices `0..2k+1` (since size is `2k+3`? Wait `size = (k+1)*2 + 1 = 2k+3`? Carefully: `size = (k+1)*2 + 1 = 2k+3`? Actually `(k+1)*2 + 1 = 2k+3`, but the code uses that as the dimension. However, the vertices are `0 .. size-1`. The code builds transitions for `i=0..k` and `i+k+1` up to `2k+1`. So vertex indices `0..k` (first block), `k+1..2k+1` (second block), plus vertex `size-1 = 2k+2`? Wait `size = (k+1)*2 + 1` is odd. Let's compute: `(k+1)*2 + 1 = 2k+3`. So vertices `0..2k+2`. The extra vertex is `2k+2`. The snippet initializes `size = (k+1)*2 + 1` and uses `tran[size-1][size-1] = 1` and `tran[size-1][k] = 1`. So the last vertex (index `2k+2`) has a self‑loop and an edge to vertex `k`. The answer sums `tran[size-1][i]` for `i=0..k+1`? But `k+1` is inside the second block? Actually the sum loop is `for (int i = 0; i <= k + 1; ++i)` summing row `size-1`. But `k+1` is a vertex index (the first of the second block). So the answer is the total number of walks from the last vertex to any vertex among `0` through `k+1` after exactly `n` steps.

For the task, we adopt exactly that graph definition: vertices `0` to `2k+2` (i.e., `size = 2k+3`). The last vertex (index `2k+2`) has a self‑loop and an edge to vertex `k`. We need to compute the sum of walks from vertex `2k+2` to vertices `0` through `k+1` inclusive.

We use matrix exponentiation with exponent `n`. The matrix size is `O(k)`. Each multiplication is `O(k^3)` but optimized using the standard triple loop. Since `k` can be up to around 40 (maxs=83, but given typical constraints, we can assume `k ≤ 40` such that `size ≤ 83`), and `n` up to `1e18`, the total complexity is `O((2k+3)^3 * log n)`. That is about `O(k^3 log n)`. Space is `O(k^2)` for the matrices. Edge cases: `n=0`? The problem likely expects `n>0`? But we handle `n=0` as identity matrix, so the answer would be 1 if vertex `2k+2` is among the target set? Actually the sum includes vertex `2k+2`? The loop `i=0..k+1` does not include `2k+2` because `k+1` is less than `2k+2` for `k ≥ 0`. So for `n=0`, the answer is `1` if `0` is in the set? Wait, row `2k+2` of identity is 1 at column `2k+2` only, so sum would be 0. That might be correct: zero steps means no walk to those vertices. We can allow `n ≥ 0`. The code uses `long long`.

Also need to precompute binomial coefficients up to `k` and powers of 2 modulo mod. Multiplication must be taken modulo `1e9+7`. Use `long long` for intermediate.

The solution function should be `int countWalks(long long n, int k)` returning the answer modulo `1e9+7`.

#include <vector>
#include <cstring>

const int MOD = 1000000007;

// Binomial coefficients up to n
std::vector<std::vector<int>> binom;

// Precompute binomial coefficients for all i up to n
void precompute_binom(int n) {
    binom.assign(n + 1, std::vector<int>(n + 1, 0));
    for (int i = 0; i <= n; ++i) {
        binom[i][0] = binom[i][i] = 1;
        for (int j = 1; j < i; ++j) {
            binom[i][j] = (binom[i-1][j-1] + binom[i-1][j]) % MOD;
        }
    }
}

// Matrix multiplication: res = a * b, both size x size, under modulo MOD
void mat_mul(const std::vector<std::vector<int>>& a,
             const std::vector<std::vector<int>>& b,
             std::vector<std::vector<int>>& res) {
    int sz = (int)a.size();
    // Use long long and accumulate to reduce mod operations
    std::vector<std::vector<long long>> tmp(sz, std::vector<long long>(sz, 0));
    for (int i = 0; i < sz; ++i) {
        for (int k = 0; k < sz; ++k) {
            if (a[i][k] != 0) {
                long long aik = a[i][k];
                for (int j = 0; j < sz; ++j) {
                    tmp[i][j] += aik * b[k][j];
                    if (tmp[i][j] >= (long long)MOD * MOD) tmp[i][j] -= (long long)MOD * MOD;
                }
            }
        }
    }
    for (int i = 0; i < sz; ++i) {
        for (int j = 0; j < sz; ++j) {
            res[i][j] = tmp[i][j] % MOD;
        }
    }
}

// Matrix exponentiation: returns a^p
std::vector<std::vector<int>> mat_pow(std::vector<std::vector<int>> a, long long p) {
    int sz = (int)a.size();
    std::vector<std::vector<int>> res(sz, std::vector<int>(sz, 0));
    for (int i = 0; i < sz; ++i) res[i][i] = 1;
    while (p > 0) {
        if (p & 1) {
            std::vector<std::vector<int>> tmp = res;
            mat_mul(tmp, a, res);
        }
        std::vector<std::vector<int>> tmp = a;
        mat_mul(tmp, a, a);
        p >>= 1;
    }
    return res;
}

// Main solution function: countWalks(n, k)
int countWalks(long long n, int k) {
    int size = 2 * k + 3;  // vertices 0..2k+2
    precompute_binom(k);

    // Build transition matrix
    std::vector<std::vector<int>> trans(size, std::vector<int>(size, 0));

    // First block (i -> j and i -> j+k+1)
    for (int i = 0; i <= k; ++i) {
        for (int j = 0; j <= i; ++j) {
            trans[i][j] = binom[i][j];  // edge i->j
            // edge i->j+k+1 with weight 2^(i-j)
            long long pow2 = 1;
            for (int p = 0; p < i - j; ++p) pow2 = (pow2 * 2) % MOD;
            trans[i][j + k + 1] = (long long)binom[i][j] * pow2 % MOD;
        }
    }

    // Second block to first block: (i+k+1) -> i
    for (int i = 0; i <= k; ++i) {
        trans[i + k + 1][i] = 1;
    }

    // Special vertex 2k+2: self loop and edge to k
    int last = size - 1;
    trans[last][last] = 1;
    trans[last][k] = 1;

    // Matrix exponentiation
    std::vector<std::vector<int>> powered = mat_pow(trans, n);

    // Sum row 'last' from column 0 to k+1
    int ans = 0;
    for (int i = 0; i <= k + 1; ++i) {
        ans = (ans + powered[last][i]) % MOD;
    }
    return ans;
}

#include <cassert>

// Assume countWalks is declared from the solution above
int countWalks(long long n, int k); // prototype

int main() {
    // Test small cases by brute-force simulation for n=1, k=0,1,2
    // k=0: size=3, vertices 0,1,2. Edges: from 0: 0->0 (binom[0][0]=1), 0->1? Actually j=0, j+k+1=1, weight 2^(0-0)=1. So 0->0 and 0->1. Also 1->0 (since i=0, i+k+1=1). Vertex2 self-loop and 2->0 (since k=0). So matrix:
    // [1,1,0]
    // [1,0,0]
    // [1,0,1]
    // For n=1, row2 (index2) sums cols 0..1: from 2->0 and 2->2? Actually 2->2 self-loop, but we sum only 0..k+1=0..1, so 2->0=1, 2->? no edge to 1, so answer=1.
    assert(countWalks(1, 0) == 1);
    // n=2: walks from 2: step1 to 0, step2: from 0 to 0 or 1, also from 2->2->? Actually 2->2 (self), then 2->0 next? Let's compute: T^2 row2: (2->0)=1, then 0->0 and 0->1 both 1, so contributes 1*1+1*1=2? plus 2->2 (1) then 2->0 (1) ->1, total to col0=1+1=2? plus to col1=1. Sum=3. Also 2->2->? no. So answer=3. Let's test:
    assert(countWalks(2, 0) == 3);

    // k=1: size=5, vertices 0..4. Verify n=1: from 4 (last) edges to 4 self, to k=1. So row4 has 1 at col4 and col1. Sum over 0..2 (k+1=2): includes col1 =1, col2? no, col0? no. So answer=1.
    assert(countWalks(1, 1) == 1);
    // n=2: from 4 to 1, then from 1: edges to 0,0? Actually 1->0 (j=0) and 1->2 (j=0+k+1=2), also 1->1? j=i=1 gives 1->1 and 1->1? Wait for i=1, j can be 0 and 1. So edges: 1->0, 1->1 (binom[1][1]=1), 1->2 (weight 2^(1-0)=2), 1->1? also j=1 gives 1->1? Actually j=1: trans[1][1]=binom[1][1]=1 and trans[1][1+2=3]? j+k+1=1+2=3, weight 2^(0)=1. So 1->1 and 1->3. Also 1->? from 1->0 (j=0) and 1->2 (j=0, j+k+1=2). So from 1: to 0,1,2,3? Wait j=1 gives to 1 and to 3. So yes. From 4 to 1 then step2 to: 0,1,2,3. Sum over 0..2: includes 0,1,2 -> that's 3. Also 4->4->? self-loop, then 4->1 next step? Actually n=2: path 4->4 (self) then 4->1, giving a walk to 1 as well. Also 4->1 then 1->... So total sum row4 to cols 0..2: let's compute directly: T = 
    // 0: [1,1,0,0,0]? Actually 0->0 (j=0), 0->1 (j=0,k+1=1), so col0=1, col1=1.
    // 1: 1->0,1->1,1->2,1->3.
    // 2 (i=1+k+1=2): 2->1
    // 3 (i=2+k+1=3): 3->1? Actually for i=1, i+k+1=2, so 2->1; for i=0? No, only i=0..k=1, so 1->? Wait second block: for i=0: 1->0? actually i=0 gives i+k+1=1 so 1->0. For i=1: 2->1. So vertex2->1, vertex3->? none? Actually vertex3 is not in second block? second block indices are 1 and 2 (i+k+1 for i=0,1). So vertex3 is not part of that. But we have size=5, vertices 0..4. Vertex4 is last. So matrix:
    // row0: [1,1,0,0,0]
    // row1: [1,1,1,1,0]  (since j=0,1: to 0,1,2,3)
    // row2: [0,1,0,0,0]
    // row3: all 0
    // row4: [0,0,1,0,1] (to k=1 at col1, and self at col4)
    // For n=2, row4^2: compute (row4 * T): row4 = [0,1,0,0,1]. Multiply by T:
    // col0: 1*T[1][0]=1*1=1
    // col1: 1*T[1][1]=1*1=1
    // col2: 1*T[1][2]=1*1=1
    // col3: 1*T[1][3]=1*1=1
    // col4: 1*T[1][4]=0 + 1*T[4][4]=1*1=1
    // So row4^2 = [1,1,1,1,1]? Wait also 1*T[1][...] plus 1*T[4][...] = 1*0 + 1*0=0 for col0? Actually T[4][0]=0, so from row4[1]=1 times T[1][0]=1 gives 1. So yes: [1,1,1,1,1]. Sum over cols 0..2 (k+1=2): 1+1+1=3. So answer=3.
    assert(countWalks(2, 1) == 3);

    // n=0: any k, sum of identity row last to cols 0..k+1 is 0 because last not in that set.
    assert(countWalks(0, 1) == 0);

    // Large n, k=2: we can brute for n=3 if we trust manual? Instead compare with a small brute-force simulation for k=2, n=3 using a simple DP.
    // We'll implement a quick brute here for verification.
    {
        int k = 2;
        long long n = 3;
        int size = 2*k+3; // 7
        // Build adjacency matrix as per spec
        std::vector<std::vector<int>> adj(size, std::vector<int>(size,0));
        // compute binom
        std::vector<std::vector<int>> bc(k+1, std::vector<int>(k+1,0));
        for (int i=0;i<=k;++i){bc[i][0]=bc[i][i]=1;for(int j=1;j<i;++j)bc[i][j]=bc[i-1][j-1]+bc[i-1][j];}
        for (int i=0;i<=k;++i){
            for (int j=0;j<=i;++j){
                adj[i][j] = bc[i][j];
                int pow2=1;
                for(int p=0;p<i-j;++p)pow2=(pow2*2)%MOD;
                adj[i][j+k+1] = (bc[i][j]*pow2)%MOD;
            }
        }
        for (int i=0;i<=k;++i) adj[i+k+1][i]=1;
        int last=size-1;
        adj[last][last]=1;
        adj[last][k]=1;
        // DP for n steps
        std::vector<long long> dp(size,0);
        dp[last]=1;
        for (long long step=0; step<n; ++step){
            std::vector<long long> ndp(size,0);
            for(int a=0;a<size;++a) if(dp[a]){
                for(int b=0;b<size;++b) if(adj[a][b]){
                    ndp[b] = (ndp[b] + dp[a]*adj[a][b]) % MOD;
                }
            }
            dp = ndp;
        }
        int ans=0;
        for(int i=0;i<=k+1;++i) ans=(ans+dp[i])%MOD;
        assert(countWalks(n,k)==ans);
    }

    return 0;
}
