// Write a C++ function `int pancakeSortMoves(int n, const std::vector<int>& perm)` that determines whether a permutation of `{1,2,...,n}` (given 1-indexed values in a 0-indexed vector) can be sorted into ascending order using only prefix reversals (pancake flips). If it is impossible, return `-1`. Otherwise, return a string representation of the sequence of flip positions (each flip is an integer `k` meaning reverse the first `k` elements) that sorts the permutation. The function must guarantee that if a solution exists, it outputs exactly `5*(n-1)/2` moves (the standard constructive solution for odd `n`; you may assume `n` is odd). The output format is a single line containing the flip positions separated by spaces (e.g., `"3 2 5"`). If no solution exists, return `"-1"`.
// The key insight is that a prefix reversal (flip of the first `k` elements) changes the parity of the position of each element moved. Specifically, an element at index `i` (0-based) moves to index `k-1-i`; the difference between the new position and old position is `k-1-2i`, which is odd when `k` is even and even when `k` is odd. Therefore, the parity of an element's value relative to its position (i.e., `(value - index)` modulo 2) must be invariant under any sequence of flips? Actually, let's compute: after a flip of length `k`, the new index `j = k-1-i`. The parity of `(value - j)` is `(value - (k-1-i)) mod 2 = (value - (k-1) + i) mod 2`. Since `k-1` is odd/even depending on `k`, this may change. However, it's known that a permutation is sortable by prefix reversals only if the parity condition holds for every element: the difference between the value and its original index (1-based) must be even. In the original snippet, they check `(a[i]-i)%2` for 1-based indices. If any difference is odd, return `-1`. This is a necessary condition (and also sufficient for odd `n` with that constructive algorithm). The algorithm works as follows: process pairs `(i, i-1)` for `i = n, n-2, ..., 3`. For each such pair, first find position `f1` of value `i`, flip `f1` to bring `i` to the front. Then find position of value `i-1`; let `f2 = pos - 1` and `f3 = pos + 1` after the first flip. Then perform flips `f2`, `f3`, `3`, and `i`. This places `i` and `i-1` correctly at the tail. The entire sequence uses `(n-1)/2` pairs, each contributing 5 flips, so total `5*(n-1)/2` moves. After processing all pairs, the first two elements are already sorted (since parity condition ensures they are correct). Time complexity is `O(n^2)` due to repeated linear scans to find values. Space complexity `O(n)` for the copy of the array.
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>

// Reverse the first k elements of the array (1-indexed k).
static void reversePrefix(std::vector<int>& arr, int k) {
    int left = 0;
    int right = k - 1;
    while (left < right) {
        std::swap(arr[left], arr[right]);
        ++left;
        --right;
    }
}

// Return flip sequence as a space-separated string, or "-1" if impossible.
std::string pancakeSortMoves(int n, const std::vector<int>& perm) {
    // Check necessary parity condition: (value - index) must be even for all elements.
    // Using 1-based index: value - index must be even.
    for (int idx = 0; idx < n; ++idx) {
        int value = perm[idx];
        int pos1 = idx + 1; // 1-based position
        if ((value - pos1) % 2 != 0) {
            return "-1";
        }
    }

    // Work on a mutable copy.
    std::vector<int> arr = perm;
    std::vector<int> moves;

    // Process pairs (i, i-1) where i = n, n-2, ..., 3.
    for (int i = n; i >= 3; i -= 2) {
        // Find position of value i.
        int pos_i = -1;
        for (int idx = 0; idx < n; ++idx) {
            if (arr[idx] == i) {
                pos_i = idx;
                break;
            }
        }
        // Flip prefix of length pos_i+1 to bring i to front.
        int f1 = pos_i + 1;
        reversePrefix(arr, f1);
        moves.push_back(f1);

        // Now find position of value i-1.
        int pos_next = -1;
        for (int idx = 0; idx < n; ++idx) {
            if (arr[idx] == i - 1) {
                pos_next = idx;
                break;
            }
        }
        // After this flip, i-1 will be at index pos_next.
        // We will flip length f2 = pos_next (0-based index of element before i-1), then f3 = pos_next+2.
        int f2 = pos_next; // because we flip first f2 elements, bringing i-1 to position f2-1? Let's follow original snippet: they compute f2 = j-1, f3 = j+1 where j is 1-based position after first flip? Actually original uses arrays 1-indexed. Here we adapt.
        // Since arr is 0-indexed, after first flip, i is at index 0.
        // Find the element i-1 at index pos_next (0-based).
        // To place i-1 and i correctly, we flip f2 = pos_next, then f3 = pos_next+1? Let's derive properly.
        // Original code: after first reverse(f1), they find j such that a[j]==i-1. Then f2=j-1, f3=j+1 (1-indexed). Then reverse(f2), reverse(f3), reverse(3), reverse(i).
        // In 0-indexed terms: let p = position of i-1 (0-based). So f2 = p, f3 = p+2? Wait original f2 = j-1, f3 = j+1 where j is 1-indexed, so in 0-indexed, f2 = (j-1)-1? Let's map: original array a[1..n]. They find j such that a[j]==i-1. Then f2 = j-1 (1-indexed length). In 0-indexed, the position is pos = j-1. So f2 in 0-indexed is also pos? Actually the flip length is the number of elements, which is the same regardless of indexing. So if original j is 1-based, then number of elements before and including j-1 is (j-1). So flip length f2 = j-1. In 0-indexed, the position of i-1 is pos = j-1. So flip length f2 = pos. Similarly f3 = j+1 = pos+2. That matches.
        int f2 = pos_next;
        reversePrefix(arr, f2);
        moves.push_back(f2);

        int f3 = pos_next + 2;
        reversePrefix(arr, f3);
        moves.push_back(f3);

        reversePrefix(arr, 3);
        moves.push_back(3);

        reversePrefix(arr, i);
        moves.push_back(i);
    }

    // Build result string.
    std::stringstream ss;
    for (size_t k = 0; k < moves.size(); ++k) {
        if (k) ss << ' ';
        ss << moves[k];
    }
    return ss.str();
}
#include <cassert>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>

// Include the solution function declaration (assume above code is included).
std::string pancakeSortMoves(int n, const std::vector<int>& perm);

// Helper to apply a sequence of flips to a vector.
static void applyFlips(std::vector<int>& arr, const std::string& seq, int n) {
    std::stringstream ss(seq);
    int k;
    while (ss >> k) {
        int left = 0, right = k - 1;
        while (left < right) {
            std::swap(arr[left], arr[right]);
            ++left; --right;
        }
    }
}

int main() {
    // Test 1: Simple even parity permutation of size 3 (odd n).
    {
        int n = 3;
        std::vector<int> perm = {2, 3, 1};
        std::string res = pancakeSortMoves(n, perm);
        std::vector<int> arr = perm;
        applyFlips(arr, res, n);
        assert(arr[0] == 1 && arr[1] == 2 && arr[2] == 3);
        // Count moves should be 5*(3-1)/2 = 5.
        int moves = 0;
        std::stringstream ss(res);
        int x;
        while (ss >> x) ++moves;
        assert(moves == 5);
    }

    // Test 2: Already sorted.
    {
        int n = 5;
        std::vector<int> perm = {1,2,3,4,5};
        std::string res = pancakeSortMoves(n, perm);
        std::vector<int> arr = perm;
        applyFlips(arr, res, n);
        assert(arr == std::vector<int>({1,2,3,4,5}));
    }

    // Test 3: Another valid permutation.
    {
        int n = 5;
        std::vector<int> perm = {3,1,5,2,4};
        std::string res = pancakeSortMoves(n, perm);
        std::vector<int> arr = perm;
        applyFlips(arr, res, n);
        assert(arr == std::vector<int>({1,2,3,4,5}));
    }

    // Test 4: Impossible parity condition (e.g., value 1 at position 2 gives diff odd).
    {
        int n = 3;
        std::vector<int> perm = {1,3,2};
        assert(pancakeSortMoves(n, perm) == "-1");
    }

    // Test 5: Larger odd n = 7.
    {
        int n = 7;
        std::vector<int> perm = {4,7,2,1,6,3,5};
        std::string res = pancakeSortMoves(n, perm);
        std::vector<int> arr = perm;
        applyFlips(arr, res, n);
        assert(arr == std::vector<int>({1,2,3,4,5,6,7}));
        int moves = 0;
        std::stringstream ss(res);
        int x;
        while (ss >> x) ++moves;
        assert(moves == 5*(n-1)/2);
    }

    // Test 6: One element (n=1).
    {
        int n = 1;
        std::vector<int> perm = {1};
        std::string res = pancakeSortMoves(n, perm);
        assert(res == "");
    }

    // Test 7: Random permutation of size 9 satisfying parity.
    {
        int n = 9;
        std::vector<int> perm = {2,9,4,7,6,5,8,3,1};
        std::string res = pancakeSortMoves(n, perm);
        std::vector<int> arr = perm;
        applyFlips(arr, res, n);
        assert(arr == std::vector<int>({1,2,3,4,5,6,7,8,9}));
        int moves = 0;
        std::stringstream ss(res);
        int x;
        while (ss >> x) ++moves;
        assert(moves == 5*(n-1)/2);
    }

    return 0;
}
