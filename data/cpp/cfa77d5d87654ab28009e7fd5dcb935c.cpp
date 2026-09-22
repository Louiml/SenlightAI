// Write a standalone C++ function that computes the `k`-th smallest element in a vector of integers using the Quickselect algorithm with random pivot selection. The function must handle duplicate values correctly; when there are duplicate values, the `k`-th smallest must refer to the value at sorted position `k` (where `k` is 0-indexed). The vector must not be modified; your function should work on a copy. The task requires implementing the algorithm from scratch (no `std::nth_element`), with a time complexity averaging `O(n)` and worst-case `O(n^2)`, and using `O(n)` auxiliary space for the copied vector plus `O(log n)` average stack space for recursion. Edge cases include `k` out of bounds (should return `std::nullopt`) or an empty vector. Assume the input is a `std::vector<int>` and the output is `std::optional<int>`.
// The solution uses the Quickselect algorithm, a variant of Quicksort that only recurses into the partition containing the target index. Since the input must remain unmodified, copy the vector. At each step, pick a random pivot index (using `rand()` or a better PRNG), swap it with the last element, and partition the subrange so that elements smaller than the pivot are on the left, equal in the middle, and larger on the right. After partitioning, determine which side contains index `k`: if `k` falls inside the equal region, return the pivot value; if `k` is left of the equal region, recurse on the left; otherwise recurse on the right. This approach correctly handles duplicates because the equal region is expanded to contain all elements equal to the pivot, and the lower bound of the equal region is tracked. Edge cases: if `k` is negative or >= vector size, return `std::nullopt`. Empty input also returns `nullopt`. Randomization helps avoid worst-case behavior on already-sorted inputs. Time complexity: average `O(n)`, worst-case `O(n^2)` when pivot choices are poor (probabilistically negligible with random selection). Space: `O(n)` for the copy and `O(log n)` average recursion depth.
#include <vector>
#include <optional>
#include <cstdlib>
#include <ctime>

// Helper function for partitioning a subrange [left, right] of the vector.
// Returns the index range [lower, upper] of elements equal to the chosen pivot.
static void partition(std::vector<int>& arr, int left, int right, int pivotIndex,
                      int& lower, int& upper) {
    int pivot = arr[pivotIndex];
    std::swap(arr[pivotIndex], arr[right]);

    lower = left;
    upper = right;

    int i = left;
    while (i <= upper) {
        if (arr[i] < pivot) {
            std::swap(arr[i], arr[lower]);
            ++lower;
            ++i;
        } else if (arr[i] > pivot) {
            std::swap(arr[i], arr[upper]);
            --upper;
        } else {
            ++i;
        }
    }
}

// Compute the k-th smallest element (0-indexed) in a copy of the input vector.
// Returns std::nullopt if k is out of bounds or the vector is empty.
std::optional<int> kthSmallest(const std::vector<int>& input, int k) {
    if (input.empty() || k < 0 || k >= static_cast<int>(input.size())) {
        return std::nullopt;
    }

    // Work on a copy to preserve the original.
    std::vector<int> arr = input;

    int left = 0;
    int right = static_cast<int>(arr.size()) - 1;

    while (left <= right) {
        int pivotIndex = left + std::rand() % (right - left + 1);
        int lower, upper;
        partition(arr, left, right, pivotIndex, lower, upper);

        if (k >= lower && k <= upper) {
            return arr[lower]; // All elements in the equal region are the pivot value.
        } else if (k < lower) {
            right = lower - 1;
        } else {
            left = upper + 1;
        }
    }

    // Should never reach here if k is valid.
    return std::nullopt;
}
#include <cassert>
#include <vector>
#include <optional>

// Declare the function from the solution (assume it is visible here).
std::optional<int> kthSmallest(const std::vector<int>& input, int k);

int main() {
    // Seed for random pivot selection (optional for reproducibility).
    std::srand(42);

    // Basic case
    std::vector<int> a = {5, 3, 8, 1, 9, 2};
    assert(kthSmallest(a, 0) == 1);
    assert(kthSmallest(a, 1) == 2);
    assert(kthSmallest(a, 5) == 9);

    // Duplicate values
    std::vector<int> b = {4, 2, 4, 1, 4, 3};
    assert(kthSmallest(b, 0) == 1);
    assert(kthSmallest(b, 1) == 2);
    assert(kthSmallest(b, 2) == 3);
    assert(kthSmallest(b, 3) == 4);
    assert(kthSmallest(b, 5) == 4);

    // Edge cases: empty vector and out-of-bounds k
    std::vector<int> c;
    assert(!kthSmallest(c, 0).has_value());
    assert(!kthSmallest(a, -1).has_value());
    assert(!kthSmallest(a, 6).has_value());

    // Single element
    std::vector<int> d = {42};
    assert(kthSmallest(d, 0) == 42);

    // Negative numbers
    std::vector<int> e = {-10, -1, -5, -3};
    assert(kthSmallest(e, 0) == -10);
    assert(kthSmallest(e, 3) == -1);

    return 0;
}
