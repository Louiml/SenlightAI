/*
Write a C++ function `minimumCutCost` that takes four parameters: an integer `m` (the number of horizontal pieces plus one, meaning there are `m-1` horizontal cut lines), an integer `n` (the number of vertical pieces plus one, meaning there are `n-1` vertical cut lines), a vector of integers `horizontalCut` (costs for each horizontal cut, with size `m-1`), and a vector of integers `verticalCut` (costs for each vertical cut, with size `n-1`). The function must simulate the process of cutting a `m` by `n` chocolate bar into unit squares using a sequence of full-length cuts. Each cut is either horizontal or vertical; horizontal cuts split a current piece across its full width (and the cost is multiplied by the current number of vertical segments), while vertical cuts split a current piece across its full height (cost multiplied by the current number of horizontal segments). You may perform cuts in any order, and you must return the minimum possible total cost to separate all unit squares. Assume all cut costs are positive integers and the input vectors are valid (sizes exactly `m-1` and `n-1`). Your solution must be efficient for large inputs (up to 100,000 cuts).
*/

#include <vector>
#include <algorithm>
#include <cstdint>

// Compute the minimum total cost to cut an m x n chocolate bar into unit squares.
// horizontalCut has size m-1, verticalCut has size n-1, all costs positive.
long long minimumCutCost(int m, int n, std::vector<int>& horizontalCut, std::vector<int>& verticalCut) {
    // Sort costs in descending order so the most expensive cuts come first.
    std::sort(horizontalCut.begin(), horizontalCut.end(), std::greater<int>());
    std::sort(verticalCut.begin(), verticalCut.end(), std::greater<int>());

    int h = 0; // index into horizontalCut
    int v = 0; // index into verticalCut
    long long hSegments = 1; // current number of horizontal segments (pieces across height)
    long long vSegments = 1; // current number of vertical segments (pieces across width)
    long long totalCost = 0;

    // Greedily choose the most expensive next cut.
    while (h < m - 1 && v < n - 1) {
        if (horizontalCut[h] >= verticalCut[v]) {
            totalCost += static_cast<long long>(horizontalCut[h]) * vSegments;
            ++h;
            ++hSegments;
        } else {
            totalCost += static_cast<long long>(verticalCut[v]) * hSegments;
            ++v;
            ++vSegments;
        }
    }

    // If only vertical cuts remain.
    while (v < n - 1) {
        totalCost += static_cast<long long>(verticalCut[v]) * hSegments;
        ++v;
        ++vSegments; // not strictly needed but keeps logic consistent
    }

    // If only horizontal cuts remain.
    while (h < m - 1) {
        totalCost += static_cast<long long>(horizontalCut[h]) * vSegments;
        ++h;
        ++hSegments;
    }

    return totalCost;
}

#include <cassert>
#include <vector>

int main() {
    {
        int m = 3, n = 2;
        std::vector<int> horizontalCut = {1, 3};
        std::vector<int> verticalCut = {5};
        assert(minimumCutCost(m, n, horizontalCut, verticalCut) == 13);
    }
    {
        int m = 2, n = 2;
        std::vector<int> horizontalCut = {7};
        std::vector<int> verticalCut = {4};
        assert(minimumCutCost(m, n, horizontalCut, verticalCut) == 15);
    }
    {
        int m = 3, n = 3;
        std::vector<int> horizontalCut = {2, 1};
        std::vector<int> verticalCut = {3, 4};
        // Sort desc: h: 2,1; v: 4,3. Steps: v(4)*1=4, h(2)*2=4, v(3)*2=6, h(1)*3=3 => total 17
        assert(minimumCutCost(m, n, horizontalCut, verticalCut) == 17);
    }
    {
        int m = 1, n = 5;
        std::vector<int> horizontalCut = {};
        std::vector<int> verticalCut = {5, 3, 2, 1};
        assert(minimumCutCost(m, n, horizontalCut, verticalCut) == 5 + 3 + 2 + 1);
    }
    {
        int m = 5, n = 1;
        std::vector<int> horizontalCut = {9, 8, 7, 6};
        std::vector<int> verticalCut = {};
        assert(minimumCutCost(m, n, horizontalCut, verticalCut) == 9 + 8 + 7 + 6);
    }
    {
        int m = 2, n = 3;
        std::vector<int> horizontalCut = {100};
        std::vector<int> verticalCut = {1, 1};
        // First do horizontal (100)*1=100, then vertical costs 1 each multiplied by 2 segments = 2+2 => total 104
        assert(minimumCutCost(m, n, horizontalCut, verticalCut) == 104);
    }
    {
        int m = 4, n = 4;
        std::vector<int> horizontalCut = {5, 5, 5};
        std::vector<int> verticalCut = {5, 5, 5};
        // All costs equal, order does not matter; total = 5*1 + 5*2 + 5*3 + 5*1 + 5*2 + 5*3 = 60
        assert(minimumCutCost(m, n, horizontalCut, verticalCut) == 60);
    }
    {
        int m = 5, n = 5;
        std::vector<int> horizontalCut = {1, 2, 3, 4};
        std::vector<int> verticalCut = {4, 3, 2, 1};
        // Sort desc both: h:4,3,2,1; v:4,3,2,1. Equal costs, merge: pick h first. Steps:
        // h(4)*1=4, v(4)*2=8, h(3)*2=6, v(3)*3=9, h(2)*3=6, v(2)*4=8, h(1)*4=4, v(1)*5=5 => total 50
        assert(minimumCutCost(m, n, horizontalCut, verticalCut) == 50);
    }
    return 0;
}

// The problem is a classic greedy cutting problem. The key observation is that each cut’s cost is multiplied by the number of segments it crosses, which grows as you make more cuts. To minimize total cost, you should always perform the most expensive cut next, because delaying an expensive cut means it will be multiplied by a larger number of segments. Therefore, sort both `horizontalCut` and `verticalCut` in descending order and use a two-pointer merge-like loop similar to merging sorted arrays. Maintain counters `hSegments` (initially 1) and `vSegments` (initially 1). At each step, compare the next largest horizontal cost and vertical cost. If the horizontal cost is greater or equal, perform that horizontal cut: add `horizontalCut[i] * vSegments` to the answer and increment `hSegments`. Otherwise perform the vertical cut: add `verticalCut[j] * hSegments` and increment `vSegments`. Continue until all cuts are used. Edge cases: when only one type of cut remains (e.g., all horizontal cuts are done), the remaining vertical cuts each cost `verticalCut[j] * m` because there are now `m` horizontal segments. Similarly, remaining horizontal cuts cost `horizontalCut[i] * n`. The algorithm is `O((m-1) log(m-1) + (n-1) log(n-1))` due to sorting, and uses `O(1)` extra space besides the input vectors. The greedy choice is optimal because the cost multiplier for any cut is non-decreasing over time, and swapping two cuts with different costs always yields higher total cost if you do the cheaper one first.
