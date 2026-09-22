You are given a rectangular grid of height `h` and width `w`, where `h, w >= 2`. A point starts at coordinates `(x1, y1)` (0-indexed) and moves diagonally with speed one cell per unit time, bouncing off the borders of the rectangle like a billiard ball. Specifically, its velocity vector is initially `(+1, +1)`, and each time it would hit a border, the corresponding component of the velocity reverses (e.g., hitting the left or right wall flips the x-component). The point is initially at `(x1, y1)`, and we are interested in another fixed cell `(x2, y2)`. At each integer time step (including time 0), before the point moves, there is a probability `P%` that the point is “caught” and the process stops. If the point is caught at time `t`, we gain a penalty equal to `t`. If the point is never caught, the process continues indefinitely. Your task is to write a function that, given `h, w, x1, y1, x2, y2, P` (where `P` is an integer 0..100), returns the expected total time until the point is caught, modulo `1,000,000,007`. All coordinates are 0-indexed and are within the grid. The answer is a rational number; output it as a value in `[0, mod)`. Note that because the point bounces, its path is periodic with period `L = 2*(h-1)*(w-1).` The point will visit the target cell at certain times within each period; enumerate those times (they are the same each period). Let those times be `b[0], b[1], ..., b[n-1]` in increasing order, with `n >= 1`. The expected stopping time can be computed from a geometric series that accounts for the possibility of passing the target multiple times without being caught.

// The key is to model the process as a discrete-time Markov chain. The point’s position at integer time `t` is deterministic and periodic with period `L`. The target cell is visited at times `t = b[i] + k*L` for each `i` and each integer `k >= 0`. At each integer time, the point is caught with probability `p = P/100` independent of position. So the time until catch is a geometric random variable with success probability `p` at each step. However, the penalty is the time itself, not just the number of steps, so we need the expected time of the first success in a sequence where the time index matters. Let `q = 1 - p`. The probability that the catch happens exactly at time `t` is `q^t * p`. The expected time is `E[T] = sum_{t>=0} t * p * q^t`. But because the point’s position does not affect catch probability, the expected time is just `(1-p)/p` for a standard geometric (with support starting at 0). That would be the answer if we ignored the special role of the target cell. However, the problem statement implies that the target cell has extra significance? Actually re-reading: the original snippet seems to compute an answer involving the set of times when the point is at the target cell, with a formula that uses sums over those times. That suggests that maybe the catch probability is only non-zero when the point is at the target cell? The description says “at each integer time step, there is a probability P% that the point is caught” – that seems independent of position. But then why would the target cell matter? The snippet clearly uses the times when the point is at the target cell to compute the expectation, implying that the catch only happens when the point is exactly at the target cell. So we should interpret: at each integer time, if the point is exactly at the target cell, there is a probability `p` that it is caught. At other times, it cannot be caught. So the process continues until the first time the point is at the target and the Bernoulli trial succeeds. This matches the snippet: it enumerates all times (mod period) when the point is at the target, and then computes the expected time using a geometric distribution over those periodic opportunities. Therefore the task is: Given the deterministic periodic path, the target is visited at times `b[0] + k*L`, ... for each `k`. Let the set of visitation times be `T = { t >= 0 : position(t) == target }`. At each such time, independently, with probability `p` the process stops; otherwise it continues. What is the expected stopping time? This is a standard geometric series over a periodic set. Let the period length be `L`. Within one period, the target is visited at times `b[0], b[1], ..., b[n-1]` (sorted). The probability that the process survives an entire period without being caught is `q^n` (since it fails to be caught at each of the `n` visits). Let `A = q^n`. The expected time can be computed by conditioning on which period the catch occurs in and which visit within that period. The expected time is sum over `k` from 0 to infinity of `A^k * (sum_{i=0}^{n-1} (b[i] + k*L) * p * q^i)`? Wait careful: within a period, the visits happen at times `b[0] + k*L, b[1] + k*L, ...`. At the `i`-th visit in period `k`, the probability that we have survived all previous visits (including the same period’s earlier visits) is `q^{k*n + i}`. The probability that we are caught at that visit is `p` times that survival probability. So the expected time is:
//
// `E = sum_{k>=0} sum_{i=0}^{n-1} (b[i] + k*L) * p * q^{k*n + i}`.
//
// Factor out `p`:
//
// `E = p * sum_{i=0}^{n-1} q^i * ( b[i] * sum_{k>=0} q^{k*n} + L * sum_{k>=0} k * q^{k*n} )`.
//
// Let `A = q^n`. Then `sum_{k>=0} A^k = 1/(1-A)`, and `sum_{k>=0} k*A^k = A/(1-A)^2`. So:
//
// `E = p * ( sum_{i=0}^{n-1} q^i * b[i] ) / (1 - q^n) + p * L * ( sum_{i=0}^{n-1} q^i ) * ( q^n / (1 - q^n)^2 )`.
//
// This matches the snippet’s computation: they define `pn = (1-p)^n`, `r1 = 1/(1-pn)`, `r2 = ...`, and then `ans += q * b[i] * r1` and `ans += q * r2` where `q = p * (1-p)^i`. Yes, that is exactly the formula. So the solution is to: compute the period `L`, simulate the point’s path for one period to find all times `t` in `[0, L)` where the position equals `(x2,y2)`. Since the path is periodic with period `L`, we only need to check `t=0..L-1`. The position at time `t` can be computed directly using reflection. For a 1D coordinate `x` with range `[0, h-1]`, after `t` steps, the position is `x = x1 + (-1)^floor(t/(h-1)) * (t mod (h-1))`? Actually the standard formula for a bouncing ball: after `t` steps, the position along one dimension (length `h` cells, distance between opposite walls is `h-1`) is given by `pos = (x1 + t) mod (2*(h-1))`, then if `pos > h-1`, `pos = 2*(h-1) - pos`. This works for both x and y independently. So we can iterate `t` from 0 to `L-1` and check both coordinates. However, `L` can be up to `2*(h-1)*(w-1)` which is at most about `2*10^10` if `h,w` up to `10^5`? The problem statement doesn’t give constraints, but we must be efficient. The snippet uses a clever approach: it computes the set of times at which the point coincides with the target by solving the modular equations. For each dimension, the times when the x-coordinate equals `x2` form an arithmetic progression with period `2*(h-1)`. Specifically, `x(t) = x2` if either `t ≡ (x2 - x1) mod 2*(h-1)` or `t ≡ (2*(h-1) - x1 - x2) mod 2*(h-1)`. Similarly for y. The intersection gives times when both coordinates match. The snippet enumerates all such times up to `L` by merging these progressions. That is clever and efficient. However, for a standalone teaching task, we can simplify: since the task is to write a function, we can simulate the path for one period directly. The period `L` can be large, but we can instead derive the times mathematically. But to keep the solution straightforward and correct, we can iterate `t` from 0 to `L-1`; however if `L` is large (e.g., `h=w=10^5` then `L` ~ `2*10^10` which is too large). The snippet’s approach is to enumerate the intersection of two sets of arithmetic progressions. We can do similar: For x, the times `t` such that `x(t)=x2` are all integers `t` satisfying `t % (2*(h-1))` in the set `{dx1, dx2}` where `dx1 = (x2 - x1) mod (2*(h-1))` and `dx2 = (2*(h-1) - x1 - x2) mod (2*(h-1))`. Similarly for y with period `2*(w-1)`. We need `t` such that `t mod Px` in `Sx` and `t mod Py` in `Sy`, where `Px=2*(h-1)`, `Py=2*(w-1)`. Since `Px` and `Py` may share a gcd, we can use the Chinese Remainder Theorem (CRT) to find all solutions mod `lcm(Px, Py)`. Note that `lcm(Px, Py)` is exactly the period `L` because the motion repeats after `L = lcm(Px, Py)` (since both coordinates repeat). The snippet defines `lim = 2*(h-1)*(w-1)` which is the lcm only if `h-1` and `w-1` are coprime, but in general the period is `lcm(2*(h-1), 2*(w-1))` = `2*lcm(h-1,w-1)`. However the snippet uses `lim = 2*(h-1)*(w-1)` and then sets `a[i]` for `i < lim` using step sizes `(h-1)*2` and similar, but that overestimates the period? Actually they use `lim` as the product, but they set `a[i]` for `i < lim` and then later they compute `r2` multiplied by `lim`. Let’s re-check the snippet: they set `lim = 2*(h-1)*(w-1)`. They then for each dimension, they mark `a[i]` for `i` from the starting offsets, stepping by `(h-1)*2` and also by `2*(h-1)` (same). For x, the step is `2*(h-1)`. For y, after swapping, the step is `2*(w-1)`. So they mark all `i` that are congruent to one of the residues mod `2*(h-1)` and also congruent to one of the residues mod `2*(w-1)`. Since `lim` is a common multiple of both `2*(h-1)` and `2*(w-1)`, they will mark all times in `[0, lim)` that satisfy both congruences. That is a superset of the actual period if `lcm` divides `lim`; but the actual period is `lcm(2*(h-1), 2*(w-1))` which divides `lim` because `lim` is a multiple of both. So marking up to `lim` will include all distinct residues modulo the true period, but also duplicate each residue `lim / true_period` times. That would cause double counting. However the snippet does not deduplicate; instead it uses the full `lim` in the expected value formula? Let’s examine: they define `lim` as the period they use, and they treat the visitation pattern as repeating with period `lim`. But if the true period is smaller, then the same visit times appear multiple times within `lim`. That would inflate the expected time because they would count each visit multiple times. But the snippet uses `r2 *= lim` to account for something. Actually, let’s look at the snippet’s `r2` computation: `r2 = pn - pn * pn * (pn - 2) / (pn - 1) / (pn - 1); r2 *= lim;` That is strange. Maybe they are using a formula that assumes the visits are at times that are multiples of something? Wait, they might be using a different derivation: perhaps they assume that the point is equally likely to be at any cell? No. Let’s step back. The snippet might be from a competitive programming problem where the target is always reachable, and they use a formula involving `lim` as the number of cells? Actually the snippet’s `r2` is used in `ans += q * r2;` where `q = p * power(1-p, i)` for each `i`. That suggests they are summing over the `n` visit times `b[i]` an additional term that depends only on `lim`. Possibly they are using the fact that the sum of `b[i]` over a period has a closed form? Let’s compute: For a full period, the point visits each cell? Actually the path visits every cell? Not necessarily. But the expected time might be expressible without enumerating all visits? The snippet appears to enumerate visits in `b`, but then uses a formula for the sum of `b[i]` and `i`? Hmm.
//
// Given the complexity, for the teaching task, we should simplify the problem to a more tractable version that still captures the essence. We can state: Given the periodic path, we assume the period is `L = 2*(h-1)*(w-1)`. This is a common multiple, but not necessarily the minimal period. To make the problem well-defined, we can define that the interval from 0 to `L-1` contains all distinct visit times exactly once (by definition we take the minimal period). However, if we use `L` as the product, we might have duplicates. To avoid that, we can either use `L = lcm(2*(h-1), 2*(w-1))` or we can simulate the path for exactly one minimal period. For a self-contained task, we can set constraints such that `h-1` and `w-1` are coprime, making `L = 2*(h-1)*(w-1)`. Or we can just implement a direct simulation for `t` from 0 to `L-1` where we compute the position using the reflection formula. But if `h,w` can be up to, say, 1000, then `L` up to ~2*10^6 which is fine. We can specify constraints that `h,w <= 200`, so `L <= 2*199*199 ≈ 79202` which is small. That allows a simple simulation and building the list `b`. Then the expected value formula can be implemented directly using modular arithmetic with the given `mod = 1e9+7`. So the task is: write a function `expected_time` that takes `h,w,x1,y1,x2,y2,P` and returns the expected time as a modular integer. The function should compute the period `L` (as `2*(h-1)*(w-1)` but we can also compute the minimal period via lcm to avoid duplicates). For simplicity, we can just simulate for `t` from 0 to `L-1` and collect times where position equals target, but we must ensure we only include each time once. If we take `L = lcm(2*(h-1), 2*(w-1))`, then the path repeats exactly with period `L`. So we compute `Lx = 2*(h-1)`, `Ly = 2*(w-1)`, `L = lcm(Lx, Ly)`. Then iterate `t` from 0 to `L-1`, compute `x(t)` and `y(t)` using the reflection formula, and if they match the target, append `t` to vector `b`. After that, `n = b.size()`. Then apply the expectation formula:
//
// Let `p = P/100` as a modular rational. `q = 1 - p`. `A = q^n`. `E = p * ( sum_{i=0}^{n-1} q^i * b[i] ) / (1 - A) + p * L * ( sum_{i=0}^{n-1} q^i ) * ( A / (1 - A)^2 )`.
//
// We compute all in modular arithmetic using the `modular` template or just using `long long` with modular inverses. Since we output code without main, we can use a simple modular exponentiation and modular inverse function. We must handle the case when `n=0`: that would mean the target is never visited. But the problem likely guarantees it is visited at least once, or else the expected time is infinite. For the task, we can assume `n >= 1`. We also need to handle `p=0` (P=0) leading to infinite expectation, but maybe constraints avoid that. We can assert `P>0`. Edge cases: if `p=1` (P=100), then catch happens at first visit, expectation is `b[0]`. Formula should still work because `q=0`, `A=0`, `1/(1-A)=1`, etc. Check: `E = p * sum_{i=0}^{n-1} q^i * b[i]` with `q=0` gives only `i=0` term, so `E = b[0]`. That’s correct. Also need to handle modular division by `1-A` which could be zero if `A=1`, i.e., `q=1` (p=0) or `n` infinite? If p=0, then q=1, A=1, denominator zero, but expectation is infinite. So we can require `P>0`. Also if `n` is large but finite, `A` is less than 1, so denominator non-zero.
//
// For the test, we can write a few assertions with small grids that can be verified manually. For example, a 2x2 grid with start and target same corner, P=50. The period `Lx=2`, `Ly=2`, `L=2`. The path at t=0: (0,0); t=1: (1,1) if start at (0,0); t=2 back to (0,0). So target (0,0) is visited at t=0,2,4,... So `b = [0]` (only t=0 within period since t=2 mod 2 is 0). So n=1. p=1/2, q=1/2. A=(1/2)^1=1/2. E = p*(q^0 * b[0])/(1-A) + p*L*(q^0)*(A/(1-A)^2) = (1/2 * 0) / (1/2) + (1/2 * 2 * (1/2) * ( (1/2)/(1/4) ) ) = 0 + (1/2 * 2 * 1/2 * 2) = (1 * 1/2 * 2) = 1. So expected time is 1. Let's verify manually: At t=0, with prob 1/2 caught, stop with time 0. If not caught (prob 1/2), continue to t=1, at t=1 point is not at target, cannot be caught. t=2: at target, with prob 1/2 catch, but conditional on survival to t=2 (prob 1/2 from t=0, and no catch possible at t=1), so actual prob of catch at t=2 is 1/4, time 2. At t=4 catch prob 1/8, time 4, etc. Expected time = 0*1/2 + 2*1/4 + 4*1/8 + ... = sum_{k>=1} 2k * (1/2)^{k+1}? Actually at t=2k (k>=1), prob = (1/2)^k * 1/2? Let's formal: survival through t=0,2,...,2k-2 is (1/2)^k, then catch at 2k with prob 1/2, so prob = (1/2)^{k+1}? Wait at t=0: prob catch = 1/2, time 0. At t=2: need survive t=0 (prob 1/2) and then at t=2 catch prob 1/2, so prob = 1/4, time 2. At t=4: survive t=0 and t=2 (prob 1/4) then catch prob 1/2 = 1/8, time 4. So expected = 0*1/2 + 2*1/4 + 4*1/8 + 6*1/16 + ... = sum_{k=1}∞ (2k) * (1/2)^{k+1} = (1/2) * sum_{k=1}∞ k * (1/2)^k? Actually (2k)*(1/2)^{k+1} = k * (1/2)^k. Sum_{k=1}∞ k*(1/2)^k = 2 (since sum k r^k = r/(1-r)^2 with r=1/2 gives 2). So expectation = 2? That contradicts 1. Wait a discrepancy. Let's recalc probability: At t=0, catch prob 1/2. If not caught, the point moves to (1,1) at t=1, not target. At t=2 back to (0,0). But the point has been at (0,0) at t=0 and then again at t=2. At t=0, if not caught, we continue. At t=2, the point is at target again, and there is a fresh Bernoulli trial with prob 1/2. The probability of reaching t=2 without being caught is (1-1/2) = 1/2, because the only chance to be caught is at t=0. At t=2, catch prob 1/2, so probability of catch at t=2 is 1/4. Similarly, probability of reaching t=4 is (1-1/2)*(1-1/2) = 1/4? Wait, from t=0 to t=2, you must survive t=0 trial (prob 1/2). Then at t=2, you might get caught (prob 1/2) or survive (prob 1/2). To reach t=4, you must survive both t=0 and t=2, so prob = (1/2)^2 = 1/4. Then catch at t=4 with prob 1/2, so probability of catch at t=4 is 1/8. So pattern: catch at t=2k for k>=1 has prob (1/2)^k * (1/2) = (1/2)^{k+1}? Wait for k=1 (t=2): prob = 1/4, which is (1/2)^2 = (1/2)^{1+1}. For k=2 (t=4): prob = 1/8 = (1/2)^3. So prob = (1/2)^{k+1}. Time = 2k. Expected = sum_{k=1}∞ 2k * (1/2)^{k+1} = sum_{k=1}∞ k * (1/2)^k = 2. So expectation is 2, not 1. Let's see our formula: n=1, b[0]=0, L=2, p=1/2, q=1/2, A=q=1/2. E = p * (q^0 * b[0]) / (1-A) + p*L*(q^0)*(A/(1-A)^2) = (1/2 * 0) / (1/2) + (1/2 * 2 * 1 * ( (1/2)/(1/4) )) = 0 + (1 * 1 * ( (1/2)*4 )) = 1 * 2 = 2. Yes, I made an arithmetic mistake earlier: (1/2 * 2 = 1) and then multiply by 1/2? Wait compute: p*L = 1/2 * 2 = 1. q^0 = 1. A/(1-A)^2 = (1/2)/(1/4) = 2. So product = 1 * 1 * 2 = 2. So correct. Good.
//
// So we can test with that.
//
// Now, we need to write a self-contained solution. Since the task asks for a free function without main, we can implement modular arithmetic in a simple way using `long long` and helper functions. We'll implement `pow_mod`, `inv_mod`, and the expected value function. The function signature could be: `long long expected_time(int h, int w, int x1, int y1, int x2, int y2, int P)`. We'll compute the minimal period `L` as lcm of `2*(h-1)` and `2*(w-1)`. Then iterate t from 0 to L-1, compute x(t) and y(t) using reflection. Use `t % (2*(h-1))` and similarly for y. For a coordinate length `n` (number of cells), the period of that coordinate is `2*(n-1)`. The position after `t` steps: `pos = (start + t) % (2*(n-1))`; if `pos >= n`, then `pos = 2*(n-1) - pos`. That gives correct coordinate. We collect times when both match. Then compute `n_visits = b.size()`. Then compute `p = P * inv(100)`, `q = 1 - p`. Compute `q^i` iteratively. Compute `sum1 = sum_{i} q^i * b[i]`, `sum2 = sum_{i} q^i`. Compute `A = q^n_visits`. Compute `E = p * ( sum1 / (1-A) + L * sum2 * A / ((1-A)^2) )`. All modulo mod. Return `E`. Need to handle modular inverses for `(1-A)` and its square. Since mod is prime, use Fermat’s little theorem. Ensure all operations are non-negative.
//
// We need to be careful: `p` and `q` are modular values, and `q` should be computed as `(1 - p + mod) % mod`. Also `P` could be 0, but then `p=0`, `q=1`, `A=1`, denominator zero. So we can assert `P > 0` or handle by returning 0? Actually if P=0, expected time is infinite, but the problem might guarantee P>0. For safety, we can return an error? Since it's a function, we can assume valid input. In tests we will use P>0.
//
// Time complexity: O(L + n) where L is lcm of periods, which could be up to about `2*(h-1)*(w-1)/gcd` – if h,w up to 200, L up to ~80k, fine. Space O(n) for b.
//
// Now write the solution code. We'll include necessary headers and use `long long` for modular arithmetic. We'll define constants and helper functions.

#include <bits/stdc++.h>
using namespace std;

const long long MOD = 1000000007LL;

long long modpow(long long a, long long e) {
    long long res = 1;
    while (e > 0) {
        if (e & 1) res = res * a % MOD;
        a = a * a % MOD;
        e >>= 1;
    }
    return res;
}

long long modinv(long long a) {
    return modpow(a, MOD - 2);
}

// Returns the expected time until the point is caught, modulo MOD.
// The point starts at (x1,y1) and bounces diagonally inside a rectangle of height h and width w.
// At each integer time when the point is exactly at (x2,y2), it is caught with probability P/100.
// h,w >= 2, coordinates are 0-indexed, P is an integer 0 < P <= 100.
long long expected_time(int h, int w, int x1, int y1, int x2, int y2, int P) {
    int px = 2 * (h - 1);
    int py = 2 * (w - 1);
    // The period of the full path is lcm(px, py)
    int L = std::lcm(px, py);  // needs C++17
  
    auto position = [&](int t, int size, int start) {
        int period = 2 * (size - 1);
        int pos = (start + t) % period;
        if (pos >= size) pos = period - pos;
        return pos;
    };
  
    vector<int> visits;
    for (int t = 0; t < L; ++t) {
        int x = position(t, h, x1);
        int y = position(t, w, y1);
        if (x == x2 && y == y2) {
            visits.push_back(t);
        }
    }
  
    int n = (int)visits.size();
    // assume n >= 1
    long long p = P * modinv(100) % MOD;
    long long q = (1 - p + MOD) % MOD;
  
    long long sum1 = 0; // sum q^i * b[i]
    long long sum2 = 0; // sum q^i
    long long qpow = 1;
    for (int i = 0; i < n; ++i) {
        sum1 = (sum1 + qpow * visits[i]) % MOD;
        sum2 = (sum2 + qpow) % MOD;
        qpow = qpow * q % MOD;
    }
  
    long long A = modpow(q, n); // q^n
    long long denom = (1 - A + MOD) % MOD;
    long long inv_denom = modinv(denom);
    long long inv_denom_sq = inv_denom * inv_denom % MOD;
  
    long long term1 = sum1 * inv_denom % MOD;
    long long term2 = L % MOD;
    term2 = term2 * sum2 % MOD;
    term2 = term2 * A % MOD;
    term2 = term2 * inv_denom_sq % MOD;
  
    long long ans = (term1 + term2) % MOD;
    ans = ans * p % MOD;
    return ans;
}

#include <cassert>
#include <numeric>
// Assume expected_time function is defined above.

int main() {
    // 2x2 grid, start and target both at (0,0), P=50 -> expected time = 2
    assert(expected_time(2, 2, 0, 0, 0, 0, 50) == 2);
  
    // 2x2 grid, start (0,0), target (1,1), P=25. 
    // Period L=2. visits at t=1 only (since (0,0) at 0, (1,1) at 1). n=1, b[0]=1.
    // p=1/4, q=3/4, A=3/4. 
    // E = p*(q^0*1)/(1-A) + p*2*(q^0)*A/(1-A)^2
    // = (1/4)*1/(1/4) + (1/4)*2*(3/4)/(1/16) = 1 + (1/4)*2*(3/4)*16 = 1 + (1/4)*2*12 = 1+6=7? Let's compute carefully: (1/4)*1 / (1/4) = 1. Second: p*L = (1/4)*2=1/2, *A=3/4 → 3/8, /(1/16)= (3/8)*16=6. Sum=7. So expected 7.
    assert(expected_time(2, 2, 0, 0, 1, 1, 25) == 7);
  
    // 3x3 grid, start (0,0), target (2,2), P=50.
    // px=4, py=4, L=4. Path: t=0:(0,0),1:(1,1),2:(2,2),3:(1,1) actually diagonal bounce: 
    // from (0,0) to (1,1) to (2,2) then bounce to (1,1)?? Wait in a 3x3, diagonal from (0,0) has direction (+1,+1). 
    // after t=1: (1,1), t=2: (2,2) then it hits corner, bounces both components: direction becomes (-1,-1), so t=3: (1,1), t=4: (0,0). 
    // So visits to (2,2) at t=2 only within period L=4. n=1, b[0]=2. p=1/2, q=1/2, A=1/2. E = p*2/(1/2) + p*4*1*(1/2)/(1/4) = (1*2?) Actually p=1/2, so term1 = (1/2)*2/(1/2)=2. term2 = (1/2)*4*(1/2)/(1/4) = (1/2*4*1/2)=1, *4 = 4? Let's compute: p*L = 2, *A=1/2 → 1, *(1/(1/4))=4 → 4. Sum=6. So expected 6.
    assert(expected_time(3, 3, 0, 0, 2, 2, 50) == 6);
  
    // 2x3 grid, start (0,1), target (1,1), P=100 (must catch at first visit).
    // px=2, py=4, L=lcm(2,4)=4. t=0:(0,1), t=1:(1,2)?? Actually width=3, so y coordinate from 1 goes to 2 at t=1? Let's compute: x moves 0->1, y moves 1->2 -> (1,2) not target. t=2: x moves back to 0, y bounces from 2 to 1 -> (0,1). t=3: (1,0). t=4 back to (0,1). So target (1,1) never visited? Actually y=1 occurs at t=0 and t=2, but x=1 only at t=1 and t=3, so never simultaneously. So n=0. But we assume n>=1. So this test would fail. So we avoid.
  
    // Instead, choose a target that is visited. For 2x3, start (0,0), target (1,1)? 
    // px=2, py=4, L=4. t=0:(0,0), t=1:(1,1) yes target. t=2:(0,2)?? Wait y from 0->1->2->1->0, so t=2: (0,2) because x flips from 1 to 0, y=2. t=3:(1,1). So visits at t=1 and t=3 within period. n=2, b=[1,3]. With P=100, expected time = b[0] = 1. Let's test that.
    assert(expected_time(2, 3, 0, 0, 1, 1, 100) == 1);
  
    // Another test with multiple visits and P not 100: 2x3, start (0,0), target (1,1), P=50.
    // We need to compute manually? Let's trust the formula.
    // Use a brute-force simulation for a few periods to verify? But for assert we just compute via our function and maybe a direct numeric calculation with floating point? 
    // Simpler: use known result for small case. Let's compute by hand for P=50, visits at 1 and 3.
    // p=1/2, q=1/2, n=2, A=q^2=1/4.
    // sum1 = q^0*1 + q^1*3 = 1*1 + (1/2)*3 = 1 + 1.5 = 2.5
    // sum2 = 1 + 1/2 = 1.5
    // term1 = p * sum1 / (1-A) = (1/2)*2.5 / (3/4) = 1.25 / 0.75 = 5/3 ≈ 1.6667
    // term2 = p * L * sum2 * A / (1-A)^2 = (1/2)*4*1.5*(1/4) / (9/16) = (2*1.5*0.25) / (9/16) = 0.75 / 0.5625 = 4/3 ≈ 1.3333
    // total = 5/3 + 4/3 = 9/3 = 3. So expected = 3.
    assert(expected_time(2, 3, 0, 0, 1, 1, 50) == 3);
  
    // Test with start == target and P=100, expected 0.
    assert(expected_time(2, 2, 0, 0, 0, 0, 100) == 0);
  
    return 0;
}
