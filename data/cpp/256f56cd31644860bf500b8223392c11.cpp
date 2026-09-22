You are given a sequence of N integers. Starting with an empty collection and a current total of 0, you process the integers one by one from left to right. For each integer, you insert it into the collection and add it to the current total. If at any point the current total becomes negative, you must remove the smallest integer currently in the collection (and subtract its value from the current total) to make the total non-negative again. You may repeat this removal step as many times as needed, but each removal discards the smallest current element permanently. Your goal is to maximize the number of integers that remain in the final collection after processing all N integers. Write a C++ function that, given a vector of integers, returns the maximum possible number of elements left in the collection at the end. The input can contain negative and positive integers, and the order matters. The function must handle up to 200,000 integers, each with absolute value up to 10^9.

#include <cassert>
#include <vector>

int main() {
    // Test 1: All positive numbers -> keep all.
    assert(maxElementsKept({1, 2, 3, 4}) == 4);

    // Test 2: All negative numbers -> keep none (or maybe one if it's 0? but negatives only).
    assert(maxElementsKept({-1, -2, -3}) == 0);

    // Test 3: Single zero -> keep it.
    assert(maxElementsKept({0}) == 1);

    // Test 4: Mix where removing one negative allows keeping all positives.
    assert(maxElementsKept({-5, 10, 10}) == 3);
    // Process: -5 (sum=-5, remove -5 -> sum=0, heap empty), 10 (sum=10), 10 (sum=20) -> keep 2? Wait, let's recalc:
    // Actually push -5, sum=-5 -> remove -5, sum=0, heap empty. Then push 10, sum=10, push 10, sum=20 -> heap size=2. So answer is 2, not 3.
    // Correct the test:
    assert(maxElementsKept({-5, 10, 10}) == 2);

    // Test 5: Both negatives and positives, last one makes it non-negative.
    assert(maxElementsKept({-3, -2, 10}) == 1);
    // Process: -3 -> sum=-3 remove -3 sum=0 heap empty. -2 -> sum=-2 remove -2 sum=0 heap empty. 10 -> sum=10 heap size=1. So answer 1.

    // Test 6: Example from snippet: n=5, input 2, 3, -8, 5, 1 -> expected answer?
    // Process: 2 (sum=2), 3 (sum=5), -8 (sum=-3, remove smallest=-8, sum=5? Wait smallest is -8, so sum becomes 5-(-8)? Actually after adding -8, sum=5, then add -8: sum=5+(-8)=-3, remove smallest=-8, sum=-3-(-8)=5, heap has 2,3. Then add 5 (sum=10), add 1 (sum=11) -> heap size=4. So answer 4.
    assert(maxElementsKept({2, 3, -8, 5, 1}) == 4);

    // Test 7: Large values to check long long.
    assert(maxElementsKept({1000000000, -1000000000, 1000000000}) == 2);
    // Process: 1e9 sum=1e9, -1e9 sum=0, 1e9 sum=1e9 -> keep 3? Wait after -1e9 sum=0, heap has 1e9 and -1e9, then add 1e9 sum=1e9 heap size=3. So answer 3.
    // Correct test:
    assert(maxElementsKept({1000000000, -1000000000, 1000000000}) == 3);

    // Test 8: Negative after positive causing removal.
    assert(maxElementsKept({5, -6, 2}) == 2);
    // Process: 5 sum=5, -6 sum=-1 remove smallest=-6 sum=5 heap has 5, then 2 sum=7 heap size=2.

    return 0;
}

#include <vector>
#include <queue>

// Returns the maximum number of integers that can remain in the collection
// after processing all input numbers, maintaining non-negative current sum.
long long maxElementsKept(const std::vector<long long>& numbers) {
    std::priority_queue<long long, std::vector<long long>, std::greater<long long>> minHeap;
    long long currentSum = 0;

    for (long long value : numbers) {
        minHeap.push(value);
        currentSum += value;

        // While the sum is negative, remove the smallest element.
        while (!minHeap.empty() && currentSum < 0) {
            currentSum -= minHeap.top();
            minHeap.pop();
        }
    }

    return static_cast<long long>(minHeap.size());
}

// The problem is a classic greedy approach using a min-heap (or multiset). Process the integers in the given order. Maintain a running sum `current_sum` and a min-heap (or multiset) of all currently selected integers. For each new integer `x`, push it into the heap and add it to `current_sum`. If `current_sum` becomes negative, we must remove the smallest element from the heap to reduce the total by as much as possible (since removing the smallest negative or smallest value gives the largest reduction, and we want to keep as many elements as possible). Subtract that smallest value from `current_sum` and pop it from the heap. Repeat this while `current_sum` is negative. The number of elements remaining at the end is simply the size of the heap. This greedy works because removing the smallest element whenever the sum goes negative is optimal: it discards the least useful element (the one that contributes the most negative value) and keeps the total as high as possible, allowing more future positives to be kept. Edge cases include all positive numbers (no removals needed), all negative numbers (remove all but possibly the largest if it brings sum to non-negative), and a mix where early negatives cause removals that later positives might not recover. Time complexity is O(N log N) due to heap operations, and space complexity is O(N) for the heap.
