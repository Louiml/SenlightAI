// Write a C++ function `std::string priorityQueueSimulation(const std::vector<int>& operations)` that simulates a max-heap-based priority queue where each integer in the input vector represents a single operation: positive values are inserted into the heap, `0` extracts and returns the maximum, and `-1` peeks at the maximum without removing it. The function should process all operations in order and return a string containing the results (for `0` and `-1` operations) separated by single spaces. If an extraction or peek is attempted on an empty heap, append `"EMPTY"` for that operation. The final output string must have no leading or trailing spaces; if no output is produced, return an empty string. Use an array-based binary max-heap as shown in the snippet, but implement it locally within the function (no external classes). The heap's capacity can be assumed large enough for all insertions.
// The core idea is to implement a max-heap using a dynamic array or a fixed-size vector, where the heap property is maintained by two helper operations: `bubbleUp` (called after inserting at the end) and `bubbleDown` (called after removing the root or replacing an element). The main function processes each integer in the input vector sequentially: for a positive value, push it onto the heap and bubble it up; for `0`, if the heap is empty, append `"EMPTY"`, otherwise swap the root with the last element, pop the last, bubble down from the root, and append the removed maximum; for `-1`, if empty append `"EMPTY"` else append the root value. Edge cases include an empty heap for extraction/peek, duplicate values, and a large number of operations. Time complexity is \(O(m \log n)\) where \(m\) is the number of operations and \(n\) is the heap size at the time of each operation, with worst-case \(O(m \log m)\) if all insertions happen before extractions. Space complexity is \(O(n)\) for the heap storage and \(O(m)\) for the output string.
#include <string>
#include <vector>
#include <algorithm>

// Simulate a priority queue using a max-heap.
std::string priorityQueueSimulation(const std::vector<int>& operations) {
    std::vector<int> heap;
    std::string result;

    auto bubbleUp = [&](int index) {
        while (index > 0) {
            int parent = (index - 1) / 2;
            if (heap[index] > heap[parent]) {
                std::swap(heap[index], heap[parent]);
                index = parent;
            } else {
                break;
            }
        }
    };

    auto bubbleDown = [&](int index) {
        int size = static_cast<int>(heap.size());
        while (true) {
            int left = 2 * index + 1;
            int right = 2 * index + 2;
            int largest = index;
            if (left < size && heap[left] > heap[largest]) largest = left;
            if (right < size && heap[right] > heap[largest]) largest = right;
            if (largest != index) {
                std::swap(heap[index], heap[largest]);
                index = largest;
            } else {
                break;
            }
        }
    };

    for (int op : operations) {
        if (op > 0) {
            heap.push_back(op);
            bubbleUp(static_cast<int>(heap.size()) - 1);
        } else if (op == 0) {
            if (heap.empty()) {
                if (!result.empty()) result += " ";
                result += "EMPTY";
            } else {
                int maxVal = heap[0];
                heap[0] = heap.back();
                heap.pop_back();
                if (!heap.empty()) bubbleDown(0);
                if (!result.empty()) result += " ";
                result += std::to_string(maxVal);
            }
        } else if (op == -1) {
            if (heap.empty()) {
                if (!result.empty()) result += " ";
                result += "EMPTY";
            } else {
                if (!result.empty()) result += " ";
                result += std::to_string(heap[0]);
            }
        }
    }

    return result;
}
#include <cassert>
#include <vector>
#include <string>

int main() {
    // Test basic insertion and extraction
    assert(priorityQueueSimulation({5, 3, 8, 0, 0}) == "8 5");
    // Test peek operation
    assert(priorityQueueSimulation({4, 2, -1, 0, -1}) == "4 4 2");
    // Test empty heap extraction
    assert(priorityQueueSimulation({0, -1}) == "EMPTY EMPTY");
    // Test mixed operations with duplicates
    assert(priorityQueueSimulation({7, 7, 7, 0, 0, 0}) == "7 7 7");
    // Test all insertions then multiple extractions
    assert(priorityQueueSimulation({1, 9, 3, 0, 0, 0}) == "9 3 1");
    // Test no output operations
    assert(priorityQueueSimulation({2, 5, 1}) == "");
    // Test empty input
    assert(priorityQueueSimulation({}) == "");
    // Test peek then extract then peek again
    assert(priorityQueueSimulation({10, 20, -1, 0, -1}) == "20 20 10");
    // Test extraction after partial emptying
    assert(priorityQueueSimulation({8, 4, 4, 0, 0, -1}) == "8 4 4");
    // Test negative numbers not inserted (only positive)
    assert(priorityQueueSimulation({-5, 3, 0, -1, 0}) == "3 3 EMPTY");
    return 0;
}
