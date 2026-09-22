Write a C++ function that takes an integer `n` and a vector of `n` pairs of integers `(a, b)`, where each `a` and `b` are non-negative integers in the range `[0, 4]` (inclusive). The function must simulate a simple game: for each pair, if `b` is equal to either `(a+1)%5` or `(a+2)%5` (modulo 5, where `%` is the remainder operator), then "dario" scores a point; otherwise "xerxes" scores a point. After processing all pairs, the function must return a string containing the name of the winner (`"dario"` if dario has strictly more points, `"xerxes"` if xerxes has strictly more points, or `"tie"` if they are equal). The function must handle empty input (n=0) by returning `"tie"` since both have zero points. The input pairs may be given in any order, and values can repeat. Ensure the function uses appropriate `const` correctness and works for any valid input, not just small n.

// The solution is straightforward: iterate through all pairs, compute `(a+1)%5` and `(a+2)%5`, and compare them to `b`. If either matches, increment dario's counter; otherwise increment xerxes's counter. After the loop, compare the two counters: if dario > xerxes return `"dario"`, if xerxes > dario return `"xerxes"`, else return `"tie"`. Edge cases include: n=0 (both counters remain 0, so tie), duplicates (they are handled normally), and cases where both `(a+1)%5` and `(a+2)%5` equal `b` (which can happen when 5 divides the difference, e.g., a=0, b=1: (0+1)%5=1 and (0+2)%5=2, but if a=0, b=0? No, because (0+1)%5=1 and (0+2)%5=2, neither equals 0; however, if a=3, b=0: (3+1)%5=4, (3+2)%5=0, only one matches; but if a=4, b=1: (4+1)%5=0, (4+2)%5=1, only one; actually it's impossible for both to equal b since (a+1)%5 and (a+2)%5 are always distinct because the difference is 1 mod 5). So no special handling needed. Time complexity is O(n) because we process each pair once, and space complexity is O(1) beyond the input vector storage.

#include <string>
#include <vector>

// Determines the winner of the "dario vs xerxes" game.
// For each pair (a, b), dario scores if b equals (a+1)%5 or (a+2)%5; else xerxes scores.
// Returns "dario", "xerxes", or "tie".
std::string determineWinner(int n, const std::vector<std::pair<int, int>>& pairs) {
    int dario_score = 0;
    int xerxes_score = 0;

    for (int i = 0; i < n; ++i) {
        int a = pairs[i].first;
        int b = pairs[i].second;
        int plus_one = (a + 1) % 5;
        int plus_two = (a + 2) % 5;
        if (b == plus_one || b == plus_two) {
            ++dario_score;
        } else {
            ++xerxes_score;
        }
    }

    if (dario_score > xerxes_score) {
        return "dario";
    } else if (xerxes_score > dario_score) {
        return "xerxes";
    } else {
        return "tie";
    }
}

#include <cassert>
#include <string>
#include <vector>

// Function declaration (must match the solution above)
std::string determineWinner(int n, const std::vector<std::pair<int, int>>& pairs);

int main() {
    // Test 1: Example from original snippet: n=3, pairs (0,2) (1,4) (2,0)
    // (0+1)%5=1,(0+2)%5=2 -> b=2 matches => dario
    // (1+1)%5=2,(1+2)%5=3 -> b=4 no match => xerxes
    // (2+1)%5=3,(2+2)%5=4 -> b=0 no match => xerxes
    // dario=1, xerxes=2 -> xerxes wins
    assert(determineWinner(3, {{0,2},{1,4},{2,0}}) == "xerxes");

    // Test 2: All dario points: a=0, b=1; a=1,b=3; a=2,b=4 (all match)
    assert(determineWinner(3, {{0,1},{1,3},{2,4}}) == "dario");

    // Test 3: Tie: n=4, two dario, two xerxes
    // (0,1) dario, (0,0) xerxes, (1,3) dario, (1,0) xerxes
    assert(determineWinner(4, {{0,1},{0,0},{1,3},{1,0}}) == "tie");

    // Test 4: Empty input
    assert(determineWinner(0, {}) == "tie");

    // Test 5: Single pair dario
    assert(determineWinner(1, {{4,0}}) == "dario"); // (4+1)%5=0, matches

    // Test 6: Single pair xerxes
    assert(determineWinner(1, {{4,4}}) == "xerxes"); // (4+1)%5=0,(4+2)%5=1, 4 not match

    // Test 7: Boundary values a=0,b=2 (dario via +2)
    assert(determineWinner(1, {{0,2}}) == "dario");

    // Test 8: Boundary values a=0,b=3 (xerxes)
    assert(determineWinner(1, {{0,3}}) == "xerxes");

    // Test 9: Many pairs, all dario possible? a=0,b=1; a=0,b=2; a=1,b=2; a=1,b=3; a=2,b=3; a=2,b=4; a=3,b=4; a=3,b=0; a=4,b=0; a=4,b=1 => 10 pairs all dario
    std::vector<std::pair<int,int>> many = {{0,1},{0,2},{1,2},{1,3},{2,3},{2,4},{3,4},{3,0},{4,0},{4,1}};
    assert(determineWinner(10, many) == "dario");

    // Test 10: Mixed with duplicate pairs
    assert(determineWinner(5, {{0,1},{0,1},{4,4},{4,4},{2,3}}) == "dario"); // dario:3, xerxes:2

    return 0;
}
