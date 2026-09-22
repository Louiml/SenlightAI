// Write a C++ function `int maxFrequencyOfRank()` that reads exactly five poker cards from standard input in the format `X Y` (where `X` is the rank and `Y` is the suit, separated by a space, with each card on its own line or separated by whitespace). The rank can be `2`–`9`, `T` (10), `J` (11), `Q` (12), `K` (13), or `A` (14). The function should ignore the suit and count how many times each rank appears among the five cards. It must return the highest frequency among all ranks (i.e., the maximum count for any rank). For example, if the five cards are `A S`, `A H`, `K D`, `2 C`, `A C`, the function should return `3`. Assume the input is always valid and contains exactly five cards.
// The solution reads the rank character from each card, converts it to a numeric value using a mapping, then increments a frequency counter array of size 15 (indexes 2–14, though we can use size 15 to be safe). While incrementing, track the largest count seen so far. Mapping: `'2'`–`'9'` map to their integer value; `'T'`→10, `'J'`→11, `'Q'`→12, `'K'`→13, `'A'`→14. The suit is ignored, so after reading the rank char we need to skip the suit char and any whitespace. Since input is line-based, we can read lines and use `cin.get()` or simpler: read a string line, take first char as rank. But to keep it simple, we can read `char rank; string suit;` for each card in a loop, because `>>` handles whitespace automatically. Edge cases: duplicate ranks, all five same rank (returns 5), all distinct (returns 1). Time complexity is O(5) = constant, space O(1) (fixed array size).
#include <array>
#include <cctype>

// Reads five poker cards from standard input (format: rank suit) and returns
// the maximum frequency of any rank among the five cards.
int maxFrequencyOfRank() {
    std::array<int, 15> freq{}; // indices 2..14
    int maxFreq = 0;

    for (int i = 0; i < 5; ++i) {
        char rank;
        char suit; // suit is unused
        std::cin >> rank >> suit;

        int value;
        if (rank >= '2' && rank <= '9') {
            value = rank - '0';
        } else {
            switch (rank) {
                case 'T': value = 10; break;
                case 'J': value = 11; break;
                case 'Q': value = 12; break;
                case 'K': value = 13; break;
                case 'A': value = 14; break;
                default: value = 0; break; // not possible with valid input
            }
        }

        ++freq[value];
        if (freq[value] > maxFreq) {
            maxFreq = freq[value];
        }
    }

    return maxFreq;
}
#include <cassert>
#include <sstream>

int main() {
    // Redirect std::cin to a stringstream for testing
    std::istringstream input1("A S\nA H\nK D\n2 C\nA C\n");
    std::cin.rdbuf(input1.rdbuf());
    assert(maxFrequencyOfRank() == 3);

    std::istringstream input2("T H\nT D\nT C\nT S\n9 H\n");
    std::cin.rdbuf(input2.rdbuf());
    assert(maxFrequencyOfRank() == 4);

    std::istringstream input3("2 C\n3 D\n4 H\n5 S\n6 C\n");
    std::cin.rdbuf(input3.rdbuf());
    assert(maxFrequencyOfRank() == 1);

    std::istringstream input4("K S\nK H\nK D\nK C\nK S\n");
    std::cin.rdbuf(input4.rdbuf());
    assert(maxFrequencyOfRank() == 5);

    std::istringstream input5("Q H\nQ D\nQ C\nJ S\nJ H\n");
    std::cin.rdbuf(input5.rdbuf());
    assert(maxFrequencyOfRank() == 3);
}
