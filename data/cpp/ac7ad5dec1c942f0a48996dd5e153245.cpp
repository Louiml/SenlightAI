Write a C++ function `void printIntersectionWithDuplicates(const int* arr1, int n1, const int* arr2, int n2)` that prints, in the order they appear in `arr2`, each element of `arr2` that also exists in `arr1`, but respecting the multiplicity of occurrences: if a value appears multiple times in both arrays, it should be printed as many times as the minimum of its counts in the two arrays (i.e., the number of times it can be paired). The function must handle empty arrays gracefully (printing nothing), and duplicate values within each array must be considered. The output should be each matching element on its own line, in the exact order they are encountered while scanning `arr2`. Assume all integers are within the range of `int` and that `arr1` and `arr2` are valid non-null pointers when their respective sizes are greater than zero.
The solution uses an `unordered_map<int, int>` to count the frequency of each element in `arr1`. First, iterate through `arr1` and increment the map value for each integer. Then, iterate through `arr2` from index 0 to n2-1. For each element in `arr2`, check if it exists in the map and if its current count is greater than zero. If so, print the element and decrement its count in the map. This ensures that duplicates are handled correctly: if a value appears twice in `arr1` and three times in `arr2`, it will be printed exactly twice, and the third occurrence in `arr2` will be ignored because the count becomes zero. An important edge case is when either array is empty—the function should produce no output, which is naturally handled because loops are not executed. Another edge case is when the same value appears many times; the map count prevents over-printing. The time complexity is \(O(n1 + n2)\) on average because each insertion and lookup in `unordered_map` is constant-time on average, and the space complexity is \(O(n1)\) for storing the frequencies. The function must be `const`-correct by taking pointer parameters as `const int*` and sizes as `int`.
#include <iostream>
#include <unordered_map>

// Prints common elements between two arrays, respecting multiplicity,
// in the order they appear in arr2.
void printIntersectionWithDuplicates(const int* arr1, int n1, const int* arr2, int n2) {
    std::unordered_map<int, int> freq;
    
    // Count occurrences in arr1
    for (int i = 0; i < n1; ++i) {
        ++freq[arr1[i]];
    }
    
    // Print matches from arr2
    for (int i = 0; i < n2; ++i) {
        auto it = freq.find(arr2[i]);
        if (it != freq.end() && it->second > 0) {
            std::cout << arr2[i] << std::endl;
            --(it->second);
        }
    }
}
#include <iostream>
#include <sstream>
#include <cassert>

// Forward declaration of the solution function
void printIntersectionWithDuplicates(const int* arr1, int n1, const int* arr2, int n2);

int main() {
    // Test 1: Basic intersection without duplicates
    {
        int arr1[] = {1, 2, 3, 4};
        int arr2[] = {3, 4, 5, 6};
        std::ostringstream out;
        std::streambuf* old = std::cout.rdbuf(out.rdbuf());
        printIntersectionWithDuplicates(arr1, 4, arr2, 4);
        std::cout.rdbuf(old);
        assert(out.str() == "3\n4\n");
    }

    // Test 2: Duplicate handling
    {
        int arr1[] = {2, 2, 3, 3, 3};
        int arr2[] = {2, 3, 2, 3, 2, 3};
        std::ostringstream out;
        std::streambuf* old = std::cout.rdbuf(out.rdbuf());
        printIntersectionWithDuplicates(arr1, 5, arr2, 6);
        std::cout.rdbuf(old);
        assert(out.str() == "2\n3\n2\n3\n");
    }

    // Test 3: Empty arr1
    {
        int arr1[] = {};
        int arr2[] = {1, 2};
        std::ostringstream out;
        std::streambuf* old = std::cout.rdbuf(out.rdbuf());
        printIntersectionWithDuplicates(arr1, 0, arr2, 2);
        std::cout.rdbuf(old);
        assert(out.str() == "");
    }

    // Test 4: Empty arr2
    {
        int arr1[] = {10, 20};
        int arr2[] = {};
        std::ostringstream out;
        std::streambuf* old = std::cout.rdbuf(out.rdbuf());
        printIntersectionWithDuplicates(arr1, 2, arr2, 0);
        std::cout.rdbuf(old);
        assert(out.str() == "");
    }

    // Test 5: All elements common with different counts
    {
        int arr1[] = {5, 5, 5, 5};
        int arr2[] = {5, 5, 5, 5, 5};
        std::ostringstream out;
        std::streambuf* old = std::cout.rdbuf(out.rdbuf());
        printIntersectionWithDuplicates(arr1, 4, arr2, 5);
        std::cout.rdbuf(old);
        assert(out.str() == "5\n5\n5\n5\n");
    }

    // Test 6: No common elements
    {
        int arr1[] = {1, 2, 3};
        int arr2[] = {4, 5, 6};
        std::ostringstream out;
        std::streambuf* old = std::cout.rdbuf(out.rdbuf());
        printIntersectionWithDuplicates(arr1, 3, arr2, 3);
        std::cout.rdbuf(old);
        assert(out.str() == "");
    }

    // Test 7: Single element each, common
    {
        int arr1[] = {42};
        int arr2[] = {42};
        std::ostringstream out;
        std::streambuf* old = std::cout.rdbuf(out.rdbuf());
        printIntersectionWithDuplicates(arr1, 1, arr2, 1);
        std::cout.rdbuf(old);
        assert(out.str() == "42\n");
    }

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
