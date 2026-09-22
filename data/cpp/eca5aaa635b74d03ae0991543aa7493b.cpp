Write a C++ function named `binarySearch` that takes a sorted array of integers (in non-decreasing order), its left and right indices, and a target value, and returns the index of the target if found, or `-1` if not present. The function must be iterative (not recursive) and must work correctly for arrays of any size (including empty ranges when `left > right`). Additionally, provide a separate function `searchInArray` that accepts a `const` reference to a `std::vector<int>` and a target value, and internally calls `binarySearch` to perform the search—this wrapper ensures the solution is safe and reusable with standard containers. The implementation should use proper `const` correctness and avoid modifying the input.
The core algorithm is binary search. Given a sorted array, we maintain a search interval `[left, right]`. At each step, compute the middle index as `mid = left + (right - left) / 2` to avoid overflow when `left + right` is large. Compare `arr[mid]` with the target `x`: if equal, return `mid`; if `arr[mid] < x`, discard the left half by setting `left = mid + 1`; otherwise discard the right half by setting `right = mid - 1`. The loop continues while `left <= right`. If the loop exits without finding the target, return `-1`. Edge cases include: (1) empty array or invalid range (`left > right`), which correctly returns `-1` because the loop condition fails immediately; (2) target smaller than all elements or larger than all elements, which narrows the range until it becomes empty; (3) duplicate elements—any matching index is acceptable; (4) very large indices that might cause integer overflow if computed naively as `(left+right)/2`—the safe midpoint formula avoids this. Time complexity is O(log n) for an array of size n, and space complexity is O(1) since only a few integer variables are used.
#include <vector>

// Iterative binary search on a sorted array segment.
// Returns the index of the target if found, otherwise -1.
int binarySearch(const int arr[], int left, int right, int x) {
    while (left <= right) {
        int mid = left + (right - left) / 2; // safe midpoint
        if (arr[mid] == x) {
            return mid;
        } else if (arr[mid] < x) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    return -1;
}

// Convenience wrapper for a std::vector<int>.
// Returns the index of the target if found, otherwise -1.
int searchInArray(const std::vector<int>& data, int target) {
    if (data.empty()) {
        return -1;
    }
    return binarySearch(data.data(), 0, static_cast<int>(data.size()) - 1, target);
}
#include <cassert>
#include <vector>

int main() {
    std::vector<int> arr1 = {1, 3, 5, 7, 9};
    assert(searchInArray(arr1, 5) == 2);
    assert(searchInArray(arr1, 1) == 0);
    assert(searchInArray(arr1, 9) == 4);
    assert(searchInArray(arr1, 4) == -1);
    assert(searchInArray(arr1, 0) == -1);
    assert(searchInArray(arr1, 10) == -1);

    std::vector<int> arr2 = {2, 2, 2, 2};
    int res = searchInArray(arr2, 2);
    assert(res >= 0 && res < 4); // any index is valid

    std::vector<int> arr3 = {42};
    assert(searchInArray(arr3, 42) == 0);
    assert(searchInArray(arr3, 41) == -1);

    std::vector<int> arr4 = {};
    assert(searchInArray(arr4, 5) == -1);

    std::vector<int> arr5 = {-10, -5, 0, 3, 8};
    assert(searchInArray(arr5, -5) == 1);
    assert(searchInArray(arr5, 0) == 2);
    assert(searchInArray(arr5, 8) == 4);
    assert(searchInArray(arr5, -11) == -1);
}
