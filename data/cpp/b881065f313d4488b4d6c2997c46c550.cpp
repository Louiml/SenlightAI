Write a C++ function named `findCommonElements` that takes three sorted vectors of integers as input (which may contain duplicate values) and returns a new vector containing the elements that appear in all three vectors, with each common element appearing only once in the result. The function must not modify the input vectors, must not use sets or maps or any other container that automatically removes duplicates, and must efficiently handle large inputs. The input vectors are guaranteed to be sorted in non-decreasing order, but may be empty. If there are no common elements, the function should return an empty vector.
The solution uses a three-pointer technique to traverse the three sorted arrays simultaneously. Since each array is sorted, we can compare the current elements at pointers `i`, `j`, `k`. If all three are equal, we record that value as a common element and advance all three pointers. To avoid duplicates in the result, after finding a match we skip any subsequent identical values in each array (since arrays may contain duplicates). If the current elements are not all equal, we advance the pointer(s) pointing to the smallest value(s) because any smaller value cannot appear in the other arrays later (as they are sorted). Edge cases include empty arrays (function returns empty vector immediately) and arrays with duplicates (skipping logic handles this). The time complexity is O(n1 + n2 + n3) where ni is the size of each array, and the auxiliary space is O(1) excluding the output vector. The function copies inputs internally to avoid modifying them, but this copying itself is O(n1+n2+n3) and uses extra space; however, this is acceptable because the spec forbids modifying inputs. An alternative is to not copy but to skip duplicates on the fly, which we implement here by advancing pointers past equal values when a match is found.
#include <vector>

// Find common elements in three sorted vectors (duplicates allowed in input, but result has unique elements)
// Returns a vector of integers that appear in all three input vectors, each appearing once.
// The input vectors are not modified.
std::vector<int> findCommonElements(
    const std::vector<int>& arr1,
    const std::vector<int>& arr2,
    const std::vector<int>& arr3
) {
    std::vector<int> result;
    size_t i = 0, j = 0, k = 0;
    const size_t n1 = arr1.size(), n2 = arr2.size(), n3 = arr3.size();

    while (i < n1 && j < n2 && k < n3) {
        int a = arr1[i];
        int b = arr2[j];
        int c = arr3[k];

        if (a == b && b == c) {
            result.push_back(a);
            // Skip all duplicates of this value in each array
            while (i < n1 && arr1[i] == a) i++;
            while (j < n2 && arr2[j] == a) j++;
            while (k < n3 && arr3[k] == a) k++;
        } else if (a <= b && a <= c) {
            // a is smallest, advance i
            i++;
        } else if (b <= a && b <= c) {
            // b is smallest, advance j
            j++;
        } else {
            // c is smallest, advance k
            k++;
        }
    }
    return result;
}
#include <cassert>
#include <vector>

// The solution function is assumed to be defined above.

int main() {
    std::vector<int> arr1 = {1, 5, 5, 10, 20, 20, 40, 80};
    std::vector<int> arr2 = {6, 7, 20, 20, 80, 100};
    std::vector<int> arr3 = {3, 4, 15, 20, 20, 70, 80, 120};
    std::vector<int> result = findCommonElements(arr1, arr2, arr3);
    assert(result == std::vector<int>({20, 80}));

    // No common elements
    std::vector<int> a1 = {1, 2, 3};
    std::vector<int> a2 = {4, 5, 6};
    std::vector<int> a3 = {7, 8, 9};
    assert(findCommonElements(a1, a2, a3) == std::vector<int>());

    // Single common element, no duplicates
    std::vector<int> b1 = {10, 20, 30};
    std::vector<int> b2 = {20, 40, 60};
    std::vector<int> b3 = {20, 50, 80};
    assert(findCommonElements(b1, b2, b3) == std::vector<int>({20}));

    // All identical arrays, duplicates
    std::vector<int> c1 = {5, 5, 5};
    std::vector<int> c2 = {5, 5, 5};
    std::vector<int> c3 = {5, 5, 5};
    assert(findCommonElements(c1, c2, c3) == std::vector<int>({5}));

    // One empty array
    std::vector<int> d1 = {1, 2};
    std::vector<int> d2 = {};
    std::vector<int> d3 = {1, 2};
    assert(findCommonElements(d1, d2, d3) == std::vector<int>());

    // Negative and zero values
    std::vector<int> e1 = {-3, -1, 0, 2};
    std::vector<int> e2 = {-1, 0, 1, 2};
    std::vector<int> e3 = {-2, -1, 0, 2};
    assert(findCommonElements(e1, e2, e3) == std::vector<int>({-1, 0, 2}));

    return 0;
}
