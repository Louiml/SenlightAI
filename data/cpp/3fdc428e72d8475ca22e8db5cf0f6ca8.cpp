// Write a C++ function `calculateMinimumCost(int n)` that computes the minimum possible cost for buying `n` items given a special offers system. Initially, the cost is `n * 3` (each item costs 3 dollars). However, there are two promotions: (1) if you buy more than 300 items, the first 300 items cost 1 dollar each (but still 3 dollars per item after 300), and (2) if you buy more than 750 items, the first 750 items cost 1 dollar each (but still 3 dollars per item after 750). The promotions are independent and you can choose only one (or neither). The function should return the lowest possible total cost among the three options: regular price, promotion 1, or promotion 2. The input `n` is a non-negative integer (0 ≤ n ≤ 10^6). For example, for n=0, the cost is 0; for n=500, the best is promotion 1: 300*1 + 200*3 = 900, while regular is 1500, and promotion 2 is 750*1 + (500-750)*3? but that's negative extra so it's just 750*1 = 750? Wait, careful: if n is less than 750, promotion 2 gives n*1, which might be even cheaper! Actually the formula in the snippet: `a=699; if(n>750){ a+=(n-750)*3; }` means that for n≤750, the cost is 699, but that seems off because 699 is fixed. Let me reinterpret the snippet: The original code computes ans = n*3, then a=299 (which is 300*1 - 1? no, it's 299, weird), but let's clarify: The promotions likely are: Option A: first 300 items cost 1 dollar each (so 300 dollars) plus 3 dollars for every item beyond 300, but there is a discount of 1 dollar total? Actually the snippet has `a=299; if(n>300){ a+=(n-300)*3; }` so for n≤300, a=299, which is lower than n*3 for n≥100? That doesn't make sense. I think the problem might be: The original code is for a different problem. Let me deduce the correct meaning: The snippet is from a TOI practice problem. Looking at the numbers: 299 = 300 - 1, 699 = 700 - 1? Possibly the promotions are: buy 300 at 1 dollar each = 300, but there is a coupon that gives 1 dollar off, so 299. Similarly 750 at 1 dollar each = 750, but with coupon 699? That doesn't fit. Better to interpret the problem directly: The cost is normally n*3. There are two discount plans: Plan A: pay 300 dollars for the first 300 items (so 1 dollar each) and then 3 dollars each for additional items. Plan B: pay 750 dollars for the first 750 items (so 1 dollar each) and then 3 dollars each for additional. But wait, the snippet initializes a=299 and a=699, which suggests that for plan A, if n≤300, you pay 299, not 300. Similarly plan B if n≤750, you pay 699. That means there is an additional 1 dollar discount for each plan regardless of n. So the cost for plan A is: if n≤300, cost = 299; else cost = 299 + (n-300)*3. Similarly plan B: if n≤750, cost = 699; else cost = 699 + (n-750)*3. But then for n=0, plan A gives 299, which is absurd. So probably the intended problem is: You have two coupons: one gives a flat discount of 1 dollar if you buy at least 300 items (but the first 300 items cost 1 each, and rest cost 3 each), and another gives a flat discount of 1 dollar if you buy at least 750 items. But for n<300, plan A is not applicable because you can't have first 300 items? But the snippet sets a=299 for any n>0? Actually the snippet does `a=299; if(n>300){ a+=(n-300)*3; }` so for n=0, a=299, ans=min(0,299)=0, so fine. For n=10, a=299, ans=min(30,299)=30, so the plan is not chosen because it's higher. So the plan is: For plan A, you pay 299 dollars flat if n≤300, and if n>300 you pay 299 + (n-300)*3. But that would be beneficial only if n is large enough so that the flat 299 is cheaper than n*3. For n=100, regular 300, plan A 299, so plan A is better by 1 dollar. So that's the intended: you get a discount coupon that gives you $1 off if you buy at least? Actually it's equivalent to: you can either pay 3 per item, or pay 1 per item for the first 300 items but with a $1 service fee? Wait 299 = 300 - 1, so it's like you pay 1 per item for first 300, but get $1 discount. Similarly 699 = 700 - 1? Actually 750*1 = 750, but 699 is 750-51, that's not $1 off. Hmm.
//
// Let me carefully analyze the snippet: 
// ```
// ans = n*3;
// a = 299;
// if(n>300) a += (n-300)*3;
// ans = min(a, ans);
// a = 699;
// if(n>750) a += (n-750)*3;
// ans = min(a, ans);
// ```
// So for plan A: if n=0, a=299, ans=min(0,299)=0. For n=1, a=299, ans=min(3,299)=3. For n=100, ans=min(300,299)=299. For n=301, a=299+1*3=302, ans=min(903,302)=302. So the plan A is: you pay a flat 299 for any n≤300, and for n>300 you pay 299 + (n-300)*3. That is equivalent to: you pay 1 dollar for the first 300 items (300 dollars) but then get a $1 discount, so 299. For n>300, you pay 299 + (n-300)*3. So the effective cost for n > 300 is 299 - 900 + 3n? Actually 299 + 3n - 900 = 3n - 601. So for n=301, 3*301-601=903-601=302. So plan A is cheaper than regular (3n) when 3n - 601 < 3n, which is always true (since -601 < 0). So plan A is always cheaper than regular for all n? For n=0, plan A gives 299, but min with 0 gives 0, so regular is better for n=0. For n=1, plan A 299, regular 3, so regular better. For n=100, plan A 299, regular 300, so plan A better. For n=299, regular 897, plan A 299, better. For n=300, regular 900, plan A 299, better. For n=301, regular 903, plan A 302, better. So plan A is better for all n≥100? Actually for n=1, plan A 299 > 3, so no. The break-even point is when 3n = 299? That is n ≈ 99.67, so for n≥100, plan A is better. For plan B: a=699, if n>750 add (n-750)*3. So for n≤750, cost=699; for n>750, cost=699+3(n-750)=3n-1551. Plan B is better than regular when 3n > 699 => n>233, and better for n>750 obviously (since 3n - 1551 < 3n). So plan B is best for n≥234? Actually for n=234, regular 702, plan B 699, better. For n=233, regular 699, plan B 699 equal. So plan B is at least as good as regular for n≥233. And plan A is better than plan B for some ranges? Compare plan A and plan B: For n≤750, plan B cost 699, plan A cost 299 if n≤300 else 299+3(n-300)=3n-601. So for n=400, plan A = 3*400-601=599, plan B = 699, so plan A better. For n=600, plan A = 3*600-601=1199, plan B = 699, so plan B better. So there is a crossover between 300 and 750. Solve 3n - 601 = 699 => 3n = 1300 => n ≈ 433.33. So for n ≤ 433, plan A is better; for n ≥ 434, plan B is better. And for n>750, plan B cost = 3n - 1551, plan A = 3n - 601, so plan A is actually cheaper by 950? Wait 3n-601 < 3n-1551? No, -601 > -1551, so 3n-601 > 3n-1551, so plan B is cheaper. So plan B is best for n≥434. So the optimal is: regular for n≤? Actually regular is 3n. For n=0, regular 0. For n=1..99, regular is less than 299 and 699, so regular best. For n=100..433, plan A best (299 or 3n-601). For n≥434, plan B best (699 if n≤750, else 3n-1551). So the function should compute min of regular, planA, planB.
//
// So the task is: Write a function `int minimumCost(int n)` that returns the minimum total cost given the three pricing schemes. The pricing schemes are:
// 1. Regular: cost = 3 * n.
// 2. Plan A: if n <= 300, cost = 299; else cost = 299 + (n-300)*3.
// 3. Plan B: if n <= 750, cost = 699; else cost = 699 + (n-750)*3.
//
// Return the minimum among these three. The input n is a non-negative integer.

// The solution is straightforward: compute the three possible costs using integer arithmetic. For Plan A, if n is 300 or less, the cost is a flat 299; otherwise it's 299 plus 3 times the number of items beyond 300. Similarly for Plan B with 750 and 699. Then take the minimum of the three values using `std::min`. Edge cases: n=0 gives regular cost 0, Plan A gives 299, Plan B gives 699, so answer 0. For small n (1 to 99), regular is cheapest. For n between 100 and 433, Plan A is cheapest; for n≥434, Plan B is cheapest. There are no special negative or overflow issues since n ≤ 10^6, so costs fit in `int`. Time complexity is O(1) with O(1) auxiliary space.

#include <algorithm>

// Compute the minimum possible total cost for buying n items.
// Regular: 3 per item.
// Plan A: flat 299 if n <= 300, otherwise 299 + (n-300)*3.
// Plan B: flat 699 if n <= 750, otherwise 699 + (n-750)*3.
// Return the minimum among the three.
int minimumCost(int n) {
    const int regular = n * 3;
    
    int planA = 299;
    if (n > 300) {
        planA += (n - 300) * 3;
    }
    
    int planB = 699;
    if (n > 750) {
        planB += (n - 750) * 3;
    }
    
    return std::min(regular, std::min(planA, planB));
}

int main() {
    assert(minimumCost(0) == 0);
    assert(minimumCost(1) == 3);
    assert(minimumCost(50) == 150);
    assert(minimumCost(100) == 299);  // Plan A: 299, regular 300, Plan B 699
    assert(minimumCost(300) == 299);  // Plan A flat 299, regular 900, Plan B 699
    assert(minimumCost(301) == 302);  // Plan A: 299+3=302, regular 903, Plan B 699
    assert(minimumCost(400) == 599);  // Plan A: 299+100*3=599, regular 1200, Plan B 699
    assert(minimumCost(433) == 698);  // Plan A: 299+133*3=698, regular 1299, Plan B 699
    assert(minimumCost(434) == 699);  // Plan B: 699, Plan A: 299+134*3=701, regular 1302
    assert(minimumCost(750) == 699);  // Plan B flat 699, Plan A: 299+450*3=1649, regular 2250
    assert(minimumCost(751) == 702);  // Plan B: 699+3=702, Plan A: 299+451*3=1652, regular 2253
    // Edge case: large n
    assert(minimumCost(1000000) == 299849? Actually compute: Plan B: 699 + (1000000-750)*3 = 699 + 999250*3 = 699 + 2997750 = 2998449. Plan A: 299 + (1000000-300)*3 = 299 + 999700*3 = 299 + 2999100 = 2999399. Regular = 3000000. So min is 2998449.
    assert(minimumCost(1000000) == 2998449);
    return 0;
}
