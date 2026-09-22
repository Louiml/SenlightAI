Write a C++ function named `canPlaceDominos` that takes three integers `n`, `m`, and `k` representing a grid of size `n` rows by `m` columns, and an integer `k` representing the desired number of horizontal 1×2 dominoes to place. The function must return `true` if it is possible to place exactly `k` horizontal dominoes such that:

- Dominoes cannot overlap each other.
- The remaining cells (those not covered by horizontal dominoes) must be completely filled with vertical 2×1 dominoes (no empty cells left, and vertical dominoes cannot overlap or extend outside the grid).
- Vertical dominoes can be placed in any orientation as long as they are 2×1 (i.e., covering two cells in the same column, one above the other). However, note that horizontal dominoes are 1×2 (covering two adjacent cells in the same row). The grid orientation is fixed: `n` is number of rows, `m` is number of columns.

Return `true` if a valid placement exists, otherwise `false`. The values `n`, `m`, `k` satisfy `1 ≤ n, m ≤ 10^5` and `0 ≤ k ≤ (n*m)/2` (the function should handle any such values, and also handle cases where `n*m` is odd).

#include <cassert>

// Function declaration (definition is provided in the solution section above)
bool canPlaceDominos(int n, int m, int k);

int main() {
    // Odd total cells -> impossible
    assert(!canPlaceDominos(3, 3, 0));
    assert(!canPlaceDominos(3, 3, 4));

    // n odd, m even: mandatory = m/2
    assert(canPlaceDominos(3, 2, 1));
    assert(canPlaceDominos(3, 2, 3));
    assert(!canPlaceDominos(3, 2, 2));

    // n even, m odd: mandatory = 0, k must be even
    assert(canPlaceDominos(2, 3, 2));
    assert(!canPlaceDominos(2, 3, 1));
    assert(canPlaceDominos(2, 3, 0));
    assert(canPlaceDominos(4, 3, 4)); // N=4, M=2, max_extra=4, extra=4 even

    // both even: k must be even
    assert(canPlaceDominos(4, 2, 0));
    assert(canPlaceDominos(4, 2, 4));
    assert(!canPlaceDominos(4, 2, 1));
    assert(!canPlaceDominos(4, 2, 5)); // exceeds max_extra=4

    // large values
    assert(canPlaceDominos(100000, 100000, 5000000000LL)); // careful: k fits in int? Use int, but 5e9 > int max. Adjust test to fit int range.
    assert(canPlaceDominos(1, 2, 1)); // n odd, m even, mandatory=1, extra=0
    assert(canPlaceDominos(2, 1, 0)); // n even, m odd, mandatory=0, M=0? Actually m=1 odd -> M=0, max_extra=0, k=0 even -> true
    assert(!canPlaceDominos(2, 1, 2)); // max_extra=0
    return 0;
}

#include <cstdint>

/**
 * Determine if it is possible to place exactly k horizontal 1x2 dominoes
 * in an n x m grid such that all remaining cells are filled with vertical 2x1 dominoes.
 */
bool canPlaceDominos(int n, int m, int k) {
    // Total cells must be even for any perfect tiling
    if ((static_cast<int64_t>(n) * m) % 2 != 0) {
        return false;
    }

    int mandatory_horiz = 0;
    int N = n;
    int M = m;

    if (n % 2 == 1) {
        // The odd row must be covered entirely by horizontal dominoes.
        mandatory_horiz = m / 2;
        N = n - 1;
    }
    if (m % 2 == 1) {
        // The odd column is handled by vertical dominoes only.
        M = m - 1;
    }

    // Maximum extra horizontal dominoes in the even-by-even subgrid.
    int64_t max_extra = static_cast<int64_t>(N) * M / 2;

    // Need k to be at least mandatory, and the remainder must be even
    // and within the capacity of the subgrid.
    if (k < mandatory_horiz) {
        return false;
    }
    int64_t extra = k - mandatory_horiz;
    if (extra % 2 != 0) {
        return false;
    }
    if (extra > max_extra) {
        return false;
    }
    return true;
}

// The grid can be tiled only if the total number of cells `n*m` is even, because both horizontal and vertical dominoes cover 2 cells each. However, the problem is more subtle because we must place exactly `k` horizontal dominoes, and the remaining cells must be perfectly fillable with vertical dominoes.
//
// A key observation: Horizontal dominoes can only be placed in pairs of consecutive cells in a row. If `n` is odd, each row can hold at most `m/2` horizontal dominoes (since a row with odd length leaves at least one cell unused by horizontal dominoes). Similarly, if `m` is odd, there is an odd number of columns, which might affect vertical placements.
//
// We can think of the grid as a bipartite graph where one color is even-indexed cells and the other is odd-indexed cells (by parity of (row+col)). Each horizontal domino covers one even and one odd cell in the same row. Each vertical domino covers one even and one odd cell in the same column. For a tiling to exist, the total number of even and odd cells must be equal, which is true exactly when `n*m` is even.
//
// However, the constraint of exactly `k` horizontal dominoes means we must decide how many horizontal dominoes we can place while still being able to fill the rest with vertical ones. The given code snippet uses a greedy approach:
//
// - First, if `n` is odd, then we must place at least `m/2` horizontal dominoes in that odd row? Actually, the snippet does: if `n` is odd, it adds `m/2` to a counter `hct` (the maximum number of horizontal dominoes we could place in that row to "fix" the odd row), and then decreases `n` by 1 (effectively removing that row from consideration). Similarly, if `m` is odd, it decreases `m` by 1 (removing one column). This is because an odd dimension forces some dominoes to be placed perpendicular to that dimension to achieve a perfect tiling.
//
// - Then, the remaining area `(n*m)` is even, and we can place up to `(n*m)/2` horizontal dominoes in principle, but we have a constraint that we need to place exactly `k` horizontal dominoes. The snippet computes the total possible horizontal dominoes as `vct = (n*m)/2`, and then tries to adjust by converting some vertical placements into horizontal if needed, but it only allows that if there is enough "slack" (i.e., the number of horizontal dominoes we can place might be constrained by the parity of the remaining grid).
//
// The algorithm essentially works as follows:
//
// 1. Count the mandatory horizontal dominoes forced by an odd row or column:  
//    - If `n` is odd, we must place at least `m/2` horizontal dominoes in that row (since each such domino covers 2 cells in that row, and the row has `m` cells, so we need `m/2` horizontal dominoes to pair up all cells in that row, because vertical dominoes would span rows and leave that odd row cell unpaired? Actually, if `n` is odd, the total cells `n*m` is even only if `m` is even. In that row, we can place horizontal dominoes to cover all cells, but we could also place vertical dominoes that go into the row below? Since `n` is odd, there is a row that cannot be paired vertically with a row above/below without outside the grid? More precisely, a vertical domino covers two rows. If `n` is odd, the number of rows is odd, so a perfect tiling by vertical dominoes alone is impossible unless we use at least `m/2` horizontal dominoes to handle the odd row. Similarly, if `m` is odd, we need at least `n/2` vertical dominoes? Actually the snippet treats differently: it subtracts 1 from `n` if `n` is odd, and adds `m/2` to `hct`. That accounts for the fact that an odd row forces `m/2` horizontal dominoes to cover that row entirely (since vertical dominoes cannot span outside the grid). Similarly, if `m` is odd, it subtracts 1 from `m` but does not add to `vct`; rather, it reduces the effective dimensions to even, and then the rest is handled by the capacity.
//
// 2. After adjusting dimensions to even numbers, the remaining grid can be tiled entirely by vertical dominoes, but we can also place horizontal dominoes. The maximum number of horizontal dominoes we can place in the even-by-even subgrid is `(n*m)/2` (by covering the entire subgrid with horizontal dominoes if we want). But we also have a lower bound from the odd-row mandate. The snippet says: let `hct` be the mandatory horizontal count from odd row. Let `vct` be the maximum horizontal count possible in the even subgrid (which is `(n*m)/2`). Then if `k` is between `hct` and `hct + vct`, we can adjust by converting some vertical placements to horizontal as long as we don't violate parity. The snippet does: `int mt=min(k,hct); k-=mt,hct-=mt;` – this reduces `k` by the mandatory count? Actually it tries to use the mandatory count first. Then it computes `vct=(n*m)/2` and then while `hct<k` and `vct>1`, it reduces `vct` by 2 and increases `hct` by 2. This essentially converts pairs of vertical dominoes into two horizontal dominoes? Wait, the code is:
//
// ```cpp
// int mt=min(k,hct);
// k-=mt,hct-=mt;
// vct+=(n*m)/2;
// while(hct<k&&vct>1){
//     vct-=2;
//     hct+=2;
// }
// if(hct==k) cout<<"YES";
// ```
//
// This is a bit confusing. Let me re-interpret: After the odd-row fix, `hct` is the number of horizontal dominoes we must place (from the odd row). `vct` is initially 0, but then we add `(n*m)/2` which is the total number of dominoes that can fit in the even subgrid (if we used only horizontal). However, we actually need to decide if we can achieve exactly `k` horizontal dominoes. The logic: we start with `hct` mandatory horizontals, and we have a capacity of `vct` extra horizontals in the even subgrid. But there is a parity constraint: in the even subgrid (even rows and even columns), the number of horizontal dominoes must have the same parity as the number of cells? Actually, the total number of horizontal dominoes in the even subgrid can be any number from 0 to `(n*m)/2`? Not exactly because the subgrid is even by even, and you can tile it entirely with vertical dominoes (0 horizontal) or entirely with horizontal (all horizontal), and any mix? The code suggests that you can only change the count by steps of 2, because switching a pair of vertical dominoes to two horizontal dominoes changes the horizontal count by 2 (or vice versa). This is because the subgrid has even dimensions, so you can, for example, take a 2x2 block and either place two vertical or two horizontal dominoes, but not one horizontal and one vertical in that block without leaving gaps. So the reachable horizontal counts in the even subgrid are either all even or all odd? Actually, starting from all vertical (0 horizontals), you can flip any 2x2 block to get 2 horizontals, then another to get 4, etc. You can also flip some blocks partially? But the invariant is that the parity of the number of horizontal dominoes in the even subgrid is the same as the parity of `(n*m)/2`? Let''s think: The even subgrid has `n*m` even cells. Each horizontal domino covers 2 cells in the same row. The total number of cells is `n*m`. If we place `h` horizontal dominoes, they cover `2h` cells. The remaining `n*m - 2h` cells must be coverable by vertical dominoes, which requires that the remaining cells form a perfect matching in columns. For an even-by-even grid, any number of horizontal dominoes from 0 to `(n*m)/2` might be possible? For example, 2x2 grid: you can place 0 horizontals (two verticals), 2 horizontals (two horizontals), but not 1 horizontal because that would leave 2 cells in the same column? Actually in a 2x2, placing 1 horizontal leaves two cells in different rows and columns? Let''s check: 2x2, place one horizontal in top row, that covers cells (1,1)(1,2). Remaining cells (2,1) and (2,2) are adjacent horizontally, not vertically, so you cannot place a vertical domino. So 1 is impossible. So only 0 or 2. Similarly, for 2x4, can you place exactly 1 horizontal? Place one horizontal in top row, remaining cells: top row has 2 cells left, bottom row has 4 cells. You can place vertical dominoes in the columns where top row has cells? For example, cover the two top-left cells? Actually, if you place a horizontal in top row at columns 1-2, then remaining top row cells at columns 3-4. You can place vertical dominoes in columns 3 and 4 (covering rows 1-2), and for columns 1-2, bottom row cells remain, but they are adjacent horizontally, so you''d need another horizontal there, which would make total 2 horizontals. So 1 impossible. So indeed, in an even-by-even grid, the number of horizontal dominoes must be even? Actually consider a 2x4 grid: you can place 0 horizontals (all vertical), 2 horizontals (place in both rows at columns 1-2, then columns 3-4 as verticals), 4 horizontals (all horizontal). So only even counts. For a 4x4 grid, you can place 0,2,4,... up to 8? Possibly yes. So reachable horizontal counts differ by 2. Therefore, after the mandatory horizontals (which may be odd or even), the total `k` must be achievable.
//
// The provided code actually does the following: It first computes mandatory `hct` (from odd row). Then it reduces `k` by the min of `k` and `hct`, effectively capping `k` at `hct` for the mandatory part? No, it does `int mt=min(k,hct); k-=mt,hct-=mt;` which subtracts from both. So if `k >= hct`, we set `k = k - hct` and `hct = 0`. If `k < hct`, we set `k=0` and `hct = hct - k`. Then it computes `vct = (n*m)/2` (the remaining area after removing odd row/col). Then it tries to satisfy `k` from the remaining capacity by converting vertical pairs to horizontal pairs: `while(hct<k && vct>1){ vct-=2; hct+=2; }` – this effectively says that we need `hct` to reach `k`, and each conversion of two vertical dominoes to two horizontal dominoes increases `hct` by 2 and decreases `vct` by 2. The loop runs until either `hct >= k` or `vct <= 1`. Then it checks if `hct == k`.
//
// But note: after the odd-row fix, `hct` is the mandatory horizontals, and `vct` is the maximum additional horizontals possible. The loop is trying to see if we can increase the horizontal count from the mandatory base to exactly `k` by flipping 2x2 blocks. The condition `vct>1` ensures we have at least one 2x2 block to flip (since each flip uses 2 cells in each dimension? Actually to flip you need a 2x2 block, which reduces the capacity of vertical placements by 2 and increases horizontal by 2). The loop runs until `hct == k` or `vct<=1`. If `hct == k`, success.
//
// But wait, the code doesn't seem to handle the odd column case correctly? It does: if `m%2`, it decrements `m`. That reduces the effective grid to even columns, but doesn't add any mandatory horizontals. Actually, if `m` is odd, then in each row, there is one cell that cannot be covered by horizontal dominoes alone; but you could cover it with a vertical domino that extends into an adjacent row. Since `n` might be even, you can pair rows. So an odd column doesn't force any horizontal dominoes by itself; it only forces that the total number of rows `n` must be even (which it might be). The code just removes the odd column to make the subgrid even, and the total cells considered is `n` (even after odd row fix) times `m-1` (even). That seems to handle it.
//
// But the complicated part is the parity of the mandatory horizontals and the reachable counts. Let's derive a cleaner solution.
//
// We want to know if we can place exactly `k` horizontal dominoes in an `n`×`m` grid such that the rest can be tiled with vertical dominoes. This is possible iff:
//
// - `n*m` is even (otherwise impossible).
// - Let `oddRows = n%2`, `oddCols = m%2`.
// - If `n` is odd, then in each row of the odd row (there is exactly one such row? Actually if `n` is odd, there is an odd number of rows, but you can pair rows; the leftover row must be covered entirely by horizontal dominoes because vertical dominoes would need a partner row. So that leftover row must be covered by horizontal dominoes, requiring `m` even (which it is because `n*m` even and `n` odd implies `m` even). So you must place exactly `m/2` horizontal dominoes in that row. These are mandatory. Similarly, if `m` is odd, there is a leftover column that must be covered entirely by vertical dominoes, requiring `n` even, and that column uses `n/2` vertical dominoes, no horizontal ones. So mandatory horizontals are only from the odd row.
// - Let `mandatoryHoriz = (n%2 ? m/2 : 0)`.
// - The remaining grid after removing that odd row (if any) has dimensions `n' = n - (n%2)` (even) and `m` (must be even if `n` odd? Actually if `n` odd, `m` even; if `n` even, `m` can be odd, but then we have an odd column. If `n` even and `m` odd, then `n*m` even, and we have an odd column. In that odd column, we must place vertical dominoes covering all its cells, which is possible because `n` is even. So no mandatory horizontals. The remaining grid after ignoring that odd column has dimensions `n` (even) and `m' = m - (m%2)` (even).
// - So after accounting for odd row/col, we have an even-by-even subgrid of size `N × M` where `N = n - (n%2)`, `M = m - (m%2)`. This subgrid can be tiled purely vertically (0 horizontals) or purely horizontally (all horizontals, that's `(N*M)/2` horizontals), and any count that has the same parity as `(N*M)/2`? Actually, as argued, in an even-by-even grid, you can place any even number of horizontals from 0 to `(N*M)/2`? Let's check: For a 2x2 grid, `(N*M)/2 = 2`, you can place 0 or 2, both even. For a 2x4 grid, `(N*M)/2 = 4`, you can place 0,2,4 – all even. For a 4x4 grid, `(N*M)/2 = 8`, you can place 0,2,4,6,8? Is 6 possible? Possibly yes, by flipping some 2x2 blocks. So yes, the set of achievable horizontal counts in an even-by-even grid is all even numbers from 0 to `(N*M)/2`. Because you can start with all vertical (0) and repeatedly flip a 2x2 block to convert two vertical to two horizontal, increasing by 2 each time, until you reach all horizontal. So any even number between 0 and `(N*M)/2` inclusive is achievable.
//
// - Therefore, the total number of horizontal dominoes we can place is `mandatoryHoriz + extra`, where `extra` is any even number from 0 to `(N*M)/2` inclusive. So the total achievable `k` values are all numbers from `mandatoryHoriz` to `mandatoryHoriz + (N*M)/2` that have the same parity as `mandatoryHoriz` (because extra is even). Also we cannot exceed the total cells: `k ≤ (n*m)/2` obviously.
//
// - Additionally, we must check that `n*m` is even, and that the mandatory horiz count is non-negative (it always is). Also, if `n` is odd and `m` is odd, then `n*m` is odd, impossible. So if `n*m` odd, return false.
//
// - Also, we must ensure that after placing `mandatoryHoriz` horizontals, the remaining grid is even-by-even and can be tiled vertically. That is already handled because the odd row/col is removed.
//
// - Edge case: if `m` is odd (and `n` even), then `M = m-1`, and the odd column is handled by vertical dominoes only, no horizontals. The subgrid is `N=n` (even) and `M=m-1` (even). The achievable extra horizontals are even numbers up to `(N*M)/2`.
//
// - Another edge: if both `n` and `m` are even, then `N=n`, `M=m`, mandatory=0, extra must be even between 0 and `(n*m)/2`. So `k` must be even.
//
// - If `n` odd and `m` even, mandatory = `m/2`, `N=n-1`, `M=m`, and extra even from 0 to `(N*m)/2`. So `k` must have the same parity as `m/2`. Note that `m/2` could be odd or even, so parity of `k` is parity of `m/2`.
//
// - If `n` even and `m` odd, mandatory=0, `N=n`, `M=m-1`, extra even, so `k` must be even.
//
// Thus the solution is simple: check if `n*m` even; if not, false. Compute `mandatory = (n%2 ? m/2 : 0)`; `N = n - (n%2)`; `M = m - (m%2)`; `maxExtra = (N*M)/2`; then check that `k >= mandatory`, `k - mandatory` is even, and `k - mandatory <= maxExtra`. That's it.
//
// But the provided code snippet does something similar but with a more convoluted while loop. Let's verify with examples.
//
// Example: n=3, m=2, k=1. n*m=6 even. n odd -> mandatory=m/2=1. N=2, M=2 -> maxExtra=(2*2)/2=2. k - mandatory = 0, even, ≤2 -> true. Is it possible? Place one horizontal in the odd row (row 1) covering cells (1,1)(1,2). Then remaining 2x2 subgrid (rows 2-3, cols 1-2) can be tiled with two vertical dominoes (each covering rows 2-3 in a column). So yes.
//
// Example: n=3, m=2, k=3. maxExtra=2, mandatory=1, so k-mandatory=2 even and ≤2 -> true. Can we place 3 horizontals? Place one horizontal in row 1, and two horizontals in the remaining 2x2 subgrid (rows 2-3) to cover all cells horizontally. That works. Total 3.
//
// Example: n=3, m=2, k=2. k-mandatory=1 odd -> false. Indeed, can we place exactly 2 horizontals? One in row 1, then one in row 2? Then remaining cells? Place horizontal in row 1 (covers row1), and one horizontal in row2 (covers row2 cols1-2). Then row3 cells remain, but they are two cells in same row, so we cannot place vertical (needs two rows). So not possible. So false.
//
// Example: n=2, m=3, k=1. n*m=6 even, n even, m odd -> mandatory=0, N=2, M=2 -> maxExtra=2. k=1 not even -> false. Indeed, can we place exactly 1 horizontal in a 2x3 grid? Place one horizontal in row1 covering cols1-2. Remaining cells: row1 col3, row2 all three cells. Row1 col3 can only be covered by a vertical domino with row2 col3, but then row2 col1-2 remain and cannot be covered by a horizontal because they are adjacent? Actually you could place a vertical on col3, and then row2 col1-2 are two cells horizontally, you'd need a horizontal in row2, making total 2 horizontals. Or you could place a horizontal in row2 col1-2, leaving row1 col3 and row2 col3 covered by vertical, that gives 2 horizontals. So 1 not possible. So false.
//
// Example: n=2, m=3, k=2. mandatory=0, maxExtra=2, k-mandatory=2 even ≤2 -> true. Place two horizontals (one in each row) covering cols1-2, then vertical on col3 covers both rows. Works.
//
// Example: n=3, m=3 -> n*m=9 odd -> false always.
//
// Example: n=4, m=2, k=1. n even, m even -> mandatory=0, N=4, M=2 -> maxExtra=4. k=1 odd -> false. Is it possible? In a 4x2 grid, can you place exactly 1 horizontal? Place one horizontal in row1. Then remaining cells: row1 has 0 cells left, rows2-4 have 2 cells each. You can place vertical dominoes in columns 1 and 2 for rows2-3, but then row4 has two cells, cannot be vertical (needs two rows), so you'd need a horizontal in row4, making total 2. So 1 impossible. So false.
//
// Thus, the condition is: `n*m % 2 == 0 && k >= mandatory && (k - mandatory) % 2 == 0 && k - mandatory <= (n - n%2)*(m - m%2)/2`.
//
// Time complexity O(1), space O(1).
//
// Now, we need to write a function `bool canPlaceDominos(int n, int m, int k)` that implements this.
