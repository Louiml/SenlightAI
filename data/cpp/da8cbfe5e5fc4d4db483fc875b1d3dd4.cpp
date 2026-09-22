Given a vector of neighboring rectangles (each represented by `cv::Rect` or a simple `Rect` struct with `x`, `y`, `width`, `height`), implement a function `detectForegroundSamples` that classifies each rectangle as foreground or background based on the `mode` parameter and the sample's spatial position. The function should take a `std::vector<Rect>` of positive samples (foreground) and a `std::vector<Rect>` of negative samples (background), plus a mode constant (`MODE_POSITIVE`, `MODE_NEGATIVE`, `MODE_ESTIMATION`), and return a vector of `bool` flags indicating whether each rectangle (in the combined order: all positives then all negatives) is considered foreground. The rule: if mode is `MODE_POSITIVE` or `MODE_ESTIMATION`, all rectangles are treated as foreground; if mode is `MODE_NEGATIVE`, all rectangles are treated as background. Edge cases: if the mode is invalid or neither of the three, throw `std::invalid_argument`. The function must be `const`-correct and should not alter input vectors.
The task is straightforward classification based on a mode parameter. The main algorithm: iterate over the combined list of rectangles (positives first, then negatives) and assign `true` for foreground if mode is `MODE_POSITIVE` or `MODE_ESTIMATION`, and `false` if mode is `MODE_NEGATIVE`. This maps directly from the code snippet where `foreground` is set based on `mode`. Edge cases: empty input vectors are fine—return an empty vector. Invalid mode should throw an exception. Time complexity is O(N) where N is the total number of rectangles (positives + negatives), and space complexity is O(N) for the output vector. No need to inspect rectangle coordinates—they are ignored for classification, but we still traverse them to produce the correct count.
#include <vector>
#include <stdexcept>

// Simple rectangle structure (stand-in for cv::Rect in the original snippet)
struct Rect {
    int x, y, width, height;
};

// Mode constants (mirroring the MIL tracker)
enum MILMode {
    MODE_POSITIVE = 0,
    MODE_NEGATIVE = 1,
    MODE_ESTIMATION = 2
};

// Given positive and negative sample rectangles and a mode, return a vector of
// booleans indicating foreground status for each sample (positives first, then negatives).
std::vector<bool> detectForegroundSamples(
    const std::vector<Rect>& positiveSamples,
    const std::vector<Rect>& negativeSamples,
    int mode)
{
    if (mode != MODE_POSITIVE && mode != MODE_NEGATIVE && mode != MODE_ESTIMATION) {
        throw std::invalid_argument("Invalid mode: must be MODE_POSITIVE, MODE_NEGATIVE, or MODE_ESTIMATION");
    }

    std::vector<bool> result;
    result.reserve(positiveSamples.size() + negativeSamples.size());

    // Determine foreground status for all samples
    bool foreground = (mode == MODE_POSITIVE || mode == MODE_ESTIMATION);
    
    // Add flags for positive samples
    for (size_t i = 0; i < positiveSamples.size(); ++i) {
        result.push_back(foreground);
    }
    
    // Add flags for negative samples
    for (size_t i = 0; i < negativeSamples.size(); ++i) {
        result.push_back(foreground);
    }

    return result;
}
#include <cassert>
#include <vector>

// Include the solution here (or rely on the precompiled header in an actual project)
// For completeness, the Rect and enum and function from the solution are assumed available.

int main() {
    // Simple positive and negative samples
    std::vector<Rect> positives = {{10, 10, 20, 20}, {30, 30, 15, 15}};
    std::vector<Rect> negatives = {{100, 100, 25, 25}, {200, 200, 30, 30}, {300, 300, 40, 40}};

    // MODE_POSITIVE: all samples are foreground
    auto result1 = detectForegroundSamples(positives, negatives, MODE_POSITIVE);
    assert(result1.size() == 5);
    for (bool flag : result1) {
        assert(flag == true);
    }

    // MODE_NEGATIVE: all samples are background
    auto result2 = detectForegroundSamples(positives, negatives, MODE_NEGATIVE);
    assert(result2.size() == 5);
    for (bool flag : result2) {
        assert(flag == false);
    }

    // MODE_ESTIMATION: all samples are foreground
    auto result3 = detectForegroundSamples(positives, negatives, MODE_ESTIMATION);
    assert(result3.size() == 5);
    for (bool flag : result3) {
        assert(flag == true);
    }

    // Edge case: empty inputs
    std::vector<Rect> empty;
    auto result4 = detectForegroundSamples(empty, empty, MODE_POSITIVE);
    assert(result4.empty());

    auto result5 = detectForegroundSamples(empty, negatives, MODE_NEGATIVE);
    assert(result5.size() == 3);
    for (bool flag : result5) {
        assert(flag == false);
    }

    // Edge case: only positives
    auto result6 = detectForegroundSamples(positives, empty, MODE_ESTIMATION);
    assert(result6.size() == 2);
    for (bool flag : result6) {
        assert(flag == true);
    }

    // Invalid mode should throw
    bool threw = false;
    try {
        detectForegroundSamples(positives, negatives, 99);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);
}
