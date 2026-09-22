Write a C++ function named `stockSpan` that simulates the Stock Spanner problem: it processes a stream of daily stock prices (given as a vector of integers) and returns a vector of integers where each element at index `i` represents the number of consecutive days (including today) up to and including day `i` for which the price on that day is less than or equal to the price on day `i`. In other words, for each price, find the span as the distance to the previous greater price (or the beginning of the array if no such price exists). For example, given prices `{100, 80, 60, 70, 60, 75, 85}`, the spans are `{1, 1, 1, 2, 1, 4, 6}`. The function should handle an empty input by returning an empty vector, and it should work efficiently for large sequences (up to 100,000 prices) with prices possibly repeated. The solution must use a monotonic stack approach to achieve linear time complexity.
#include <cassert>
#include <vector>

int main() {
    // Empty input
    assert(stockSpan({}) == std::vector<int>({}));
    
    // Single price
    assert(stockSpan({10}) == std::vector<int>({1}));
    
    // Standard example
    std::vector<int> prices1 = {100, 80, 60, 70, 60, 75, 85};
    std::vector<int> expected1 = {1, 1, 1, 2, 1, 4, 6};
    assert(stockSpan(prices1) == expected1);
    
    // Strictly increasing prices
    std::vector<int> prices2 = {1, 2, 3, 4, 5};
    std::vector<int> expected2 = {1, 2, 3, 4, 5};
    assert(stockSpan(prices2) == expected2);
    
    // Strictly decreasing prices
    std::vector<int> prices3 = {5, 4, 3, 2, 1};
    std::vector<int> expected3 = {1, 1, 1, 1, 1};
    assert(stockSpan(prices3) == expected3);
    
    // With duplicates
    std::vector<int> prices4 = {10, 10, 10, 5, 10};
    std::vector<int> expected4 = {1, 2, 3, 1, 4};
    assert(stockSpan(prices4) == expected4);
    
    // All equal
    std::vector<int> prices5 = {7, 7, 7, 7};
    std::vector<int> expected5 = {1, 2, 3, 4};
    assert(stockSpan(prices5) == expected5);
    
    // Mixed example with reset
    std::vector<int> prices6 = {30, 20, 25, 20, 30};
    std::vector<int> expected6 = {1, 1, 2, 1, 5};
    assert(stockSpan(prices6) == expected6);
    
    return 0;
}
#include <vector>
#include <stack>

// Given a vector of daily stock prices, return the stock span for each day.
// Span at index i = number of consecutive days (including i) with price <= price[i].
std::vector<int> stockSpan(const std::vector<int>& prices) {
    std::vector<int> spans;
    spans.reserve(prices.size());
    std::stack<std::pair<int, int>> st; // {price, span}
    
    for (int price : prices) {
        int span = 1;
        while (!st.empty() && st.top().first <= price) {
            span += st.top().second;
            st.pop();
        }
        st.push({price, span});
        spans.push_back(span);
    }
    return spans;
}
// The problem is the classic Stock Spanner. The key insight is to maintain a stack of pairs `(price, span)` where each entry represents a price and the span computed for that price at the time it was pushed. As we iterate through each price, we initialize a current span of 1 and then repeatedly pop from the stack while the top price is less than or equal to the current price, accumulating the popped span into the current span. This works because any previous price that is ≤ the current price is "covered" by the current day (since the span counts consecutive days with prices ≤ current). Once we pop all smaller or equal prices, the stack top (if any) is strictly greater than the current price, meaning the current span is correctly computed. We then push the current price with its span. This maintains a strictly decreasing stack of prices from bottom to top, ensuring each price is pushed and popped at most once, so the total time is O(n) and auxiliary space is O(n) in the worst case (when prices are strictly increasing). Edge cases: empty input returns an empty vector; single price returns [1]; duplicate prices: when a duplicate is encountered, the previous equal price is popped (since condition is `<=`), so the span includes all consecutive equal or smaller prices. The stack stores the span at the time of pushing, which is correct because spans of popped items are accumulated and never change afterward.
