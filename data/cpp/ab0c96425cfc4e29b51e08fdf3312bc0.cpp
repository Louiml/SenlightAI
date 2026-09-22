Write a standalone C++ function named `sieveStateAfterSteps` that simulates a simplified version of the number-theoretic sieve used in the provided code snippet. The function takes an initial odd starting value `G` (unsigned long long), an initial odd divisor `p` (unsigned long long, starting at 3), and a number of steps `numSteps` (unsigned long long). Each step performs the following operations in order: compute `q = G / p` and `rest = G % p`; if `rest == 0` OR `p > q`, then set `p = 3` and `G += 2`; otherwise, set `p += 2`. The function must return a struct `SieveResult` containing the final `G`, `p`, `q`, `rest`, and a boolean `primeFound` that is true if and only if during any step the condition `rest != 0 && p > q` was true (meaning the current `G` is prime and we moved to the next candidate). Implement the function with appropriate `const` correctness (no mutation of inputs, return by value). The input `G` is guaranteed to be odd and greater than or equal to 3, `p` is guaranteed to be odd and greater than or equal to 3, and `numSteps` is at least 1. Handle the case where `p > G` initially (then the first step will trigger the primeFound condition if rest != 0). The function should not allocate dynamic memory beyond the returned struct.

#include <cassert>

int main() {
    // Test 1: Simple increment, no reset.
    SieveResult r1 = sieveStateAfterSteps(9, 3, 1); // 9/3 = 3, rest=0 -> reset p=3, G=11
    assert(r1.G == 11 && r1.p == 3 && r1.q == 3 && r1.rest == 2 && r1.primeFound == false);

    // Test 2: Prime found when p > q and rest != 0. Initial G=11, p=3: 11/3=3, rest=2, p>q (3>3 is false? Actually 3>3 is false, so p+=2 -> p=5). Step 2: 11/5=2, rest=1, p>q (5>2 true), rest != 0 -> primeFound=true, reset p=3, G=13.
    SieveResult r2 = sieveStateAfterSteps(11, 3, 2);
    assert(r2.G == 13 && r2.p == 3 && r2.q == 4 && r2.rest == 1 && r2.primeFound == true);

    // Test 3: p greater than G initially. G=5, p=7: 5/7=0, rest=5, p>q (7>0 true), rest!=0 -> primeFound=true, reset p=3, G=7.
    SieveResult r3 = sieveStateAfterSteps(5, 7, 1);
    assert(r3.G == 7 && r3.p == 3 && r3.q == 2 && r3.rest == 1 && r3.primeFound == true);

    // Test 4: Multiple steps with resets.
    SieveResult r4 = sieveStateAfterSteps(9, 3, 2); // step1: G=11,p=3; step2: 11/3=3 rest=2 -> p+=2 => p=5. final recompute: 11/5=2 rest=1.
    assert(r4.G == 11 && r4.p == 5 && r4.q == 2 && r4.rest == 1 && r4.primeFound == false);

    // Test 5: Large numbers, just check no overflow and values.
    SieveResult r5 = sieveStateAfterSteps(1000000007ULL, 3, 1); // 1000000007/3=333333335, rest=2 -> p+=2 => p=5
    assert(r5.G == 1000000007ULL && r5.p == 5 && r5.q == 200000001ULL && r5.rest == 2 && r5.primeFound == false);

    // Test 6: Exact division resets and does not mark prime.
    SieveResult r6 = sieveStateAfterSteps(15, 3, 1); // 15/3=5 rest=0 -> reset, primeFound false, G=17,p=3
    assert(r6.G == 17 && r6.p == 3 && r6.q == 5 && r6.rest == 2 && r6.primeFound == false);

    // Test 7: primeFound should be true if any step had rest!=0 and p>q, even if later steps happen.
    SieveResult r7 = sieveStateAfterSteps(11, 3, 3); // step1: p=5; step2: primeFound, reset to G=13,p=3; step3: 13/3=4 rest=1, p=3 not > q (3>4 false) -> p=5. final: G=13,p=5,q=2,rest=3.
    assert(r7.primeFound == true && r7.G == 13 && r7.p == 5 && r7.q == 2 && r7.rest == 3);
}

#include <cstdint>

struct SieveResult {
    unsigned long long G;
    unsigned long long p;
    unsigned long long q;
    unsigned long long rest;
    bool primeFound;
};

// Simulate numSteps iterations of the sieve step.
// G and p are odd, G >= 3, p >= 3, numSteps >= 1.
SieveResult sieveStateAfterSteps(unsigned long long G, unsigned long long p, unsigned long long numSteps) {
    SieveResult result;
    result.G = G;
    result.p = p;
    result.primeFound = false;

    for (unsigned long long step = 0; step < numSteps; ++step) {
        result.q = result.G / result.p;
        result.rest = result.G % result.p;

        if (result.rest == 0 || result.p > result.q) {
            if (result.rest != 0 && result.p > result.q) {
                result.primeFound = true;
            }
            result.p = 3;
            result.G += 2;
        } else {
            result.p += 2;
        }
    }

    // Recompute q and rest for the final state after the loop? 
    // The snippet's state after a step does not recompute q/rest until the next step.
    // However, the task requires the final q and rest. We compute them based on the final G and p.
    // Note: after the loop, the state's q and rest are from the last computed step,
    // but if we reset p and increment G, they are stale. To match the semantics of 
    // "final G, p, q, rest", we recompute q and rest from the final G and p.
    result.q = result.G / result.p;
    result.rest = result.G % result.p;

    return result;
}

// The algorithm directly translates the step logic from the snippet into a loop. For each step, we compute integer division and modulo. The main decision is whether to reset `p` to 3 and increment `G` by 2 (when `rest == 0` or `p > q`) or just increment `p` by 2. The prime-found condition is a special case: when `rest != 0` and `p > q`, this means `p` is greater than the square root of `G` (since `q = floor(G/p)`) and no divisor was found up to `p`, so `G` is prime. We set `primeFound = true` and then treat it like the reset case (since the condition `p > q` is true). Edge cases include: if `p` is already greater than `G` (then `q` = 0, `rest` = `G`), the condition `p > q` is true; if `rest == 0`, it means `p` divides `G`. Time complexity is O(numSteps) because each step is constant-time arithmetic. Space complexity is O(1) auxiliary plus the returned struct.
