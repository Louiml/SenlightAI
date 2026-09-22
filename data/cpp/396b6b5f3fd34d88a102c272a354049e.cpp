// Write a C++ function that takes a vector of integers representing houses arranged in a circle and returns the maximum total amount that can be robbed without robbing two adjacent houses (adjacency includes the first and last houses because of the circle). The input vector will contain at least one house, and all values are non-negative. The function must be named `robCircular` and accept a `const std::vector<int>&` for efficiency and correctness. Your solution must handle the circular constraint correctly and operate in linear time with constant extra space.

// The core problem is a classic House Robber variant with a circle. The circular constraint means we cannot rob both the first and last houses simultaneously. The key insight is to reduce the problem to two linear (non-circular) House Robber subproblems: one that excludes the last house (rob from index 0 to n-2) and one that excludes the first house (rob from index 1 to n-1). The maximum of these two results is the answer. For the linear subproblem, we use dynamic programming with two variables: `prev` stores the maximum amount up to the previous house, and `curr` stores the maximum up to the current house. For each house, the new maximum is `max(prev + nums[i], curr)`. We iterate through the range and update `prev` and `curr` accordingly. Edge cases include a single house (return its value directly) and two houses (take the max of the two). Time complexity is O(n) because each subproblem processes each house once, and we run two subproblems. Space complexity is O(1) since we only use a few integer variables.

#include <vector>
#include <algorithm>

// Returns the maximum amount robbable in a circular arrangement of houses.
// Houses are considered adjacent if they are consecutive or if they are the first and last.
int robCircular(const std::vector<int>& nums) {
    int n = nums.size();
    if (n == 1) {
        return nums[0];
    }
    
    // Helper lambda to solve linear house robber for a given range [start, end]
    auto robber = [&nums](int start, int end) {
        int prev = 0;
        int curr = 0;
        for (int i = start; i <= end; ++i) {
            int next = std::max(prev + nums[i], curr);
            prev = curr;
            curr = next;
        }
        return curr;
    };
    
    // Case 1: exclude last house, Case 2: exclude first house
    int excludeLast = robber(0, n - 2);
    int excludeFirst = robber(1, n - 1);
    
    return std::max(excludeLast, excludeFirst);
}

#include <cassert>
#include <vector>
#include <iostream>

int robCircular(const std::vector<int>& nums); // forward declaration

int main() {
    // Single house
    assert(robCircular({5}) == 5);
    
    // Two houses: take the larger
    assert(robCircular({3, 7}) == 7);
    
    // Classic circular case: cannot rob first and last, so choose middle
    assert(robCircular({2, 3, 2}) == 3);
    
    // Larger example: optimal is rob indices 1 and 3 (1+3=4)
    assert(robCircular({1, 2, 3, 1}) == 4);
    
    // All zeros
    assert(robCircular({0, 0, 0}) == 0);
    
    // Non-monotonic values
    assert(robCircular({2, 7, 9, 3, 1}) == 11); // rob indices 1 and 2? Actually 7+1=8, 2+9+1=12? Let's check: circular: can't rob 0&4, but 2+9+=11, 7+3=10, 2+9+1=12? 0,2,4 are not adjacent? 0 and 4 are adjacent, so exclude. Correct max is 11 (rob indices 1 and 3? 7+3=10, rob indices 0 and 2? 2+9=11, rob indices 2 and 4? 9+1=10) -> 11.
    assert(robCircular({2, 7, 9, 3, 1}) == 11);
    
    // Large values
    assert(robCircular({1000, 1000, 1000}) == 1000);
    
    // Four houses with equal pattern
    assert(robCircular({1, 1, 1, 1}) == 2);
    
    // Five houses with increasing values
    assert(robCircular({1, 2, 3, 4, 5}) == 8); // rob 1,3? 2+4=6, 3+5=8, 1+3+5=9? but 1 and 5 adjacent? No, 1 and 5 are adjacent in circle, so can't take both. Check: options: rob 1,3->2+4=6; rob 2,4->3+5=8; rob 0,2,4->1+3+5=9 but 0 and 4 adjacent, so no. So 8 is correct.
    
    // Already known edge case from snippet
    assert(robCircular({2, 3, 2}) == 3);
    
    std::cout << "All tests passed!\n";
    return 0;
}
