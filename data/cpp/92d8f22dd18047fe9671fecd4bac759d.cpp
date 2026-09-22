Given a grid of dimensions \( v \) rows and \( h \) columns where each cell contains a non-negative integer cost, a number of items \( n \) (1 ≤ n ≤ 5) to place in the grid, and parameters for two colors \( c_1, r_1, m_1 \) and \( c_2, r_2, m_2 \), write a C++ function that computes the minimum total cost. For each row independently, you must choose a vertical split position \( k_1 \) (0 ≤ k_1 ≤ h) and \( k_2 = h - k_1 \). The cost for a row includes: summing \( c_1 \times s[i][j] \) for columns \( 0 \) to \( k_1 \) inclusive (where index 0 is defined as cost 0, so effectively indexes 1..k1), summing \( c_2 \times s[i][k] \) for columns \( k_1+1 \) to \( h \) (i.e., k from h down to k1+1), plus a penalty of \( (k_1 - k_2 - 1) \times r_1 \) if \( k_1 > k_2 \), or \( (k_2 - k_1 - 1) \times r_2 \) if \( k_2 > k_1 \). For each row, take the minimum cost over all possible \( k_1 \). Then, for the whole grid, you must select exactly \( n \) rows (distinct, and each selected row must be separated by at least one non-selected row, i.e., index difference at least 2) to place items. The total cost is the sum of the per‑row minimum costs for the selected rows, plus an extra term \( (m_1^2 + m_2^2) \times (\text{last selected row index} - \text{first selected row index}) \). The function must return the minimal possible total cost. You may assume all inputs are integers and the grid size is small (v ≤ 15, h ≤ 300). Return the minimum total cost as an integer.

#include <cassert>
#include <vector>
#include <iostream>

// Include the solution function here or link it.

int main() {
    // Test case 1: Simple 1 row, 1 item, no extra cost
    std::vector<std::vector<int>> s1(2, std::vector<int>(4, 0));
    // v=1, h=3, fill row 1 with values 1,2,3 (indices 1..3)
    s1[1][1] = 1; s1[1][2] = 2; s1[1][3] = 3;
    int result1 = minimumTotalCost(s1, 1, 3, 1, 1, 5, 1, 2, 7, 1);
    // Row cost: try k1=0: cost = c2*(1+2+3)=2*6=12, no penalty
    // k1=1: c1*1 + c2*(2+3)=1+10=11
    // k1=2: c1*(1+2)+c2*3=3+6=9
    // k1=3: c1*(1+2+3)=6
    // min=6. total = 6 + (1+1)*(1-1)=6. Assert.
    assert(result1 == 6);

    // Test case 2: 2 rows, 1 item, should pick min row cost
    std::vector<std::vector<int>> s2(3, std::vector<int>(4, 0));
    // v=2, h=3, row1: [10,10,10], row2: [1,1,1]
    for (int j=1; j<=3; ++j) {
        s2[1][j] = 10;
        s2[2][j] = 1;
    }
    // Row1 min cost: k1=0: c2*30=2*30=60, k1=3: c1*30=1*30=30, min=30
    // Row2 min cost: k1=0: 2*3=6, k1=3: 3, min=3
    int result2 = minimumTotalCost(s2, 2, 3, 1, 1, 5, 1, 2, 7, 1);
    // Choose row2: cost=3 + (1+1)*(2-2)=3. Assert.
    assert(result2 == 3);

    // Test case 3: 2 rows, need 2 items => must pick both rows (separation diff 1? Actually diff=1 <2, so invalid). 
    // But input guarantees valid, so we test a valid case with 3 rows, n=2.
    std::vector<std::vector<int>> s3(4, std::vector<int>(2, 0)); // h=1, v=3
    // All rows have a single column value say 5, but we can set row values.
    s3[1][1] = 5; s3[2][1] = 1; s3[3][1] = 5;
    // For h=1: each row has k1=0 or 1. For k1=0: cost = c2*val. For k1=1: cost=c1*val.
    // Let's set c1=1, c2=1, r1=r2=0, m1=m2=1.
    // Row costs: row1: min(1*5,1*5)=5; row2: min(1,1)=1; row3:5.
    int result3 = minimumTotalCost(s3, 3, 1, 2, 1, 0, 1, 1, 0, 1);
    // Possible combinations: (1,3) diff=2 valid. cost=5+5+ (1+1)*(3-1)=10+4=14
    // (1,2) invalid, (2,3) invalid. So answer 14.
    assert(result3 == 14);

    // Test case 4: Edge case n=1 with v=5, ensure works
    std::vector<std::vector<int>> s4(6, std::vector<int>(2, 0));
    for (int i=1; i<=5; ++i) s4[i][1] = i;
    // h=1, c1=1, c2=1, r1=r2=0, m1=m2=1
    int result4 = minimumTotalCost(s4, 5, 1, 1, 1, 0, 1, 1, 0, 1);
    // min row cost is 1 (row1) => 1 + (1+1)*0 = 1
    assert(result4 == 1);

    // Test case 5: Check penalty with imbalance
    // v=1, h=2, c1=1, c2=100, r1=1000, r2=0
    std::vector<std::vector<int>> s5(2, std::vector<int>(3, 0));
    s5[1][1] = 1; s5[1][2] = 1;
    // k1=0: cost = c2*(1+1)=200, k2=2 > k1=0 => penalty (2-0-1)*r2 = 1*0=0 => 200
    // k1=1: cost = c1*1 + c2*1 = 1+100=101, k1==k2 no penalty => 101
    // k1=2: cost = c1*(1+1)=2, k1=2>k2=0 => penalty (2-0-1)*r1=1*1000=1000 => 1002
    // min=101. Total = 101 + (m1^2+m2^2)*0. Let m1=0,m2=0 => 101.
    int result5 = minimumTotalCost(s5, 1, 2, 1, 1, 1000, 0, 100, 0, 0);
    assert(result5 == 101);

    std::cout << "All tests passed!" << std::endl;
    return 0;
}

#include <vector>
#include <algorithm>
#include <limits>

// Compute minimum total cost for placing n items on a grid.
// Parameters:
//   s: 2D vector of size (v+1) x (h+1), indices 1..v and 1..h used.
//   v: number of rows
//   h: number of columns
//   n: number of items (1..5)
//   c1, r1, m1, c2, r2, m2: cost parameters
// Returns minimal total cost as integer.
int minimumTotalCost(const std::vector<std::vector<int>>& s, int v, int h,
                     int n, int c1, int r1, int m1,
                     int c2, int r2, int m2) {
    // Compute per-row minimum cost for each row.
    std::vector<int> rowCost(v + 1, std::numeric_limits<int>::max());
    for (int i = 1; i <= v; ++i) {
        int best = std::numeric_limits<int>::max();
        for (int k1 = 0; k1 <= h; ++k1) {
            int k2 = h - k1;
            int cost = 0;
            // Left part: columns 1..k1 (since index 0 is unused, effectively zero)
            for (int j = 1; j <= k1; ++j) {
                cost += c1 * s[i][j];
            }
            // Right part: columns k1+1..h
            for (int k = h; k > k1; --k) {
                cost += c2 * s[i][k];
            }
            // Penalty for imbalance
            if (k1 > k2) {
                cost += (k1 - k2 - 1) * r1;
            } else if (k2 > k1) {
                cost += (k2 - k1 - 1) * r2;
            }
            best = std::min(best, cost);
        }
        rowCost[i] = best;
    }

    int finalCost = std::numeric_limits<int>::max();
    int separationCostMultiplier = m1 * m1 + m2 * m2;

    // Generate all combinations of n rows with separation at least 2.
    // Since n <= 5 and v <= 15, simple recursion or nested loops.
    // Use a helper lambda for recursion.
    std::vector<int> selected;
    // Recursively build combinations.
    std::function<void(int start, int count)> search = [&](int start, int count) {
        if (count == n) {
            // Compute cost for this selection
            int sum = 0;
            for (int idx : selected) sum += rowCost[idx];
            int first = selected.front();
            int last = selected.back();
            sum += separationCostMultiplier * (last - first);
            finalCost = std::min(finalCost, sum);
            return;
        }
        // Need at least (n - count - 1) rows after the current to allow separation.
        int maxStart = v - 2 * (n - count); // because need 2 gaps for each remaining after current? Actually careful.
        // Simpler: iterate from start to v, but ensure enough remaining.
        for (int idx = start; idx <= v; ++idx) {
            // Check if enough rows left to place remaining items with separation 2
            int remaining = n - count - 1;
            if (idx + 2 * remaining > v) continue; // not enough space
            selected.push_back(idx);
            search(idx + 2, count + 1); // next must be at least idx+2
            selected.pop_back();
        }
    };

    search(1, 0);

    return finalCost;
}

// The problem is solved in two stages.  
// **Stage 1 – Per‑row cost:** For each row \( i \) (1-indexed), iterate over all possible split positions \( k_1 \) from 0 to \( h \). For each \( k_1 \), compute the cost as described: sum left part using \( c_1 \), sum right part using \( c_2 \), add penalty if unbalanced. Track the minimum for that row and store it in an array `rowCost[i]`. This is \( O(v \times h^2) \) if done naively per row because for each split we sum up to \( h \) cells. However, given small constraints (h up to 300, v up to 15), this is acceptable. Edge cases: when \( k_1 = 0 \), left part has no cells (the loop from j=0 to 0 adds \( c1* s[i][0] \) which is zero since input is 1-indexed, so effectively no left cells). When \( k_1 = h \), right part has no cells (inner loop runs from h down to h+1, so no iterations). Also handle penalty correctly: if \( k_1 = k_2 \), no penalty.  
// **Stage 2 – Selecting rows:** We need to choose \( n \) rows such that indices are at least 2 apart (i.e., difference ≥ 2). For each valid combination, total cost = sum of `rowCost` of selected rows + \( (m_1^2 + m_2^2) \times (\text{last} - \text{first}) \). Since \( n \le 5 \) and \( v \le 15 \), we can brute force all combinations. For each combination, check the separation condition. Keep the minimum over all combinations. Edge cases: if \( n = 1 \), any row is valid (difference from itself is 0), so just take min of `rowCost`. If `n` is larger than possible valid combinations (e.g., v=2, n=2 is impossible because need diff ≥2), then the problem guarantees there is a valid solution (so we assume valid input).  
// **Complexity:** Stage 1: For each row, we have \( O(h) \) splits, each split costs \( O(h) \) to sum, so \( O(v h^2) \). With v≤15, h≤300, worst-case ~1.35M operations, fine. Stage 2: Number of combinations is at most \( \binom{v}{n} \) but with separation constraint reduced; worst-case v=15, n=5 gives at most C(15,5)=3003, each checked quickly, so negligible. Total time \( O(v h^2 + \binom{v}{n}) \), space \( O(v) \) for rowCost.
