// You are given three positive integers representing the number of cards in three piles. In each move, you may take exactly one card from each of two different piles (so the total number of cards decreases by 2). You may repeat this operation as many times as you like, but you cannot take from the same pile twice in the same move. Write a C++ function `int maxMoves(int a, int b, int c)` that returns the maximum number of moves you can perform before no legal move is possible. The input integers can be up to \(10^9\). The function must handle all orderings of the three piles and any values (including 0 for any pile).

// The problem is equivalent to: given three piles, repeatedly remove one from two distinct piles. This is a classic problem equivalent to finding the maximum number of pairs you can form without using the same pile twice in a pair. The optimal strategy is to always combine the largest pile with the second largest, then adjust. Let the piles be sorted as \(x \le y \le z\). Each move reduces two piles by 1. The maximum number of moves is limited by either the sum of the two smaller piles (since every move uses at most one from the two smallest combined) or by the floor of the total sum divided by 2. More precisely:  
// - Let `total = x + y + z`.  
// - The maximum possible moves is `min(x + y, total / 2)`.  
// Why? Each move consumes one from two distinct piles, so total cards decrease by 2 per move, so upper bound is `total/2`. Also, the largest pile `z` cannot be used in more moves than the other two combined `x + y` (because each move using `z` needs another card from `x` or `y`). So the answer is `min(x + y, total / 2)`. This works for all edge cases: if one pile is very large, the answer is `x + y`; if all are equal, it's `total/2`; if one is 0, it's `min(x + y, total/2)` which correctly becomes 0 if both other piles are 0 or one. Time complexity: O(1) after sorting (sorting three elements is constant). Space: O(1).

#include <algorithm>

// Return the maximum number of moves possible when each move removes one card
// from two distinct piles.
int maxMoves(int a, int b, int c) {
    // Sort the three pile sizes.
    int piles[3] = {a, b, c};
    std::sort(piles, piles + 3);

    // The two smaller piles are piles[0] and piles[1].
    int twoSmallestSum = piles[0] + piles[1];
    long long total = static_cast<long long>(a) + b + c;

    // Upper bound from total cards: each move uses 2 cards.
    long long upperByTotal = total / 2;

    // Cap by the combined size of the two smaller piles.
    long long answer = std::min(static_cast<long long>(twoSmallestSum), upperByTotal);
    return static_cast<int>(answer);
}

#include <cassert>

int maxMoves(int a, int b, int c); // declaration

int main() {
    // Basic cases
    assert(maxMoves(1, 1, 1) == 1);          // total=3, floor=1, twoSmallest=2 -> 1
    assert(maxMoves(2, 2, 2) == 3);          // total=6, floor=3, twoSmallest=4 -> 3
    assert(maxMoves(1, 2, 3) == 3);          // total=6, floor=3, twoSmallest=3 -> 3
    assert(maxMoves(0, 0, 0) == 0);          // no cards
    assert(maxMoves(0, 5, 5) == 5);          // total=10, floor=5, twoSmallest=5 -> 5
    assert(maxMoves(0, 0, 5) == 0);          // need two piles with cards
    // Large numbers
    assert(maxMoves(1000000000, 1000000000, 1000000000) == 1500000000);
    assert(maxMoves(1000000000, 1, 1) == 2); // total=1000000002, floor=500000001, twoSmallest=2 -> 2
    assert(maxMoves(7, 3, 4) == 7);          // total=14, floor=7, twoSmallest=7 -> 7
    assert(maxMoves(9, 5, 2) == 7);          // total=16, floor=8, twoSmallest=7 -> 7
    return 0;
}
