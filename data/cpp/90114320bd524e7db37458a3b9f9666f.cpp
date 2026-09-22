Given a permutation of integers from 1 to n (where n is odd or even) stored in a 1-indexed array `a[1..n]`, write a C++ function that constructs and returns a new permutation `b[1..n]` satisfying the following condition: for every position `i`, `a[i] + b[i]` must be an odd number. The original array `a` is a permutation (each number 1..n appears exactly once); the output `b` must also be a permutation of 1..n. If multiple valid `b` exist, any is acceptable. The function should take the input array (as a vector of length n, 1-indexed values) and return a vector of length n (1-indexed values) representing the result. The solution must handle all possible n ≥ 1, including n=1 (where the only output is [1] because 1+1=2 even, but wait: condition fails? Actually for n=1, a[1]=1, we need b[1] such that 1+b[1] odd, so b[1] must be even, but only available number is 1 (odd), so impossible. However the original code snippet assumes a solution exists, so the task should specify that n is even and at least 2, or that the input is guaranteed to have a solution. Let's assume n is even and n ≥ 2, because the given code works for even n only? Let's check: For n odd, e.g., n=3, a=[1,2,3], posiMax=3 (odd). For position 1 (odd) same parity as posiMax, assign largest remaining: b[1]=3, then n becomes 2. Next position 3 (odd) same parity, assign b[3]=2, n=1. Then position 2 (even) different parity, assign b[2]=1. Check: a[1]+b[1]=1+3=4 even -> invalid. So for n odd, the condition cannot be satisfied for all positions because the sum parity requirement forces an even number of odd positions vs even positions, but with odd n the counts mismatch. Therefore the task must guarantee that n is even. I'll spec that. Write a function `std::vector<int> constructPairPermutation(const std::vector<int>& a)` that takes a permutation of 1..n (with n even) and returns a permutation b such that for every i, (a[i]+b[i]) % 2 == 1. The function must run in O(n log n) time and O(n) space.)

// The key observation is that the parity condition `a[i] + b[i]` odd means `a[i]` and `b[i]` must have opposite parity. Since both `a` and `b` are permutations of 1..n, and n is even, there are exactly n/2 odd numbers and n/2 even numbers in each. The problem reduces to matching each element of `a` with an element of `b` of opposite parity at the same index. The given approach works by identifying the position of the maximum value `n` in `a`. All positions with the same parity as that position will receive the largest available numbers in decreasing order, and the other positions receive the remaining smaller numbers in decreasing order. Why does this work? For any position `i`, if `posMax` is the position of `n` (which is odd if n is odd? but n even, so n is even, so parity of n is even). Wait: n is even, so n is even. The position of n can be odd or even. The algorithm assigns to positions with the same parity as `posMax` the values n, n-2, n-4, ... (the largest numbers, all of which have the same parity as n? Actually n even, n-2 even, etc., so all those assigned numbers are even). To positions with opposite parity it assigns n-1, n-3, ... (odd numbers). Now consider a position `i` that has the same parity as `posMax`. Then `a[i]` has some parity, and `b[i]` is even. For `a[i]+b[i]` to be odd, `a[i]` must be odd. But is that guaranteed? Not necessarily. The algorithm from the snippet works because it sorts `posi` by value ascending, but then assigns based on parity of position, not parity of `a[i]`. Let's verify with an example: n=4, a=[2,1,4,3]. posiMax=3 (odd). Positions with same parity as 3 are 1 and 3. Sort by value ascending: (1,2),(2,1),(3,4),(4,3). Assign for same parity positions: first same parity is (1,2) gets n=4 -> b[1]=4, then (3,4) gets n=3 -> b[3]=3. Then opposite parity positions: (2,1) gets n=2 -> b[2]=2, (4,3) gets n=1 -> b[4]=1. Check: a[1]+b[1]=2+4=6 even -> fails. So the snippet's algorithm is actually incorrect for arbitrary permutations! It only works if the input `a` has a special property? Let's test again: maybe the snippet assumes something else, like the input is not arbitrary but from a specific problem where the permutation has a pattern? Actually the snippet is from a competitive programming problem likely "Construct the Permutation" where you are given a permutation and you need to output another permutation such that the element at position i is different from a[i]? No, the condition here is sum odd. Let's derive a correct algorithm. The correct approach: For each position i, we need b[i] to have opposite parity to a[i]. Since b must be a permutation, we can collect all odd positions and all even positions. We can simply assign to each position i with a[i] odd, some even number from 1..n, and to each position i with a[i] even, some odd number. This is a bipartite matching problem, and since counts are equal (n even), a simple greedy works: sort positions by a[i] value? Actually we can just collect all odd numbers and all even numbers from 1..n. Then for each position i in order, if a[i] is odd, assign the next available even number; if a[i] is even, assign the next available odd number. This guarantees opposite parity at every position, and because counts match, it will use each number exactly once. For example, a=[2,1,4,3] (n=4). a[1]=2 even -> assign odd 1, a[2]=1 odd -> assign even 2, a[3]=4 even -> assign odd 3, a[4]=3 odd -> assign even 4. Result b=[1,2,3,4]. Check sums: 2+1=3 odd, 1+2=3 odd, 4+3=7 odd, 3+4=7 odd. Works. This is O(n) if we keep two queues/vectors of available odds and evens. Edge cases: n even guaranteed, but if n=2, a=[1,2] -> a[1] odd assign even 2, a[2] even assign odd 1 -> b=[2,1], sums 3 and 3 odd. Works. The time complexity is O(n) and space O(n). The snippet's approach is flawed, but the task is to create a correct solution.

#include <vector>
#include <cstdint>

// Given a permutation a[1..n] (n even), return a permutation b[1..n]
// such that for every i, a[i] + b[i] is odd.
std::vector<int> constructOppositeParityPermutation(const std::vector<int>& a) {
    const int n = static_cast<int>(a.size()) - 1; // a is 1-indexed, size n+1
    std::vector<int> oddNumbers, evenNumbers;
    for (int v = 1; v <= n; ++v) {
        if (v % 2 == 1) oddNumbers.push_back(v);
        else evenNumbers.push_back(v);
    }
    int oddIdx = 0, evenIdx = 0;
    std::vector<int> b(n + 1);
    for (int i = 1; i <= n; ++i) {
        if (a[i] % 2 == 1) {
            // Need an even number for b[i]
            b[i] = evenNumbers[evenIdx++];
        } else {
            // Need an odd number for b[i]
            b[i] = oddNumbers[oddIdx++];
        }
    }
    return b;
}

#include <cassert>
#include <vector>

// Include the solution function here (or in a header)
std::vector<int> constructOppositeParityPermutation(const std::vector<int>& a);

int main() {
    // Test case 1: n=2
    std::vector<int> a1 = {0, 1, 2}; // 1-indexed, a[1]=1, a[2]=2
    std::vector<int> b1 = constructOppositeParityPermutation(a1);
    assert((b1[1] + a1[1]) % 2 == 1);
    assert((b1[2] + a1[2]) % 2 == 1);
    // Also verify b is a permutation of 1..2
    assert(b1[1] != b1[2] && (b1[1] == 1 || b1[1] == 2) && (b1[2] == 1 || b1[2] == 2));

    // Test case 2: n=4, a=[2,1,4,3]
    std::vector<int> a2 = {0, 2, 1, 4, 3};
    std::vector<int> b2 = constructOppositeParityPermutation(a2);
    for (int i = 1; i <= 4; ++i) {
        assert((a2[i] + b2[i]) % 2 == 1);
    }
    std::vector<bool> seen(5, false);
    for (int i = 1; i <= 4; ++i) {
        assert(b2[i] >= 1 && b2[i] <= 4);
        assert(!seen[b2[i]]);
        seen[b2[i]] = true;
    }

    // Test case 3: n=4, a=[4,3,2,1]
    std::vector<int> a3 = {0, 4, 3, 2, 1};
    std::vector<int> b3 = constructOppositeParityPermutation(a3);
    for (int i = 1; i <= 4; ++i) {
        assert((a3[i] + b3[i]) % 2 == 1);
    }

    // Test case 4: n=6, a=[1,3,5,2,4,6]
    std::vector<int> a4 = {0, 1, 3, 5, 2, 4, 6};
    std::vector<int> b4 = constructOppositeParityPermutation(a4);
    for (int i = 1; i <= 6; ++i) {
        assert((a4[i] + b4[i]) % 2 == 1);
    }
    std::vector<bool> seen4(7, false);
    for (int i = 1; i <= 6; ++i) {
        assert(!seen4[b4[i]]);
        seen4[b4[i]] = true;
    }

    // Test case 5: n=10, random permutation
    std::vector<int> a5 = {0, 10, 1, 9, 2, 8, 3, 7, 4, 6, 5};
    std::vector<int> b5 = constructOppositeParityPermutation(a5);
    for (int i = 1; i <= 10; ++i) {
        assert((a5[i] + b5[i]) % 2 == 1);
    }
    return 0;
}
