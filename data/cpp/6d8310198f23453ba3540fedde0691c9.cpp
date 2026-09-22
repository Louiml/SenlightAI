Write a C++ function `double expectedWeightedScore(const std::vector<int>& order, bool useSeededRandom)` that, given a permutation of the numbers `{1,2,3,4}`, computes a score using the following rules, then simulates 1000 random shuffles of the same input set (shuffling a fresh copy each time) and returns the average score. The score for a single shuffle is determined by: first, let `a = order[0]`, `b = order[1]`, `c = order[2]`, `d = order[3]` (indices as given). Compute an integer `s = solve(c, b)` using the helper: if `c == 1` and `b == 2`, `s = 0`; if `c == 1` and `b != 2`, `s = 1`; if `c == 2` and `b == 3`, `s = 1`; if `c == 2` and `b != 3`, `s = 2`; otherwise `s = 2`. Then compute the score as `score = calc(c, b)` where `calc` returns `3` if `s == 0`, `2` if `s == 2`, and `0` if `s == 1`. For each of 1000 repetitions, shuffle the input vector randomly (using `std::shuffle` with a `std::mt19937` seeded deterministically if `useSeededRandom` is true, otherwise with a random device), compute the score for the shuffled vector, and accumulate. Return the average score as a `double`. The function must not print anything and must not modify the original `order` vector.
The problem is straightforward: we need to implement a deterministic scoring function `solve` and `calc` exactly as described, then perform 1000 random permutations of the input set and average the resulting scores. The key points are: (1) We must not modify the caller’s vector, so we copy it. (2) Random shuffling must be done with `std::shuffle` (not the deprecated `random_shuffle`) and we need to control the RNG: if `useSeededRandom` is true, seed `std::mt19937` with a fixed value (e.g., 42) so the result is deterministic and testable; otherwise use `std::random_device` for true randomness. (3) The scoring function depends only on two elements at indices 2 and 1 of the shuffled vector. The logic: `solve(c,b)` returns one of {0,1,2}, and `calc` maps 0→3, 2→2, 1→0. We just implement these as small helper functions inside the solution function or as separate static functions. Edge cases: the input is always a permutation of {1,2,3,4}, but we should still handle any vector of at least 3 elements (ignoring index 3 completely because scoring only uses indices 1 and 2). If the vector has fewer than 3 elements, behavior is undefined—but the problem guarantees a permutation of 4 numbers. Time complexity: O(1000 * N) where N is the size of the vector (here N=4), so effectively constant, and O(N) auxiliary space for the copy. Space complexity is O(N) for the shuffled copy.
#include <vector>
#include <random>
#include <algorithm>
#include <cstdint>

namespace {

// Helper: compute the base value s for a pair (x,y).
// Matches the reference logic exactly.
int computeS(int x, int y) {
    if (x == 1) {
        return (y == 2) ? 0 : 1;
    } else if (x == 2) {
        return (y == 3) ? 1 : 2;
    } else {
        return 2;
    }
}

// Helper: convert the base value s into a score.
int convertScore(int s) {
    if (s == 0) return 3;
    if (s == 2) return 2;
    return 0; // s == 1
}

} // namespace

// Given a vector that is a permutation of {1,2,3,4}, return the average
// score over 1000 random shuffles. If useSeededRandom is true, use a fixed
// seed so the result is reproducible.
double expectedWeightedScore(const std::vector<int>& order, bool useSeededRandom) {
    // Copy the input so we never modify the caller's data.
    std::vector<int> shuffled = order;

    std::mt19937 rng;
    if (useSeededRandom) {
        rng.seed(42);
    } else {
        std::random_device rd;
        rng.seed(rd());
    }

    long long total = 0;
    constexpr int TRIALS = 1000;

    for (int i = 0; i < TRIALS; ++i) {
        std::shuffle(shuffled.begin(), shuffled.end(), rng);
        // According to the original code, c = nums[2] and b = nums[1].
        int c = shuffled[2];
        int b = shuffled[1];
        int s = computeS(c, b);
        total += convertScore(s);
    }

    return static_cast<double>(total) / TRIALS;
}
#include <cassert>
#include <vector>

// Declaration of the function under test (should be in the same file).
double expectedWeightedScore(const std::vector<int>& order, bool useSeededRandom);

int main() {
    // The input is always a permutation of {1,2,3,4}.
    std::vector<int> perm = {1, 2, 3, 4};

    // With a fixed seed, the result is deterministic. Compute once and compare.
    double result_seeded1 = expectedWeightedScore(perm, true);
    double result_seeded2 = expectedWeightedScore(perm, true);
    // The two calls with the same seed must give the same value.
    assert(result_seeded1 == result_seeded2);

    // The average cannot be negative or exceed 3 (each trial score is 0,2,3).
    assert(result_seeded1 >= 0.0 && result_seeded1 <= 3.0);

    // Edge case: a different permutation with a different fixed seed also works.
    std::vector<int> perm2 = {4, 3, 2, 1};
    double result_other = expectedWeightedScore(perm2, true);
    assert(result_other >= 0.0 && result_other <= 3.0);

    // Check that the original vectors are unchanged.
    assert((perm == std::vector<int>{1, 2, 3, 4}));
    assert((perm2 == std::vector<int>{4, 3, 2, 1}));

    // Test the underlying helper logic indirectly: if we brute-force all
    // 24 permutations of {1,2,3,4}, the average over all permutations
    // (deterministic, no shuffling) should be computable manually.
    // Compute the exact average by enumerating all permutations and using
    // the same scoring rule but without shuffling (i.e., one trial per permutation).
    // This is not the same as the function's random simulation, but we can
    // verify the scoring function itself by a separate brute-force.
    // We'll just ensure the function returns a value consistent with the
    // possible set of averages (must be a multiple of 0.001).
    double scaled = result_seeded1 * 1000.0;
    double int_part;
    double frac = std::modf(scaled, &int_part);
    assert(frac < 1e-9 || frac > 1.0 - 1e-9);

    return 0;
}
