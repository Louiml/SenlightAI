Create a C++ function that processes a vector of floating-point numbers and returns a new vector where each element is the mathematical floor of the corresponding input element, preserving the original order. The function must handle empty input gracefully (return an empty vector), support negative values correctly (e.g., floor(-2.3) = -3.0), and operate on `std::vector<float>` with `const` reference input and a value-returned output.

The solution iterates over each element in the input vector and applies `std::floor` from `<cmath>`. The main algorithm is a simple element-wise transformation: for each value `x` in the input, compute `std::floor(x)`, which returns the largest integer value not greater than `x` (as a floating-point type). Edge cases include empty input (return an empty vector immediately) and negative numbers, where `std::floor` correctly rounds toward negative infinity (e.g., floor(-0.5) = -1.0). Time complexity is O(n) where n is the number of elements, because each element is visited exactly once. Space complexity is O(n) for the output vector, plus O(1) auxiliary space for the loop variable and temporary storage.

#include <vector>
#include <cmath>

// Return a vector containing the mathematical floor of each element in `input`.
std::vector<float> applyFloor(const std::vector<float>& input) {
    std::vector<float> output;
    output.reserve(input.size());

    for (const float value : input) {
        output.push_back(std::floor(value));
    }

    return output;
}

int main() {
    // Test empty input
    assert(applyFloor({}) == std::vector<float>{});

    // Test positive values
    assert(applyFloor({0.0f, 1.5f, 2.99f}) == std::vector<float>({0.0f, 1.0f, 2.0f}));

    // Test negative values
    assert(applyFloor({-0.5f, -1.0f, -2.3f}) == std::vector<float>({-1.0f, -1.0f, -3.0f}));

    // Test mixed values
    assert(applyFloor({-2.7f, 0.1f, 3.0f, -4.0f}) == std::vector<float>({-3.0f, 0.0f, 3.0f, -4.0f}));

    // Test exact integers (including zero)
    assert(applyFloor({-3.0f, 0.0f, 7.0f}) == std::vector<float>({-3.0f, 0.0f, 7.0f}));
}
