// Write a C++ function `std::string formatContestMatches(int n)` that accepts an integer `n` which is a power of two (2, 4, 8, 16, ...). It should return a string representing the tournament bracket for the initial round of a knockout competition with teams numbered 1 through `n`, paired in the standard "strength-balancing" manner: the strongest team (1) faces the weakest (n), the second strongest (2) faces the second weakest (n-1), and so on. For each pairing, the output must be formatted as `(strongTeam,weakTeam)`. These pairs are then recursively combined in the same fashion: the winner of the first pair (which contains team 1) faces the winner of the last pair (which contains team n), the winner of the second pair faces the winner of the second-to-last pair, and so on, with each combination wrapped in parentheses and separated by a comma. The final output is a single string representing the full tournament tree up to the final match. For example, `n=2` gives `"(1,2)"`, `n=4` gives `"((1,4),(2,3))"`, and `n=8` gives `"(((1,8),(4,5)),((2,7),(3,6)))"`. The function must handle `n` being 2 or any higher power of two, and you can assume input is always valid.

The problem is essentially constructing a full binary tree where the leaves are the initial team pairings, and internal nodes combine two subtrees in a specific order. The key observation is that the pairing for the first round always pairs the smallest remaining team with the largest remaining team, the second smallest with the second largest, etc. When combining, the structure is recursive: to build the bracket for `n`, you first build the bracket for the first half (teams 1 to n/2) and the second half (teams n/2+1 to n), then combine them as `(firstHalf,secondHalf)` where the ordering is dictated by the recursive placement of the initial pairings. A simpler recursive approach is to define a function that builds the string for a range of teams from `start` to `end` (inclusive) where the length is a power of two. For `length == 2`, return `"(start,end)"`. For larger lengths, recursively build the left half as `build(start, start+length/2-1)` and the right half as `build(start+length/2, end)`, then return `"(" + left + "," + right + ")"`. This works because the recursive splitting naturally produces the correct pairing order: the first recursive call covers the lower-numbered teams, and the second covers the higher-numbered ones, and when unpacked, the initial leaf pairings match the required strongest-weakest pattern. Edge cases: the base case is `n=2`. Time complexity is O(n) because each team appears exactly once in the output, and the total string length is O(n log n) due to parentheses, but the construction visits each node once, so time is O(n). Space complexity is O(log n) due to recursion depth, plus the output string size O(n log n).

#include <string>

// Builds the tournament bracket string for teams numbered from `low` to `high`
// inclusive, where the number of teams is a power of two.
std::string buildBracket(int low, int high) {
    if (low == high) {
        return std::to_string(low);
    }
    int mid = low + (high - low) / 2;
    std::string left = buildBracket(low, mid);
    std::string right = buildBracket(mid + 1, high);
    return "(" + left + "," + right + ")";
}

// Formats the initial round of a contest with `n` teams (n is power of two).
// The returned string represents the full tournament tree.
std::string formatContestMatches(int n) {
    return buildBracket(1, n);
}

#include <cassert>
#include <string>

// Assume formatContestMatches is defined above.

int main() {
    assert(formatContestMatches(2) == "(1,2)");
    assert(formatContestMatches(4) == "((1,4),(2,3))");
    assert(formatContestMatches(8) == "(((1,8),(4,5)),((2,7),(3,6)))");
    assert(formatContestMatches(16) == "((((1,16),(8,9)),((4,13),(5,12))),(((2,15),(7,10)),((3,14),(6,11))))");
    assert(formatContestMatches(2) == "(1,2)");
    assert(formatContestMatches(4) != "((1,3),(2,4))");
    assert(formatContestMatches(8).front() == '(');
    assert(formatContestMatches(8).back() == ')');
    assert(formatContestMatches(2).size() == 5);
    assert(formatContestMatches(8).size() == 31);
}
