You are catering a party and need to purchase a total of `n` burgers. There are two types: premium burgers costing `x` dollars each and regular burgers costing `y` dollars each. Budget is limited, and you also have a requirement that at least `r` dollars must be reserved for drinks (i.e., the total spent on burgers cannot exceed `(total_money - r)` after accounting for the drink reservation). Write a C++ function that, given `x, y, n, r` (where `x > y > 0`, `n >= 0`, and all values are non-negative integers), returns a `std::pair<int,int>` containing the number of premium burgers and regular burgers to buy so that the total number of burgers is exactly `n` and the total cost is exactly `(total_money - r)`, where `total_money` is not given but rather you must find any solution that satisfies: **cost of `p` premium and `n-p` regular equals `r`?** Wait—let’s restate clearly: Actually the problem is: You have `n` burgers total, premium cost `x`, regular cost `y`. You have a budget `r`? No—the original code is confusing. Let’s define precisely: The function takes `x, y, n, r` where `x` = premium price, `y` = regular price, `n` = total burgers needed, and `r` = total money available (budget). You must buy exactly `n` burgers, and the total cost must be **at most** `r`? The original code checks equality: `x*(n-mid) == r - y*mid` where `mid` is regular burgers? Actually original uses `mid` as premium? Let’s infer from the output `n-ans << ans`: The code prints `n-ans` (regular?) and `ans` (premium?). Let me clarify: In the original, `isPossible(int mid,...)` computes `normal_burger_price = r - y*mid`? That seems to mean if you buy `mid` regular burgers (cost `y*mid`), the remaining money `r - y*mid` must exactly equal the cost of premium burgers `x*(n-mid)`. So `mid` is number of regular burgers, `n-mid` premium. The condition is `x*(n-mid) == r - y*mid`. So the goal is to find an integer `p` = number of regular burgers (0..n) such that total cost `y*p + x*(n-p) == r` exactly. If multiple solutions, the code returns the one with **maximum regular burgers** (since it searches from low to high and updates ans when possible). So the task is: Given `x, y, n, r`, find the maximum number of regular burgers (and consequently premium) such that total cost equals `r`. If none, return `{-1,-1}`. Write a function `std::pair<int,int> maxRegularBurgers(int premiumPrice, int regularPrice, int totalBurgers, int budget)` that returns `{regularCount, premiumCount}` (or `{-1,-1}`). The original code prints `n-ans` then `ans` meaning premium then regular? Actually output `n-ans` and `ans` – if ans is regular, then premium = n-ans, regular = ans. So it prints premium then regular. But let’s standardize: we return a pair `{regularCount, premiumCount}` for clarity. The task must be exactly that.

The problem reduces to finding an integer `p` (0 ≤ p ≤ n) such that `y*p + x*(n-p) == r`. Expanding gives `p*(y-x) + x*n == r`, so `p*(y-x) == r - x*n`. Since `y-x` is negative (because y < x), we can rewrite as `p*(x-y) == x*n - r`. Thus `p = (x*n - r) / (x-y)` must be an integer between 0 and n. The original algorithm uses binary search on `p` to find the maximum valid `p` because we want the most regular burgers (since they are cheaper). The binary search checks if `x*(n-mid) == r - y*mid`; if true, we try to increase `mid` (more regular burgers) to maximize. Important edge cases: if no integer solution exists, return `{-1,-1}`. Also `x-n` could be large, but careful with overflow: use `long long` for multiplication operations because `x*n` and `y*p` may exceed 32-bit int. Time complexity is O(log n) for binary search (or O(1) with formula, but the original uses binary search). We'll use the direct formula for O(1) but must check bounds carefully. However, since the task asks to solve the same problem, we can use the direct arithmetic for clarity: compute `diff = x-y`, then `p = (x*n - r) / diff`. But we must ensure `p` is non-negative integer and `0 <= p <= n`, and also verify exact equality: `y*p + x*(n-p) == r`. If not, return `{-1,-1}`. Also handle `diff == 0` (when x==y) – then cost is always `x*n`, so if `r == x*n`, any p works; but to maximize p, we choose p = n. If `r != x*n`, no solution. Space is O(1).

#include <utility>

// Returns the maximum number of regular burgers (and corresponding premium burgers)
// such that total burgers equals totalBurgers and total cost equals budget.
// If no solution exists, returns {-1, -1}.
// Regular burgers cost regularPrice, premium cost premiumPrice (premium > regular assumed).
std::pair<int, int> maxRegularBurgers(int premiumPrice, int regularPrice, int totalBurgers, int budget) {
    // Use long long to avoid overflow during arithmetic.
    const long long x = premiumPrice;
    const long long y = regularPrice;
    const long long n = totalBurgers;
    const long long r = budget;

    // If premium and regular costs are equal, total cost is fixed.
    if (x == y) {
        if (n * x == r) {
            return {static_cast<int>(n), 0}; // All regular maximizes regular count.
        }
        return {-1, -1};
    }

    // From the equation: y*p + x*(n-p) = r  ->  p = (x*n - r) / (x - y)
    const long long diff = x - y;
    const long long numerator = x * n - r;
    
    // Numerator must be divisible by diff and p must be within [0, n].
    if (numerator % diff != 0) {
        return {-1, -1};
    }
    const long long p = numerator / diff; // p = regular burgers (since diff > 0)
    
    if (p < 0 || p > n) {
        return {-1, -1};
    }
    
    // Verify the exact equality (safety check for any edge oversights).
    if (y * p + x * (n - p) != r) {
        return {-1, -1};
    }
    
    // Since the solution is unique when diff > 0, this is automatically the maximum regular.
    return {static_cast<int>(p), static_cast<int>(n - p)};
}

#include <cassert>
#include <utility>

// Declaration from the solution (for completeness in this test file).
std::pair<int, int> maxRegularBurgers(int, int, int, int);

int main() {
    // Basic case: 5 burgers, premium 10, regular 5, budget 35 -> 2 regular (cost 10) + 3 premium (cost 30) = 40? 35? Let's compute: p regular, n-p premium => 5p + 10(5-p) = 50 -5p = 35 -> p=3. So 3 regular, 2 premium.
    auto res = maxRegularBurgers(10, 5, 5, 35);
    assert(res.first == 3 && res.second == 2);

    // No solution: 1 burger, premium 10, regular 5, budget 7 -> impossible (costs 5 or 10 only).
    res = maxRegularBurgers(10, 5, 1, 7);
    assert(res.first == -1 && res.second == -1);

    // All regular: 3 burgers, premium 10, regular 4, budget 12 -> 3 regular cost 12.
    res = maxRegularBurgers(10, 4, 3, 12);
    assert(res.first == 3 && res.second == 0);

    // All premium: 3 burgers, premium 10, regular 4, budget 30 -> 3 premium.
    res = maxRegularBurgers(10, 4, 3, 30);
    assert(res.first == 0 && res.second == 3);

    // Larger values to check overflow: 1e9 burgers, premium 1000, regular 1, budget 1e12.
    res = maxRegularBurgers(1000, 1, 1000000000, 1000000000000LL);
    // Solve: 1*p + 1000*(1e9 - p) = 1e12 -> 1e12 - 999p = 1e12 -> p=0. So all premium.
    assert(res.first == 0 && res.second == 1000000000);

    // Edge case: x == y. 4 burgers, both cost 5, budget 20 -> any combination works; max regular is 4.
    res = maxRegularBurgers(5, 5, 4, 20);
    assert(res.first == 4 && res.second == 0);

    // Edge case: x == y but wrong budget.
    res = maxRegularBurgers(5, 5, 4, 21);
    assert(res.first == -1 && res.second == -1);

    // Zero burgers: n=0, budget=0 -> any? Must have exactly 0 burgers cost 0 -> regular 0, premium 0.
    res = maxRegularBurgers(10, 5, 0, 0);
    assert(res.first == 0 && res.second == 0);

    // Negative? Not needed as inputs are non-negative per problem, but check zero budget with n>0 -> no solution if price >0.
    res = maxRegularBurgers(10, 5, 2, 0);
    assert(res.first == -1 && res.second == -1);

    return 0;
}
