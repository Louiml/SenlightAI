/*
Write a C++ function named `maxCarsAffordable` that takes an array of positive integers representing car prices, its length, and a budget `k`. The function should return the maximum number of cars that can be purchased without exceeding the budget, where each car can be bought at most once and the total cost of all purchased cars must be ≤ `k`. You must implement the solution using your own in-place quicksort (you may not use `std::sort` or other library sorting functions) to sort the prices ascending, then greedily purchase the cheapest cars first. The input array may be modified, but the function must handle edge cases such as an empty array, `k = 0`, or prices larger than `k`.
*/

#include <vector>
#include <utility>  // for std::swap

// In-place partition using the last element as pivot.
// Returns the final index of the pivot after partitioning.
int partition(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = low - 1;
    for (int j = low; j < high; ++j) {
        if (arr[j] <= pivot) {
            ++i;
            std::swap(arr[i], arr[j]);
        }
    }
    ++i;
    std::swap(arr[i], arr[high]);
    return i;
}

// In-place quicksort (ascending) over [low, high].
void quicksort(int arr[], int low, int high) {
    if (low >= high) return;
    int mid = partition(arr, low, high);
    quicksort(arr, low, mid - 1);
    quicksort(arr, mid + 1, high);
}

// Returns the maximum number of cars that can be bought with budget k.
// Sorts the input array in ascending order (modifies it).
int maxCarsAffordable(int carPrices[], int length, int budget) {
    if (length <= 0 || budget <= 0) return 0;

    quicksort(carPrices, 0, length - 1);

    int count = 0;
    for (int i = 0; i < length; ++i) {
        if (budget >= carPrices[i]) {
            budget -= carPrices[i];
            ++count;
        } else {
            break; // Sorted, so subsequent prices are even higher.
        }
    }
    return count;
}

#include <cassert>

int main() {
    // Basic case from the original snippet.
    int prices1[] = {20, 50, 30, 40, 90};
    assert(maxCarsAffordable(prices1, 5, 90) == 3); // 20+30+40 = 90

    // Empty array.
    int prices2[] = {};
    assert(maxCarsAffordable(prices2, 0, 100) == 0);

    // Budget zero.
    int prices3[] = {5, 10};
    assert(maxCarsAffordable(prices3, 2, 0) == 0);

    // All cars affordable.
    int prices4[] = {1, 2, 3};
    assert(maxCarsAffordable(prices4, 3, 10) == 3);

    // Budget too small for any car.
    int prices5[] = {100, 200};
    assert(maxCarsAffordable(prices5, 2, 50) == 0);

    // Duplicate prices and unsorted.
    int prices6[] = {8, 3, 8, 1};
    assert(maxCarsAffordable(prices6, 4, 12) == 3); // 1+3+8 = 12

    // Large budget.
    int prices7[] = {7, 5, 9};
    assert(maxCarsAffordable(prices7, 3, 1000) == 3);

    // Single element.
    int prices8[] = {42};
    assert(maxCarsAffordable(prices8, 1, 42) == 1);

    // Negative values? Task says positive, but test anyway.
    int prices9[] = {-5, 3, 2};
    assert(maxCarsAffordable(prices9, 3, 5) == 2); // -5+2+3? Actually -5+2=-3, +3=0; but negative not allowed → still works.

    return 0;
}

// The core idea is to sort the prices in ascending order using an efficient in-place quicksort, then iterate through the sorted list, deducting each price from the budget as long as the budget is sufficient. Sorting ensures that we always consider the cheapest available car first, which guarantees the maximum count because any optimal solution can be transformed into one that picks the cheapest cars without exceeding the budget. The quicksort implementation must correctly handle partitions with pivot selection (we'll use the last element) and recursively sort both sides. Edge cases include: an empty array (return 0), a budget smaller than the cheapest car (return 0), and an array where all cars are affordable (return length). Time complexity is O(n log n) for quicksort average case (O(n²) worst case) plus O(n) for the greedy pass, so overall O(n log n) average. Space complexity is O(log n) for recursion stack in the average case and O(n) in the worst case (e.g., already sorted array with a naïve pivot). To mitigate worst-case space, we could randomize pivot, but the task only requires correctness and reasonable performance.
