// You are given a vector `amount` of exactly three positive integers, where each integer represents the number of cups of a different type (e.g., hot water, milk, and coffee) needed to prepare some drinks. In one operation, you can fill either one cup of any type or two cups of different types simultaneously. Write a C++ function `int minimumOperations(std::vector<int> amount)` that returns the minimum number of operations required to fill all cups (i.e., reduce all three counts to zero). The input vector always contains exactly three elements, each between 1 and 1000 inclusive. The function should handle cases where the largest type dominates, and where the counts are balanced. Do not modify the input vector.

// The key insight is that the most efficient strategy is always to pair the two largest types whenever possible, because filling two at once saves an operation. Sort the three values in non-decreasing order so `a <= b <= c`. Two cases arise:  
// 1. If `a + b <= c`, then the two smaller types together cannot keep up with the largest type. We can pair each of the smaller cups with a cup from the largest type, and after they are exhausted, only the largest type remains, requiring exactly `c` operations. This is optimal because every operation can fill at most two cups, and we cannot reduce below `c` since the largest type alone needs at least `c` operations.  
// 2. If `a + b > c`, then all types can be paired such that no operation fills only one cup except possibly the last. The total number of cups is `a + b + c`. In the best case, we perform `ceil(total / 2)` operations, because each operation fills at most 2 cups and the sum is even or odd. This bound is achievable by always pairing the two currently largest types.  
// Edge cases: all equal values (e.g., `[5,5,5]`) fall into case 2; a single dominant value (e.g., `[1,1,100]`) falls into case 1; and minimal values like `[1,1,1]` yield 2 operations. Time complexity is O(1) because sorting three elements is constant, and space complexity is O(1).

#include <vector>
#include <algorithm>

// Returns the minimum number of operations to fill all cups.
int minimumOperations(std::vector<int> amount) {
    // Sort the three counts in non-decreasing order.
    std::sort(amount.begin(), amount.end());
    int a = amount[0];
    int b = amount[1];
    int c = amount[2];
    
    // If the two smaller types together cannot match the largest,
    // each operation must pair with the largest, so c operations are needed.
    if (a + b <= c) {
        return c;
    }
    // Otherwise, pair two cups per operation as much as possible.
    // The ceiling of total/2 is the minimum.
    return (a + b + c + 1) / 2;
}

#include <cassert>
#include <vector>

int minimumOperations(std::vector<int> amount);

int main() {
    assert(minimumOperations({1, 4, 2}) == 4);
    assert(minimumOperations({5, 4, 4}) == 7);
    assert(minimumOperations({1, 1, 1}) == 2);
    assert(minimumOperations({100, 1, 1}) == 100);
    assert(minimumOperations({2, 2, 2}) == 3);
    assert(minimumOperations({3, 5, 4}) == 6);
    assert(minimumOperations({1, 1, 1000}) == 1000);
    assert(minimumOperations({10, 10, 10}) == 15);
    assert(minimumOperations({7, 3, 8}) == 9);
    assert(minimumOperations({1, 999, 1}) == 999);
    return 0;
}
