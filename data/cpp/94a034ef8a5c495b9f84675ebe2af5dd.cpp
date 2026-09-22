// Write a C++ function that simulates the parameter-parsing portion of the provided MEX interface for an optical flow routine. Given a vector of floating-point parameters (as would be passed from MATLAB) and a default value for each of the six settings (alpha, ratio, minWidth, nOuterFPIterations, nInnerFPIterations, nSORIterations), the function should overwrite each setting with the corresponding parameter value if it exists in the input vector, and return a struct (or a tuple) containing the final six values. The input parameter vector may have between 0 and 6 entries; any missing trailing parameters must retain their default values. All six parameters are positive, with alpha, ratio, and minWidth being real numbers, and the three iteration counts being integers (though they may be passed as doubles).

#include <cassert>
#include <vector>

// Struct definition is above; test code assumes it is available.
int main() {
    // Defaults when empty.
    OpticalFlowParams p = parseOpticalFlowParams({});
    assert(p.alpha == 1.0);
    assert(p.ratio == 0.5);
    assert(p.minWidth == 40);
    assert(p.nOuterFPIterations == 3);
    assert(p.nInnerFPIterations == 1);
    assert(p.nSORIterations == 20);
    
    // Partial parameters: only alpha and ratio provided.
    p = parseOpticalFlowParams({0.2, 0.7});
    assert(p.alpha == 0.2);
    assert(p.ratio == 0.7);
    assert(p.minWidth == 40);
    assert(p.nOuterFPIterations == 3);
    assert(p.nInnerFPIterations == 1);
    assert(p.nSORIterations == 20);
    
    // All six parameters: verify integer casting.
    p = parseOpticalFlowParams({0.1, 0.9, 32, 5, 2, 10});
    assert(p.alpha == 0.1);
    assert(p.ratio == 0.9);
    assert(p.minWidth == 32);
    assert(p.nOuterFPIterations == 5);
    assert(p.nInnerFPIterations == 2);
    assert(p.nSORIterations == 10);
    
    // More than 6 entries: extras ignored.
    p = parseOpticalFlowParams({0.3, 0.4, 25, 7, 3, 15, 99.0, -1.0});
    assert(p.alpha == 0.3);
    assert(p.ratio == 0.4);
    assert(p.minWidth == 25);
    assert(p.nOuterFPIterations == 7);
    assert(p.nInnerFPIterations == 3);
    assert(p.nSORIterations == 15);
    
    // Integer parameters passed as whole doubles.
    p = parseOpticalFlowParams({2.0, 0.3, 50.0, 4.0, 1.0, 20.0});
    assert(p.alpha == 2.0);
    assert(p.ratio == 0.3);
    assert(p.minWidth == 50);
    assert(p.nOuterFPIterations == 4);
    assert(p.nInnerFPIterations == 1);
    assert(p.nSORIterations == 20);
    
    return 0;
}

#include <vector>
#include <cstddef>

// Holds the six adjustable optical flow parameters.
struct OpticalFlowParams {
    double alpha;
    double ratio;
    int minWidth;
    int nOuterFPIterations;
    int nInnerFPIterations;
    int nSORIterations;
};

// Parse a vector of provided parameters, applying defaults for missing entries.
// The parameter vector may contain 0 to 6 values in the order:
// alpha, ratio, minWidth, nOuterFPIterations, nInnerFPIterations, nSORIterations.
// Any extra entries beyond 6 are ignored.
OpticalFlowParams parseOpticalFlowParams(const std::vector<double>& params) {
    OpticalFlowParams result;
    
    // Defaults from original code.
    result.alpha = 1.0;
    result.ratio = 0.5;
    result.minWidth = 40;
    result.nOuterFPIterations = 3;
    result.nInnerFPIterations = 1;
    result.nSORIterations = 20;
    
    const std::size_t n = params.size();
    
    if (n > 0) result.alpha = params[0];
    if (n > 1) result.ratio = params[1];
    if (n > 2) result.minWidth = static_cast<int>(params[2]);
    if (n > 3) result.nOuterFPIterations = static_cast<int>(params[3]);
    if (n > 4) result.nInnerFPIterations = static_cast<int>(params[4]);
    if (n > 5) result.nSORIterations = static_cast<int>(params[5]);
    
    return result;
}

// The solution must first determine how many parameters were provided, which is simply `min(parameters.size(), 6)`. Then iterate through the provided entries and assign them to the corresponding output fields, leaving the remaining fields at their default values. Since the input is a vector of doubles, the integer fields must be cast to integers (e.g., `static_cast<int>`). The function should be `const`-correct, taking the parameter vector by `const std::vector<double>&` and returning a struct with the six fields. Edge cases: an empty vector yields all defaults; a vector with more than 6 entries ignores the extras (matching the original code which reads at most 6). Time complexity is O(min(parameters.size(), 6)), effectively O(1) since the upper bound is constant; space complexity is O(1) for the returned struct.
