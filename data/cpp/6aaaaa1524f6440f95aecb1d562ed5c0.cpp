Write a C++ function that, given a non-empty vector of integers, returns the minimum total cost to combine all numbers into one number. At each step, you may pick any two numbers, remove them from the set, and add their sum back to the set; the cost of that step is the sum of the two chosen numbers. The total cost is the sum of costs over all steps until only one number remains. The input vector may contain duplicate values, negative numbers, and can be of any length (including 1, in which case the total cost is 0). The function must handle large inputs efficiently using a min-heap (priority queue) to always combine the two smallest current numbers, which is guaranteed to minimize the total cost.

// The optimal strategy is to always combine the two smallest numbers currently available. This is a classic greedy approach (similar to Huffman coding) because merging the smallest pair minimizes the immediate cost and also keeps the intermediate sums as small as possible for future merges. The algorithm pushes all elements into a min-heap (priority queue with `greater<int>`). While more than one element remains, the two smallest values are popped, their sum is added to the answer, and the sum is pushed back. This repeats until the heap has size 1. Edge cases: if the vector has size 1, the loop never executes and the answer is 0; negative numbers are handled naturally by the heap ordering and arithmetic; duplicates do not require special treatment. Time complexity is O(n log n) due to n heap insertions and O(n) heap pops/pushes total (each pop/push is O(log n)), and space complexity is O(n) for the heap. Const-correctness: the input vector is passed by const reference since it is only read, not modified.

#include <vector>
#include <queue>

// Returns the minimum total cost to combine all integers in `numbers` into one.
int minCombinationCost(const std::vector<int>& numbers) {
    if (numbers.empty()) {
        return 0;
    }

    // Min-heap to always access the two smallest current numbers.
    std::priority_queue<int, std::vector<int>, std::greater<int>> minHeap;

    for (int value : numbers) {
        minHeap.push(value);
    }

    int totalCost = 0;

    while (minHeap.size() > 1) {
        int first = minHeap.top();
        minHeap.pop();
        int second = minHeap.top();
        minHeap.pop();

        int sum = first + second;
        totalCost += sum;
        minHeap.push(sum);
    }

    return totalCost;
}

#include <cassert>
#include <vector>

int minCombinationCost(const std::vector<int>& numbers); // declaration from solution

int main() {
    // Single element: no merges needed, cost = 0
    assert(minCombinationCost({5}) == 0);

    // Two elements: one merge cost = 2 + 3 = 5
    assert(minCombinationCost({2, 3}) == 5);

    // Classic example with positive numbers
    // Merge 1+2=3 (cost 3), then 3+3=6 (cost 6), total 9
    assert(minCombinationCost({1, 2, 3}) == 9);

    // Duplicate values
    // Merge 2+2=4 (cost 4), then 4+5=9 (cost 9), total 13
    assert(minCombinationCost({2, 2, 5}) == 13);

    // Negative numbers: merge -3+ -1 = -4 (cost -4), then -4+2 = -2 (cost -2), total -6
    assert(minCombinationCost({-3, -1, 2}) == -6);

    // Larger test: merge 1+2=3 (3), 3+3=6 (6), 6+4=10 (10), total 19
    assert(minCombinationCost({1, 2, 3, 4}) == 19);

    // Mixed positives and negatives: 
    // merge -10+1=-9 (cost -9), -9+2=-7 (cost -7), -7+100=93 (cost 93), total 77
    assert(minCombinationCost({-10, 1, 2, 100}) == 77);

    // Empty vector (even though spec says non-empty, handle gracefully)
    assert(minCombinationCost({}) == 0);

    // Large list with many equal numbers: all 10s, four items
    // 10+10=20 (cost 20), 20+10=30 (cost 30), 30+10=40 (cost 40), total 90
    assert(minCombinationCost({10, 10, 10, 10}) == 90);

    return 0;
}
