// Write a C++ function `Stats *stat(const Array *array)` that analyzes a sorted array stored in an `Array` structure (containing `int *data` and `int size`) and returns a dynamically allocated `Stats` structure. The `Stats` structure must contain: `float avg` (the average of all elements), `float median` (the middle value if odd size, or the average of the two middle values if even size), `int modFreq` (the highest frequency count of any value), and `Array *mode` (a dynamically allocated array of all modes—values that appear with frequency equal to `modFreq`; if no value repeats, the mode array must have `size` set to 0 and `data` left null). The input array is guaranteed to be sorted in non-decreasing order. Handle edge cases such as empty arrays (size 0), arrays with all identical values, and arrays with multiple distinct modes.
The solution must process a sorted array efficiently. First, compute the average by summing all elements and dividing by `size`, storing the result in `avg`. For the median: if `size` is odd, take the middle element at index `size/2`; if even, average the two middle elements at indices `size/2 - 1` and `size/2`. For modes, traverse the sorted array in a single pass, tracking the current value, its frequency, the maximum frequency seen so far, and collecting all values that achieve that maximum frequency. Since the array is sorted, repeated values are contiguous; a single linear scan can identify runs and update the mode set. Initialize `modFreq` to 0 and `mode->size` to 0 with `mode->data` set to `nullptr`. After the scan, if `modFreq` is 1, that means no value repeats, so leave the mode set empty. If `modFreq` > 1, allocate exactly `mode->size` integers and fill them with the collected modes. Time complexity is O(n) for traversal and O(1) additional space besides the output mode array; the mode array requires O(k) space where k is the number of distinct modes. No sorting is needed because the input is already sorted.
#include <cstddef>

struct Array {
    int *data;
    int size;
};

struct Stats {
    float avg;
    float median;
    int modFreq;
    Array *mode;
};

// Compute average, median, and all modes from a sorted array.
Stats *stat(const Array *array) {
    Stats *stats = new Stats;
    stats->mode = new Array;
    stats->mode->data = nullptr;
    stats->mode->size = 0;
    stats->modFreq = 0;

    if (array->size == 0) {
        stats->avg = 0.0f;
        stats->median = 0.0f;
        return stats;
    }

    // Average
    long long sum = 0;
    for (int i = 0; i < array->size; ++i) {
        sum += array->data[i];
    }
    stats->avg = static_cast<float>(sum) / array->size;

    // Median
    if (array->size % 2 == 1) {
        stats->median = array->data[array->size / 2];
    } else {
        stats->median = (array->data[array->size / 2 - 1] +
                         array->data[array->size / 2]) / 2.0f;
    }

    // Modes (single pass over sorted array)
    int currentFreq = 1;
    int maxFreq = 1;
    int *modes = new int[array->size]; // worst case all distinct, but we only keep if freq > 1
    int modeCount = 0;

    for (int i = 1; i <= array->size; ++i) {
        if (i < array->size && array->data[i] == array->data[i - 1]) {
            ++currentFreq;
        } else {
            if (currentFreq > maxFreq) {
                maxFreq = currentFreq;
                modeCount = 0;
                modes[modeCount++] = array->data[i - 1];
            } else if (currentFreq == maxFreq && currentFreq > 1) {
                modes[modeCount++] = array->data[i - 1];
            }
            currentFreq = 1;
        }
    }

    stats->modFreq = maxFreq > 1 ? maxFreq : 0;

    if (stats->modFreq > 0 && modeCount > 0) {
        stats->mode->size = modeCount;
        stats->mode->data = new int[modeCount];
        for (int i = 0; i < modeCount; ++i) {
            stats->mode->data[i] = modes[i];
        }
    }

    delete[] modes;
    return stats;
}
#include <cassert>
#include <cmath>

// Assume the Array and Stats structs and the stat function are defined above (or included).

int main() {
    // Test 1: Simple sorted array with one mode
    int d1[] = {1, 2, 2, 3, 4};
    Array a1 = {d1, 5};
    Stats *s1 = stat(&a1);
    assert(std::fabs(s1->avg - 2.4f) < 1e-6);
    assert(std::fabs(s1->median - 2.0f) < 1e-6);
    assert(s1->modFreq == 2);
    assert(s1->mode->size == 1);
    assert(s1->mode->data[0] == 2);
    delete[] s1->mode->data; delete s1->mode; delete s1;

    // Test 2: All identical
    int d2[] = {5, 5, 5, 5};
    Array a2 = {d2, 4};
    Stats *s2 = stat(&a2);
    assert(std::fabs(s2->avg - 5.0f) < 1e-6);
    assert(std::fabs(s2->median - 5.0f) < 1e-6);
    assert(s2->modFreq == 4);
    assert(s2->mode->size == 1);
    assert(s2->mode->data[0] == 5);
    delete[] s2->mode->data; delete s2->mode; delete s2;

    // Test 3: Two modes
    int d3[] = {1, 1, 2, 2, 3};
    Array a3 = {d3, 5};
    Stats *s3 = stat(&a3);
    assert(s3->modFreq == 2);
    assert(s3->mode->size == 2);
    assert(s3->mode->data[0] == 1);
    assert(s3->mode->data[1] == 2);
    assert(std::fabs(s3->avg - 1.8f) < 1e-6);
    assert(std::fabs(s3->median - 2.0f) < 1e-6);
    delete[] s3->mode->data; delete s3->mode; delete s3;

    // Test 4: No mode (all distinct)
    int d4[] = {10, 20, 30};
    Array a4 = {d4, 3};
    Stats *s4 = stat(&a4);
    assert(s4->modFreq == 0);
    assert(s4->mode->size == 0);
    assert(s4->mode->data == nullptr);
    assert(std::fabs(s4->avg - 20.0f) < 1e-6);
    assert(std::fabs(s4->median - 20.0f) < 1e-6);
    delete s4->mode; delete s4;

    // Test 5: Even size median average
    int d5[] = {1, 2, 3, 4};
    Array a5 = {d5, 4};
    Stats *s5 = stat(&a5);
    assert(std::fabs(s5->median - 2.5f) < 1e-6);
    assert(s5->modFreq == 0);
    delete s5->mode; delete s5;

    // Test 6: Empty array
    int *d6 = nullptr;
    Array a6 = {d6, 0};
    Stats *s6 = stat(&a6);
    assert(s6->avg == 0.0f);
    assert(s6->median == 0.0f);
    assert(s6->mode->size == 0);
    assert(s6->mode->data == nullptr);
    delete s6->mode; delete s6;

    // Test 7: Negative numbers with mode
    int d7[] = {-3, -3, -2, -1};
    Array a7 = {d7, 4};
    Stats *s7 = stat(&a7);
    assert(s7->modFreq == 2);
    assert(s7->mode->size == 1);
    assert(s7->mode->data[0] == -3);
    delete[] s7->mode->data; delete s7->mode; delete s7;

    return 0;
}
