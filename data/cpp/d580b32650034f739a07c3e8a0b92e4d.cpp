// Write a C++ function named `guessNumberGame` that simulates a number-guessing game with a fixed secret number `4`. The game allows at most 5 total attempts, including the initial guess. For each guess, the function returns a string describing the outcome: if the guess equals 4, return `"Congratulations! You Won"`; if the guess is greater than 4 and attempts remain, return `"Sorry! Try again with a Lower Number"`; if the guess is less than 4 and attempts remain, return `"Sorry! Try again with a Greater Number"`. After 5 attempts without a correct guess, return `"You Lost"`. The function must accept an array of integer guesses (already collected externally) and its size, and it should also output the total number of tries used via a reference parameter. The function should handle cases where the correct guess occurs early (then stop reading further guesses) or when the input array is empty (return `"You Lost"` with tries = 0). For extra clarity, the secret number is fixed as a constant inside the function.
The solution processes the sequence of guesses sequentially. The main algorithm is a loop over the guesses, maintaining a counter `tries`. For each guess: increment `tries`, compare it to the secret (4). If equal, return the win message immediately and set the tries reference to the current count. If not equal, check if `tries` has reached 5; if so, break out and return the loss message. Otherwise, produce a hint message (lower or greater) and continue to the next guess. Edge cases: empty input → return `"You Lost"` with tries=0; a correct guess on the last allowed attempt (5th) → win is returned; if there are more than 5 guesses but the correct one appears later, the loop stops at 5 tries and returns loss, ignoring remaining guesses. Time complexity is O(n) where n is the number of guesses provided, but effectively at most 5 iterations because of the attempt cap. Space complexity is O(1) auxiliary, not counting the input array. The tries reference is always updated before any return, matching the count of guesses actually processed.
#include <string>

// Simulates the guessing game with secret number 4.
// Guesses are provided in the array; max 5 tries allowed.
// Returns a message string and sets tries to the number of guesses used.
std::string guessNumberGame(const int guesses[], int size, int& tries) {
    const int secret = 4;
    const int maxTries = 5;
    tries = 0;

    for (int i = 0; i < size && tries < maxTries; ++i) {
        ++tries;
        if (guesses[i] == secret) {
            return "Congratulations! You Won";
        }
        if (tries == maxTries) {
            break;
        }
        // Hint based on comparison
        if (guesses[i] > secret) {
            // Continue loop, but return hint only if not last try (handled above)
            // Actually we return after loop, but we need to return hint immediately.
            // So we return here for non-final incorrect tries.
            tries = tries; // keep as is
            // but we need to return the hint, so we do:
            // We'll handle by returning in the loop after incrementing.
            // Let's restructure: for each guess, if not correct and not last try, return hint.
            // But we already used 'tries' for max check, so:
            if (tries < maxTries) {
                return "Sorry! Try again with a Lower Number";
            }
        } else {
            if (tries < maxTries) {
                return "Sorry! Try again with a Greater Number";
            }
        }
    }
    tries = (tries == 0) ? 0 : tries; // keep tries as count
    return "You Lost";
}
*Note: The above solution is flawed because it returns on the first hint, which is incorrect for a multi-guess simulation. The correct implementation should consume all guesses in the loop, building a result message, but since the task expects a single function call with an array, we must process all guesses internally and return the final outcome, not intermediate hints. I need to correct that.*  
*Corrected solution below:*

#include <string>

// Simulates the number guessing game with secret = 4.
// The function processes the entire sequence of guesses (up to max 5) and
// returns the final outcome message. The 'tries' reference is set to the number
// of guesses actually processed (caps at 5).
std::string guessNumberGame(const int guesses[], int size, int& tries) {
    const int secret = 4;
    const int maxTries = 5;
    tries = 0;

    for (int i = 0; i < size && tries < maxTries; ++i) {
        ++tries;
        if (guesses[i] == secret) {
            return "Congratulations! You Won";
        }
        // If we reach here, guess is wrong.
        // If this was the last allowed attempt, we lose.
        if (tries == maxTries) {
            break;
        }
        // Otherwise continue to next guess; no output needed for intermediate guesses.
    }
    return "You Lost";
}
*That is the correct final solution.*
#include <cassert>

int main() {
    int tries;

    // Correct guess on first try
    int g1[] = {4};
    assert(guessNumberGame(g1, 1, tries) == "Congratulations! You Won");
    assert(tries == 1);

    // Correct guess on third try
    int g2[] = {3, 5, 4};
    assert(guessNumberGame(g2, 3, tries) == "Congratulations! You Won");
    assert(tries == 3);

    // All wrong guesses, 5 tries
    int g3[] = {1, 2, 3, 5, 6};
    assert(guessNumberGame(g3, 5, tries) == "You Lost");
    assert(tries == 5);

    // All wrong guesses but only 3 tries provided (not reaching max)
    int g4[] = {1, 2, 3};
    assert(guessNumberGame(g4, 3, tries) == "You Lost");
    assert(tries == 3);

    // Empty input
    int g5[] = {};
    assert(guessNumberGame(g5, 0, tries) == "You Lost");
    assert(tries == 0);

    // Correct guess on the 5th attempt (last allowed)
    int g6[] = {1, 2, 3, 5, 4};
    assert(guessNumberGame(g6, 5, tries) == "Congratulations! You Won");
    assert(tries == 5);

    // More guesses than allowed, but correct guess appears after 5 tries -> lost
    int g7[] = {1, 2, 3, 5, 6, 4};
    assert(guessNumberGame(g7, 6, tries) == "You Lost");
    assert(tries == 5);

    return 0;
}
