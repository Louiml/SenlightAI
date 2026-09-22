// Write a C++ function `minimumRopeJoinCost` that takes a `const std::vector<int>&` representing the lengths of `n` ropes (where `n >= 1`), and returns the minimum total cost (as an `int`) to join all ropes into one. The cost to join two ropes is the sum of their lengths. You must repeatedly connect the two shortest ropes to minimize the total cost. The input vector may contain positive integers only, and there may be duplicate rope lengths. Return `0` if the vector has only one rope (no joining needed).
The optimal strategy to minimize total cost is to always merge the two smallest ropes, because each merge's cost directly adds to the total, and keeping longer ropes unmerged as long as possible reduces how many times their length is included in intermediate sums. A min-heap (priority queue) efficiently extracts the two smallest elements repeatedly. Initialize the heap with all rope lengths. While more than one rope remains, pop the two smallest, compute their sum, add that sum to the total cost, and push the sum back into the heap. This reduces the count by one each iteration, so the loop runs exactly `n-1` times. Edge cases: if the input has one rope, the loop does not run and cost remains `0`; if the vector is empty (though specified non-empty, we can handle gracefully), return `0`. With `n` ropes, each heap operation takes `O(log n)`, giving `O(n log n)` time and `O(n)` auxiliary space for the heap.
#include <queue>
#include <vector>

// Returns the minimum total cost to join all ropes using optimal merging.
int minimumRopeJoinCost(const std::vector<int>& ropes) {
    if (ropes.size() <= 1) {
        return 0; // No merges needed for zero or one rope.
    }

    // Min-heap to always get the two shortest ropes.
    std::priority_queue<int, std::vector<int>, std::greater<int>> minHeap;

    for (int length : ropes) {
        minHeap.push(length);
    }

    int totalCost = 0;

    while (minHeap.size() > 1) {
        int first = minHeap.top();
        minHeap.pop();
        int second = minHeap.top();
        minHeap.pop();

        int merged = first + second;
        totalCost += merged;
        minHeap.push(merged);
    }

    return totalCost;
}
#include <cassert>
#include <vector>

// The solution function is declared above (in the same translation unit).
int main() {
    // Single rope: no cost.
    assert(minimumRopeJoinCost({5}) == 0);

    // Two ropes: cost is their sum.
    assert(minimumRopeJoinCost({10, 20}) == 30);

    // Original example: {10, 17, 45, 89} → optimal cost = 10+17=27 → 27+45=72 → 72+89=161 → total 27+72+161=260
    assert(minimumRopeJoinCost({10, 17, 45, 89}) == 260);

    // Duplicate values: {1, 1, 1} → 1+1=2 (cost 2) → 2+1=3 (cost 3) → total 5
    assert(minimumRopeJoinCost({1, 1, 1}) == 5);

    // Already sorted input: {1, 2, 3, 4} → 1+2=3 (cost 3) → 3+3=6 (cost 6) → 6+4=10 (cost 10) → total 19
    assert(minimumRopeJoinCost({1, 2, 3, 4}) == 19);

    // Large values: {100, 100, 100} → 100+100=200 (cost 200) → 200+100=300 (cost 300) → total 500
    assert(minimumRopeJoinCost({100, 100, 100}) == 500);

    // Unsorted to verify heap: {4, 3, 2, 1} must give same as sorted: 19
    assert(minimumRopeJoinCost({4, 3, 2, 1}) == 19);

    // Two equal ropes: {7, 7} → cost 14
    assert(minimumRopeJoinCost({7, 7}) == 14);

    // Five ropes: {1, 2, 3, 4, 5}
    // 1+2=3 (cost 3) → {3,3,4,5} → 3+3=6 (cost 6) → {4,5,6} → 4+5=9 (cost 9) → {6,9} → 6+9=15 (cost 15) → total 3+6+9+15=33
    assert(minimumRopeJoinCost({1, 2, 3, 4, 5}) == 33);
}
