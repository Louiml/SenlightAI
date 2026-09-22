// Given an integer sequence stored in a `std::vector<int>`, write a C++ function named `longestIncreasingSubsequence` that returns a `std::vector<int>` containing one longest strictly increasing subsequence (LIS) of the input. If there are multiple valid LIS of the same maximal length, any one of them is acceptable. The input may contain duplicate values, negative numbers, and have length up to 1,000. The function must handle empty input by returning an empty vector. The subsequence must maintain the original relative order of elements, and the returned vector’s size should equal the length of the LIS.
//
// ##

#include <cassert>
#include <vector>

// Declaration of the function under test
std::vector<int> longestIncreasingSubsequence(const std::vector<int>& seq);

int main() {
    // Basic case with a known LIS length 4: [10, 20, 30, 50] or [10, 20, 30, 40]
    auto r1 = longestIncreasingSubsequence({10, 20, 10, 30, 20, 40, 50});
    assert(r1.size() == 5);  // The LIS is 10,20,30,40,50

    // Empty input
    assert(longestIncreasingSubsequence({}).empty());

    // Single element
    auto r2 = longestIncreasingSubsequence({7});
    assert(r2.size() == 1 && r2[0] == 7);

    // All equal elements => only one can be taken
    auto r3 = longestIncreasingSubsequence({5, 5, 5, 5});
    assert(r3.size() == 1 && r3[0] == 5);

    // Strictly decreasing sequence => LIS is just one element
    auto r4 = longestIncreasingSubsequence({9, 8, 7, 6, 5});
    assert(r4.size() == 1);

    // Negative numbers and duplicates mixed
    auto r5 = longestIncreasingSubsequence({-2, -1, -1, 0, 1});
    assert(r5.size() == 4);  // e.g., -2,-1,0,1

    // Sequence with large values
    auto r6 = longestIncreasingSubsequence({100, 1, 2, 3, 4, 5});
    assert(r6.size() == 5);  // 1,2,3,4,5

    // Already sorted increasing
    auto r7 = longestIncreasingSubsequence({1, 2, 3, 4, 5});
    assert(r7.size() == 5 && r7 == std::vector<int>({1, 2, 3, 4, 5}));

    // Check that the returned subsequence is strictly increasing
    auto r8 = longestIncreasingSubsequence({3, 1, 4, 1, 5, 9, 2, 6});
    for (size_t i = 1; i < r8.size(); ++i) {
        assert(r8[i - 1] < r8[i]);
    }

    // Larger random test with length 10
    std::vector<int> input = {0, 8, 4, 12, 2, 10, 6, 14, 1, 9};
    auto r9 = longestIncreasingSubsequence(input);
    assert(r9.size() == 4);  // LIS is 0,4,10,14 or 0,2,6,9 etc.

    return 0;
}

#include <vector>
#include <algorithm>
#include <functional>

// Returns one longest strictly increasing subsequence of the input.
std::vector<int> longestIncreasingSubsequence(const std::vector<int>& seq) {
    const int n = static_cast<int>(seq.size());
    if (n == 0) return {};

    std::vector<int> memo(n + 1, -1);   // memo[i] = length of LIS starting at index i-1 (0 means start from -1)
    std::vector<int> choices(n + 1, -1); // choices[i] = next index to take from state i-1

    std::function<int(int)> lis = [&](int start) -> int {
        // start is -1 meaning we haven't picked any element yet.
        int& ret = memo[start + 1];
        if (ret != -1) return ret;

        ret = 1; // default length including current element
        int bestNext = -1;

        for (int next = start + 1; next < n; ++next) {
            if (start == -1 || seq[start] < seq[next]) {
                int cand = lis(next) + 1;
                if (cand > ret) {
                    ret = cand;
                    bestNext = next;
                }
            }
        }
        choices[start + 1] = bestNext;
        return ret;
    };

    // Call with start=-1; the returned length includes the virtual start, so subtract 1.
    lis(-1);

    std::vector<int> result;
    std::function<void(int)> reconstruct = [&](int start) {
        if (start != -1) result.push_back(seq[start]);
        int next = choices[start + 1];
        if (next != -1) reconstruct(next);
    };

    reconstruct(-1);
    return result;
}

##

// The problem is the classic Longest Increasing Subsequence (LIS) reconstruction. The provided snippet uses memoized recursion with a `choices` array to store the next index in the optimal subsequence. For each starting index, the function computes the length of the longest increasing subsequence beginning at that index (or from a virtual start of -1) by trying all subsequent larger elements. To reconstruct, we follow the stored `choices` from the start. This approach is straightforward but runs in O(N²) time due to the nested loop for each state, and O(N) space for the memo and choices arrays. Edge cases: duplicates cannot be used in the subsequence because the condition requires strictly increasing values; an empty sequence returns an empty vector; a single-element sequence returns that element. The recursion depth could reach N, but N≤1000 is safe. Since the task calls for a free function, we avoid global variables and instead pass the sequence as input, using memoization with local vectors initialized to -1. Time complexity O(N²), space O(N).
//
// ##
