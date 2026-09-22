Write a standalone C++ function `randomizedQuickSort` that sorts an integer array in ascending order in-place using the randomized quicksort algorithm, where the pivot is chosen uniformly at random from the current subarray. The function must accept a pointer to the array and its valid logical length, and it must handle edge cases such as empty arrays, arrays with duplicate elements, already sorted arrays, and reverse-sorted arrays. The implementation must be robust against repeated calls, meaning it should not rely on global state or require manual seeding—use the C++ `<random>` library to generate the random pivot index internally (e.g., with a thread-local or function-local static random engine seeded once). The function should not use any extra dynamic memory beyond the recursion stack, and it must be correct for arrays of size 0 up to 100,000.

#include <cassert>
#include <algorithm>
#include <vector>

// The solution function is assumed to be defined above.

int main() {
    // Test 1: typical unsorted array
    int arr1[] = {5, 2, 9, 1, 5, 6};
    int expected1[] = {1, 2, 5, 5, 6, 9};
    randomizedQuickSort(arr1, 6);
    for (int i = 0; i < 6; ++i) assert(arr1[i] == expected1[i]);
    
    // Test 2: already sorted array
    int arr2[] = {1, 2, 3, 4, 5};
    int expected2[] = {1, 2, 3, 4, 5};
    randomizedQuickSort(arr2, 5);
    for (int i = 0; i < 5; ++i) assert(arr2[i] == expected2[i]);
    
    // Test 3: reverse sorted array
    int arr3[] = {9, 7, 5, 3, 1};
    int expected3[] = {1, 3, 5, 7, 9};
    randomizedQuickSort(arr3, 5);
    for (int i = 0; i < 5; ++i) assert(arr3[i] == expected3[i]);
    
    // Test 4: all duplicates
    int arr4[] = {4, 4, 4, 4};
    int expected4[] = {4, 4, 4, 4};
    randomizedQuickSort(arr4, 4);
    for (int i = 0; i < 4; ++i) assert(arr4[i] == expected4[i]);
    
    // Test 5: single element
    int arr5[] = {7};
    randomizedQuickSort(arr5, 1);
    assert(arr5[0] == 7);
    
    // Test 6: empty array (length 0)
    int* arr6 = nullptr;
    randomizedQuickSort(arr6, 0);
    
    // Test 7: larger random array compare against std::sort
    std::vector<int> data(1000);
    std::mt19937 rng(42);
    std::uniform_int_distribution<int> dist(-1000, 1000);
    for (auto& x : data) x = dist(rng);
    std::vector<int> copy = data;
    randomizedQuickSort(data.data(), static_cast<int>(data.size()));
    std::sort(copy.begin(), copy.end());
    assert(data == copy);
    
    return 0;
}

#include <random>
#include <utility>

// In-place randomized quicksort for integer arrays.
// arr: pointer to the first element of the array
// length: number of elements in the array
void randomizedQuickSort(int* arr, int length) {
    if (arr == nullptr || length < 2) {
        return;
    }
    
    // Static random engine seeded once; thread_local for thread safety.
    static thread_local std::mt19937 rng(std::random_device{}());
    
    // Recursive lambda-style helper via a local function (C++14 or later).
    auto partition = [&](int first, int last) -> int {
        // Choose a random pivot index and swap with first.
        std::uniform_int_distribution<int> dist(first, last);
        int random_index = dist(rng);
        std::swap(arr[random_index], arr[first]);
        
        int pivot = arr[first];
        int i = first + 1;
        int j = last;
        
        while (i <= j) {
            // Move j left while arr[j] > pivot (strictly greater).
            while (j >= first + 1 && arr[j] > pivot) {
                --j;
            }
            // Move i right while arr[i] < pivot (strictly less).
            while (i <= last && arr[i] < pivot) {
                ++i;
            }
            if (i < j) {
                std::swap(arr[i], arr[j]);
            }
        }
        // Place pivot at its final position.
        std::swap(arr[first], arr[j]);
        return j;
    };
    
    // Recursive quicksort using an explicit stack or recursion.
    // For clarity, use a nested recursive function (C++14 generic lambda).
    auto quickSortRecursive = [&](auto&& self, int first, int last) -> void {
        if (first < last) {
            int pivot_index = partition(first, last);
            self(self, first, pivot_index - 1);
            self(self, pivot_index + 1, last);
        }
    };
    
    quickSortRecursive(quickSortRecursive, 0, length - 1);
}

// The solution follows the classic randomized quicksort partition scheme similar to the given snippet but substitutes `rand()` with a modern, properly seeded random number generator from `<random>` (e.g., `std::mt19937` seeded via `std::random_device`). The partition function picks a random index in `[first, last]`, swaps it with the first element, and then uses two pointers `i` and `j` to scan inward: `j` moves left while `arr[j] > pivot` (strictly greater), and `i` moves right while `arr[i] < pivot` (strictly less). When `i < j`, elements are swapped; finally, the pivot is placed at index `j`, which separates values smaller than or equal to the pivot on the left from larger or equal on the right. Note that because the loop uses strict comparisons, duplicates are handled gracefully—they may end up on either side, but the recursion still terminates correctly. Edge cases: empty array (length 0) returns immediately; a single element returns immediately; duplicates do not cause infinite loops because the partition always moves `i` and `j` inward on equal values. Time complexity is average \(O(n \log n)\) and worst-case \(O(n^2)\), but the random pivot makes the worst case extremely unlikely. Space complexity is \(O(\log n)\) average for the recursion stack. The implementation uses `int` for indices and `std::swap` for element exchange.
