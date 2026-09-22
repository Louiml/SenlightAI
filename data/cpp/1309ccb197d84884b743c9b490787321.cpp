/*
Write a C++ function `long long minConnectionCost(vector<int>& ropes)` that, given a list of rope lengths, repeatedly connects the two shortest ropes (using a min-heap) until only one rope remains. The cost of each connection is the sum of the two chosen ropes taken modulo `1'000'000'007`; accumulate the total cost modulo the same constant. The function should return the minimal total cost. The input vector may be empty (return 0) or contain up to 10^5 integers, each between 1 and 10^6; the function must handle large values without overflow by using `long long` for the accumulator and modulo after each addition.
*/

#include <queue>
#include <vector>

// Returns the minimum total cost to connect all ropes, with each merge cost
// and the total taken modulo 1'000'000'007.
long long minConnectionCost(std::vector<int>& ropes) {
    const long long MOD = 1000000007LL;
    if (ropes.empty()) return 0LL;

    std::priority_queue<int, std::vector<int>, std::greater<int>> minHeap;
    for (int length : ropes) {
        minHeap.push(length);
    }

    long long totalCost = 0LL;
    while (minHeap.size() > 1) {
        int first = minHeap.top();
        minHeap.pop();
        int second = minHeap.top();
        minHeap.pop();

        long long mergeCost = (static_cast<long long>(first) + second) % MOD;
        totalCost = (totalCost + mergeCost) % MOD;

        minHeap.push(static_cast<int>(mergeCost));
    }

    return totalCost;
}

#include <cassert>
#include <vector>

// Declare the function (or include the solution header in practice)
long long minConnectionCost(std::vector<int>& ropes);

int main() {
    std::vector<int> r1 = {4, 3, 2, 6};
    assert(minConnectionCost(r1) == 29LL); // (2+3)=5, (4+5)=9, (6+9)=15 -> 5+9+15=29

    std::vector<int> r2 = {1, 2, 3};
    assert(minConnectionCost(r2) == 9LL); // (1+2)=3, (3+3)=6 -> 3+6=9

    std::vector<int> r3 = {5};
    assert(minConnectionCost(r3) == 0LL); // only one rope, no cost

    std::vector<int> r4 = {};
    assert(minConnectionCost(r4) == 0LL); // empty input

    std::vector<int> r5 = {1000000000, 1000000000}; 
    // merge cost = (1e9 + 1e9) % 1e9+7 = 2000000000 % 1000000007 = 999999993
    assert(minConnectionCost(r5) == 999999993LL);

    std::vector<int> r6 = {1, 1, 1, 1, 1};
    // merges: 1+1=2, 1+1=2, 2+1=3, 2+3=5 -> total 2+2+3+5=12
    assert(minConnectionCost(r6) == 12LL);

    return 0;
}

// The problem is the classic "Minimum Cost to Connect Ropes" (also known as Huffman coding / optimal merge pattern). The optimal strategy is to always merge the two smallest current lengths, because this minimizes the total sum of all merge costs. Use a min-heap (`priority_queue<int, vector<int>, greater<int>>`) initialized with all rope lengths. While more than one rope remains, pop the two smallest, compute `(a + b) % MOD`, add this cost to a `long long` accumulator modulo `MOD`, and push the merged length back into the heap. Edge cases: empty input returns 0; a single rope costs 0 (no merge needed). Since modulus is applied at each addition, the accumulator never overflows even for large inputs. Time complexity: building the heap is O(n), each pop/push is O(log n), and there are exactly n-1 merges, so total O(n log n). Space complexity: O(n) for the heap.
