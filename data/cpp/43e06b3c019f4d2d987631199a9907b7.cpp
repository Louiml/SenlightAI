// Write a C++ function that simulates a circular elimination game. Given a positive integer `n` and a vector of `n` positive integers `a`, the game starts with all indices `0` to `n-1` arranged in a circle. A pointer `x = 0` is active. In each round, we look at the next active index in clockwise order after `x` (if `x` is not active, we first find the next active index after `x`; if none, wrap around). Let this index be `i`. Let `j` be the predecessor of `i` in the current active circle (the active index immediately counter-clockwise). If `gcd(a[i], a[j]) == 1`, then index `i` is eliminated (removed from the circle) and we append `i+1` to the list of eliminated indices. The pointer moves to the next active index after `i` (clockwise) for the next round. If the gcd is not 1, index `i` is not eliminated, but we move the pointer `x` to `i` and continue. The process repeats until no active indices can be eliminated in a full cycle. The function should return a vector of integers: the first element is the total count of eliminated indices, followed by the eliminated indices (1-based) in the order they were removed. If no elimination occurs, the vector should contain just `{0}`.

The game is a classic cyclic elimination with a pointer that moves only when a non-elimination occurs. Use a `set<int>` to store active indices, and a `set<int>` `con` to store "candidate" indices that need to be checked. Initially all indices are both active and candidates. The pointer `x` starts at 0. In each iteration, we pick the next candidate `i` after `x` (circularly). If `i` and its predecessor `pre(i)` in the active set have gcd 1, then `i` is eliminated: remove from `s` and `con`, record `i+1`, and the pointer moves to `nxt(i)` (the next active after removed index), and insert that next index as a new candidate. If gcd is not 1, we remove `i` from `con` and set `x = i` (so that next candidate is after this index). The loop ends when `con` becomes empty. This works because only indices that are eliminated can affect their neighbors, so we only need to re-check the successor after a removal. Each index is removed at most once, and each time we insert a successor, the total operations are O(n log n). Edge case: when only one index remains, the loop ends because no new candidates are inserted after elimination, and `con` becomes empty. Time complexity: O(n log n) due to set operations. Space: O(n).

#include <bits/stdc++.h>
using namespace std;

// Simulates the elimination game and returns {count, eliminated indices in order}
vector<int> eliminationGame(int n, const vector<int>& a) {
    set<int> active;      // currently active indices (0-based)
    set<int> candidates;  // indices to check next
    vector<int> result;
    result.push_back(0);  // placeholder for count

    for (int i = 0; i < n; ++i) {
        active.insert(i);
        candidates.insert(i);
    }

    auto nextActive = [&](int i) -> int {
        auto it = active.upper_bound(i);
        if (it == active.end()) it = active.begin();
        return *it;
    };

    auto prevActive = [&](int i) -> int {
        auto it = active.lower_bound(i);
        if (it == active.begin()) it = active.end();
        --it;
        return *it;
    };

    int x = 0;  // current pointer
    while (!candidates.empty()) {
        auto it = candidates.upper_bound(x);
        if (it == candidates.end()) it = candidates.begin();
        int i = *it;

        int j = prevActive(i);
        if (std::gcd(a[i], a[j]) == 1) {
            // eliminate i
            candidates.erase(i);
            active.erase(i);
            result.push_back(i + 1);
            if (!active.empty()) {
                int nxt = nextActive(i);
                x = nxt;
                candidates.insert(nxt);
            }
        } else {
            candidates.erase(i);
            x = i;
        }
    }

    result[0] = result.size() - 1;  // count = number of eliminations
    return result;
}

#include <bits/stdc++.h>
using namespace std;

// include the solution function here or paste above

int main() {
    // Test 1: Example from typical usage: n=5, a={1,2,3,4,5}
    // Simulation: start x=0. Next candidate is 1 (since after 0). pre(1)=0, gcd(2,1)=1 -> remove 1, next=2. candidate 2, pre(2)=0, gcd(3,1)=1 -> remove 2, next=3. candidate 3, pre(3)=0, gcd(4,1)=1 -> remove 3, next=4. candidate 4, pre(4)=0, gcd(5,1)=1 -> remove 4, next=0. candidate 0, pre(0)=0? active={0}, pre(0)=0, gcd(1,1)=1 -> remove 0. result count=5, order 1..5.
    vector<int> v1 = {1,2,3,4,5};
    vector<int> r1 = eliminationGame(5, v1);
    vector<int> e1 = {5,1,2,3,4,5};
    assert(r1 == e1);

    // Test 2: All same numbers, gcd(i,i) != 1 unless value 1. Use all 2s -> gcd(2,2)=2 never eliminate.
    vector<int> v2 = {2,2,2};
    vector<int> r2 = eliminationGame(3, v2);
    assert(r2 == vector<int>{0});

    // Test 3: Single element with value 1: no predecessor? Actually pre(0)=0, gcd(1,1)=1 -> remove 0
    vector<int> v3 = {1};
    vector<int> r3 = eliminationGame(1, v3);
    vector<int> e3 = {1,1};
    assert(r3 == e3);

    // Test 4: Single element value 2: gcd(2,2)=2 -> no elimination
    vector<int> v4 = {2};
    vector<int> r4 = eliminationGame(1, v4);
    assert(r4 == vector<int>{0});

    // Test 5: n=4, a={2,3,4,5}
    // Start x=0. next candidate 1, pre(1)=0, gcd(3,2)=1 -> remove1, next=2. candidate2, pre(2)=0, gcd(4,2)=2 not 1 -> x=2. next candidate 3, pre(3)=2, gcd(5,4)=1 -> remove3, next=0. candidate0, pre(0)=2, gcd(2,4)=2 -> x=0. next candidate 2 (since 0,1 removed, 2 active), pre(2)=0, gcd(4,2)=2 -> x=2. no more candidates -> done. removed {2,4} count=2.
    vector<int> v5 = {2,3,4,5};
    vector<int> r5 = eliminationGame(4, v5);
    vector<int> e5 = {2,2,4};
    assert(r5 == e5);

    // Test 6: n=6, a={1,6,1,6,1,6}
    // Expected: remove indices 1,3,5, then 0,2,4? Let's reason: Initially all candidates. x=0. next 1, pre(0)=0, gcd(6,1)=1 -> remove1, next2. candidate2, pre(2)=0, gcd(1,1)=1 -> remove2, next3. candidate3, pre(3)=0, gcd(6,1)=1 -> remove3, next4. candidate4, pre(4)=0, gcd(1,1)=1 -> remove4, next5. candidate5, pre(5)=0, gcd(6,1)=1 -> remove5, next0. candidate0, active={0}, pre(0)=0, gcd(1,1)=1 -> remove0. result count=6 order 2,3,4,5,6,1? Actually order is: 2,3,4,5,6,1? Let's compute: remove1->push2, remove2->push3, remove3->push4, remove4->push5, remove5->push6, remove0->push1. So {6,2,3,4,5,6,1}.
    vector<int> v6 = {1,6,1,6,1,6};
    vector<int> r6 = eliminationGame(6, v6);
    vector<int> e6 = {6,2,3,4,5,6,1};
    assert(r6 == e6);

    return 0;
}
