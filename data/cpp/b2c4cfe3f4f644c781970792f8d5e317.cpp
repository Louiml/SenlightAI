// Write a C++ function `int countInversions(std::vector<int>& data)` that sorts a vector of integers in ascending order using the provided merge sort algorithm and simultaneously counts the number of inversions in the original array. An inversion is a pair of indices `(i, j)` such that `i < j` and `data[i] > data[j]`. The function must modify the vector in-place (so after the call, `data` is sorted) and return the total inversion count. The vector may contain duplicate values, negative numbers, and be empty. For an empty or single-element vector, return 0. You must implement the merge sort logic yourself (do not use `std::sort` or any library sorting function) and adapt the counting into the merge step.

// The core approach mirrors the provided merge sort. In the `merge` step, when we take an element from the right subarray (`theArray[first2]`) before taking an element from the left subarray (`theArray[first1]`), that means the right element is smaller than the left element, and since all remaining elements in the left subarray (from `first1` to `last1`) are larger than or equal to that right element, each such pair is an inversion. Thus, when copying from the right, we add `(last1 - first1 + 1)` to the inversion count. The function recursively sorts the left and right halves, adds the counts from recursion, then adds the count computed during the merge. Edge cases: empty or single-element vector → return 0. Duplicates: since we use `<=` when comparing (copy left on equality), no inversions are counted for equal values, which is correct. Time complexity is \(O(n \log n)\) for both sorting and counting; space complexity is \(O(n)\) due to the temporary array in each merge.

#include <vector>

// Merge two sorted subarrays data[first..mid] and data[mid+1..last] into data[first..last],
// and count inversions where an element from the right subarray is placed before an element from the left.
int mergeAndCount(std::vector<int>& data, int first, int mid, int last) {
    std::vector<int> temp(last - first + 1);
    int first1 = first;
    int last1 = mid;
    int first2 = mid + 1;
    int last2 = last;
    int index = 0;
    int inversionCount = 0;

    while (first1 <= last1 && first2 <= last2) {
        if (data[first1] <= data[first2]) {
            temp[index++] = data[first1++];
        } else {
            // All remaining elements in left subarray (first1..last1) are greater than data[first2],
            // so each forms an inversion with data[first2].
            inversionCount += (last1 - first1 + 1);
            temp[index++] = data[first2++];
        }
    }
    while (first1 <= last1) temp[index++] = data[first1++];
    while (first2 <= last2) temp[index++] = data[first2++];

    for (int i = 0; i < temp.size(); ++i) {
        data[first + i] = temp[i];
    }
    return inversionCount;
}

// Sorts the vector in ascending order and returns the number of inversions in the original ordering.
int countInversions(std::vector<int>& data) {
    if (data.size() <= 1) return 0; // Trivial case

    // Recursive helper that sorts the segment data[first..last] and returns its inversion count.
    auto mergeSortCount = [&](auto&& self, int first, int last) -> int {
        if (first >= last) return 0;
        int mid = first + (last - first) / 2;
        int leftCount = self(self, first, mid);
        int rightCount = self(self, mid + 1, last);
        int mergeCount = mergeAndCount(data, first, mid, last);
        return leftCount + rightCount + mergeCount;
    };

    return mergeSortCount(mergeSortCount, 0, data.size() - 1);
}

#include <cassert>
#include <vector>

// Forward declaration of the solution function (if needed, but the solution is included above).
int countInversions(std::vector<int>& data);

int main() {
    // Empty and single element
    std::vector<int> empty;
    assert(countInversions(empty) == 0 && empty.empty());

    std::vector<int> single = {5};
    assert(countInversions(single) == 0 && single == std::vector<int>{5});

    // Already sorted — zero inversions
    std::vector<int> sorted = {1, 2, 3, 4};
    assert(countInversions(sorted) == 0 && sorted == std::vector<int>({1, 2, 3, 4}));

    // Reverse sorted — maximum inversions for 4 elements: 6
    std::vector<int> reverse = {4, 3, 2, 1};
    assert(countInversions(reverse) == 6 && reverse == std::vector<int>({1, 2, 3, 4}));

    // Mixed with duplicates and negatives
    std::vector<int> mixed = {3, 1, 2, 1, -1};
    // Original: (3,1),(3,2),(3,1),(3,-1),(1,-1),(2,-1),(1,-1) = 7 inversions
    std::vector<int> mixedSorted = {-1, 1, 1, 2, 3};
    assert(countInversions(mixed) == 7 && mixed == mixedSorted);

    // Larger test: [5,4,3,2,1] → 10 inversions
    std::vector<int> large = {5, 4, 3, 2, 1};
    assert(countInversions(large) == 10 && large == std::vector<int>({1, 2, 3, 4, 5}));

    // Duplicates where equal values are not inversions
    std::vector<int> dups = {2, 2, 2};
    assert(countInversions(dups) == 0 && dups == std::vector<int>({2, 2, 2}));

    return 0;
}
