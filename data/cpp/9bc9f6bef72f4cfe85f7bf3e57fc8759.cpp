// Implement a function that simulates a simplified peer address selection system. Given a vector of peer addresses (represented as integers), a "last seen time" (as an integer), and a "last try time" (as an integer), write a function that returns the probability chance of selecting that peer, as a double, using the same `GetChance` logic from the code snippet. The function should take a struct representing the peer information (containing `nTime`, `nLastTry`, `nAttempts`, and `nLastSuccess` fields) and the current time `nNow`. The chance calculation is: start with 1.0, multiply by `600.0 / (600.0 + max(0, nNow - nTime))`, multiply by `0.01` if `max(0, nNow - nLastTry) < 600`, and divide by `1.5` for each attempt recorded. Return the resulting double.
The solution requires implementing a single mathematical formula with straightforward conditional logic. First, compute `nSinceLastSeen = max(0, nNow - nTime)` and `nSinceLastTry = max(0, nNow - nLastTry)`. Initialize `fChance = 1.0`. Multiply `fChance` by `600.0 / (600.0 + nSinceLastSeen)`. If `nSinceLastTry < 600` (i.e., 10 minutes in seconds), multiply `fChance` by `0.01`. Finally, loop from 0 to `nAttempts-1`, dividing `fChance` by `1.5` each iteration. The edge case of negative time differences is handled by clamping to zero. Time complexity is O(nAttempts) due to the loop, and space complexity is O(1). The function should be a free function that takes a const reference to the peer struct to avoid copying.
#include <algorithm>

// Struct representing peer address information relevant for selection chance
struct PeerInfo {
    int64_t nTime;         // last time seen (Unix timestamp)
    int64_t nLastTry;      // last time attempted (Unix timestamp)
    int nAttempts;         // number of failed attempts
    int64_t nLastSuccess;  // last successful connection time (not used in chance)
};

// Compute the chance weight for selecting a peer, using logic similar to GetChance
double GetPeerChance(const PeerInfo& info, int64_t nNow) {
    double fChance = 1.0;

    int64_t nSinceLastSeen = nNow - info.nTime;
    int64_t nSinceLastTry = nNow - info.nLastTry;

    if (nSinceLastSeen < 0) nSinceLastSeen = 0;
    if (nSinceLastTry < 0) nSinceLastTry = 0;

    fChance *= 600.0 / (600.0 + nSinceLastSeen);

    // deprioritize very recent attempts away
    if (nSinceLastTry < 60*10)
        fChance *= 0.01;

    // deprioritize 50% after each failed attempt
    for (int n = 0; n < info.nAttempts; n++)
        fChance /= 1.5;

    return fChance;
}
int main() {
    // Test 1: brand new peer, no attempts, seen now
    PeerInfo p1{1000, 1000, 0, 0};
    double c1 = GetPeerChance(p1, 1000);
    assert(c1 == 1.0); // 600/(600+0) = 1, no recent try penalty (0 < 600), no attempts

    // Test 2: peer seen long ago, no attempts
    PeerInfo p2{0, 0, 0, 0};
    double c2 = GetPeerChance(p2, 3600);
    // nSinceLastSeen = 3600, nSinceLastTry = 3600
    // chance = 600/(600+3600) = 0.142857, no try penalty, no attempts
    assert(std::abs(c2 - 0.142857) < 1e-5);

    // Test 3: peer with recent try but old seen time
    PeerInfo p3{100, 3500, 0, 0};
    double c3 = GetPeerChance(p3, 3600);
    // nSinceLastSeen = 3500, nSinceLastTry = 100 (recent, <600) -> multiply by 0.01
    // base = 600/(600+3500) = 0.14634, then *0.01 = 0.0014634
    assert(std::abs(c3 - 0.0014634) < 1e-5);

    // Test 4: peer with attempts
    PeerInfo p4{1000, 1000, 2, 0};
    double c4 = GetPeerChance(p4, 1000);
    // nSinceLastSeen=0 -> factor 1.0, nSinceLastTry=0 -> *0.01, then /1.5 twice
    // 1.0 * 0.01 / 1.5 / 1.5 = 0.0044444
    assert(std::abs(c4 - 0.0044444) < 1e-5);

    // Test 5: negative time differences (should clamp to 0)
    PeerInfo p5{2000, 2000, 0, 0};
    double c5 = GetPeerChance(p5, 1000);
    assert(c5 == 1.0); // both clamped to 0, no penalties

    // Test 6: many attempts, recent try
    PeerInfo p6{1000, 1000, 5, 0};
    double c6 = GetPeerChance(p6, 1000);
    // 1.0 * 0.01 / (1.5^5) = 0.01 / 7.59375 = 0.00131687
    assert(std::abs(c6 - 0.00131687) < 1e-5);

    // Test 7: old seen, no try penalty, some attempts
    PeerInfo p7{0, 0, 3, 0};
    double c7 = GetPeerChance(p7, 1000);
    // nSinceLastSeen=1000 -> 600/1600=0.375, nSinceLastTry=1000 (not <600) no penalty
    // then /1.5^3 = 0.375/3.375 = 0.111111
    assert(std::abs(c7 - 0.111111) < 1e-5);
}
