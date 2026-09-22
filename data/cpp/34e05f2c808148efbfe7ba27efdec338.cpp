You are developing a utility for a guessing-game backend. The server holds a secret integer between `1` and `N` inclusive. A client can submit a guess, and the server will respond with `0` if the guess is correct, `-1` if the secret is larger than the guess, or `1` if the secret is smaller than the guess. However, in this implementation, the secret is not fixed; instead, the game follows a deterministic binary-search-like protocol: the client always guesses the midpoint of the current interval, and the server’s response is consistent with a hidden target that is *chosen adversarially* but always within the current interval. Your task: write a standalone function `int findSecret(int n, const vector<int>& responses)` that, given the maximum value `n` and a vector of responses (each `0`, `1`, or `-1`) obtained by following the protocol (starting with interval `[1, n]`, guessing `mid = (l+r+1)/2`, and updating `l` or `r` as per the response), returns the secret value when the response `0` is encountered. If the vector ends without a `0` (i.e., the protocol would continue but the responses are incomplete), return `-1`. The function must simulate the protocol correctly and handle edge cases such as `n=1`, all responses being non-zero, and responses that would force an invalid interval (in which case return `-1` immediately). Do not include any input/output; just the pure simulation logic.
#include <cassert>
#include <vector>

// The solution function is assumed to be declared above.
int findSecret(int n, const std::vector<int>& responses);

int main() {
    // Basic case: n=5, guesses: mid=3, response -1 => l=4, r=5, mid=5, response 0 => secret 5
    assert(findSecret(5, {-1, 0}) == 5);
    // Case: n=5, first guess correct
    assert(findSecret(5, {0}) == 3); // (1+5+1)/2 = 3
    // n=1, guess is 1, correct
    assert(findSecret(1, {0}) == 1);
    // Incomplete responses: no 0 found
    assert(findSecret(10, {-1, -1}) == -1);
    // Inconsistent: after response 1 on guess 5, r=4, but l=1, okay; next response -1 on guess 3 => l=4, now l>r
    assert(findSecret(10, {1, 1}) == -1); // first mid=5, 1 => r=4; second mid=(1+4+1)/2=3, 1 => r=2; no 0, incomplete? Actually still l=1<=r=2, but no 0 -> -1
    // A valid longer path: n=10, guesses: mid=5 (1 => r=4), mid=3 (-1 => l=4), mid=4 (0) => secret 4
    assert(findSecret(10, {1, -1, 0}) == 4);
    // Empty responses
    assert(findSecret(7, {}) == -1);
    // Sequence that goes to a boundary: n=2, mid=2 (since (1+2+1)/2=2), response -1 => l=3, r=2 invalid => -1
    assert(findSecret(2, {-1}) == -1);
    // n=2, mid=2, response 0 => secret 2
    assert(findSecret(2, {0}) == 2);
    // n=100, simulate a simple path: first mid=50, 1 => r=49; second mid=25, -1 => l=26; third mid=38, 0 => secret 38
    assert(findSecret(100, {1, -1, 0}) == 38);
    return 0;
}
#include <vector>

// Simulate the game protocol and return the secret value if found.
// responses: sequence of server replies. 0 = correct, -1 = secret larger, 1 = secret smaller.
// Returns the secret if a 0 is encountered, otherwise -1 for inconsistency/incomplete.
int findSecret(int n, const std::vector<int>& responses) {
    int l = 1;
    int r = n;

    for (int response : responses) {
        int mid = (l + r + 1) / 2;
        if (response == 0) {
            return mid;
        } else if (response == -1) {
            // Secret is larger than mid
            l = mid + 1;
        } else { // response == 1
            // Secret is smaller than mid
            r = mid - 1;
        }
        if (l > r) {
            // Invalid interval: responses inconsistent
            return -1;
        }
    }

    // No correct guess encountered
    return -1;
}
// The task is essentially to simulate the described binary search on the interval `[1, n]`. At each step, compute `mid = (l + r + 1) / 2`. If the response is `0`, the secret is `mid`. If the response is `-1`, the secret is greater than `mid`, so set `l = mid + 1`. If the response is `1`, the secret is less than `mid`, so set `r = mid - 1`. If after any update `l > r`, the responses are inconsistent, and we return `-1`. If we exhaust the responses without seeing a `0`, return `-1` because the protocol would continue but we lack data. Important edge cases: `n` can be 1, where the first guess is `1`; responses may be empty (return `-1`); the vector may contain a `0` not on the last element (we return that `mid` immediately); also, after a `-1` or `1`, the next guess must be computed with the updated interval. Time complexity is O(k) where k is the number of responses, and space is O(1) auxiliary (excluding the input vector). No need for extra data structures.
