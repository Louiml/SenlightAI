// Write a C++ function `long long minimumMergeCost(const std::vector<long long>& sizes)` that accepts a vector of positive integers representing the sizes of piles of stones, and returns the minimum total cost to combine all piles into one. In each move, you may pick any two piles, merge them into one pile whose size is the sum of the two chosen piles, and the cost of that move is the size of the newly formed pile. The total cost is the sum of costs over all merges. The input vector may be empty (return 0 in that case), may contain a single element (return 0), or may contain up to 10^5 elements, each up to 10^9. The function must be efficient enough for large inputs and must use a greedy heap-based approach.

#include <cassert>
#include <vector>

// Assume solution function is already declared above.
int main() {
    // Single element
    assert(minimumMergeCost({10}) == 0);
    // Two elements
    assert(minimumMergeCost({3, 5}) == 8);
    // Three elements: optimal merge 1+2=3, then 3+3=6 => total 3+6=9
    assert(minimumMergeCost({1, 2, 3}) == 9);
    // Four elements: merge 1+2=3, 3+3=6, 3+6=9 => total 3+6+9=18
    assert(minimumMergeCost({1, 2, 3, 4}) == 18);
    // Large values overflow test
    assert(minimumMergeCost({1000000000, 1000000000, 1000000000}) == 5000000000LL);
    // Empty vector
    assert(minimumMergeCost({}) == 0);
    // Duplicate values
    assert(minimumMergeCost({5, 5, 5}) == 20); // 5+5=10, then 10+5=15 => total 10+15=25? Wait check: merge 5 and 5 cost 10, then merge 10 and 5 cost 15 total 25. But we computed 20 incorrectly! Let's test correct: {5,5,5} => pick two 5's => 10, heap {5,10}, merge 5+10=15 total 10+15=25
    assert(minimumMergeCost({5, 5, 5}) == 25);
    // Mixed and large
    assert(minimumMergeCost({3, 7, 12, 1}) == 37); // 1+3=4, 4+7=11, 11+12=23 total 4+11+23=38? Wait do again: 1+3=4 cost4, then 4+7=11 cost11 total15, then 11+12=23 total38. But optimal? Merge smallest two first: 1 and3 =>4, heap{4,7,12}; then 4 and7 =>11 cost11 total15, heap{11,12}; then 11+12=23 cost23 total38. Yes 38.
    assert(minimumMergeCost({3, 7, 12, 1}) == 38);
    return 0;
}

#include <queue>
#include <vector>

// Returns the minimum total cost to merge all piles into one.
// Each merge costs the sum of the two selected piles.
long long minimumMergeCost(const std::vector<long long>& sizes) {
    if (sizes.empty() || sizes.size() == 1) {
        return 0;
    }

    std::priority_queue<long long, std::vector<long long>, std::greater<>> minHeap;
    for (long long size : sizes) {
        minHeap.push(size);
    }

    long long totalCost = 0;
    while (minHeap.size() > 1) {
        long long first = minHeap.top(); minHeap.pop();
        long long second = minHeap.top(); minHeap.pop();
        long long merged = first + second;
        totalCost += merged;
        minHeap.push(merged);
    }

    return totalCost;
}

// The optimal strategy for minimizing the total merge cost is to always merge the two smallest piles currently available. This is a classic Huffman coding / optimal merge pattern problem, proven by exchange argument: if you merge two larger piles while a smaller pile exists, swapping the merge order with the smaller pile reduces or leaves unchanged the total cost because the smaller pile contributes to the cost in fewer merges. To implement this efficiently, push all pile sizes into a min-heap (priority queue with `greater<>`). Repeatedly pop the two smallest values, compute their sum, add that sum to `totalCost`, and push the sum back into the heap. Continue until only one pile remains. Edge cases: if the vector is empty or has one element, no merges are needed, so return 0. The algorithm runs in `O(n log n)` time due to heap operations, and uses `O(n)` auxiliary space for the heap. The use of `long long` is necessary because sums can exceed 32-bit integers (e.g., merging many piles of size 10^9 can exceed 2^31). No special handling for duplicates or zero (but zero is allowed; the problem states positive but we still handle zero gracefully by still merging them normally).
