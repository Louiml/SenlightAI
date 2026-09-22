Write a C++ function named `SampleTimesToParentTime` that takes three parameters: a `std::map<double, std::string>` representing local sample data (keyed by sample time, value as a placeholder string, but you may ignore the value), a `double` representing a reference "frame time" in seconds relative to an animation's start, and a `double` representing frames-per-second (fps). The function must iterate over all keys (sample times) in the input map. For each sample time `s`, compute the relative time as `(s - frameTime) * fps`, clamping results that are within an epsilon of `1e-4` (in absolute value) to exactly `0.0`. Return a new `std::map<double, double>` where each key is the original sample time and the value is the computed relative time. If the input map is empty, return an empty map. The function must not modify the input, and must handle negative frame times and negative sample times correctly.

The solution iterates over the sorted keys of the input map (since `std::map` is ordered). For each key `s`, we compute `relative = (s - frameTime) * fps`. Then we check if `fabs(relative) < epsilon` (with epsilon = `1e-4`) and if so, set it to `0.0` to avoid floating-point noise. We store the pair `(s, relative)` in the output map, preserving the original sample time as key and the relative time as value. Edge cases: empty input returns empty output; negative frameTime or fps produce correct arithmetic; sample times exactly equal to frameTime produce relative zero (which gets clamped); and very small relative values within epsilon are clamped to zero. Time complexity is O(n log n) due to map insertion (though the input is already sorted, insertion into a new map is O(log n) per element, so total O(n log n)). Space complexity is O(n) for the output map.

#include <map>
#include <cmath>
#include <algorithm>

// Given a map of local sample times (keys) and a frame time reference,
// return a map from each original sample time to its relative time computed as
// (sampleTime - frameTime) * fps, with values within epsilon of 0 clamped to 0.
std::map<double, double> SampleTimesToParentTime(
        const std::map<double, std::string>& localSamples,
        double frameTime,
        double fps)
{
    std::map<double, double> result;
    const double epsilon = 1e-4;

    for (const auto& entry : localSamples)
    {
        double sampleTime = entry.first;
        double relative = (sampleTime - frameTime) * fps;
        if (std::fabs(relative) < epsilon)
        {
            relative = 0.0;
        }
        result[sampleTime] = relative;
    }

    return result;
}

#include <cassert>
#include <map>
#include <string>

int main()
{
    // Empty input -> empty output
    std::map<double, std::string> empty;
    auto out = SampleTimesToParentTime(empty, 1.0, 24.0);
    assert(out.empty());

    // Simple case: sample times around frame time
    std::map<double, std::string> samples;
    samples[0.0] = "a";
    samples[0.5] = "b";
    samples[1.0] = "c";
    samples[1.5] = "d";
    auto res = SampleTimesToParentTime(samples, 1.0, 24.0);
    assert(res.size() == 4);
    assert(res.at(0.0) == -24.0);
    assert(res.at(0.5) == -12.0);
    assert(res.at(1.0) == 0.0);
    assert(res.at(1.5) == 12.0);

    // Clamping within epsilon
    std::map<double, std::string> tiny;
    tiny[1.0000001] = "x";
    auto res2 = SampleTimesToParentTime(tiny, 1.0, 24.0);
    assert(res2.at(1.0000001) == 0.0);

    // Negative frame time and negative sample times
    std::map<double, std::string> neg;
    neg[-2.0] = "p";
    neg[-1.0] = "q";
    auto res3 = SampleTimesToParentTime(neg, -1.5, 30.0);
    assert(res3.at(-2.0) == -15.0);
    assert(res3.at(-1.0) == 15.0);

    // fps of 1 and large time difference
    std::map<double, std::string> large;
    large[10.0] = "z";
    auto res4 = SampleTimesToParentTime(large, 2.0, 1.0);
    assert(res4.at(10.0) == 8.0);

    return 0;
}
