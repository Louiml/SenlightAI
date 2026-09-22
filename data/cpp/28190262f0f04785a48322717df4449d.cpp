/*
Write a C++ function that takes a vector of integers and returns a new vector with all duplicate adjacent elements removed after sorting the input in non-decreasing order. The input vector may contain negative numbers, zeros, and repeated values. The function must preserve the frequency of each value (not remove duplicates entirely), but only collapse consecutive equal values into a single occurrence following the sort. For example, the input `{3, 1, 3, 2, 2}` should first be sorted to `{1, 2, 2, 3, 3}`, then adjacent duplicates are removed to yield `{1, 2, 3}`. The function must be const-correct, take the vector by const reference, and return a new vector.
*/
#include <vector>
#include <algorithm>

// Remove adjacent duplicates from a sorted version of the input.
// Returns a new vector with each distinct value appearing once, in ascending order.
std::vector<int> uniqueSorted(const std::vector<int>& numbers) {
    if (numbers.empty()) {
        return {};
    }
    
    std::vector<int> sorted = numbers;
    std::sort(sorted.begin(), sorted.end());
    
    std::vector<int> result;
    result.reserve(sorted.size());
    
    for (size_t i = 0; i < sorted.size(); ++i) {
        if (i == 0 || sorted[i] != sorted[i - 1]) {
            result.push_back(sorted[i]);
        }
    }
    
    return result;
}
#include <cassert>
#include <vector>

// The solution function declaration is assumed to be visible here.
std::vector<int> uniqueSorted(const std::vector<int>& numbers);

int main() {
    assert(uniqueSorted({}) == std::vector<int>{});
    assert(uniqueSorted({5}) == std::vector<int>{5});
    assert(uniqueSorted({3, 1, 3, 2, 2}) == std::vector<int>{1, 2, 3});
    assert(uniqueSorted({-1, -1, 0, 0, 0, 2}) == std::vector<int>{-1, 0, 2});
    assert(uniqueSorted({7, 7, 7, 7}) == std::vector<int>{7});
    assert(uniqueSorted({2, 1, 2, 1, 2, 1}) == std::vector<int>{1, 2});
    assert(uniqueSorted({-10, 5, -10, 5, 0}) == std::vector<int>{-10, 0, 5});
    assert(uniqueSorted({1, 2, 3, 4}) == std::vector<int>{1, 2, 3, 4});
    return 0;
}
// The core idea is to first sort the input vector using an efficient sort (e.g., `std::sort`), which places equal values together. After sorting, iterate through the sorted vector and copy each element to a result vector only if it is different from the previous element (or is the first element). This ensures that all adjacent duplicates are collapsed into one occurrence per unique value. Edge cases include an empty input vector (returns an empty result), a vector with one element (returns that single element), and a vector with all identical values (returns a single element). Negative numbers and zeros are handled correctly by the standard comparison. Time complexity is \(O(n \log n)\) due to sorting, where \(n\) is the number of elements. Space complexity is \(O(n)\) for the result vector, plus \(O(\log n)\) for the sort's internal stack (if using `std::sort`). The auxiliary space for the algorithm itself (excluding the result) is \(O(1)\).
