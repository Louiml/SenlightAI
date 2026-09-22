/*
You are given a vector of positive integers `nums` and an integer `k`. In each of exactly `k` operations, you must select the current maximum element in the vector, add its original value to a running total, then replace that selected element with the ceiling of its value divided by 3 (i.e., `ceil(value / 3.0)`). Write a C++ function `long long maxKelements(const std::vector<int>& nums, int k)` that returns the maximum possible total after performing exactly `k` operations. The vector is not modified externally (your function should work on a copy or use a priority queue). You may assume `nums` is non-empty and all values are positive integers. The answer may exceed 32-bit range, so use `long long` for the total.
*/
#include <vector>
#include <queue>

// Performs exactly k operations: each time pick the current maximum,
// add it to the total, then replace it with ceil(value / 3).
// Returns the maximum possible sum.
long long maxKelements(const std::vector<int>& nums, int k) {
    std::priority_queue<int> heap(nums.begin(), nums.end());

    long long total = 0;
    for (int i = 0; i < k; ++i) {
        int currentMax = heap.top();
        heap.pop();

        total += currentMax;

        // Integer ceiling division: (a + b - 1) / b
        int nextValue = (currentMax + 2) / 3;
        heap.push(nextValue);
    }

    return total;
}
#include <cassert>
#include <vector>

// (The solution is assumed to be included above.)
int main() {
    // Example from typical problem: nums = [10,10,10,10,10], k = 5 → sum = 50
    assert(maxKelements({10,10,10,10,10}, 5) == 50);

    // Single element, one operation: pick 9, replace with 3, sum = 9
    assert(maxKelements({9}, 1) == 9);

    // Two elements: pick 5, replace with 2; next pick 4, replace with 2; sum = 9
    assert(maxKelements({4,5}, 2) == 9);

    // Large values: pick 8, replace with 3; then pick 3, replace with 1; sum = 11
    assert(maxKelements({8,2}, 2) == 11);

    // All equal: pick 4, replace with 2; next pick 4, replace with 2; then pick 4 → sum = 12
    assert(maxKelements({4,4,4}, 3) == 12);

    // k equals size, values shrink: [5,5] k=4: 5+5+2+2 = 14
    assert(maxKelements({5,5}, 4) == 14);

    // Case with tiny k: [100,1] k=1 → pick 100, sum = 100
    assert(maxKelements({100,1}, 1) == 100);

    // Case where ceiling matters: [1] k=10 → every pick gives 1 (since ceil(1/3)=1) → sum = 10
    assert(maxKelements({1}, 10) == 10);
}
// The optimal strategy is a greedy one: always pick the current largest element because replacing a larger element with its third produces a larger next value than replacing a smaller one, so it maximizes every immediate gain and future potential. Use a max-heap (`std::priority_queue<int>`) initialized from the input vector. In each of `k` iterations, pop the top element `maxEl`, add it to the accumulator, compute `ceil(maxEl / 3.0)` using integer arithmetic: `(maxEl + 2) / 3` (which avoids floating‑point issues), and push that new value back. Edge cases: if `k` is large, the values shrink but never become zero; duplicates are handled naturally by the heap; `nums` being non‑empty guarantees a valid first pop. Time complexity: building the heap is `O(n)` (using the constructor from iterators), each of the `k` operations does a pop and a push, each `O(log n)`, so total `O(n + k log n)`. Space complexity: `O(n)` for the heap.
