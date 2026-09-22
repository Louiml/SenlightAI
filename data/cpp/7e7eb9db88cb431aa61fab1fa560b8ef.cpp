Write a C++ function `vector<int> shufflePreservingOriginal(const vector<int>& input)` that returns a uniformly random permutation of the input vector. The function must be deterministic in the sense that it uses the standard `rand()` (which you may seed externally) and must guarantee that every possible permutation of `n` elements has an equal chance of being produced. Additionally, the function must not modify the original input vector (it should return a new shuffled copy). Use the Fisher–Yates (Knuth) shuffle algorithm: for each index `i` from 0 to `n-1`, pick a random index `j` uniformly from `i` to `n-1` (inclusive) and swap the elements at `i` and `j`. The function must be `const`-correct: it takes a const reference and returns a new vector. Edge case: an empty vector should return an empty vector.

#include <cassert>
#include <vector>
#include <algorithm>
#include <set>

int main() {
    // Test empty input
    std::vector<int> empty;
    assert(shufflePreservingOriginal(empty).empty());

    // Test single element
    std::vector<int> single = {42};
    assert(shufflePreservingOriginal(single) == single);

    // Test that original is not modified and result is a permutation
    std::vector<int> input = {1, 2, 3, 4, 5};
    std::vector<int> originalCopy = input;
    auto shuffled = shufflePreservingOriginal(input);
    assert(input == originalCopy); // original unchanged
    assert(shuffled.size() == input.size());
    std::vector<int> sortedShuffled = shuffled;
    std::vector<int> sortedInput = input;
    std::sort(sortedShuffled.begin(), sortedShuffled.end());
    std::sort(sortedInput.begin(), sortedInput.end());
    assert(sortedShuffled == sortedInput); // same elements

    // Test with duplicates
    std::vector<int> dup = {1, 1, 2, 2};
    auto dupShuffled = shufflePreservingOriginal(dup);
    assert(dupShuffled.size() == dup.size());
    assert(std::is_permutation(dupShuffled.begin(), dupShuffled.end(), dup.begin()));

    // Test with negative numbers
    std::vector<int> neg = {-5, -3, 0, 7, 10};
    auto negShuffled = shufflePreservingOriginal(neg);
    assert(negShuffled.size() == neg.size());
    assert(std::is_permutation(negShuffled.begin(), negShuffled.end(), neg.begin()));

    // Test with many elements (e.g., 1000) to ensure no crash and permutation property
    std::vector<int> big;
    for (int i = 0; i < 1000; ++i) big.push_back(i);
    auto bigShuffled = shufflePreservingOriginal(big);
    assert(bigShuffled.size() == 1000);
    assert(std::is_permutation(bigShuffled.begin(), bigShuffled.end(), big.begin()));

    // Test that shuffle can produce different orders (probabilistic but should succeed with high probability)
    std::srand(12345);
    std::vector<int> small = {1, 2, 3};
    std::set<std::vector<int>> seen;
    for (int i = 0; i < 100; ++i) {
        seen.insert(shufflePreservingOriginal(small));
    }
    // With 3 elements, there are 6 permutations; with 100 trials, we expect to see at least 3 distinct ones.
    assert(seen.size() >= 3);
}

#include <vector>
#include <cstdlib>
#include <algorithm>

// Returns a uniformly random permutation of the input vector using Fisher–Yates shuffle.
// The original input vector is not modified; a new shuffled copy is returned.
std::vector<int> shufflePreservingOriginal(const std::vector<int>& input) {
    std::vector<int> result = input;  // copy input
    int n = static_cast<int>(result.size());

    for (int i = 0; i < n; ++i) {
        // Pick a random index j uniformly from [i, n-1]
        int j = i + std::rand() % (n - i);
        std::swap(result[i], result[j]);
    }

    return result;
}

// The Fisher–Yates shuffle works by iterating from the first element to the second-to-last element. At each step `i`, a random index `j` is chosen uniformly from the range `[i, n-1]`, and the elements at positions `i` and `j` are swapped. This ensures that after processing index `i`, the element at position `i` is fixed and will never be moved again. The algorithm produces each permutation with equal probability because at each step, the number of choices is `n - i`, and the product of these choices over all steps is `n!`, matching the number of distinct permutations. The main edge case is an empty or single-element vector, where the loop either does nothing or only one step with `j == i`, resulting in no changes. The algorithm uses `O(1)` extra space (only a few integer variables) and runs in `O(n)` time. Since the input is a const reference, we must copy it into a local vector before shuffling to avoid modifying the caller's data.
