/*
Write a standalone C++ function `computeSwapDistance` that takes two elections represented as vectors of vectors of integers (`el1` and `el2`), where each inner vector is a voter's preference ranking over `m` candidates (permutation of 0..m-1). The function must compute the minimum total swap distance (Kendall tau distance) between all pairs of voters from the two elections, after optimally relabeling candidates in `el1` via a permutation to align with `el2`. For each pair of voters (one from each election), the swap distance is the number of pairs of candidates whose relative order differs between the two rankings. The problem requires iterating over all `m!` candidate permutations, computing for each permutation an `n x n` cost matrix where entry `(i,j)` equals the swap distance between voter `i` (with relabeled ranking) and voter `j`, then solving a minimum-weight perfect matching (assignment problem) on that matrix using the provided `lap` function (included in full). Return the minimal total cost over all permutations. The input sizes satisfy: `1 ≤ n ≤ 20`, `1 ≤ m ≤ 8` (so `m!` ≤ 40320, feasible), and the total distance fits in a 32-bit `int`. Use the provided `lap` implementation exactly, and implement helper functions for distance and permutation generation via `std::next_permutation`. The function signature is `int computeSwapDistance(const std::vector<std::vector<int>>& el1, const std::vector<std::vector<int>>& el2`). Do not include a `main` function.
*/
#include <vector>
#include <algorithm>
#include <climits>
#include <cstdlib>
#include <cstdint>
#include <iostream>

typedef int row;
typedef int col;
typedef int cost;
typedef int boolean;

#define TRUE 1
#define FALSE 0
#define BIG 100000

/* Provided lap function: solves assignment problem */
cost lap(int dim, cost **assigncost, col *rowsol, row *colsol, cost *u, cost *v) {
    boolean unassignedfound;
    row i, imin, numfree = 0, prvnumfree, f, i0, k, freerow, *pred, *free;
    col j, j1, endofpath, low, up, *collist, *matches;
    cost h, umin, usubmin, v2, *d;
    cost min = 9999999;
    col last = -1;
    col j2 = -1;

    free = new row[dim];
    collist = new col[dim];
    matches = new col[dim];
    d = new cost[dim];
    pred = new row[dim];

    for (i = 0; i < dim; i++) matches[i] = 0;

    // COLUMN REDUCTION
    for (j = dim; j--;) {
        min = assigncost[0][j];
        imin = 0;
        for (i = 1; i < dim; i++)
            if (assigncost[i][j] < min) {
                min = assigncost[i][j];
                imin = i;
            }
        v[j] = min;
        if (++matches[imin] == 1) {
            rowsol[imin] = j;
            colsol[j] = imin;
        }
        else if (v[j] < v[rowsol[imin]]) {
            int j1 = rowsol[imin];
            rowsol[imin] = j;
            colsol[j] = imin;
            colsol[j1] = -1;
        }
        else colsol[j] = -1;
    }

    // REDUCTION TRANSFER
    for (i = 0; i < dim; i++)
        if (matches[i] == 0) free[numfree++] = i;
        else if (matches[i] == 1) {
            j1 = rowsol[i];
            min = BIG;
            for (j = 0; j < dim; j++)
                if (j != j1)
                    if (assigncost[i][j] - v[j] < min)
                        min = assigncost[i][j] - v[j];
            v[j1] = v[j1] - min;
        }

    // AUGMENTING ROW REDUCTION
    int loopcnt = 0;
    do {
        loopcnt++;
        k = 0;
        prvnumfree = numfree;
        numfree = 0;
        while (k < prvnumfree) {
            i = free[k];
            k++;
            umin = assigncost[i][0] - v[0];
            j1 = 0;
            usubmin = BIG;
            for (j = 1; j < dim; j++) {
                h = assigncost[i][j] - v[j];
                if (h < usubmin) {
                    if (h >= umin) {
                        usubmin = h;
                        j2 = j;
                    } else {
                        usubmin = umin;
                        umin = h;
                        j2 = j1;
                        j1 = j;
                    }
                }
            }
            i0 = colsol[j1];
            if (umin < usubmin) v[j1] = v[j1] - (usubmin - umin);
            else {
                if (i0 > -1) {
                    j1 = j2;
                    i0 = colsol[j2];
                }
            }
            rowsol[i] = j1;
            colsol[j1] = i;
            if (i0 > -1) {
                if (umin < usubmin) free[--k] = i0;
                else free[numfree++] = i0;
            }
        }
    } while (loopcnt < 2);

    // AUGMENT SOLUTION for each free row
    for (f = 0; f < numfree; f++) {
        freerow = free[f];
        for (j = dim; j--;) {
            d[j] = assigncost[freerow][j] - v[j];
            pred[j] = freerow;
            collist[j] = j;
        }
        low = 0;
        up = 0;
        unassignedfound = FALSE;
        do {
            if (up == low) {
                last = low - 1;
                min = d[collist[up++]];
                for (k = up; k < dim; k++) {
                    j = collist[k];
                    h = d[j];
                    if (h <= min) {
                        if (h < min) {
                            up = low;
                            min = h;
                        }
                        collist[k] = collist[up];
                        collist[up++] = j;
                    }
                }
                for (k = low; k < up; k++)
                    if (colsol[collist[k]] < 0) {
                        endofpath = collist[k];
                        unassignedfound = TRUE;
                        break;
                    }
            }
            if (!unassignedfound) {
                j1 = collist[low];
                low++;
                i = colsol[j1];
                h = assigncost[i][j1] - v[j1] - min;
                for (k = up; k < dim; k++) {
                    j = collist[k];
                    v2 = assigncost[i][j] - v[j] - h;
                    if (v2 < d[j]) {
                        pred[j] = i;
                        if (v2 == min) {
                            if (colsol[j] < 0) {
                                endofpath = j;
                                unassignedfound = TRUE;
                                break;
                            } else {
                                collist[k] = collist[up];
                                collist[up++] = j;
                            }
                        }
                        d[j] = v2;
                    }
                }
            }
        } while (!unassignedfound);

        for (k = last + 1; k--;) {
            j1 = collist[k];
            v[j1] = v[j1] + d[j1] - min;
        }
        do {
            i = pred[endofpath];
            colsol[endofpath] = i;
            j1 = endofpath;
            endofpath = rowsol[i];
            rowsol[i] = j1;
        } while (i != freerow);
    }

    cost lapcost = 0;
    for (i = dim; i--;) {
        j = rowsol[i];
        u[i] = assigncost[i][j] - v[j];
        lapcost = lapcost + assigncost[i][j];
    }
    delete[] pred;
    delete[] free;
    delete[] collist;
    delete[] matches;
    delete[] d;
    return lapcost;
}

// Helper: compute swap distance between two rankings (permutations of 0..m-1)
int swapDistancePerm(const std::vector<int>& rank1, const std::vector<int>& rank2) {
    int m = (int)rank1.size();
    int inv = 0;
    for (int a = 0; a < m - 1; a++) {
        for (int b = a + 1; b < m; b++) {
            // Determine relative order in rank1 and rank2
            int pos1a, pos1b, pos2a, pos2b;
            // Find positions quickly? Since m ≤ 8, linear scan is fine
            for (int t = 0; t < m; t++) {
                if (rank1[t] == a) pos1a = t;
                if (rank1[t] == b) pos1b = t;
                if (rank2[t] == a) pos2a = t;
                if (rank2[t] == b) pos2b = t;
            }
            if ((pos1a < pos1b) != (pos2a < pos2b)) inv++;
        }
    }
    return inv;
}

// Main solution function
int computeSwapDistance(const std::vector<std::vector<int>>& el1, const std::vector<std::vector<int>>& el2) {
    int n = (int)el1.size();
    int m = (int)el1[0].size();
    int min_total = INT_MAX;

    // Generate all permutations of 0..m-1
    std::vector<int> perm(m);
    for (int i = 0; i < m; i++) perm[i] = i;

    do {
        // Build relabeled el1: for each voter, map candidate el1[i][t] -> perm[el1[i][t]]
        std::vector<std::vector<int>> relabeled(n, std::vector<int>(m));
        for (int i = 0; i < n; i++) {
            for (int t = 0; t < m; t++) {
                relabeled[i][t] = perm[el1[i][t]];
            }
        }

        // Build cost matrix C[i][j] = swap distance between relabeled voter i and original voter j
        int** costMatrix = new int*[n];
        for (int i = 0; i < n; i++) costMatrix[i] = new int[n];
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                costMatrix[i][j] = swapDistancePerm(relabeled[i], el2[j]);
            }
        }

        // Solve assignment problem
        col* rowsol = new col[n];
        row* colsol = new row[n];
        cost* u = new cost[n];
        cost* v = new cost[n];
        int dist = lap(n, costMatrix, rowsol, colsol, u, v);
        if (dist < min_total) min_total = dist;

        delete[] rowsol;
        delete[] colsol;
        delete[] u;
        delete[] v;
        for (int i = 0; i < n; i++) delete[] costMatrix[i];
        delete[] costMatrix;

    } while (std::next_permutation(perm.begin(), perm.end()));

    return min_total;
}
#include <cassert>
#include <vector>

int main() {
    // Test 1: single voter, identical
    std::vector<std::vector<int>> el1 = {{0,1,2}};
    std::vector<std::vector<int>> el2 = {{0,1,2}};
    assert(computeSwapDistance(el1, el2) == 0);

    // Test 2: n=2, m=2, el1 and el2 identical
    el1 = {{0,1},{1,0}};
    el2 = {{0,1},{1,0}};
    assert(computeSwapDistance(el1, el2) == 0);

    // Test 3: n=1, m=2, opposite rankings -> after optimal relabel, distance 0
    el1 = {{0,1}};
    el2 = {{1,0}};
    assert(computeSwapDistance(el1, el2) == 0);

    // Test 4: n=2, m=3, case where minimal total = 2
    el1 = {{0,1,2},{0,2,1}};
    el2 = {{0,1,2},{1,0,2}};
    assert(computeSwapDistance(el1, el2) == 2);

    // Test 5: m=1, any n
    el1 = {{0},{0}};
    el2 = {{0},{0}};
    assert(computeSwapDistance(el1, el2) == 0);

    // Test 6: n=2, m=3, identical but shifted? already tested
    el1 = {{0,1,2},{2,1,0}};
    el2 = {{0,1,2},{2,1,0}};
    assert(computeSwapDistance(el1, el2) == 0);
}
// The core approach: For each candidate permutation `sigma` of `{0,...,m-1}`, we relabel each voter in `el1` by mapping candidate `el1[i][t]` to `sigma[el1[i][t]]`, producing a relabeled ranking. For each pair of voters `(i,j)` from relabeled `el1` and original `el2`, compute the swap distance: count ordered pairs `(a,b)` of candidates such that the relative order differs between the two rankings. This is done efficiently in O(m^2) per pair by direct comparison. Build an `n x n` cost matrix `C[i][j]` = that distance. Then solve the assignment problem minimizing sum of `C[i][rowsol[i]]` using `lap`, which returns the minimal total cost for that permutation. Keep the minimum over all permutations. Edge cases: `n=1` yields a trivial assignment (distance = swap distance between the two single voters). `m=1` has only one permutation and swap distance always 0. For `m=0` (not possible per constraints) would be degenerate. Time complexity: `O(m! * n^2 * m^2)` for building matrices plus `O(m! * n^3)` for the assignment solver (since lap is O(n^3)), but with n ≤ 20 and m ≤ 8, this is feasible (40320 * 400 * 64 ≈ 1 billion for the matrix building, but n=20 is worst-case; typical smaller. The assignment solver is called m! times, each O(n^3) so 40320 * 8000 = 322M, acceptable in C++). Space is O(n^2) for the matrix plus O(n) for lap's arrays, per permutation.
