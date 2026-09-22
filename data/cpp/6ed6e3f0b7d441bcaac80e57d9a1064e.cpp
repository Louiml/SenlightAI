// Implement a C++ function named `kthLargest` that takes a `const std::vector<int>&` and an integer `k` (where 1 ≤ k ≤ vector size), and returns the k-th largest distinct element from the vector. The vector may contain duplicate values, which should be treated as a single occurrence when determining distinct elements. For example, for the vector `{10, 8, 7, 10, 6, 8, 5}` and k=2, the distinct sorted values are `{5, 6, 7, 8, 10}`, so the 2nd largest is `8`. The function should use a max-heap–based approach (you may implement a heap class or use `std::priority_queue`) to efficiently find the k-th largest distinct element without fully sorting the entire vector. The function must handle edge cases such as k equal to 1 (largest distinct value), k equal to the number of distinct values (smallest distinct value), and vectors where all elements are identical.
#include <cassert>
#include <vector>

// Forward declaration of the function to test.
int kthLargestDistinct(const std::vector<int>& numbers, int k);

int main() {
    // Basic case with mixed distinct values.
    assert(kthLargestDistinct({10, 8, 7, 6, 5, 4, 3, 2, 1}, 1) == 10);
    assert(kthLargestDistinct({10, 8, 7, 6, 5, 4, 3, 2, 1}, 3) == 8);
    assert(kthLargestDistinct({10, 8, 7, 6, 5, 4, 3, 2, 1}, 9) == 1);

    // Duplicates should be treated as one distinct value.
    assert(kthLargestDistinct({10, 8, 7, 10, 8, 7, 1}, 2) == 8);
    assert(kthLargestDistinct({5, 5, 5, 5}, 1) == 5);

    // k equals the number of distinct values → smallest distinct.
    assert(kthLargestDistinct({3, 1, 2, 3, 1, 2}, 3) == 1);

    // Negative numbers and zero.
    assert(kthLargestDistinct({-5, -1, -10, 0, 5, 5}, 2) == 0);

    // Empty vector is not allowed per spec, but test with a single element.
    assert(kthLargestDistinct({42}, 1) == 42);

    // Larger mixed test.
    std::vector<int> test = {100, 50, 75, 25, 100, 50, 75, 25, 1, 2};
    // Distinct: {1,2,25,50,75,100} → k=4 → 25 (since sorted desc: 100,75,50,25)
    assert(kthLargestDistinct(test, 4) == 25);

    return 0;
}
#include <vector>
#include <queue>
#include <unordered_set>

// Returns the k-th largest distinct element from the input vector.
// Assumes 1 <= k <= number of distinct elements in the vector.
int kthLargestDistinct(const std::vector<int>& numbers, int k) {
    // Deduplicate using an unordered_set.
    std::unordered_set<int> distinctSet(numbers.begin(), numbers.end());

    // Push all distinct values into a max-heap.
    std::priority_queue<int> maxHeap;
    for (int value : distinctSet) {
        maxHeap.push(value);
    }

    // Remove the (k-1) largest elements; the top is the k-th largest.
    for (int i = 1; i < k; ++i) {
        maxHeap.pop();
    }

    return maxHeap.top();
}
// The solution requires extracting distinct values from the input vector, then finding the k-th largest among them. A clean approach: first insert all vector elements into a `std::set` or `std::unordered_set` to automatically remove duplicates (the `std::set` also sorts them, but we can avoid full sorting by using a max-heap). However, to emphasize the heap concept from the snippet, we can use a max-heap (e.g., `std::priority_queue<int>`) to store distinct values. Steps: (1) Traverse the input vector and insert each value into an unordered_set to deduplicate. (2) Then push all distinct values into a max-heap. (3) Pop the heap k-1 times to discard the largest (k-1) elements, then the heap's top is the k-th largest distinct value. Edge cases: if k is 1, no pops are needed. If the vector has fewer distinct values than k, that's invalid since the function is guaranteed to receive a valid k within the distinct count. Time complexity is O(n + d log d) where n is vector size and d is number of distinct elements (due to heap construction and pops). Space complexity is O(d) for the set and heap. Using a `std::priority_queue` is straightforward and efficient. For very large inputs, this avoids full sort of all n elements.
