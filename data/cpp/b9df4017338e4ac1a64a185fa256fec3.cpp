Write a C++ function named `isHappyNumber` that takes an integer `n` and returns `true` if `n` is a "happy number", and `false` otherwise. A happy number is defined by the following process: starting with any positive integer, replace the number by the sum of the squares of its digits, and repeat the process until the number equals 1 (where it will stay), or it loops endlessly in a cycle that does not include 1. Return `false` for non-positive inputs (including zero and negative numbers). The function must not use recursion, but you may use a `std::set` or `std::unordered_set` to detect cycles. Ensure the function is `const`-correct and uses appropriate types.

#include <cassert>

int main() {
    // Basic happy numbers
    assert(isHappyNumber(1) == true);
    assert(isHappyNumber(7) == true);
    assert(isHappyNumber(10) == true);
    assert(isHappyNumber(13) == true);
    
    // Basic unhappy numbers
    assert(isHappyNumber(2) == false);
    assert(isHappyNumber(3) == false);
    assert(isHappyNumber(4) == false);
    assert(isHappyNumber(5) == false);
    
    // Edge cases: non-positive inputs
    assert(isHappyNumber(0) == false);
    assert(isHappyNumber(-1) == false);
    assert(isHappyNumber(-100) == false);
    
    // Larger happy number
    assert(isHappyNumber(19) == true);
    assert(isHappyNumber(100) == true);
    
    // Larger unhappy number
    assert(isHappyNumber(20) == false);
    assert(isHappyNumber(123) == false);
    
    return 0;
}

#include <set>

// Returns true if the positive integer n is a happy number.
// A happy number eventually reaches 1 under repeated sum of squared digits.
// Non-positive inputs are considered unhappy.
bool isHappyNumber(int n) {
    if (n <= 0) {
        return false;
    }
    if (n == 1) {
        return true;
    }
    
    std::set<int> seen;
    int current = n;
    
    while (true) {
        int sum = 0;
        int temp = current;
        while (temp > 0) {
            int digit = temp % 10;
            sum += digit * digit;
            temp /= 10;
        }
        
        if (sum == 1) {
            return true;
        }
        
        // If this sum has appeared before, we are in a cycle.
        if (seen.find(sum) != seen.end()) {
            return false;
        }
        seen.insert(sum);
        current = sum;
    }
}

// The algorithm repeatedly computes the sum of the squares of the digits of the current number. If the result equals 1, the number is happy. Otherwise, check if the result has been seen before: if yes, a cycle exists and the number is not happy; if no, store the result in a set and continue with the new number as the current value. Edge cases: any non-positive input immediately returns `false` because the definition assumes positive integers. The special case `n == 1` returns `true` because the process ends immediately. For time complexity, in the worst case the number of iterations is bounded by the number of distinct sums possible; for a `d`-digit number, the maximum sum of squares is at most `81*d`, so the sequence quickly enters a small cycle; thus, it runs in nearly constant time for practical integer sizes, but formally it is `O(log n)` iterations with each iteration costing `O(log n)` digit operations, giving `O((log n)^2)`. Space complexity is `O(log n)` for the set storing the sums. The use of `std::set` is fine though `std::unordered_set` would give expected `O(1)` per lookup; `std::set` gives `O(log k)` lookup where `k` is the number of stored values.
