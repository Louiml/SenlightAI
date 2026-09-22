// Write a C++ function that takes two integers `n` and `k` as input, where `n` is a non-negative integer (the number of Fibonacci terms to compute) and `k` is a positive integer (the modulus). The function should output the first `n` Fibonacci numbers (starting with F0 = 0, F1 = 1) modulo `k`, each on a separate line, exactly as the provided snippet does. The function must handle the edge case where `n` is 0 or 1 correctly, printing only the appropriate initial terms. Do not read from standard input or print inside the function; instead, return a `std::vector<int>` containing the required mod values in order, and let the caller handle output. Use 64-bit arithmetic internally to avoid overflow when computing Fibonacci terms before taking modulo.
// The algorithm is straightforward: initialize F0 = 0 and F1 = 1. For the first two terms (if n ≥ 1 and n ≥ 2 respectively), push F0 % k and then F1 % k, but the snippet only prints F0 % k initially, then enters a loop that runs while n ≥ 2, printing F % k each time. Note the snippet uses a loop condition `while(n>=2)` and decrements `n` each iteration, which effectively prints exactly `n` terms total (since initial print counts as term 0). The key is to iterate `n` times total: for i from 0 to n-1, print the current Fibonacci modulo k. The recurrence F = F0 + F1 (with F1 = F0, F0 = F) needs to be updated carefully after each output. Use `long long` for F0, F1, F to avoid overflow for large n (since Fibonacci grows exponentially but we take modulo after addition, but the addition itself could overflow if not using 64-bit, especially if n is large but k is small; actually since we take modulo each step, we can reduce each term modulo k before next addition if we care, but the snippet doesn't, so to match behavior exactly we compute the raw Fibonacci then mod. However, n can be large enough that raw Fibonacci exceeds even 64-bit? Actually Fibonacci numbers grow exponentially, F(93) ≈ 1.2e19 which fits in unsigned 64-bit, but F(94) > 1.8e19 overflows signed 64-bit. So if n can be larger than 93, raw computation overflows. To be safe, we can compute modulo k at each step using the fact that (a+b)%k = ((a%k)+(b%k))%k. But that changes behavior if k is 0? k is positive. The original snippet does raw addition without modulo reduction before printing F%k, so it may overflow for large n, but we are writing a robust solution. We'll do the standard approach: maintain F0 and F1 as long long, but reduce modulo k after each addition? No, to mimic overflow behavior we'd need to let it overflow, but better to define the task as "compute Fibonacci numbers modulo k" which is usually defined with modular arithmetic, so we can reduce each term modulo k each step. The snippet prints F0%k initially, then computes F=F0+F1 (raw), prints F%k, then shifts. For n large, this overflows, but since the task is to recreate the snippet's output exactly for all n, we need to replicate that overflow? That's undefined behavior. So we should assume n is small enough (or we can use __int128 or handle overflow by noting that modulo operation after overflow is implementation-defined). To be safe and correct, we'll implement the standard modular Fibonacci: F0=0, F1=1, then for each term, print F0%k, then compute next = (F0 + F1) % k, then shift. But note the snippet prints F0%k first, then for n>=2 it prints F%k where F=F0+F1 (raw). If we do raw addition, for n=2 and k large, it's fine. But to handle arbitrary n, we'll do modular addition to avoid overflow, which yields same result as long as no overflow in original raw addition. Since the problem is to replicate the output, and overflow would cause undefined behavior anyway, the best practice is to use modular reduction each step. So we'll produce: sequence: term0 = 0 % k, term1 = 1 % k, term2 = (term0+term1)%k, etc. The original snippet does: prints 0%k, then for n>=2 prints (F0+F1)%k where F0=0,F1=1 initially, so term2 = (0+1)%k = 1%k. Then shifts: F1 = F0 (which was 0), F0 = F (which was 1), so next iteration F0=1, F1=0, F = 1+0=1 again? Wait careful: original snippet's shifting is wrong? Let's trace: F0=0, F1=1, n=3. Print 0%k. Loop: while(n>=2) -> n=3, F = 0+1 =1, print 1%k, then F1 = F0 (0), F0 = F (1). n-- ->2. Loop: n>=2, F = F0+F1 = 1+0 =1, print 1%k, then F1 = F0 (1), F0 = F (1). n-- ->1. Loop ends. So it prints 0,1,1. That's correct Fibonacci mod k. But note after first iteration F1 becomes 0, F0 becomes 1, then next F = 1+0=1, then shift: F1=1, F0=1. Good. So it works. So we can implement: vector<int> result; if (n>=1) push 0%k; if (n>=2) push 1%k; then for i from 2 to n-1, compute next = (F0+F1)%k, push, then F1 = F0, F0 = next. But careful with initial state: For n=1, output only 0%k. For n=2, output 0%k and 1%k. For n>=3, after that we compute. But the snippet does it differently: it prints 0%k, then loops n-1 times printing F%k. So we can do: result.push_back(0%k); if n>=2: result.push_back(1%k); then for i=2; i<n; i++: compute next = (prev + current)%k, push, then prev = current, current = next. But we need to set up correctly. Simpler: maintain two variables a=0, b=1. For i=0 to n-1: if i==0 push a%k; else if i==1 push b%k; else compute c = (a+b)%k, push c, then a=b, b=c. But to match the snippet's exact behavior, we can just do iterative: vector<int> res; long long f0=0, f1=1; if n>=1 res.push_back(f0%k); for(int i=2; i<=n; i++) { long long f = (f0+f1)%k; res.push_back(f%k); f1 = f0; f0 = f; } But careful: This logic gives for n=1 -> only 0. For n=2 -> after initial push 0, i=2: f=0+1=1, push 1, shift. So output 0,1. For n=3: initial 0, i=2 push 1, shift (f1=0, f0=1), i=3: f=1+0=1, push 1 -> 0,1,1. Works. So complexity O(n) time, O(n) space for the vector (or O(1) if we output directly, but we return vector per spec). Edge cases: n=0 -> return empty vector (since snippet prints nothing if n<1? Actually snippet reads n, then prints F0%k regardless, even if n=0? Let's check snippet: it prints F0%k always, then while(n>=2) loop. So if n=0, it still prints 0%k. But the task says "first n Fibonacci numbers" — so for n=0, output nothing? The snippet prints one term even for n=0, which is odd. But the task says "takes two integers n and k ... output the first n Fibonacci numbers". So we should follow the task specification, not the snippet's bug. We'll define that for n=0, return empty vector. For n=1, return {0%k}. For n>=2, return the sequence. The snippet actually outputs n terms? Let's test: n=0 -> prints 0%k (1 term). n=1 -> prints 0%k, loop condition n>=2 false, so only that (1 term). n=2 -> prints 0%k, then loop: n=2, print F%k=1%k, n-- ->1, loop ends, so 2 terms. n=3 -> prints 0, then loop runs twice (n=3->2, then n=2->1) prints two F's, total 3 terms. So snippet prints n terms when n>=1, but for n=0 it prints 1 term. We'll adopt the more logical behavior: n terms, with n=0 returning empty. If we want to be faithful, we'd handle n=0 as printing 0, but the task says "first n", so we'll define properly. To avoid ambiguity, we'll state in the task that n is non-negative, and if n=0, the function returns an empty vector. Otherwise it returns the first n Fibonacci numbers modulo k.
//
// Time: O(n). Space: O(n) for result vector, but we can store and return that, so that's necessary. If we were to output directly, O(1) extra space, but we return vector.
#include <vector>
#include <cstdint>

// Return the first n Fibonacci numbers modulo k, starting with F0=0 and F1=1.
// For n=0, returns an empty vector. k must be positive.
std::vector<int> fibonacciModuloN(long long n, int k) {
    std::vector<int> result;
    if (n <= 0) return result;

    result.push_back(0 % k);  // F0 % k
    if (n == 1) return result;

    result.push_back(1 % k);  // F1 % k
    if (n == 2) return result;

    long long f0 = 0, f1 = 1;
    for (long long i = 2; i < n; ++i) {
        long long next = (f0 + f1) % k;  // compute Fibonacci modulo k to avoid overflow
        result.push_back(static_cast<int>(next % k));
        f1 = f0;
        f0 = next;
    }
    return result;
}
#include <cassert>
#include <vector>

// (The solution function is included above for completeness in test context.)

int main() {
    // n=0 -> empty vector
    assert(fibonacciModuloN(0, 5) == std::vector<int>{});
    
    // n=1 -> only F0 mod k
    assert(fibonacciModuloN(1, 5) == std::vector<int>{0});
    
    // n=2 -> F0, F1
    assert(fibonacciModuloN(2, 5) == std::vector<int>{0, 1});
    
    // n=3 -> 0,1,1
    assert(fibonacciModuloN(3, 5) == std::vector<int>{0, 1, 1});
    
    // n=5, k=5 -> 0,1,1,2,3 -> mod5 = 0,1,1,2,3
    assert(fibonacciModuloN(5, 5) == std::vector<int>{0, 1, 1, 2, 3});
    
    // n=8, k=7 -> Fibonacci: 0,1,1,2,3,5,8,13 -> mod7: 0,1,1,2,3,5,1,6
    assert(fibonacciModuloN(8, 7) == std::vector<int>{0, 1, 1, 2, 3, 5, 1, 6});
    
    // Large n with small k to test modular reduction
    auto big = fibonacciModuloN(20, 3);
    // Expected first 20 Fibonacci mod 3: 0,1,1,2,0,2,2,1,0,1,1,2,0,2,2,1,0,1,1,2
    std::vector<int> expected = {0,1,1,2,0,2,2,1,0,1,1,2,0,2,2,1,0,1,1,2};
    assert(big == expected);
    
    // Negative n treated as 0
    assert(fibonacciModuloN(-1, 2) == std::vector<int>{});
    
    // k=1 -> all zeros
    assert(fibonacciModuloN(5, 1) == std::vector<int>{0,0,0,0,0});
    
    return 0;
}
