// Given an array of non-negative integers and its length \(n\), write a C++ function `solveArray` that returns a vector of integers representing the result: the first element is 1 if the array can be reduced to all zeros using the allowed operation, and 0 otherwise; if the first element is 1, the following elements are \(k\) (the number of operations) followed by \(3k\) integers encoding the triples \((l, r, d)\) of valid operations in the required order. The operation is: for any indices \(l<r\), choose a positive integer \(d\) so that \(val_l\) and \(val_r\) decrease by \(d\), and \(val_{l+1}, \dots, val_{r-1}\) increase by \(d\). The task must produce a valid sequence (print in the format: first "YES" or "NO", then if YES print the number of operations and each triple on a separate line) when the total XOR of the array is 0 if \(n\) is even, and always if \(n\) is odd. The required operation count must be \(n-2\) for even \(n\) and \(n-1\) for odd \(n\), with the specific pattern: for even \(n\), perform two passes of operations \((1, x, x+1)\) for \(x = 2, 4, \dots, n-2\); for odd \(n\), perform two passes of \((1, x, x+1)\) for \(x = 2, 4, \dots, n-1\). The function must return the sequence in the order: "YES" or "NO", then count, then triples. If no solution, return vector containing only 0.
The key observation is that the operation \((l, r, d)\) adds \(-d\) to positions \(l\) and \(r\), and \(+d\) to interior positions. This operation preserves the total XOR of the array if and only if the XOR of all elements is invariant under the operation. In fact, for this specific operation, the XOR of all elements remains unchanged because each operation toggles exactly two bits (by subtracting an integer) and adds to a contiguous block, but the parity of the sum of elements' parity changes. However, a more direct invariant is that the XOR of the entire array is invariant. By analyzing, one finds that if \(n\) is even, a solution exists only if the XOR of all elements is 0. If \(n\) is odd, a solution always exists. The construction: for even \(n\), perform operations \((1,2,3), (1,4,5), \dots, (1, n-2, n-1)\) in a first pass, then repeat the same sequence in a second pass. This works because each operation makes the leftmost element equal to the rightmost element's value in a controlled way, and after two passes all zeros. For odd \(n\), do the same but extend to \((1, n-1, n)\). The total number of operations is \(n-2\) for even (two passes of \(n/2-1\) operations each) and \(n-1\) for odd (two passes of \((n-1)/2\) operations each). The time complexity is \(O(n^2)\) if we simulate the array, but we do not need to actually modify the array for output—just check XOR condition and generate the sequence. Space complexity is \(O(n)\) for the output vector.
#include <vector>
#include <cstdint>

// Returns a vector describing the solution:
// vector[0] = 1 if YES, 0 if NO.
// If YES: vector[1] = operation count k,
// then vector[2..2+3k-1] are triples (l, r, d).
std::vector<long long> solveArray(const std::vector<int>& A) {
    int n = (int)A.size();
    std::vector<long long> result;
    
    // Check XOR condition for even n
    if (n % 2 == 0) {
        int xr = 0;
        for (int v : A) xr ^= v;
        if (xr != 0) {
            result.push_back(0); // NO
            return result;
        }
        result.push_back(1); // YES
        int k = n - 2;
        result.push_back(k);
        // First pass: (1, 2, 3), (1, 4, 5), ..., (1, n-2, n-1)
        for (int x = 2; x < n; x += 2) {
            result.push_back(1);
            result.push_back(x);
            result.push_back(x + 1);
        }
        // Second pass: same as first
        for (int x = 2; x < n; x += 2) {
            result.push_back(1);
            result.push_back(x);
            result.push_back(x + 1);
        }
    } else {
        // Odd n: always YES
        result.push_back(1);
        int k = n - 1;
        result.push_back(k);
        // First pass: (1, 2, 3), (1, 4, 5), ..., (1, n-1, n)
        for (int x = 2; x <= n; x += 2) {
            result.push_back(1);
            result.push_back(x);
            result.push_back(x + 1);
        }
        // Second pass: same as first
        for (int x = 2; x <= n; x += 2) {
            result.push_back(1);
            result.push_back(x);
            result.push_back(x + 1);
        }
    }
    return result;
}
#include <cassert>
#include <vector>
#include <iostream>

// Include the solution function here (or copy above)

int main() {
    // Even length with XOR 0 -> YES
    std::vector<int> a1 = {1, 2, 3, 0};
    auto r1 = solveArray(a1);
    assert(r1[0] == 1);
    assert(r1[1] == 2); // n-2 = 2
    // Triples: (1,2,3) and (1,2,3) again
    assert(r1.size() == 2 + 3*2);
    assert(r1[2] == 1 && r1[3] == 2 && r1[4] == 3);
    assert(r1[5] == 1 && r1[6] == 2 && r1[7] == 3);

    // Even length with XOR non-zero -> NO
    std::vector<int> a2 = {1, 2, 3, 4}; // XOR = 1^2^3^4 = 4
    auto r2 = solveArray(a2);
    assert(r2.size() == 1 && r2[0] == 0);

    // Odd length always YES
    std::vector<int> a3 = {5, 6, 7};
    auto r3 = solveArray(a3);
    assert(r3[0] == 1);
    assert(r3[1] == 2); // n-1 = 2
    // Triples: (1,2,3) twice
    assert(r3.size() == 2 + 3*2);
    assert(r3[2] == 1 && r3[3] == 2 && r3[4] == 3);
    assert(r3[5] == 1 && r3[6] == 2 && r3[7] == 3);

    // Odd length with n=5
    std::vector<int> a4 = {0, 0, 0, 0, 0};
    auto r4 = solveArray(a4);
    assert(r4[0] == 1);
    assert(r4[1] == 4); // n-1 = 4
    assert(r4.size() == 2 + 3*4);
    // Operations: (1,2,3), (1,4,5) twice
    assert(r4[2] == 1 && r4[3] == 2 && r4[4] == 3);
    assert(r4[5] == 1 && r4[6] == 4 && r4[7] == 5);
    assert(r4[8] == 1 && r4[9] == 2 && r4[10] == 3);
    assert(r4[11] == 1 && r4[12] == 4 && r4[13] == 5);

    // Even length n=6 with XOR 0
    std::vector<int> a5 = {1, 1, 1, 1, 1, 1};
    auto r5 = solveArray(a5);
    assert(r5[0] == 1);
    assert(r5[1] == 4); // n-2 = 4
    assert(r5.size() == 2 + 3*4);
    // Operations: (1,2,3), (1,4,5) twice
    assert(r5[2] == 1 && r5[3] == 2 && r5[4] == 3);
    assert(r5[5] == 1 && r5[6] == 4 && r5[7] == 5);
    assert(r5[8] == 1 && r5[9] == 2 && r5[10] == 3);
    assert(r5[11] == 1 && r5[12] == 4 && r5[13] == 5);

    // Edge case: n=1 (odd)
    std::vector<int> a6 = {42};
    auto r6 = solveArray(a6);
    assert(r6[0] == 1);
    assert(r6[1] == 0); // n-1 = 0, no operations
    assert(r6.size() == 2);

    // Edge case: n=2 (even) with XOR 0 -> YES, 0 operations
    std::vector<int> a7 = {3, 3};
    auto r7 = solveArray(a7);
    assert(r7[0] == 1);
    assert(r7[1] == 0);
    assert(r7.size() == 2);

    // Edge case: n=2 (even) with XOR non-zero -> NO
    std::vector<int> a8 = {1, 2};
    auto r8 = solveArray(a8);
    assert(r8.size() == 1 && r8[0] == 0);

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
