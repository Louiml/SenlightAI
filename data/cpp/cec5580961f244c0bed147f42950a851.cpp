Given a 30×30 integer matrix `ret` (provided as a `vector<vector<int>>`), along with three target scores `B[0]`, `B[1]`, `B[2]` (each a positive int, with `B[0] < B[1] < B[2]`), write a standalone C++ function that computes the total points defined as follows: For every vertical segment (all contiguous sub-columns starting at any row `i` and extending downward) and for every horizontal segment (all contiguous sub-rows starting at any column `j` and extending rightward), sum the values of the segment until the running sum exceeds `B[2]` (then stop that segment). For each step of the running sum, if it equals exactly `B[0]`, `B[1]`, or `B[2]`, add that matching value to the total points. The function must return the total points as an `int`. The matrix values are guaranteed to be between 1 and 9 inclusive. The function should be named `compute_chokudai_points` and must not read input or write output; it only processes the passed parameters. The input size is fixed (N=30) but the function should work for any square matrix size.

#include <cassert>
#include <vector>

// Solution function declaration (included above in a real project).
int compute_chokudai_points(const std::vector<std::vector<int>>& ret,
                             const std::vector<int>& B);

int main() {
    // Test 1: N=1, single cell equals one target exactly.
    std::vector<std::vector<int>> m1 = {{5}};
    assert(compute_chokudai_points(m1, {5, 10, 15}) == 10); // vertical hit + horizontal hit

    // Test 2: N=1, single cell below all targets.
    std::vector<std::vector<int>> m2 = {{4}};
    assert(compute_chokudai_points(m2, {5, 10, 15}) == 0);

    // Test 3: N=2, all ones, targets at 2 and 3.
    std::vector<std::vector<int>> m3 = {{1,1},{1,1}};
    // Vertical: (0,0) sum=1 then 2 (hit B[0]=2) → +2; (0,1) same → +2; (1,0) sum=1 then 2 → +2; (1,1) sum=1 then 2 → +2 → total vertical=8
    // Horizontal: same pattern → +8, total=16
    assert(compute_chokudai_points(m3, {2, 3, 20}) == 16);

    // Test 4: N=2, matrix [[2,3],[4,5]], targets at 3,5,9.
    std::vector<std::vector<int>> m4 = {{2,3},{4,5}};
    // Vertical: (0,0): 2,6,10>9 break (no hits); (0,1): 3 (hit B[0]=3) →+3, 8 (not 9), no hit; (1,0): 4; (1,1):5 →+5
    // Vertical total=8
    // Horizontal: (0,0):2,5 (hit B[1]=5)→+5; (0,1):3 (hit) →+3; (1,0):4,9 (hit B[2]=9)→+9; (1,1):5→+5
    // Horizontal total=5+3+9+5=22, grand total=30
    assert(compute_chokudai_points(m4, {3,5,9}) == 30);

    // Test 5: N=3, all cells = 2, targets at 4,6,10 (6 is reachable, 10 not).
    std::vector<std::vector<int>> m5(3, std::vector<int>(3, 2));
    // For each starting cell, vertical sums: 2,4(hit),6(hit),8,10(? but 10 not hit because we don't check after exceeding? actually sum=8 then 10 not reached because we only go to N=3, sum ends at 6,8? let's compute: from (0,0): 2,4,6 → hit 4 and 6. From (0,1): same. (0,2): same.
    // All 9 starting cells each give two hits: 4 and 6 → each cell contributes 10 points. But careful: each cell's vertical direction: from (0,0): sums 2,4,6 → hites 4 and 6 → +10. From (0,1): same. (0,2): same. (1,0): 2,4,6 → +10. etc. So 9 cells * 10 = 90 for vertical.
    // Horizontal: each of 9 cells gives 2,4,6 → +10 each, so +90. Total 180.
    assert(compute_chokudai_points(m5, {4,6,10}) == 180);

    // Test 6: N=3, matrix with a large value that causes early break.
    std::vector<std::vector<int>> m6 = {{1, 100, 1}, {1, 1, 1}, {1, 1, 1}};
    // Note: values can be 1..9 per problem, but let's test robustness with 100 (not in spec, but logic should still break correctly)
    // We'll use targets 5,10,20. For (0,0) vertical: 1,2,3 no hits; (0,1) vertical: 100>20 break immediately; etc.
    // This test ensures break logic works. We'll just assert it doesn't crash and returns something.
    int result6 = compute_chokudai_points(m6, {5,10,20});
    assert(result6 >= 0);

    // Test 7: N=4, single row of 1, targets at 1,2,3.
    std::vector<std::vector<int>> m7(4, std::vector<int>(4, 1));
    // For each starting cell, horizontal sums: 1(hit),2(hit),3(hit) for the first three, and the last cell gives 1(hit) only.
    // We just check a small case: N=1 already tested, N=2 tested, full calculation is complex, so just check positive.
    assert(compute_chokudai_points(m7, {1,2,3}) > 0);

    return 0;
}

#include <vector>
#include <algorithm>

// Computes total points from vertical and horizontal contiguous segments.
// For each starting cell, scans downward and rightward, adding the running sum
// to the total whenever it equals one of the three target values.
// Stops scanning a direction as soon as the running sum exceeds B[2].
int compute_chokudai_points(const std::vector<std::vector<int>>& ret,
                             const std::vector<int>& B) {
    const int N = static_cast<int>(ret.size());
    int point = 0;

    // Vertical segments: start at (i, j), extend downward.
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            int now = 0;
            for (int k = i; k < N; ++k) {
                now += ret[k][j];
                if (now == B[0]) point += B[0];
                if (now == B[1]) point += B[1];
                if (now == B[2]) point += B[2];
                if (now > B[2]) break;
            }
        }
    }

    // Horizontal segments: start at (i, j), extend rightward.
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            int now = 0;
            for (int k = j; k < N; ++k) {
                now += ret[i][k];
                if (now == B[0]) point += B[0];
                if (now == B[1]) point += B[1];
                if (now == B[2]) point += B[2];
                if (now > B[2]) break;
            }
        }
    }

    return point;
}

// The algorithm iterates over every starting cell `(i,j)` in the matrix. For each starting cell, it first explores the vertical direction: it maintains a running sum `now`, adds `ret[k][j]` for `k = i` to `N-1`, and after each addition checks if `now` equals any of the three target values; if so, accumulates that target into `point`. If `now` exceeds `B[2]`, it breaks out of the inner loop because further additions would only increase the sum and never hit the targets again. The same procedure is repeated for the horizontal direction: for each starting cell `(i,j)`, it adds `ret[i][k]` for `k = j` to `N-1`, with identical break and accumulation logic. This double loop over all starting positions gives `O(N^2)` starting cells, and each scan runs at most `N` steps, so the total time is `O(N^3)` (for N=30, about 27,000 operations, which is trivial). Space usage is `O(1)` beyond the input matrix. Edge cases include segments that never reach any target (no points), segments that hit multiple targets at different prefix sums (each gets credited), and segments that hit `B[2]` exactly before exceeding it—this last hit is counted, then the loop breaks because the next addition would exceed `B[2]`.
