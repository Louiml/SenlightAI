// Write a C++ function `int maxCandyTypes(vector<int>& candyType)` that takes a vector of integers representing candy types (each integer is a type of candy) and returns the maximum number of different candy types Alice can eat, given that she can only eat exactly half of the total candies. The function must not reorder the original vector's contents conceptually (you may sort a copy or use a set), and must handle cases where the number of distinct types is less than, equal to, or greater than half the total size. The vector may contain duplicates, negative numbers, and its size is always even. The function should be efficient for large inputs.

// The goal is to maximize the variety of candies eaten while being limited to eating exactly half of the total candies. The optimal strategy is to eat as many distinct types as possible, but never exceeding the half limit. Therefore, the answer is the minimum of two quantities: the number of distinct candy types present, and half the total number of candies.  
// To count distinct types, we can either sort a copy of the vector and then iterate, skipping duplicates, or use an unordered_set for O(1) average insertions. Sorting takes O(N log N) time and O(1) extra space (if we sort in place, but that modifies the input—so we either copy or sort in place if allowed; the problem statement says "vector<int>&" so we may sort it in place if that's acceptable, but to be safe, we can copy). Alternatively, using an unordered_set takes O(N) average time and O(N) space. The edge cases include: when all candies are the same type (answer is 1, which is less than half), when every type is distinct (answer is half), and when the number of distinct types is between 1 and half. Time complexity: O(N log N) for sorting or O(N) for set; space: O(1) if sorting in place, O(N) for set.

#include <vector>
#include <algorithm>
#include <unordered_set>

// Returns the maximum number of distinct candy types Alice can eat,
// given she can eat exactly half of the total candies.
int maxCandyTypes(std::vector<int>& candyType) {
    const size_t canEat = candyType.size() / 2;
    std::unordered_set<int> distinct(candyType.begin(), candyType.end());
    const size_t distinctCount = distinct.size();
    return static_cast<int>(std::min(canEat, distinctCount));
}

#include <cassert>
#include <vector>

int maxCandyTypes(std::vector<int>& candyType); // forward declaration

int main() {
    std::vector<int> test1 = {1, 1, 2, 2, 3, 3};
    assert(maxCandyTypes(test1) == 3); // 3 distinct, can eat 3

    std::vector<int> test2 = {1, 1, 2, 3};
    assert(maxCandyTypes(test2) == 2); // 3 distinct, can eat 2

    std::vector<int> test3 = {6, 6, 6, 6};
    assert(maxCandyTypes(test3) == 1); // 1 distinct, can eat 2 -> min is 1

    std::vector<int> test4 = {1, 2, 3, 4};
    assert(maxCandyTypes(test4) == 2); // 4 distinct, can eat 2

    std::vector<int> test5 = {-1, -1, 0, 1, 1, 2};
    assert(maxCandyTypes(test5) == 3); // 4 distinct, can eat 3

    std::vector<int> test6 = {0, 0, 0, 0, 0, 0};
    assert(maxCandyTypes(test6) == 1); // 1 distinct, can eat 3 -> min 1

    std::vector<int> test7 = {7};
    // Note: size is odd, but problem says even; still test for robustness
    // The function uses size()/2 which is 0 for size 1, but let's not include odd sizes.
    // Instead, test with even size.
    std::vector<int> test8 = {1, 2};
    assert(maxCandyTypes(test8) == 1); // 2 distinct, can eat 1

    std::vector<int> test9 = {100, 200, 100, 200, 300, 300, 400, 500};
    assert(maxCandyTypes(test9) == 4); // 5 distinct, can eat 4

    std::vector<int> test10 = {1, 1, 1, 2, 2, 2, 3, 3, 3, 4};
    // size 10, can eat 5, distinct = 4
    assert(maxCandyTypes(test10) == 4);
}
