// Write a C++ function `int distributeCandies(const std::vector<int>& candyType)` that takes a vector of integers where each integer represents a type of candy. The function must return the maximum number of different types of candies a person can eat if they can eat at most half of the total candies. The input vector is guaranteed to be non-empty and have even length. For example, if `candyType = {1,1,2,2,3,3}`, the person can eat at most 3 candies and can choose one of each type, so the result is 3. If `candyType = {1,1,2,3}`, they can eat 2 candies and can choose types 2 and 3 (or 1 and 2), so the result is 2. The function should handle duplicate types and large inputs efficiently.
The key observation is that the answer is the minimum of two quantities: the maximum number of candies the person is allowed to eat (which is `candyType.size() / 2`) and the number of distinct candy types (which is the size of a set of unique elements). If there are more distinct types than the allowed half, the person can pick at most half distinct types. If there are fewer distinct types than half, the person can eat all distinct types (and must repeat some types to fill the remaining allowance). The algorithm: iterate through the vector, insert each element into an `unordered_set` to count unique types. Then return the smaller of `candyType.size() / 2` and the set size. Edge cases: all same type (result 1 if size ≥ 2), or all distinct (result is exactly half). Time complexity is O(n) on average due to hash set operations, and space complexity is O(k) where k is the number of distinct types, at most n. No special handling for size zero as vector is non-empty and even length per problem constraints.
#include <vector>
#include <unordered_set>
#include <algorithm>

// Return the maximum number of different candy types that can be eaten,
// given that at most half of the total candies can be consumed.
int distributeCandies(const std::vector<int>& candyType) {
    std::unordered_set<int> uniqueTypes;
    for (int type : candyType) {
        uniqueTypes.insert(type);
    }
    return std::min(candyType.size() / 2, uniqueTypes.size());
}
#include <cassert>
#include <vector>
#include <functional>

// Forward declaration for testing (the function is defined above)
int distributeCandies(const std::vector<int>& candyType);

int main() {
    // Basic case with more distinct types than allowed
    assert(distributeCandies({1,1,2,2,3,3}) == 3);
    // Duplicates reduce distinct types below half
    assert(distributeCandies({1,1,2,3}) == 2);
    // All same type -> only 1 distinct
    assert(distributeCandies({7,7,7,7}) == 1);
    // All distinct types -> exactly half
    assert(distributeCandies({1,2,3,4}) == 2);
    // Minimal even-length vector
    assert(distributeCandies({5,5}) == 1);
    assert(distributeCandies({5,6}) == 1);
    // Larger example with many duplicates
    std::vector<int> test = {1,1,1,2,2,3,3,3,4,4};
    assert(distributeCandies(test) == 4);
    // Extreme duplicate: half of types are distinct
    assert(distributeCandies({10,10,20,20,30,30,40,40}) == 4);
    // Edge: only two types present but many candies
    assert(distributeCandies({1,1,1,2,2,2,1,1}) == 2);
    return 0;
}
