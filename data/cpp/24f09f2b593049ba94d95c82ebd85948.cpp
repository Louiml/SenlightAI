/*
Write a C++ function that takes a vector of pairs of integers and returns a new vector containing only the unique pairs, sorted in descending order by first value, and for pairs with equal first values, sorted in ascending order by second value. The input vector may contain duplicates, and the output should preserve the relative order of unique pairs after sorting. The function must not modify the input vector and should work with empty input, returning an empty vector in that case.
*/
#include <vector>
#include <algorithm>
#include <utility>

// Sort pairs in descending order by first, ascending by second.
bool descFirstAscSecond(const std::pair<int,int>& a, const std::pair<int,int>& b) {
    if (a.first != b.first)
        return a.first > b.first;
    return a.second < b.second;
}

// Return unique pairs from the input, sorted as specified.
std::vector<std::pair<int,int>> uniqueSortedPairs(const std::vector<std::pair<int,int>>& input) {
    std::vector<std::pair<int,int>> result = input; // copy to preserve const input
    std::sort(result.begin(), result.end(), descFirstAscSecond);
    
    // Remove consecutive duplicates by copying unique elements to a new vector
    std::vector<std::pair<int,int>> unique;
    for (const auto& p : result) {
        if (unique.empty() || unique.back() != p) {
            unique.push_back(p);
        }
    }
    return unique;
}
#include <cassert>
#include <vector>
#include <utility>

// The solution function is declared above; here we test it.
// (Assume the function definition is included before main.)
int main() {
    // Basic test with duplicates and mixed order
    std::vector<std::pair<int,int>> v1 = {{3,2}, {1,5}, {3,2}, {2,4}, {1,5}, {3,1}};
    auto r1 = uniqueSortedPairs(v1);
    std::vector<std::pair<int,int>> e1 = {{3,1}, {3,2}, {2,4}, {1,5}};
    assert(r1 == e1);

    // Empty input
    std::vector<std::pair<int,int>> v2;
    auto r2 = uniqueSortedPairs(v2);
    assert(r2.empty());

    // All identical pairs
    std::vector<std::pair<int,int>> v3 = {{5,5}, {5,5}, {5,5}};
    auto r3 = uniqueSortedPairs(v3);
    std::vector<std::pair<int,int>> e3 = {{5,5}};
    assert(r3 == e3);

    // Already sorted (descending first, ascending second) with no duplicates
    std::vector<std::pair<int,int>> v4 = {{10,1}, {9,2}, {9,1}, {8,3}};
    auto r4 = uniqueSortedPairs(v4);
    // Since 9,1 < 9,2 (second ascending), sorted order is {10,1}, {9,1}, {9,2}, {8,3}
    std::vector<std::pair<int,int>> e4 = {{10,1}, {9,1}, {9,2}, {8,3}};
    assert(r4 == e4);

    // Non-consecutive duplicates before sorting
    std::vector<std::pair<int,int>> v5 = {{2,1}, {4,4}, {2,1}, {3,3}, {4,4}};
    auto r5 = uniqueSortedPairs(v5);
    std::vector<std::pair<int,int>> e5 = {{4,4}, {3,3}, {2,1}};
    assert(r5 == e5);

    // Input with negative and zero values
    std::vector<std::pair<int,int>> v6 = {{-1,2}, {0,0}, {-1,2}, {0,1}};
    auto r6 = uniqueSortedPairs(v6);
    std::vector<std::pair<int,int>> e6 = {{0,0}, {0,1}, {-1,2}};
    assert(r6 == e6);

    return 0;
}
// The solution involves two main steps: first, sort the input vector using a custom comparator that compares pairs by first value in descending order (larger first values come first), and for equal first values, compares second values in ascending order (smaller second values come first). This ensures that after sorting, identical pairs are adjacent to each other. Second, remove consecutive duplicates by iterating through the sorted vector and copying only the first occurrence of each unique pair into a result vector. Edge cases include an empty input vector, a vector with all identical pairs, and a vector where multiple duplicates appear non-consecutively before sorting (which sorting handles by making them consecutive). Time complexity is O(n log n) due to sorting, where n is the number of pairs, and space complexity is O(n) for the result vector (or O(1) auxiliary space if we ignore the output). Using `std::sort` with a stable comparator is not necessary, but using `std::unique` combined with an output vector is straightforward.
