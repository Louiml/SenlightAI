Write a C++ function that takes an integer `n` and an integer `k` as parameters, and returns a 2D vector (or equivalent) representing an `n x n` symmetric binary adjacency matrix such that the matrix contains exactly `k` ones, the main diagonal is allowed to have at most one `1` per position, and for every off-diagonal `1` at position `(i,j)`, there must be a matching `1` at `(j,i)`. If it is impossible to place exactly `k` ones under these constraints, the function should return an empty matrix (e.g., an empty `vector<vector<int>>`). The returned matrix must be symmetric, and the diagonal entries can be either 0 or 1 only if needed to satisfy the count; however, placing a `1` on the diagonal uses only one count, while placing an off-diagonal symmetric pair uses two counts. The function should try to fill the matrix greedily row by row and column by column, but must correctly return an empty matrix if the requested `k` cannot be achieved given `n`.
The problem reduces to counting how many "slots" are available. Diagonal entries: there are `n` positions, each contributes 1 to the count if set to 1. Off-diagonal entries: there are `n*(n-1)/2` independent symmetric pairs, each contributes 2 to the count if both are set. The total possible distinct counts are all integers from 0 up to `n*(n-1)/2 * 2 + n`, but with parity constraints? Actually every off-diagonal pair adds exactly 2, and each diagonal adds 1, so any `k` from 0 to `n + 2*(n*(n-1)/2)` = `n + n*(n-1) = n^2` is achievable except possibly some values? Let's check: you can choose any number of diagonal ones (0..n) and any number of off-diagonal pairs (0..n*(n-1)/2). The total count = d + 2*p, where `d` in [0,n] and `p` in [0, M] with M = n*(n-1)/2. So the possible sums are all integers from 0 to n+2M = n^2, but is there any gap? For any k, if k <= n, you can set k diagonal ones. If k > n, let r = k - n. Since n <= k <= n^2, r ranges from 0 to n(n-1). Since M = n(n-1)/2, the maximum even number less than or equal to r is achievable by choosing p = floor(r/2) but careful: if r is odd, you need one more odd unit, which you can get from a diagonal (you already used all n diagonals? Actually if k > n, you must use all n diagonals? No, you can mix. For example n=2, possible k: 0,1,2 (both diagonals), 3 (diag+offdiag pair? Actually offdiag pair adds 2, plus one diagonal gives 3), 4 (both diag + offdiag pair). So all 0..4 are achievable. In general, because you can always use one diagonal instead of two offdiagonal, all counts from 0 to n^2 are achievable? For n=3, max=9, any k 0..9? k=7: use 3 diagonals + 2 offdiag pairs (3+4=7). k=8: 2 diagonals + 3 offdiag pairs (2+6=8). k=9: 3+6=9. Yes all. So the only impossible case is when k > n^2 or k < 0, or when n <= 0? The original snippet had a bug: it breaks out of inner loop if k becomes 0 but then continues outer loop? Actually the snippet's logic: for each i and j, if k==0 break (only breaks inner loop, then outer continues but break is inside inner, so after breaking inner, outer moves to next i and inner restarts, but since k still 0, it breaks again, so effectively stops generating but then after loops it checks if k>0 and prints -1. However the snippet also sets off-diagonal only if k>1, but the condition `if(!a[i][j] && i==j && k>0)` and then `if(!a[i][j] && k>1 && i!=j)` – note that after setting diagonal, it continues, then may also set off-diagonal in same iteration? But because it uses `if` not `else if`, it can set both diagonal and off-diagonal in same (i,j) if i==j? Actually for i==j, the first if sets diagonal, then second if checks `i!=j` which is false, so fine. For i!=j, first if fails, second if may set both a[i][j] and a[j][i] if k>1. The loops go i from 1..n, j from 1..n, but when it sets a[j][i] for a later j? Since j loop runs increasing, when i=1,j=2 sets a[1][2] and a[2][1], then later when i=2,j=1, it checks `!a[i][j]` (a[2][1] is already 1) so skip. So it works. But the snippet's failure condition is `if(k) return -1` – meaning if after processing all cells there is still k left, it outputs -1. But since it breaks inner loops when k==0, it leaves the rest zeros. Actually it only breaks inner, but outer continues, and because k==0, the first if `!k` break only inner, but then inner re-enters and immediately breaks because !k, so effectively no more assignments. Then after all loops, if k>0 prints -1. However for some k that are impossible (like k > n^2), it would print -1. Also for some k that are possible but the greedy algorithm might fail? Let's test n=1, k=2: available max is 1, so impossible, correct. n=1, k=0: possible, returns all zeros. n=1,k=1: diagonal set, works. For n=2, k=1: possible (one diagonal), but the snippet: i=1,j=1, k>0, set a[1][1]=1,k=0; then inner breaks; outer i=2, inner j=1, !k break, so matrix has one 1 at (1,1), symmetric? diagonal only, fine. k=2: i=1,j=1 set diagonal, k=1; then i=1,j=2, !a[1][2] && k>1? k=1 so skip; i=1,j=3? n=2 so loop ends; i=2,j=1, a[2][1] is 0 and k>1? k=1, skip; i=2,j=2, set diagonal? but a[2][2] was 0, i==j, k>0 true, set a[2][2]=1, k=0. So diagonal both, count 2. Works. k=3: i=1,j=1 set diag (k=2), i=1,j=2: k>1, set a[1][2]=1 (k=1) and a[2][1]=1 (k=0), then break. So matrix has (1,1), (1,2),(2,1) – 3 ones. Works. k=4: i=1,1 diag (k=3), i=1,2 offdiag (k=2 then k=1? Actually set a[1][2]=1 k-- => k=2, a[2][1]=1 k-- => k=1), then i=1,3? n=2 so no, i=2,1: a[2][1] already 1, skip; i=2,2: diag set (k=0). Total 4. Works. So the greedy works for all feasible k. The task is to implement a clean version. The function returns a matrix or empty if impossible. Need to handle n=0? Probably assume n>=1. Edge cases: k=0 returns all zeros. k=n^2 returns all ones. k just above n^2 returns empty. Also note that the matrix size is n x n with indices 0-based in the solution. Time O(n^2) because we iterate all cells once. Space O(n^2) for output.
#include <vector>

// Build an n x n symmetric binary matrix with exactly k ones.
// Returns an empty matrix if impossible.
std::vector<std::vector<int>> buildSymmetricMatrix(int n, int k) {
    if (n <= 0 || k < 0 || k > n * n) {
        return {};
    }

    std::vector<std::vector<int>> mat(n, std::vector<int>(n, 0));
    int remaining = k;

    for (int i = 0; i < n && remaining > 0; ++i) {
        for (int j = 0; j < n && remaining > 0; ++j) {
            if (mat[i][j] != 0) {
                continue;
            }
            if (i == j) {
                // Diagonal entry uses 1 one
                mat[i][j] = 1;
                --remaining;
            } else if (remaining >= 2) {
                // Off-diagonal symmetric pair uses 2 ones
                mat[i][j] = 1;
                mat[j][i] = 1;
                remaining -= 2;
            }
        }
    }

    if (remaining != 0) {
        return {};
    }
    return mat;
}
#include <cassert>
#include <vector>

// Declaration (the function definition is above, but for test we include it here)
std::vector<std::vector<int>> buildSymmetricMatrix(int n, int k);

int main() {
    // Test 1: n=3, k=0 -> all zeros
    auto m0 = buildSymmetricMatrix(3, 0);
    assert(m0.size() == 3);
    for (auto& row : m0) {
        for (int v : row) {
            assert(v == 0);
        }
    }

    // Test 2: n=3, k=9 -> all ones
    auto m9 = buildSymmetricMatrix(3, 9);
    assert(m9.size() == 3);
    for (auto& row : m9) {
        for (int v : row) {
            assert(v == 1);
        }
    }

    // Test 3: n=1, k=1 -> single 1
    auto m1 = buildSymmetricMatrix(1, 1);
    assert(m1.size() == 1 && m1[0][0] == 1);

    // Test 4: n=1, k=2 -> impossible -> empty
    auto m2 = buildSymmetricMatrix(1, 2);
    assert(m2.empty());

    // Test 5: n=2, k=3 -> symmetric with 3 ones
    auto m3 = buildSymmetricMatrix(2, 3);
    assert(m3.size() == 2);
    int count3 = 0;
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            count3 += m3[i][j];
            assert(m3[i][j] == m3[j][i]);
        }
    }
    assert(count3 == 3);

    // Test 6: n=4, k=7 -> possible
    auto m7 = buildSymmetricMatrix(4, 7);
    assert(m7.size() == 4);
    int count7 = 0;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            count7 += m7[i][j];
            assert(m7[i][j] == m7[j][i]);
            assert(m7[i][j] == 0 || m7[i][j] == 1);
        }
    }
    assert(count7 == 7);

    // Test 7: n=5, k=25 -> all ones
    auto m25 = buildSymmetricMatrix(5, 25);
    assert(m25.size() == 5);
    int count25 = 0;
    for (auto& row : m25) {
        for (int v : row) {
            count25 += v;
        }
    }
    assert(count25 == 25);

    // Test 8: n=3, k=5 -> possible
    auto m5 = buildSymmetricMatrix(3, 5);
    assert(m5.size() == 3);
    int count5 = 0;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            count5 += m5[i][j];
            assert(m5[i][j] == m5[j][i]);
        }
    }
    assert(count5 == 5);

    return 0;
}
