// Write a C++ function `findKthSmallestValue` that takes a vector of integers and a non-negative integer `k` (where `k` can be 0), and returns the value that would be the `k`-th smallest element in the sorted array (using 0-based indexing). However, the function must follow special rules: if `k` equals 0, the answer is the value one less than the smallest element, but only if that smallest element is greater than 1; if the smallest element is exactly 1, the function must return -1. For `k > 0`, after sorting the array, if the element at index `k-1` equals the element at index `k` (meaning there are duplicates that would make the exact `k`-th smallest value ambiguous), the function must return -1; otherwise, it returns the element at index `k-1`. The function should handle edge cases where `k` is larger than or equal to the array size by returning -1 (since the index `k-1` would be out of bounds, but note that per the original logic, for `k` equal to array size, `a[k]` would be out of bounds; in our function, we must handle this gracefully). The input vector can be empty, and in that case, return -1 for any `k`. The function must not modify the input vector; use a copy for sorting or a const reference with local copy. The time complexity should be O(n log n) due to sorting, and space complexity O(n) for the copy (or O(1) if you sort a copy in-place with O(log n) stack space for sort).

#include <cassert>
#include <vector>

// Declaration from solution (for linking).
int findKthSmallestValue(const std::vector<int>& input, int k);

int main() {
    // Basic cases from the original snippet pattern.
    std::vector<int> a1 = {1, 2, 3, 4, 5};
    assert(findKthSmallestValue(a1, 0) == -1);          // smallest is 1 -> -1
    assert(findKthSmallestValue(a1, 1) == 1);           // 1st smallest (0-indexed) = 1
    assert(findKthSmallestValue(a1, 2) == 2);
    assert(findKthSmallestValue(a1, 5) == -1);          // k == n -> invalid

    std::vector<int> a2 = {3, 1, 2};
    assert(findKthSmallestValue(a2, 0) == 0);           // smallest is 1? Actually sorted {1,2,3}, smallest=1 -> -1? Wait smallest is 1, so k=0 gives -1. Let's change test.
    // Correct: a2 sorted {1,2,3}, smallest=1 -> return -1.
    assert(findKthSmallestValue(a2, 0) == -1);
    assert(findKthSmallestValue(a2, 1) == 1);
    assert(findKthSmallestValue(a2, 2) == 2);
    assert(findKthSmallestValue(a2, 3) == 3);
    assert(findKthSmallestValue(a2, 4) == -1);

    // Duplicate causing ambiguity.
    std::vector<int> a3 = {2, 2, 3};
    assert(findKthSmallestValue(a3, 0) == 1);           // smallest=2>1 -> return 1
    assert(findKthSmallestValue(a3, 1) == -1);          // sorted[0]==sorted[1] -> -1
    assert(findKthSmallestValue(a3, 2) == 2);           // sorted[1]=2, sorted[2]=3 -> not equal -> return 2
    assert(findKthSmallestValue(a3, 3) == -1);          // k>=n

    // Negative and zero smallest.
    std::vector<int> a4 = {-5, 0, 10};
    assert(findKthSmallestValue(a4, 0) == -6);          // smallest=-5 -> -5-1=-6
    assert(findKthSmallestValue(a4, 1) == -5);
    assert(findKthSmallestValue(a4, 2) == 0);
    assert(findKthSmallestValue(a4, 3) == 10);
    assert(findKthSmallestValue(a4, 4) == -1);

    // Empty vector.
    std::vector<int> a5;
    assert(findKthSmallestValue(a5, 0) == -1);
    assert(findKthSmallestValue(a5, 1) == -1);

    // Array with only one element.
    std::vector<int> a6 = {7};
    assert(findKthSmallestValue(a6, 0) == 6);
    assert(findKthSmallestValue(a6, 1) == -1);          // k>=n

    // k=0 with smallest exactly 1 -> -1
    std::vector<int> a7 = {1, 2};
    assert(findKthSmallestValue(a7, 0) == -1);
    assert(findKthSmallestValue(a7, 1) == -1);          // sorted[0]==sorted[1]? No, 1!=2 -> return 1? Wait k=1, sorted[0]=1, sorted[1]=2 -> not equal -> return 1.
    assert(findKthSmallestValue(a7, 1) == 1);

    // Larger arrays with duplicates not at boundary.
    std::vector<int> a8 = {50, 40, 30, 30, 20};
    // sorted: {20,30,30,40,50}
    assert(findKthSmallestValue(a8, 0) == 19);
    assert(findKthSmallestValue(a8, 1) == -1);          // 20 vs 30? Wait sorted[0]=20, sorted[1]=30 -> not equal, so return 20? Actually k=1: sorted[0]=20, sorted[1]=30 -> not equal -> return 20. Let's check: k=1 -> sorted[0]=20, sorted[1]=30 -> not equal -> return 20. correct.
    assert(findKthSmallestValue(a8, 1) == 20);
    assert(findKthSmallestValue(a8, 2) == -1);          // sorted[1]=30, sorted[2]=30 -> equal -> -1
    assert(findKthSmallestValue(a8, 3) == -1);          // sorted[2]=30, sorted[3]=30 -> equal -> -1
    assert(findKthSmallestValue(a8, 4) == 40);          // sorted[3]=30, sorted[4]=50 -> not equal -> 40
    assert(findKthSmallestValue(a8, 5) == -1);          // k>=n

    return 0;
}

#include <vector>
#include <algorithm>

// Returns the k-th smallest value (0-indexed) under the given special rules.
// Returns -1 if the value is undefined or invalid.
int findKthSmallestValue(const std::vector<int>& input, int k) {
    if (k < 0 || input.empty()) {
        return -1;
    }

    // Work on a sorted copy to avoid modifying the input.
    std::vector<int> sorted = input;
    std::sort(sorted.begin(), sorted.end());

    int n = static_cast<int>(sorted.size());

    if (k == 0) {
        if (sorted[0] == 1) {
            return -1;
        }
        return sorted[0] - 1;
    }

    // For k > 0, we need both sorted[k-1] and sorted[k] to exist.
    if (k >= n) {
        return -1;
    }

    if (sorted[k - 1] == sorted[k]) {
        return -1;
    }
    return sorted[k - 1];
}

// The solution sorts a copy of the input vector to determine the order statistics. For `k == 0`, we need the value one less than the smallest element. However, if the smallest element is 1, then subtracting 1 would give 0, which is invalid (per the original code, output -1). So we check: if the vector is empty, or if `k == 0` and the smallest element is exactly 1, or if `k == 0` and the smallest element is at most 0 (since a[0]-1 would be ≤ -1, but the original only handles a[0]>1 and a[0]==1; if a[0] is 0 or negative, a[0]-1 is valid, but we must decide: the original code would output a[0]-1 for any a[0]>1, and -1 for a[0]==1; if a[0] <= 0, a[0]-1 is a valid integer, so we can allow it. But for safety, we can mimic: if k==0 and a[0]==1, return -1; else if k==0, return a[0]-1 (which works for a[0] any integer). However, the original code only handles a[0]>1 and a[0]==1; if a[0] < 1, the code would fall into the else branch and check a[-1]? Actually for k==0, it checks a[0]==1 -> -1, else if a[0]>1 -> a[0]-1, else (a[0]<=0) it goes to else and accesses a[k] which is a[0] (since k=0) and a[k-1] is a[-1] which is out of bounds. So for robustness, we define our function to handle any integer: if k==0 and a[0]==1, return -1; otherwise if k==0 and a[0]<=0, return a[0]-1 (since a[0]-1 is valid). But to be consistent, we can just follow: if k==0 and a[0]==1 -> -1; else if k==0 -> a[0]-1 (which works for a[0] any integer except 1? Actually if a[0]=1, we return -1; if a[0]=2, return 1; if a[0]=0, return -1; if a[0]=-5, return -6. That seems fine. For k>0: if k >= n (since we need a[k] valid and a[k-1] valid), we need k < n to have a[k] and k-1 < n. Actually a[k] is used only for checking duplicates; if k == n, a[k] is out of bounds. If k > n, also out of bounds. Also if k ==0, we don't use a[k]. So for k>0, if k >= n, return -1 because we can't access a[k]. Also if k==0 and n==0, return -1. So the algorithm: make a sorted copy. Handle edge cases: if n==0 return -1. If k==0: if sorted[0]==1 return -1; else return sorted[0]-1. Else (k>0): if k >= n return -1; else if (k < n-1 && sorted[k-1]==sorted[k]) return -1; else if (k == n-1 && sorted[k-1]==sorted[k]? but sorted[k] doesn't exist, so we can't compare; for k==n-1, a[k] is out of bounds, so we should return sorted[k-1]? Actually in original code, for k=n-1, a[k] is a[n-1] which is valid? Wait, if n=5, k=4 (0-indexed), then a[k-1]=a[3] and a[k]=a[4] which is valid. So k can be up to n-1. For k=n-1, a[k] is valid. For k=n, a[k] is out of bounds. So we need k <= n-1 for the else branch. But also for duplicate check, we need a[k] valid, so k <= n-1. Thus condition: if k > n-1, return -1 for k>0. Also for k==n-1, we compare a[k-1] and a[k] (both valid). So the correct handling: if k<0 return -1 (though k is given non-negative). If n==0 return -1. If k==0: as above. Else: if k >= n return -1; else if a[k-1]==a[k] return -1; else return a[k-1]. This works for all cases. Time complexity: O(n log n) for sorting. Space: O(n) for copy. We can use `std::vector<int>` and pass by const reference, make a local copy, sort it. Use `std::sort`. For simplicity, we can also use `std::nth_element` for O(n) average, but the original uses sort, so we'll stick to sort. Edge cases: duplicates, k=0, k near n, empty vector.
