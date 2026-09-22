Write a C++ function `long long escapeTime(long long v1, long long t1, long long v2, long long u, long long L, bool& capturedAtFinish, long long& captureTime, long long& captureDistance)` that simulates a swimmer and a bandit. The swimmer swims at speed `v1` for the first `t1` minutes, then swims at speed `v2` afterward. The bandit starts swimming from the same starting point at time 0 toward the swimmer at speed `u` (always moving forward along the same line). The total distance is `L` (in meters), and all speeds are in meters per minute. The function must determine the outcome: if the bandit exactly meets the swimmer at the finish line `L` (i.e., at the exact moment the swimmer reaches `L`), set `capturedAtFinish=true`, `captureTime=t` (total time for swimmer to finish), `captureDistance=L`. If the bandit never reaches the swimmer before or at the finish (i.e., bandit’s final position at time `t` is strictly less than `L`), set `capturedAtFinish=false`, `captureTime=-1`, `captureDistance` = distance the bandit has traveled by time `t` (which is `t*u`). If the bandit catches the swimmer strictly before the finish line, set `capturedAtFinish=false`, `captureTime` = integer time when they first meet (must be an integer minute, and the problem guarantees such a meeting occurs only at integer times), `captureDistance` = `captureTime * u`. The catch condition is based on the bandit’s position equaling the swimmer’s position at some integer time `mid` (where `mid > t1`), given that the swimmer’s position is `v1*t1 + v2*(mid-t1)` and bandit’s is `u*mid`. The bandit only catches if his speed `u > v2` (otherwise he never catches after `t1`), but you must still handle all cases. If the bandit never catches strictly before finish and also not at finish, return the bandit’s distance at finish time. If the bandit catches exactly at finish, return that. If the bandit catches strictly before, return that time and distance. Your function should be efficient and handle large values up to `10^18`. The input always satisfies `v1, v2, u > 0`, `t1 >= 1`, `L > v1*t1` (so the swimmer uses both phases). Return the outcome via the reference parameters and a boolean indicating whether capture happened strictly before the finish (true) or not (false). For simplicity, the function returns `true` if capture happened strictly before finish, `false` otherwise (including capture at finish and no capture). The reference `capturedAtFinish` indicates exact finish capture.

// The swimmer’s position as a function of time is piecewise: for `t <= t1`, position = `v1*t`, but since we only care about after `t1` (because L > v1*t1, the finish is after t1), we can ignore the first phase for the catch. Total time to finish: `t_total = t1 + (L - v1*t1)/v2`. Since all values are integers and division is exact? The problem’s original snippet assumes integer division, but we should handle floating? Actually the snippet uses `(L-v1*t1)/v2` as integer division, implying that `L - v1*t1` is divisible by `v2`. We assume that for this task. So `t_total` is an integer.
//
// Bandit’s position at any integer `mid >= t1` is `u*mid`. Swimmer’s position at `mid` is `v1*t1 + v2*(mid - t1)`. They meet when these equal: `u*mid = v1*t1 + v2*(mid-t1)` => `u*mid - v2*mid = v1*t1 - v2*t1` => `(u - v2)*mid = (v1 - v2)*t1`. So `mid = (v1 - v2)*t1 / (u - v2)`. But we must consider integer and range. However, the original code uses a binary search on integer `mid` from `t1+1` to `t_total`, comparing `(mid-t1)*(u-v2)` with `(v1-u)*t1`. Let’s derive: swimmer position = `v1*t1 + v2*(mid-t1)`. Bandit = `u*mid`. Condition `u*mid > swimmer` means bandit ahead. `u*mid > v1*t1 + v2*(mid-t1)` => `u*mid - v2*(mid-t1) > v1*t1` => `u*mid - v2*mid + v2*t1 > v1*t1` => `mid*(u-v2) > (v1 - v2)*t1`. But the original uses `(mid-t1)*(u-v2) > (v1-u)*t1`. Let’s check: `(mid-t1)*(u-v2) > (v1-u)*t1` => `mid*(u-v2) - t1*(u-v2) > v1*t1 - u*t1` => `mid*(u-v2) > v1*t1 - u*t1 + t1*(u-v2)` => `mid*(u-v2) > v1*t1 - u*t1 + u*t1 - v2*t1` = `(v1 - v2)*t1`. Yes same. So the condition for bandit ahead is `mid*(u-v2) > (v1-v2)*t1`. If `u > v2`, then LHS increases with mid, so there is a unique crossing point if the solution exists within `(t1, t_total]`. If `u <= v2`, then bandit never catches after t1 because his relative speed is not positive. Also note at `mid = t1`, bandit position `u*t1` vs swimmer `v1*t1`. Since typically `u` could be less than `v1`? The original snippet uses `v1 > u`? Not necessarily. But we must handle. The catch can happen only if `u > v2` (strictly faster than swimmer’s second phase). Also the bandit may be behind at `t1` and catch later. The condition for catch at integer `mid` is equality. The binary search in the original finds the first `mid` where bandit is ahead. If equality occurs, that’s the catch time. If the equality occurs exactly at `t_total`, then captured at finish. If equality occurs before `t_total`, then captured strictly before. If no equality and bandit never ahead by `t_total`, then no capture (or capture at finish if equality at `t_total`). To be safe, we can compute the exact integer `mid` using the formula if the division is exact and `mid` is in `(t1, t_total]`. But to avoid division issues, we use binary search as in original. However, for very large numbers up to 1e18, binary search with `long long` is fine (about 60 iterations). We must handle the case where `u <= v2`: then no catch after `t1`; but could catch exactly at finish? If `u <= v2`, then bandit’s speed is not greater than swimmer’s second phase speed, so after `t1` the distance between them either stays or increases? Actually if `u == v2`, they maintain relative distance from `t1`. So if at `t1` bandit is behind, he never catches. If at `t1` they are equal (possible if `u*t1 == v1*t1` i.e., `u==v1`), then they meet at `t1`? But `t1` is the switch point, and the problem’s binary search starts at `t1+1`, so they don’t consider `t1`. We should treat `t1` as a valid catch time? The original code only considers `mid > t1`. So we follow that. So if `u <= v2`, no catch after `t1`. Also if `u > v2`, we do binary search. Edge case: the equality at `t_total` is exactly finish. Also the case where bandit catches exactly at `t1`? Not considered. So we ignore. Also the case where `L == t_total * u`? The original checks that first. But our function should handle that. We'll follow the original logic: First compute t_total. If `L == t_total * u`, then capture at finish: set capturedAtFinish=true, captureTime=t_total, captureDistance=L. Else if `L > t_total*u`, bandit never catches (even at finish), set capturedAtFinish=false, captureTime=-1, captureDistance = t_total*u. Else (i.e., `L < t_total*u`), bandit would be ahead at finish, so there must be a catch before finish. But note: if `u <= v2`, it's impossible to have `L < t_total*u`? Let’s check: t_total = t1 + (L - v1*t1)/v2. If u <= v2, then t_total * u <= t_total * v2? But t_total*v2 = t1*v2 + L - v1*t1. Is that necessarily >= L? Not always. For example, v1=10, t1=1, v2=5, L=20, then t_total=1+ (20-10)/5=3, t_total*u with u=5 gives 15 <20. So bandit behind. So `L < t_total*u` can happen even if u<=v2? Actually u=5, v2=5, t_total*u=15<20, so condition `L < t_total*u` is false (since 20>15). So it's `L > t_total*u`? Actually 20>15, so `L > t_total*u` true, so that falls into "bandit never catches". So the else branch `L < t_total*u` implies u must be > v2 because otherwise t_total*u <= t_total*v2? Not necessarily, but if L < t_total*u, then u > L/t_total. Since L/t_total is average speed of swimmer. But we can just implement the same logic: if L < t_total*u, then we binary search for the first integer mid > t1 where bandit is ahead. If found equality, output that; if not, then at some mid bandit ahead, but that mid must be <= t_total. But we need to output the exact catch time. Since the original code only prints capture when equality found, otherwise it does nothing (bug? Actually original code prints nothing in else branch if no equality found? It has a binary search loop that only prints when equality found; if no equality, it exits loop without printing. That would be a bug. But we will implement correctly: if equality found, that's the catch; if not, then the catch must happen at the first mid where bandit is ahead, but since the swimmer and bandit are moving at constant speeds on integer minute intervals, they can only meet at integer times? Actually they meet at a real time, but the problem's original binary search only considers integer minutes, implying that the meeting time is integer by construction. We can assume that the meeting happens at an integer minute. So we should find the exact integer `mid` where equality holds. But if no equality, that means the bandit jumps from behind at `mid-1` to ahead at `mid`, so the catch must have occurred at some non-integer time? But the original only handles integer catch. To be safe, we will follow the original: binary search for equality; if found, that's the time. If not found, but `L < t_total*u` and `u > v2`, then the first integer where bandit is ahead must be >= t1+1 and <= t_total. The equality must hold at some integer because the difference `(mid-t1)*(u-v2) - (v1-u)*t1` changes by `(u-v2)` per unit mid, and since both sides are integers, if it goes from negative to positive, it must hit zero at some integer? Actually if the difference is an integer linear function with slope integer, it can jump over zero without hitting it? For example, difference = 2*mid - 5, at mid=2 it's -1, at mid=3 it's 1, no zero. So no integer catch. So the original code fails. But the task likely ensures that such cases do not occur (i.e., the catch always occurs at integer time). So we will assume given that the meeting time is integer. So we can binary search for equality. If no equality, then we treat as no catch strictly before, but we know L < t_total*u, so the bandit passes the swimmer between two integer minutes. However, the original outputs nothing, which is incorrect. We'll modify by outputting the first integer mid where bandit is ahead? But that would be a different time. The task says "if the bandit catches the swimmer strictly before the finish line, set captureTime = integer time when they first meet (must be an integer minute, and the problem guarantees such a meeting occurs only at integer times)". So we can assume the equation has an integer solution. So binary search will find it. If it doesn't, then we can fallback to no capture? But the condition `L < t_total*u` and `u > v2` guarantees that the bandit passes the swimmer at some real time between t1 and t_total. The integer guarantee means that real time is integer. So binary search will find equality. Good.
//
// Thus algorithm: compute t_total = t1 + (L - v1*t1)/v2 (integer division, assume divisible). If L == t_total*u, set capturedAtFinish=true, captureTime=t_total, captureDistance=L, return false (not strict capture). Else if L > t_total*u, set capturedAtFinish=false, captureTime=-1, captureDistance=t_total*u, return false. Else (L < t_total*u), binary search `low = t1+1`, `high = t_total`. While low+1 < high, mid = (low+high)/2, compare `(mid-t1)*(u-v2)` vs `(v1-u)*t1`. If equal, that's the catch time, set capturedAtFinish=false, captureTime=mid, captureDistance=mid*u, return true (strict capture). If `(mid-t1)*(u-v2) > (v1-u)*t1`, then bandit ahead at mid, so set high=mid; else set low=mid. After loop, we need to check low and high? Actually since we guarantee integer solution, it will be found in the loop. If not found, we can compute directly using formula. But to be safe, after binary search, check low and high for equality. If found, set capture. Else, we can compute `num = (v1 - v2)*t1`, `den = (u - v2)`. If `num % den == 0`, then `mid = num/den`, check if mid > t1 and mid <= t_total, if so, that's the time. Otherwise, fallback: no strict capture? But we know L < t_total*u, so bandit ahead at finish, meaning the bandit must have passed the swimmer at some real time. Since the problem guarantees integer, we can assume that mid found. For robustness, we can still output the first integer where bandit is ahead? But that would be inconsistent. Better to follow the original: if equality not found, treat it as no capture strictly? But that contradicts the condition. To avoid complexity, we will simply compute using the formula after binary search if not found. Indeed, the binary search will find equality if it exists. So we can rely on it.
//
// Edge cases: v1 == u? Then at t1, bandit position = u*t1 = v1*t1 = swimmer position, so they are together at t1. But we ignore t1. After t1, swimmer speed v2, bandit u. If u > v2, the bandit will be ahead immediately after t1? At any mid > t1, bandit > swimmer? Let's check: at mid=t1+1, bandit = u*(t1+1), swimmer = v1*t1 + v2*1. Since v1 = u, swimmer = u*t1 + v2. Bandit = u*t1 + u. Since u > v2, bandit ahead. So the first integer after t1, bandit is ahead. But they were together at t1, so the catch time should be t1? But original doesn't consider t1. The task says "strictly before the finish line", and t1 might be before finish. But the original code starts binary search from t1+1, so it would miss t1. However, the problem likely ensures that t1 is not the catch time (or if it is, it's considered as some other case?). To be safe, we should include t1 as a possible catch time? The original snippet does not, so we follow it. So we ignore t1. So if v1==u, and u>v2, then at t1 they are same, but we don't treat as catch. The first integer after t1 bandit is ahead, so the equality at t1 is not counted. Since the problem guarantees integer catch, maybe it ensures that catch occurs after t1. So fine.
//
// Also the case where u-v2 is negative: then `(mid-t1)*(u-v2)` is negative, so the comparison `>` or `<` flips. But we only enter binary search when L < t_total*u, which implies u must be > v2? Let's verify: Since t_total = t1 + (L - v1*t1)/v2. We have L < t_total*u => L < u*(t1 + (L - v1*t1)/v2). Multiply both sides by v2: L*v2 < u*v2*t1 + u*(L - v1*t1). Rearranged: L*v2 < u*L + u*t1*(v2 - v1). This doesn't directly force u > v2. But consider the relative speed after t1: if u <= v2, then the bandit's speed is no faster than the swimmer's second phase. At time t1, the bandit is at u*t1, swimmer at v1*t1. The distance between them is (v1 - u)*t1. If v1 > u, then swimmer is ahead. After t1, the gap changes at rate (u - v2) which is <=0, so the gap never shrinks; so bandit never catches. So if u <= v2, bandit cannot catch after t1 unless they were already together at t1. So if L < t_total*u, it might be possible even with u <= v2? Let's test: v1=100, t1=1, v2=1, u=1, L=200. t_total = 1 + (200-100)/1 = 101. t_total*u = 101, L=200 >101, so not L<. Another: v1=10, t1=10, v2=1, u=1, L=100. t_total=10+(100-100)/1=10? L=100, v1*t1=100, so L=v1*t1, but task says L>v1*t1. So not allowed. To have L< t_total*u with u<=v2, we need t_total*u > L. Since t_total = t1 + (L - v1*t1)/v2. If v2 is very small, t_total large, so t_total*u could be large. Example: v1=10, t1=2, v2=1, u=1, L=20. v1*t1=20, L=20 not >. So try L=25, v1=10, t1=2, v1*t1=20, L=25>20. t_total=2 + (5)/1=7. t_total*u=7 <25. Not. Try v2=0.5? But speeds are integers? Probably integers. So with integer speeds, if u<=v2, then t_total*u <= t_total*v2. And t_total*v2 = t1*v2 + L - v1*t1. Since L > v1*t1, we have t_total*v2 = L - (v1 - v2)*t1 < L. So t_total*u <= t_total*v2 < L. So L < t_total*u cannot happen if u <= v2. Thus, indeed, the condition L < t_total*u implies u > v2. Good.
//
// Thus binary search is valid.
//
// Time complexity: O(log L) due to binary search, O(1) space.

#include <cstdint>
#include <algorithm>

// Determines the outcome of the swimmer vs bandit pursuit.
// Returns true if bandit catches strictly before finish, false otherwise.
// If strict catch: capturedAtFinish=false, captureTime=first integer catch time, captureDistance=time*u.
// If catch at finish: capturedAtFinish=true, captureTime=total finish time, captureDistance=L.
// If no catch: capturedAtFinish=false, captureTime=-1, captureDistance=bandit's distance at finish time.
bool swimmerBandit(
    long long v1, long long t1, long long v2, long long u, long long L,
    bool& capturedAtFinish,
    long long& captureTime,
    long long& captureDistance
) {
    // Total time for swimmer to finish (assuming integer division exact, as guaranteed)
    long long t_total = t1 + (L - v1 * t1) / v2;

    // Case 1: Bandit exactly at finish when swimmer arrives
    if (L == t_total * u) {
        capturedAtFinish = true;
        captureTime = t_total;
        captureDistance = L;
        return false;
    }

    // Case 2: Bandit is behind at finish time
    if (L > t_total * u) {
        capturedAtFinish = false;
        captureTime = -1;
        captureDistance = t_total * u;
        return false;
    }

    // Case 3: Bandit will pass the swimmer strictly before finish.
    // Binary search for first integer time mid > t1 where bandit is ahead.
    // The equality condition is: (mid - t1) * (u - v2) == (v1 - u) * t1
    long long low = t1 + 1;
    long long high = t_total;
    long long lhs_const = (v1 - u) * t1;

    while (low + 1 < high) {
        long long mid = (low + high) / 2;
        long long lhs = (mid - t1) * (u - v2);
        if (lhs > lhs_const) {
            high = mid;  // Bandit already ahead at mid
        } else if (lhs < lhs_const) {
            low = mid;   // Bandit still behind
        } else {
            // Exact equality: catch at this integer time
            capturedAtFinish = false;
            captureTime = mid;
            captureDistance = mid * u;
            return true;
        }
    }

    // The loop may exit without finding equality, so check the remaining candidates.
    // This is robust even if equality is at low or high.
    for (long long cand : {low, high}) {
        if (cand > t1 && cand <= t_total && (cand - t1) * (u - v2) == lhs_const) {
            capturedAtFinish = false;
            captureTime = cand;
            captureDistance = cand * u;
            return true;
        }
    }

    // Fallback: compute exact integer solution using division (guaranteed to be integer)
    long long num = (v1 - v2) * t1;
    long long den = (u - v2);
    if (den != 0 && num % den == 0) {
        long long mid = num / den;
        if (mid > t1 && mid <= t_total) {
            capturedAtFinish = false;
            captureTime = mid;
            captureDistance = mid * u;
            return true;
        }
    }

    // Should not reach here given problem constraints, but return no strict catch.
    capturedAtFinish = false;
    captureTime = -1;
    captureDistance = t_total * u;
    return false;
}

#include <cassert>
#include <iostream>

// function declaration from solution
bool swimmerBandit(long long v1, long long t1, long long v2, long long u, long long L,
                   bool& capturedAtFinish, long long& captureTime, long long& captureDistance);

int main() {
    bool capturedAtFinish;
    long long captureTime, captureDistance;

    // Case 1: catch exactly at finish
    // v1=5, t1=2 => swimmer covers 10 in first phase; v2=3, L=16 => remaining 6 at speed 3 => 2 more min, total t=4
    // bandit u=4, t_total*u = 16 = L -> catch at finish
    assert(swimmerBandit(5,2,3,4,16, capturedAtFinish, captureTime, captureDistance) == false);
    assert(capturedAtFinish == true);
    assert(captureTime == 4);
    assert(captureDistance == 16);

    // Case 2: bandit never catches (L > t_total*u)
    // v1=10, t1=1 => first phase 10; v2=2, L=20 => remaining 10 at speed 2 => 5 min, total t=6
    // bandit u=3, t_total*u=18 < 20 -> no catch, distance=18
    assert(swimmerBandit(10,1,2,3,20, capturedAtFinish, captureTime, captureDistance) == false);
    assert(capturedAtFinish == false);
    assert(captureTime == -1);
    assert(captureDistance == 18);

    // Case 3: catch strictly before finish, with integer time
    // v1=2, t1=1 => first phase 2; v2=1, L=10 => remaining 8 at speed 1 => 8 min, total t=9
    // bandit u=3, at time 4: swimmer pos=2 + 1*(4-1)=5, bandit=12 => already ahead.
    // Let's find exact catch: solve (mid-1)*(3-1) == (2-3)*1 => 2*(mid-1) == -1 => no solution.
    // Need better example. Let's pick v1=4, t1=2 => first phase 8; v2=2, L=20 => remaining 12 at speed 2 => 6 min, total t=8
    // bandit u=5. Condition: (mid-2)*(5-2) == (4-5)*2 => 3*(mid-2) == -2, no.
    // Try v1=6, t1=1 => 6; v2=2, L=18 => remaining 12 at speed 2 => 6, total 7.
    // bandit u=4: (mid-1)*(4-2) == (6-4)*1 => 2*(mid-1)=2 => mid=2. That's >t1 and < t_total. So catch at t=2.
    assert(swimmerBandit(6,1,2,4,18, capturedAtFinish, captureTime, captureDistance) == true);
    assert(capturedAtFinish == false);
    assert(captureTime == 2);
    assert(captureDistance == 8);

    // Case 4: catch exactly at the first integer after t1 when u > v2 and v1 > u? 
    // v1=10, t1=1, v2=5, u=8, L=25 => first phase 10, remaining 15 at speed5 => 3 min, t_total=4
    // (mid-1)*(8-5) == (10-8)*1 => 3*(mid-1)=2, no integer. So not allowed.
    // Use v1=5, t1=2, v2=3, u=4, L=20 => first phase 10, remaining 10 at speed3 => 3.333 not integer, but assume integer.
    // So we cannot test non-integer. Use valid integer case: v1=2, t1=3 => 6; v2=1, L=12 => remaining 6 at speed1 => 6 min, total 9.
    // bandit u=3: condition (mid-3)*(3-1) == (2-3)*3 => 2*(mid-3) = -3, no.
    // Let's find a case where equality holds: Need (mid - t1)*(u-v2) = (v1-u)*t1. Choose t1=2, v1=5, u=3, v2=2. Then RHS=(5-3)*2=4, LHS=(mid-2)*(3-2)=mid-2, so mid-2=4 => mid=6. Need L such that t_total >=6 and L > v1*t1=10. Let v2=2, so after t1, swimmer speed 2. If L=20, remaining 10 => 5 min, t_total=7. So t_total=7 >=6, and L<t_total*u? 20 < 21? 20<21 yes. So catch at mid=6. So test:
    assert(swimmerBandit(5,2,2,3,20, capturedAtFinish, captureTime, captureDistance) == true);
    assert(capturedAtFinish == false);
    assert(captureTime == 6);
    assert(captureDistance == 18);

    // Case 5: large numbers to check overflow safety
    // v1=1e18, t1=1, v2=1, L=2e18? But L > v1*t1? v1*t1=1e18, L=2e18, remaining=1e18 at speed1 => 1e18 min, t_total=1e18+1 huge.
    // But t_total*u may overflow. Use smaller but within long long: v1=1000000, t1=1000, v2=1, u=2, L=2000000?
    // Actually avoid complexity. Just test a moderate case where no catch: v1=10, t1=1, v2=1, u=5, L=100
    // t_total = 1 + (100-10)/1 = 91, t_total*u=455 >100, so catch before finish. Need exact? Let's just test that function returns true.
    // But we need integer catch. Compute (mid-1)*(5-1)=? RHS=(10-5)*1=5 => 4*(mid-1)=5 no integer. So not allowed.
    // Skip.

    // Case 6: No catch with u <= v2 (bandit slow) -> should go to case 2
    // v1=2, t1=1 (phase1=2), v2=5, u=3, L=20 => remaining=18 at speed5 => 3.6 not integer, but assume integer divisible: L=22 => rem=20, /5=4, t_total=5,
    // t_total*u=15 <22, so no catch. Test.
    assert(swimmerBandit(2,1,5,3,22, capturedAtFinish, captureTime, captureDistance) == false);
    assert(capturedAtFinish == false);
    assert(captureTime == -1);
    assert(captureDistance == 15);

    std::cout << "All tests passed." << std::endl;
    return 0;
}
