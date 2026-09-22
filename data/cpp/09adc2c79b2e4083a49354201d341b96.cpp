/*
Write a C++ function `int calculateReceipt(int pattern, int price, const std::string& name, int quantity)` that mimics the logic of the given code snippet. The function receives: `pattern` (either 1 or 2), `price` (base price per item), `name` (a string, only used when `pattern == 2`), and `quantity` (number of items). The function must return the total cost as `price * quantity`. However, when `pattern == 2`, it must also (as a side effect) print to standard output the name followed by an exclamation mark on its own line, exactly like the snippet does. For `pattern == 1`, no extra printing occurs. The function should handle only patterns 1 and 2; assume valid input. Ensure the output formatting is identical: for pattern 2, print `name + "!"` followed by a newline, then the final total is returned.
*/

#include <string>
#include <iostream>

// Returns total cost = price * quantity.
// If pattern == 2, prints name followed by '!' and a newline.
int calculateReceipt(int pattern, int price, const std::string& name, int quantity) {
    if (pattern == 2) {
        std::cout << name << "!" << std::endl;
    }
    return price * quantity;
}

#include <cassert>
#include <sstream>
#include <iostream>
#include <string>

// Declare the solution function
int calculateReceipt(int pattern, int price, const std::string& name, int quantity);

int main() {
    // Test pattern 1: no output, just product
    assert(calculateReceipt(1, 100, "unused", 3) == 300);
    
    // Test pattern 1 with zero quantity
    assert(calculateReceipt(1, 200, "unused", 0) == 0);
    
    // Test pattern 1 with negative quantity
    assert(calculateReceipt(1, 50, "unused", -2) == -100);
    
    // Test pattern 2: check output and product
    {
        std::ostringstream oss;
        std::streambuf* oldCout = std::cout.rdbuf(oss.rdbuf());
        int result = calculateReceipt(2, 80, "apple", 5);
        std::cout.rdbuf(oldCout);
        assert(result == 400);
        assert(oss.str() == "apple!\n");
    }
    
    // Test pattern 2 with different name and quantity
    {
        std::ostringstream oss;
        std::streambuf* oldCout = std::cout.rdbuf(oss.rdbuf());
        int result = calculateReceipt(2, 10, "x", 1);
        std::cout.rdbuf(oldCout);
        assert(result == 10);
        assert(oss.str() == "x!\n");
    }
    
    // Test pattern 1 with quantity 1 and price 0
    assert(calculateReceipt(1, 0, "whatever", 1) == 0);
    
    // Test pattern 2 with price 0
    {
        std::ostringstream oss;
        std::streambuf* oldCout = std::cout.rdbuf(oss.rdbuf());
        int result = calculateReceipt(2, 0, "free", 999);
        std::cout.rdbuf(oldCout);
        assert(result == 0);
        assert(oss.str() == "free!\n");
    }

    // Test pattern 1 with large numbers
    assert(calculateReceipt(1, 12345, "ignored", 6789) == 12345 * 6789);
    
    // Test pattern 2 with name containing spaces
    {
        std::ostringstream oss;
        std::streambuf* oldCout = std::cout.rdbuf(oss.rdbuf());
        int result = calculateReceipt(2, 5, "my item", 2);
        std::cout.rdbuf(oldCout);
        assert(result == 10);
        assert(oss.str() == "my item!\n");
    }

    // Test pattern 2 with empty name
    {
        std::ostringstream oss;
        std::streambuf* oldCout = std::cout.rdbuf(oss.rdbuf());
        int result = calculateReceipt(2, 7, "", 4);
        std::cout.rdbuf(oldCout);
        assert(result == 28);
        assert(oss.str() == "!\n");
    }

    return 0;
}

// The core logic is straightforward: the total is always `price * quantity`, but the function must perform an additional output action depending on the pattern. The snippet reads `p`, then conditionally reads `price` (pattern 1) or `text` and `price` (pattern 2), then reads `N`, prints `text!` for pattern 2, and finally prints the product. In the function, we directly take all inputs as parameters, so no reading is needed. The main steps:  
// 1. If `pattern == 2`, output `name` followed by `!` and a newline.  
// 2. Return `price * quantity`.  
//
// Edge cases:  
// - For pattern 1, `name` is ignored; the function still works because we only use it in the condition.  
// - `quantity` could be negative or zero; multiplication is still correct.  
// - The function must output exactly `name!` with no spaces, and a newline.  
// - Since the snippet uses `int` for all numeric values, we keep the same.  
//
// Time complexity: O(1) (constant time, ignoring print overhead). Space complexity: O(1) auxiliary (no extra storage beyond parameters).
