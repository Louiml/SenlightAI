// Write a C++ function named `stockSpan` that simulates the online stock span problem. The function should accept a vector of daily stock prices (positive integers) and return a vector of integers where each element at index `i` represents the number of consecutive days (including the current day) the price on day `i` was less than or equal to the price on day `i` (going backward). For example, given prices `[100, 80, 60, 70, 60, 75, 85]`, the span for day 0 (100) is 1, day 1 (80) is 1, day 2 (60) is 1, day 3 (70) is 2 (days 2 and 3), day 4 (60) is 1, day 5 (75) is 4 (days 2,3,4,5), and day 6 (85) is 6 (days 1–6). The input vector may be empty, in which case return an empty vector. Ensure the solution uses a stack-based approach for efficiency.

#include <cassert>
#include <vector>

int main() {
    // Basic example from the problem description
    std::vector<int> prices1 = {100, 80, 60, 70, 60, 75, 85};
    std::vector<int> expected1 = {1, 1, 1, 2, 1, 4, 6};
    assert(stockSpan(prices1) == expected1);

    // Empty input
    std::vector<int> prices2 = {};
    assert(stockSpan(prices2).empty());

    // Single element
    std::vector<int> prices3 = {42};
    assert(stockSpan(prices3) == std::vector<int>{1});

    // Strictly decreasing prices → all spans are 1
    std::vector<int> prices4 = {5, 4, 3, 2, 1};
    assert(stockSpan(prices4) == std::vector<int>({1, 1, 1, 1, 1}));

    // Strictly increasing prices → spans are 1,2,3,...
    std::vector<int> prices5 = {1, 2, 3, 4, 5};
    assert(stockSpan(prices5) == std::vector<int>({1, 2, 3, 4, 5}));

    // All equal prices → spans accumulate
    std::vector<int> prices6 = {7, 7, 7, 7};
    assert(stockSpan(prices6) == std::vector<int>({1, 2, 3, 4}));

    // Mixed with duplicate values
    std::vector<int> prices7 = {10, 10, 5, 6, 6};
    // Day0:10→1, Day1:10→2, Day2:5→1, Day3:6→2, Day4:6→3 (days 2,3,4)
    assert(stockSpan(prices7) == std::vector<int>({1, 2, 1, 2, 3}));
}

#include <vector>
#include <stack>

// Compute the stock span for each day's price.
// Each output element is the number of consecutive previous days
// (including the current day) where the price was <= current price.
std::vector<int> stockSpan(const std::vector<int>& prices) {
    std::vector<int> result;
    result.reserve(prices.size());
    
    std::stack<std::pair<int, int>> st; // {price, span}
    
    for (int price : prices) {
        int span = 1;
        while (!st.empty() && st.top().first <= price) {
            span += st.top().second;
            st.pop();
        }
        st.push({price, span});
        result.push_back(span);
    }
    
    return result;
}

// The classic stack-based method processes prices sequentially. Maintain a stack of pairs `(price, span)` where `price` is the stock price and `span` is the cumulative span for that price. For each new price, start with a span of 1. While the stack is non-empty and the top price is less than or equal to the current price, pop the top and add its span to the current span. This works because all consecutive previous days with prices ≤ current price are "covered" by those popped entries, and their spans are merged. After merging, push the current `(price, span)` onto the stack. The returned span for each day is the computed `span` value. Edge cases: empty input returns empty vector; a monotonically decreasing sequence always yields span 1; a monotonically increasing sequence yields spans 1,2,3,...n. Time complexity is O(n) because each price is pushed and popped at most once. Space complexity is O(n) in the worst case (e.g., strictly decreasing prices keep all entries on the stack).
