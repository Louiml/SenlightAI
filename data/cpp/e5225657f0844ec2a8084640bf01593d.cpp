Write a standalone C++ function that takes as input an integer `n` (with `n >= 1`) and an array of `n` `double` values, where all elements except the first are guaranteed to be sorted in ascending order. The function must validate this constraint: if any adjacent pair among indices 2 through `n-1` (using 1-based indexing) violates the non-decreasing order, it should indicate an error — for simplicity, have the function return `false` if the input is invalid and `true` if valid. If valid, the function must implement a simple selection sort on the entire array (including the first element, which may be out of order) and output the sorted array to standard output, with each element followed by a space. The function should not modify the input array unless it first verifies the precondition; for safety, copy the array before sorting. The main algorithm must use nested loops (no `std::sort`), and the function must be `const`-correct with respect to the original input. Your task is to implement this logic in a free function named `validateAndSort`, taking parameters (`const std::vector<double>& arr`) and returning a `bool`. The function prints the sorted array only if the precondition holds, otherwise it prints nothing and returns `false`.
#include <cassert>
#include <sstream>

int main() {
    // Valid case: first element arbitrary, rest sorted.
    {
        std::ostringstream oss;
        std::streambuf* oldCout = std::cout.rdbuf(oss.rdbuf());
        bool ok = validateAndSort({5.0, 1.0, 2.0, 3.0});
        std::cout.rdbuf(oldCout);
        assert(ok);
        assert(oss.str() == "1 2 3 5 \n");
    }

    // Valid case: already fully sorted.
    {
        std::ostringstream oss;
        std::streambuf* oldCout = std::cout.rdbuf(oss.rdbuf());
        bool ok = validateAndSort({1.0, 2.0, 3.0, 4.0});
        std::cout.rdbuf(oldCout);
        assert(ok);
        assert(oss.str() == "1 2 3 4 \n");
    }

    // Valid case: single element.
    {
        std::ostringstream oss;
        std::streambuf* oldCout = std::cout.rdbuf(oss.rdbuf());
        bool ok = validateAndSort({7.5});
        std::cout.rdbuf(oldCout);
        assert(ok);
        assert(oss.str() == "7.5 \n");
    }

    // Invalid case: second-to-last element greater than last element.
    {
        std::ostringstream oss;
        std::streambuf* oldCout = std::cout.rdbuf(oss.rdbuf());
        bool ok = validateAndSort({0.0, 1.0, 5.0, 2.0});
        std::cout.rdbuf(oldCout);
        assert(!ok);
        assert(oss.str().empty());
    }

    // Invalid case: duplicate violation in the sorted portion (strictly descending).
    {
        std::ostringstream oss;
        std::streambuf* oldCout = std::cout.rdbuf(oss.rdbuf());
        bool ok = validateAndSort({0.0, 3.0, 2.0, 1.0});
        std::cout.rdbuf(oldCout);
        assert(!ok);
        assert(oss.str().empty());
    }

    // Valid case with duplicates in the sorted portion.
    {
        std::ostringstream oss;
        std::streambuf* oldCout = std::cout.rdbuf(oss.rdbuf());
        bool ok = validateAndSort({10.0, 1.0, 1.0, 2.0, 2.0});
        std::cout.rdbuf(oldCout);
        assert(ok);
        assert(oss.str() == "1 1 2 2 10 \n");
    }

    return 0;
}
#include <vector>
#include <iostream>

// Validates that all elements except the first are non-decreasing, then
// selection-sorts a copy of the array and prints the sorted result.
// Returns true on success (valid input), false if the precondition fails.
bool validateAndSort(const std::vector<double>& arr) {
    int n = static_cast<int>(arr.size());
    if (n < 1) return false;

    // Check that arr[1] through arr[n-1] are sorted ascending (non-decreasing).
    for (int i = 1; i < n - 1; ++i) {
        if (arr[i] > arr[i + 1]) {
            return false;
        }
    }

    // Create a copy to avoid modifying the caller's vector.
    std::vector<double> a = arr;

    // Selection sort on the copy.
    for (int i = 0; i < n - 1; ++i) {
        int minIndex = i;
        for (int j = i + 1; j < n; ++j) {
            if (a[j] < a[minIndex]) {
                minIndex = j;
            }
        }
        if (minIndex != i) {
            std::swap(a[i], a[minIndex]);
        }
    }

    // Print the sorted array with spaces.
    for (int i = 0; i < n; ++i) {
        std::cout << a[i] << " ";
    }
    std::cout << std::endl;

    return true;
}
// The solution first checks the precondition by iterating from index 1 to `n-2` (0-based) and verifying that `arr[i] <= arr[i+1]`. If any violation is found, return `false` immediately without modifying or printing anything. If the precondition holds, create a copy of the input vector (to avoid modifying the original), then perform selection sort on the copy: for each position `i` from 0 to `n-2`, find the index of the smallest element in the subarray `[i, n-1]`, and swap it with the element at `i`. After sorting, print each element followed by a space, then return `true`. Edge cases: `n=1` is valid (the loop over pairs does nothing, sorting is trivial), and duplicate values in the sorted portion are acceptable because the check uses `<=`. Time complexity is `O(n^2)` for the selection sort (the validation is `O(n)`), and space complexity is `O(n)` due to the copy. The use of `const` ensures the original array is not changed, and the function returns a status bool for error handling.
