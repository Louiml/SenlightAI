Given an array of integers and a positive integer k, write a C++ function that returns a vector containing the k largest distinct values from the array, sorted in descending order. If the number of distinct values is less than k, return an empty vector. The function should handle duplicate values by considering each distinct value only once. For example, given input {5, 3, 5, 8, 3, 9} and k = 3, the output should be {9, 8, 5} because these are the three largest distinct values in descending order.

// The problem requires extracting the k largest distinct elements from a collection. The simplest approach leverages the properties of a sorted container or sorting. Since we only need distinct values, we can store the elements in a `std::set<int>` which automatically removes duplicates and keeps elements sorted in ascending order. Then, to get the k largest, we can iterate the set in reverse order (from largest to smallest) and collect up to k elements. If we encounter fewer than k elements before exhausting the set, we return an empty vector to indicate insufficient distinct values. Edge cases include: k = 0 (return empty vector), all elements duplicate (distinct count may be less than k), and negative numbers (handled naturally by the set). The time complexity is O(n log n) due to inserting n elements into a set (each insertion O(log n)) and then iterating over at most min(k, distinct count) elements. The space complexity is O(n) for storing the distinct elements in the set and the output vector.  
// An alternative would be to sort a copy of the unique elements in descending order, but the set already provides the necessary ordering and uniqueness conveniently.

#include <vector>
#include <set>

// Return the k largest distinct values from the input array, sorted descending.
// If there are fewer than k distinct values, return an empty vector.
std::vector<int> kLargestDistinct(const std::vector<int>& arr, int k) {
    if (k <= 0) {
        return {};
    }

    // Insert all elements into a set to remove duplicates and sort ascending.
    std::set<int> distinct(arr.begin(), arr.end());

    // If not enough distinct values, return empty.
    if (static_cast<int>(distinct.size()) < k) {
        return {};
    }

    // Collect the k largest values by iterating in reverse.
    std::vector<int> result;
    result.reserve(k);
    int count = 0;
    for (auto it = distinct.rbegin(); it != distinct.rend() && count < k; ++it, ++count) {
        result.push_back(*it);
    }
    return result;
}

#include <cassert>

int main() {
    // Basic case with distinct values
    assert(kLargestDistinct({5, 3, 8, 9, 1}, 3) == std::vector<int>({9, 8, 5}));
    
    // Duplicates are ignored
    assert(kLargestDistinct({5, 5, 3, 3, 8, 8, 9, 9, 1, 1}, 3) == std::vector<int>({9, 8, 5}));
    
    // k larger than distinct count returns empty
    assert(kLargestDistinct({1, 2, 3}, 5) == std::vector<int>());
    
    // k equal to distinct count returns all descending
    assert(kLargestDistinct({4, -1, 7, 0}, 4) == std::vector<int>({7, 4, 0, -1}));
    
    // k = 0 returns empty
    assert(kLargestDistinct({1, 2, 3}, 0) == std::vector<int>());
    
    // Negative numbers handled
    assert(kLargestDistinct({-5, -2, -8, -2}, 2) == std::vector<int>({-2, -5}));
    
    // Single element
    assert(kLargestDistinct({42}, 1) == std::vector<int>({42}));
    
    // All same values, k=1 works
    assert(kLargestDistinct({7, 7, 7}, 1) == std::vector<int>({7}));
    
    // All same values, k=2 returns empty
    assert(kLargestDistinct({7, 7, 7}, 2) == std::vector<int>());
    
    // Large set, ensure only first k are taken
    assert(kLargestDistinct({10, 20, 30, 40, 50, 60}, 2) == std::vector<int>({60, 50}));
}
