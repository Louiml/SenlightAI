Given an integer array `nums` and a list of queries where each query is `[val, index]`, write a C++ function `vector<int> sumEvenAfterQueries(vector<int>& nums, vector<vector<int>>& queries)` that processes each query by adding `val` to `nums[index]`, then after each update records the sum of all even-valued elements in the current array into the result vector. The function must modify the array in place, process queries sequentially from first to last, and return a vector of sums with the same length as `queries`. The input array will contain at least one element, and indices in queries are guaranteed valid. The values in `nums` and queries can be negative, zero, or positive integers. Your implementation should avoid recomputing the even sum from scratch after every query to achieve efficiency, and must handle the four possible parity-change scenarios when an element is updated: even→even, even→odd, odd→even, and odd→odd. The function should have proper `const` correctness where applicable (though the array must be mutable), include necessary headers, and provide clear comments. Do not write a `main` function in the solution section.
The core idea is to maintain a running total `s` of all even elements in `nums` before processing any queries, then update this total incrementally as each query modifies one element. For each query `[v, i]`, retrieve the old value `old = nums[i]`, compute the new value `nums[i] = old + v`, and adjust `s` based on the parity transition:  
- If both old and new are even, the sum changes by `v` (since only that element changed and it remains even).  
- If old is odd and new is even, the new element becomes even, so add `nums[i]` to `s`.  
- If old is even and new is odd, the element leaves the even set, so subtract `old` from `s`.  
- If both are odd, the sum is unchanged.  
After updating `s`, push its current value into the answer vector. Edge cases include negative values (where `%` yields non-negative remainder in C++ for positive divisors, but any negative even number still satisfies `num % 2 == 0`), zero (which is even), and queries where `v` is zero or negative. The algorithm runs in `O(n + q)` time, where `n` is the length of `nums` and `q` is the number of queries, and uses `O(1)` extra space besides the result vector (which is `O(q)` for the output). The precomputation of the initial even sum is `O(n)`, and each query update is `O(1)`.
#include <vector>

// Given an array and a list of queries [val, index], add val to nums[index]
// and record the current sum of all even numbers after each query.
// Returns a vector of sums, one for each query in order.
std::vector<int> sumEvenAfterQueries(std::vector<int>& nums,
                                     const std::vector<std::vector<int>>& queries) {
    // Compute initial sum of all even numbers in nums.
    int evenSum = 0;
    for (int num : nums) {
        if (num % 2 == 0) {
            evenSum += num;
        }
    }

    std::vector<int> result;
    result.reserve(queries.size());

    for (const auto& query : queries) {
        int value = query[0];
        int index = query[1];

        int oldValue = nums[index];
        nums[index] += value;  // Update the array in place.

        // Adjust evenSum based on parity change of the updated element.
        if (nums[index] % 2 == 0 && oldValue % 2 == 0) {
            evenSum += value;
        } else if (nums[index] % 2 == 0 && oldValue % 2 != 0) {
            evenSum += nums[index];
        } else if (oldValue % 2 == 0) {
            evenSum -= oldValue;
        }
        // If both are odd, evenSum remains unchanged.

        result.push_back(evenSum);
    }
    return result;
}
#include <cassert>
#include <vector>

// Function declaration (solution from above)
std::vector<int> sumEvenAfterQueries(std::vector<int>& nums,
                                     const std::vector<std::vector<int>>& queries);

int main() {
    // Test 1: Basic case with mixed parity changes.
    {
        std::vector<int> nums = {1, 2, 3, 4};
        std::vector<std::vector<int>> queries = {{1, 0}, {-3, 1}, {-4, 0}, {2, 3}};
        std::vector<int> result = sumEvenAfterQueries(nums, queries);
        std::vector<int> expected = {8, 6, 2, 4};
        assert(result == expected);
    }

    // Test 2: All even numbers, then turning one odd.
    {
        std::vector<int> nums = {2, 4, 6};
        std::vector<std::vector<int>> queries = {{-2, 0}, {5, 1}};
        std::vector<int> result = sumEvenAfterQueries(nums, queries);
        std::vector<int> expected = {10, 6};
        assert(result == expected);
    }

    // Test 3: All odd numbers, turning one even.
    {
        std::vector<int> nums = {1, 3, 5};
        std::vector<std::vector<int>> queries = {{1, 0}, {0, 2}};
        std::vector<int> result = sumEvenAfterQueries(nums, queries);
        std::vector<int> expected = {2, 4};
        assert(result == expected);
    }

    // Test 4: Negative even numbers and zero.
    {
        std::vector<int> nums = {-2, 0, 3};
        std::vector<std::vector<int>> queries = {{2, 0}, {1, 2}, {-3, 0}};
        std::vector<int> result = sumEvenAfterQueries(nums, queries);
        std::vector<int> expected = {0, 0, 0};
        assert(result == expected);
    }

    // Test 5: Single element, multiple queries.
    {
        std::vector<int> nums = {4};
        std::vector<std::vector<int>> queries = {{-2, 0}, {6, 0}, {0, 0}};
        std::vector<int> result = sumEvenAfterQueries(nums, queries);
        std::vector<int> expected = {2, 8, 8};
        assert(result == expected);
    }

    // Test 6: No even numbers initially and never create one.
    {
        std::vector<int> nums = {3, 7, 1};
        std::vector<std::vector<int>> queries = {{2, 0}, {-4, 1}};
        std::vector<int> result = sumEvenAfterQueries(nums, queries);
        std::vector<int> expected = {0, 0};
        assert(result == expected);
    }

    return 0;
}
