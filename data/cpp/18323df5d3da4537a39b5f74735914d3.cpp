Given an array of `n` positive integers (2 ≤ n ≤ 10000), write a C++ function that computes the minimal total cost of repeatedly merging adjacent elements into a single element, where each merge operation takes two adjacent elements and replaces them with their sum, and the cost of each merge is the sum of the two elements. The goal is to find the minimal possible total cost to reduce the array to a single element. The array must not be modified. Return the minimal total cost as an integer. The input array may contain duplicate values, and all values are positive so sums fit within a 32-bit integer.
This problem is equivalent to building an optimal Huffman-like merging tree, but with the restriction that only adjacent elements can be merged at each step, which changes the optimal strategy. The classic optimal solution for this adjacent-only merging problem is a greedy algorithm that repeatedly merges the smallest adjacent pair (by sum) in the current sequence. One efficient way to simulate this is to maintain a min-heap of adjacent pair sums, but since the array is small (n ≤ 10000), a simpler `O(n^2)` approach suffices: repeatedly scan the current sequence to find the adjacent pair with the smallest sum, add that sum to the total, replace that pair with their sum, and continue. This mimics the provided snippet’s behavior after sorting, but here the array is unsorted and we must not modify it. The provided snippet sorts first, which is incorrect for the general adjacent-merge problem; the correct approach is to work on the original order. Edge cases: when `n = 2`, only one merge is possible, and the total cost is simply `a[0] + a[1]`. When there are ties for the smallest adjacent sum, any choice yields the same total cost for optimality. Time complexity: `O(n^2)` in the worst case, since each merge reduces the size by one, and each scan takes `O(n)`. Space complexity: `O(n)` for a copy of the array to avoid modifying input.
#include <vector>
#include <climits>

// Compute the minimal total cost to merge all adjacent elements into one,
// where each merge combines two adjacent elements at the cost of their sum.
// The input array is not modified.
int minimalAdjacentMergeCost(const std::vector<int>& nums) {
    int n = static_cast<int>(nums.size());
    if (n < 2) return 0;

    // Work on a copy so the input remains unchanged.
    std::vector<long long> seq(nums.begin(), nums.end());
    long long totalCost = 0;

    while (seq.size() > 1) {
        // Find the adjacent pair with the smallest sum.
        int bestIndex = 0;
        long long bestSum = seq[0] + seq[1];
        for (int i = 1; i + 1 < static_cast<int>(seq.size()); ++i) {
            long long curSum = seq[i] + seq[i + 1];
            if (curSum < bestSum) {
                bestSum = curSum;
                bestIndex = i;
            }
        }

        // Add the merge cost.
        totalCost += bestSum;

        // Replace the chosen pair with their sum.
        seq[bestIndex] = bestSum;
        seq.erase(seq.begin() + bestIndex + 1);
    }

    return static_cast<int>(totalCost);
}
#include <cassert>
#include <vector>

int minimalAdjacentMergeCost(const std::vector<int>& nums);

int main() {
    // Basic case with two elements.
    assert(minimalAdjacentMergeCost({5, 7}) == 12);

    // Simple three-element case: optimal merges 2&3 first, then 1&5 => 5+6=11.
    assert(minimalAdjacentMergeCost({1, 2, 3}) == 9); // 1+2=3, then 3+3=6, total=9? Actually let's compute: merge 1+2 cost 3 => [3,3]; merge 3+3 cost 6 => total 9. Yes.

    // Another three-element: 4 1 5 => merge 1+5=6 cost 6 => [4,6]; merge 4+6=10 => total 16.
    assert(minimalAdjacentMergeCost({4, 1, 5}) == 16);

    // Duplicates: 2 2 2 => merge 2+2=4 cost 4 => [4,2]; merge 4+2=6 => total 10.
    assert(minimalAdjacentMergeCost({2, 2, 2}) == 10);

    // Four elements: 1 3 2 4 => optimal: merge 1+3=4 => [4,2,4]; merge 2+4=6 => [4,6]; merge 4+6=10 => total 20.
    assert(minimalAdjacentMergeCost({1, 3, 2, 4}) == 20);

    // Larger case that matches the snippet’s logic if sorted: but unsorted matters.
    // For [10, 1, 1, 10], optimal: merge 1+1=2 => [10,2,10]; merge 2+10=12 => [10,12]; merge 10+12=22 => total 36.
    assert(minimalAdjacentMergeCost({10, 1, 1, 10}) == 36);

    // Edge: all equal [5,5,5,5] => merge any adjacent: 10+10 (two merges) then 20 = total 40.
    // Actually: merge 5+5=10 => [10,5,5]; merge 5+5=10 => [10,10]; merge 10+10=20 => total 40.
    assert(minimalAdjacentMergeCost({5, 5, 5, 5}) == 40);

    // Large-ish: [100, 1, 100, 1] => optimal: merge 1+100=101 => [100,101,1]; merge 101+1=102 => [100,102]; merge 100+102=202 => total 405.
    assert(minimalAdjacentMergeCost({100, 1, 100, 1}) == 405);

    return 0;
}
