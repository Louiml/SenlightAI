Write a C++ function `long long maxCutPieces(long long n, long long m, long long k)` that, given a rectangular grid with `n` rows and `m` columns, and a target total number of blocks `k`, determines the maximum possible "cuts bonus" achievable. The bonus is defined as `(number of horizontal cuts) + (number of vertical cuts)` where you may make an **equal number** of horizontal cuts across every row and an **equal number** of vertical cuts across every column. More precisely: you choose an integer `h` (1 ≤ h ≤ n) representing the height of each resulting horizontal strip, and an integer `w` (1 ≤ w ≤ m) representing the width of each resulting vertical strip. Then you perform `n/h - 1` horizontal cuts (if `n` is divisible by `h`, otherwise you cannot use that `h`) and `m/w - 1` vertical cuts (if `m` is divisible by `w`). The bonus equals `(n/h - 1) + (m/w - 1)`. Additionally, the total number of blocks (pieces) after cutting must be **exactly** `k`. The total number of blocks equals `(n/h) * (m/w)`. However, your function must treat the problem as given in the original snippet, which uses a different but equivalent interpretation: you choose `w` (1 ≤ w ≤ m) and compute `h = ceil(k / w)`. Then you require `h ≤ n`, and the bonus is `(n/h - 1) + (m/w - 1)`, but only if both `n` is divisible by `h` and `m` is divisible by `w`. The function returns the maximum bonus achievable under these constraints; if no valid `(w, h)` exists, return `0`. Note: the original code does not check divisibility of `n` by `h` or `m` by `w` explicitly; it uses integer division by `h` for `n/h` which truncates. To match the original exactly, your function should follow that behavior (i.e., use integer division for `n/h` and `m/w`, without requiring exact divisibility). The original code also does not check `m` divisibility by `w`; it uses `m/w` integer division. So implement exactly as the snippet: for each `w` from 1 to `m`, compute `h = (k + w - 1) / w` (ceil), skip if `h > n`, then compute `bonus = (n / h - 1) + (m / w - 1)`, and track the maximum. Return that maximum. The input `n`, `m`, `k` are all positive integers (`1 ≤ n, m, k ≤ 10^18`), and the result fits in a `long long`.

int main() {
    // Basic cases
    assert(maxCutPieces(3, 3, 1) == 4); // w=3, h=1 -> (3/1-1)+(3/3-1)=2+0=2? Wait let's check: w=1 h=1 -> (3/1-1)+(3/1-1)=2+2=4
    assert(maxCutPieces(3, 3, 4) == 2); // w=2 h=2 -> (3/2-1)+(3/2-1)=1+1=2; w=1 h=4>3 skip; w=3 h=2 -> (3/2-1)+(3/3-1)=1+0=1
    assert(maxCutPieces(5, 5, 25) == 0); // w=1 h=25>5 skip, all skip -> 0
    assert(maxCutPieces(10, 10, 1) == 18); // w=10 h=1 -> (10/1-1)+(10/10-1)=9+0=9; w=1 h=1 -> 9+9=18
    // Edge: large numbers
    assert(maxCutPieces(1000000000000000000LL, 1, 1) == 999999999999999999LL); // w=1 h=1 -> (1e18/1-1)+(1/1-1)=1e18-1+0
    assert(maxCutPieces(1, 1000000000000000000LL, 1) == 999999999999999999LL); // w=1e18 h=1 -> (1/1-1)+(1e18/1e18-1)=0+0=0; w=1 h=1 -> 0+ (1e18-1)=1e18-1
    // k larger than grid capacity
    assert(maxCutPieces(2, 2, 5) == 0);
    // Divisibility irrelevant per spec
    assert(maxCutPieces(7, 3, 10) == 2); // w=2 h=5 -> (7/5-1)+(3/2-1)=1+1=2; w=3 h=4 -> (7/4-1)+(3/3-1)=1+0=1; w=1 h=10>7 skip
    return 0;
}

#include <algorithm>

// Given a grid with n rows, m columns, and a target block count k,
// return the maximum bonus = (n/h - 1) + (m/w - 1) where w is chosen
// from 1..m, h = ceil(k / w), and h <= n.
// Uses integer division (truncation) for n/h and m/w exactly as in the original snippet.
long long maxCutPieces(long long n, long long m, long long k) {
    long long best = 0;
    for (long long w = 1; w <= m; ++w) {
        // h = ceil(k / w)
        long long h = (k + w - 1) / w;
        if (h > n) continue;
        long long bonus = (n / h - 1) + (m / w - 1);
        best = std::max(best, bonus);
    }
    return best;
}

// The solution iterates over all possible widths `w` from 1 to `m`. For each `w`, the required height `h` is the smallest integer such that `h * w >= k`, computed as `ceil(k / w)` using integer arithmetic: `(k + w - 1) / w`. If `h > n`, then this `w` is impossible because the grid is too short to form `k` blocks with width `w`. Otherwise, we compute the number of horizontal cuts as `n / h - 1` and vertical cuts as `m / w - 1` using integer division (truncating). The sum is the bonus candidate. We take the maximum over all `w`. The edge cases are: when `k` is larger than `n * m`, no `w` will satisfy `h ≤ n` because `ceil(k / w) > n` for all `w`; in that case the function returns `0` (though the snippet would output `0`). Also when `k` is small, the best bonus might come from large `w` or small `w`; the loop covers all. The time complexity is `O(m)` which for `m` up to `10^18` is too slow, but the task is to replicate the snippet's algorithm exactly, so we accept it. Space complexity is `O(1)`. Important: integer overflow is avoided because `n`, `m`, `k` are `long long` and the intermediate `k + w - 1` may overflow if `k` and `w` are huge; but since `w` is up to `m` and `k` up to `10^18`, `k + w - 1` can exceed `2^63`? No, `10^18` is well below `9.22e18`, and `w` up to `10^18` means `k + w - 1` could be up to `2e18` which fits in `long long` (max ~9.22e18). So it's safe.
