/*
Write a C++ function `minimumTotalTime` that takes four parameters: an integer `a` (the maximum time one explosion can contribute per bomb), an integer `b` (the initial time counter), an integer `n` (the number of bombs), and a vector of integers `nums` (the individual bomb timers). The function must return the minimum total time required to defuse all bombs, where each bomb with a timer less than or equal to `a` adds its full timer value to the total, while bombs with a timer greater than `a` only add `a`. The total is then incremented by `b-1` as a final constant. The input vector may be unsorted and contain positive integers only, and the function must not modify the original vector if it is passed by const reference. The result must fit in a 32-bit signed integer.
*/
#include <vector>
#include <algorithm>

// Returns the minimum total time to defuse all bombs.
// a: maximum contribution per bomb, b: initial counter, n: number of bombs, nums: bomb timers.
// The function does not modify the input vector.
int minimumTotalTime(int a, int b, int n, const std::vector<int>& nums) {
    // Create a local copy to sort without modifying the caller's data.
    std::vector<int> timers = nums;
    std::sort(timers.begin(), timers.end());
    
    long long total = 0; // use long long to avoid overflow in intermediate sum
    for (int i = 0; i < n; ++i) {
        if (timers[i] <= a) {
            total += timers[i];
        } else {
            total += a;
        }
    }
    
    total += (b - 1);
    return static_cast<int>(total);
}
#include <cassert>
#include <vector>

// Function declaration
int minimumTotalTime(int a, int b, int n, const std::vector<int>& nums);

int main() {
    // Test 1: Basic case with mixed timers
    std::vector<int> v1 = {2, 5, 1, 4, 3};
    assert(minimumTotalTime(3, 10, 5, v1) == (1+2+3+3+3) + 9); // sum = 12, +9 = 21
    
    // Test 2: All timers <= a
    std::vector<int> v2 = {1, 2, 3};
    assert(minimumTotalTime(5, 1, 3, v2) == (1+2+3) + 0); // sum=6, +0=6
    
    // Test 3: All timers > a
    std::vector<int> v3 = {10, 20, 30};
    assert(minimumTotalTime(7, 5, 3, v3) == (7+7+7) + 4); // sum=21 +4=25
    
    // Test 4: Single bomb timer exactly equal to a
    std::vector<int> v4 = {4};
    assert(minimumTotalTime(4, 0, 1, v4) == 4 + (-1)); // 3
    
    // Test 5: Empty vector (n=0)
    std::vector<int> v5;
    assert(minimumTotalTime(2, 3, 0, v5) == 2); // b-1 = 2
    
    // Test 6: Negative b (possible if b < 1)
    std::vector<int> v6 = {1, 100};
    assert(minimumTotalTime(50, 0, 2, v6) == (1+50) + (-1) ); // 50
    
    // Test 7: Vector with duplicate values
    std::vector<int> v7 = {3, 3, 3};
    assert(minimumTotalTime(3, 2, 3, v7) == (3+3+3) +1); // 10
    
    // Test 8: Unsorted large values
    std::vector<int> v8 = {1000, 1, 500, 200, 100};
    assert(minimumTotalTime(100, 7, 5, v8) == (1+100+100+100+100) + 6); // 401+6=407
    
    return 0;
}
// The solution is straightforward if we sort the vector of bomb timers in ascending order. After sorting, we iterate through each timer. For any timer value that is less than or equal to `a`, we add the full timer value to the cumulative sum. For any timer greater than `a`, we add `a` instead. This is because the constraint allows at most `a` time contribution from any single bomb regardless of its actual timer. After processing all bombs, we add the constant `b-1` to the sum and return it. Sorting is necessary to ensure we process timers in any order, but since the contribution rule depends only on each individual timer relative to `a`, ordering doesn't change the final sum. However, sorting is still a common practice to clarify the logic and handle potential future modifications. Edge cases include when `n=0` (empty vector), in which case the sum should be `b-1`; when all timers are less than `a`, the sum is the sum of all timers plus `b-1`; when all timers are greater than `a`, the sum is `n*a + b-1`. Time complexity is O(n log n) due to sorting, and space complexity is O(1) auxiliary (excluding the input vector copy if passed by value). We can avoid a copy by sorting a copy if the original must not be modified, but the problem statement allows modifying the vector if passed by non-const reference; however, we'll design the function to take a const reference and create a local copy for sorting to ensure const correctness.
