// Write a C++ function named `findMaxRandomValue` that simulates the behavior of the provided code snippet but in a more robust and reusable manner. The function should take two parameters: an integer `numIterations` (the number of random numbers to generate, which must be at least 1) and an unsigned integer `seed` (used to initialize the pseudo-random number generator). The function should generate `numIterations` random numbers using `rand()` (after seeding with `srand(seed)`), track the maximum value among those generated numbers, and return that maximum value as an `int`. Additionally, the function should handle the edge case where `numIterations` is less than 1 by returning `rand()` after seeding (i.e., treat it as 1 iteration). Note that the original code loops up to 2,147,000,000 times, which is impractical for testing; your function must work correctly for any positive number of iterations, including small ones. Do not include any output statements or a `main` function in your solution—only the function definition.

#include <cassert>
#include <cstdlib>  // for rand, srand

// The solution function is already declared above; here we test it.
int findMaxRandomValue(int numIterations, unsigned int seed);

int main() {
    // Test 1: Single iteration always returns the first random value for a given seed
    unsigned int seed = 12345;
    std::srand(seed);
    int firstRandom = std::rand();
    assert(findMaxRandomValue(1, seed) == firstRandom);

    // Test 2: Two iterations must return the maximum of the first two randoms
    std::srand(seed);
    int r1 = std::rand();
    int r2 = std::rand();
    int expectedMax = (r1 > r2) ? r1 : r2;
    assert(findMaxRandomValue(2, seed) == expectedMax);

    // Test 3: Zero or negative iterations are treated as 1 iteration
    assert(findMaxRandomValue(0, seed) == firstRandom);
    assert(findMaxRandomValue(-5, seed) == firstRandom);

    // Test 4: With a fixed seed, running multiple times with the same count gives the same max
    assert(findMaxRandomValue(10, seed) == findMaxRandomValue(10, seed));

    // Test 5: Large iteration count (e.g., 1000) with a different seed should produce a max >= any single random
    unsigned int seed2 = 999;
    std::srand(seed2);
    int maxFromSingle = std::rand();
    int maxFromMany = findMaxRandomValue(1000, seed2);
    assert(maxFromMany >= maxFromSingle);

    // Test 6: The maximum of a sequence is non-negative (since rand() returns non-negative)
    int result = findMaxRandomValue(100, 42);
    assert(result >= 0);

    // Test 7: Deterministic behavior—same seed and count always produce same result
    assert(findMaxRandomValue(7, 555) == findMaxRandomValue(7, 555));

    return 0;
}

#include <cstdlib>  // for srand, rand

/**
 * Generate a sequence of random numbers using rand() and return the maximum.
 * @param numIterations Number of random numbers to generate (must be >= 1; treated as 1 if less).
 * @param seed Seed for the pseudo-random number generator.
 * @return The maximum random value found in the sequence.
 */
int findMaxRandomValue(int numIterations, unsigned int seed) {
    // Seed the random number generator
    std::srand(seed);

    // Handle invalid iteration counts by treating as 1 iteration
    if (numIterations < 1) {
        numIterations = 1;
    }

    // Initialize maximum with the first random number
    int maxVal = std::rand();

    // Iterate from the second random number onward
    for (int i = 1; i < numIterations; ++i) {
        int current = std::rand();
        if (current > maxVal) {
            maxVal = current;
        }
    }

    return maxVal;
}

// The solution approach mirrors the original code but abstracts it into a reusable function. The core algorithm is straightforward: seed the random number generator with the provided `seed` using `srand(seed)`, then generate the first random number and set it as the initial maximum. If `numIterations` is greater than 1, iterate from the second iteration up to `numIterations`, generating a new random number each time and updating the maximum if the new number is larger. This is a linear scan over the generated sequence, so the time complexity is \(O(n)\), where \(n\) is `numIterations`. The space complexity is \(O(1)\) because only a few scalar variables are used (the current random number and the maximum). Edge cases to consider: (1) If `numIterations` is 0 or negative, the function should still produce a valid result—treat it as 1 iteration to avoid an empty loop. (2) The function must use `srand` and `rand` from `<cstdlib>` and should include necessary headers. (3) Since `rand()` can return values in the range [0, `RAND_MAX`], the maximum will always be non-negative, but the function returns `int` for compatibility. (4) To ensure deterministic behavior for testing, the seed parameter is essential; the same seed and iteration count always produce the same sequence. Note that `rand()` is not thread-safe, but that is irrelevant for this standalone task. The function should apply `const` correctness to parameters where appropriate (e.g., pass by value for simple types) and avoid global state beyond the standard library.
