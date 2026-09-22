/*
Write a C++ function that, given an integer `n` (1 ≤ n ≤ 10), an integer `k`, and an `n×n` matrix `C` of integers (each between 0 and 10), returns a vector of strings where the first element is the total count of permutations of `{1,2,...,n}` such that the sum of `C[i][A[i]]` for all `i` from 1 to `n` equals exactly `k`, and each subsequent element is a space-separated permutation (with indices starting at 1 in the natural ordering `1 2 ... n` first, then lexicographically generated permutations) that satisfies this sum condition. If no permutation satisfies the condition, the vector should contain only the string `"0"`. The function must handle all permutations without repetition; permutations are generated in lexicographic order starting from `[1,2,...,n]`. Assume the input matrix uses 1-based indexing for rows and columns (i.e., element `C[i][j]` corresponds to row `i` and column `j`). The function signature is: `std::vector<std::string> solvePermutationSum(int n, int k, const std::vector<std::vector<int>>& C)`.
*/
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>

// Enumerate all permutations of {1,...,n} and count/list those where sum(C[i][A[i]]) == k.
// C is a 1-indexed n x n matrix represented as 0-indexed vector of vectors.
std::vector<std::string> solvePermutationSum(int n, int k, const std::vector<std::vector<int>>& C) {
    std::vector<std::string> result;
    std::vector<int> perm(n);
    std::iota(perm.begin(), perm.end(), 1); // initialize to 1,2,...,n

    long long count = 0;
    std::vector<std::string> validPerms;

    // Generate all permutations lexicographically
    do {
        int sum = 0;
        for (int i = 0; i < n; ++i) {
            // C[i][perm[i]-1] because C is 0-indexed but perm has 1-based values
            sum += C[i][perm[i]-1];
        }
        if (sum == k) {
            count++;
            std::string line;
            for (int i = 0; i < n; ++i) {
                if (i > 0) line += " ";
                line += std::to_string(perm[i]);
            }
            validPerms.push_back(line);
        }
    } while (std::next_permutation(perm.begin(), perm.end()));

    result.push_back(std::to_string(count));
    for (const std::string& s : validPerms) {
        result.push_back(s);
    }
    return result;
}
#include <cassert>
#include <vector>
#include <string>

int main() {
    // Test 1: trivial n=1, k must match C[1][1]
    std::vector<std::vector<int>> C1 = {{5}};
    auto r1 = solvePermutationSum(1, 5, C1);
    assert(r1.size() == 2 && r1[0] == "1" && r1[1] == "1");
    auto r1b = solvePermutationSum(1, 3, C1);
    assert(r1b.size() == 1 && r1b[0] == "0");

    // Test 2: n=2, C = [[1,2],[3,4]], permutations: [1,2] sum=1+4=5, [2,1] sum=2+3=5 -> both equal k=5
    std::vector<std::vector<int>> C2 = {{1,2},{3,4}};
    auto r2 = solvePermutationSum(2, 5, C2);
    assert(r2.size() == 3 && r2[0] == "2");
    assert(r2[1] == "1 2");
    assert(r2[2] == "2 1");

    // Test 3: n=2, k=0, no match
    auto r3 = solvePermutationSum(2, 0, C2);
    assert(r3.size() == 1 && r3[0] == "0");

    // Test 4: n=3, identity matrix with ones on diagonal, permutations where A[i]==i contribute sum=3, so k=3 only when perm==[1,2,3]
    std::vector<std::vector<int>> C3 = {{1,0,0},{0,1,0},{0,0,1}};
    auto r4 = solvePermutationSum(3, 3, C3);
    assert(r4.size() == 2 && r4[0] == "1");
    assert(r4[1] == "1 2 3");

    // Test 5: n=3, all zeros, sum always 0, k=0 gives all 6 permutations
    std::vector<std::vector<int>> C4(3, std::vector<int>(3, 0));
    auto r5 = solvePermutationSum(3, 0, C4);
    assert(r5.size() == 7 && r5[0] == "6");
    assert(r5[1] == "1 2 3");
    assert(r5[6] == "3 2 1");

    // Test 6: n=3, matrix where sum always equals 15 regardless of permutation, k=15 should return 6 permutations
    std::vector<std::vector<int>> C5 = {{1,2,3},{4,5,6},{7,8,9}};
    auto r6 = solvePermutationSum(3, 15, C5);
    assert(r6.size() == 7 && r6[0] == "6");
    // Check first and last lexicographic permutations
    assert(r6[1] == "1 2 3");
    assert(r6[6] == "3 2 1");

    // Test 7: n=4, ensure count matches brute force (here we just check a known small case)
    std::vector<std::vector<int>> C6 = {{0,1,1,1},{1,0,1,1},{1,1,0,1},{1,1,1,0}};
    // Sum = sum of off-diagonal entries? Let's manually: for permutation [1,2,3,4], sum = C[1][1]=0 + C[2][2]=0 + C[3][3]=0 + C[4][4]=0 = 0, k=0 only identity permutation matches, others have at least one diagonal.
    auto r7 = solvePermutationSum(4, 0, C6);
    assert(r7.size() == 2 && r7[0] == "1");
    assert(r7[1] == "1 2 3 4");

    return 0;
}
// The problem requires enumerating all permutations of `n` distinct integers from 1 to `n`, computing the weighted sum `Σ_{i=1..n} C[i][A[i]]` for each permutation, and counting how many equal `k`, then listing those permutations. The natural approach is to generate permutations lexicographically using the standard next-permutation algorithm: start with `A = [1,2,...,n]`, and repeatedly transform it to the next permutation by finding the largest index `i` such that `A[i] < A[i+1]`, then swapping `A[i]` with the smallest larger element to its right, and reversing the suffix. This produces all `n!` permutations without repetition. For each permutation, compute the sum in O(n) time; collect the count and the valid permutations as strings. Edge cases include `n=1` (only one permutation), `k` outside possible sum range (e.g., sums cannot exceed `n*maxC`), and when `n` is large (but constraint n≤10 ensures at most ~3.6M permutations, which is feasible). Time complexity is O(n!·n) for sum computation plus O(n) per permutation for string formatting, so asymptotically O(n!·n) time and O(n!·n) space in the worst case (for storing all valid permutations). Since `n≤10`, memory is acceptable. Also, the function must be self-contained and not rely on global variables.
