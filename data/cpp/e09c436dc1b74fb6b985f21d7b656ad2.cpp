/*
Given a positive integer n, write a C++ function that constructs a permutation of the integers from 1 to n (inclusive) such that no two adjacent elements in the permutation are consecutive integers (i.e., their absolute difference is never exactly 1). The function should return the permutation as a `std::vector<long long>`. The permutation must be valid for all n ≥ 1. For n = 1, the only permutation is `{1}`; for n = 2, there is no possible valid permutation, but you may assume n is always at least 3 in the tests. The function must handle arbitrary n efficiently, and the output permutation for each n should be deterministic. For n = 3, a valid permutation is `{1,3,2}` because adjacent differences are 2 and 1? (Wait: 1 and 3 differ by 2, 3 and 2 differ by 1 — that is invalid!) Actually, you must ensure no adjacent pair has difference 1, so for n=3, valid permutations include `{2,4,3}`? But n=3 only has 1,2,3. Check: {1,3,2}: differences 2 and 1 — invalid. {2,1,3}: diff 1 invalid. {3,1,2}: diff 2 and 1 invalid. So n=3 has no valid permutation? Let's re-check: For n=3, possible pairs: (1,2) diff 1, (2,3) diff 1, (1,3) diff 2. So any permutation of three distinct adjacent elements must contain at least one adjacent pair that is (1,2) or (2,3) because there are only three numbers and you need two adjacencies, but you can use (1,3) once, leaving one adjacency must be either (1,2) or (2,3) because the third number is 2. Indeed no valid permutation for n=3. So the problem must require n ≥ 4. In the original code, the construction works for all n ≥ 1? Let's test n=1: ans[0]=1, loop no, output 1. n=2: ans[1]=2, ans[0]=? n-2-0 = 0? That is wrong. Actually in code for even n, ans[i] = n-2-i for i from 0 to n-3, so for n=2, n-2=0, so ans[0]=0-0=0, invalid. For odd n ≥ 5, it works. For n=3, ans[0]=1, ans[1]=3, ans[2]=2 -> output 1 3 2, but as we noted invalid. So the code is not for the "no adjacent consecutive" problem; it seems to be a specific known permutation pattern. Looking at the pattern: For even n, it produces descending from n-2 down to 1, then n-1 and n. For odd n, it produces 1,3,2, then descending from n-2 down to 4? Actually for n=5: ans[0]=1, ans[1]=3, ans[2]=2, then for i=3 (<3? n-2=3, so i=3 is not <3, so loop doesn't run, so ans[3]=? we have ans[n-1]=5, ans[n-2]=4, so ans[3] is uninitialized? That is a bug. Wait, for n=5: vector ans(5) initialized to 0. ans[4]=5, ans[3]=4. Then odd: ans[0]=1, ans[1]=3, ans[2]=2. Then for i=3;i<n-2=3;i++ -> no loop. So ans[3] is already 4, ans[4] is 5, but ans[3] is also assigned as 4? But we also had ans[n-2] = n-1 = 4, which is index 3. So it's fine? ans[3] was set to 4 earlier, then in odd branch, ans[1]=3, ans[2]=2, ans[0]=1, but ans[3] remains 4, ans[4] remains 5. So output is 1 3 2 4 5. Check differences: 1-3 diff 2, 3-2 diff 1 (invalid). So indeed not that problem. Actually this code is from AtCoder ABC 210 C? No. It might be from a problem that asks for a permutation where the absolute difference between adjacent elements is at least 2 and at most something? The code seems to produce a permutation where adjacent differences are at least 2? For even n=4: ans[3]=4, ans[2]=3, then i=0,1: ans[0]=2, ans[1]=1 -> output 2 1 3 4? Wait compute: n=4, ans[3]=4, ans[2]=3. Then even: for i=0; i<2; i++: ans[0] = 4-2-0=2, ans[1]=4-2-1=1. So output 2 1 3 4. Differences: 2-1 diff 1 invalid. So still invalid. So maybe the problem is to output a permutation such that the sum of adjacent absolute differences is maximized? Or it's a known construction for "beautiful permutation" from Codeforces Round 1732 A? Actually, I recall a Codeforces problem "A. Best Permutation" (CF 1732A) that asks to maximize the sum of adjacent absolute differences, and the construction is: for even n: n-2, n-4, ..., 2, 1, 3, 5, ...? No. Alternatively, the given code might be from a problem about "permutation with no consecutive differences modulo something". However, as a teaching assistant, I need to create an independent task inspired by this code. The code clearly constructs a specific permutation of size n. I will create a task that asks to construct a permutation of 1..n such that all adjacent absolute differences are at least 2, but that is impossible for n=3 and n=2, so I need to adjust. Alternatively, note that the code for odd n ≥5 produces something like: for n=5: 1 3 2 4 5 -> has a difference of 1 between 2 and 4? Actually 2-4 diff 2, 4-5 diff 1. So not. For n=7: ans[0]=1, ans[1]=3, ans[2]=2, then i=3 to 4: ans[3]=7-2-3+3=5? Compute: n-2-i+3 = 7-2-3+3=5, i=4: 7-2-4+3=4, then ans[5]=6, ans[6]=7 -> output 1 3 2 5 4 6 7. Differences: 1-3=2, 3-2=1, 2-5=3, 5-4=1, 4-6=2, 6-7=1. So again has adjacent differences of 1. So it's not about avoiding diff 1. Looking at the pattern, it seems to be constructing a permutation that is "almost sorted" but with a specific swap. For even n: it reverses the first n-2 elements and then appends n-1,n. For odd n: it starts with 1,3,2, then reverses the rest? For n=5: starts 1,3,2, then 4,5. For n=7: 1,3,2, then 5,4, then 6,7? Actually it's 1,3,2,5,4,6,7. So it's grouping pairs. This might be from a problem that asks to output a permutation where the number of adjacent pairs with odd sum is maximized? Or something. Better to create a new task that is clearly defined and uses the same construction logic but with a clear spec. I will create a task: Given n (≥1), construct a permutation of size n using the following rule: the last two elements are n and n-1. Then, if n is even, fill the remaining positions from left to right with n-2, n-3, ..., 1. If n is odd, fill the first three positions with 1, 3, 2, and then fill the remaining positions (indices 3 to n-3) with n-2, n-3, ..., 4, in decreasing order. Return the permutation. This is exactly the code's logic. Then the test will check that the output matches the expected sequences for various n. So the task is to implement this construction.
*/

#include <vector>

// Construct a permutation of 1..n using the described pattern.
// Precondition: n >= 3.
std::vector<long long> constructPermutation(long long n) {
    std::vector<long long> ans(n);
    // Last two positions hold the two largest numbers.
    ans[n-1] = n;
    ans[n-2] = n-1;

    if (n % 2 == 0) {
        // Even n: fill positions 0..n-3 with decreasing values from n-2 down to 1.
        for (long long i = 0; i < n-2; ++i) {
            ans[i] = n-2 - i;
        }
    } else {
        // Odd n: first three positions are 1, 3, 2.
        ans[0] = 1;
        ans[1] = 3;
        ans[2] = 2;
        // Fill positions 3..n-3 with decreasing values from n-2 down to 4.
        for (long long i = 3; i < n-2; ++i) {
            ans[i] = n-2 - i + 3;
        }
    }
    return ans;
}

#include <cassert>
#include <vector>

// Function declaration (or include the above solution)
std::vector<long long> constructPermutation(long long n);

int main() {
    // n = 3
    std::vector<long long> v3 = constructPermutation(3);
    assert((v3 == std::vector<long long>{1, 3, 2}));

    // n = 4
    std::vector<long long> v4 = constructPermutation(4);
    assert((v4 == std::vector<long long>{2, 1, 3, 4}));

    // n = 5
    std::vector<long long> v5 = constructPermutation(5);
    assert((v5 == std::vector<long long>{1, 3, 2, 4, 5}));

    // n = 6
    std::vector<long long> v6 = constructPermutation(6);
    assert((v6 == std::vector<long long>{4, 3, 2, 1, 5, 6}));

    // n = 7
    std::vector<long long> v7 = constructPermutation(7);
    assert((v7 == std::vector<long long>{1, 3, 2, 5, 4, 6, 7}));

    // n = 8 (even)
    std::vector<long long> v8 = constructPermutation(8);
    assert((v8 == std::vector<long long>{6, 5, 4, 3, 2, 1, 7, 8}));

    // n = 9 (odd)
    std::vector<long long> v9 = constructPermutation(9);
    assert((v9 == std::vector<long long>{1, 3, 2, 7, 6, 5, 4, 8, 9}));

    // Verify each is a permutation of 1..n
    for (long long n = 3; n <= 20; ++n) {
        auto vec = constructPermutation(n);
        assert(vec.size() == (size_t)n);
        std::vector<bool> seen(n+1, false);
        for (long long x : vec) {
            assert(x >= 1 && x <= n);
            assert(!seen[(size_t)x]);
            seen[(size_t)x] = true;
        }
    }

    return 0;
}

// The construction is simple and follows the pattern from the given code. For a given n, we first allocate a vector of size n. We set the last two positions (indices n-1 and n-2) to n and n-1 respectively. Then we check parity:
// - If n is even, we fill indices 0 through n-3 with the values n-2, n-3, ..., 1 in that order. This can be done with a loop `for (int i = 0; i < n-2; i++) ans[i] = n-2-i;`.
// - If n is odd, we set ans[0]=1, ans[1]=3, ans[2]=2, and then fill the remaining indices from 3 to n-3 (inclusive) with values n-2, n-3, ..., 4 in decreasing order. This is achieved by looping `for (int i = 3; i < n-2; i++) ans[i] = n-2-i+3;`. For n=1 and n=3, this loop doesn't execute (for n=3, n-2=1, so i=3 is not <1), so the first three elements are set as 1,3,2 and the last two are set as 3 and 2, but that leads to duplicate values (since ans[n-2]=2 and ans[2]=2). Actually for n=3, the code would set ans[0]=1, ans[1]=3, ans[2]=2, and also ans[2]= n-1 = 2 (from earlier), so it's consistent? Wait ans[n-2] = ans[1] = n-1 = 2, but we later set ans[1]=3. That would override. Let's trace: In the code, first set ans[n-1]=n, ans[n-2]=n-1. For n=3, ans[2]=3, ans[1]=2. Then odd branch: ans[0]=1, ans[1]=3, ans[2]=2. So that overrides ans[1] to 3 and ans[2] to 2, giving 1 3 2, but that's not a permutation of 1..3? 1,3,2 is a permutation actually (1,3,2) – yes it uses 1,2,3 exactly once. So it works for n=3. For n=1: ans[0]=1, then ans[0]=n=1, then even branch? n is odd, so odd branch: ans[0]=1, ans[1] does not exist, but code for odd sets ans[1]=3 which is out of bounds for n=1? Actually the code checks if n%2==0, else it does ans[0]=1; ans[1]=3; ans[2]=2; for n=1, ans[1] and ans[2] are out of bounds – that's a bug. So the original code works for n>=3? For n=3, it works. For n=2, it breaks. So the task should specify n >= 3. I'll specify n >= 3. The construction guarantees that the output is a permutation of 1..n for all n >= 3. We can verify: For n=3, output 1 3 2 (valid). For n=4, even branch: ans[3]=4, ans[2]=3, then i=0: ans[0]=2, i=1: ans[1]=1 -> output 2 1 3 4 (valid permutation). For n=5, odd: ans[4]=5, ans[3]=4, then set 1,3,2, then loop i=3 to <3 none, so ans[3] remains 4, ans[4]=5 -> output 1 3 2 4 5 (valid permutation). For n=6, even: ans[5]=6, ans[4]=5, loop i=0..3: ans[0]=4, ans[1]=3, ans[2]=2, ans[3]=1 -> output 4 3 2 1 5 6 (valid). For n=7, odd: ans[6]=7, ans[5]=6, set 1,3,2, then loop i=3..4: i=3: ans[3]=7-2-3+3=5, i=4: ans[4]=7-2-4+3=4 -> output 1 3 2 5 4 6 7 (valid). So it always produces a permutation. Time complexity O(n) because one loop over up to n elements. Space O(n) for the vector. Edge cases: n=3 works, n=4 works, larger sizes work. The parity branch ensures correct values.
