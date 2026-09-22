// Implement a C++ function template `mergeSortInPlace(std::vector<T>& data)` that sorts the given vector in ascending order using the merge sort algorithm. The function must work for any type `T` that supports `operator<` (or `operator<=` for stability) and `operator=`. It should modify the vector in place and not return anything. You may define internal helper functions (e.g., a recursive `mergeSortHelper` and a `merge` function) inside anonymous namespace or as static helper functions. The implementation must be correct for empty vectors, vectors with a single element, vectors with duplicate values, and vectors already in descending order. Do not use `std::sort` or other STL sorting functions—only the merge sort logic.
// The core algorithm is a classic divide-and-conquer merge sort. The function recursively splits the vector into two halves using a middle index `mid = left + (right - left) / 2` (to avoid overflow for large `int` values). The base case is when the subarray has zero or one element (i.e., `left >= right`), in which case it is already sorted. After sorting both halves recursively, the `merge` step combines them into a temporary vector `L` and `R` (copies of left and right halves), then writes back to the original array in sorted order. The merge uses two indices to compare elements, copying the smaller one; if equal, the left element is taken first to keep stability. After one side is exhausted, remaining elements from the other side are appended. Edge cases include empty input (`size() == 0`), where the function should simply return, and when `n1` or `n2` is zero (which never happens because `mid` is inside the range). Time complexity is always `O(n log n)` for all cases, and space complexity is `O(n)` due to the temporary vectors at each merge level (though re-used per recursion, the peak is `O(n)`).
#include <vector>

namespace {

// Merge two sorted subarrays [left..mid] and [mid+1..right] into the original array.
template <typename T>
void mergeSubarrays(std::vector<T>& data, int left, int mid, int right) {
    int leftSize = mid - left + 1;
    int rightSize = right - mid;

    std::vector<T> leftPart(leftSize);
    std::vector<T> rightPart(rightSize);

    for (int i = 0; i < leftSize; ++i) {
        leftPart[i] = data[left + i];
    }
    for (int j = 0; j < rightSize; ++j) {
        rightPart[j] = data[mid + 1 + j];
    }

    int i = 0, j = 0, index = left;
    while (i < leftSize && j < rightSize) {
        if (leftPart[i] <= rightPart[j]) {
            data[index] = leftPart[i];
            ++i;
        } else {
            data[index] = rightPart[j];
            ++j;
        }
        ++index;
    }

    while (i < leftSize) {
        data[index] = leftPart[i];
        ++i;
        ++index;
    }

    while (j < rightSize) {
        data[index] = rightPart[j];
        ++j;
        ++index;
    }
}

// Recursive merge sort helper.
template <typename T>
void mergeSortHelper(std::vector<T>& data, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSortHelper(data, left, mid);
        mergeSortHelper(data, mid + 1, right);
        mergeSubarrays(data, left, mid, right);
    }
}

} // namespace

// Sorts the vector in ascending order using merge sort.
template <typename T>
void mergeSortInPlace(std::vector<T>& data) {
    if (data.empty() || data.size() < 2) {
        return;
    }
    mergeSortHelper(data, 0, static_cast<int>(data.size()) - 1);
}
#include <cassert>
#include <vector>
#include <string>

// Function template declaration (must match the solution's signature)
template <typename T>
void mergeSortInPlace(std::vector<T>& data);

int main() {
    // Test with integers
    std::vector<int> empty;
    mergeSortInPlace(empty);
    assert(empty.empty());

    std::vector<int> single = {42};
    mergeSortInPlace(single);
    assert(single.size() == 1 && single[0] == 42);

    std::vector<int> descending = {5, 4, 3, 2, 1};
    mergeSortInPlace(descending);
    assert((descending == std::vector<int>{1, 2, 3, 4, 5}));

    std::vector<int> duplicates = {3, 1, 3, 2, 1, 3};
    mergeSortInPlace(duplicates);
    assert((duplicates == std::vector<int>{1, 1, 2, 3, 3, 3}));

    std::vector<int> random = {9, -2, 7, -5, 0, 12, 3};
    mergeSortInPlace(random);
    assert((random == std::vector<int>{-5, -2, 0, 3, 7, 9, 12}));

    // Test with floating point numbers
    std::vector<double> decimals = {2.5, -1.0, 3.14, 2.5, 0.0};
    mergeSortInPlace(decimals);
    assert((decimals == std::vector<double>{-1.0, 0.0, 2.5, 2.5, 3.14}));

    // Test with strings (lexicographic order)
    std::vector<std::string> words = {"pear", "apple", "banana", "cherry"};
    mergeSortInPlace(words);
    assert((words == std::vector<std::string>{"apple", "banana", "cherry", "pear"}));

    // Test with large vector to ensure no crash (but we only assert the size and it's sorted)
    std::vector<int> large(1000);
    for (int i = 0; i < 1000; ++i) {
        large[i] = (i * 37) % 101; // produces duplicates and unsorted pattern
    }
    mergeSortInPlace(large);
    for (size_t i = 1; i < large.size(); ++i) {
        assert(large[i - 1] <= large[i]);
    }

    return 0;
}
