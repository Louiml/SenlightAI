Given an array of n positive integers representing block heights, m towers initially empty, and a maximum allowed height difference x, write a C++ function `assignHeights` that takes the array, n, m, and x as parameters and returns a vector of n integers indicating which tower (1-indexed) each block should be assigned to, such that after all assignments, the final heights of any two towers differ by at most x. It is guaranteed that such an assignment always exists. The function should use a greedy min-heap approach: repeatedly assign each block to the currently shortest tower. The output vector should list the tower index for each block in the original order.

#include <cassert>
#include <vector>
#include <iostream>

// Include the solution function here or above

int main() {
    // Test case 1: basic
    std::vector<int> h1 = {1, 2, 3};
    auto r1 = assignHeights(h1, 2, 3);
    // Validate that assignments are valid: each tower's sum differences <= 3
    // We compute tower sums manually
    std::vector<long long> sums(2, 0);
    for (size_t i = 0; i < h1.size(); ++i) {
        sums[r1[i]-1] += h1[i];
    }
    assert(std::abs(sums[0] - sums[1]) <= 3);

    // Test case 2: single tower
    std::vector<int> h2 = {5, 10, 15};
    auto r2 = assignHeights(h2, 1, 20);
    assert(r2.size() == 3);
    for (int idx : r2) assert(idx == 1);

    // Test case 3: many blocks, large m
    std::vector<int> h3 = {4, 4, 4, 4, 4};
    auto r3 = assignHeights(h3, 3, 4);
    std::vector<long long> sums3(3, 0);
    for (size_t i = 0; i < h3.size(); ++i) {
        sums3[r3[i]-1] += h3[i];
    }
    for (int i = 0; i < 3; ++i) {
        for (int j = i+1; j < 3; ++j) {
            assert(std::abs(sums3[i] - sums3[j]) <= 4);
        }
    }

    // Test case 4: empty input
    std::vector<int> h4 = {};
    auto r4 = assignHeights(h4, 5, 10);
    assert(r4.empty());

    // Test case 5: large values, ensure correctness via greedy property
    std::vector<int> h5 = {100, 1, 1, 1, 1};
    auto r5 = assignHeights(h5, 2, 100);
    std::vector<long long> sums5(2, 0);
    for (size_t i = 0; i < h5.size(); ++i) {
        sums5[r5[i]-1] += h5[i];
    }
    assert(std::abs(sums5[0] - sums5[1]) <= 100);

    std::cout << "All tests passed!" << std::endl;
    return 0;
}

#include <vector>
#include <queue>
#include <utility>

// Assign each block to a tower such that final tower heights differ by at most x.
// Guaranteed that a valid assignment exists. Returns tower indices (1-indexed) for each block.
std::vector<int> assignHeights(const std::vector<int>& heights, int m, int x) {
    // Min-heap of pairs (current_height, tower_index)
    std::priority_queue<std::pair<long long, int>, 
                        std::vector<std::pair<long long, int>>, 
                        std::greater<std::pair<long long, int>>> pq;
    
    // Initialize all towers with height 0
    for (int i = 1; i <= m; ++i) {
        pq.push({0, i});
    }
    
    std::vector<int> result;
    result.reserve(heights.size());
    
    for (int h : heights) {
        auto top = pq.top();
        pq.pop();
        top.first += h; // add block height
        result.push_back(top.second); // record tower index
        pq.push(top); // push updated height
    }
    
    return result;
}

// The problem is a classic "load balancing" with a fairness constraint. The greedy strategy works because we always place the next block on the tower with the current minimum height. This ensures that, at any point, the difference between the tallest and shortest towers is minimized. At the end, because the initial difference is 0 (all towers start at 0), and we always level the shortest tower, the final maximum difference cannot exceed the largest single block height. However, the problem guarantees that x is at least the maximum block height (or that a valid assignment exists). Actually, the greedy approach guarantees the final max difference is at most the maximum block height, so if x is at least that, it works. If x is smaller, no solution exists, but the problem states one exists. The algorithm uses a min-heap (priority_queue) storing pairs (current_height, tower_index). Initially, push all m towers with height 0. For each block, pop the smallest, add the block height, record the tower index, and push back. Edge cases: m can be 1 (all blocks go to same tower, difference 0), n can be 0 (empty output). Time complexity: O(n log m) for n heap operations. Space: O(m) for the heap.
