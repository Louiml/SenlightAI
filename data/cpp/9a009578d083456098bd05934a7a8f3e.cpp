// Write a C++ function `int simulateQueueGame(int n, const std::vector<int>& initialValues, const std::vector<char>& operations)` that models the following process. You are given `n` items labeled `1` through `n`, each with an initial score provided in `initialValues` (where index `i` corresponds to item `i+1`). Two priority queues are maintained: one that always pops the item with the **smallest** score first (min-queue), and one that always pops the item with the **largest** score first (max-queue). Initially, all items are pushed into **both** queues. Then, for each character in `operations` (exactly `2*n` operations):
// - If the character is `'0'` (pop-small): repeatedly pop from the max-queue until you find an item that has not been “taken” yet (i.e., its removal count is 0). Take that item, meaning its removal count becomes 1, and immediately push that same item back into the min-queue (with its original score) so it can be popped later by a `'1'` operation, and output/collect the item’s ID.
// - If the character is `'1'` (pop-large): repeatedly pop from the min-queue until you find an item that has a removal count of exactly 1 (i.e., it was previously taken by a `'0'` operation). Take that item (removal count becomes 2), pop it permanently, and output/collect the item’s ID.
// The function must return a vector of integers containing the IDs in the order they were output. You can assume the operations are always valid (i.e., enough items exist in the correct state). Ensure your implementation handles duplicate scores correctly—the priority queues order by score first, but if scores are equal, any tie-breaking is acceptable as long as the simulation remains consistent with the queue semantics as described.

The core idea is to simulate the two-heap system described. We maintain two priority queues storing `(score, id)` pairs: a max-heap (`qMax`) and a min-heap (`qMin`). We also maintain an array `state` of size `n+1` (1-indexed) where `0` = never taken, `1` = taken once (available for `'1'` operation), `2` = taken twice (finished). Initially, push all items into both heaps and set `state[i]=0` for all. For each operation:
- For `'0'`: Pop from `qMax`. If the popped item’s `state[id] == 0`, then set `state[id]=1`, push it into `qMin` (with the same score), and record its id. If `state[id]` is not `0`, discard that popped entry and continue popping until a valid one is found.
- For `'1'`: Pop from `qMin`. If `state[id] == 1`, set `state[id]=2`, record its id, and do not push it anywhere (it is permanently removed). If `state[id]` is not `1`, discard and continue popping.
Because each item is pushed exactly once into `qMax` and at most once into `qMin` (during the `'0'` operation), the total number of heap operations across all `2n` steps is `O(n log n)` in the worst case (each pop from a heap is amortized constant due to popping invalid entries, and each item is pushed a constant number of times). Space complexity is `O(n)` for the heaps, state array, and output. Edge cases: duplicate scores—the heaps order by score, but the id tie-break is arbitrary; since we always check `state[id]` after popping, correctness holds regardless of tie-breaking. Also, because operations are guaranteed valid, we never reach an empty heap when trying to find a valid item for the required state.

#include <vector>
#include <queue>
#include <utility>
#include <functional>

// Simulate the two-priority-queue game and return the sequence of taken IDs.
std::vector<int> simulateQueueGame(int n,
                                   const std::vector<int>& initialValues,
                                   const std::vector<char>& operations) {
    // state[id] = 0 (never taken), 1 (taken once, ready for '1'), 2 (fully taken)
    std::vector<int> state(n + 1, 0);

    // Max-heap for '0' operations: largest score first.
    std::priority_queue<std::pair<int, int>> qMax;
    // Min-heap for '1' operations: smallest score first.
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>,
                        std::greater<std::pair<int, int>>> qMin;

    // Push all items into both heaps. initialValues has size n, index i -> item i+1.
    for (int i = 0; i < n; ++i) {
        int score = initialValues[i];
        int id = i + 1;
        qMax.push({score, id});
        qMin.push({score, id});
    }

    std::vector<int> result;
    result.reserve(operations.size());

    for (char op : operations) {
        if (op == '0') {
            // Pop from max-heap until we find an item with state 0.
            while (!qMax.empty()) {
                auto [score, id] = qMax.top();
                qMax.pop();
                if (state[id] == 0) {
                    state[id] = 1;
                    // Push into min-heap so it can be taken by a '1' operation later.
                    qMin.push({score, id});
                    result.push_back(id);
                    break;
                }
                // else: invalid entry, discard and continue.
            }
        } else { // op == '1'
            // Pop from min-heap until we find an item with state 1.
            while (!qMin.empty()) {
                auto [score, id] = qMin.top();
                qMin.pop();
                if (state[id] == 1) {
                    state[id] = 2;
                    result.push_back(id);
                    break;
                }
                // else: invalid entry, discard and continue.
            }
        }
    }

    return result;
}

#include <cassert>
#include <vector>

// Assume the solution function is defined above.

int main() {
    // Test 1: Simple increasing scores, operations alternate 0 and 1.
    {
        int n = 3;
        std::vector<int> vals = {5, 1, 3};
        std::vector<char> ops = {'0','1','0','1','0','1'};
        // Step 0: '0' -> max is id 1 (score 5) -> take 1
        // Step 1: '1' -> min among {1(score 1), 2(score 1? wait id2=1, id3=3)} -> both have score 1? Actually id2=1, id3=3, id1=5.
        //        min is id2 (score 1) -> take 2
        // Step 2: '0' -> max remaining: id1 (5) or id3 (3) -> take id1 (5)
        // Step 3: '1' -> min remaining among id1(5) and id3(3) -> take id3 (3)
        // Step 4: '0' -> max remaining: id1(5) -> take id1 (5) (it was pushed back after step 2)
        // Step 5: '1' -> min remaining among id1(5) and id3(3)? Wait id3 was taken in step3, id1 taken twice? Let's recompute carefully.
        // Initial: qMax: (5,1),(3,3),(1,2); qMin same.
        // '0': pop (5,1) state0 -> take 1, push (5,1) into qMin. qMax now: (3,3),(1,2). qMin now: (1,2),(3,3),(5,1). result=[1]
        // '1': pop qMin top (1,2) state0? Actually state[2]=0, but op '1' requires state==1, so pop it and discard? Wait state[2]=0, not 1, so discard (1,2) from qMin. Next pop qMin top (3,3) state0 -> discard. Next pop (5,1) state1 -> take 1? But that would be taking id1 again? Actually state[1]=1, so take id1, set state[1]=2. But that's incorrect because we need to take an item that was previously '0'-taken, which is id1. So result=[1,1]? But that'd be odd. Let's rethink: the simulation is deterministic but may produce same id twice? The problem statement says each item can be taken twice: once by '0' and once by '1'. So yes, id1 can appear twice in result. Let's just verify via code.
        std::vector<int> res = simulateQueueGame(n, vals, ops);
        // We'll just check size and that all values are in [1,n] and consistent with state transitions.
        assert(res.size() == ops.size());
        // More precise: after simulation, every id appears exactly twice? Actually not necessarily because some may be taken only once if operations don't pair up? But total operations = 2n, and each '0' marks an item state 1, each '1' marks state 2. So exactly n items are taken twice. So each id 1..n appears exactly twice in res. Check that.
        std::vector<int> count(n+1,0);
        for (int x : res) { assert(x>=1 && x<=n); count[x]++; }
        for (int i=1;i<=n;i++) assert(count[i]==2);
    }

    // Test 2: All equal scores.
    {
        int n = 2;
        std::vector<int> vals = {7, 7};
        std::vector<char> ops = {'0','1','0','1'};
        std::vector<int> res = simulateQueueGame(n, vals, ops);
        std::vector<int> count(n+1,0);
        for (int x : res) count[x]++;
        assert(count[1]==2 && count[2]==2);
    }

    // Test 3: Single item.
    {
        int n = 1;
        std::vector<int> vals = {42};
        std::vector<char> ops = {'0','1'};
        std::vector<int> res = simulateQueueGame(n, vals, ops);
        assert(res.size()==2);
        assert(res[0]==1 && res[1]==1);
    }

    // Test 4: A more complex sequence where '0' and '1' are interleaved but not perfectly paired.
    // For instance, two '0's first then two '1's. That should be valid? Yes, because after two '0's we have two items state=1, then two '1's take them.
    {
        int n = 3;
        std::vector<int> vals = {10, 20, 30};
        std::vector<char> ops = {'0','0','1','1','0','1'};
        std::vector<int> res = simulateQueueGame(n, vals, ops);
        std::vector<int> count(n+1,0);
        for (int x : res) count[x]++;
        for (int i=1;i<=n;i++) assert(count[i]==2);
    }

    // Test 5: Negative scores.
    {
        int n = 2;
        std::vector<int> vals = {-5, -1};
        std::vector<char> ops = {'0','1','0','1'};
        std::vector<int> res = simulateQueueGame(n, vals, ops);
        std::vector<int> count(n+1,0);
        for (int x : res) count[x]++;
        assert(count[1]==2 && count[2]==2);
    }

    return 0;
}
