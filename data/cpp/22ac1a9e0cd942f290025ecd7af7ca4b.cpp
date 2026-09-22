// Write a C++ function `runningMedian()` that reads a sequence of integers from a `std::istream` (e.g., standard input) until end-of-file, and for each integer read, outputs the running median (after processing that integer) to a provided `std::ostream` (e.g., standard output). The median is defined as follows: if the total number of processed elements is odd, the median is the middle element when sorted; if even, the median is the average of the two middle elements. For integer input, the running median may be fractional, so output it as a double with default formatting (or fixed to two decimal places, but the task expects exact comparison in tests, so use `std::setprecision` to ensure exact representation). Edge cases: the first element is the median itself; negative numbers and duplicates are allowed; if an even count, the average may result in .5 (e.g., for 1 and 2, median = 1.5). Your function must be efficient for up to 10^5 integers, and should not use external libraries beyond standard C++.

The classic approach uses two heaps: a max-heap for the lower half of numbers and a min-heap for the upper half. Maintain the invariant that the max-heap size is either equal to the min-heap size or exactly one larger. The median is then the top of the max-heap if sizes are unequal, or the average of the two tops if sizes are equal. When a new number arrives, compare it to the current median (or the appropriate heap top) to decide where to insert, then rebalance to maintain the size invariant. The provided snippet uses a median variable and pushes negative values into the min-heap to simulate a min-heap using `priority_queue<double>` (since that is a max-heap by default). Alternatively, use `std::priority_queue<double, std::vector<double>, std::greater<double>>` for the min-heap. Care must be taken: the median variable should be updated after each insertion, and when sizes are equal, the median is the average of the two heap tops (if both heaps non-empty). For the very first element, both heaps are empty, so we must handle that separately by pushing into the max-heap and setting median to that value. Edge cases: when heaps have equal sizes and we insert, we need to decide which heap to push to, but the general algorithm handles it: if the new number is less than the current median, push to max-heap; else push to min-heap. Then rebalance: if max-heap size > min-heap size + 1, move the top of max-heap to min-heap; if min-heap size > max-heap size, move the top of min-heap to max-heap. After rebalancing, compute the median as described. Complexity: each insertion is O(log n) for heap operations, and we process n elements, so total O(n log n) time. Auxiliary space is O(n) for the heaps. Output is done online as we read each number.

#include <queue>
#include <vector>
#include <istream>
#include <ostream>
#include <iomanip>

// Reads integers from 'input' until EOF, computes the running median after each integer,
// and writes each median to 'output' on its own line.
void runningMedian(std::istream& input, std::ostream& output) {
    std::priority_queue<double> max_heap; // lower half
    std::priority_queue<double, std::vector<double>, std::greater<double>> min_heap; // upper half

    double value;
    while (input >> value) {
        // Insert into appropriate heap
        if (max_heap.empty() || value <= max_heap.top()) {
            max_heap.push(value);
        } else {
            min_heap.push(value);
        }

        // Rebalance: max_heap can have at most one more element than min_heap
        if (max_heap.size() > min_heap.size() + 1) {
            min_heap.push(max_heap.top());
            max_heap.pop();
        } else if (min_heap.size() > max_heap.size()) {
            max_heap.push(min_heap.top());
            min_heap.pop();
        }

        // Compute median
        double median;
        if (max_heap.size() > min_heap.size()) {
            median = max_heap.top();
        } else {
            median = (max_heap.top() + min_heap.top()) / 2.0;
        }
        output << std::fixed << std::setprecision(1) << median << '\n';
    }
}

#include <sstream>
#include <cassert>

int main() {
    // Test 1: Simple sequence
    std::istringstream in1("1 2 3");
    std::ostringstream out1;
    runningMedian(in1, out1);
    assert(out1.str() == "1.0\n1.5\n2.0\n");

    // Test 2: Negative and duplicate numbers
    std::istringstream in2("-5 -1 -10");
    std::ostringstream out2;
    runningMedian(in2, out2);
    assert(out2.str() == "-5.0\n-3.0\n-5.0\n");

    // Test 3: Single element
    std::istringstream in3("7");
    std::ostringstream out3;
    runningMedian(in3, out3);
    assert(out3.str() == "7.0\n");

    // Test 4: Even count with .5 median
    std::istringstream in4("1 2");
    std::ostringstream out4;
    runningMedian(in4, out4);
    assert(out4.str() == "1.0\n1.5\n");

    // Test 5: Larger set and all equal
    std::istringstream in5("5 5 5");
    std::ostringstream out5;
    runningMedian(in5, out5);
    assert(out5.str() == "5.0\n5.0\n5.0\n");
}
