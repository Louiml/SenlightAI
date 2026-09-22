Write a C++ function named `quickSortWithMetrics` that accepts a vector of integers by reference, sorts it in ascending order using the quicksort algorithm, and also returns (via output parameters) the total number of key comparisons and data movements (swaps) performed during sorting. The function should implement the classic quicksort with a pivot chosen as the first element of the current subarray, exactly as in the provided snippet, and it must correctly handle empty vectors and vectors with duplicate values. The function signature should be: `void quickSortWithMetrics(std::vector<int>& data, int& comparisons, int& movements);`. You must implement the recursive partitioning and sorting logic yourself, without using `std::sort` or similar library functions.
// The core algorithm is quicksort with a "first element" pivot. For a subarray from index `low` to `high`, the pivot is chosen as `data[low]`. We then partition the remaining elements (from `low+1` to `high`) by scanning from the left to find an element greater than the pivot, and from the right to find an element less than or equal to the pivot. When such a pair is found with the left index less than the right index, we swap them and count a movement. After the left and right indices cross, the pivot is placed at its correct position `j` (the right index) by swapping with `data[low]`, counting another movement. Then we recursively sort the subarray to the left of the pivot (`low` to `j-1`) and the right of the pivot (`j+1` to `high`). Comparisons are counted each time we test an element against the pivot; the tricky part is that the provided snippet increments a comparison counter both inside the inner while loops and after each loop terminates, which double-counts in some cases; to stay faithful, we replicate that behavior exactly. Edge cases include an empty vector (no sorting, zero comparisons/movements), a vector with one element (already sorted, no comparisons/movements), and duplicate values (the algorithm still works because it moves elements less than or equal to the pivot to the right side). Time complexity is O(n log n) average and O(n²) worst-case (when pivot is always the smallest or largest), and space complexity is O(log n) average for the call stack (recursive depth), O(n) worst-case for a completely unbalanced partition.
#include <vector>

void quickSortWithMetrics(std::vector<int>& data, int& comparisons, int& movements) {
    comparisons = 0;
    movements = 0;

    // Recursive lambda or separate helper; we define a local recursive function via std::function
    // But to keep it simple and avoid overhead, we'll use a helper lambda with captures.
    // However, the task requires a free function, we can implement an inner recursive function
    // using a lambda with std::function or a private static helper.
    // To keep it clean, we'll define a lambda inside that calls itself via std::function.
    std::function<void(int, int)> qSort = [&](int low, int high) {
        if (low > high) return;

        int pivot = data[low];
        int i = low + 1;
        int j = high;

        while (i <= j) {
            // Scan from left for element > pivot
            while ((i <= high) && (data[i] <= pivot)) {
                i++;
                comparisons++;
            }
            comparisons++; // Count the failed comparison when loop exits

            // Scan from right for element <= pivot
            while ((j >= low) && (data[j] > pivot)) {
                j--;
                comparisons++;
            }
            comparisons++; // Count the failed comparison when loop exits

            if (i < j) {
                std::swap(data[i], data[j]);
                movements++;
            }
        }

        if (low < j) {
            std::swap(data[low], data[j]);
            movements++;
        }

        qSort(low, j - 1);
        qSort(j + 1, high);
    };

    qSort(0, static_cast<int>(data.size()) - 1);
}
#include <cassert>
#include <vector>
#include <functional>

// Include the solution above (in a real test, the function is defined before main)

int main() {
    // Test 1: empty vector
    std::vector<int> a;
    int cmp, mov;
    quickSortWithMetrics(a, cmp, mov);
    assert(a.empty());
    assert(cmp == 0 && mov == 0);

    // Test 2: single element
    std::vector<int> b = {42};
    quickSortWithMetrics(b, cmp, mov);
    assert(b.size() == 1 && b[0] == 42);
    assert(cmp == 0 && mov == 0);

    // Test 3: already sorted
    std::vector<int> c = {1, 2, 3, 4, 5};
    quickSortWithMetrics(c, cmp, mov);
    assert(c == std::vector<int>({1, 2, 3, 4, 5}));
    assert(cmp > 0 && mov == 0); // no swaps but comparisons happen

    // Test 4: reverse sorted
    std::vector<int> d = {5, 4, 3, 2, 1};
    quickSortWithMetrics(d, cmp, mov);
    assert(d == std::vector<int>({1, 2, 3, 4, 5}));
    assert(mov > 0);

    // Test 5: duplicates
    std::vector<int> e = {3, 1, 3, 2, 3};
    quickSortWithMetrics(e, cmp, mov);
    assert(e == std::vector<int>({1, 2, 3, 3, 3}));

    // Test 6: larger random-ish set
    std::vector<int> f = {10, -3, 7, 0, -5, 8, 2};
    quickSortWithMetrics(f, cmp, mov);
    assert(f == std::vector<int>({-5, -3, 0, 2, 7, 8, 10}));
    assert(cmp > 0 && mov > 0);

    // Test 7: all equal
    std::vector<int> g = {7, 7, 7, 7};
    quickSortWithMetrics(g, cmp, mov);
    assert(g == std::vector<int>({7, 7, 7, 7}));
    // No swaps should occur because pivot stays at first position after partitioning
    assert(mov == 0);

    // Test 8: two elements unsorted
    std::vector<int> h = {2, 1};
    quickSortWithMetrics(h, cmp, mov);
    assert(h == std::vector<int>({1, 2}));
    assert(mov == 1);

    // Test 9: two elements sorted
    std::vector<int> i = {1, 2};
    quickSortWithMetrics(i, cmp, mov);
    assert(i == std::vector<int>({1, 2}));
    assert(mov == 0);

    // Test 10: negative numbers and zero
    std::vector<int> j = {-10, 0, -20, 5};
    quickSortWithMetrics(j, cmp, mov);
    assert(j == std::vector<int>({-20, -10, 0, 5}));
}
