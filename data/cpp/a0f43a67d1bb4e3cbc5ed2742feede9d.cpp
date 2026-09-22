Write a C++ function `long long countRightTriangles(const std::vector<std::pair<int,int>>& points)` that, given a list of \(N\) integer grid points (with \(N \ge 3\) and coordinates up to \(10^9\) in absolute value), returns the number of unordered triples of points that form a right triangle whose right angle is at one of the three selected points. The triangle is considered right if the squared lengths of the two legs sum to the squared length of the hypotenuse. However, the problem is not about Euclidean right angles; it is about a particular parity condition on the gcd of coordinate differences. Actually, reinterpret the task: Given the points, count unordered triples \((A,B,C)\) such that for the three pairwise differences, the sum of the three gcds of the absolute differences of x and y coordinates (i.e., \(g_1 + g_2 + g_3\)) is even. The answer must fit in a 64-bit signed integer. The input points are distinct? They may contain duplicates? In the original snippet, points are read as pairs (a,b), and the code groups them by parity class of (a/2 % 2, b/2 % 2) — that reduces the problem to counting triples of points (possibly with multiplicities) where the parity of that gcd sum is even. The function should return the count directly. Points may repeat, but each occurrence is considered a separate point for the purpose of counting triples. The output is the number of unordered triples (i.e., combinations) of indices (positions in the input vector) that satisfy the condition. If there are duplicate coordinates, each repeated occurrence is distinct.
// The key observation is that the condition on the gcd parity depends only on the parity class of each point modulo 4 after dividing by 2. Specifically, for a point (a,b), the parity class is ((a/2)%2, (b/2)%2). Since we only care about the gcd of absolute differences modulo 2, and gcd(x,y) is even iff both x and y are even. For two points, the gcd of differences is even iff both coordinate differences are even. That happens exactly when the two points have the same parity class (since the difference of two numbers that are congruent modulo 4 after dividing by 2 means their difference is divisible by 4, hence even). So for a triple, each pairwise gcd is even exactly when the two points are in the same parity class, otherwise it is odd (since at least one coordinate difference is odd, making the gcd odd). Therefore the sum of the three gcds is even iff the number of pairs in the triple that are in the same parity class is even. With three points, there are three pairs. The number of same-class pairs is 0 (all three in different classes) or 3 (all in same class) — both give even sum (0 or 3? 3 is odd, so that would be odd). Wait, let's compute: If all three are in the same class, each pair has even gcd, so sum = even+even+even = even. If all three are in different classes, each pair has odd gcd (since different classes), sum = odd+odd+odd = odd*3 = odd. If two are in one class and the third in another, then two pairs have even gcd (the two same-class pairs) and one pair (the one with the third) has odd gcd, so sum = even+even+odd = odd. So the condition is even iff all three points are in the same parity class. Therefore, the problem reduces to counting unordered triples of points that all lie in the same parity class. Since there are only 4 classes (0,0), (0,1), (1,0), (1,1), compute the count of points in each class (call them c0, c1, c2, c3). For each class with count c, the number of unordered triples is C(c,3) = c*(c-1)*(c-2)/6. Sum over all classes. That is the answer. Edge cases: N up to maybe 2e5, points may have negative coordinates, but division by 2 and modulo 2 works for negative numbers in C++ (since integer division truncates toward zero, but we need the correct parity? Let's verify: For a negative number like -1, (-1)/2 = 0 in C++ (truncation), 0%2=0, but the actual modulo 4 parity class? The original code uses `a/2 % 2`. For a=-1, -1/2 = 0, 0%2=0. For a=-2, -2/2 = -1, -1%2 = -1 in C++ (since modulo of negative is negative). That may be problematic. However, the original code assumes positive? But we can safely compute the parity class as `((a % 4 + 4) % 4) / 2` or simpler: compute `a % 4` and map: 0->0,1->0,2->1,3->1. Actually we want the parity of the floor of a/2? Actually we want the second least significant bit of the integer (i.e., bit 1). For any integer, that is simply `(a >> 1) & 1` but careful with negative numbers: in two's complement, right shift of negative is implementation-defined but usually arithmetic shift, which gives correct bit? Better to use unsigned: `((unsigned)a >> 1) & 1`. But to be safe, use modular arithmetic with non-negative modulo: `int cls = (a % 4 + 4) % 4; cls = (cls >= 2) ? 1 : 0;` That gives the parity of a/2 when dividing by 2 with floor? Actually we want the bit 1 of the binary representation, which is consistent: For any integer, bit 1 is 0 for %4 in {0,1}, and 1 for %4 in {2,3}. That works for negative as well with proper modulo. So use that. Time complexity O(N) to classify, O(1) to compute combinations. Space O(1). The answer fits in 64-bit since N up to 2e5, C(2e5,3) ~ 1.3e15 fits in 64-bit.
#include <vector>
#include <utility>

// Count unordered triples of points that all have the same parity class
// where class = ((x/2)%2, (y/2)%2) interpreted via bit 1 of x and y.
// Return value fits in 64-bit.
long long countRightTriangles(const std::vector<std::pair<int,int>>& points) {
    // Count points in each of the 4 classes.
    // class index = (bit1(x))*2 + bit1(y), where bit1(z) = ((z % 4 + 4) % 4) / 2
    long long cnt[4] = {0,0,0,0};
    for (const auto& p : points) {
        int bx = ((p.first % 4 + 4) % 4) / 2;   // 0 or 1
        int by = ((p.second % 4 + 4) % 4) / 2;  // 0 or 1
        ++cnt[bx*2 + by];
    }
    long long ans = 0;
    for (int i = 0; i < 4; ++i) {
        long long c = cnt[i];
        ans += c * (c-1) * (c-2) / 6;
    }
    return ans;
}
#include <cassert>
#include <vector>
#include <utility>

// Declare the function under test (or include the solution above)
long long countRightTriangles(const std::vector<std::pair<int,int>>& points);

int main() {
    // Single class all same: 3 points -> 1 triple
    assert(countRightTriangles({{0,0},{4,4},{8,0}}) == 1);

    // Mixed classes: no triple all in same class
    assert(countRightTriangles({{0,0},{1,0},{0,1}}) == 0);

    // 4 points in same class -> C(4,3)=4
    assert(countRightTriangles({{0,0},{4,0},{0,4},{4,4}}) == 4);

    // Two in class A, two in class B -> each class gives C(2,3)=0
    assert(countRightTriangles({{0,0},{4,0},{1,0},{5,0}}) == 0);

    // Test with negative coordinates: -4,-4 is class (bit1 of -4? -4%4=0 -> 0, same as 0)
    assert(countRightTriangles({{-4,-4},{0,0},{4,4}}) == 1);

    // Duplicate points count separately: same coordinates repeated 3 times -> 1 triple
    assert(countRightTriangles({{1,1},{1,1},{1,1}}) == 1);

    // Mixed: three in class A and two in class B -> C(3,3)=1 + C(2,3)=0
    assert(countRightTriangles({{0,0},{4,0},{8,0},{1,0},{1,1}}) == 1);

    // Larger: 5 points all in same class -> C(5,3)=10
    assert(countRightTriangles({{0,0},{4,0},{0,4},{4,4},{2,2}}) == 10);

    // Empty? Not allowed per constraints, but test returns 0
    assert(countRightTriangles({}) == 0);

    // Points with different bit1 for x and y: (2,0) x=2 -> bit1=1, y=0->0; (6,0) same; (2,4) same -> triple
    assert(countRightTriangles({{2,0},{6,0},{2,4}}) == 1);

    return 0;
}
