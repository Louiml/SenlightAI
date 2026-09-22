/*
Write a C++ function `long long minCostToBuy(long long black, long long white, long long blackPrice, long long whitePrice, long long conversionCost)` that returns the minimum total cost to purchase exactly `black` black shirts and exactly `white` white shirts. You can buy each shirt at its regular price, or you can buy a shirt of one color and pay the conversion cost to transform it into the other color. The function takes the required quantities and prices as parameters and returns the minimum possible total cost. All inputs are non-negative integers, and the result may exceed 32-bit range, so use `long long` throughout. The function should work for any non-negative inputs including zeros.
*/

#include <algorithm>

// Compute the minimum total cost to buy the exact required quantities of black and white shirts.
long long minCostToBuy(long long black, long long white, long long blackPrice, long long whitePrice, long long conversionCost) {
    // Effective cost per black shirt: buy black directly, or buy white and convert.
    long long effectiveBlackPrice = std::min(blackPrice, whitePrice + conversionCost);
    // Effective cost per white shirt: buy white directly, or buy black and convert.
    long long effectiveWhitePrice = std::min(whitePrice, blackPrice + conversionCost);
    
    return effectiveBlackPrice * black + effectiveWhitePrice * white;
}

#include <cassert>

int main() {
    // Basic cases from the original problem style.
    assert(minCostToBuy(1, 1, 1, 1, 1) == 2); // 1*1 + 1*1
    assert(minCostToBuy(2, 3, 4, 5, 1) == 2*min(4,5+1) + 3*min(5,4+1) == 2*4 + 3*5 == 8 + 15 == 23);
    
    // Conversion cheaper than direct for one color.
    assert(minCostToBuy(10, 0, 10, 1, 2) == 10*min(10,1+2) == 10*3 == 30);
    assert(minCostToBuy(0, 5, 100, 50, 10) == 5*min(50,100+10) == 250);
    
    // Both colors cheaper via conversion.
    assert(minCostToBuy(2, 2, 10, 10, 1) == 2*min(10,10+1) + 2*min(10,10+1) == 2*10 + 2*10 == 40);
    
    // Large numbers to verify long long usage.
    assert(minCostToBuy(1000000000, 1000000000, 1000000000, 1000000000, 1) == 2000000000000000000LL);
    
    // Zero quantities.
    assert(minCostToBuy(0, 0, 5, 5, 5) == 0);
    
    // Conversion is expensive, direct buying always.
    assert(minCostToBuy(3, 4, 7, 8, 100) == 3*7 + 4*8 == 53);
    
    // One color price zero.
    assert(minCostToBuy(5, 0, 0, 10, 100) == 0); // black free
    assert(minCostToBuy(0, 3, 10, 0, 100) == 0); // white free
    
    // Conversion cost zero.
    assert(minCostToBuy(2, 2, 10, 20, 0) == 2*min(10,20) + 2*min(20,10) == 2*10 + 2*10 == 40);
    
    return 0;
}

// The key insight is that for each color, there are two ways to obtain a shirt: buy it directly at its base price, or buy the opposite color and convert. For black shirts, the effective price per black shirt is `min(blackPrice, whitePrice + conversionCost)` because you can either buy black directly, or buy a white shirt (cost `whitePrice`) and convert it (cost `conversionCost`). Similarly, for white shirts, the effective price is `min(whitePrice, blackPrice + conversionCost)`. Once these effective per-unit costs are known, the total minimum cost is simply `effectiveBlackPrice * black + effectiveWhitePrice * white`. This works because there is no interaction between the two requirements beyond the prices — you never need to convert a converted shirt back, and the quantities are fixed. Edge cases include when one quantity is zero (then that term contributes zero), when one price is zero, and when conversion is more expensive than direct buying (then we simply buy directly). The algorithm runs in O(1) time and O(1) auxiliary space.
