// Write a C++ function `bool constructRightTriangle(LL N, LL M, LL K, vector<pair<LL, LL>>& vertices)` that determines whether a right triangle with integer-coordinate vertices can be placed so that its area equals exactly `N * M / K` (where `N`, `M`, `K` are positive integers), with the right angle at the origin `(0,0)` and the two legs lying along the positive x-axis and positive y-axis. The function must return `true` if such a triangle exists and fill `vertices` with the three vertices in the order: origin, point on x-axis, point on y-axis. If no such triangle exists, return `false` and leave `vertices` unchanged. The area condition requires `2*N*M` to be divisible by `K`, meaning the triangle's area must be exactly `N*M/K`. The coordinates must be positive integers (greater than 0) when the triangle exists, except the origin which is (0,0). The function should handle large values up to 10^12 for N, M, K.

// The problem reduces to finding positive integers `a` and `b` such that the area of the right triangle with vertices (0,0), (a,0), (0,b) is `a*b/2 = N*M/K`. This implies `a*b = 2*N*M/K`. Since `a` and `b` must be integers, we require that `2*N*M` is divisible by `K`. If not, return false immediately.
//
// A standard construction uses the greatest common divisor. Let `g = gcd(N, K)`. Then we can set `a = N/g` and `b = 2*M*g/K`. However, this requires `K` to divide `2*M*g`. Since `g` divides `K`, write `K = g * k` where `k = K/g`. Then `b = 2*M/k`. So we need `k` to divide `2*M`. But is that always true given `2*N*M % K == 0`? Let's check: `2*N*M` divisible by `K` implies `K` divides `2*N*M`. Since `g = gcd(N,K)`, `N = g * n` with `gcd(n,k)=1`. Then `2*N*M = 2*g*n*M` must be divisible by `g*k`, so `k` divides `2*n*M`. Since `gcd(n,k)=1`, `k` divides `2*M`. Thus indeed `k` divides `2*M`. So this construction always works when divisibility holds. However, we must ensure both `a` and `b` are positive integers. If `K == g` (i.e., `k=1`), then `b = 2*M` and `a = N/g`. This still gives positive integers. But the original snippet used a special case where it doubled the x-coordinate and halved y-coordinate when `K == x`. In our simpler formulation, we just use `a = N/g`, `b = 2*M*g/K`. Note that `b` might become zero if `K` is very large? But `K` cannot exceed `2*N*M` because divisibility check ensures `K <= 2*N*M`. Also positivity is guaranteed because all inputs positive and `g` positive. Since `k` divides `2*M`, `b` is at least 1. Also `a = N/g >= 1`. So the construction always works when divisibility holds. Edge cases: when `K` equals `2*N*M`, then `b = 2*M*g/(2*N*M) = g/N`. Since `g` divides `N`, `g/N` is not an integer unless `g=N`, which would give `b=1`. But if `K=2*N*M`, then `g = gcd(N, 2*N*M) = N` only if `N` divides `K`? Actually `gcd(N, K) = gcd(N, 2*N*M) = N` because N divides 2*N*M. So `g=N`, `a=N/N=1`, `b=2*M*N/(2*N*M)=1`. That works. So fine.
//
// Time complexity: O(log(min(N,K))) for gcd, O(1) space.

#include <bits/stdc++.h>
using namespace std;
using LL = long long;

// Determines if a right triangle with integer coordinates and area N*M/K exists
// with right angle at origin and legs along axes. If so, fills vertices with
// { (0,0), (a,0), (0,b) } and returns true; otherwise returns false and leaves vertices empty.
bool constructRightTriangle(LL N, LL M, LL K, vector<pair<LL, LL>>& vertices) {
    // Area = a*b/2 must equal N*M/K, so a*b = 2*N*M/K must be integer.
    if ((2LL * N * M) % K != 0) {
        return false;
    }

    LL g = std::gcd(N, K);
    LL a = N / g;
    // b = 2*N*M/(K*a) = 2*N*M/(K * N/g) = 2*M*g/K
    LL b = 2LL * M * g / K;

    // Ensure a and b are positive integers (they must be, given divisibility condition).
    if (a <= 0 || b <= 0) {
        return false;
    }

    vertices.clear();
    vertices.emplace_back(0, 0);
    vertices.emplace_back(a, 0);
    vertices.emplace_back(0, b);
    return true;
}

#include <bits/stdc++.h>
using namespace std;
using LL = long long;

// Declaration of the function being tested
bool constructRightTriangle(LL N, LL M, LL K, vector<pair<LL, LL>>& vertices);

int main() {
    vector<pair<LL, LL>> v;

    // Basic case: N=2, M=3, K=2 -> area = 3, a*b=6 -> (a=2,b=3) works.
    assert(constructRightTriangle(2, 3, 2, v) == true);
    assert(v.size() == 3);
    assert(v[0] == make_pair(0LL, 0LL));
    assert(v[1].first > 0 && v[1].second == 0);
    assert(v[2].first == 0 && v[2].second > 0);
    // Verify area: a*b/2
    LL a = v[1].first;
    LL b = v[2].second;
    assert(a * b == 2LL * 2 * 3 / 2); // 6
    assert(a * b / 2 == 2 * 3 / 2);

    // Case where K does not divide 2*N*M -> impossible.
    v.clear();
    assert(constructRightTriangle(3, 5, 7, v) == false);
    assert(v.empty());

    // Large values, K = 2*N*M -> area = 1, a=b=1.
    v.clear();
    assert(constructRightTriangle(1000000000000LL, 1000000000000LL, 2LL*1000000000000LL*1000000000000LL, v) == true);
    assert(v[1].first == 1 && v[2].second == 1);

    // Case where K > N and gcd helps.
    v.clear();
    // N=12, M=5, K=8 -> 2*12*5=120, 120%8=0, area=15, a*b=30.
    assert(constructRightTriangle(12, 5, 8, v) == true);
    a = v[1].first;
    b = v[2].second;
    assert(a * b == 30); // Since 2*N*M/K = 120/8 = 15, a*b=30.

    // K = 1 -> area = N*M huge, a=N, b=2*M.
    v.clear();
    assert(constructRightTriangle(7, 11, 1, v) == true);
    assert(v[1] == make_pair(7LL, 0LL));
    assert(v[2] == make_pair(0LL, 22LL));

    // Edge where K equals N but not dividing 2*M? Example N=6, M=4, K=6 -> 2*6*4=48, 48%6=0, g=6, a=1, b=2*4*6/6=8.
    v.clear();
    assert(constructRightTriangle(6, 4, 6, v) == true);
    assert(v[1].first == 1 && v[2].second == 8);

    // Test with K = 2*N but not dividing M? N=5, M=3, K=10 -> 2*5*3=30, 30%10=0, g=gcd(5,10)=5, a=1, b=2*3*5/10=3.
    v.clear();
    assert(constructRightTriangle(5, 3, 10, v) == true);
    assert(v[1].first == 1 && v[2].second == 3);

    cout << "All tests passed!" << endl;
    return 0;
}
