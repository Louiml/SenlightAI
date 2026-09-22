/*
You are helping a dance troupe coordinate costume colors for a final performance. There are `n` dancers standing in a line, and the show requires the lineup to be a palindrome: for every `i`, dancer `i` and dancer `n-1-i` must wear the same color. Each dancer is currently assigned a costume color, given as an integer array `costume` of length `n`, where `0` means white, `1` means black, and `2` means “not assigned yet”. You may assign any not-assigned dancer either white (cost `a`) or black (cost `b`). If a dancer already has a fixed color, you cannot change it. If the current fixed assignments already violate the palindrome condition (i.e., there exists a pair `(i, n-1-i)` where both are fixed and different), the setup is impossible. Otherwise, write a function `minCostPalindrome(int n, int a, int b, const std::vector<int>& costume)` that returns the minimum total cost to assign all unassigned dancers so the entire lineup is a palindrome, or returns `-1` if it is impossible.
*/

#include <vector>
#include <algorithm>

// Returns the minimum total cost to make the costume line a palindrome,
// or -1 if the fixed assignments already conflict.
int minCostPalindrome(int n, int a, int b, const std::vector<int>& costume) {
    int total_cost = 0;

    // Check symmetric pairs in the first half.
    for (int i = 0; i < n / 2; ++i) {
        int left = costume[i];
        int right = costume[n - 1 - i];

        // Both fixed: must be equal.
        if (left != 2 && right != 2) {
            if (left != right) {
                return -1;
            }
            continue;
        }

        // At least one is free.
        if (left == 0 || right == 0) {
            total_cost += a;
        } else if (left == 1 || right == 1) {
            total_cost += b;
        } else {
            // Both are 2 (free).
            total_cost += 2 * std::min(a, b);
        }
    }

    // Handle the middle element if n is odd.
    if (n % 2 == 1 && costume[n / 2] == 2) {
        total_cost += std::min(a, b);
    }

    return total_cost;
}

#include <cassert>
#include <vector>

// The solution function is defined above (for brevity, assumed included).
int main() {
    // Example from the problem: n=4, a=10, b=20, all free → each pair costs 2*min=20, two pairs → 40
    assert(minCostPalindrome(4, 10, 20, {2, 2, 2, 2}) == 40);

    // Impossible: fixed pair mismatch
    assert(minCostPalindrome(4, 10, 20, {0, 2, 2, 1}) == -1);

    // Already valid and all fixed: no cost
    assert(minCostPalindrome(4, 10, 20, {0, 1, 1, 0}) == 0);

    // One free in a pair, forced color
    assert(minCostPalindrome(3, 5, 7, {0, 2, 0}) == 5);

    // n=1, free middle
    assert(minCostPalindrome(1, 3, 9, {2}) == 3);

    // Mixed pairs: first pair left=1, right=2 → cost b=6; second pair left=2,right=2 → cost 2*min(4,6)=8; n=5, middle=2 → cost 4. total=18
    assert(minCostPalindrome(5, 4, 6, {1, 2, 2, 2, 2}) == 18);

    // n=2, one fixed 0, one free → cost a
    assert(minCostPalindrome(2, 4, 6, {0, 2}) == 4);

    // n=2, one fixed 1, one free → cost b
    assert(minCostPalindrome(2, 4, 6, {2, 1}) == 6);

    // n=0? Not typical but if n=0, loop does nothing, returns 0
    assert(minCostPalindrome(0, 4, 6, {}) == 0);
}

// The key insight is that a valid setup exists if and only if for every symmetric pair `(i, n-1-i)` where both dancers have fixed colors, those colors are equal. Once that condition holds, we must decide each pair independently. For a pair where one dancer is fixed and the other is not, we must assign the free dancer the fixed color, incurring the corresponding cost (`a` if the fixed color is 0, `b` if it is 1). If both are free, we assign the cheaper color to both, costing `2 * min(a, b)`. If both are fixed and equal, no cost is incurred. For the middle element when `n` is odd, if it is free, it costs `min(a, b)` to assign it either color. The algorithm iterates over only the first half of the array, so it runs in `O(n)` time and uses `O(1)` extra space (excluding the input vector). Edge cases include `n=1`, all free dancers, and pairs where one is free and the other fixed.
