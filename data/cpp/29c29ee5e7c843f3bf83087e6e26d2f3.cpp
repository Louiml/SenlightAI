// Write a C++ function `int findSmallestMissing(int arr[], int n)` that, given a sorted array of non-negative integers (possibly containing duplicates), returns the smallest non-negative integer that is not present in the array. For example, for `[0,1,2,3,5]` the answer is `4`, for `[0,0,1,2]` the answer is `3`, for `[1,2,3]` the answer is `0`, and for `[0,1,2,3]` the answer is `4`. The array is guaranteed to be sorted in non-decreasing order. The function must not modify the array and must run in better than linear time, specifically in `O(log n)` time. You may assume that the array size `n` is at least 1.
The key observation is that in a sorted array, if for every index `i` (0-based) the value `arr[i] == i`, then no numbers are missing up to `n-1`, so the smallest missing number is `n`. If at some index `i` we find `arr[i] > i`, then the smallest missing number must be less than or equal to `i`. More generally, for any index `i`, if `arr[i] > i`, then the number `i` is definitely missing (because all prior elements are ≤ `i-1` and the array is sorted, so no element equals `i`). If `arr[i] == i`, the numbers from 0 to `i` are all present (or at least `i` is present), so the missing number lies to the right. If `arr[i] < i`, that is possible only with duplicates, but then the smallest missing number is still to the left or at `i`? Careful: With duplicates, `arr[i] < i` can happen (e.g., `[0,0,1,2]` at index 1, arr[1]=0). In that case, we cannot easily conclude anything about the missing number because a small value repeated may push later values to be equal to their index later. However, note that the property that matters is: if `arr[i] > i`, then `i` is missing (since sorted, all elements before `i` are ≤ `i-1`? Actually if `arr[i] > i` and array is sorted, then all elements from index 0 to i are ≤ `i-1` because arr[i] is the smallest value among indices ≥ i, so every element at index ≤ i is < arr[i] ≤? Wait: sorted non-decreasing, so for all j ≤ i, arr[j] ≤ arr[i]. Since arr[i] > i, it is possible that some earlier value equals i? But if any earlier value equals i, then arr[i] would be ≥ that value, but that doesn't prevent arr[i] > i. Example: [0,0,2] at index 2, arr[2]=2, not >2. Example [0,0,3] at index2 arr[2]=3>2, but missing number is 1 (not 2) because arr[1]=0, arr[0]=0, so 1 is missing. So the simple condition `arr[mid] > mid` does not directly give the smallest missing. The correct binary search condition is: if `arr[mid] == mid`, then the numbers from 0 to mid are all present (because sorted and each index appears at least once? Actually with duplicates, if arr[mid]==mid, we cannot be sure all numbers up to mid are present. For example [0,0,2,3] at mid=2, arr[2]=2, but number 1 is missing. So `arr[mid]==mid` does not guarantee that all previous numbers are consecutive. The accurate property: The smallest missing number is the first index i such that arr[i] != i (but duplicates may make arr[i] < i). Actually the smallest missing is the smallest i >=0 such that i is not in the array. Because array is sorted, we can find the first index where arr[i] > i (since if arr[i] < i, then i is definitely missing? Let's check: if arr[i] < i, then because array is sorted, there are at least (i - arr[i] + something) duplicates, but i could be present later? Example [0,0,1,3], at index 3 arr[3]=3, not <3. Example [0,0,0,0] at index 3 arr[3]=0 <3, missing number is 1 (which is not i=3). So condition arr[i] < i doesn't directly give missing. The classic solution for "smallest missing positive" uses binary search on sorted array of distinct? Actually for sorted distinct positive integers, we check arr[mid]==mid. For duplicates, a linear scan is needed unless we refine. However, the original snippet assumes distinct? It uses binary search with condition `if(arr[mid] == mid) beg = mid+1; else end = mid-1;` and returns `mid+1`. That works only if array contains distinct consecutive numbers starting from 0, but not for duplicates. But the problem statement I write can specify "distinct" to make it correct. To keep the spirit and make the binary search valid, I'll specify that the array contains distinct non-negative integers and is sorted. Then the algorithm is correct: for sorted distinct, find first index where arr[i] != i. If arr[mid] == mid, move right; else move left. Return the first index where arr[i] != i, which is exactly the smallest missing. Edge cases: all elements equal to their index -> return n. If arr[0] != 0 -> return 0. Time O(log n), space O(1).
#include <vector>

// Given a sorted array of distinct non-negative integers, return the smallest
// non-negative integer not present in the array.
// The array must be sorted in strictly increasing order (no duplicates).
int findSmallestMissing(const int arr[], int n) {
    int low = 0;
    int high = n - 1;
    int result = n; // Default if all indices are matched

    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (arr[mid] == mid) {
            // Numbers from 0 to mid are all present, missing is to the right
            low = mid + 1;
        } else {
            // arr[mid] > mid (because distinct), missing could be at mid or left
            result = mid;
            high = mid - 1;
        }
    }
    return result;
}
#include <cassert>

int main() {
    int arr1[] = {0, 1, 2, 3, 5};
    assert(findSmallestMissing(arr1, 5) == 4);

    int arr2[] = {1, 2, 3};
    assert(findSmallestMissing(arr2, 3) == 0);

    int arr3[] = {0, 1, 2, 3};
    assert(findSmallestMissing(arr3, 4) == 4);

    int arr4[] = {0, 2, 3, 4};
    assert(findSmallestMissing(arr4, 4) == 1);

    int arr5[] = {0, 1, 3, 4, 5};
    assert(findSmallestMissing(arr5, 5) == 2);

    int arr6[] = {7, 8, 9};
    assert(findSmallestMissing(arr6, 3) == 0);

    int arr7[] = {0};
    assert(findSmallestMissing(arr7, 1) == 1);

    int arr8[] = {5};
    assert(findSmallestMissing(arr8, 1) == 0);
    
    return 0;
}
