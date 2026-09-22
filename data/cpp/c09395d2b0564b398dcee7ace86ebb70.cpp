// Given a list of 3D boxes where each box is defined by its three side lengths, write a C++ function `bestPlacement` that takes a vector of triples (each triple contains three positive integers representing the dimensions of a box, in arbitrary order) and returns a `pair<int, int>` where the first element is 1 or 2 indicating the best placement strategy:
// - Strategy 1: Place a single box on a table so that its height is the smallest side of that box (i.e., the box stands on its largest face). The "score" is the height = minimum dimension of that box.
// - Strategy 2: Stack two boxes such that their bottom faces match exactly (same two dimensions forming the base). The two boxes are placed one on top of the other, both standing on the same base pair (the two matching dimensions). The total height is the sum of the two heights (the third dimension of each box, perpendicular to the base). The score is the minimum of (the common base side length) and (the total height).
// Choose the strategy that yields the maximum score. If Strategy 1 gives a score greater than Strategy 2, return `{1, boxIndex}` (1-indexed) where `boxIndex` is the index of the chosen box. If Strategy 2 gives a score greater than or equal to Strategy 1, return `{2, firstBoxIndex, secondBoxIndex}` (1-indexed, the two box indices in any consistent order, but the pair should be sorted by index in output). In case of ties in strategy 2, prioritize the pair with the larger sum of heights; if still tied, any valid pair is acceptable. The function should compute the best score and return the appropriate pair.
// For each box with dimensions sorted as `a <= b <= c`, we consider all possible orientations where the box stands on a face. For a given orientation, the base is defined by two of its dimensions and the height is the remaining dimension. Because the box can be rotated, we can generate up to three distinct (base, height) combinations:  
// - base `(a,b)`, height `c`  
// - base `(a,c)`, height `b`  
// - base `(b,c)`, height `a`  
// However, if two dimensions are equal, some of these coincide, and we must avoid duplicates in the map (though duplicate entries won’t cause incorrect results, they increase time; we can skip duplicates for efficiency).  
//
// For Strategy 1, the best single box is simply the one with the largest minimum dimension (i.e., largest `a` after sorting). Track its index.  
//
// For Strategy 2, we group all generated orientations by their base pair (as an ordered pair, e.g., `(min_base, max_base)` to avoid orientation issues). For each base pair, store all orientations that share that base, each with its height and original box index. Then for each group with at least two entries, sort the entries by height descending, and consider the top two. The total height is `h1 + h2`. The score for that pair is `min(common_base_side, h1+h2)` (where common_base_side is the smallest of the two base dimensions, since the stack must fit on the table and the base side length is the limiting horizontal dimension). Track the maximum such score and the two indices.  
//
// Edge cases:  
// - A box with all three sides equal will produce only one orientation; it cannot be stacked with itself in the same group unless there are at least two different boxes with the same base, so duplicate base pairs from the same box must be ignored when selecting two boxes (but the map will store multiple entries per box if we don't skip; we must be careful to not pair a box with itself). To avoid this, when we process a group, we must ensure the two chosen entries come from different boxes. The simplest approach: when adding to a group, we store `(height, index)`. When processing a group, we sort by height descending, but we need the two highest heights that come from distinct indices. Since indices are unique per box, we can iterate through the sorted list, pick the first, then scan for the next with a different index. This is O(k) per group after sorting.  
// - If there is no valid pair for Strategy 2, ans2 remains 0 and we output Strategy 1.  
// - Ties: If ans1 >= ans2, output Strategy 1; otherwise Strategy 2.  
//
// Time complexity: For each of `n` boxes, we generate up to 3 orientations, so the map has O(3n) entries. Sorting per group: total sorting cost is O(3n log(3n)) in the worst case (or O(3n log k) per group). Overall O(n log n). Space: O(n).
#include <bits/stdc++.h>
using namespace std;

// Represents a box orientation: base pair (base1, base2) and height.
struct Orientation {
    int base1;
    int base2;
    int height;
    int boxIndex;
};

// Represents the result: strategy (1 or 2) and the chosen box indices (1-indexed).
struct PlacementResult {
    int strategy;
    vector<int> boxIndices;
};

PlacementResult bestPlacement(const vector<array<int,3>>& boxes) {
    int n = (int)boxes.size();
    if (n == 0) return {1, {}};

    // Strategy 1: best single box (maximum minimum dimension).
    int bestSingleScore = 0;
    int bestSingleIndex = -1;
    for (int i = 0; i < n; ++i) {
        array<int,3> v = boxes[i];
        sort(v.begin(), v.end());
        if (v[0] > bestSingleScore) {
            bestSingleScore = v[0];
            bestSingleIndex = i + 1; // 1-indexed
        }
    }

    // Build map from ordered base pair to list of (height, index).
    map<pair<int,int>, vector<pair<int,int>>> baseMap;
    for (int i = 0; i < n; ++i) {
        array<int,3> v = boxes[i];
        sort(v.begin(), v.end());
        // Orientation 1: base (v[0], v[1]), height v[2]
        baseMap[make_pair(v[0], v[1])].push_back(make_pair(v[2], i + 1));
        // Orientation 2: base (v[0], v[2]), height v[1], only if distinct from previous
        if (v[1] != v[2]) {
            baseMap[make_pair(v[0], v[2])].push_back(make_pair(v[1], i + 1));
        }
        // Orientation 3: base (v[1], v[2]), height v[0], only if distinct from previous two
        if (v[0] != v[1] && v[0] != v[2]) {
            baseMap[make_pair(v[1], v[2])].push_back(make_pair(v[0], i + 1));
        }
    }

    // Strategy 2: best pair with same base.
    int bestDoubleScore = 0;
    pair<int,int> bestPair = make_pair(-1, -1);
    for (const auto& entry : baseMap) {
        const auto& base = entry.first;
        const auto& orientations = entry.second;
        if (orientations.size() < 2) continue;

        // Sort by height descending.
        vector<pair<int,int>> sorted = orientations;
        sort(sorted.begin(), sorted.end(), greater<pair<int,int>>());

        // Find the two distinct box indices with the largest heights.
        int firstHeight = sorted[0].first;
        int firstIndex = sorted[0].second;
        int secondHeight = -1;
        int secondIndex = -1;
        for (size_t j = 0; j < sorted.size(); ++j) {
            if (sorted[j].second != firstIndex) {
                secondHeight = sorted[j].first;
                secondIndex = sorted[j].second;
                break;
            }
        }
        if (secondIndex == -1) continue; // no distinct second box

        int commonBaseSide = min(base.first, base.second);
        int totalHeight = firstHeight + secondHeight;
        int score = min(commonBaseSide, totalHeight);
        if (score > bestDoubleScore) {
            bestDoubleScore = score;
            // Ensure indices are in increasing order for output.
            int a = min(firstIndex, secondIndex);
            int b = max(firstIndex, secondIndex);
            bestPair = make_pair(a, b);
        }
    }

    // Compare strategies.
    if (bestSingleScore > bestDoubleScore) {
        return {1, {bestSingleIndex}};
    } else {
        return {2, {bestPair.first, bestPair.second}};
    }
}
#include <bits/stdc++.h>
using namespace std;

// Include the solution function declaration here (or copy the code above).
// For the test, we assume the function is defined above.

int main() {
    // Test 1: Single box, simplest case.
    {
        vector<array<int,3>> boxes = {{1,2,3}};
        auto res = bestPlacement(boxes);
        assert(res.strategy == 1);
        assert(res.boxIndices.size() == 1 && res.boxIndices[0] == 1);
    }

    // Test 2: Two identical boxes, should stack.
    {
        vector<array<int,3>> boxes = {{2,2,2}, {2,2,2}};
        auto res = bestPlacement(boxes);
        assert(res.strategy == 2);
        assert(res.boxIndices.size() == 2);
        assert(res.boxIndices[0] == 1 && res.boxIndices[1] == 2);
    }

    // Test 3: Two different boxes where stacking is better.
    {
        vector<array<int,3>> boxes = {{5,5,1}, {5,5,1}};
        auto res = bestPlacement(boxes);
        assert(res.strategy == 2);
        assert(res.boxIndices[0] == 1 && res.boxIndices[1] == 2);
    }

    // Test 4: Single box with large min side beats any stack.
    {
        vector<array<int,3>> boxes = {{10,10,1}, {1,1,1}};
        // Box 1 min side = 1, box 2 min side = 1, stack base (1,1) heights 10 and 1 => score min(1,11)=1
        // Single best: box1 min=1, box2 min=1 => score 1, but single score is 1, stack score 1 -> tie -> strategy 2 (since >=)
        // Actually tie: both 1, strategy 2 output.
        auto res = bestPlacement(boxes);
        // Since ans1 = 1, ans2 = 1, we choose 2.
        assert(res.strategy == 2);
    }

    // Test 5: Boxes with no common base pair, single best wins.
    {
        vector<array<int,3>> boxes = {{1,2,3}, {4,5,6}};
        // No common base, ans2 = 0, ans1 = 1 (from first box min=1) -> strategy 1.
        auto res = bestPlacement(boxes);
        assert(res.strategy == 1);
        assert(res.boxIndices[0] == 1);
    }

    // Test 6: More than two boxes in same base group, pick largest two heights.
    {
        vector<array<int,3>> boxes = {{1,1,10}, {1,1,20}, {1,1,30}};
        // Base (1,1) heights: 10,20,30. Top two: 30 and 20 => total 50, score min(1,50)=1
        // Single best: min side = 1 (all have min=1) => ans1=1, ans2=1 -> strategy 2.
        auto res = bestPlacement(boxes);
        assert(res.strategy == 2);
        // The two indices should be 2 and 3 (heights 20 and 30).
        assert(res.boxIndices.size() == 2);
        assert(res.boxIndices[0] == 2 && res.boxIndices[1] == 3);
    }

    // Test 7: Box with duplicate dimensions, ensure self not paired with itself.
    {
        vector<array<int,3>> boxes = {{2,2,5}, {2,2,1}};
        // Box 1 orientations: base(2,2) height5, base(2,2) height5 (dup), base(2,2) height2 (dup) -> only one unique orientation (2,2,5)
        // Box 2: base(2,2) height1
        // Group (2,2): heights 5 and 1, total 6, score min(2,6)=2
        // Single best: min of box1 = 2, min of box2=1 -> ans1=2, ans2=2 -> tie -> strategy 2.
        auto res = bestPlacement(boxes);
        assert(res.strategy == 2);
        assert(res.boxIndices.size() == 2);
        assert(res.boxIndices[0] == 1 && res.boxIndices[1] == 2);
    }

    // Test 8: Large values and many boxes, sanity.
    {
        vector<array<int,3>> boxes;
        for (int i = 1; i <= 100; ++i) {
            boxes.push_back({i, i, i+1});
        }
        // Best single: box 100 has min=100, max height? Actually min=100.
        // Best stack: need common base. Box 99: dims sorted (99,99,100) -> base(99,99) height100
        // Box 100: dims sorted (100,100,101) -> no common base with 99.
        // Actually each box has unique base (i,i) so only two boxes with same base? No, box i and box i+1? Box i: (i,i,i+1) -> base(i,i), box i+1: (i+1,i+1,i+2) -> base(i+1,i+1) no match.
        // So ans2=0, ans1=100 -> strategy 1, index 100.
        auto res = bestPlacement(boxes);
        assert(res.strategy == 1);
        assert(res.boxIndices[0] == 100);
    }

    // Test 9: Edge with zero boxes (should not happen but handle).
    {
        vector<array<int,3>> boxes;
        auto res = bestPlacement(boxes);
        assert(res.strategy == 1);
        assert(res.boxIndices.empty());
    }

    // Test 10: Random test with known outcome.
    {
        vector<array<int,3>> boxes = {{3,4,5}, {3,4,6}, {1,1,10}};
        // Box1: min=3, base(3,4) height5, base(3,5) height4, base(4,5) height3
        // Box2: min=3, base(3,4) height6, base(3,6) height4, base(4,6) height3
        // Box3: min=1, base(1,1) height10
        // Strategy1 best: min=3 (boxes 1 and 2) -> ans1=3
        // Strategy2: base(3,4) has heights 5 and 6 -> total 11, common side=3 -> score min(3,11)=3
        // ans2=3 -> tie -> strategy 2. Pair indices 1 and 2.
        auto res = bestPlacement(boxes);
        assert(res.strategy == 2);
        assert(res.boxIndices.size() == 2);
        assert(res.boxIndices[0] == 1 && res.boxIndices[1] == 2);
    }

    cout << "All tests passed!" << endl;
    return 0;
}
