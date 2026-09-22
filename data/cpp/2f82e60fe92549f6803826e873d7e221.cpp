Write a C++ function `vector<int> stockSpan(const vector<int>& prices)` that, given a vector of daily stock prices (one integer per day in chronological order), returns a vector where the element at index `i` is the number of consecutive days (including the current day) immediately before and including `i` for which the price was less than or equal to `prices[i]`. For example, if prices are `[100, 80, 60, 70, 60, 75, 85]`, the output should be `[1, 1, 1, 2, 1, 4, 6]`. The input vector will not be empty, and prices can be zero or positive integers. The function must be efficient for large input sizes (up to 10^5 elements).

The solution uses a monotonic decreasing stack that stores indices of prices. Iterate through the array from left to right. For each price, pop indices from the stack while the price at the stack's top is less than or equal to the current price, because those previous prices will no longer be able to "see" past the current higher (or equal) price. After popping, if the stack is empty, that means all previous prices are <= current, so the span is `i + 1` (all days up to and including today). If the stack is not empty, the top index is the nearest day to the left with a price strictly greater than the current price; the span is `i - st.top()`. Then push the current index. Edge cases: equal prices are treated as "less than or equal" for spanning (so a price equal to a previous one extends the span); single-element input yields `[1]`; strictly rising prices yield spans `[1,2,3,...]`, and strictly falling prices yield all ones. Time complexity is O(n) because each index is pushed and popped at most once. Space complexity is O(n) for the stack and answer vector.

#include <vector>
#include <stack>

// Calculate the stock span for each day.
// Span[i] = number of consecutive previous days (including i)
// with price <= prices[i].
std::vector<int> stockSpan(const std::vector<int>& prices) {
    const int n = static_cast<int>(prices.size());
    std::vector<int> span(n, 0);
    std::stack<int> stack; // stores indices with strictly decreasing prices

    for (int i = 0; i < n; ++i) {
        // Pop all indices whose price is <= current price
        while (!stack.empty() && prices[stack.top()] <= prices[i]) {
            stack.pop();
        }
        // If stack empty, all previous prices are <= current
        if (stack.empty()) {
            span[i] = i + 1;
        } else {
            // Nearest index to the left with price > current
            span[i] = i - stack.top();
        }
        stack.push(i);
    }
    return span;
}

#include <cassert>
#include <vector>

// The solution function is declared above.
int main() {
    // Basic examples
    std::vector<int> prices1 = {100, 80, 60, 70, 60, 75, 85};
    std::vector<int> expected1 = {1, 1, 1, 2, 1, 4, 6};
    assert(stockSpan(prices1) == expected1);

    // Single element
    std::vector<int> prices2 = {10};
    std::vector<int> expected2 = {1};
    assert(stockSpan(prices2) == expected2);

    // All equal prices
    std::vector<int> prices3 = {5, 5, 5, 5};
    std::vector<int> expected3 = {1, 2, 3, 4};
    assert(stockSpan(prices3) == expected3);

    // Strictly increasing
    std::vector<int> prices4 = {1, 2, 3, 4, 5};
    std::vector<int> expected4 = {1, 2, 3, 4, 5};
    assert(stockSpan(prices4) == expected4);

    // Strictly decreasing
    std::vector<int> prices5 = {5, 4, 3, 2, 1};
    std::vector<int> expected5 = {1, 1, 1, 1, 1};
    assert(stockSpan(prices5) == expected5);

    // Mixed with zeros
    std::vector<int> prices6 = {0, 0, 1, 0, 2};
    std::vector<int> expected6 = {1, 2, 3, 1, 5};
    assert(stockSpan(prices6) == expected6);

    // Random test
    std::vector<int> prices7 = {3, 1, 2, 4, 1, 1, 5};
    std::vector<int> expected7 = {1, 1, 2, 4, 1, 2, 7};
    assert(stockSpan(prices7) == expected7);

    return 0;
}
