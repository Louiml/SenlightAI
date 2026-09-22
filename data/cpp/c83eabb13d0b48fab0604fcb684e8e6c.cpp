Given two integers A and B where A represents an 8% consumption tax amount in yen and B represents a price in tens of yen (i.e., the actual price is B*10), write a C++ function `int findPrice(int A, int B)` that returns the smallest integer price (in yen) that satisfies the following condition: the floor of 8% of that price equals exactly A, and the price must be at least B*10 (the price can be equal to or greater than B*10). If no such price exists within the range [B*10, B*10 + 9] (a total of 10 consecutive integers), return -1. Note that the original snippet only checks up to 9 increments after the initial price, but your function should consider the full range of 10 integers starting from B*10 (inclusive). The price is a non-negative integer, and A and B are non-negative integers where B ≤ 1000 and A ≤ 10000.

#include <cassert>

int findPrice(int A, int B); // forward declaration

int main() {
    // Example from the snippet: A=8, B=10 -> start=100, tax(100)=8, so price=100
    assert(findPrice(8, 10) == 100);
    
    // Basic case: A=0, B=0 -> price 0 gives tax 0
    assert(findPrice(0, 0) == 0);
    
    // A=1, B=1 -> start=10, tax(10)=0, 11->0, ..., 19->1, so 19
    assert(findPrice(1, 1) == 19);
    
    // No match in the range
    assert(findPrice(5, 10) == -1);
    
    // Boundary: exactly the last price in range
    // start=20, prices 20..29, tax(20)=1, ..., tax(29)=2, so A=2 matches at 29
    assert(findPrice(2, 2) == 29);
    
    // A higher than possible for the range
    assert(findPrice(100, 100) == -1);
    
    // Large B still safe
    assert(findPrice(80, 1000) == 10000); // 10000*0.08=800, but floor=800, A=800? Wait: 10000*8/100=800, so A must be 800, not 80. Let's fix.
    // Correct: B=1000, start=10000, tax(10000)=800, so A=800.
    assert(findPrice(800, 1000) == 10000);
    
    // Edge: B=10, A=8 already tested, also test A=9 -> none in 100..109? 109*8/100=872/100=8, so no
    assert(findPrice(9, 10) == -1);
}

#include <cstdint>

// Find the smallest integer price p in [B*10, B*10+9] such that floor(0.08*p) == A.
// Return -1 if no such price exists in that range.
int findPrice(int A, int B) {
    const int start = B * 10;
    constexpr int rangeSize = 10;
    for (int i = 0; i < rangeSize; ++i) {
        const int price = start + i;
        // Using integer arithmetic to compute floor(0.08 * price) exactly.
        const int tax = (price * 8) / 100;
        if (tax == A) {
            return price;
        }
    }
    return -1;
}

// The problem is a simple brute-force search over a small bounded range. The price starts at `B * 10`. For each candidate price `p` from `B * 10` to `B * 10 + 9` (inclusive), compute `floor(0.08 * p)` using integer arithmetic to avoid floating-point precision issues: `tax = (p * 8) / 100` works because `floor(0.08 * p) = floor(p * 8 / 100)` exactly when using integer division. Compare `tax` with `A`; if equal, return `p`. If none of the 10 candidates match, return -1. Edge cases: if `B` is 0, the starting price is 0 and the range is [0,9]; also, `p * 8` can be as large as (B*10+9)*8, which for B ≤ 1000 is at most 80,072, well within 32-bit int range. The time complexity is O(1) because at most 10 iterations are performed. Space complexity is O(1).
