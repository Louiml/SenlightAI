Given three distinct or possibly equal positive integers a, b, c (each between 1 and 10^9), write a C++ function that returns a string in the format "YES\nx y z" if there exist positive integers x, y, z such that all three pairwise sums (x+y, y+z, z+x) equal exactly the three given numbers a, b, c in some order; otherwise, it returns "NO". The function should handle all possible orderings of the input and must output exactly one of the two possible valid triples (the one with the smallest first element, and if ties, smallest second, then smallest third) when a solution exists. If no solution exists, return "NO". The function must be deterministic and efficient for large inputs.

#include <cassert>
#include <string>
#include <iostream>

// Assume findTriple is declared above.

int main() {
    // Basic valid case: 3,4,5 -> x=2,y=1,z=3 sorted -> 1 2 3? Wait compute: p=3,q=4,r=5 -> x=(3+4-5)/2=1, y=(3+5-4)/2=2, z=(4+5-3)/2=3 => "1 2 3"
    assert(findTriple(3,4,5) == "YES\n1 2 3");

    // All equal even: 2,2,2 -> p=2,q=2,r=2 -> x=1,y=1,z=1
    assert(findTriple(2,2,2) == "YES\n1 1 1");

    // All equal odd: 3,3,3 -> sum odd -> NO
    assert(findTriple(3,3,3) == "NO");

    // Sum too small: 1,1,3 -> p+q=2 <= r=3 -> NO
    assert(findTriple(1,1,3) == "NO");

    // Different order: 5,3,4 -> same as above
    assert(findTriple(5,3,4) == "YES\n1 2 3");

    // Large numbers: 1000000000, 1000000000, 1000000000 -> sum even? 3e9 even? 3e9 is 3000000000, even? 3e9 mod 2 = 0? 3000000000 is even, so x=500000000, y=500000000, z=500000000
    assert(findTriple(1000000000LL, 1000000000LL, 1000000000LL) == "YES\n500000000 500000000 500000000");

    // p+q == r: 2,3,5 -> x=(2+3-5)/2=0 -> condition p+q<=r -> NO
    assert(findTriple(2,3,5) == "NO");

    // p+q < r: 1,2,4 -> NO
    assert(findTriple(1,2,4) == "NO");

    // Edge: p+q > r but sum odd: 1,2,2 -> sum=5 odd -> NO
    assert(findTriple(1,2,2) == "NO");

    // Edge: p+q > r and sum even but gives zero? 2,4,4 -> p+q=6 >4, sum=10 even, x=(2+4-4)/2=1, y=(2+4-4)/2=1, z=(4+4-2)/2=3 => valid
    assert(findTriple(2,4,4) == "YES\n1 1 3");

    // Check that sums match: for valid cases, we can verify but assert string is enough.

    std::cout << "All tests passed." << std::endl;
    return 0;
}

#include <string>
#include <algorithm>
#include <vector>
#include <cstdint>

// Returns "YES\nx y z" if a valid triple exists, otherwise "NO".
// x, y, z are positive integers such that pairwise sums equal a, b, c in some order.
std::string findTriple(long long a, long long b, long long c) {
    std::vector<long long> nums = {a, b, c};
    std::sort(nums.begin(), nums.end());
    long long p = nums[0], q = nums[1], r = nums[2];

    // Check necessary and sufficient condition: p + q > r and total sum even.
    if (p + q <= r || ((p + q + r) % 2 != 0)) {
        return "NO";
    }

    // Compute the canonical triple (x <= y <= z).
    long long x = (p + q - r) / 2;
    long long y = (p + r - q) / 2;
    long long z = (q + r - p) / 2;

    // Ensure all are positive (they should be given the condition).
    if (x <= 0 || y <= 0 || z <= 0) {
        return "NO";
    }

    return "YES\n" + std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(z);
}

// Sort the three input numbers as p ≤ q ≤ r. Since the pairwise sums are symmetric, if a solution exists, we can assume without loss of generality that x ≤ y ≤ z. Then the three sums in non-decreasing order are p = x+y, q = x+z, r = y+z. Solving this linear system gives x = (p+q-r)/2, y = (p+r-q)/2, z = (q+r-p)/2. For x, y, z to be positive integers, each numerator must be positive and even. The smallest numerator is p+q-r, so a necessary and sufficient condition is: p+q > r, and (p+q-r), (p+r-q), (q+r-p) are all even. Since p, q, r are integers, if p+q-r is even, then the other two are even as well because (p+r-q) = (p+q-r) + 2(q - r)? Actually check parity: (p+q-r) mod 2 gives parity of p+q+r (since -r ≡ r mod 2, so p+q-r ≡ p+q+r mod 2). Similarly (p+r-q) ≡ p+q+r mod 2. So all three have same parity. Thus it's enough to check that p+q+r is even and p+q > r. So the condition simplifies to: p+q > r and (p+q+r) % 2 == 0. Then compute the triple using integer division. Time complexity is O(1) as we only do a sort of three elements and a few arithmetic operations. Space complexity is O(1). Edge cases: all three equal (p=q=r) gives p+q=2p, r=p, so p+q > r true if p>0, and sum parity: 3p even requires p even. For p=2, triple (2+2-2)/2=1, (2+2-2)/2=1, (2+2-2)/2=1? Actually r=p=2, p+q-r = 2+2-2=2, x=1, y=1, z=1 works, sums 2,2,2. So valid. For p=1, sum=3 odd, no solution. Good. Another edge: p+q == r gives x=0, not positive, so condition p+q > r. If p+q < r, then x negative, no solution. So we return "NO". We output x,y,z in sorted order, which is already x≤y≤z. Since the problem asks for lexicographically smallest triple among all valid permutations, and we have unique solution from the three sums, but there might be multiple triples due to different assignments of which sum corresponds to which variable. However, since we sorted the input and assigned the smallest sum to x+y, etc., we get a canonical triple. Other assignments might give a different triple, but the problem says "any valid triple" or "the lexicographically smallest". To be safe, we can output the canonical one, which is non-decreasing. Since the solution is unique up to permutation of x,y,z, the non-decreasing one is lexicographically smallest if we consider ordered triples. So it's fine.
