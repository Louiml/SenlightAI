Write a C++ function named `minimumRollsForTripleDouble` that simulates rolling three six-sided dice. The function should repeatedly roll all three dice until the same value appears on all three dice in two consecutive rounds. It must return the total number of individual die rolls performed (i.e., the number of rounds multiplied by 3). The function should use a pseudo-random number generator seeded with the current time. Each die roll must produce an integer between 1 and 6 inclusive. The function must not take any input parameters and must return an `unsigned int` (or `int`) representing the total rolls. The simulation must continue indefinitely until the condition is met, so there is no maximum number of attempts; however, you may assume the random generator will eventually produce the required sequence.

// The solution uses a loop that continues until two consecutive rounds each have all three dice showing the same face. In each round, three random numbers are generated in `[1,6]`. A counter `consecutiveTriples` tracks how many consecutive rounds satisfied the condition. If a round produces three equal values, the counter increments; otherwise it resets to zero. The total number of individual rolls accumulates by adding 3 after each round. The loop exits when the counter reaches 2. The key edge case: if the first round is a triple, the counter becomes 1, but the loop continues because 2 is needed; if the next round is not a triple, the counter resets. The algorithm runs in `O(rounds)` time, where rounds is the random number of attempts needed (statistically expected around 6^3/1 * 6^3/1? Actually the probability of a triple is 6/216 = 1/36, so the expected number of rounds to get two consecutive triples is around 36^2 + 36 = 1332, so the function is very fast). Space complexity is `O(1)`.

#include <cstdlib>
#include <ctime>

// Simulate rolling three six-sided dice repeatedly until two consecutive rounds
// produce all three dice with the same face value. Return total individual die rolls.
unsigned int minimumRollsForTripleDouble() {
    // Seed the random number generator once per program run
    static bool seeded = false;
    if (!seeded) {
        std::srand(static_cast<unsigned int>(std::time(nullptr)));
        seeded = true;
    }

    unsigned int totalRolls = 0;
    int consecutiveTriples = 0;

    while (consecutiveTriples < 2) {
        int die1 = std::rand() % 6 + 1;
        int die2 = std::rand() % 6 + 1;
        int die3 = std::rand() % 6 + 1;

        totalRolls += 3;

        if (die1 == die2 && die2 == die3) {
            ++consecutiveTriples;
        } else {
            consecutiveTriples = 0;
        }
    }

    return totalRolls;
}

#include <cassert>

// Forward declaration of the solution function
unsigned int minimumRollsForTripleDouble();

int main() {
    // The function returns a positive multiple of 3 (since each round rolls 3 dice)
    unsigned int result1 = minimumRollsForTripleDouble();
    assert(result1 >= 6);  // At least two full rounds are needed

    // Check that the result is divisible by 3
    assert(result1 % 3 == 0);

    // Run multiple times to verify consistency: every result must be a multiple of 3 and at least 6
    for (int i = 0; i < 100; ++i) {
        unsigned int r = minimumRollsForTripleDouble();
        assert(r >= 6);
        assert(r % 3 == 0);
    }

    // Since randomness, no fixed value can be asserted; but we can check the minimum possible:
    // The absolute minimum possible outcome is 6 (two consecutive triple rolls).
    // We cannot force that, but we can at least check the function returns something valid.
    return 0;
}
