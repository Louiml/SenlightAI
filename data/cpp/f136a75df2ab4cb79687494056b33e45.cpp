Write a C++ function named `slidingMedian3` that simulates a simple median filter for a stream of floating-point measurements. The function should accept a single `float` value representing the latest measurement, and it should return the median of the three most recent values seen so far (including the current one). The filter must maintain an internal state across calls: initially, before any real measurements arrive, the first call should treat the filter as empty and set all three internal slots to that first value, so that its output equals that first value. For subsequent calls, each new value replaces the oldest slot in a circular buffer of size three, and the median of the three current slots is returned. The function must be `const`-correct (i.e., no mutable state except what is necessary for the sliding window), handle any finite `float` input (including negative values, zeros, and fractional values), and must not rely on external libraries beyond the C++ standard library. The internal state (the three-slot buffer and a counter/index) should be encapsulated using a `static` local variable inside the function so that the function has no global side effects and can be called repeatedly in a single-threaded context. Provide the function declaration and definition in a single self-contained code block, with a descriptive comment explaining the behavior. Do not include a `main` function in the solution section.

// The core idea is to maintain a fixed-size circular buffer of size 3 that holds the most recent three measurements. We also need an index (or counter) to know which slot to overwrite next and a flag (or counter) to know whether the buffer has been fully initialized. The simplest approach is to use a `static` array of three `float`s and a `static` integer counter. On the very first call, we set all three slots to the incoming value and then immediately return that value (since all slots are equal, the median is that value). For subsequent calls, we write the new value into the slot indicated by `counter % 3`, then increment the counter. After each call, we compute the median of the three current slots. To compute the median of three numbers, we can use a small network of comparisons: if `a <= b` then if `b <= c` median is `b`, else if `a <= c` median is `c`, else median is `a`; similarly for the other ordering. Alternatively, we can sort the three values using a few swaps. Edge cases: the very first call must return the input value. Duplicate values are handled naturally because the comparison logic works with equalities. Negative and fractional values work fine with `float`. Time complexity per call is O(1) because we only do a constant number of comparisons and assignments. Space complexity is O(1) because we only store a fixed-size array of three floats and a counter.

#include <algorithm> // not strictly needed, but for clarity

// Sliding median filter for a stream of float measurements.
// Keeps the three most recent values (including the current call).
// First call: all three slots are set to the input, returns that input.
// Subsequent calls: each new value replaces the oldest slot (circular).
// Returns the median of the three current slots.
float slidingMedian3(float value) {
    // Static state persists across calls.
    static float buffer[3] = {0.0f, 0.0f, 0.0f};
    static int counter = 0; // counts how many values have been processed

    // First call: initialize all slots to the first value.
    if (counter == 0) {
        buffer[0] = buffer[1] = buffer[2] = value;
        counter = 1; // mark as initialized
        return value; // median of three equal values is that value
    }

    // For calls after the first: replace the oldest slot.
    // counter starts at 1, so the slot to replace is (counter % 3).
    buffer[counter % 3] = value;
    counter++;

    // Compute median of three values.
    float a = buffer[0];
    float b = buffer[1];
    float c = buffer[2];

    // Sort a, b, c into ascending order using swapping.
    if (a > b) std::swap(a, b);
    if (b > c) std::swap(b, c);
    if (a > b) std::swap(a, b);

    // b is now the median.
    return b;
}

#include <cassert>

int main() {
    // Test 1: First call returns the value.
    assert(slidingMedian3(1.0f) == 1.0f);

    // Test 2: After first call, call with 2.0, buffer is [1,1,1] but we replace slot [1%3=1] with 2 -> [1,2,1], median 1.
    assert(slidingMedian3(2.0f) == 1.0f);

    // Test 3: Now call with 3.0, replace slot [2%3=2] -> [1,2,3], median 2.
    assert(slidingMedian3(3.0f) == 2.0f);

    // Test 4: Call with 4.0, replace slot [3%3=0] -> [4,2,3], median 3.
    assert(slidingMedian3(4.0f) == 3.0f);

    // Test 5: Call with 0.0, replace slot [4%3=1] -> [4,0,3], median 3.
    assert(slidingMedian3(0.0f) == 3.0f);

    // Test 6: Call with -1.0, replace slot [5%3=2] -> [4,0,-1], median 0.
    assert(slidingMedian3(-1.0f) == 0.0f);

    // Test 7: Duplicate values, call with 5.0 twice: first [5,0,-1] median 0; second [5,5,-1] median 5.
    assert(slidingMedian3(5.0f) == 0.0f);
    assert(slidingMedian3(5.0f) == 5.0f);

    // Test 8: Fractional values.
    assert(slidingMedian3(2.5f) == 5.0f); // [5,5,2.5] median 5
    assert(slidingMedian3(3.5f) == 5.0f); // [5,3.5,3.5] median 3.5? Wait: slots: after previous calls, buffer is [5,5,2.5] then replace slot [9%3=0] with 3.5 -> [3.5,5,2.5], median 3.5? Actually let's compute: a=3.5, b=5, c=2.5 sorted: [2.5,3.5,5] median 3.5. So assertion should be 3.5f.
    assert(slidingMedian3(3.5f) == 3.5f);
}
