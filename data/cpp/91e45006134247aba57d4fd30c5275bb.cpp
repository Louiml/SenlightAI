// Write a standalone C++ function `int countFactorPaths(int n)` that counts the number of distinct ways to factor the integer `n` into factors greater than 1, where each factorization is considered as an ordered sequence of factors (i.e., the order matters), and the sequence must be non-decreasing (each factor is at least as large as the previous one). The function should return the total count of such sequences for a given positive integer `n` (1 ≤ n ≤ 10^6). For example, for `n = 12`, the valid sequences are: `[12]`, `[2,6]`, `[2,2,3]`, `[3,4]` — so the result is 4. For `n = 1`, the only trivial sequence is `[1]` (since 1 is a factor), so the result is 1. The function must be efficient enough to handle the maximum input without recursion depth issues (use iterative or optimized recursion) and must not use global mutable state.
The problem reduces to counting the number of ways to express `n` as a product of integers ≥ 2, with the additional constraint that the sequence is non-decreasing. This is essentially counting the number of ordered factorizations where factors are sorted. The classic recursive approach is: for a current value `a` and a lower bound `x` (the minimum allowed factor), iterate over all divisors `d` of `a` such that `d ≥ x` and `d > 1`. If `d == a`, that contributes one sequence (the sequence `[a]`). Otherwise, recurse on `a/d` with lower bound `d`. This matches the given snippet's logic. However, the snippet uses global state and a recursive function; we must adapt it to a self-contained function. Key edge cases: `n = 1` must return 1 (the empty product or `[1]`). For prime numbers, the only divisor is the number itself, so the answer is 1. The recursion depth is at most the number of prime factors (log₂ n ≈ 20 for n ≤ 10^6), so recursion is safe. The time complexity is bounded by the number of divisors visited. For `n = 10^6`, the number of divisors is at most 240, and each divisor leads to further recursion, but overall the complexity is roughly proportional to the total number of valid factor sequences, which for n up to 10^6 is manageable (worst case around a few thousand). More formally, the branching factor is the number of divisors of `a` that are ≥ current lower bound; the total work is O(d(n) * depth) in the worst case, but typically much less. Space complexity is O(depth) for recursion stack, which is O(log n).
#include <cmath>

// Count the number of non-decreasing factorizations of n into factors > 1.
int countFactorPaths(int n) {
    if (n == 1) return 1; // only the sequence [1]

    // Recursive helper: count factorizations of a with minimum factor lower.
    // The lower bound ensures non-decreasing order.
    // We use lambda for closure to avoid global state.
    // We pass a and lower by value.
    // Recursion depth is bounded by log2(n), safe.
    int result = 0;
    // Use a helper function with recursion. We need to iterate divisors.
    // We'll define a recursive lambda via std::function.
    // But to keep it simple and standalone, use a private helper function.
    // Since we cannot add a main, we provide a free helper in the same file.
    // Actually we'll implement a recursive function with parameters.
    // Let's define a static recursive function outside? But we are only allowed to output a single function.
    // So we'll implement recursion via an internal lambda.
    // But lambdas need to capture result by reference.
    // We'll use std::function for recursive lambda.
    // However, to avoid extra includes, we can use a nested function via a helper that is defined inside.
    // Simpler: use recursion with a static counter? No, avoid static.
    // We'll write a recursive lambda with std::function.
    
    // But the problem says "output code only" and "do not include a main function". 
    // We can include extra helper functions in the same solution block. 
    // So we'll add a helper function outside the main function.
    // But the instruction says "free function" - it can have helper functions.
    // So we'll define a helper with parameters.
    
    return countRecursive(n, 2);
}

// Helper: count factorizations of a with minimum factor x.
int countRecursive(int a, int x) {
    if (a == 1) return 1; // reached a complete factorization
    int count = 0;
    for (int d = x; d <= a; ++d) {
        if (a % d == 0) {
            count += countRecursive(a / d, d);
        }
    }
    return count;
}
Wait, the above helper returns count for a given a and x. For a=1, it returns 1 (the empty product). For the main call n=1, we return 1 directly. For n>1, we call countRecursive(n, 2). But careful: the loop includes d == a, which gives a factorization [a]. That's correct. However, for a=1, the loop does not run, and we return 1, which represents the empty sequence (or the sequence [1]? Actually the problem says for n=1 return 1. Our helper for a=1 returns 1, but if we call countRecursive(1,2) we get 1, which is fine. But if we call countRecursive(12,2), it will count all sequences including [12] (when d=12, a/d=1, then recursion returns 1). So the count is correct. However, we must ensure that for n=1 the function returns 1. We handle that separately. But what about the case where a=1 is reached from a division? For example, when d=a, a/d=1, recursion returns 1, counting that path. Good. So the solution is simple. But we need to avoid excessive loop iterations. For each a, we iterate d from x to a, which could be O(a) for large a, but since we only check divisibility, we can optimize by only iterating up to sqrt(a) and handling both divisors. However, to keep it simple and correct, we can iterate up to a, but for n up to 10^6, that could be slow in the worst case. For example, if n is prime, loop runs from 2 to n, which is O(n) = 10^6, acceptable. But recursion might cause multiple loops. But to be safe, we can restrict loop to d <= a, but we can also break early because if d > a? No, d goes up to a. For prime n, the only d that divides is d=n, so loop runs through all numbers from 2 to n-1 without divisibility, which is O(n) per call, but there's only one call at top, so O(n) overall, which is fine for 10^6. But if n is a power of 2, e.g., 2^20 = 1048576, the recursion might generate many calls. However, each call has a loop that goes up to a, which could be large. For example, a=2^19, loop from 2 to 2^19, but only divisors that are powers of 2 will be checked. Still, we iterate through all numbers, which is expensive. The total time could be O(n log n) in worst case, which for n=10^6 is about 20 million operations, acceptable. But we can improve by iterating only over divisors. That would be more efficient. Let's implement an optimized version: iterate d from x to sqrt(a), and if d divides a, then also consider a/d as a possible divisor (if a/d >= d to maintain non-decreasing). But careful: the sequence must be non-decreasing, so if we consider divisor d, then the next factor must be at least d, so we need to recurse on a/d with lower bound d. If we also consider the complementary divisor a/d, then the next factor would be at least a/d, but we must ensure that a/d >= d to keep order. Actually, when we iterate over d from x to sqrt(a), we find a divisor d such that d <= sqrt(a). The complementary divisor is a/d, which is >= d. But the sequence can start with d or with a/d? Wait, the first factor in the sequence must be at least x. So if we take the first factor as d (where d <= sqrt(a) and d >= x), then the remaining product is a/d, and the next factor must be >= d. That's valid. If we take the first factor as a/d (which is >= sqrt(a) >= d), then the remaining product is d, and the next factor must be >= a/d. But since d <= sqrt(a) <= a/d, the condition a/d >= x holds, but we must ensure that the next factor (which is d) is >= a/d? Actually, if we start with a/d, then the remaining product is d, and the next factor must be at least a/d. But d is smaller than a/d, so that would violate non-decreasing unless d >= a/d, which only happens when a is a perfect square and d = sqrt(a). So in general, we cannot arbitrarily choose a/d as the first factor because the remaining product d is smaller, and the next factor would have to be at least a/d, which is larger than the remaining product, so it's impossible unless the remaining product is 1. Wait, let's think: For a factorization [f1, f2, ...] with f1 >= x, and all factors non-decreasing. If we choose f1 = a/d (where d is a divisor and a/d is larger), then the remaining product is d, and we need to factor d into factors all >= a/d. But since d <= a/d (because d <= sqrt(a)), the only way is if d = 1? Then the sequence would be just [a/d] and that's valid if a/d >= x. That is the case when d=1? But d must be a divisor and we are considering divisors d >= x? Actually, the standard approach is to iterate over all divisors D of a such that D >= x and D <= a. For each such D, recurse on a/D with lower bound D. That is what we did in the simple loop. To optimize, we can note that if D > sqrt(a), then a/D < sqrt(a) < D, so the complementary divisor is smaller. But if we start with D (which is > sqrt(a)), then the remaining product is a/D < D, so the next factor must be at least D, but the remaining product is smaller than D, so the only possibility is that the remaining product is 1, i.e., a/D == 1, which means D == a. So the only divisor greater than sqrt(a) that can be used as the first factor is D == a itself. Therefore, in the loop over divisors from x to a, we really only need to consider divisors D such that D <= sqrt(a) (and D >= x) plus also D == a if a >= x. For D <= sqrt(a), the complementary divisor a/D is larger, but we cannot use it as the first factor because that would violate non-decreasing (since the next factor would be D, which is smaller than a/D). So we only need to consider D <= sqrt(a) and also D == a. So an optimized loop: iterate d from x to sqrt(a), and if a % d == 0, then recurse on a/d with lower bound d. After the loop, if a >= x (which is always true because x <= a), we add 1 for the sequence [a]? Actually, the sequence [a] is counted when we take d = a. In our loop, we don't iterate d up to a; we handle the case d == a separately. But note: If a is composite, the sequence [a] is valid. So we should add 1 for the trivial sequence [a] if a >= x (which is always true). But careful: In the recursive call, when a=1, we return 1 to count the empty product. So the total count for a given a and x is: sum over proper divisors d of a (d >= x, d < a) of countRecursive(a/d, d) plus 1 for the sequence [a] (if a >= x). But we must ensure that if a is 1, we return 1 (the empty) and not add extra. So the algorithm: 
int countRecursive(a, x) {
    if (a == 1) return 1;
    int count = 0;
    // Iterate over possible first factors d from x to sqrt(a)
    for (int d = x; d * d <= a; ++d) {
        if (a % d == 0) {
            count += countRecursive(a/d, d);
        }
    }
    // Add the sequence [a] itself if a >= x (always true)
    count += 1;
    return count;
}
But wait, this misses the case where d is larger than sqrt(a) but not equal to a. For example, a=12, sqrt≈3.46, so we iterate d from 2 to 3. Divisors: d=2 divides, count += f(6,2); d=3 divides, count += f(4,3). Then we add 1 for [12]. That gives us [2,6], [2,2,3] (from f(6,2) which yields [6] and [2,3]), [3,4] (from f(4,3) yields [4]? Actually f(4,3) yields [4] and? d from 3 to 2? sqrt(4)=2, d starts at 3, loop condition d*d <= a -> 9 <=4 false, so no loop, then add 1 for [4]. So f(4,3) returns 1, giving [3,4]. Then total: from d=2: f(6,2). f(6,2): d from 2 to sqrt(6)≈2.44, d=2 divides, count += f(3,2); then add 1 for [6]. f(3,2): d from 2 to sqrt(3)≈1.73, loop false, add 1 for [3] -> returns 1. So f(6,2) returns 1 (for [6]) + 1 (for [2,3]) = 2. Then d=3: f(4,3) returns 1. Then plus 1 for [12] gives total 1+2+1 = 4. Correct. For a=prime, e.g., 7, sqrt≈2.64, d from 2 to 2, no divisor, add 1 for [7] -> returns 1. Good. For a=1, we handle separately. So this optimized version is correct and efficient, O(sqrt(n)) per call but actually O(sqrt(a)) for each recursion, but the recursion depth is limited. For n=10^6, sqrt is 1000, so each call loops at most 1000 times. The number of calls is the number of factor sequences? Actually, the recursive calls are made only for divisors d that divide a. So the total work is roughly the sum of sqrt(a) over all visited a values, but the visited a values are the divisors chain. For worst-case n=2^20, the recursion will consider d=2 at each level, leading to a chain of depth 20, and at each level a is halved, so sqrt(a) decreases. The total sum is about sqrt(2^20)+sqrt(2^19)+...+sqrt(2) ≈ 1000+707+...+1 ≈ geometric, around 2000 operations. So very efficient. For n with many divisors, e.g., 720720, it will consider many divisors, but still manageable. So we'll use this optimized version. We also need to handle n=1: return 1. The main function `countFactorPaths` calls the recursive helper with x=2. We'll define the helper as a separate free function (not inside the solution function? The instruction says "with a descriptively named free function" - it likely means the main solution function. We can have additional helper functions. So we'll provide:

int countFactorPathsHelper(int a, int minFactor) {
    if (a == 1) return 1;
    int count = 1; // the sequence [a]
    for (int d = minFactor; d * d <= a; ++d) {
        if (a % d == 0) {
            count += countFactorPathsHelper(a / d, d);
        }
    }
    return count;
}

int countFactorPaths(int n) {
    if (n == 1) return 1;
    return countFactorPathsHelper(n, 2);
}

But careful: The loop condition `d * d <= a` might overflow if a is large, but n <= 10^6, so d <= 1000, product <= 10^6, safe. Also, we must ensure that for a composite, we count the sequence [a] correctly. Our helper returns 1 for a=1. For a>1, we start count=1 for [a]. Then for each divisor d satisfying minFactor <= d <= sqrt(a), we add the count of factorizations of a/d with lower bound d. This gives all sequences whose first factor is d (including the sequence [d, ...]). The sequence [a] is counted separately. This is correct. One subtlety: For a=12, minFactor=2, loop d=2 and 3, works. For a=4, minFactor=2, loop d=2 (2*2<=4) and d=2 divides, add f(2,2). f(2,2): a=2, loop d=2, 2*2<=2? false (4<=2 false), so no loop, count=1 for [2]. So f(2,2)=1. And a=4 adds 1 for [4], total 2. That matches sequences [4] and [2,2]. Good. For a=8, minFactor=2, loop d=2 (4<=8) divides, add f(4,2); f(4,2) returns 2 ([4] and [2,2]); plus 1 for [8] gives 3, correct sequences: [8], [2,4], [2,2,2]. Good. So solution is correct.

Now we need to provide the solution as a free function without a main. The helper can be in the same block. We'll include necessary headers: `<cmath>` for sqrt? We don't actually need sqrt, we use d*d <= a. So no header needed except maybe nothing. But we can include `<cstddef>` or just no include. However, to be safe, we can include `<cmath>` or `<cstdint>`? Not needed. We'll include nothing or just `<cstddef>`? The function uses int, which is built-in. So we can have no includes. But the instruction says "Include necessary headers" - for this simple function, no headers are needed. But to be safe, we might include `<cstdint>` or just leave empty. We'll write:

// Count non-decreasing factorizations of n (>1) into factors >1.
int countFactorPaths(int n) {
    if (n == 1) return 1;
    // Recursive helper (static? No, we'll define a separate function).
}

int countFactorPathsRecursive(int a, int minFactor) {
    if (a == 1) return 1;
    int total = 1; // the sequence [a]
    for (int d = minFactor; d * d <= a; ++d) {
        if (a % d == 0) {
            total += countFactorPathsRecursive(a / d, d);
        }
    }
    return total;
}
But we need to put the helper before the solution function. We'll put the solution function first? In C++, functions need to be declared before use. So we should define the helper first or forward declare. Let's define the helper first and then the main solution function. We'll provide both in the solution block. The main solution function is `countFactorPaths` which calls the helper. The helper is a free function too. That's acceptable.
#include <cassert>

int countFactorPaths(int n); // forward declaration for test

// We need to include the solution code in the test? The test code must call the function.
// Since the solution block is provided separately, the test code includes the header? 
// In this task, the test code is runnable on its own, so it must include the function definition.
// But the instruction says "Provide 1-10 runnable C++ assert checks inside a global main function." 
// Usually, the test code is compiled together with the solution. So we can copy the function into the test? 
// The example output shows test code that includes the function definition implicitly? Actually the example test just calls the function, assuming it's defined elsewhere. 
// So we'll write test code that includes the function definitions (by copying the solution code) or just the declarations with extern? 
// To make it runnable standalone, we need to include the solution code in the test block as well? But the instruction says exactly sections; the test section should contain the main function. 
// In the example, they only show the main function with asserts, and the solution is separate. 
// So in our test, we will have a main function with asserts, and we'll assume the solution is linked. 
// But to be self-contained, we can also include the solution code in the test block? No, the test block is separate. 
// Typically, in such tasks, the test code only contains the main and includes the header or pastes the function. 
// Since we are writing a single response, we can put the solution in the block, and the test in block with a main that calls it. 
// The test code does not need to repeat the function definition; it just calls it. 
// But the instruction says "runnable C++ assert checks inside a global main function" - this implies the main function is complete and runnable. 
// To make it runnable, we need to include the solution code. So we can either paste the solution code at the top of the test block, or use #include "solution.h" etc. 
// Since we are writing a single file, we'll just include the function definitions in the test section before main. 
// But the instruction says "Provide 1-10 runnable C++ assert checks inside a global main function." It doesn't forbid adding the function definitions. 
// To be safe, I'll put the function implementations at the top of the test block, then the main. 
// However, the example output for test only shows main without including the function. That's because the function is presumably in a library. 
// But to be self-contained as per the task, I'll include the solution code in the test block as well? That would duplicate. 
// Actually, the task says "a standalone C++ programming task" - the solution and test are separate parts of the response. The test code is meant to verify the solution. So the test code likely includes the solution via copy-paste. 
// I'll do that: in the block, I'll write a main function that calls the solution, and I'll include the solution function definitions at the top of the test block. 
// But that duplicates the solution. Alternatively, I can assume the solution is in the same translation unit. I'll just write the main and calls, and add a comment that the function is defined above. But to be runnable standalone, I'll copy the solution code into the test block. 
// Given the example, they only provide main without including the function. So I'll follow that pattern: just main with asserts, and the function is assumed to be available (e.g., from the solution section). 
// Since the response is a single document, the reader can copy both sections together. So I'll write the test separately without redefining the function. 
// But to be safe, I'll write the test with the function definition included, because the instruction says "runnable" and the user might copy only the test section. 
// I'll include the function definition in the test block, but that means the solution block and test block would both have it, which is redundant but okay. 
// Maybe the intended way is that the test code includes the function from the solution. Since the solution is a free function, they can put it in a separate file and include it. 
// To keep it simple, I'll write the test block with the main and assume the function is defined elsewhere. I'll add a comment: "// Assume countFactorPaths is defined as in the solution." 
// But the instruction says "Provide 1-10 runnable C++ assert checks" - for it to be runnable, it must compile. So I'll include the function definition at the top of the test block. 
// I'll do that.

// Here is my test block:
#include <cassert>

// Copy of the solution function (for standalone test)
int countFactorPathsHelper(int a, int minFactor) {
    if (a == 1) return 1;
    int total = 1; // the sequence [a]
    for (int d = minFactor; d * d <= a; ++d) {
        if (a % d == 0) {
            total += countFactorPathsHelper(a / d, d);
        }
    }
    return total;
}

int countFactorPaths(int n) {
    if (n == 1) return 1;
    return countFactorPathsHelper(n, 2);
}

int main() {
    assert(countFactorPaths(1) == 1);
    assert(countFactorPaths(2) == 1);
    assert(countFactorPaths(3) == 1);
    assert(countFactorPaths(4) == 2); // [4], [2,2]
    assert(countFactorPaths(6) == 2); // [6], [2,3]
    assert(countFactorPaths(8) == 3); // [8], [2,4], [2,2,2]
    assert(countFactorPaths(12) == 4); // [12], [2,6], [2,2,3], [3,4]
    assert(countFactorPaths(16) == 5); // [16], [2,8], [2,2,4], [2,2,2,2], [4,4]
    assert(countFactorPaths(30) == 5); // [30], [2,15], [3,10], [5,6], [2,3,5]? Let's verify: [2,15], [2,3,5]? Actually [2,15] and [2,3,5] and [3,10] and [5,6] and [30] => 5. Also [2,5,3] not allowed because order must be non-decreasing. So 5.
    assert(countFactorPaths(36) == 9); // Let's compute manually: sequences of factors for 36. Known result: number of ordered factorizations (non-decreasing) for 36 is 9? Let's trust.
    return 0;
}

But we need to ensure the asserts are correct. Let's verify some values quickly:
- 16: factors >1: [16], [2,8], [2,2,4], [2,2,2,2], [4,4] -> 5. Our function? Let's test mentally: helper(16,2): loop d from 2 to 4 (since d*d <=16, d=2,3,4). d=2 divides: helper(8,2) returns? helper(8,2): loop d=2 to 2 (d*d<=8 => d=2), d=2 divides: helper(4,2) returns? helper(4,2): loop d=2 (4<=4) divides: helper(2,2) returns 1 ([2]? actually a=2, loop d=2, d*d<=2? false, total=1 -> returns 1), so helper(4,2)=1+1=2. Then helper(8,2) = 1 (for [8]) + 2 = 3. Then back to helper(16,2): total starts 1 ([16]) + helper(8,2)=3 => 4. Then d=3: 16%3!=0, d=4: 16%4==0, helper(4,4) -> a=4, min=4, loop d=4, 16<=4? false, total=1 ([4]) returns 1. So total=4+1=5. Good.
- 30: helper(30,2): loop d=2 to 5 (since sqrt(30)≈5.48), d=2 divides: helper(15,2) -> loop d=2 to 3, d=2? 15%2!=0, d=3 divides: helper(5,3) -> helper(5,3): loop d=3, 9<=5? false, total=1 -> returns 1. So helper(15,2)=1 (for [15]) + 1 = 2. d=3 divides: helper(10,3) -> loop d=3 to 3 (9<=10) d=3? 10%3!=0, so total=1 ([10]) -> returns 1. d=4 not, d=5 divides: helper(6,5) -> loop d=5, 25<=6? false, total=1 ([6]) -> returns 1. So helper(30,2) total = 1 ([30]) + 2 + 1 + 1 = 5. Good.
- 36: Let's compute known values: The number of ordered factorizations (non-decreasing) for 36 is indeed 9? Let's list: [36], [2,18], [2,2,9], [2,2,3,3], [2,3,6], [3,12], [3,3,4], [4,9], [6,6]? That's 9. Also [2,6,3] not allowed. So 9. Good.

So the test asserts are correct.

Now, the solution block should contain the function implementation with appropriate const correctness? The function takes an int by value, no const needed. The helper also takes int by value. We'll add comments.

We need to output code only in the section. So I'll write the solution as:

// Count the number of non-decreasing factorizations of n into factors > 1.
// For n=1, returns 1 (the sequence [1]).
int countFactorPathsHelper(int a, int minFactor) {
    if (a == 1) return 1; // reached end, count this path
    int total = 1; // the factorization consisting only of [a]
    for (int d = minFactor; d * d <= a; ++d) {
        if (a % d == 0) {
            total += countFactorPathsHelper(a / d, d);
        }
    }
    return total;
}

// Public entry point.
int countFactorPaths(int n) {
    if (n == 1) return 1;
    return countFactorPathsHelper(n, 2);
}

Note: The helper is a free function; we might want to mark it static or put it in an anonymous namespace to avoid linkage issues, but that's optional. We'll keep it simple.

Now, the test block will include the same code plus main. But to avoid duplication, I can just write the main and assume the functions are defined elsewhere. However, the instruction says "runnable C++ assert checks inside a global main function" - I'll include the function definitions in the test block to make it standalone. I'll do that.

This is a complete task. Let's write the response in the required format. I'll ensure the is one paragraph, explains approach, complexity, edge cases. has code only. has main with asserts.

Let's write it.
