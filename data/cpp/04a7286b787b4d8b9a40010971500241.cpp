Write a C++ function `ll countValidWalks(int h, int w)` that returns the number of ways to tile an `h × w` grid (where `h` is the number of rows, `w` is the number of columns) with dominoes (2×1 or 1×2 tiles) such that no two adjacent columns have the same horizontal orientation pattern. More precisely, consider a tiling where each row is filled with a sequence of "left" and "right" moves representing a walk from left to right in each row (i.e., for each row, we choose whether the domino covering the first cell extends left or right, and similarly for all cells). The constraint is that the net horizontal displacement (sum over rows of +1 for a right-extending domino, -1 for a left-extending domino) modulo `w` must be a generator of the cyclic group of order `w` when `w` is odd, and when `w` is even, the half of that net displacement modulo `w` must be odd. Count all valid tilings modulo 998244353. If `h` is even, return 0 regardless of `w`.

#include <cassert>
#include <bits/stdc++.h>
using namespace std;

// Include the solution function here (countValidWalks) and its Mint/factorial setup.

int main() {
    // Simple small cases
    assert(countValidWalks(1, 1) == 1); // only one leftcnt=0, net=1, gcd(1,1)=1? gcd(1,1)=1, so 1 way
    assert(countValidWalks(2, 3) == 0); // h even
    assert(countValidWalks(1, 2) == 1); // h=1, w even: leftcnt=0 -> net=1 odd skip; leftcnt=1 -> net=-1 odd skip? Actually none qualify? Wait: net=1-0=1 odd skip; net=0-1=-1 odd skip -> 0? But expected? Let's check: h=1, w=2, valid? Only one row, net must be even? not, so 0. But original code had bug; we'll test our own logic: for w even, net/2 must be odd, net=±1 not even, so 0. So assert 0.
    assert(countValidWalks(1, 2) == 0);
    assert(countValidWalks(1, 3) == 2); // h=1, w=3 odd: net=1 and -1, both gcd(1,3)=1, gcd(2,3)=1? wait net=1 -> gcd(1,3)=1; net=-1 mod 3=2, gcd(2,3)=1, so two ways: leftcnt=0 and 1.
    assert(countValidWalks(3, 3) == 2 * 2 + 2? Actually compute: h=3, w=3 odd. leftcnt=0 -> net=3, mod 3=0, gcd(0,3)=3 not 1. leftcnt=1 -> net=1, mod=1, gcd=1 -> add C(3,1)=3. leftcnt=2 -> net=-1 mod=2, gcd(2,3)=1 -> add C(3,2)=3. leftcnt=3 -> net=-3 mod=0 -> skip. total=6. So assert 6.
    assert(countValidWalks(3, 3) == 6);
    // Larger sanity check: h=5, w=1 (odd), net always 0 mod 1? gcd(0,1)=1, all leftcnt valid: sum C(5,k)=32.
    assert(countValidWalks(5, 1) == 32);
    // h=5, w=2 (even): net must be even and net/2 odd. h=5, leftcnt from 0..5: net=5,3,1,-1,-3,-5 all odd -> skip all? So 0.
    assert(countValidWalks(5, 2) == 0);
    // h=4 even returns 0
    assert(countValidWalks(4, 100) == 0);
    cout << "All tests passed!" << endl;
    return 0;
}

#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef long double ld;

template<int P>
struct Mint {
    int x;
    constexpr Mint(): x{} {}
    constexpr Mint(ll x): x{static_cast<int>(x % P)} {
        if (x < 0) x += P;
    }
    constexpr int val() const { return x; }
    constexpr Mint operator-() const {
        Mint res;
        res.x = (x == 0 ? 0 : P - x);
        return res;
    }
    constexpr Mint inv() const {
        ll a = x, b = P, u = 1, v = 0;
        while (b) {
            ll t = a / b;
            a -= t * b; swap(a, b);
            u -= t * v; swap(u, v);
        }
        return Mint{u};
    }
    constexpr Mint &operator*=(Mint rhs) & {
        x = 1LL * x * rhs.x % P;
        return *this;
    }
    constexpr Mint &operator+=(Mint rhs) & {
        x += rhs.x;
        if (x >= P) x -= P;
        return *this;
    }
    constexpr Mint &operator-=(Mint rhs) & {
        x -= rhs.x;
        if (x < 0) x += P;
        return *this;
    }
    constexpr Mint &operator/=(Mint rhs) & {
        return *this *= rhs.inv();
    }
    friend constexpr Mint operator*(Mint lhs, Mint rhs) {
        Mint res = lhs; res *= rhs; return res;
    }
    friend constexpr Mint operator+(Mint lhs, Mint rhs) {
        Mint res = lhs; res += rhs; return res;
    }
    friend constexpr Mint operator-(Mint lhs, Mint rhs) {
        Mint res = lhs; res -= rhs; return res;
    }
    friend constexpr Mint operator/(Mint lhs, Mint rhs) {
        Mint res = lhs; res /= rhs; return res;
    }
};

constexpr int MOD = 998244353;
using Z = Mint<MOD>;

const int MAXN = 2000005;
Z fact[MAXN];
Z inv_fact[MAXN];

// Precompute factorials and inverse factorials up to MAXN.
void init_fact() {
    fact[0] = 1;
    inv_fact[0] = 1;
    for (int i = 1; i < MAXN; i++) {
        fact[i] = fact[i-1] * i;
    }
    inv_fact[MAXN-1] = 1 / fact[MAXN-1];
    for (int i = MAXN-2; i >= 1; i--) {
        inv_fact[i] = inv_fact[i+1] * (i+1);
    }
}

inline Z choose(int n, int k) {
    if (k < 0 || k > n) return 0;
    return fact[n] * inv_fact[k] * inv_fact[n-k];
}

// Count valid tilings modulo MOD. h = rows, w = columns.
ll countValidWalks(int h, int w) {
    if (h % 2 == 0) return 0;
    static bool initialized = false;
    if (!initialized) {
        init_fact();
        initialized = true;
    }
    Z ways = 0;
    for (int leftcnt = 0; leftcnt <= h; leftcnt++) {
        int rightcnt = h - leftcnt;
        int net = rightcnt - leftcnt; // = h - 2*leftcnt
        if (w % 2 == 1) {
            int sign = (net >= 0) ? 1 : -1;
            int net_mod = (net % w + w) % w;
            if (__gcd(net_mod, w) == 1) {
                ways += choose(h, leftcnt);
            }
        } else {
            // w even: net must be even, and net/2 must be odd and coprime to w
            if (net % 2 != 0) continue;
            int half = net / 2;
            int half_mod = (half % w + w) % w;
            if (half_mod % 2 == 1 && __gcd(half_mod, w) == 1) {
                ways += choose(h, leftcnt);
            }
        }
    }
    return ways.val();
}

// The problem is derived from a known combinatorial construction: each row tiling by dominoes can be represented as a sequence of left/right extensions. For a fixed column, the number of left-extending dominos across all rows is `leftcnt`, and right-extending is `rightcnt = h - leftcnt`. The net displacement is `net = rightcnt - leftcnt = h - 2*leftcnt`. For the grid to be properly periodic with period `w` (i.e., no periodic boundary mismatches), the net displacement modulo `w` must be coprime to `w` when `w` is odd, meaning it generates all residues. When `w` is even, the condition is stricter: the halved net displacement must be odd and coprime to `w` (which reduces to being odd because `gcd(odd, even) = 1` if the odd number has no common factor with 2, but since `w` is even, `gcd(net/2, w)=1` implies `net/2` is odd). However, note that the original snippet has a bug for even `w`: it multiplies `h` by 2, effectively considering `h` rows as `2h` rows but then computes `net` based on the doubled height, which changes the binomial coefficient base. In this task, we will not double `h`; we treat the problem as given: for odd `w`, check `gcd(net, w) == 1`; for even `w`, check `gcd(net/2, w) == 1` (which forces `net` to be even, and `net/2` odd). For each `leftcnt` from 0 to `h`, compute `net`, reduce modulo `w` (ensuring non-negative), and if the condition holds, add `C(h, leftcnt)` to the answer. The factorial precomputation is standard. Time complexity is O(h) per query plus O(MAXN) for precomputation, and space O(MAXN). Edge cases: when `h` is even, return 0 immediately. When `w` is even, `net` must be even; if `net` is odd, `net/2` is not integer, so skip that `leftcnt`. Also handle `net=0` separately (gcd(0, w) = w, not 1, so skip unless w=1 which is trivially not 1). The answer is taken modulo 998244353.
