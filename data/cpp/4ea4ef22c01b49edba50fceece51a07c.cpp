/*
Write a standalone C++ function named `partialSums` that, given a vector of 32-bit integers and a block size `group_size`, returns a vector of integers where each element is the sum of a consecutive block of `group_size` elements from the input (the last block may be smaller if the input size is not a multiple of `group_size`). The function must compute the sums exactly as if using OpenCL local-memory reduction with a single work-group per block, meaning each block's sum is computed sequentially within that block (not parallel-accumulated in a different order), although for integer addition the order does not affect the result. The function should handle empty input by returning an empty vector, and handle `group_size` equal to zero by throwing an `std::invalid_argument`. Assume the input vector contains values that may overflow when summed; the function should wrap around using unsigned 32-bit arithmetic (i.e., use `uint32_t` and let overflow wrap) to match OpenCL `cl_int` behavior where addition wraps modulo 2^32. The output size must be `ceil(N / group_size)`.
*/
#include <vector>
#include <stdexcept>
#include <cstdint>

// Compute per-block sums of size group_size from input vector.
// Throws std::invalid_argument if group_size == 0.
// Returns a vector of uint32_t where each element is the sum of a
// consecutive block of group_size elements (last block may be smaller).
std::vector<uint32_t> partialSums(const std::vector<int32_t>& input, size_t group_size) {
    if (group_size == 0) {
        throw std::invalid_argument("group_size must be > 0");
    }

    const size_t N = input.size();
    const size_t n_groups = (N + group_size - 1) / group_size; // ceil division, handles N=0 correctly
    std::vector<uint32_t> result(n_groups, 0);

    for (size_t block = 0; block < n_groups; ++block) {
        uint32_t sum = 0;
        const size_t start = block * group_size;
        const size_t end = std::min(start + group_size, N); // exclusive end
        for (size_t i = start; i < end; ++i) {
            // Cast to uint32_t to allow wrap-around on overflow.
            sum += static_cast<uint32_t>(input[i]);
        }
        result[block] = sum;
    }

    return result;
}
#include <cassert>
#include <cstdint>
#include <vector>

// Include the solution function here (or link to it).
// (Place the partialSums function definition above.)

int main() {
    // Standard case: N=7, group_size=3 => blocks: [1,2,3]=6, [4,5,6]=15, [7]=7
    std::vector<int32_t> v1 = {1,2,3,4,5,6,7};
    std::vector<uint32_t> r1 = partialSums(v1, 3);
    assert(r1.size() == 3);
    assert(r1 == std::vector<uint32_t>({6, 15, 7}));

    // Exact multiple: N=6, group_size=3 => blocks: [1,2,3]=6, [4,5,6]=15
    std::vector<int32_t> v2 = {1,2,3,4,5,6};
    auto r2 = partialSums(v2, 3);
    assert(r2 == std::vector<uint32_t>({6, 15}));

    // group_size=1 => each element is its own sum
    std::vector<int32_t> v3 = {10, -20, 30};
    auto r3 = partialSums(v3, 1);
    assert(r3 == std::vector<uint32_t>({10, static_cast<uint32_t>(-20), 30}));

    // group_size larger than N => single block with total sum
    std::vector<int32_t> v4 = {5, 6, 7};
    auto r4 = partialSums(v4, 10);
    assert(r4.size() == 1);
    assert(r4[0] == 18);

    // Empty input => empty output
    std::vector<int32_t> v5;
    auto r5 = partialSums(v5, 4);
    assert(r5.empty());

    // Overflow behavior wraps modulo 2^32
    std::vector<int32_t> v6 = {INT32_MAX, 1};
    auto r6 = partialSums(v6, 2);
    // INT32_MAX + 1 wraps to INT32_MIN as uint32_t
    assert(r6[0] == static_cast<uint32_t>(INT32_MIN));

    // Negative numbers sum correctly modulo 2^32
    std::vector<int32_t> v7 = {-1, -2, -3};
    auto r7 = partialSums(v7, 2);
    // block0: -1 + -2 = -3, block1: -3
    assert(r7[0] == static_cast<uint32_t>(-3));
    assert(r7[1] == static_cast<uint32_t>(-3));

    // group_size=0 should throw
    bool threw = false;
    try {
        partialSums(v1, 0);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);

    return 0;
}
// The problem mirrors the provided OpenCL kernel pattern: the host divides the input into `n_groups = (N + group_size - 1) / group_size` blocks, each of size `group_size` except possibly the last block, which has size `N % group_size` (or `group_size` if N is a multiple). For each block, we accumulate the sum of its elements into a single result. Since the specification requires exact block sums, we iterate over each block and sum its elements sequentially. Use `uint32_t` to match OpenCL's `cl_int` which is a 32-bit signed integer, but with addition that wraps modulo 2^32 (two's complement). We must cast each input int to `uint32_t` for summation, then store the result as `uint32_t` in the output vector (or as `int32_t` if desired, but the problem asks for integers; we can return `vector<uint32_t>` or `vector<int32_t>` – the specification says "vector of integers", and we can choose `vector<uint32_t>` for clarity of wrapping). Edge cases: empty input → empty output; `group_size` ≤ 0 → throw `invalid_argument`; `N` not a multiple of `group_size` → last block smaller. Time complexity is O(N) because each element is visited once; space complexity is O(ceil(N/group_size)) for the output, which is O(N) in the worst case (when `group_size` = 1).
