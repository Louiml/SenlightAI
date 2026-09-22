Write a C++ function `int minimumMoves(long long x, long long y)` that, given two non-negative integers `x` and `y` with `x <= y`, returns the minimum number of moves needed to travel from position `x` to position `y` on a number line, where in the `k`-th move you may move exactly `k` units in either direction. The function should handle very large inputs (up to 1e18) and must not rely on floating-point precision issues. The input is guaranteed to satisfy `0 <= x <= y <= 1e18`.

#include <cassert>

int main() {
    assert(minimumMoves(0, 0) == 0);
    assert(minimumMoves(0, 1) == 1);
    assert(minimumMoves(0, 2) == 3);
    assert(minimumMoves(0, 3) == 2);
    assert(minimumMoves(0, 4) == 3);
    assert(minimumMoves(0, 5) == 5);
    assert(minimumMoves(0, 6) == 3);
    assert(minimumMoves(0, 7) == 5);
    assert(minimumMoves(5, 12) == 3); // d=7
    assert(minimumMoves(100, 100) == 0);
    assert(minimumMoves(0, 1000000000000000000LL) == 1414213563); // known result (from pattern)
    return 0;
}

#include <cmath>

// Returns the minimum number of moves to go from x to y,
// where the k-th move has length exactly k and can be in either direction.
// Precondition: 0 <= x <= y <= 1e18.
long long minimumMoves(long long x, long long y) {
    long long d = y - x;
    if (d == 0) return 0;

    // Find smallest n with n*(n+1)/2 >= d using integer binary search.
    long long lo = 1, hi = 2000000000LL; // safe upper bound for d <= 1e18
    while (lo < hi) {
        long long mid = lo + (hi - lo) / 2;
        long long total = mid * (mid + 1) / 2;
        if (total >= d) hi = mid;
        else lo = mid + 1;
    }
    long long n = lo;

    // Adjust parity: total - d must be even.
    while (true) {
        long long total = n * (n + 1) / 2;
        if ((total - d) % 2 == 0) break;
        ++n;
    }
    return n;
}

// This is the classic "minimum number of moves to reach a point on a number line with increments 1, 2, 3, ..." problem. The key observation is that the sequence of moves forms a sum like `1 + 2 + ... + n = n(n+1)/2`. The optimal strategy is symmetric: you first increase by 1, 2, 3, ..., maybe stop, then decrease symmetrically if needed. The minimal number of moves for a distance `d = y - x` is found by taking `n` as the smallest integer such that `n(n+1)/2 >= d`, then adjusting based on the difference. Actually, the well-known formula is: compute `n = ceil((sqrt(8*d + 1) - 1)/2)`. Then let `total = n(n+1)/2`. The difference `diff = total - d` determines the answer: if `diff` is even, answer is `n`; if `diff` is odd, answer is `n+1` if `(total - n) >= d`, otherwise `n+2`. However, a simpler equivalent: if `total == d`, answer = `n`; else if `total - d` is even, answer = `n`; else answer = `n+1`. Wait – need to check carefully. For example, d=2: n=2 (since 2*3/2=3 >=2), total=3, diff=1 odd, but the correct answer is 3 moves (1+2-1? Actually 1+2 = 3, to get 2 you can do 1+2-1 = 2? That uses 3 moves. So answer is 3, which is n+1. d=5: n=3 (3*4/2=6 >=5), diff=1 odd, answer is 3? 1+2-? 1+2+3=6, to get 5 you can do 1+2+3-1=5 but that's 4 moves. Actually the correct minimal moves for 5 is 3? Let's test: 1+2+? 1+2+? 1+2+? To reach 5 in 3 moves: 1+2+? =5 -> ?=2 but second move already 2? No, moves must be distinct lengths? Actually the problem says in the k-th move you move exactly k units, so moves are 1,2,3,... each used at most once. So for distance 5, possible sequences: 1+2+3=6, then overshoot by 1, but you can't go negative? You can move left too. So do 1+2+3 -? That would be 4 moves if you subtract. Alternatively 1+3-? Actually you can go left or right each move, but lengths used are distinct: you can do +1 +2 -3 +5? No, fifth move is 5 units but you only have 4 moves. Hmm. Actually the sequence uses moves 1..n exactly once each, each can be positive or negative. So sum of n signed distinct integers from 1..n. The minimal n such that the range of achievable sums covers d. The maximum sum is n(n+1)/2, and by flipping signs the achievable sums are all integers with same parity as total and between -total and total. So distance d is achievable with n moves iff d <= total and (total - d) is even. Since total and d are integers, parity condition: (total - d) % 2 == 0. So minimal n is the smallest n such that total >= d and (total - d) % 2 == 0. But careful: you can also flip a sign to reduce sum by 2*value, so the reachable set is all integers that have the same parity as total and absolute value <= total. So you need n such that n(n+1)/2 >= d and (n(n+1)/2 - d) % 2 == 0. If the first n that satisfies total >= d doesn't meet parity, then try n+1 (or n+2). Actually n+1 will have different parity? Let's examine: total(n+1) = total(n) + (n+1). So parity flips if n+1 is odd. So sometimes n+1 works, else n+2 works. A known result: the answer is the smallest n with n(n+1)/2 >= d and (n(n+1)/2 - d) even. Since total grows quadratically, you can find n by integer sqrt. For d=2: n=2 (total=3), diff=1 odd -> try n=3 total=6 diff=4 even -> answer 3. For d=5: n=3 (total=6) diff=1 odd -> n=4 total=10 diff=5 odd -> n=5 total=15 diff=10 even -> answer 5? But wait, is 5 moves minimal for distance 5? Let's test manually: possible sums with 1..3: max 6, but parity odd (6-5=1) cannot be made. With 1..4: total=10, diff=5 odd impossible; with 1..5 total=15, diff=10 even, so yes 5 moves. But actually for distance 5, is there a 4-move solution? Moves 1,2,3,4 can sum to any number between -10 and 10 with parity same as 10 (even), so only even distances. 5 is odd, so not possible. So 5 moves is correct. However, common competitive programming solution uses rounding square root, but that's approximate. The correct approach is integer arithmetic: compute n = (sqrt(8*d + 1) - 1)/2 using integer sqrt and adjustment. But to avoid floating point, we can binary search or use integer sqrt function. Edge cases: d=0 -> 0 moves. d=1 -> n=1 total=1 diff=0 even -> 1. d=3 -> n=2 total=3 diff=0 -> 2 moves (1+2). d=4 -> n=2 total=3 <4, n=3 total=6 diff=2 even -> 3. d=6 -> n=3 total=6 -> 3. d=7 -> n=3 total=6 <7, n=4 total=10 diff=3 odd, n=5 total=15 diff=8 even -> 5? But actually for 7, can do 1+2+3+? 1+2+3+? That's 4 moves? 1+2+3+? to get 7 you need +1 but 1 used. Try 1+2+4? moves are 1,2,3,4,5... but you can skip? No, you must use each length exactly once? Actually the problem statement says "in the k-th move you may move exactly k units", so you have to make moves with lengths 1,2,3,... sequentially? That interpretation would force using all lengths up to some n. But typically this classic problem allows using any subset? Wait, re-read: "in the k-th move you may move exactly k units" suggests that the k-th move is a fixed length k, so you must make moves of lengths 1,2,3,... in order. So the number of moves is exactly n, and you decide direction for each. So you must use all lengths 1..n. Then the achievable sums are as described. So for distance 7: try n=4: lengths 1,2,3,4 can sum to? Max 10, parity even, so can get 7? 10-7=3 odd cannot. n=5: lengths 1..5 total=15, parity odd? 15 odd, need 7 odd, diff=8 even, yes can. So answer 5. So the algorithm: let d = y - x. If d == 0 return 0. Compute smallest n such that n(n+1)/2 >= d. Then while ((n(n+1)/2 - d) % 2 != 0) n++. Return n. Since n is at most about sqrt(2d) + 2, this is efficient. To compute initial n without floating point, we can use integer binary search or `sqrt` with careful adjustment. For d up to 1e18, n up to ~1.4e9, so binary search on n from 0 to 2e9 works. Or use integer sqrt and adjust. Time complexity O(log d) for binary search, or O(1) with sqrt and adjustment. Space O(1). Edge cases: d=0, d very large, and ensure no overflow (use long long, but n can be up to ~1.4e9, n(n+1)/2 up to ~1e18 fits in long long).
