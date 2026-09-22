Write a C++ function `std::pair<std::vector<int>, std::vector<int>> binarySearchReport(const std::vector<int>& arr, const std::vector<int>& queries)` that, for each query value, performs a classic binary search on a sorted array `arr` (which is guaranteed to be non-empty and sorted in non-decreasing order) and returns two parallel vectors: the first contains the index of the query if found, otherwise `-1`; the second contains the number of iterations (i.e., the number of times the middle element is compared) used during that binary search. The binary search must follow the exact procedure: initialize `l` and `r` to the full range, and in each iteration increment a counter, compute `mid = (l + r) / 2`, compare `arr[mid]` with the target, and adjust `l` or `r` accordingly. The function must handle duplicate values (return the first index found by the search, which may be any occurrence) and must not modify the input arrays. If multiple queries exist, each is processed independently.
// The solution is straightforward: for each query, run a standard binary search on the sorted array. The main algorithm is a loop over queries, and inside each query, we perform binary search while tracking the number of mid computations (iterations). Important edge cases: the array may contain duplicates, and the search might return any matching index (the algorithm stops at the first hit). The search must handle the case when the target is smaller than the first element or larger than the last—then `l` will exceed `r` and return `-1`. Also handle the case of a single-element array. The iteration count must be incremented inside the while loop before computing `mid` each time, matching the original snippet. Time complexity: each binary search takes \(O(\log n)\), so for `q` queries total is \(O(q \log n)\). Space complexity: \(O(q)\) for the output vectors, plus \(O(1)\) auxiliary per search.
#include <vector>
#include <utility>

// Performs binary search on a sorted vector and counts the number of mid comparisons.
// Returns a vector of indices (or -1 if not found) and a vector of iteration counts.
std::pair<std::vector<int>, std::vector<int>> binarySearchReport(
    const std::vector<int>& arr, 
    const std::vector<int>& queries) 
{
    std::vector<int> indexes;
    std::vector<int> iterations;
    indexes.reserve(queries.size());
    iterations.reserve(queries.size());

    for (int target : queries) {
        int l = 0;
        int r = static_cast<int>(arr.size()) - 1;
        int count = 0;
        int found = -1;

        while (l <= r) {
            ++count;
            int mid = l + (r - l) / 2; // avoids potential overflow
            if (arr[mid] == target) {
                found = mid;
                break;
            } else if (arr[mid] < target) {
                l = mid + 1;
            } else {
                r = mid - 1;
            }
        }

        indexes.push_back(found);
        iterations.push_back(count);
    }

    return {indexes, iterations};
}
#include <cassert>
#include <vector>
#include <utility>

// (The solution function is assumed to be included above.)

int main() {
    // Basic test: array of distinct numbers
    std::vector<int> arr1 = {1, 2, 3, 4, 5};
    std::vector<int> queries1 = {3, 1, 5, 0, 6};
    auto res1 = binarySearchReport(arr1, queries1);
    assert(res1.first == std::vector<int>({2, 0, 4, -1, -1}));
    // Iteration counts may be verified for correctness of the algorithm:
    // For 3: l=0,r=4, mid=2 -> found after 1
    // For 1: mid=2 -> arr[2]=3 > 1, r=1; mid=0 -> found after 2
    // For 5: mid=2 -> 3 < 5, l=3; mid=4 -> found after 2
    // For 0: mid=2 -> 3 > 0, r=1; mid=0 -> 1 > 0, r=-1; l=0,r=-1 exit after 2
    // For 6: mid=2 -> 3 < 6, l=3; mid=4 -> 5 < 6, l=5; exit after 2
    assert(res1.second == std::vector<int>({1, 2, 2, 2, 2}));

    // Duplicate values: returns the first found index
    std::vector<int> arr2 = {1, 2, 2, 2, 3};
    auto res2 = binarySearchReport(arr2, {2});
    assert(res2.first[0] >= 1 && res2.first[0] <= 3); // any duplicate index is acceptable
    assert(res2.second[0] > 0);

    // Single element array
    std::vector<int> arr3 = {7};
    auto res3 = binarySearchReport(arr3, {7, 8});
    assert(res3.first == std::vector<int>({0, -1}));
    assert(res3.second == std::vector<int>({1, 1}));

    // Empty query list
    std::vector<int> arr4 = {1, 2, 3};
    auto res4 = binarySearchReport(arr4, {});
    assert(res4.first.empty() && res4.second.empty());

    // Lower and upper bounds
    std::vector<int> arr5 = {1, 3, 5, 7, 9};
    auto res5 = binarySearchReport(arr5, {1, 9});
    assert(res5.first == std::vector<int>({0, 4}));
    assert(res5.second == std::vector<int>({3, 3})); // 1: mid=2->5>1, r=1; mid=0 found; 9: mid=2->5<9, l=3; mid=4 found

    // Empty array is not allowed per spec, but we can skip.

    return 0;
}
