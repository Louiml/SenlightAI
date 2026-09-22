// Create a C++ function that takes an array of integers, its length, and a size for a temporary buffer, and performs a stable in-place merge sort using the provided buffer to merge runs. The function must be stable (preserve the relative order of equal elements), must sort in ascending order, and must not use any additional allocation beyond the given buffer. If the buffer is too small to hold even one half of the array, the function should fall back to a simple insertion sort (still stable). The function should return a boolean indicating whether the buffer was large enough to use the merge-based approach or whether it had to fall back.
#include <cassert>
#include <vector>
#include <algorithm>

int main() {
    // Test 1: Small array with no duplicates.
    {
        int arr[] = {5, 3, 8, 1, 9, 2};
        int buffer[10];
        bool usedBuffer = stableMergeSortWithBuffer(arr, 6, buffer, 10);
        assert(usedBuffer == true);
        assert(std::is_sorted(arr, arr + 6));
        int expected[] = {1, 2, 3, 5, 8, 9};
        assert(std::equal(arr, arr + 6, expected));
    }

    // Test 2: Array with duplicates, check stability.
    {
        struct Item { int key; int originalIndex; };
        // We'll simulate with pairs in a vector, but the function takes int*, so we'll use a custom check.
        // Since the function is for ints, stability check is trivial for equal ints (they are indistinguishable).
        // For true stability, we test an array of integers; duplicates are identical, so no visible difference.
        int arr[] = {2, 1, 2, 1, 2};
        int buffer[10];
        stableMergeSortWithBuffer(arr, 5, buffer, 10);
        assert(std::is_sorted(arr, arr + 5));
        // Expect all 1s then all 2s.
        assert(arr[0] == 1 && arr[1] == 1 && arr[2] == 2 && arr[3] == 2 && arr[4] == 2);
    }

    // Test 3: Buffer too small, falls back to insertion sort.
    {
        int arr[] = {3, 1, 2};
        int buffer[1]; // Too small for any merge (needs at least 2 for n=3).
        bool usedBuffer = stableMergeSortWithBuffer(arr, 3, buffer, 1);
        assert(usedBuffer == false);
        assert(std::is_sorted(arr, arr + 3));
    }

    // Test 4: Large array with random values, compare with std::sort.
    {
        std::vector<int> vec(1000);
        for (int i = 0; i < 1000; ++i) vec[i] = rand() % 100;
        std::vector<int> original = vec;
        std::vector<int> buffer(5000);
        stableMergeSortWithBuffer(vec.data(), vec.size(), buffer.data(), buffer.size());
        std::sort(original.begin(), original.end());
        assert(vec == original);
    }

    // Test 5: Already sorted array.
    {
        int arr[] = {1, 2, 3, 4};
        int buffer[10];
        bool usedBuffer = stableMergeSortWithBuffer(arr, 4, buffer, 10);
        assert(usedBuffer == true);
        assert(std::is_sorted(arr, arr + 4));
        assert(arr[0] == 1 && arr[3] == 4);
    }

    // Test 6: Single element.
    {
        int arr[] = {42};
        int buffer[10];
        bool usedBuffer = stableMergeSortWithBuffer(arr, 1, buffer, 10);
        assert(usedBuffer == true);
        assert(arr[0] == 42);
    }

    // Test 7: Empty array.
    {
        int buffer[10];
        bool usedBuffer = stableMergeSortWithBuffer(nullptr, 0, buffer, 10);
        assert(usedBuffer == true);
    }

    // Test 8: Reverse sorted.
    {
        int arr[] = {5, 4, 3, 2, 1};
        int buffer[10];
        stableMergeSortWithBuffer(arr, 5, buffer, 10);
        assert(std::is_sorted(arr, arr + 5));
    }

    // Test 9: Exactly buffer size equals half (minimum requirement).
    {
        int arr[] = {4, 3, 2, 1};
        int buffer[2]; // Exactly half.
        bool usedBuffer = stableMergeSortWithBuffer(arr, 4, buffer, 2);
        assert(usedBuffer == true);
        assert(std::is_sorted(arr, arr + 4));
        assert(arr[0] == 1 && arr[3] == 4);
    }

    // Test 10: Buffer size exactly one less than needed, fallback.
    {
        int arr[] = {4, 3, 2, 1};
        int buffer[1]; // Need 2 for n=4.
        bool usedBuffer = stableMergeSortWithBuffer(arr, 4, buffer, 1);
        assert(usedBuffer == false);
        assert(std::is_sorted(arr, arr + 4));
    }

    return 0;
}
#include <cstddef>
#include <vector>

// Stable in-place merge sort using an external buffer.
// Returns true if the buffer was used for merging, false if we fell back to insertion sort.
// The buffer size is given in number of integers, not bytes.
bool stableMergeSortWithBuffer(int* arr, std::size_t n, int* buffer, std::size_t bufferSize) {
    if (n <= 1) return true; // Already sorted; buffer not needed.

    // Simple stable insertion sort for a subarray [begin, end).
    auto insertionSort = [](int* begin, int* end) {
        for (int* i = begin + 1; i < end; ++i) {
            int key = *i;
            int* j = i - 1;
            while (j >= begin && *j > key) {
                *(j + 1) = *j;
                --j;
            }
            *(j + 1) = key;
        }
    };

    // If the buffer is too small to hold at least half the array, fall back to insertion sort.
    // Actually we need buffer for the left half during merge; bufferSize must be at least (n+1)/2.
    if (bufferSize < (n + 1) / 2) {
        insertionSort(arr, arr + n);
        return false;
    }

    // Initial run size is the buffer size (but at most n).
    std::size_t runSize = bufferSize < n ? bufferSize : n;

    // Sort each run individually using insertion sort.
    for (std::size_t start = 0; start < n; start += runSize) {
        std::size_t end = (start + runSize < n) ? start + runSize : n;
        insertionSort(arr + start, arr + end);
    }

    // Merge pairs of runs with increasing run sizes.
    for (std::size_t width = runSize; width < n; width *= 2) {
        for (std::size_t left = 0; left < n; left += 2 * width) {
            std::size_t mid = (left + width < n) ? left + width : n;
            std::size_t right = (left + 2 * width < n) ? left + 2 * width : n;
            if (mid >= right) continue; // Only one run in this segment.

            // Copy the left run into the buffer.
            std::size_t leftLen = mid - left;
            for (std::size_t i = 0; i < leftLen; ++i) {
                buffer[i] = arr[left + i];
            }

            // Merge buffer (left run) and right run into arr[left..right).
            std::size_t i = 0;       // index in buffer
            std::size_t j = mid;     // index in right run
            std::size_t k = left;    // index in output area

            while (i < leftLen && j < right) {
                // Stability: take from left (buffer) when equal.
                if (buffer[i] <= arr[j]) {
                    arr[k++] = buffer[i++];
                } else {
                    arr[k++] = arr[j++];
                }
            }
            while (i < leftLen) {
                arr[k++] = buffer[i++];
            }
            // Remaining right elements are already in place.
        }
    }

    return true;
}
// The core algorithm is a classic bottom-up merge sort that uses an external buffer for merging. The input array is divided into runs of length `bufferSize` first, and each run is sorted individually using an in-place stable insertion sort (since O(k^2) is acceptable for small runs, and it is stable). Then, pairs of adjacent runs are merged using the provided buffer as temporary storage. The merge process copies the left run into the buffer and then merges the buffered left run with the right run directly into the original array, preserving stability by always taking from the left (buffer) when elements are equal. After merging, new runs are twice the size, and the process repeats until the entire array is sorted. The key edge case is when the buffer is too small to hold even one run: in that case, we cannot use the merge approach, so we sort the entire array with the same insertion sort (which is stable and O(n^2)). Time complexity is O(n log n) when the buffer is sufficient (each merge pass is O(n), and there are O(log(n/bufferSize)) passes, but in practice with a buffer of size at least half the array, it is optimal). Space complexity is O(bufferSize) beyond the input. The function must handle zero-length arrays gracefully and must be const-correct (the input array is mutable, but the buffer is treated as writable only through the function).
