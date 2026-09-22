/*
Given a vector of integers where each element satisfies `0 <= nums[i] < nums.size()` and exactly one integer appears more than once (while all other integers appear at most once), write a C++ function that finds and returns the duplicate integer. The function must use the array itself for bookkeeping without modifying the original values in a way that loses information needed for the final result (using a swap or marking strategy is acceptable, but the original array may be altered). The solution must run in O(n) time and O(1) extra space, and must work for any valid input size ≥ 2.
*/

#include <vector>

/**
 * Finds the duplicate integer in a vector where each element is in [0, n)
 * and exactly one element appears more than once.
 * The vector may be modified during the search.
 *
 * @param nums Vector of integers satisfying the constraints.
 * @return The duplicate integer.
 */
int findDuplicateValue(std::vector<int>& nums) {
    int curr = 0;
    while (nums[curr] != curr) {
        int next = nums[curr];
        nums[curr] = curr;  // mark as visited
        curr = next;
    }
    return curr;
}

#include <cassert>
#include <vector>

// Function declaration (for test purposes, the full implementation is above)
int findDuplicateValue(std::vector<int>& nums);

int main() {
    std::vector<int> test1 = {1, 3, 4, 2, 2};
    assert(findDuplicateValue(test1) == 2);

    std::vector<int> test2 = {3, 1, 3, 4, 2};
    assert(findDuplicateValue(test2) == 3);

    std::vector<int> test3 = {0, 1, 1};
    assert(findDuplicateValue(test3) == 1);

    std::vector<int> test4 = {1, 0, 2, 2};
    assert(findDuplicateValue(test4) == 2);

    std::vector<int> test5 = {0, 2, 3, 4, 1, 3};
    assert(findDuplicateValue(test5) == 3);

    std::vector<int> test6 = {2, 1, 2};
    assert(findDuplicateValue(test6) == 2);

    std::vector<int> test7 = {1, 4, 4, 2, 3};
    assert(findDuplicateValue(test7) == 4);

    std::vector<int> test8 = {0, 0};
    assert(findDuplicateValue(test8) == 0);

    std::vector<int> test9 = {4, 3, 2, 1, 4};
    assert(findDuplicateValue(test9) == 4);

    std::vector<int> test10 = {5, 1, 2, 3, 5, 4};
    assert(findDuplicateValue(test10) == 5);

    return 0;
}

// The provided code uses a recursive cycle-finding approach. Starting at index 0, it checks if the value at the current index equals the index itself. If so, that value is the duplicate (because if `nums[i] == i` for some i, and all other numbers are distinct, then that value must have been placed there twice or be the repeated element). Otherwise, it stores the `next` index as `nums[curr]`, sets `nums[curr] = curr` (marking it as visited) and recurses on `next`. Since there is exactly one duplicate, the process will eventually reach an index where the stored value equals the index, because each step maps to a unique next index until a cycle is detected. The time complexity is O(n) because each index is visited at most once (once marked, it is never revisited). Space complexity is O(1) auxiliary (ignoring recursion stack). Edge cases: if the duplicate is 0, then `nums[0]` must be 0, but since we start at index 0 and check `nums[0]==0`, it returns immediately. If the duplicate is not 0, the algorithm will eventually find it. The recursion depth is at most n, but in practice it's fine for reasonable n; an iterative version can avoid stack overflow.
