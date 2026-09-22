/*
Write a C++ function `smallestRange` that takes a vector of sorted integer vectors (each representing a sorted list) and returns a vector of two integers `{start, end}` representing the smallest range (i.e., the range with the smallest length, and if ties, the one with the smallest start value) that includes at least one element from each of the input lists. All lists are non-empty and sorted in ascending order. The function should handle up to 10^5 total elements across all lists and return the result as a `std::vector<int>`. For example, given `nums = {{4,10,15,24,26},{0,9,12,20},{5,18,22,30}}`, the result is `{20,24}` because that range of length 4 includes 24 from the first list, 20 from the second, and 22 from the third, which is the smallest possible.
*/

#include <vector>
#include <queue>
#include <limits>

// Find the smallest range that includes at least one element from each sorted list.
std::vector<int> smallestRange(std::vector<std::vector<int>>& nums) {
    // Min-heap storing {value, list index, element index}
    using HeapNode = std::vector<int>;
    std::priority_queue<HeapNode, std::vector<HeapNode>, std::greater<HeapNode>> minHeap;
    int curMax = std::numeric_limits<int>::min();
    
    // Initialize heap with first element of each list
    for (size_t i = 0; i < nums.size(); ++i) {
        minHeap.push({nums[i][0], static_cast<int>(i), 0});
        curMax = std::max(curMax, nums[i][0]);
    }
    
    // Best range found so far
    std::vector<int> bestRange = {0, std::numeric_limits<int>::max()};
    
    while (!minHeap.empty()) {
        // Pop the minimum element
        HeapNode curr = minHeap.top();
        minHeap.pop();
        int curMin = curr[0];
        int listIdx = curr[1];
        int elemIdx = curr[2];
        
        // Update best range if current range is better
        if ((curMax - curMin < bestRange[1] - bestRange[0]) ||
            (curMax - curMin == bestRange[1] - bestRange[0] && curMin < bestRange[0])) {
            bestRange[0] = curMin;
            bestRange[1] = curMax;
        }
        
        // Move to next element in the same list
        if (elemIdx + 1 < static_cast<int>(nums[listIdx].size())) {
            int nextVal = nums[listIdx][elemIdx + 1];
            minHeap.push({nextVal, listIdx, elemIdx + 1});
            curMax = std::max(curMax, nextVal);
        } else {
            // If any list is exhausted, no further range is possible
            break;
        }
    }
    
    return bestRange;
}

#include <cassert>
#include <vector>

int main() {
    // Basic example from problem statement
    std::vector<std::vector<int>> nums1 = {{4,10,15,24,26},{0,9,12,20},{5,18,22,30}};
    assert(smallestRange(nums1) == std::vector<int>({20,24}));
    
    // Single list
    std::vector<std::vector<int>> nums2 = {{1,2,3}};
    assert(smallestRange(nums2) == std::vector<int>({1,3}));
    
    // Two lists with overlapping values
    std::vector<std::vector<int>> nums3 = {{1,2,3},{1,2,3}};
    assert(smallestRange(nums3) == std::vector<int>({1,1}));
    
    // Lists with negative numbers
    std::vector<std::vector<int>> nums4 = {{-5,-1},{0,2},{-3,4}};
    // Possible ranges: [-5,0] (len5), [-3,0] (len3), [-1,2] (len3), etc. Best is [-1,2] or [-3,0] len3; start -3 is smaller, but check valid: [-3,0] includes -3 from third, -1 from first, 0 from second => valid. So expect {-3,0}
    assert(smallestRange(nums4) == std::vector<int>({-3,0}));
    
    // Duplicate values within a list
    std::vector<std::vector<int>> nums5 = {{1,1,2},{1,3},{2,4}};
    // Range [1,2] includes 1 from first, 1 from second, 2 from third => len1? Actually diff=1, valid. Check [1,1]? Need from third, third has 2 only, so no. So best is [1,2]
    assert(smallestRange(nums5) == std::vector<int>({1,2}));
    
    // Large gap lists
    std::vector<std::vector<int>> nums6 = {{0,100},{1,2},{50,60}};
    // Need include 0 or 100 from first, 1 or2 from second, 50 or60 from third. Best range: [0,50]? includes 0,1,50 len50; [1,50] len49; [2,50] len48; [2,60] len58; [1,60] etc. Best is [2,50]? includes 2 from second, 50 from third, but first has 0 or100, 0 not in [2,50] so no. Actually need first element in range: if range [0,50] includes 0 from first, 1 from second, 50 from third -> len50. Range [0,2]? no third. Range [50,100] includes 100 from first, 2 from second? no. So only [0,50] works? also [0,60] len60, [1,50] includes 1 from second, 50 third, but first? 0 not included, 100 not, so invalid. Wait first only has 0 and 100. So range must include either 0 or 100. If include 100, range [100,100]? no second. So must include 0. Then include some second and third. To include second min is1, third min is50. So range [0,50] len50 is best. Assert.
    assert(smallestRange(nums6) == std::vector<int>({0,50}));
    
    // All lists have same single element
    std::vector<std::vector<int>> nums7 = {{7},{7},{7}};
    assert(smallestRange(nums7) == std::vector<int>({7,7}));
    
    // Test tie-breaking: two different ranges of same length, choose smaller start
    std::vector<std::vector<int>> nums8 = {{1,10},{2,3},{4,5}};
    // Possible: [1,2]? needs third: 4 or5, no. [2,4] includes 2 (second), 4 (third), but first? 1 not,10 not. Invalid. [3,5] includes 3,4,10? no. Let's enumerate: first must be 1 or10. If include 1, then need second (min2) and third(min4): range [1,4] len3. If include 10, then need second(max3) and third(max5): range [3,10] len7. So best [1,4] len3. Also check [1,5] len4. So answer {1,4}
    assert(smallestRange(nums8) == std::vector<int>({1,4}));
    
    return 0;
}

// The algorithm uses a min-heap (priority queue) to efficiently maintain the current smallest element among the lists while tracking the current maximum element among the lists' current pointers. We initialize the heap with the first element of each list and set `curMax` to the maximum of these first elements. The initial candidate range is `[curMin, curMax]` where `curMin` is the heap's minimum (which is the first element of the first list in the heap after initialization). In each iteration, we pop the smallest element from the heap (which gives the current minimum of the selected elements). We update the best range if the current range `[curMin, curMax]` is smaller than the best so far (or equal length with smaller start). Then we advance the pointer in the list that contributed the popped element, pushing its next element (if any) into the heap and updating `curMax` to the maximum of itself and this new value. If any list is exhausted (i.e., we pop its last element and there is no next), we break because we cannot include that list anymore, so the current range cannot be extended further. This process guarantees we consider all possible ranges that include at least one element from each list. Edge cases include: when there is only one list (the range should be the single element repeated), when lists have duplicate values, and when multiple ranges have the same length (we choose the one with the smallest start). Time complexity is O(N log k) where N is total number of elements across all lists and k is the number of lists, due to heap operations. Space complexity is O(k) for the heap.
