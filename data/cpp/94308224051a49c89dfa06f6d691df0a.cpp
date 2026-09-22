// Write a C++ function `findSingleNumber` that takes a non-empty `std::vector<int>` where every element appears exactly twice except for one element that appears exactly once, and returns that unique element. You must not modify the input vector (keep it `const`). The function should work correctly for vectors of size 1 (where the single element is the answer) and for vectors containing negative numbers, zeros, and large values, including duplicates in any order. Do not rely on sorting, hash maps, hash sets, or any container that introduces extra memory beyond a few scalar variables—use a bitwise operation to achieve an optimal solution in both time and space.
The optimal approach uses the XOR bitwise operator. The property of XOR is that `a ^ a = 0` and `a ^ 0 = a`, and it is both commutative and associative. Therefore, XORing all elements in the vector cancels out every pair of identical numbers (each pair yields 0), leaving only the single number that appears once. This works for any integer values, including negatives (two's complement representation) and zeros. The algorithm iterates through the vector exactly once, accumulating the XOR result in a single integer variable initialized to 0. Edge cases: if the vector has size 1, the loop runs once and returns that element; if the unique element is 0 or negative, it is handled naturally. Time complexity is O(n), where n is the number of elements. Space complexity is O(1), as only one integer accumulator is used. No modification of the input is needed, so const correctness is maintained.
#include <vector>

// Returns the element that appears only once in a vector where every other element appears twice.
int findSingleNumber(const std::vector<int>& nums) {
    int result = 0;
    for (int num : nums) {
        result ^= num;
    }
    return result;
}
#include <cassert>
#include <vector>

// (Solution function is assumed to be defined above)

int main() {
    // Basic case with positive numbers
    std::vector<int> v1 = {2, 2, 1};
    assert(findSingleNumber(v1) == 1);

    // Case with negative numbers
    std::vector<int> v2 = {-3, -3, -7, -7, 5};
    assert(findSingleNumber(v2) == 5);

    // Case with zero as the single element
    std::vector<int> v3 = {4, 0, 4, 9, 9};
    assert(findSingleNumber(v3) == 0);

    // Case with only one element
    std::vector<int> v4 = {42};
    assert(findSingleNumber(v4) == 42);

    // Case with larger numbers and duplicates in random order
    std::vector<int> v5 = {1000000, 1000000, -1000000, -1000000, 12345};
    assert(findSingleNumber(v5) == 12345);

    // Case with repeated pairs and the single element at the end
    std::vector<int> v6 = {1, 2, 3, 2, 1, 4, 4, 5, 5};
    assert(findSingleNumber(v6) == 3);

    // Case with all zeros except one
    std::vector<int> v7 = {0, 0, 0, 0, 7};
    assert(findSingleNumber(v7) == 7);

    // Edge: single negative number
    std::vector<int> v8 = {-1};
    assert(findSingleNumber(v8) == -1);

    // Edge: two identical numbers and one single (size 3)
    std::vector<int> v9 = {10, 10, -20};
    assert(findSingleNumber(v9) == -20);

    // Edge: many pairs, unique is the smallest or largest
    std::vector<int> v10 = {100, 1, 100, 2, 2, 3, 3, 1, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9};
    assert(findSingleNumber(v10) == 0); // Wait, this includes 0? Actually compute: pairs cancel, only 0 remains? No, there's no 0. Let's adjust: we need a correct test.
}

**Correction for the last test**: The provided vector in the test has no single non-paired element—it contains pairs from 1 to 9 and 100 appears twice, so all cancel? Let's replace it with a valid test:  
#include <cassert>
#include <vector>

// (Solution function is assumed to be defined above)

int main() {
    // Basic case with positive numbers
    std::vector<int> v1 = {2, 2, 1};
    assert(findSingleNumber(v1) == 1);

    // Case with negative numbers
    std::vector<int> v2 = {-3, -3, -7, -7, 5};
    assert(findSingleNumber(v2) == 5);

    // Case with zero as the single element
    std::vector<int> v3 = {4, 0, 4, 9, 9};
    assert(findSingleNumber(v3) == 0);

    // Case with only one element
    std::vector<int> v4 = {42};
    assert(findSingleNumber(v4) == 42);

    // Case with larger numbers and duplicates in random order
    std::vector<int> v5 = {1000000, 1000000, -1000000, -1000000, 12345};
    assert(findSingleNumber(v5) == 12345);

    // Case with repeated pairs and the single element at the end
    std::vector<int> v6 = {1, 2, 3, 2, 1, 4, 4, 5, 5};
    assert(findSingleNumber(v6) == 3);

    // Case with all zeros except one
    std::vector<int> v7 = {0, 0, 0, 0, 7};
    assert(findSingleNumber(v7) == 7);

    // Edge: single negative number
    std::vector<int> v8 = {-1};
    assert(findSingleNumber(v8) == -1);

    // Edge: two identical numbers and one single (size 3)
    std::vector<int> v9 = {10, 10, -20};
    assert(findSingleNumber(v9) == -20);

    // Many pairs with unique as the largest
    std::vector<int> v10 = {1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 99};
    assert(findSingleNumber(v10) == 99);
}
