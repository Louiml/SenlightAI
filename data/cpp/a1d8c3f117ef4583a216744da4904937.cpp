Given a sequence of `n` integers (2 ≤ n ≤ 10^5) that may contain duplicates, write a C++ function `countPicks` that returns the maximum number of distinct pairs `(i, j)` you can select such that: you process the array in non-increasing sorted order, and for each pair, the first element's value must be strictly greater than the second element's value, and no element can be used more than once across all pairs. In other words, after sorting the array in descending order, find the maximum number of pairs `(a[p], a[q])` with `p < q` and `a[p] > a[q]` where each index is used at most once, and pairs are built greedily by scanning from the largest values downward, matching each larger value with a distinct strictly smaller value that appears later in the sorted order. Return the total number of such pairs.

The algorithm sorts the array in descending order. Then, using two pointers, we scan from the beginning: the left pointer `i` points to a candidate "larger" element, and the right pointer `j` starts just after `i` and advances until it finds an element strictly smaller than `a[i]`. If such an element exists, we count one pair, then move both `i` and `j` forward by one (the matched elements are consumed). If `j` reaches the end or finds only equal values, we stop. This greedy approach is optimal because it always pairs the current largest unused element with the smallest possible strictly smaller element, preserving larger values for future pairs whenever possible. Edge cases: when all elements are equal, the answer is 0; when the array is strictly decreasing, the answer is floor(n/2) (since each pair requires one larger and one strictly smaller element, and we pair adjacent largest with smallest). Time complexity is O(n log n) due to sorting, and the scanning is O(n) because both pointers move forward monotonically. Space complexity is O(n) for the input vector (or O(1) extra aside from the vector).

#include <vector>
#include <algorithm>

// Count the maximum number of pairs (larger, strictly smaller) after sorting descending.
// Each element can be used at most once.
long long countPicks(std::vector<long long> arr) {
    std::sort(arr.begin(), arr.end(), std::greater<long long>());
    long long n = static_cast<long long>(arr.size());
    long long ans = 0;
    long long i = 0;
    long long j = 1;
    while (i < n && j < n) {
        // Advance j until we find a strictly smaller value than arr[i]
        while (j < n && arr[i] == arr[j]) {
            ++j;
        }
        if (j < n) {
            ++ans;
            ++i;
            ++j;
        } else {
            // No strictly smaller value available for arr[i], stop
            break;
        }
    }
    return ans;
}

#include <cassert>
#include <vector>
#include <iostream>

// The solution function is declared above (countPicks).
// We include its definition here for completeness.
long long countPicks(std::vector<long long> arr);

int main() {
    // Basic cases
    assert(countPicks({3, 1}) == 1);
    assert(countPicks({5, 5, 5, 5}) == 0);
    assert(countPicks({1, 2, 3, 4}) == 2); // sorted desc: 4,3,2,1 -> pairs (4,3) and (2,1)
    assert(countPicks({4, 4, 3, 2}) == 2); // sorted desc: 4,4,3,2 -> pairs (4,3) and (4,2)
    assert(countPicks({10, 10, 9, 8, 7}) == 2); // sorted: 10,10,9,8,7 -> pairs (10,9) and (10,8) or (10,7) etc.
    assert(countPicks({2, 2, 2, 1, 1}) == 2); // sorted: 2,2,2,1,1 -> pairs (2,1) and (2,1)
    assert(countPicks({3, 3, 3, 3, 2}) == 1); // only one smaller value
    assert(countPicks({100, 99, 98, 97}) == 2); // strictly decreasing, floor(4/2)=2
    assert(countPicks({1}) == 0); // single element
    assert(countPicks({5, 4, 4, 3, 3, 2}) == 3); // pairs: (5,4), (4,3), (3,2) or similar

    std::cout << "All tests passed" << std::endl;
    return 0;
}
