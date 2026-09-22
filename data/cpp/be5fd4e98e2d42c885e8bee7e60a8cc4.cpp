Write a C++ function named `findUniqueNumbers` that accepts a `std::vector<int>` containing exactly `2N + 2` elements, where `N` is a non‑negative integer (i.e., the vector’s size is even and at least 2). All elements appear exactly twice except for two distinct elements that appear exactly once. The function must return a `std::vector<int>` containing those two unique numbers in any order. You may assume the input vector is non‑empty, has even length, and always contains exactly two elements with odd frequency; duplicates of the values that appear twice can be anywhere in the vector. The function must be `const`‑correct with respect to the input (take the vector by `const std::vector<int>&`) and should not modify the input. Provide a self‑contained implementation with no `main` function.
The straightforward approach is to count frequencies using a hash map (e.g., `std::unordered_map<int, int>`), then iterate over the map and collect all keys whose count equals 1. Because the problem guarantees exactly two such keys, we can stop early once we have collected two values, but that is not necessary. Edge cases: the vector has size 2 (both elements are unique, e.g., `{7, 3}`) → return both; the vector has only two values and they are the same? That would violate the problem statement (since then no unique numbers), so it is not considered. Also, negative numbers and large integers are handled naturally by the map. Time complexity: `O(M)` where `M` is the total number of elements; each insertion and lookup in an unordered map averages `O(1)` time. Space complexity: `O(K)` where `K` is the number of distinct values; in the worst case, `K = M/2 + 2`, so space is linear in input size.
#include <vector>
#include <unordered_map>

// Return a vector containing the two numbers that appear exactly once.
// The input vector has an even length >= 2, and every value appears either
// twice or once, with exactly two values appearing once.
std::vector<int> findUniqueNumbers(const std::vector<int>& nums) {
    std::unordered_map<int, int> frequency;
    for (int x : nums) {
        ++frequency[x];
    }
    
    std::vector<int> result;
    result.reserve(2);
    for (const auto& [value, count] : frequency) {
        if (count == 1) {
            result.push_back(value);
            if (result.size() == 2) {
                break;
            }
        }
    }
    return result;
}
#include <cassert>
#include <vector>
#include <algorithm>

// The free function is declared above or included, and we test it here.
int main() {
    // Example from the snippet: [1,2,1,3,2,5] → {3,5} in any order
    {
        std::vector<int> input = {1, 2, 1, 3, 2, 5};
        std::vector<int> result = findUniqueNumbers(input);
        assert(result.size() == 2);
        std::sort(result.begin(), result.end());
        assert((result == std::vector<int>{3, 5}));
    }
    // Two unique numbers only
    {
        std::vector<int> input = {7, 3};
        std::vector<int> result = findUniqueNumbers(input);
        assert(result.size() == 2);
        std::sort(result.begin(), result.end());
        assert((result == std::vector<int>{3, 7}));
    }
    // Duplicates at the end, unique values at the start
    {
        std::vector<int> input = {-2, 4, -2, 4, 9, 0, 9, 0, 100, 200};
        std::vector<int> result = findUniqueNumbers(input);
        assert(result.size() == 2);
        std::sort(result.begin(), result.end());
        assert((result == std::vector<int>{100, 200}));
    }
    // Large values and many duplicates
    {
        std::vector<int> input = {1000000, -1000000, 1000000, -1000000, 12345, 54321};
        std::vector<int> result = findUniqueNumbers(input);
        assert(result.size() == 2);
        std::sort(result.begin(), result.end());
        assert((result == std::vector<int>{12345, 54321}));
    }
    // Negative numbers and zeros
    {
        std::vector<int> input = {0, 0, -1, -1, -5, 5};
        std::vector<int> result = findUniqueNumbers(input);
        assert(result.size() == 2);
        std::sort(result.begin(), result.end());
        assert((result == std::vector<int>{-5, 5}));
    }
    // Stress: large vector where unique numbers are at the end
    {
        std::vector<int> input;
        for (int i = 0; i < 10000; ++i) {
            input.push_back(i);
            input.push_back(i);
        }
        input.push_back(12345);
        input.push_back(-1);
        std::vector<int> result = findUniqueNumbers(input);
        assert(result.size() == 2);
        std::sort(result.begin(), result.end());
        assert((result == std::vector<int>{-1, 12345}));
    }
    return 0;
}
