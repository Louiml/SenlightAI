// Write a C++ function `longestProbeSequence` that, given a hash table size `m`, a primary hash function `h1(key) = key % m`, a secondary hash function `h2(key) = 1 + (key % (m-1))`, and a vector of non-negative integer keys to insert sequentially using double hashing with open addressing, returns the maximum number of probe steps (including the initial probe) needed to insert any single key. If a key cannot be inserted because the table becomes full during the insertion process, stop inserting immediately and return the maximum number of probes encountered up to that point (not counting the failed insertion). Use an integer array of size `m` initialized to -1 to represent empty slots. Keys are distinct and all less than 10^9. Assume `m >= 2` and `m` is prime for simplicity.
// We simulate the insertion of each key into a hash table of size `m` using double hashing. For each key, we compute the initial probe index `idx = h1(key) = key % m`. If `table[idx] == -1`, we place the key there and record 1 probe. Otherwise, we increment `step` from 1 upward, computing the next index as `(h1(key) + step * h2(key)) % m`, where `h2(key) = 1 + (key % (m-1))`. We continue probing until we find an empty slot or have probed `m` times (meaning the table is full). For each successful insertion, we track the maximum number of probes across all insertions. If at any point a key cannot be placed after `m` probes, we break out of the loop and return the maximum probes recorded from prior successful insertions. Edge cases: if the first key fails (table size 0? but m>=2 so not), or if during an insertion we revisit the starting index before finding an empty slot—this indicates full table. Time complexity: O(n * m) worst-case, where n is the number of keys, but typically O(n) because each insertion is O(1) average. Space complexity: O(m) for the table.
#include <vector>
#include <algorithm>

// Simulates double-hashing insertion and returns the maximum number of probe steps
// needed to successfully insert any key. Stops early if a key cannot be inserted.
int longestProbeSequence(int m, const std::vector<int>& keys) {
    std::vector<int> table(m, -1);  // -1 denotes empty slot
    int maxProbes = 0;

    for (int key : keys) {
        int h1 = key % m;
        int h2 = 1 + (key % (m - 1));
        int probes = 0;
        int idx = h1;
        bool inserted = false;

        for (int step = 0; step < m; ++step) {
            ++probes;
            if (table[idx] == -1) {
                table[idx] = key;
                inserted = true;
                break;
            }
            idx = (h1 + (step + 1) * h2) % m;  // next probe index
        }

        if (!inserted) {
            // Table is full; stop inserting further keys
            break;
        }
        maxProbes = std::max(maxProbes, probes);
    }

    return maxProbes;
}
#include <cassert>
#include <vector>

int main() {
    // Basic case: no collisions, each key takes 1 probe
    assert(longestProbeSequence(5, {0, 1, 2, 3, 4}) == 1);

    // Collisions: keys 0 and 5 both hash to 0 with h2=1
    // Insert 0 (1 probe), insert 5 (probes: 0->0, 1->1) => 2 probes
    assert(longestProbeSequence(5, {0, 5}) == 2);

    // More collisions: 0,5,10 all hash to 0 with h2=1
    // Insert 0 (1), 5 (2 probes), 10 (3 probes) => max 3
    assert(longestProbeSequence(5, {0, 5, 10}) == 3);

    // Double hashing with h2=2: keys 0 and 6 (m=5, h1=0 and 1? Actually 6%5=1, h2=1+(6%4)=3)
    // Let's choose m=7, keys 0 (h1=0, h2=1) and 7 (h1=0, h2=1) then 14 (h1=0, h2=1)
    // Table size 7, insert 0 (1), 7 (probes: 0,1 -> 2), 14 (probes:0,1,2 ->3) => 3
    assert(longestProbeSequence(7, {0, 7, 14}) == 3);

    // Test with secondary hash larger: m=7, key 5 (h1=5, h2=1+(5%6)=6)
    // Insert 5 (1), then 12 (h1=5, h2=1+(12%6)=1) => probe 5 then 6 => 2 probes
    assert(longestProbeSequence(7, {5, 12}) == 2);

    // Table full: m=3, insert keys 0,1,2 (all go in 1 probe each)
    // Next key 3 (h1=0) tries all 3 slots, fails => stop, maxProbes remains 1
    assert(longestProbeSequence(3, {0, 1, 2, 3}) == 1);

    // Table fills after collisions: m=3, insert 0 (1), 3 (h1=0, h2=1 => probes 0 then 1 => 2), 6 (h1=0, probes 0,1,2 => 3) all succeed, max=3.
    // Next key 9 fails, but we don't update max after failure.
    assert(longestProbeSequence(3, {0, 3, 6, 9}) == 3);

    // Edge: single key
    assert(longestProbeSequence(2, {42}) == 1);

    // Edge: m=2, keys 0 (h1=0,h2=1) and 2 (h1=0,h2=1) then 4 (h1=0,h2=1)
    // Insert 0 (1), 2 (probes: 0,1 => 2), then 4 fails because table full => stop, max=2
    assert(longestProbeSequence(2, {0, 2, 4}) == 2);
}
