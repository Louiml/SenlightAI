/*
Write a C++ function named `transformVector` that takes a non-empty `std::vector<int>` by value, removes the last element using `pop_back()`, then appends the value `81` using `push_back()`. The function should return the modified vector. Additionally, write a separate helper function `describeVector` that takes a `const std::vector<int>&` and prints the first element (`front()`), the last element (`back()`), the size, and the capacity, each on its own line with descriptive labels. The main logic should demonstrate the use of range-based iteration to print all elements after transformation. The task emphasizes correct use of vector member functions and understanding of dynamic array behavior (size vs. capacity). Ensure the solution handles a vector with at least one element before calling `pop_back()` (i.e., input size ≥ 1), and note that after `pop_back()` and `push_back()`, the size remains the same as the original, but capacity may change depending on the implementation and reallocation policy. For testing, verify that the returned vector's content, size, and first/last values match expectations, and that `describeVector` output is correct (you may capture `cout` to a string for assertions, but simpler to just assert on the vector contents and manually verify output via visible test run).
*/
#include <vector>
#include <iostream>

// Transform a vector by removing its last element and appending 81.
std::vector<int> transformVector(std::vector<int> vec) {
    if (!vec.empty()) {
        vec.pop_back();
    }
    vec.push_back(81);
    return vec;
}

// Print first, last, size, and capacity of a vector.
void describeVector(const std::vector<int>& vec) {
    if (!vec.empty()) {
        std::cout << "First value: " << vec.front() << '\n';
        std::cout << "Last value: " << vec.back() << '\n';
    }
    std::cout << "Size of vector: " << vec.size() << '\n';
    std::cout << "Capacity of vector: " << vec.capacity() << '\n';
}
#include <cassert>
#include <vector>
#include <sstream>
#include <iostream>

// Assume transformVector and describeVector are declared above.

int main() {
    // Test 1: Original snippet's vector
    std::vector<int> input1 = {91, 23, 54, 63, 84, 69, 1, 39};
    std::vector<int> result1 = transformVector(input1);
    assert(result1.size() == 8);
    assert(result1.front() == 91);
    assert(result1.back() == 81);
    std::vector<int> expected1 = {91, 23, 54, 63, 84, 69, 1, 81};
    assert(result1 == expected1);

    // Test 2: Single-element vector
    std::vector<int> input2 = {7};
    std::vector<int> result2 = transformVector(input2);
    assert(result2.size() == 1);
    assert(result2.front() == 81);
    assert(result2.back() == 81);
    assert(result2 == std::vector<int>{81});

    // Test 3: Empty input is not allowed per spec, but test with size 1 still fine
    std::vector<int> input3 = {42};
    std::vector<int> result3 = transformVector(input3);
    assert(result3.size() == 1);
    assert(result3[0] == 81);

    // Test 4: Larger vector with negative numbers
    std::vector<int> input4 = {-5, -3, 0, 2};
    std::vector<int> result4 = transformVector(input4);
    std::vector<int> expected4 = {-5, -3, 0, 81};
    assert(result4 == expected4);

    // Test 5: Verify describeVector output by capturing cout
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    describeVector(result1);
    std::cout.rdbuf(old);
    std::string output = buffer.str();
    assert(output.find("First value: 91") != std::string::npos);
    assert(output.find("Last value: 81") != std::string::npos);
    assert(output.find("Size of vector: 8") != std::string::npos);
    // capacity is implementation-defined, but must be >= size (8). We can check that.
    size_t cap_pos = output.find("Capacity of vector: ");
    assert(cap_pos != std::string::npos);
    int cap = std::stoi(output.substr(cap_pos + 20));
    assert(cap >= 8);

    // If all asserts pass, print success message (optional)
    std::cout << "All tests passed.\n";
    return 0;
}
// The core algorithm is straightforward: copy the input vector (since passed by value), remove its last element with `pop_back()` (which reduces size by 1 and invalidates no iterators, but does not change capacity), then append `81` with `push_back()`, which may trigger reallocation if `size() == capacity()` before insertion. For the given initial vector of 8 elements, typical capacity after creation is 8; after `pop_back()` size becomes 7, capacity remains 8; pushing `81` does not reallocate because size (7) < capacity (8), so final capacity stays 8, but this is implementation-defined in general. The helper function `describeVector` must be `const`-correct, taking a `const` reference and using `front()`, `back()`, `size()`, and `capacity()`—none of which modify the vector. Edge cases: input size exactly 1—after `pop_back()` the vector becomes empty, pushing `81` makes size 1; ensure we do not call `front()` on an empty vector. Time complexity is O(n) for printing all elements via range-for, and O(1) for `pop_back`/`push_back` on average (amortized). Space complexity is O(n) for the copy (since passed by value), but if using move semantics in the test we could avoid it—but per spec we pass by value, so it's O(n). For the test assertions, we directly check the returned vector's size, front/back, and content equality with expected vectors.
