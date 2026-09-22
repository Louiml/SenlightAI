// Write a C++ function named `determineWinner` that takes a vector of positive integers (each greater than 0) and returns a string `"Alice"` or `"Bob"` based on a game rule derived from the classic "divide by gcd" game. The rule is: if `(maxElement / gcdOfAll - n)` is odd, the winner is `"Alice"`, otherwise `"Bob"`, where `n` is the number of elements. The vector will always have at least one element, and the integers can be up to `10^9`. The function must not modify the input vector and must use constant space (no extra arrays of size dependent on input) besides the built-in gcd computation.
// The solution identifies two key values from the vector: the greatest common divisor (gcd) of all elements and the maximum element.  
// - The gcd is computed iteratively using `std::gcd` (C++17) or `__gcd` (GNU), starting from the first element and updating with each subsequent element.  
// - The maximum is tracked similarly with `std::max`.  
// - Then the formula `(maxVal / gcd - n) & 1` determines parity. If the result is odd, return `"Alice"`; even, return `"Bob"`.  
// Edge cases:  
// - Single element: gcd = that element, max = same, so `(1 - 1) = 0` → Bob.  
// - All elements equal: gcd = max, so `(1 - n)` can be negative. In C++, negative numbers modulo 2 with bitwise AND works correctly for odd/even checks (negative odd yields 1, negative even yields 0).  
// - Large values: division and subtraction fit in 64-bit, but 32-bit may overflow; use `long long` internally.  
// Time complexity: O(n) for a single pass, O(1) auxiliary space.
#include <vector>
#include <string>
#include <numeric>
#include <algorithm>

// Determine winner based on game rule: if (max/gcd - n) is odd, Alice wins, else Bob.
std::string determineWinner(const std::vector<int>& numbers) {
    if (numbers.empty()) return "Bob"; // not expected by spec, safe fallback
    
    long long gcdVal = numbers[0];
    long long maxVal = numbers[0];
    const long long n = static_cast<long long>(numbers.size());
    
    for (size_t i = 1; i < numbers.size(); ++i) {
        gcdVal = std::gcd(gcdVal, static_cast<long long>(numbers[i]));
        maxVal = std::max(maxVal, static_cast<long long>(numbers[i]));
    }
    
    long long moves = (maxVal / gcdVal) - n;
    return (moves & 1LL) ? "Alice" : "Bob";
}
#include <cassert>
#include <vector>
#include <string>

// Include the solution function (assuming it's defined above in the same translation unit)
std::string determineWinner(const std::vector<int>&);

int main() {
    // Single element: max/gcd = 1, n = 1 → 0 → Bob
    assert(determineWinner({5}) == "Bob");
    
    // Classic example: [2, 2] → max/gcd = 1, n = 2 → -1 (odd) → Alice
    assert(determineWinner({2, 2}) == "Alice");
    
    // [4, 6] → gcd=2, max=6 → 6/2 - 2 = 3 - 2 = 1 (odd) → Alice
    assert(determineWinner({4, 6}) == "Alice");
    
    // [8, 12, 16] → gcd=4, max=16 → 16/4 - 3 = 4 - 3 = 1 (odd) → Alice
    assert(determineWinner({8, 12, 16}) == "Alice");
    
    // [10, 15] → gcd=5, max=15 → 15/5 - 2 = 3 - 2 = 1 → Alice
    assert(determineWinner({10, 15}) == "Alice");
    
    // [7, 7, 7] → gcd=7, max=7 → 1 - 3 = -2 (even) → Bob
    assert(determineWinner({7, 7, 7}) == "Bob");
    
    // [2, 3, 5] → gcd=1, max=5 → 5 - 3 = 2 (even) → Bob
    assert(determineWinner({2, 3, 5}) == "Bob");
    
    // [1000000000, 2000000000] → gcd=1000000000, max=2000000000 → 2 - 2 = 0 → Bob
    assert(determineWinner({1000000000, 2000000000}) == "Bob");
    
    // [1, 2] → gcd=1, max=2 → 2 - 2 = 0 → Bob
    assert(determineWinner({1, 2}) == "Bob");
    
    // [100, 100, 100] → gcd=100, max=100 → 1 - 3 = -2 → Bob
    assert(determineWinner({100, 100, 100}) == "Bob");
    
    return 0;
}
