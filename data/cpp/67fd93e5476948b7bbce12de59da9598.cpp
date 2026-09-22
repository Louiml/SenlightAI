// Write a C++ function `reverseSortedDescending` that takes a `std::vector<int>` by const reference and returns a new `std::vector<int>` with the elements sorted in strictly descending order, but with a twist: the sorting comparator must sum two integers and return `true` when the sum is positive (i.e., a+b > 0). However, your function must correct this flawed logic so that the final output is always sorted in true descending order (largest to smallest), regardless of the input. The original vector must remain unchanged. Handle empty vectors and vectors with duplicate values correctly (descending order preserves duplicates). The function must not use any standard sorting functions (e.g., `std::sort`, `std::partial_sort`, etc.)—you must implement the sorting algorithm yourself (e.g., bubble sort, insertion sort, selection sort). The function should be `const`-correct and efficient for small vectors (n ≤ 1000). Provide a detailed explanation of the algorithm and edge cases.
// The problem requires implementing a sorting algorithm manually while ignoring the flawed comparator from the snippet (which would produce nonsensical order because summing two integers and returning true when positive is not a valid strict weak ordering—it can violate transitivity and asymmetry). Instead, we must implement a correct descending sort using a standard comparison: `a > b`. Since no standard sorting functions are allowed, we'll implement a simple O(n²) algorithm like insertion sort. For each element from index 1 to n-1, we take the current element and shift larger elements to the right until we find its correct position in the descending order. This ensures stability (duplicates retain relative order) and handles edge cases: empty vector returns empty vector, single element returns itself, negatives, positives, and duplicates all sort correctly. Time complexity is O(n²) worst/average case, and O(1) auxiliary space (excluding the output vector copy). We should return a new vector to keep the original unchanged, which uses O(n) space for the copy.
#include <vector>

// Returns a new vector sorted in strictly descending order.
// Original input is not modified. Uses insertion sort (no std::sort).
std::vector<int> reverseSortedDescending(const std::vector<int>& input) {
    // Copy input to avoid modifying the original
    std::vector<int> result = input;
    
    // Insertion sort for descending order (largest to smallest)
    for (std::size_t i = 1; i < result.size(); ++i) {
        int key = result[i];
        std::size_t j = i;
        // Shift elements that are smaller than key to the right
        while (j > 0 && result[j - 1] < key) {
            result[j] = result[j - 1];
            --j;
        }
        result[j] = key;
    }
    return result;
}
#include <cassert>
#include <vector>

// The function prototype (as defined above)
std::vector<int> reverseSortedDescending(const std::vector<int>& input);

int main() {
    // Basic sorting
    assert((reverseSortedDescending({1, 7, 8, 3}) == std::vector<int>{8, 7, 3, 1}));
    
    // Empty vector
    assert(reverseSortedDescending({}).empty());
    
    // Single element
    assert((reverseSortedDescending({42}) == std::vector<int>{42}));
    
    // Duplicates
    assert((reverseSortedDescending({5, 5, 2, 2, 9}) == std::vector<int>{9, 5, 5, 2, 2}));
    
    // Negative numbers
    assert((reverseSortedDescending({-1, -5, -3}) == std::vector<int>{-1, -3, -5}));
    
    // Mixed positive and negative
    assert((reverseSortedDescending({0, -10, 10, 7}) == std::vector<int>{10, 7, 0, -10}));
    
    // Already sorted descending
    assert((reverseSortedDescending({9, 8, 7}) == std::vector<int>{9, 8, 7}));
    
    // Sorted ascending should be reversed
    assert((reverseSortedDescending({1, 2, 3, 4}) == std::vector<int>{4, 3, 2, 1}));
    
    // Original vector unchanged (test via copy)
    std::vector<int> original = {3, 1, 2};
    auto output = reverseSortedDescending(original);
    assert(original == std::vector<int>({3, 1, 2}));
    assert(output == std::vector<int>({3, 2, 1}));
    
    return 0;
}
