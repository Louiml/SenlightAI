// Write a C++ function `int minimumCommonMultiple(std::vector<int> const& v)` that takes a permutation of the integers `0` to `n-1` (where `n = v.size()`), and returns the least common multiple (LCM) of the lengths of all cycles in the permutation. For example, if the permutation maps `0->1, 1->2, 2->0, 3->3, 4->5, 5->4`, then the cycle lengths are `{3, 1, 2}`, and the LCM is `6`. The input vector is guaranteed to contain a permutation (each element from `0` to `n-1` appears exactly once). You may assume `n >= 1`. Return the LCM as an `int`. The function should not modify the input vector.
The problem is to decompose a permutation into its disjoint cycles and compute the least common multiple of their lengths. The approach: maintain a boolean vector `visited` (size `n`, initialized to `false`). For each index `i` from `0` to `n-1`, if not visited, we traverse the cycle starting at `i`: follow `v[current]` until we return to the starting index. While traversing, mark each visited index. For each cycle, count its length (number of elements). After finding all cycle lengths, compute the LCM. To compute LCM of two numbers `a` and `b`, use `a / gcd(a,b) * b` (to avoid overflow, divide first). We can either accumulate the LCM directly as we find cycle lengths, or store them in a vector then fold. Edge cases: single-element cycles (length 1) do not affect LCM; if the permutation is one large cycle, the answer is `n`. Time complexity is `O(n)` because each element is visited exactly once. Space complexity is `O(n)` for the visited array plus `O(number of cycles)` for storing lengths (or `O(1)` if we accumulate directly). The algorithm is straightforward and robust.
#include <vector>
#include <numeric> // for std::gcd (C++17)

// Compute the least common multiple of all cycle lengths in a permutation.
// The input vector v is a permutation of 0..n-1.
int minimumCommonMultiple(std::vector<int> const& v) {
    int n = static_cast<int>(v.size());
    std::vector<bool> visited(n, false);
    int lcm = 1;

    for (int i = 0; i < n; ++i) {
        if (!visited[i]) {
            // Count the length of the cycle containing i.
            int current = i;
            int length = 0;
            while (!visited[current]) {
                visited[current] = true;
                current = v[current];
                ++length;
            }
            // Update LCM: lcm = lcm * length / gcd(lcm, length)
            int g = std::gcd(lcm, length);
            lcm = (lcm / g) * length;
        }
    }
    return lcm;
}
#include <cassert>
#include <vector>

// The solution function is defined above.

int main() {
    // Single element permutation: cycle length 1 -> LCM = 1
    assert(minimumCommonMultiple({0}) == 1);

    // Identity permutation of size 5: all cycles length 1 -> LCM = 1
    assert(minimumCommonMultiple({0, 1, 2, 3, 4}) == 1);

    // One 3-cycle and one 2-cycle: LCM = 6
    std::vector<int> v1 = {1, 2, 0, 4, 3};
    assert(minimumCommonMultiple(v1) == 6);

    // One 4-cycle and one 1-cycle: LCM = 4
    std::vector<int> v2 = {1, 2, 3, 0, 4};
    assert(minimumCommonMultiple(v2) == 4);

    // Three cycles of lengths 2, 4, 6: LCM = 12
    std::vector<int> v3 = {1, 0, 3, 2, 5, 4, 7, 6, 9, 8, 11, 10, 13, 12, 15, 14, 17, 16, 19, 18};
    assert(minimumCommonMultiple(v3) == 12);

    // One large cycle of length 7: LCM = 7
    std::vector<int> v4 = {1, 2, 3, 4, 5, 6, 0};
    assert(minimumCommonMultiple(v4) == 7);

    // Two cycles of lengths 3 and 5: LCM = 15
    std::vector<int> v5 = {1, 2, 0, 4, 5, 6, 3};
    assert(minimumCommonMultiple(v5) == 15);

    // Cycles of lengths 2 and 3 -> LCM = 6, and a self-loop (length 1) doesn't change it
    std::vector<int> v6 = {1, 0, 4, 3, 2};
    assert(minimumCommonMultiple(v6) == 6);

    // Large permutation with cycles of lengths 8 and 12 -> LCM = 24
    std::vector<int> v7 = {1,2,3,4,5,6,7,0, 9,10,11,12,13,14,15,16,17,18,19,8};
    assert(minimumCommonMultiple(v7) == 24);

    return 0;
}
