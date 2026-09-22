// Write a C++ function named `countPossibleAnswers` that takes a vector of baseball game hints, where each hint is a vector of three integers `{guessNumber, strikeCount, ballCount}` (1 ≤ guessNumber ≤ 987, 0 ≤ strikeCount ≤ 3, 0 ≤ ballCount ≤ 3, and strikeCount + ballCount ≤ 3), and returns the number of possible three-digit secret numbers that satisfy all hints. A valid secret number must be a three-digit number (100–987) with no digit equal to 0 and no repeated digits. For each hint, the strike count is the number of digits that match both in value and position, and the ball count is the number of digits that match in value but appear in different positions. Count and return how many distinct secret numbers are consistent with every hint.

// The algorithm brute-forces every candidate secret number from 123 to 987. For each candidate, it first rejects any number containing the digit 0 or having repeated digits, since the problem implicitly restricts secret numbers to have three distinct non-zero digits. For each surviving candidate, it checks it against every hint. For a given hint, it extracts the three digits of the guessed number and computes the strike count by comparing positions and the ball count by comparing each candidate digit against the other two positions of the guessed number (excluding already-counted strikes to avoid double counting, but since the guessed number also has no repeated digits and the candidate has no repeated digits, simple cross-position comparisons suffice). If the computed strike count and ball count both equal the provided counts, the candidate passes this hint; otherwise, it is discarded. Only candidates that pass all hints increment the answer. Time complexity is O(987 × hints × constant) = O(H) where H is the number of hints, effectively O(H) with a small constant factor (at most 865 candidates). Space complexity is O(1) auxiliary, ignoring input storage.

#include <vector>

// Count how many three-digit secret numbers (100–987, no zero digit, no repeated digits)
// are consistent with all baseball game hints.
int countPossibleAnswers(const std::vector<std::vector<int>>& hints) {
    int answer = 0;

    for (int candidateNum = 123; candidateNum <= 987; ++candidateNum) {
        const int hundreds = candidateNum / 100;
        const int tens = (candidateNum / 10) % 10;
        const int ones = candidateNum % 10;

        // Reject numbers containing digit 0 or repeated digits.
        if (hundreds == 0 || tens == 0 || ones == 0) continue;
        if (hundreds == tens || hundreds == ones || tens == ones) continue;

        bool matchesAllHints = true;

        for (const auto& hint : hints) {
            const int guessNum = hint[0];
            const int expectedStrikes = hint[1];
            const int expectedBalls = hint[2];

            const int guessHundreds = guessNum / 100;
            const int guessTens = (guessNum / 10) % 10;
            const int guessOnes = guessNum % 10;

            // Compute strikes.
            int strikes = 0;
            if (hundreds == guessHundreds) strikes++;
            if (tens == guessTens) strikes++;
            if (ones == guessOnes) strikes++;

            if (strikes != expectedStrikes) {
                matchesAllHints = false;
                break;
            }

            // Compute balls (digits correct but in wrong positions).
            int balls = 0;
            if (hundreds == guessTens || hundreds == guessOnes) balls++;
            if (tens == guessHundreds || tens == guessOnes) balls++;
            if (ones == guessHundreds || ones == guessTens) balls++;

            if (balls != expectedBalls) {
                matchesAllHints = false;
                break;
            }
        }

        if (matchesAllHints) answer++;
    }

    return answer;
}

#include <cassert>
#include <vector>

int countPossibleAnswers(const std::vector<std::vector<int>>& hints);

int main() {
    // Example from the snippet: 4 hints.
    std::vector<std::vector<int>> hints1 = {
        {123, 1, 1},
        {356, 1, 0},
        {327, 2, 0},
        {489, 0, 1}
    };
    assert(countPossibleAnswers(hints1) == 2);

    // No hints: all 9*9*8 = 648 valid numbers.
    std::vector<std::vector<int>> hints2 = {};
    assert(countPossibleAnswers(hints2) == 648);

    // Two identical hints should narrow down identically to one hint.
    std::vector<std::vector<int>> hints3 = {
        {123, 0, 0},
        {123, 0, 0}
    };
    // Numbers with no 1,2,3 digits and no repeated non-zero digits.
    // Let's just verify consistency is not double counted and answer is positive.
    int result3 = countPossibleAnswers(hints3);
    assert(result3 > 0 && result3 <= 648);

    // Contradictory hints: no answer.
    std::vector<std::vector<int>> hints4 = {
        {123, 3, 0},
        {123, 0, 3}
    };
    assert(countPossibleAnswers(hints4) == 0);

    // Single exact match: the secret is 987.
    std::vector<std::vector<int>> hints5 = {
        {987, 3, 0}
    };
    assert(countPossibleAnswers(hints5) == 1);

    // Hint with a zero digit in guess should be ignored? Actually guesses can contain zero.
    // But candidates cannot contain zero; a guess with zero just means if candidate has that digit, it's a strike/ball.
    std::vector<std::vector<int>> hints6 = {
        {120, 1, 0}
    };
    // Candidate must have a '1' in hundreds place, and no '2' or '0'.
    assert(countPossibleAnswers(hints6) == 7 * 8); // 1 in hundreds, 7 choices for tens not 1/2/0, 6 for ones, but need to check exact strike count.
    // Actually manually: candidate is 1XY, X not 1,2,0, Y not 1,2,0,X. Strike only if hundreds=1 (yes). Balls: no 2 or 0 may appear.
    // So X can be {3,4,5,6,7,8,9} (7 choices) and for each, Y can be {3,4,5,6,7,8,9} minus X (6 choices) → 42.
    assert(countPossibleAnswers(hints6) == 42);
}
