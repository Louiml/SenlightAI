/*
Write a C++ function that takes a vector of positive integers and returns the minimum number of times you must repeatedly select the largest element in the current array, halve it (replacing the original value with its half), and continue until the total sum of all elements in the array becomes at most half of the original total sum. For example, given `[5, 19, 8, 1]`, the original sum is 33, half is 16.5; halving 19 → 9.5 makes sum 23.5 (still >16.5), halving 9.5 → 4.75 makes sum 18.25 (still >16.5), halving 8 → 4 makes sum 14.25 (≤16.5), so the answer is 3. All numbers are positive integers, and the vector is non-empty.
*/

#include <vector>
#include <queue>

// Returns the minimum number of halving operations on the current largest element
// needed to reduce the total sum of the vector to at most half its original sum.
int minHalvingsToHalveSum(std::vector<int>& nums) {
    double current_sum = 0.0;
    for (int val : nums) {
        current_sum += val;
    }
    const double original_sum = current_sum;
    
    std::priority_queue<double> max_heap;
    for (int val : nums) {
        max_heap.push(static_cast<double>(val));
    }
    
    int operations = 0;
    while ((original_sum - current_sum) < (original_sum / 2.0)) {
        ++operations;
        double largest = max_heap.top();
        max_heap.pop();
        current_sum -= largest;
        current_sum += largest / 2.0;
        max_heap.push(largest / 2.0);
    }
    return operations;
}

#include <cassert>
#include <vector>

int main() {
    std::vector<int> v1 = {5, 19, 8, 1};
    assert(minHalvingsToHalveSum(v1) == 3);
    
    std::vector<int> v2 = {1};
    assert(minHalvingsToHalveSum(v2) == 1);
    
    std::vector<int> v3 = {1, 1, 1, 1};
    // sum=4, half=2, operations: 1->0.5 sum=3.5, 1->0.5 sum=3, 1->0.5 sum=2.5, 1->0.5 sum=2 (stop) => 4
    assert(minHalvingsToHalveSum(v3) == 4);
    
    std::vector<int> v4 = {100};
    assert(minHalvingsToHalveSum(v4) == 1);
    
    std::vector<int> v5 = {10, 10};
    // sum=20 half=10, halve 10->5 sum=15, halve 5->2.5 sum=12.5, halve 10->5 sum=7.5 => 3? Let's simulate: original 20, target<=10. Step1: largest=10, sum=15, step2: largest=10 (other), sum=10 → stop. Actually 2 operations enough. Check: after halving one 10 -> [5,10] sum=15 >10. Halve 10 -> [5,5] sum=10 → stop. So ans=2.
    assert(minHalvingsToHalveSum(v5) == 2);
    
    std::vector<int> v6 = {2, 2, 2};
    // sum=6 half=3, halve 2->1 sum=5, halve 2->1 sum=4, halve 2->1 sum=3 => 3 operations.
    assert(minHalvingsToHalveSum(v6) == 3);
    
    std::vector<int> v7 = {3, 3, 3, 3};
    // sum=12 half=6, halve 3->1.5 sum=10.5, halve 3->1.5 sum=9, halve 3->1.5 sum=7.5, halve 3->1.5 sum=6 → 4 ops.
    assert(minHalvingsToHalveSum(v7) == 4);
}

// The key is to greedily always halve the current maximum element, because halving a larger number reduces the total sum more than halving a smaller number. Use a max-heap (priority_queue) to always access the largest current value in O(log n) time. Maintain two sums: `original_sum` (fixed) and `current_sum` (updated after each halving). Stop when `current_sum` is less than or equal to `original_sum / 2`. Each operation: pop the max, subtract it from `current_sum`, add its half, push the half back, and increment the answer counter. Edge case: if the vector has only one element, halving it once will always reduce the sum to half (exactly) or below, so answer is 1. If the sum is already zero (not possible since positive integers), but if all numbers are small, still at least 1 operation is needed unless the original sum is already ≤ half? Since original sum is positive and half is strictly smaller, you always need at least 1 operation. Time complexity: O(k log n) where k is the number of operations, and k can be up to O(n * log(max_value)) because each halving roughly halves a value. Space complexity: O(n) for the heap.
