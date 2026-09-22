/*
Given a non-empty array of positive integers, write a C++ function `int largestConnectedComponentSize(const std::vector<int>& nums)` that returns the size of the largest connected component in a graph where two numbers are connected if they share a common prime factor. The graph is built implicitly: each number in the array is a node, and an edge exists between any two numbers whose greatest common divisor (GCD) is greater than 1. The function must compute the largest group of numbers where every pair in the group is connected via a chain of such edges (i.e., the largest connected component in the union-find sense). The input array may contain duplicate values. The function should not modify the input vector. For example, given `{4, 6, 15, 35}`, 4 and 6 are connected (share 2), 6 and 15 are connected (share 3), and 15 and 35 are connected (share 5), so all four belong to one component, and the result is 4. However, `{2, 3, 9, 5}` yields two components: {2}, {3, 9} (share 3), and {5}, so the largest size is 2. The solution must be efficient for arrays up to tens of thousands of elements, and the maximum value in the array is at most \(10^5\).
*/
#include <vector>
#include <numeric>
#include <algorithm>

class UnionFind {
private:
    std::vector<int> parent;
public:
    explicit UnionFind(int n) : parent(n) {
        std::iota(parent.begin(), parent.end(), 0);
    }

    int find(int x) {
        if (parent[x] != x) {
            parent[x] = find(parent[x]);
        }
        return parent[x];
    }

    void unite(int a, int b) {
        int ra = find(a), rb = find(b);
        if (ra != rb) {
            parent[ra] = rb;
        }
    }
};

int largestConnectedComponentSize(const std::vector<int>& nums) {
    if (nums.empty()) return 0;
    int maxVal = *std::max_element(nums.begin(), nums.end());
    UnionFind uf(maxVal + 1); // nodes are numbers from 0 to maxVal

    // Union each number with its prime factors
    for (int v : nums) {
        int x = v;
        // Handle the case v == 1: no prime factors, stays isolated
        for (int p = 2; p * p <= x; ++p) {
            if (x % p == 0) {
                uf.unite(v, p); // connect v to prime p
                // For prime p, we also union v with the remaining quotient if it's prime
                while (x % p == 0) {
                    x /= p;
                }
                // After removing all factors of p, if x > 1 it's a prime factor as well
                if (x > 1) {
                    uf.unite(v, x);
                }
            }
        }
        if (x > 1) {
            // x is a prime factor of the original v (or v itself if v is prime)
            uf.unite(v, x);
        }
    }

    // Count component sizes for numbers that actually appear in nums
    std::vector<int> count(maxVal + 1, 0);
    int best = 0;
    for (int v : nums) {
        int root = uf.find(v);
        ++count[root];
        best = std::max(best, count[root]);
    }
    return best;
}
#include <cassert>
#include <vector>

int main() {
    // Example from problem: all connected through shared factors
    std::vector<int> test1 = {4, 6, 15, 35};
    assert(largestConnectedComponentSize(test1) == 4);

    // Isolated primes and one composite pair
    std::vector<int> test2 = {2, 3, 9, 5};
    assert(largestConnectedComponentSize(test2) == 2); // {3,9} size 2

    // Single element
    std::vector<int> test3 = {7};
    assert(largestConnectedComponentSize(test3) == 1);

    // All same number: duplicates form one component
    std::vector<int> test4 = {12, 12, 12};
    assert(largestConnectedComponentSize(test4) == 3);

    // Number 1 is isolated
    std::vector<int> test5 = {1, 2, 4};
    assert(largestConnectedComponentSize(test5) == 2); // {2,4} share factor 2, 1 alone

    // Two disjoint groups: {6,10} share 2, {15,21} share 3
    std::vector<int> test6 = {6, 10, 15, 21};
    assert(largestConnectedComponentSize(test6) == 2);

    // Large prime numbers: all isolated unless they repeat
    std::vector<int> test7 = {9973, 9973, 10007};
    assert(largestConnectedComponentSize(test7) == 2);

    // Mixed: 30 connects to 12 and 70, 70 connects to 35
    std::vector<int> test8 = {30, 12, 70, 35, 11};
    // 30,12 share 2/3; 30,70 share 2/5; 70,35 share 5/7 => all except 11 are connected (size 4)
    assert(largestConnectedComponentSize(test8) == 4);

    // Empty input (though problem says non-empty, robust check)
    std::vector<int> test9 = {};
    assert(largestConnectedComponentSize(test9) == 0);

    return 0;
}
// The problem is a classic application of union-find (disjoint set union) over the prime factors of the numbers. For each number `v` in the input, we factorize it completely by trial division up to `sqrt(v)`. For every distinct prime factor `p` of `v`, we union the index of `v` with the index of `p` in a union-find structure that has size equal to `max(nums) + 1`. However, since we need to connect via prime factors, we can instead union each number directly with its prime factors, using the numbers themselves as node IDs (rather than array indices). This way, two different numbers that share a prime factor will end up in the same connected component because they both connect to that prime. After processing all numbers, we count how many original array values fall into each root's component, and return the maximum count. 
//
// Important edge cases: 
// - A prime number `p` appears in the array: it is connected to itself (and to any other number divisible by `p`), so we still count it as a component of size at least 1.
// - Duplicate values: they are separate nodes but since they have the same value, they will be connected through their prime factors; duplicates are counted individually.
// - The number 1 has no prime factors (it is not divisible by any prime). It must be treated as its own isolated component of size 1. In the solution, when we factorize, we skip numbers that are 1 (no loops with `i <= v/i`), so 1 remains a separate component with size 1.
//
// Time complexity: For each number `v`, factoring by trial division up to `sqrt(v)` takes \(O(\sqrt{v})\). Over `n` numbers, if the maximum value is `m`, the worst-case total is \(O(n \sqrt{m})\). The union operations are nearly constant (inverse Ackermann). Thus total time is \(O(n \sqrt{m} \cdot \alpha(m))\), and space is \(O(m)\) for the union-find structure plus \(O(n)\) for the counting array if we use a map, but we can use a `vector<int>` of size `m+1` for counting.
