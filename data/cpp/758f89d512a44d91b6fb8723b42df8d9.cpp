/*
Given a positive integer `n` followed by a sequence of `n` integers where each integer is between 1 and 20, write a C++ function `int countValidPairs(const std::vector<int>& sequence)` that processes pairs of equal numbers that appear more than once in the sequence. The goal is to count how many pairs can be selected such that no two selected pairs overlap in the original sequence and no two selected pairs share the same value. More precisely, for each value `x` that appears at least twice in the sequence, consider every consecutive occurrence of `x` (i.e., each adjacent pair of occurrences of `x` in the original order). A pair is defined by its first occurrence index `i` and its second occurrence index `j`, with `j > i`. A pair is considered "valid" if the interval `[i, j]` does not strictly contain the interval of any previously valid pair of a different value, and it does not strictly get contained by any previously valid pair of a different value. Pairs of the same value are treated as alternatives: for each value, at most one pair may be selected, and once a pair for that value is selected, no other pair of that value can be selected. The algorithm must greedily select pairs in order of increasing distance `(j - i)` and, for equal distance, increasing starting index `i`. A pair is selected if it does not conflict with any already selected pair (i.e., the intervals do not overlap, except that touching at endpoints is allowed). The final answer is twice the number of selected pairs. For example, if the sequence is `[1, 2, 1, 2]`, there are four possible pairs: (1,3) for value 1 with distance 2, (2,4) for value 2 with distance 2, (1,5) is invalid because value 1 only appears twice. Since distances equal, order by `i`: first consider (1,3) for value 1, select it; then (2,4) for value 2, select it; both have intervals [1,3] and [2,4] which overlap (since 2 < 3), so the second is not selected. Only one pair selected, answer is 2. But if the sequence is `[1, 1, 2, 2]`, pairs are (1,2) for value 1 and (3,4) for value 2, both non-overlapping, select both, answer 4. If a value appears more than twice, e.g., `[1, 1, 1]`, the possible pairs are (1,2) and (2,3) for value 1, both with distance 1, order by `i` gives (1,2) selected, then (2,3) is not selected because same value already used, so answer 2. If the sequence is `[1, 2, 1, 2, 1]`, value 1 appears thrice, value 2 twice. Pairs for value 1: (1,3) distance 2, (3,5) distance 2; pairs for value 2: (2,4) distance 2. Order by distance then `i`: (1,3) distance 2 i=1, (2,4) distance 2 i=2, (3,5) distance 2 i=3. Select (1,3) for value 1, then (2,4) for value 2 overlaps with (1,3) because 2 < 3, so skip; then (3,5) for value 1 but same value already used, skip. So one pair selected, answer 2. The function should return the total count (twice the number of selected pairs).
*/
#include <bits/stdc++.h>

// Counts twice the number of non-overlapping "consecutive occurrence" pairs
// from a sequence where each element is in [1, 20].
// Each value can be used at most once in a selected pair.
int countValidPairs(const std::vector<int>& sequence) {
    const int MAX_VAL = 20;
    
    // Pair representation: start index, end index, value
    struct PairData {
        int start, end, value;
        PairData(int s, int e, int v) : start(s), end(e), value(v) {}
    };
    
    // Comparator for min-heap: shorter distance first, then smaller start
    struct Compare {
        bool operator()(const PairData& a, const PairData& b) const {
            int distA = a.end - a.start;
            int distB = b.end - b.start;
            if (distA != distB) return distA > distB;
            return a.start > b.start;
        }
    };
    
    std::priority_queue<PairData, std::vector<PairData>, Compare> pq;
    std::vector<int> last(MAX_VAL + 1, -1);
    std::vector<bool> available(MAX_VAL + 1, false);
    
    int n = static_cast<int>(sequence.size());
    for (int i = 0; i < n; ++i) {
        int x = sequence[i];
        if (last[x] != -1) {
            // Consecutive occurrence pair from last[x] to i
            pq.emplace(last[x], i, x);
        }
        last[x] = i;
        available[x] = true;
    }
    
    // Set of selected intervals, sorted by left endpoint
    std::set<std::pair<int, int>> selected;
    int count = 0;
    
    while (!pq.empty()) {
        PairData cur = pq.top();
        pq.pop();
        
        // Check if value still unused
        if (!available[cur.value]) {
            continue;
        }
        
        int left = cur.start;
        int right = cur.end;
        
        // Find first selected interval with start >= left
        auto it = selected.upper_bound({left, left});
        bool noRightOverlap = (it == selected.end()) || (right < it->first);
        bool noLeftOverlap = (it == selected.begin()) || (std::prev(it)->second < left);
        
        if (noRightOverlap && noLeftOverlap) {
            selected.insert({left, right});
            available[cur.value] = false;
            ++count;
        }
    }
    
    return 2 * count;
}
#include <cassert>
#include <vector>

// Include the solution function here (or link it)

int main() {
    // Example from description
    std::vector<int> seq1 = {1, 2, 1, 2};
    assert(countValidPairs(seq1) == 2);
    
    // Two non-overlapping pairs
    std::vector<int> seq2 = {1, 1, 2, 2};
    assert(countValidPairs(seq2) == 4);
    
    // Value appears three times, only one pair selectable
    std::vector<int> seq3 = {1, 1, 1};
    assert(countValidPairs(seq3) == 2);
    
    // Mixed case from description
    std::vector<int> seq4 = {1, 2, 1, 2, 1};
    assert(countValidPairs(seq4) == 2);
    
    // Single element (no pairs)
    std::vector<int> seq5 = {5};
    assert(countValidPairs(seq5) == 0);
    
    // All distinct (no repeats)
    std::vector<int> seq6 = {1, 2, 3, 4};
    assert(countValidPairs(seq6) == 0);
    
    // Touching intervals allowed
    std::vector<int> seq7 = {1, 1, 2, 2, 3, 3};
    // Pairs: (0,1) val1, (2,3) val2, (4,5) val3 – all non-overlapping
    assert(countValidPairs(seq7) == 6);
    
    // Two values with overlapping intervals, longer distance selected later
    std::vector<int> seq8 = {1, 2, 1, 2, 1, 2};
    // val1 pairs: (0,2) dist2, (2,4) dist2, (4,? only two pairs)
    // Actually positions: 0:1,1:2,2:1,3:2,4:1,5:2
    // val1: (0,2) and (2,4) – val2: (1,3) and (3,5)
    // Process by distance (all dist 2) then start: (0,2) val1 selected, (1,3) val2 overlaps with (0,2) because 3>2? actually (1,3) overlaps (0,2) since 2>1. skip. (2,4) val1 but val1 used, skip. (3,5) val2 overlaps with (0,2) since 3<5 and 5>2? skip. So only one, answer 2.
    assert(countValidPairs(seq8) == 2);
    
    // A case where longer distance avoids overlap with shorter
    std::vector<int> seq9 = {1, 2, 2, 1, 1, 2};
    // val1: positions 0,3,4 -> pairs (0,3) dist3, (3,4) dist1
    // val2: positions 1,2,5 -> pairs (1,2) dist1, (2,5) dist3
    // Process: (3,4) dist1 val1, (1,2) dist1 val2 (start 1 < 3) – first process (1,2) then (3,4) because start 1 < 3? Actually priority by distance then start: both dist1, starts 1 and 3, so (1,2) val2 selected first. Then (3,4) val1 does not overlap with (1,2) because 2 < 3, so selected. Then longer pairs: (0,3) val1 but val1 used, skip; (2,5) val2 overlaps with selected (1,2) because 2<5 and 2<5? Also overlaps with (3,4) because 2<4 and 5>3, skip. So two selected, answer 4.
    assert(countValidPairs(seq9) == 4);
    
    return 0;
}
// The problem is equivalent to a greedy interval scheduling with a constraint that each value can only have one selected interval and intervals from different values must not overlap (with endpoints allowed to touch). The input is a sequence of length `n` where each element is in `1..20`. First, we preprocess the sequence to produce, for each value, all intervals corresponding to consecutive occurrences of that value. That is, we iterate through the sequence and maintain the previous occurrence index for each value. When we see value `x` at position `i`, if there was a previous occurrence at `last[x]`, then we create a pair `(last[x], i, x)` representing the interval from `last[x]` to `i` with distance `i - last[x]`. Then update `last[x] = i`. This gives us all possible pairs exactly once. We store these pairs in a priority queue (min-heap) with ordering by distance, then by start index (as in the original code's comparator). We also maintain a `check[x]` boolean indicating whether value `x` is still available for selection (initially true for all values that appear, and remains true even if only appears once, but such values produce no pairs). We also maintain a set `lr` of selected intervals, stored as pairs `{left, right}`, ordered by left coordinate. To decide whether a candidate interval `[i, j]` can be selected, we need to check three conditions: (1) The value `x` is still available (`check[x]` true). (2) The interval does not overlap with any already selected interval. Because intervals are stored sorted by left coordinate, we find the first selected interval whose left endpoint is >= `i` (using `upper_bound({i,i})`). Let `it` be that iterator. The candidate does not overlap if either `it` is end, or `j < it->first` (so candidate ends before the next interval starts). Also, if `it` is not the first, let `prev(it)` be the previous interval; we need `prev(it)->second < i` (so previous interval ends before candidate starts). If both these conditions hold, and the value is available, then we insert `[i, j]` into the set, mark `check[x] = false`, and count it. Otherwise we ignore. We process all pairs in priority queue order (by distance then start). The final answer is twice the size of `lr` (since each selected pair contributes 2 to output). Edge cases: values that appear only once produce no pairs; multiple consecutive occurrences of the same value yield adjacent intervals; intervals that touch at endpoints are allowed, so condition uses strict `<`. The maximum possible number of pairs is `n - (#distinct values)` but that is not directly useful. Time complexity: generating pairs is O(n). Each pair is processed once, and each processing involves an O(log M) lookup in the set, where M is number of selected intervals, at most n/2. So overall O(n log n). Space O(n) for storing all pairs in priority queue and the set.
