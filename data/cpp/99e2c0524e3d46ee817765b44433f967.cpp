// Write a C++ function that, given a non-negative integer `n`, returns an `std::pair<long long int, long long int>` where the first element is the number of function calls required to compute the nth Fibonacci number using the standard naive recursive definition `F(0)=0, F(1)=1, F(n)=F(n-1)+F(n-2)` for `n≥2`, and the second element is the nth Fibonacci number itself. The function must handle `n` up to 45 efficiently by using memoization for both the Fibonacci values and the call counts. The call count for `n=0` and `n=1` is defined as 0 (since the base cases are not considered calls that recurse), and for `n=2` it is 2 (calls to `F(1)` and `F(0)`). For `n>2`, the call count is `2 + calls(n-1) + calls(n-2)`. The function must be declared as `std::pair<long long int, long long int> fibWithCalls(int n)` and must not rely on any global mutable state across calls—meaning it should be re-entrant and thread-safe for independent invocations, although internal memoization per call is acceptable.

#include <cassert>

int main() {
    // Base cases
    auto r0 = fibWithCalls(0);
    assert(r0.first == 0 && r0.second == 0);

    auto r1 = fibWithCalls(1);
    assert(r1.first == 0 && r1.second == 1);

    auto r2 = fibWithCalls(2);
    assert(r2.first == 2 && r2.second == 1);

    // Small values
    auto r3 = fibWithCalls(3);
    assert(r3.first == 4 && r3.second == 2);

    auto r4 = fibWithCalls(4);
    assert(r4.first == 8 && r4.second == 3);

    auto r5 = fibWithCalls(5);
    assert(r5.first == 14 && r5.second == 5);

    auto r6 = fibWithCalls(6);
    assert(r6.first == 24 && r6.second == 8);

    auto r7 = fibWithCalls(7);
    assert(r7.first == 40 && r7.second == 13);

    // Larger known value
    auto r10 = fibWithCalls(10);
    assert(r10.first == 176 && r10.second == 55);

    // Maximum allowed n (45) — known Fibonacci 1134903170
    auto r45 = fibWithCalls(45);
    assert(r45.first == 3672623804LL && r45.second == 1134903170LL);

    return 0;
}

#include <utility>
#include <vector>

// Returns {number_of_calls, nth_fibonacci} for input n (0 <= n <= 45).
std::pair<long long int, long long int> fibWithCalls(int n) {
    // Memoization tables, sized for maximum n=45.
    std::vector<long long int> fib(46, -1);
    std::vector<long long int> calls(46, -1);

    // Helper lambda for recursion with access to tables.
    auto solve = [&](auto&& self, int x) -> void {
        if (fib[x] != -1) return; // Already computed
        if (x == 0) {
            fib[0] = 0;
            calls[0] = 0;
            return;
        }
        if (x == 1) {
            fib[1] = 1;
            calls[1] = 0;
            return;
        }
        if (x == 2) {
            fib[2] = 1;
            calls[2] = 2;
            return;
        }
        self(self, x - 1);
        self(self, x - 2);
        fib[x] = fib[x - 1] + fib[x - 2];
        calls[x] = 2 + calls[x - 1] + calls[x - 2];
    };

    solve(solve, n);
    return {calls[n], fib[n]};
}

// The solution uses two memoization arrays of size 46 (for indices 0..45) to avoid recomputation: one for Fibonacci numbers and one for the number of calls. A helper recursive function computes both values simultaneously, filling the arrays as needed. Base cases are set directly: `fib[0]=0`, `fib[1]=1`, `calls[0]=calls[1]=0`, and `calls[2]=2` while `fib[2]=1`. For `n≥3`, recursively compute `fib` and `calls` for `n-1` and `n-2`, then set `fib[n]=fib[n-1]+fib[n-2]` and `calls[n]=2+calls[n-1]+calls[n-2]`. Edge case: `n=0` and `n=1` must return call count 0. The maximum Fibonacci value for n=45 is 1,134,903,170 which fits in `long long int`. Time complexity is O(n) per unique `n` due to memoization (each index computed once), and space complexity is O(n) for the arrays. The function returns an `std::pair` to pack both results.
