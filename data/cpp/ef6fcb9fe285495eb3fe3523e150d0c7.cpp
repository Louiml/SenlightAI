// Write a C++ function `int countZeroGrundyGraphs(int n, const std::vector<std::pair<int,int>>& edges)` that, given a directed acyclic graph with vertices numbered `1` through `n` and a list of directed edges, computes the number of ways to choose a lucky vertex set such that the XOR of the Grundy numbers of all chosen vertices is zero. In this game, each vertex has a Grundy number computed via the standard Sprague–Grundy theorem for a directed graph (mex of the Grundy numbers of its outgoing neighbors), and the game is played on a single token that moves along edges; the XOR sum of Grundy numbers of chosen vertices must be zero. Since the answer can be huge, return it modulo `998244353`. The graph may be disconnected, but it is guaranteed to be acyclic. The function must handle up to `n = 1e5` vertices and `m = 1e5` edges, and the Grundy numbers of vertices will be at most `511` (since the maximum degree and paths are bounded) — but your algorithm should reliably compute Grundy numbers using DFS and memoization.
The solution first computes the Grundy number for every vertex using a depth-first search with memoization on a directed acyclic graph. For each vertex, we collect the Grundy numbers of its outgoing neighbors, sort them, and take the smallest non-negative integer not present (mex). This is `O(m log m)` in total due to sorting per vertex, but since total edges is `m`, overall `O(m log m)`; in practice, sorting small vectors is efficient. After that, we need to count subsets (i.e., selecting a set of vertices) where the XOR of their Grundy numbers equals zero. However, the problem asks for the number of ways to choose a vertex set such that the XOR of Grundy numbers of chosen vertices is zero. This is essentially counting subsets of vertices whose XOR-sum is 0, modulo `998244353`. That is a linear algebra problem over GF(2) with weights modulo a prime. We build a system of linear equations: for each vertex `j`, we add a variable `x_j` that is 0 or 1 indicating whether vertex `j` is selected. The condition is that for each possible Grundy value `g` (0..511), the number of selected vertices with Grundy value `g` must have even parity? Actually no, XOR of Grundy numbers of selected vertices is zero means the XOR of the Grundy values (as integers) is 0. This is not a linear condition over GF(2) on the selection variables unless we treat the Grundy numbers as bit vectors. We can model each bit position independently: for each bit `b` from 0 to 8 (since max Grundy <= 511), the parity of selected vertices whose Grundy has bit `b` set must be even. That gives 9 linear equations over GF(2) on the `n` binary variables. The number of solutions to a homogeneous system of linear equations over GF(2) is `2^(n - rank)`, where rank is the rank of the coefficient matrix. However, we must be careful: the problem likely expects counting subsets of vertices such that XOR of their Grundy numbers equals zero, and this count is `2^(n - rank)` mod `MOD`. But the given code uses a Gaussian elimination on a matrix of size 512x512 with modular arithmetic, effectively computing something like the probability that a random subset has XOR zero, then multiplies by `2^n`. Actually the code builds a 512x512 matrix where each row `i` corresponds to the linear transformation on the XOR of Grundy numbers: it defines a Markov chain on the current XOR state, and solves for the expected probability of ending at 0 after choosing each vertex independently with probability `1/(n+1)`. That is more complex. Let me reinterpret: The code computes the probability that if we pick each vertex independently with probability `p = 1/(n+1)`, the XOR of Grundy numbers of picked vertices is zero, then computes the probability that a random subset has XOR zero. That probability equals the number of subsets with XOR zero divided by `2^n`. But the code outputs `1 - mat[0][MAQ+1]` where `mat[0][MAQ+1]` is the probability that starting from XOR 0 and applying each vertex with probability `p` ends at XOR 0? Actually the matrix is a linear system for the expected value of something. Let's simplify: The task is probably to count subsets with XOR zero modulo a prime. For that, we can use Gaussian elimination over GF(2) with 9 bits, but the number of subsets is `2^(n - rank)` mod `998244353`. However, `n` can be 1e5, so we need to compute rank of an n x 9 matrix over GF(2), which is simple: for each bit position, create a vector of length n (but we only need rank). Since there are only 9 bits, the rank is at most 9. So the number of subsets with XOR zero is `2^(n - rank)` mod MOD. But we must be careful: the XOR of Grundy numbers can be any integer up to 511, but bits 0..8 suffice. So we can write a linear system `A * x = 0` over GF(2), where `A` is a 9 x n matrix, and the number of solutions is `2^(n - rank(A))` because homogeneous system. That is correct. Edge cases: if no vertices, n=0? The problem likely has n>=1. For disconnected graphs, DFS computes Grundy numbers correctly. Time complexity: O(m log m) for Grundy due to sorting, plus O(9n) for building the matrix, and Gaussian elimination over GF(2) on 9 rows and n columns is O(9^2 * n) ~ O(81n) which is fine. Space: O(n + m) for graph and Grundy. The modulo is prime, so exponentiation is straightforward. I will design the function to compute Grundy numbers via DFS with memoization, then build a 9 x n binary matrix, perform Gaussian elimination to find rank, and return `pow_mod(2, n - rank, MOD)`. This matches the spirit of the original code but is simpler and correct for counting subsets with XOR zero.
#include <vector>
#include <algorithm>
#include <functional>
#include <cstdint>

const long long MOD = 998244353;

long long mod_pow(long long base, long long exp, long long mod) {
    long long result = 1;
    while (exp > 0) {
        if (exp & 1) result = (result * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }
    return result;
}

// Counts the number of subsets of vertices whose XOR of Grundy numbers is 0.
// n: number of vertices (1-indexed)
// edges: directed edges (u, v) meaning u -> v
int countZeroGrundyGraphs(int n, const std::vector<std::pair<int,int>>& edges) {
    std::vector<std::vector<int>> graph(n + 1);
    for (const auto& e : edges) {
        graph[e.first].push_back(e.second);
    }

    std::vector<int> grundy(n + 1, -1);
    std::vector<bool> visited(n + 1, false);

    // DFS to compute Grundy numbers
    std::function<void(int)> dfs = [&](int u) {
        visited[u] = true;
        for (int v : graph[u]) {
            if (!visited[v]) dfs(v);
        }
        // mex of outgoing neighbors' Grundy numbers
        std::vector<int> neighbor_grundy;
        for (int v : graph[u]) {
            neighbor_grundy.push_back(grundy[v]);
        }
        std::sort(neighbor_grundy.begin(), neighbor_grundy.end());
        int mex = 0;
        for (int g : neighbor_grundy) {
            if (g == mex) ++mex;
            else if (g > mex) break;
        }
        grundy[u] = mex;
    };

    for (int i = 1; i <= n; ++i) {
        if (!visited[i]) dfs(i);
    }

    // Build a 9 x n binary matrix over GF(2).
    // Column j corresponds to vertex j; each row is a bit position (0..8).
    std::vector<std::vector<int>> matrix(9, std::vector<int>(n, 0));
    for (int j = 1; j <= n; ++j) {
        int g = grundy[j];
        for (int b = 0; b < 9; ++b) {
            if (g & (1 << b)) matrix[b][j - 1] = 1;
        }
    }

    // Gaussian elimination to find rank of the 9 x n matrix over GF(2).
    int rank = 0;
    std::vector<int> col_pivot(n, -1); // not needed, just track pivot columns
    for (int col = 0; col < n && rank < 9; ++col) {
        // find a row with a 1 in this column among rows rank..8
        int pivot_row = -1;
        for (int r = rank; r < 9; ++r) {
            if (matrix[r][col]) {
                pivot_row = r;
                break;
            }
        }
        if (pivot_row == -1) continue;
        // swap rows
        std::swap(matrix[rank], matrix[pivot_row]);
        // eliminate this column from all other rows
        for (int r = 0; r < 9; ++r) {
            if (r != rank && matrix[r][col]) {
                for (int c = 0; c < n; ++c) {
                    matrix[r][c] ^= matrix[rank][c];
                }
            }
        }
        ++rank;
    }

    // Number of solutions to A*x = 0 over GF(2) is 2^(n - rank)
    long long exponent = n - rank;
    return (int)mod_pow(2, exponent, MOD);
}
#include <cassert>
#include <vector>
#include <utility>

// Declare the function from the solution
int countZeroGrundyGraphs(int n, const std::vector<std::pair<int,int>>& edges);

int main() {
    // Example 1: single vertex, no edges. Grundy[1]=0. Subsets with XOR zero: empty set only (since {1} gives XOR 0? Actually {1} gives 0 too, so both subsets: {} and {1} => 2 subsets)
    {
        std::vector<std::pair<int,int>> edges;
        int result = countZeroGrundyGraphs(1, edges);
        // Grundy[1]=0, so any subset has XOR 0. There are 2 subsets.
        assert(result == 2);
    }
    // Example 2: two vertices with edge 1->2. Grundy[2]=0, Grundy[1]=mex{0}=1. Subsets with XOR zero: {} only? Check: {} -> 0; {1}->1; {2}->0; {1,2}->1 xor 0 =1. So only {} and {2} give 0? Actually {2} has Grundy 0, so XOR 0, so {2} also works. So {} and {2} => 2 subsets.
    {
        std::vector<std::pair<int,int>> edges = {{1,2}};
        assert(countZeroGrundyGraphs(2, edges) == 2);
    }
    // Example 3: disconnected two isolated vertices. Both Grundy 0. All 4 subsets have XOR 0 => 4.
    {
        std::vector<std::pair<int,int>> edges;
        assert(countZeroGrundyGraphs(2, edges) == 4);
    }
    // Example 4: path 1->2->3. Grundy[3]=0, Grundy[2]=mex{0}=1, Grundy[1]=mex{1}=0. Grundy values: [0,1,0]. Subsets with XOR zero: all subsets where the number of vertices with Grundy 1 is even (since 0's don't matter). There are two vertices with Grundy 1? Actually only vertex 2 has Grundy 1. So condition: vertex 2 must be absent. Then vertices 1 and 3 can be arbitrary: 2^2=4 subsets. Plus vertex 2 present? XOR would be 1, not zero. So total 4.
    {
        std::vector<std::pair<int,int>> edges = {{1,2},{2,3}};
        assert(countZeroGrundyGraphs(3, edges) == 4);
    }
    // Example 5: two branches: 1->3, 2->3. Grundy[3]=0, Grundy[1]=mex{0}=1, Grundy[2]=1. So two vertices have Grundy 1. Subsets with XOR zero: need even number of selected among {1,2} (0 or 2), and vertex 3 arbitrary. So 2 choices for {1,2} (none or both) * 2 choices for {3} = 4.
    {
        std::vector<std::pair<int,int>> edges = {{1,3},{2,3}};
        assert(countZeroGrundyGraphs(3, edges) == 4);
    }
    // Example 6: n=0? Not valid, but we skip.
    // Example 7: larger graph with a cycle? Not allowed (DAG), skip.
    return 0;
}
