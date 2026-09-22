// Write a C++ function that takes an integer `n` and a vector of `n` integers, sorts them in non-decreasing order, and then rearranges them into a "zigzag" sequence by repeatedly taking the smallest remaining element, then the largest remaining element, then the next smallest, then the next largest, and so on, finally outputting this sequence in reverse order. If `n` is odd, the remaining middle element is placed at the end of the zigzag sequence before reversing. Return a vector containing the final reversed zigzag sequence.

// The problem is straightforward after sorting: sort the input array in ascending order. Then build a new vector `v` by iterating through the first half of the sorted array and, for each index `i` from 0 to n/2-1, append `a[i]` (the next smallest) and then `a[n-1-i]` (the next largest). If `n` is odd, append the middle element `a[n/2]`. After constructing this "zigzag" sequence, reverse it in-place and return it. The algorithm's core is sorting, which takes \(O(n \log n)\) time, and the zigzag construction and reversal each take \(O(n)\) time, so total time is \(O(n \log n)\). Space usage is \(O(n)\) for the output vector, assuming the input vector is modified or copied; if we sort in-place and build `v`, we use \(O(n)\) additional space. Edge cases: `n=0` should return an empty vector; `n=1` returns the single element; duplicate values are handled naturally by sorting; even and odd lengths are distinguished properly, ensuring the middle element is inserted only for odd `n`. The final reversal matches the exact output format from the snippet, which printed the reversed zigzag vector.

#include <vector>
#include <algorithm>

// Given a vector of integers, sort it and build a reversed zigzag sequence.
std::vector<long long> reversedZigzag(std::vector<long long> a) {
    std::sort(a.begin(), a.end());
    const std::size_t n = a.size();
    std::vector<long long> v;
    v.reserve(n);
    for (std::size_t i = 0; i < n / 2; ++i) {
        v.push_back(a[i]);          // smallest remaining
        v.push_back(a[n - 1 - i]);  // largest remaining
    }
    if (n % 2 == 1) {
        v.push_back(a[n / 2]);      // middle element for odd length
    }
    std::reverse(v.begin(), v.end());
    return v;
}

#include <cassert>
#include <vector>
#include <initializer_list>

int main() {
    // Case 1: even length, distinct values
    std::vector<long long> r1 = reversedZigzag({4, 1, 3, 2});
    assert((r1 == std::vector<long long>{1, 4, 2, 3}));

    // Case 2: odd length
    std::vector<long long> r2 = reversedZigzag({5, 1, 9, 2, 7});
    assert((r2 == std::vector<long long>{1, 9, 2, 7, 5}));

    // Case 3: single element
    std::vector<long long> r3 = reversedZigzag({42});
    assert((r3 == std::vector<long long>{42}));

    // Case 4: empty vector
    std::vector<long long> r4 = reversedZigzag({});
    assert(r4.empty());

    // Case 5: duplicates
    std::vector<long long> r5 = reversedZigzag({2, 2, 2, 2});
    assert((r5 == std::vector<long long>{2, 2, 2, 2}));

    // Case 6: many elements, alternating extremes
    std::vector<long long> input = {10, -5, 3, 0, -1, 100};
    std::vector<long long> r6 = reversedZigzag(input);
    assert((r6 == std::vector<long long>{-5, 100, -1, 10, 0, 3}));

    // Case 7: already sorted ascending
    std::vector<long long> r7 = reversedZigzag({1, 2, 3, 4});
    assert((r7 == std::vector<long long>{1, 4, 2, 3}));

    // Case 8: already sorted descending
    std::vector<long long> r8 = reversedZigzag({9, 8, 7, 6});
    assert((r8 == std::vector<long long>{6, 9, 7, 8}));

    // Case 9: all negative
    std::vector<long long> r9 = reversedZigzag({-3, -1, -2});
    assert((r9 == std::vector<long long>{-3, -1, -2}));

    return 0;
}
