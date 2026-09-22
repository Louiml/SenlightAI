Write a C++ function named `segregateBinary` that takes a non-empty `std::vector<int>&` containing only the values 0 and 1, and rearranges the elements in-place so that all 0s appear before all 1s. The function must preserve the relative order of the original elements as much as possible (i.e., it should be stable), but since stability is not strictly required for this common problem variant, the primary requirement is that after the call, the vector has no 0 after any 1. The function must not use additional arrays or containers for the rearrangement (only O(1) auxiliary space is allowed). It should handle edge cases like all zeros, all ones, and a single element. The function must be efficient for large inputs.

#include <cassert>
#include <vector>

// Assume segregateBinary is defined above.

int main() {
    // Mixed case
    std::vector<int> a = {0, 1, 0, 1, 0, 1};
    segregateBinary(a);
    assert((a == std::vector<int>{0, 0, 0, 1, 1, 1}));

    // All zeros
    std::vector<int> b = {0, 0, 0};
    segregateBinary(b);
    assert((b == std::vector<int>{0, 0, 0}));

    // All ones
    std::vector<int> c = {1, 1, 1};
    segregateBinary(c);
    assert((c == std::vector<int>{1, 1, 1}));

    // Single element zero
    std::vector<int> d = {0};
    segregateBinary(d);
    assert((d == std::vector<int>{0}));

    // Single element one
    std::vector<int> e = {1};
    segregateBinary(e);
    assert((e == std::vector<int>{1}));

    // Already sorted
    std::vector<int> f = {0, 0, 1, 1};
    segregateBinary(f);
    assert((f == std::vector<int>{0, 0, 1, 1}));

    // Reverse order
    std::vector<int> g = {1, 1, 0, 0};
    segregateBinary(g);
    assert((g == std::vector<int>{0, 0, 1, 1}));

    // Large vector: 1000 elements, first 400 are 1, rest 0
    std::vector<int> h(1000, 1);
    for (int i = 400; i < 1000; ++i) h[i] = 0;
    segregateBinary(h);
    for (int i = 0; i < 600; ++i) assert(h[i] == 0);
    for (int i = 600; i < 1000; ++i) assert(h[i] == 1);
}

#include <vector>
#include <utility>  // for std::swap

// Segregates a vector of 0s and 1s so all 0s appear before all 1s.
void segregateBinary(std::vector<int>& arr) {
    if (arr.empty()) return;

    int left = 0;
    int right = static_cast<int>(arr.size()) - 1;

    // Skip leading zeros and trailing ones to find initial out-of-place positions.
    while (left < arr.size() && arr[left] == 0) ++left;
    while (right >= 0 && arr[right] == 1) --right;

    // Swap mismatched pairs until pointers cross.
    while (left < right) {
        // At this point, arr[left] == 1 and arr[right] == 0.
        std::swap(arr[left], arr[right]);

        // Move left forward past any zeros.
        while (left < arr.size() && arr[left] == 0) ++left;
        // Move right backward past any ones.
        while (right >= 0 && arr[right] == 1) --right;
    }
}

// The provided snippet uses a two-pointer approach where `left` points to the first position that may hold a 1 (after skipping leading zeros) and `right` points to the last position that may hold a 0 (after skipping trailing ones). Then it swaps the elements at `left` and `right` if they are out of order (i.e., left is 1 and right is 0), and then advances `left` past any zeros and decrements `right` past any ones. This continues until `left` and `right` cross. This correctly segregates the array in a single pass. Important edge cases: if the array is already sorted or has only one distinct value, the initial while loops will set `left` or `right` such that the main loop condition fails immediately. For an empty vector, the initial accesses would be invalid, so the task specifies non-empty input. Time complexity is O(n) because each element is visited at most a constant number of times by the pointer movements. Space complexity is O(1) extra space. The algorithm is not stable (order of 0s and 1s relative to each other is not preserved), but that is acceptable per the task.
