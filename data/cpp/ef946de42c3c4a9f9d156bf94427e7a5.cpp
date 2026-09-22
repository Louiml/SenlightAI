// Write a C++ function `countColoringOptions` that takes an integer `n` representing the number of vertices in a graph (specifically, the graph formed by the integers from 2 to n+1, where two numbers are adjacent if one divides the other) and returns the minimum number of colors needed to properly color the graph (no two adjacent vertices share a color), followed by an assignment of colors to vertices 2 through n+1 in that order. The function should return a `std::pair<int, std::vector<int>>` where the first element is the chromatic number and the second is the color assignment (using colors 1, 2, 3, ...). The graph is defined such that for any two distinct integers `a, b` in [2, n+1], they are adjacent if and only if `a | b` or `b | a` (i.e., one divides the other). For `n ≤ 2`, the chromatic number is 1, and all vertices share the same color. For `n > 2`, prove that 2 colors suffice and provide an explicit coloring based on primality. The function must handle `n` up to 10^5 efficiently.

The graph is defined on integers from 2 to n+1, with edges between any two numbers where one divides the other. The key observation is that this graph is bipartite for all n. Why? Consider the property that every integer > 1 has at least one prime factor. Assign each number to one of two classes based on the total number of prime factors (with multiplicity): class A = numbers with an odd number of prime factors, class B = numbers with an even number of prime factors. If one number divides another, then the divisor has strictly fewer prime factors than its multiple (since the quotient is at least 2, adding at least one prime factor). Therefore, the parity of the number of prime factors always changes when traversing a divisibility edge. So every edge connects a number with odd prime factor count to one with even prime factor count, proving the graph is bipartite. Hence 2 colors suffice for n > 2. However, the classic simpler coloring (as seen in many ICPC problems) is: color prime numbers with color 1 and composite numbers with color 2, because no prime divides another prime, and any composite is divisible by at least one prime, so a prime and a composite may share an edge, but two composites might also share an edge (e.g., 4 and 8). So that simpler coloring is incorrect! Wait, the original snippet used `is_prime[i]` to decide color, but that seems wrong for divisibility graph because 4 and 8 are both composite and adjacent. However, notice the original code only prints colors for i from 2 to n+1, but the graph is actually not defined in the snippet; it's just a guess. For a proper task, we need a correct bipartite coloring. The correct coloring is based on the parity of the number of prime factors (with multiplicity). For n up to 10^5, we can precompute the smallest prime factor (SPF) for each number up to n+1 using a sieve. Then for each number, compute the parity of its prime factor count by repeatedly dividing by SPF. This gives O(n log log n) time. Edge cases: n=1 -> one vertex, chromatic number 1, color 1. n=2 -> two vertices 2 and 3, are they adjacent? 2 doesn't divide 3, 3 doesn't divide 2, so no edge, but the problem statement says for n≤2 chromatic number is 1 (since the graph is edgeless for n=1, but for n=2 vertices 2 and 3 are coprime, so no edge either; so 1 color suffices). For n>2, chromatic number is exactly 2 because the graph is bipartite and has at least one edge (e.g., 2 and 4 when n>=4, or for n=3, vertices 2 and 3 are not adjacent, but 2 and 4? n=3 => vertices 2,3,4; 2 divides 4, so edge exists, so need 2 colors). So algorithm: if n <= 2, return {1, vector of 1's}. Else compute parity of prime factor count for each i from 2 to n+1: if parity is 1 (odd), color 1; if even, color 2. This ensures adjacent vertices have different colors because divisor always changes parity. Time complexity O(n log log n) for sieve, space O(n). Return pair.

#include <vector>
#include <utility>

// Compute chromatic number and a valid coloring for the divisibility graph on {2,...,n+1}.
// Returns a pair: first = minimum number of colors (1 or 2), second = color assignment (1-indexed colors).
std::pair<int, std::vector<int>> countColoringOptions(int n) {
    const int limit = n + 1;
    std::vector<int> spf(limit + 1, 0);  // smallest prime factor
    for (int i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {
            for (int j = i; j <= limit; j += i) {
                if (spf[j] == 0) spf[j] = i;
            }
        }
    }

    std::vector<int> colors(limit + 1, 1);
    if (n <= 2) {
        // All vertices share color 1, no edges exist.
        return {1, colors};
    }

    // For each number, compute parity of total prime factors (with multiplicity).
    for (int i = 2; i <= limit; ++i) {
        int x = i;
        int parity = 0;
        while (x > 1) {
            int p = spf[x];
            while (x % p == 0) {
                x /= p;
                parity ^= 1;
            }
        }
        colors[i] = (parity == 1) ? 1 : 2;
    }

    return {2, colors};
}

#include <cassert>
#include <vector>
#include <utility>

// The solution function is assumed to be defined above this main.
int main() {
    // n = 1: only vertex 2, no edges, chromatic number 1.
    auto res1 = countColoringOptions(1);
    assert(res1.first == 1);
    assert(res1.second[2] == 1);

    // n = 2: vertices 2 and 3, no divisibility, chromatic number 1.
    auto res2 = countColoringOptions(2);
    assert(res2.first == 1);
    assert(res2.second[2] == 1 && res2.second[3] == 1);

    // n = 3: vertices 2,3,4; edge 2-4, so need 2 colors.
    auto res3 = countColoringOptions(3);
    assert(res3.first == 2);
    assert(res3.second[2] != res3.second[4]);
    assert(res3.second[2] == res3.second[3]); // no edge between 2 and 3, but color 1 for odd prime count.

    // n = 4: vertices 2,3,4,5; edges 2-4. Check validity.
    auto res4 = countColoringOptions(4);
    assert(res4.first == 2);
    for (int i = 2; i <= 5; ++i) {
        for (int j = i + 1; j <= 5; ++j) {
            if (j % i == 0) {
                assert(res4.second[i] != res4.second[j]);
            }
        }
    }

    // n = 10: test full graph up to 11.
    auto res5 = countColoringOptions(10);
    assert(res5.first == 2);
    for (int i = 2; i <= 11; ++i) {
        assert(res5.second[i] >= 1 && res5.second[i] <= 2);
        for (int j = i + 1; j <= 11; ++j) {
            if (j % i == 0) {
                assert(res5.second[i] != res5.second[j]);
            }
        }
    }

    // n = 100000: ensure it runs fast and returns correct chromatic number.
    auto res6 = countColoringOptions(100000);
    assert(res6.first == 2);
    // Spot-check a known edge: 2 and 4
    assert(res6.second[2] != res6.second[4]);
}
