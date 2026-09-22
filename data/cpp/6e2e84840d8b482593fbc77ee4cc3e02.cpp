Write a C++ function `findMedianFromStream` that takes a `std::vector<int>` representing a sequence of numbers added one at a time, and returns a `std::vector<double>` where the i-th element is the median of all numbers from index 0 to i (inclusive) in the input vector. The input vector may contain negative numbers, duplicates, and any number of elements (including empty). For each prefix of the vector, compute the median: if the prefix has an odd number of elements, the median is the middle value after sorting; if it has an even number, it is the average of the two middle values. The function must process the stream efficiently, not by re-sorting the entire prefix each time. For an empty input vector, return an empty output vector.

// The solution uses two heaps: a max-heap storing the smaller half of the numbers seen so far, and a min-heap storing the larger half. The max-heap is kept such that its size is always either equal to the min-heap size or exactly one greater. This invariant ensures the median is either the top of the max-heap (when sizes differ by one) or the average of the two heap tops (when sizes are equal). For each new number, push it into the max-heap, then re-balance: if the max-heap’s top is greater than the min-heap’s top (and min-heap is not empty), swap the tops between heaps; then equalize sizes so that max-heap is never smaller than min-heap and never more than one element larger. This maintains correct ordering because all elements in max-heap are ≤ all elements in min-heap after balancing. Each `add` operation takes $O(\log n)$ time due to heap operations, and finding the median is $O(1)$. The total time for $n$ elements is $O(n \log n)$, with $O(n)$ auxiliary space for storing all numbers in the heaps. Edge cases include empty input (return empty), single element (median is that element), duplicates (handled naturally), and negative numbers (heap comparisons work fine). Also note the result must be returned as a `double` even when the median is an integer.

#include <vector>
#include <queue>
#include <functional>

// Compute the running median for each prefix of the input stream.
std::vector<double> findMedianFromStream(const std::vector<int>& nums) {
    std::vector<double> medians;
    std::priority_queue<int> maxHeap; // stores smaller half
    std::priority_queue<int, std::vector<int>, std::greater<int>> minHeap; // stores larger half

    for (int num : nums) {
        // Step 1: insert into max-heap
        maxHeap.push(num);

        // Step 2: ensure maxHeap's top is <= minHeap's top
        if (!minHeap.empty() && maxHeap.top() > minHeap.top()) {
            minHeap.push(maxHeap.top());
            maxHeap.pop();
        }

        // Step 3: balance sizes so maxHeap.size() == minHeap.size() or == minHeap.size()+1
        if (maxHeap.size() > minHeap.size() + 1) {
            minHeap.push(maxHeap.top());
            maxHeap.pop();
        } else if (minHeap.size() > maxHeap.size()) {
            maxHeap.push(minHeap.top());
            minHeap.pop();
        }

        // Compute median for current prefix
        if (maxHeap.size() > minHeap.size()) {
            medians.push_back(static_cast<double>(maxHeap.top()));
        } else {
            medians.push_back((static_cast<double>(maxHeap.top()) + minHeap.top()) / 2.0);
        }
    }

    return medians;
}

#include <cassert>
#include <vector>

int main() {
    // Test 1: Basic odd and even prefixes
    std::vector<int> input1 = {1, 2, 3};
    std::vector<double> expected1 = {1.0, 1.5, 2.0};
    assert(findMedianFromStream(input1) == expected1);

    // Test 2: Negative numbers and duplicates
    std::vector<int> input2 = {-5, -5, 10};
    std::vector<double> expected2 = {-5.0, -5.0, -5.0};
    assert(findMedianFromStream(input2) == expected2);

    // Test 3: Single element
    std::vector<int> input3 = {42};
    std::vector<double> expected3 = {42.0};
    assert(findMedianFromStream(input3) == expected3);

    // Test 4: Large mixed values
    std::vector<int> input4 = {1, 0, 2, 3, 4};
    std::vector<double> expected4 = {1.0, 0.5, 1.0, 1.5, 2.0};
    assert(findMedianFromStream(input4) == expected4);

    // Test 5: Empty input
    std::vector<int> input5;
    assert(findMedianFromStream(input5).empty());

    // Test 6: Even-length final sequence
    std::vector<int> input6 = {3, 1, 2};
    std::vector<double> expected6 = {3.0, 2.0, 2.0};
    assert(findMedianFromStream(input6) == expected6);

    // Test 7: Descending order
    std::vector<int> input7 = {5, 4, 3, 2};
    std::vector<double> expected7 = {5.0, 4.5, 4.0, 3.5};
    assert(findMedianFromStream(input7) == expected7);

    // Test 8: All equal
    std::vector<int> input8 = {7, 7, 7, 7};
    std::vector<double> expected8 = {7.0, 7.0, 7.0, 7.0};
    assert(findMedianFromStream(input8) == expected8);

    return 0;
}
