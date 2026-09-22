// Given an array of positive integers, write a C++ function `long long minCuts(const std::vector<long long>& a)` that returns the minimum possible "cost" defined as follows. You may choose any prime divisor `p` (greater than 1) of the total sum `S` of the array. Then you must partition the array into contiguous segments such that each segment's sum is exactly divisible by `p`? Actually, the original problem is more subtle: you choose a prime factor `p` of the total sum, and you are allowed to cut the array at arbitrary positions, but each cut splits the array into two parts; the "cost" is the sum over all cuts of the absolute difference between the left and right prefix sums taken modulo `p` after each cut. More precisely: Given an array `a[0..n-1]` with total sum `S`, for a chosen prime `p` dividing `S`, you repeatedly make cuts: you may cut the current segment at any point. After each cut, you pay the absolute difference between the sum of the left part (taken modulo `p`? Wait, the original code uses modulo p on prefix sums) — the actual rule from the snippet: You have an array of numbers. You pick a prime divisor `p` of the total sum. Then you go through the array left to right, maintaining a running sum modulo `p`. Each time the running sum resets to 0 after being reduced (i.e., whenever the running sum reaches the point where you can subtract `p`), you pay a cost equal to the minimum prefix sum deficit encountered since the last reset. This is hard to paraphrase exactly.
//
// Simplify: Write a function that, given a positive integer array and given a prime `p` that divides the total sum, computes the minimum total "penalty" defined as follows: Partition the array into contiguous groups; each group must have sum divisible by `p`. The penalty of a partition is the sum over all groups of the minimum prefix sum (within that group) of the group's elements taken mod `p`. You can choose any prime divisor `p` of the total sum; the answer is the minimum penalty over all such primes. If the total sum is 1, return -1. The function should handle up to 10^5 elements and sums up to 10^6.
//
// Actually, looking at the original code, it computes a value using a greedy-like dynamic programming, but the task can be restated as: Given an array of positive integers, for each prime `p` dividing the total sum `S`, define `f(p)` as the minimum over all ways to cut the array into segments whose sums are multiples of `p` of the sum of `(segment_length? no)` — the code does something with a DP on prefix sums modulo `p`. To make it self-contained, we can rephrase the problem in a simpler but equivalent way: You can place cuts anywhere. After each cut, you pay the absolute difference between the sum of the left part and the right part, but you are allowed to adjust? The original code uses a two-pass algorithm that seems to compute something like: simulate a process where you walk through the array, maintaining a running sum `cur` modulo `p`. Each time `cur` reaches 0 after you add an element, you must "pay" the minimum value that `cur` attained (in terms of actual remainder) since the last reset, and then reset. The total cost is sum of such payments. This is deterministic given `p`, not minimized over cuts? Actually the code does a single deterministic pass, but that does not maximize or minimize; it just computes a specific value. However, the snippet uses `check(i)` and takes the min over prime divisors, so the final answer is the minimum `check(p)` over all prime divisors `p` of the total sum.
//
// Thus the task: Write a function `long long minCost(const std::vector<long long>& a)` that computes that minimum `check(p)` as defined by the algorithm in the snippet. You may assume all numbers are positive, n up to 100000, each a[i] up to 10^6, total sum up to 10^6? Actually the original uses N=1e6 sieve. The function should implement the same logic: For each prime factor p of the total sum, compute `check(p)` as described, and return the minimum. If total sum is 1, return -1.
//
// We can describe `check(p)` in a clean way: 
//
// Let `a[0..n-1]` be the array, total sum `S`. Define `t[i] = a[i]` for all i. For a given `p`, compute a sequence `tb[j]` for j from 0 to n: Initialize `tb[0]=0`, `b=0`. For each i from 0 to n-1: compute `x = t[i] % p` (if t[i] >= p, take remainder, else use t[i]). Then `b += x`; if `b > p`, then `b -= p; tb[i+1] = b`; else `tb[i+1] = tb[i] + b`. After this, do a backward pass: Initialize `b=0, c=0, d=INF, e=0`. For i from n-1 down to 0: update `d = min(d, tb[i] + c)`; then `b += (t[i] % p)` (or t[i] if less than p); if `b >= p`, then `b -= p; d = min(d, c); e += d; c = 0; d = INF;` else do nothing; then `c += b`. Return `e`. That is exactly the code's `check`.
//
// So the task is to implement that faithfully.

The algorithm first computes all primes up to 1e6 using a sieve. Then it reads the array and sums it. If the total sum is 1, return -1. Otherwise, factorize the total sum by iterating over primes: for each prime `i`, if `i` divides the total sum, repeatedly remove `i` from a copy `pri` of the total sum, and call `check(i)` on the array, taking the minimum. After iterating, if `pri > 1`, then `pri` is a prime factor, so call `check(pri)` as well. The `check(p)` function does two passes: forward to compute prefix "carry" values `tb`, then backward to compute the cost. The logic ensures that the forward pass simulates the remainder after each element when trying to reduce running sum by `p` whenever it exceeds `p`; the backward pass computes a minimal cost based on these remainders. Complexity: Sieve is O(N log log N) for N=1e6, which is fine. For each distinct prime factor (at most 7 distinct for numbers up to 1e6), `check` runs in O(n). So total O(n * number_of_distinct_prime_factors + N log log N). Space O(N) for sieve and O(n) for temporary arrays. Edge cases: total sum 1, array with a single element, prime factors that may be larger than 1e6 if the total sum is a product of large primes, but the total sum is at most n * 1e6 = 1e11, but the original code only considers primes up to 1e6 and then checks if remaining `pri` > 1. Our implementation can do the same.

#include <vector>
#include <algorithm>
#include <cstdint>

// Compute the cost for a given prime p according to the described algorithm.
static long long checkForPrime(const std::vector<long long>& a, long long p) {
    int n = (int)a.size();
    std::vector<long long> tb(n + 1, 0);
    long long b = 0;
    for (int j = 0; j < n; ++j) {
        long long x = (a[j] >= p) ? (a[j] % p) : a[j];
        b += x;
        if (b > p) {
            b -= p;
            tb[j + 1] = b;
        } else {
            tb[j + 1] = tb[j] + b;
        }
    }
    b = 0;
    long long c = 0;
    long long d = (long long)1 << 60;
    long long e = 0;
    for (int j = n - 1; j >= 0; --j) {
        d = std::min(d, tb[j] + c);
        long long x = (a[j] >= p) ? (a[j] % p) : a[j];
        b += x;
        if (b >= p) {
            b -= p;
            d = std::min(d, c);
            e += d;
            c = 0;
            d = (long long)1 << 60;
        }
        c += b;
    }
    return e;
}

// Return the minimum cost over all prime divisors of the total sum.
long long minCost(const std::vector<long long>& a) {
    int n = (int)a.size();
    long long S = 0;
    for (long long v : a) S += v;
    if (S == 1) return -1;

    // Sieve of Eratosthenes up to 1e6 (as in original problem constraints)
    const int N = 1000000;
    std::vector<bool> isPrime(N + 1, true);
    isPrime[0] = isPrime[1] = false;
    for (int i = 2; i * i <= N; ++i) {
        if (isPrime[i]) {
            for (int j = i * i; j <= N; j += i) isPrime[j] = false;
        }
    }

    long long best = (long long)1 << 60;
    long long pri = S;
    for (long long i = 2; i <= N; ++i) {
        if (pri < i) break;
        if (pri < N && !isPrime[pri]) break;  // pri is already smaller than any prime we haven't checked
        if (isPrime[i] && S % i == 0) {
            while (pri % i == 0) pri /= i;
            best = std::min(best, checkForPrime(a, i));
        }
    }
    if (pri > 1) {
        best = std::min(best, checkForPrime(a, pri));
    }
    return best;
}

#include <cassert>
#include <vector>

// Declare the function
long long minCost(const std::vector<long long>& a);

int main() {
    // Example 1: array [2, 2], total sum 4, prime factors 2 -> check cost? Let's compute manually:
    // checkForPrime(p=2): a=[2,2] -> forward: x=0 (2%2=0), b=0, tb[1]=0; segunda: x=0, b=0, tb[2]=0. backward: i=1: d=min(tb[1]+0=0,INF)=0; x=0,b=0; b<2; c=0; i=0: d=min(tb[0]+0=0,0)=0; x=0; b=0; b<2; c=0; final e=0. So best=0.
    assert(minCost({2,2}) == 0);

    // Example 2: [6, 10, 15], total 31 (prime). p=31. Compute check? Might be high, but we can just assert it returns a non-negative value.
    long long val = minCost({6,10,15});
    assert(val >= 0);

    // Example 3: total sum 1 -> -1
    assert(minCost({1}) == -1);

    // Example 4: [3, 5], total 8, prime factor 2. check(2): forward: a[0]=3%2=1,b=1,tb[1]=1; a[1]=5%2=1,b=2,tb[2]=tb[1]+2=3? Actually b=2, not >p, so tb[2]=tb[1]+2=1+2=3. backward: i=1: d=min(tb[1]+c=1+0=1,INF)=1; b+=1 -> b=1,c+=1=1; i=0: d=min(tb[0]+c=0+1=1,1)=1; b+=1 -> b=2>=2 -> b=0, d=min(d,c)=min(1,1)=1, e+=1=1, c=0,d=INF. Then e=1. So cost 1? Actually the code would produce e=1. So minCost should be 1.
    assert(minCost({3,5}) == 1);

    // Example 5: [1, 2, 3], total 6, prime factors 2 and 3. check(2) and check(3). Let's compute check(2): forward: 1%2=1,b=1,tb1=1; 2%2=0,b=1,tb2=1+1=2? Actually tb2=tb1+b=1+1=2; 3%2=1,b=2,tb3=2+2=4? Wait code: if b>p (2>2 false) else tb[i+1]=tb[i]+b. So after 3: b=1+0+1=2, not >2, tb3=tb2+2=2+2=4. Backward: i=2: d=min(tb2+c=2+0=2,INF)=2; b+=1=1,c=1; i=1: d=min(tb1+c=1+1=2,2)=2; b+=0=1,c=2; i=0: d=min(tb0+c=0+2=2,2)=2; b+=1=2>=2 -> b=0,d=min(d,c)=min(2,2)=2,e+=2=2,c=0,d=INF. So cost 2. check(3): forward: 1, b=1, tb1=1; 2, b=3, since b>3? 3>3 false, tb2=1+3=4; 3%3=0, b=3, tb3=4+3=7. Backward: i=2: d=min(tb2+0=4,INF)=4; b+=0=0,c=0; i=1: d=min(tb1+0=1,4)=1; b+=2=2,c=2; i=0: d=min(tb0+2=2,1)=1; b+=1=3>=3 -> b=0,d=min(d,c)=min(1,2)=1,e+=1=1,c=0,d=INF. So cost 1. Min is 1. So minCost should be 1.
    assert(minCost({1,2,3}) == 1);

    // Example 6: [10], total 10, prime factors 2,5. check(2): a[0]=10%2=0 -> forward: b=0, tb1=0; backward: i=0: d=min(tb0+0=0,INF)=0; b+=0; b>=2? no; c+=0; e=0. check(5): a=0 -> cost 0. So minCost = 0.
    assert(minCost({10}) == 0);

    // Example 7: Large array, just ensure it runs without error
    std::vector<long long> big(100000, 1); // sum = 100000, prime factors 2^5*5^5, valid
    long long bigVal = minCost(big);
    assert(bigVal >= 0);
}
