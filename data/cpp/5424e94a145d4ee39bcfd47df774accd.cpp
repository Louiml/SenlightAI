Write a C++ function `long long maximumTasksWithinTime(const std::vector<int>& taskDurations, long long availableTime)` that takes a vector of positive integer task durations and a total available time, and returns the maximum number of tasks that can be completed within that time limit. Tasks are independent and can be completed in any order; each task consumes exactly its duration from the available time, and once started, it must finish. The function must use a merge sort algorithm (not `std::sort`) to sort the task durations in ascending order, then greedily select tasks from shortest to longest until adding the next task would exceed the remaining time. The function should handle an empty vector (return 0), duplicate durations, and large time values (use `long long` for the time variables to avoid overflow). The design must be modular: split the sorting into a recursive `mergeSort` helper and a `merge` helper, both clearly commented. Provide a standalone solution.
// The task requires sorting the task durations in ascending order so that we can always attempt the shortest remaining task first, which maximizes the count of tasks completed within the fixed time budget. The approach:  
// 1. If the vector is empty, return 0 immediately (edge case).  
// 2. Implement merge sort on the vector: recursively divide the array into halves until single elements, then merge the halves by comparing the first elements of two temporary subarrays and placing the smaller into the original array. The merge routine needs three indices (left start, mid, right start) and two temporary vectors for the left and right halves. The merge step runs in O(n) per level, and since there are O(log n) levels, the total sort time is O(n log n).  
// 3. After sorting, iterate through the sorted vector, accumulating the sum of durations. For each duration, if `currentTime + duration <= availableTime`, add it, increment the count; otherwise, break because all later durations are larger, so no more tasks can be added.  
// Time complexity: O(n log n) for sorting + O(n) for greedy selection = O(n log n). Space complexity: O(n) due to the temporary arrays in merge (at any depth, the total auxiliary space is O(n), but the recursion stack also takes O(log n)). Edge cases: empty vector, single task, all tasks fit, no tasks fit, very large time (use `long long` to avoid overflow when summing many tasks).
#include <vector>
#include <cstddef> // for size_t

// Merges two sorted subarrays arr[left..mid] and arr[mid+1..right] into arr[left..right].
void merge(std::vector<int>& arr, int left, int mid, int right) {
    int leftSize = mid - left + 1;
    int rightSize = right - mid;

    // Temporary vectors for left and right halves.
    std::vector<int> leftPart(leftSize);
    std::vector<int> rightPart(rightSize);

    // Copy elements into temporary vectors.
    for (int i = 0; i < leftSize; ++i) {
        leftPart[i] = arr[left + i];
    }
    for (int j = 0; j < rightSize; ++j) {
        rightPart[j] = arr[mid + 1 + j];
    }

    // Merge the two subarrays back into arr[left..right].
    int i = 0, j = 0, k = left;
    while (i < leftSize && j < rightSize) {
        if (leftPart[i] <= rightPart[j]) {
            arr[k] = leftPart[i];
            ++i;
        } else {
            arr[k] = rightPart[j];
            ++j;
        }
        ++k;
    }

    // Copy any remaining elements from leftPart.
    while (i < leftSize) {
        arr[k] = leftPart[i];
        ++i;
        ++k;
    }

    // Copy any remaining elements from rightPart.
    while (j < rightSize) {
        arr[k] = rightPart[j];
        ++j;
        ++k;
    }
}

// Recursive merge sort on arr[left..right].
void mergeSort(std::vector<int>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2; // Avoid overflow for large indices.
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

// Returns the maximum number of tasks that can be completed within availableTime.
// Tasks are sorted in ascending order using merge sort, then greedily selected.
long long maximumTasksWithinTime(const std::vector<int>& taskDurations, long long availableTime) {
    if (taskDurations.empty()) {
        return 0;
    }

    // Make a copy to sort, leaving the input unchanged.
    std::vector<int> durations = taskDurations;
    mergeSort(durations, 0, static_cast<int>(durations.size()) - 1);

    long long usedTime = 0;
    long long count = 0;

    for (int duration : durations) {
        if (usedTime + static_cast<long long>(duration) <= availableTime) {
            usedTime += duration;
            ++count;
        } else {
            // Since durations are sorted, no later task can be added either.
            break;
        }
    }

    return count;
}
#include <cassert>
#include <vector>
#include <cstddef>

// (The solution function and its helpers are assumed to be included here.)

int main() {
    // Basic cases
    assert(maximumTasksWithinTime({5, 3, 1}, 5) == 2);          // 1 and 3 fit, total 4
    assert(maximumTasksWithinTime({5, 3, 1}, 4) == 2);          // 1 and 3 fit, total 4
    assert(maximumTasksWithinTime({5, 3, 1}, 3) == 1);          // only 1 or 3 fits
    assert(maximumTasksWithinTime({2, 2, 2, 2}, 5) == 2);       // two 2s fit, total 4

    // Edge cases: empty vector, single task, no tasks fit
    assert(maximumTasksWithinTime({}, 10) == 0);
    assert(maximumTasksWithinTime({7}, 7) == 1);
    assert(maximumTasksWithinTime({7}, 6) == 0);
    assert(maximumTasksWithinTime({8, 9, 10}, 7) == 0);

    // Duplicates and large time values
    assert(maximumTasksWithinTime({1, 1, 1, 1, 1}, 3) == 3);
    assert(maximumTasksWithinTime({1000000, 2000000, 3000000}, 1000000000000LL) == 3);
    assert(maximumTasksWithinTime({999999999, 1, 2}, 1000000000000LL) == 3);

    // Unsorted input tests sorting correctness
    assert(maximumTasksWithinTime({4, 2, 3, 1}, 6) == 3);       // 1+2+3=6
    assert(maximumTasksWithinTime({10, 20, 30, 40}, 45) == 2);  // 10+20=30, can't add 30

    return 0;
}
