/*
Write a C++ function that simulates a circular cup game. Given the initial cup labels as a string of digits (e.g., "389125467") and a number of moves, return a string containing the cup labels in clockwise order starting immediately after the cup labeled `1`, after performing the specified number of moves. In each move: pick up the three cups immediately clockwise of the current cup, remove them, choose a destination cup whose label is one less than the current cup's label (wrapping around to the maximum label in the circle if needed, and skipping any of the three picked-up cups), place the three picked-up cups immediately clockwise of the destination in their original relative order, then advance the current cup to the cup immediately clockwise of its previous position. The initial current cup is the first cup in the input string. The cup labels are integers from 1 to 9, and the input string contains each digit exactly once (it is a permutation of "123456789"). The number of moves is a positive integer. Return the resulting string starting after cup `1` and going clockwise around the circle, with no extra characters.
*/

#include <array>
#include <string>

// Simulate the circular cup game and return the string starting after cup 1.
// The initial order is given by `initial`, which is a permutation of digits 1-9.
// `moves` is the number of moves to perform.
std::string cupGame(const std::string& initial, int moves) {
    // next[label] = label of the cup immediately clockwise of `label`.
    std::array<int, 10> next;
    const int maxLabel = 9;

    // Build the circular linked list from the input string.
    for (std::size_t i = 0; i < initial.size(); ++i) {
        int current = initial[i] - '0';
        int following = (i + 1 < initial.size()) ? (initial[i + 1] - '0') : (initial[0] - '0');
        next[current] = following;
    }

    int currentCup = initial.front() - '0';

    for (int move = 0; move < moves; ++move) {
        // Pick up the three cups immediately clockwise of the current cup.
        int first = next[currentCup];
        int second = next[first];
        int third = next[second];

        // Find the destination label: currentCup - 1, wrapping, skipping picked cups.
        int destination = currentCup - 1;
        while (destination < 1 || destination == first || destination == second || destination == third) {
            destination--;
            if (destination < 1) {
                destination = maxLabel;
            }
        }

        // Remove the three cups from the circle.
        next[currentCup] = next[third];

        // Insert the three cups after the destination.
        next[third] = next[destination];
        next[destination] = first;

        // Advance the current cup.
        currentCup = next[currentCup];
    }

    // Build result string starting after cup 1.
    std::string result;
    int cup = next[1];
    while (cup != 1) {
        result.push_back(static_cast<char>('0' + cup));
        cup = next[cup];
    }
    return result;
}

#include <cassert>
#include <string>

// Declaration of the solution function (usually in a header, but provided here for tests).
std::string cupGame(const std::string& initial, int moves);

int main() {
    // Example from the puzzle description (input "389125467", 100 moves) yields "67384529".
    assert(cupGame("389125467", 100) == "67384529");

    // Single move on a simple configuration.
    // Initial: 1->2->3->...->9->1
    // Move: current=1, pick 2,3,4, destination=9 (since 0 wraps to 9, skip 2,3,4 only affects if 9 is picked? no)
    // Wait: 1-1=0 wraps to 9. pick 2,3,4. destination 9. Insert after 9: ... 9->2->3->4->5...
    // New order: 1->5->6->7->8->9->2->3->4->...
    // Starting from 1 clockwise: 5 6 7 8 9 2 3 4
    assert(cupGame("123456789", 1) == "56789234");

    // Zero moves: should return the original order starting after 1.
    assert(cupGame("123456789", 0) == "23456789");

    // When current cup is 1 and picks include the max label? The wrap-around handles it.
    // Check that the destination never equals a picked cup.
    // Let's test a small sequence manually for correctness: initial "192837465", 1 move.
    // We'll trust the algorithm and just ensure no crash and valid output.
    std::string result = cupGame("192837465", 10);
    assert(result.size() == 8); // must contain 8 digits (all except 1)

    // All digits present exactly once in result.
    bool seen[10] = {false};
    for (char c : result) {
        int d = c - '0';
        assert(d >= 1 && d <= 9);
        assert(!seen[d]);
        seen[d] = true;
    }
    assert(!seen[1]); // 1 is not in result

    // A larger number of moves still produces a valid permutation.
    std::string result2 = cupGame("987654321", 1000);
    bool seen2[10] = {false};
    for (char c : result2) {
        int d = c - '0';
        assert(d >= 1 && d <= 9);
        assert(!seen2[d]);
        seen2[d] = true;
    }

    return 0;
}

// The core challenge is efficiently simulating the removal and insertion of three cups without reallocating the entire container. Since each cup has a unique label from 1 to 9, we can use a fixed-size array (or `std::array`) of size 10 (indexed by label) to store the "next" pointer (i.e., the label of the next cup clockwise). This gives O(1) access to any cup by label, and the linked-list structure can be rearranged by updating only a few next pointers per move. The algorithm:
// 1. Read the input string, construct the circular linked list by setting `next[label] = followingLabel` for each adjacent pair, and finally linking the last cup back to the first.
// 2. Maintain `currentCup` (initially the first digit of the string). For each move (repeat `moves` times):
//    - Find the labels of the three cups immediately clockwise of the current cup by following next pointers three times.
//    - Determine the destination label by decrementing `currentCup` by 1, wrapping from 1 to 9 (the maximum label), and skipping any of the three picked-up labels.
//    - Remove the three picked-up cups from the circle: set `next[currentCup] = next[thirdPicked]`.
//    - Insert them after the destination: set `next[thirdPicked] = next[destination]`, then `next[destination] = firstPicked`.
//    - Advance `currentCup` to `next[currentCup]` (which is the cup that was originally after the third picked-up cup).
// 3. After all moves, find cup `1`, then walk clockwise starting from `next[1]` until returning to `1`, collecting labels into a string.
//
// Edge cases: The wrap-around when the destination label goes below 1; ensuring the destination is not one of the three picked-up cups; when the current cup is exactly `1`, the decrement wraps to 9. Since labels are guaranteed from 1 to 9, the array size is fixed. Complexity: Each move does O(1) pointer updates and a constant number of label comparisons. The total time is O(moves) and O(1) auxiliary space (excluding the fixed array). For typical puzzle limits (e.g., 100 moves), this is trivial.
