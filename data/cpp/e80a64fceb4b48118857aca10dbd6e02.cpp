// Write a standalone C++ function named `resizeLatticeLeft` that models a simplified version of the dynamic lattice resizing shown in the given code snippet. The function should take four parameters: a vector of doubles `q` (representing positions), a vector of doubles `p` (representing momenta), a vector of doubles `energyDistr` (representing normalized energy per site), and an integer `latticeSize` (the current number of sites). The function must check whether the energy at index 10 of `energyDistr` exceeds `1E-150`. If true, it should: (1) resize all three vectors to `latticeSize + 20`, (2) rotate `q` and `p` so that the last 20 elements are moved to the front (simulating a leftward expansion), (3) leave `energyDistr` unrotated (just resized), and (4) return the new lattice size (`latticeSize + 20`). If the condition is false, the function should return `latticeSize` unchanged. The function must handle edge cases where `energyDistr` has fewer than 11 elements (in which case it should not resize). Do not include any random potential generation, ODE integration, or class definitions—only the resizing logic.
#include <cassert>
#include <vector>

// Declaration of the function under test.
int resizeLatticeLeft(std::vector<double>& q, std::vector<double>& p,
                      std::vector<double>& energyDistr, int latticeSize);

int main() {
    // Test 1: No resize when energyDistr[10] is not above threshold.
    {
        std::vector<double> q(60, 0.0);
        std::vector<double> p(60, 0.0);
        std::vector<double> e(60, 0.0);
        e[10] = 1E-160; // below threshold
        int newSize = resizeLatticeLeft(q, p, e, 60);
        assert(newSize == 60);
        assert(q.size() == 60 && p.size() == 60 && e.size() == 60);
    }

    // Test 2: Resize occurs when energyDistr[10] exceeds threshold.
    {
        std::vector<double> q(60, 0.0);
        std::vector<double> p(60, 0.0);
        std::vector<double> e(60, 0.0);
        e[10] = 1E-100; // above threshold
        int newSize = resizeLatticeLeft(q, p, e, 60);
        assert(newSize == 80);
        assert(q.size() == 80 && p.size() == 80 && e.size() == 80);
        // After rotation, the first 20 elements should be zeros (from newly appended)
        for (int i = 0; i < 20; ++i) {
            assert(q[i] == 0.0);
            assert(p[i] == 0.0);
        }
        // Original data (which was all zeros) is now shifted to positions 20..79
        // But we didn't set any nonzero values, so just check size.
    }

    // Test 3: Edge case when energyDistr has fewer than 11 elements.
    {
        std::vector<double> q(5, 0.0);
        std::vector<double> p(5, 0.0);
        std::vector<double> e(5, 0.0);
        int newSize = resizeLatticeLeft(q, p, e, 5);
        assert(newSize == 5);
        assert(q.size() == 5 && p.size() == 5 && e.size() == 5);
    }

    // Test 4: Verify that rotation correctly shifts data by 20 positions.
    {
        std::vector<double> q(60, 0.0);
        std::vector<double> p(60, 0.0);
        std::vector<double> e(60, 0.0);
        // Set a nonzero value at the tail (position 59) to check rotation.
        q[59] = 1.0;
        p[59] = 2.0;
        e[10] = 0.5; // trigger resize

        int newSize = resizeLatticeLeft(q, p, e, 60);
        assert(newSize == 80);
        // After rotation, the original last element (index 59) should be at index 20? 
        // Let's reason: Original data indices 0..59. After resize to 80, indices 60..79 are zeros.
        // Rotate by end()-20 (which points to index 60) moves indices 60..79 to front (positions 0..19).
        // Then indices 0..59 are shifted to positions 20..79. So q[59] (original) is now at q[79].
        assert(q[79] == 1.0);
        assert(p[79] == 2.0);
        // The first 20 elements are zeros (from the newly added).
        for (int i = 0; i < 20; ++i) {
            assert(q[i] == 0.0);
            assert(p[i] == 0.0);
        }
    }

    // Test 5: Ensure that when no resize, the vectors are untouched.
    {
        std::vector<double> q(60, 3.14);
        std::vector<double> p(60, 2.71);
        std::vector<double> e(60, 0.0);
        e[10] = 0.0; // not > threshold
        int newSize = resizeLatticeLeft(q, p, e, 60);
        assert(newSize == 60);
        assert(q[0] == 3.14 && q[59] == 3.14);
        assert(p[0] == 2.71 && p[59] == 2.71);
    }

    return 0;
}
#include <vector>
#include <algorithm>

// Resizes the lattice vectors if energyDistr[10] exceeds threshold.
// Returns the new lattice size (old + 20) if resized, otherwise old size.
int resizeLatticeLeft(std::vector<double>& q, std::vector<double>& p,
                      std::vector<double>& energyDistr, int latticeSize) {
    const double threshold = 1E-150;

    // Guard against invalid index.
    if (energyDistr.size() < 11) {
        return latticeSize;
    }

    if (energyDistr[10] > threshold) {
        const int newSize = latticeSize + 20;

        // Resize all vectors to the new size (new elements are zero-initialized).
        q.resize(newSize);
        p.resize(newSize);
        energyDistr.resize(newSize);

        // Rotate the last 20 elements (which are the newly added zeros) to the front.
        // This shifts original data to the right, effectively expanding the lattice leftward.
        std::rotate(q.begin(), q.end() - 20, q.end());
        std::rotate(p.begin(), p.end() - 20, p.end());

        return newSize;
    }

    return latticeSize;
}
// The core task is to conditionally resize and rotate vectors based on a threshold check. The algorithm proceeds as follows: first, check if `energyDistr.size()` is at least 11 (so that index 10 is valid). If not, return the current size. Next, check if `energyDistr[10] > 1E-150`. If the condition holds, compute `newSize = latticeSize + 20`. Resize all three vectors to `newSize` (which will fill new elements with zeros for `q`, `p`, and `energyDistr`). Then perform a left rotation on `q` and `p` by 20 positions: since we first resized, the last 20 elements are originally the newly appended zeros, but we need to move the *old last 20 elements* to the front. However, the intended behavior from the original snippet is to rotate by `end()-20` after resizing—but since the resized vectors have zeros at the end, rotating by 20 effectively discards the zeros and places old tail elements at the front. In this simplified task, we rotate by exactly 20 using `std::rotate(q.begin(), q.end()-20, q.end())` after resizing, which works correctly because the last 20 elements after resizing are the newly added zeros—but wait, that would rotate zeros to the front, which is not intended. To match the original intent, we should first save the last 20 elements of the original vectors, then resize, then assign those saved elements to the front positions, then clear the remainder. A simpler approach: resize to newSize, then perform `std::rotate(q.begin(), q.begin()+20, q.end())`? No—that rotates the front elements to the back. The correct interpretation from the original code: after `do_resize` to `size+20`, they call `rotate(begin, end-20, end)` which moves the *last 20 elements* (which are now the newly added zeros) to the front. That would place zeros at the front, but in the original code, the condition `distr[10] > 1E-150` implies energy has propagated left, and they want to bring the tail (which contains meaningful data) to the front. Actually, in the original, before resizing, `distr` has size `N`, and `distr[10]` is near the left edge. After resizing to `N+20`, they rotate by `end()-20` (the last 20 elements are the zeros) to the front, which would move zeros to the front and shift the original data to the right—that seems opposite. Let me re-read: In the original, they rotate `state.first.begin()` to `state.first.end()-20` to `end()`. This rotates the *last 20* elements to the front. Since after resizing the last 20 are zeros, that moves zeros to the front, which would shift the original data to the right. That contradicts the comment "resized left". Wait, actually, maybe the direction is correct: If we want to add 20 sites on the left, we should shift existing data to the right by 20. Rotating the last 20 (zeros) to the front effectively shifts everything right by 20. That matches "resized left": the lattice expands to the left, so the original sites move to the right. Yes, that makes sense. So in the simplified task, we should: resize all vectors to `newSize`, then rotate `q` and `p` by moving the last 20 elements (which are the newly added zeros) to the front. Since `std::rotate(first, middle, last)` with `middle = q.end()-20` moves the element range `[middle, last)` to the front. After resizing, `q.end()-20` points to the first of the 20 newly appended zeros, so rotating moves those zeros to the front, shifting the original data to the right. That is correct. For `p` same. `energyDistr` is not rotated—only resized. Edge cases: if `energyDistr` has fewer than 11 elements, do nothing. Also ensure `latticeSize` matches `q.size()` etc., but for safety, compute newSize from `latticeSize + 20`. Time complexity: resizing is O(newSize), rotation is O(newSize) because `std::rotate` is linear. Space: O(1) extra besides the vector allocations.
