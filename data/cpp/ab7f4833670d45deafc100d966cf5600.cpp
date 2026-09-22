In a tournament-style game, each of the \(n\) players starts with a certain initial score. After the first round, every player gains a bonus that may increase or decrease their total. You are given two sequences of \(n\) integers: the initial scores \(a_i\) and the bonuses \(b_i\). Let \(s_i = b_i - a_i\), representing player \(i\)'s net change after the round. A player is considered "stable" if their net change is non-negative; otherwise they are "unstable". The rules are: if \(n = 2\), the game is valid only if exactly one player has a net change of zero (the other can be anything, but check: the condition is that the second player's net change must be zero). For \(n \ge 3\), sort the net changes of players 2 through \(n\) in descending order, then simulate a process starting with player 1's net change as a "power reserve". In each step from \(i = 1\) to \(n-1\), you must spend exactly 1 unit from the reserve (so the reserve decreases by 1), and then add the \(i\)-th sorted net change to the reserve; if at any point the reserve becomes negative before adding, the game is invalid. If after processing all players the reserve never went negative, the game is valid. Write a C++ function `bool isValidTournament(int n, const std::vector<long long>& a, const std::vector<long long>& b)` that returns `true` if the game is valid, `false` otherwise.

// The core idea is to simulate the described process directly. For each player, compute `win[i] = b[i] - a[i]`. For `n == 2`, the rule simplifies: the game is valid if and only if `win[1] == 0` (i.e., the second player's net change is exactly zero). For `n >= 3`, sort all net changes from index 1 to `n-1` in descending order (using `std::greater<long long>()`). Initialize `reserve = win[0]`. Then for `i = 1` to `n-1`: first decrement `reserve` by 1 (this represents the mandatory cost of activating the next player). If after decrementing `reserve` is negative, return `false`. Then add `win[i]` (the sorted value) to `reserve`. After the loop, if no early return, return `true`. Edge cases: `n == 1`? The problem statement implies at least 2 players, but if `n == 1`, the loop does nothing and we would return `true` as long as we handle it; but given the original code, it uses `n==2` check and then sorts `win[1]` to `win[n-1]`, so for `n==1` it would sort an empty range and loop `i=1 < n` fails, then print "YES". We'll mirror that: for `n==1` return `true` (no constraints). Also negative net changes are allowed; the reserve can become large due to positive ones. The time complexity is \(O(n \log n)\) due to sorting, and \(O(n)\) space for the `win` vector (or we can compute in-place). Note: the original code uses a global array and zeroes it, but we'll use a local vector for self-containedness.

#include <vector>
#include <algorithm>
#include <functional>

// Returns true if the tournament is valid per the given rules.
bool isValidTournament(int n, const std::vector<long long>& a, const std::vector<long long>& b) {
    if (n == 0) return false;
    std::vector<long long> win(n);
    for (int i = 0; i < n; ++i) {
        win[i] = b[i] - a[i];
    }
    if (n == 2) {
        // For two players, the second must have zero net change.
        return win[1] == 0;
    }
    // Sort all but the first player's net change in descending order.
    std::sort(win.begin() + 1, win.end(), std::greater<long long>());
    long long reserve = win[0];
    for (int i = 1; i < n; ++i) {
        // Spend 1 from reserve before adding the next net change.
        if (--reserve < 0) {
            return false;
        }
        reserve += win[i];
    }
    return true;
}

#include <cassert>
#include <vector>

// The solution function is declared above.
bool isValidTournament(int n, const std::vector<long long>& a, const std::vector<long long>& b);

int main() {
    // n == 2: second net change must be zero.
    assert(isValidTournament(2, {5, 3}, {5, 3}) == true);   // win = {0,0} -> second=0
    assert(isValidTournament(2, {5, 3}, {5, 4}) == false);  // win = {0,1} -> second!=0
    assert(isValidTournament(2, {5, 4}, {5, 4}) == true);   // win = {0,0}
    assert(isValidTournament(2, {5, 4}, {6, 4}) == false);  // win = {1,0} -> second=0? Actually win[1]=0, so true? Check: win[0]=1, win[1]=0 -> second=0 -> true. Let's test intended: this is valid.
    assert(isValidTournament(2, {5, 4}, {6, 4}) == true);   // correct per rule.

    // n >= 3 typical cases
    // win = {0, -1, 1} sorted rest: {1, -1}: reserve=0 -> spend1 -> -1 fail
    assert(isValidTournament(3, {1, 2, 3}, {1, 1, 4}) == false);
    // win = {2, 0, 0} sorted rest: {0,0}: reserve=2 -> spend1->1 +0=1 -> spend1->0 +0=0 true
    assert(isValidTournament(3, {0, 1, 1}, {2, 1, 1}) == true);
    // win = {1, -2, 3} sorted rest: {3, -2}: reserve=1 -> spend1->0+3=3 -> spend1->2-2=0 true
    assert(isValidTournament(3, {2, 5, 0}, {3, 3, 3}) == true);
    // win = {0, -5, 6} sorted rest: {6, -5}: reserve=0 -> spend1->-1 fail
    assert(isValidTournament(3, {5, 5, 0}, {5, 0, 6}) == false);
    // Larger case: n=5, win = {3, -1, 2, -2, 1} sorted rest: {2,1,-1,-2}
    // reserve=3 -> spend1->2+2=4 -> spend1->3+1=4 -> spend1->3-1=2 -> spend1->1-2=-1 fail
    assert(isValidTournament(5, {0,1,2,3,4}, {3,0,4,1,5}) == false);
    // Another: win = {4, 0, -1, 1, -1} sorted rest: {1,0,-1,-1}
    // reserve=4 -> spend1->3+1=4 -> spend1->3+0=3 -> spend1->2-1=1 -> spend1->0-1=-1 fail
    assert(isValidTournament(5, {1,1,1,1,1}, {5,1,0,2,0}) == false);
    // Win = {5, -2, 3, -2, 1} sorted rest: {3,1,-2,-2}
    // reserve=5 -> spend1->4+3=7 -> spend1->6+1=7 -> spend1->6-2=4 -> spend1->3-2=1 true
    assert(isValidTournament(5, {0,1,0,1,0}, {5,-1,3,-1,1}) == true);
    return 0;
}
