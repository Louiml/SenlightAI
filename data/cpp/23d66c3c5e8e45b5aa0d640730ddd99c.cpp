// Write a C++ function `vector<pair<int,int>> constructPairs(const vector<int>& sequence)` that processes an array of integers where each element is 1, 2, or 3. The function must simulate a pairing process scanning the array from right to left. For each element: a value of 1 is stored for potential later pairing; a value of 2 must be paired with the most recently stored 1 (from the right side); a value of 3 must be paired with the most recently stored 3 or 1, and then itself becomes available for future pairing. The function returns a list of coordinate-pairs (row index, column index) where each pair represents a vertical or horizontal segment in a triangular pattern: every element with value ≥ 1 generates a pair (i, i) to itself, and every element with value 2 or 3 generates an additional pair (i, j) where j is the index it was paired with during the right-to-left scan. If at any point a 2 or 3 cannot find an available partner, the function should return an empty vector. The returned pairs must be sorted by row index (first element of the pair). The function must not modify the input array; use const reference for the parameter. The returned vector contains pairs where both indices are 0-based; note that the original code outputs 1-based coordinates, but your function returns 0-based pairs.

// The solution processes the array from right to left while maintaining two stacks: one (`ones`) for indices of elements with value 1 that are still unpaired, and another (`threes`) for indices of elements with value 3 that are available for pairing. When encountering a 1, push its index onto `ones`. For a 2, pop from `ones` (if empty, fail). For a 3, pop from `threes` if available, otherwise pop from `ones` (if both empty, fail), and then push the current index onto `threes` because this 3 can serve as a partner for future elements to its left. Store each successful pairing (the current right-side index with the popped left-side index) in a temporary vector `edges`. After the scan, sort `edges` by the first (row) index because later we need to consume these in row order. Then, iterate left to right through the array: for each element with value ≥1, add the pair (i,i). For each element with value ≥2, also add the pair (i, j) where j is the second element from the next edge in the sorted `edges` list (which corresponds to the matched column). Important edge case: the original code mutates `a[x[j].second]` to handle overlapping segments for value 3, but for our abstract task, we only need to output pairs; we must ensure that the pairing logic matches the original behavior. The original logic for each row i with a[i]>=2 pairs it with the edge's second element but also decrements the value at that second element if the sum of the row's and second's original values equals 3 (i.e., one is 2 and the other is 3). However, since we do not need to output the matrix, we can ignore that mutation for the pair list; the pair list itself is unaffected. The time complexity is O(n log n) due to sorting the edges (which are at most n), and O(n) auxiliary space for stacks and the result vector. Edge cases: if any 2 or 3 has no partner, return an empty vector. Also handle empty input by returning empty vector.

#include <vector>
#include <algorithm>

// Process the sequence from right to left, pairing elements as described.
// Return a vector of 0-based index pairs (row, col) sorted by row.
// If an impossible pairing occurs, return an empty vector.
std::vector<std::pair<int, int>> constructPairs(const std::vector<int>& sequence) {
    const int n = static_cast<int>(sequence.size());
    std::vector<int> ones;   // indices of 1s available for pairing
    std::vector<int> threes; // indices of 3s available for pairing
    std::vector<std::pair<int, int>> edges; // (rightIndex, leftPartner)

    // Scan from right to left.
    for (int i = n - 1; i >= 0; --i) {
        if (sequence[i] == 1) {
            ones.push_back(i);
        } else if (sequence[i] == 2) {
            if (ones.empty()) {
                return {};
            }
            int partner = ones.back();
            ones.pop_back();
            edges.emplace_back(i, partner);
        } else if (sequence[i] == 3) {
            if (!threes.empty()) {
                int partner = threes.back();
                threes.pop_back();
                edges.emplace_back(i, partner);
            } else if (!ones.empty()) {
                int partner = ones.back();
                ones.pop_back();
                edges.emplace_back(i, partner);
            } else {
                return {};
            }
            threes.push_back(i); // This 3 becomes available for future (left) partners.
        }
    }

    // Sort edges by row (first index) for later consumption.
    std::sort(edges.begin(), edges.end());

    std::vector<std::pair<int, int>> result;
    size_t edgeIdx = 0;
    for (int i = 0; i < n; ++i) {
        if (sequence[i] >= 1) {
            result.emplace_back(i, i);
        }
        if (sequence[i] >= 2) {
            if (edgeIdx >= edges.size()) {
                return {}; // Shouldn't happen if the right-to-left scan succeeded.
            }
            int col = edges[edgeIdx].second;
            ++edgeIdx;
            result.emplace_back(i, col);
        }
    }
    return result;
}

#include <cassert>
#include <vector>

// The solution function is assumed to be defined above.

int main() {
    // Test 1: simple pattern with 1,2
    {
        std::vector<int> seq = {1, 2};
        auto res = constructPairs(seq);
        std::vector<std::pair<int,int>> expected = {{0,0}, {1,1}, {1,0}};
        assert(res == expected);
    }
    // Test 2: pattern with 3 using a 1 as partner
    {
        std::vector<int> seq = {1, 3};
        auto res = constructPairs(seq);
        std::vector<std::pair<int,int>> expected = {{0,0}, {1,1}, {1,0}};
        assert(res == expected);
    }
    // Test 3: impossible case (2 with no 1 to its right)
    {
        std::vector<int> seq = {2};
        assert(constructPairs(seq).empty());
    }
    // Test 4: chain of 3s
    {
        std::vector<int> seq = {3, 3};
        auto res = constructPairs(seq);
        std::vector<std::pair<int,int>> expected = {{0,0}, {1,1}, {1,0}};
        assert(res == expected);
    }
    // Test 5: mixed 1,2,3 in order that works
    {
        std::vector<int> seq = {1, 2, 3};
        auto res = constructPairs(seq);
        // Scan right-to-left:
        // index 2 (3): pairs with index0 (1) -> edge (2,0), then push 2 to threes
        // index 1 (2): ones empty? yes -> fail
        // Actually sequence {1,2,3}: rightmost is 3 -> pairs with 1 (index0), then threes has 2.
        // Then index1 is 2: ones empty -> fail.
        assert(constructPairs(seq).empty());
    }
    // Test 6: all ones
    {
        std::vector<int> seq = {1, 1, 1};
        auto res = constructPairs(seq);
        std::vector<std::pair<int,int>> expected = {{0,0}, {1,1}, {2,2}};
        assert(res == expected);
    }
    // Test 7: longer valid sequence
    {
        std::vector<int> seq = {1, 1, 2, 3};
        // right-to-left:
        // index3 (3): threes empty, ones has {2,1}? Actually ones stack has indices 0,1.
        // Wait ones push indices scanning from right: at i=3 (3): threes empty, ones has indexes? scan from right:
        // i=3 is 3: threes empty, ones empty? No, ones gets filled from right to left? Actually we scan leftwards, so at i=3, we haven't seen index2 yet. We only have seen index3 itself. So ones empty -> fail? Let's check original code's behavior: a = {1,1,2,3}. n=4. Scan ri=3: a[3]=3, l2 empty, l3 empty -> fail. So return empty.
        assert(constructPairs(seq).empty());
    }
    // Test 8: valid with 3 pairing with another 3
    {
        std::vector<int> seq = {3, 1, 3};
        // scanning right: i=2 (3): threes empty, ones empty? actually rightmost is index2, no ones seen yet -> fail.
        assert(constructPairs(seq).empty());
    }
    // Test 9: a valid configuration
    {
        std::vector<int> seq = {1, 3, 2};
        // right-to-left:
        // i=2 (2): ones has? we haven't seen index0 yet because we scan from right. At i=2, we have not seen any ones to the right, because index0 is left. So ones empty -> fail.
        assert(constructPairs(seq).empty());
    }
    // Test 10: known valid example from original snippet (e.g., 1 2 1 3)
    {
        std::vector<int> seq = {1, 2, 1, 3};
        // scan right:
        // i=3 (3): threes empty, ones empty? Actually we have not seen index2 yet because it's left. So ones empty -> fail.
        assert(constructPairs(seq).empty());
        // Wait, original code for {1,2,1,3}: n=4. ri=3 (3): l2 empty, l3 empty -> fail. Yes.
    }
    // Provide a positive test: {1,2,1} works? scan right: i=2 (1) push to l2; i=1 (2) pop l2 -> pair (1,2); i=0 (1) push. So output pairs: for i=0 (1): (0,0); i=1 (2): (1,1) and (1,2); i=2 (1): (2,2). So expected: (0,0),(1,1),(1,2),(2,2). Sorted by row: (0,0),(1,1),(1,2),(2,2). Test that.
    {
        std::vector<int> seq = {1, 2, 1};
        auto res = constructPairs(seq);
        std::vector<std::pair<int,int>> expected = {{0,0}, {1,1}, {1,2}, {2,2}};
        assert(res == expected);
    }
    return 0;
}
