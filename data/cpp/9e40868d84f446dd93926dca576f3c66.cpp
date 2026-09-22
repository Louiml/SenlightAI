Given two positive integers `N` and `M` (where `1 ≤ N, M ≤ 8`), and a list of `N` distinct integers, write a C++ function that generates and returns all sequences of exactly `M` integers chosen from the given list, allowing repetitions, such that the order of the chosen integers matters (i.e., permutations with repetition). The output must be produced in the order determined by the sorted ascending order of the input list: for each position from 1 to `M`, the first element of each sequence is fixed, then for each such fixed prefix, the second element cycles through the sorted list from smallest to largest, and so on—this is exactly the lexicographic order based on the sorted input list. The function should not print anything; instead, it should collect all generated sequences into a vector of vectors (each inner vector containing `M` integers in the generated order) and return that vector. If `M` is 0, return an empty vector. The input list is guaranteed to contain distinct integers, but they may be unsorted and may include negative values. Your function must handle any valid `N` and `M` within the given bounds, and for efficiency, it should avoid unnecessary copies.
#include <cassert>
#include <vector>
#include <string>

// The solution function is provided above; here is a test harness.
int main() {
    // Test 1: N=2, M=2, list {2,1} (unsorted) -> sorted {1,2}, sequences of length 2 in lex order.
    std::vector<std::vector<int>> r1 = generateSequences({2, 1}, 2);
    assert(r1 == std::vector<std::vector<int>>({{1,1},{1,2},{2,1},{2,2}}));

    // Test 2: N=3, M=1, list {5, -1, 3} -> sorted {-1,3,5}.
    std::vector<std::vector<int>> r2 = generateSequences({5, -1, 3}, 1);
    assert(r2 == std::vector<std::vector<int>>({{-1},{3},{5}}));

    // Test 3: N=1, M=3, list {7} -> all sequences are {7,7,7}.
    std::vector<std::vector<int>> r3 = generateSequences({7}, 3);
    assert(r3 == std::vector<std::vector<int>>({{7,7,7}}));

    // Test 4: M=0 -> empty result.
    std::vector<std::vector<int>> r4 = generateSequences({1,2,3}, 0);
    assert(r4.empty());

    // Test 5: N=3, M=2, list {0, -2, 4} -> sorted {-2,0,4}.
    std::vector<std::vector<int>> r5 = generateSequences({0, -2, 4}, 2);
    assert(r5 == std::vector<std::vector<int>>({{-2,-2},{-2,0},{-2,4},{0,-2},{0,0},{0,4},{4,-2},{4,0},{4,4}}));

    // Test 6: N=2, M=4, list {1, 2} -> 16 sequences, first few verified.
    auto r6 = generateSequences({1,2}, 4);
    assert(r6.size() == 16);
    assert(r6[0] == std::vector<int>({1,1,1,1}));
    assert(r6[15] == std::vector<int>({2,2,2,2}));

    // Test 7: N=4, M=1, list {10, -1, 3, 9} -> sorted {-1,3,9,10}.
    auto r7 = generateSequences({10, -1, 3, 9}, 1);
    assert(r7 == std::vector<std::vector<int>>({{-1},{3},{9},{10}}));

    // Test 8: N=3, M=3, list of negatives -> sorted {-5,-3,-1}.
    auto r8 = generateSequences({-3, -1, -5}, 3);
    assert(r8.size() == 27);
    assert(r8[0] == std::vector<int>({-5,-5,-5}));
    assert(r8[26] == std::vector<int>({-1,-1,-1}));

    // Test 9: N=1, M=1 -> single element.
    auto r9 = generateSequences({42}, 1);
    assert(r9 == std::vector<std::vector<int>>({{42}}));

    // Test 10: N=2, M=2, list with duplicates? Input is guaranteed distinct, but test with distinct.
    auto r10 = generateSequences({-7, 0}, 2);
    assert(r10 == std::vector<std::vector<int>>({{-7,-7},{-7,0},{0,-7},{0,0}}));

    return 0;
}
#include <vector>
#include <algorithm>

// Generate all sequences of length M (with repetition) from sortedDistinct values,
// in lexicographic order based on the sorted order of the input list.
// Returns a vector of vectors, each inner vector containing one sequence.
std::vector<std::vector<int>> generateSequences(std::vector<int> values, int M) {
    std::vector<std::vector<int>> result;
    if (M <= 0) {
        return result;
    }

    // Sort the input list to define the lexicographic order.
    std::sort(values.begin(), values.end());
    int N = static_cast<int>(values.size());

    // Temporary storage for the current sequence.
    std::vector<int> current(M);

    // Recursive backtracking function.
    std::function<void(int)> backtrack = [&](int k) {
        if (k == M) {
            result.push_back(current);
            return;
        }
        for (int i = 0; i < N; ++i) {
            current[k] = values[i];
            backtrack(k + 1);
        }
    };

    backtrack(0);
    return result;
}
// The core algorithm is a recursive backtracking/DFS approach exactly modeled after the provided snippet. We first sort the input list of `N` distinct integers in ascending order. Then we define a recursive function `backtrack(k)` where `k` is the current depth (0-indexed position in the sequence). At each call, if `k == M`, we have a complete sequence of length `M`, so we push a copy of the current candidate sequence (stored in a temporary array) into the result vector and return. Otherwise, we iterate over all `N` indices from 0 to `N-1` in ascending order, assign the current position `k` to that index, and recurse with `k+1`. This guarantees that the sequences are generated in lexicographic order relative to the sorted list. Edge cases: if `M == 0`, the base case immediately triggers at the root, so the result is empty (correct, since there are no sequences of length 0). If `N == 0` (but problem says `N ≥ 1`), we would simply produce nothing. The sorted order is essential for lexicographic output. Time complexity: There are exactly `N^M` sequences, each of length `M`, so generating all of them requires `O(N^M * M)` time (for copying each sequence). The recursion depth is `M`, and auxiliary space is `O(M)` for the temporary array plus `O(N^M * M)` for the result storage (which is unavoidable). Since `N, M ≤ 8`, the maximum number of sequences is `8^8 = 16,777,216`, which is large but feasible given constraints. No special handling is needed for duplicates because the input is distinct.
