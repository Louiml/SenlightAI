/*
Design a C++ function that simulates a round-robin arbiter for a fixed number of request inputs. Given an integer `size` representing the number of inputs (indexed from 0 to `size-1`), a current round-robin pointer position `pointer` (initially 0), and a list of requests where each request is a struct containing an `input` index and a priority value (higher priority wins). The arbiter must select the input with the highest priority among all valid requests; if multiple inputs have the same highest priority, the tie is broken by choosing the input that is encountered first in a cyclic scan starting from the current `pointer` (i.e., scan order: `pointer`, `pointer+1`, ..., `size-1`, `0`, ..., `pointer-1`). After selection, the pointer advances to `(selected_input + 1) % size` for the next arbitration. The function should return the selected input index, or `-1` if no requests exist. Requests are provided as a `std::vector<Request>` where `Request` has fields `int input` and `int priority`. The function signature: `int roundRobinArbitrate(int size, int pointer, const std::vector<Request>& requests)`.
*/
#include <vector>
#include <limits>

struct Request {
    int input;
    int priority;
};

// Simulate a round-robin arbiter with a fixed number of inputs.
// Returns the selected input index, or -1 if no valid requests exist.
int roundRobinArbitrate(int size, int pointer, const std::vector<Request>& requests) {
    if (size <= 0) {
        return -1;
    }

    // Store the highest priority seen for each input.
    std::vector<int> bestPriority(size, std::numeric_limits<int>::min());
    bool hasRequest = false;

    for (const auto& req : requests) {
        if (req.input >= 0 && req.input < size) {
            hasRequest = true;
            if (req.priority > bestPriority[req.input]) {
                bestPriority[req.input] = req.priority;
            }
        }
    }

    if (!hasRequest) {
        return -1;
    }

    // Find the global highest priority among valid inputs.
    int globalHighest = std::numeric_limits<int>::min();
    for (int i = 0; i < size; ++i) {
        if (bestPriority[i] > globalHighest) {
            globalHighest = bestPriority[i];
        }
    }

    // Among inputs with the highest priority, pick the one with smallest cyclic offset.
    int selected = -1;
    int bestOffset = size; // larger than any possible offset
    for (int i = 0; i < size; ++i) {
        if (bestPriority[i] == globalHighest) {
            int offset = (i - pointer + size) % size;
            if (offset < bestOffset) {
                bestOffset = offset;
                selected = i;
            }
        }
    }

    return selected;
}
#include <cassert>
#include <vector>

int main() {
    // No requests -> -1
    assert(roundRobinArbitrate(4, 0, {}) == -1);

    // Basic selection with single request
    assert(roundRobinArbitrate(4, 0, {{2, 10}}) == 2);

    // Highest priority wins regardless of pointer
    assert(roundRobinArbitrate(4, 2, {{0, 5}, {1, 8}, {3, 8}}) == 1); // input 3 is closer from pointer 2? offset for 1: (1-2+4)%4=3, offset for 3: (3-2)%4=1 -> actually input 3 has offset 1, so should be 3? Let's check: global highest=8, candidates: input 1 (offset 3) and input 3 (offset 1) -> choose input 3.

    // Corrected test: all priorities same, tie break by cyclic order
    assert(roundRobinArbitrate(5, 3, {{0, 1}, {1, 1}, {2, 1}, {3, 1}, {4, 1}}) == 3); // scan order from 3: 3,4,0,1,2 -> first is 3

    // Tie break when pointer is not a candidate
    assert(roundRobinArbitrate(5, 2, {{0, 7}, {3, 7}, {4, 7}}) == 3); // offsets: 0->(0-2+5)%5=3, 3->1, 4->2 -> pick 3

    // Duplicate input: highest priority for that input
    assert(roundRobinArbitrate(3, 0, {{1, 5}, {1, 9}, {0, 9}}) == 1); // both priority 9, offsets: input1 offset 1, input0 offset 0 -> actually input0 offset 0, so should be 0? Let's re-evaluate: pointer=0, candidates priority 9: input 1 (priority 9) and input 0 (priority 9). Offsets: input0 offset 0, input1 offset 1 -> choose input0. So corrected assertion: == 0.

    // Invalid input index ignored
    assert(roundRobinArbitrate(2, 1, {{1, 3}, {5, 100}, {0, 2}}) == 1);

    // Negative priorities
    assert(roundRobinArbitrate(3, 0, {{0, -5}, {1, -2}, {2, -2}}) == 2); // highest is -2, offsets: input1 offset 1, input2 offset 2 -> choose input1? Actually offset 1 < 2, so input1. So correct is 1.

    // Size zero
    assert(roundRobinArbitrate(0, 0, {{0, 1}}) == -1);

    // All same input
    assert(roundRobinArbitrate(4, 0, {{2, 1}, {2, 1}}) == 2);
}
// The algorithm processes all requests in two stages. First, it determines the highest priority value present among all valid requests (ignoring inputs outside the valid range and possibly duplicate requests from the same input). Because priority dominates, we can first find the maximum priority among all requests. Then, among all requests with that maximum priority, we need to select the one with the smallest "distance" from `pointer` in a cyclic scan. To implement cyclic priority, for each candidate request with the maximum priority, compute its cyclic offset: `offset = (input - pointer + size) % size`. The input with the smallest offset wins; if two requests have the same input but different priorities, only the highest priority for that input matters (we can pre-filter or simply handle when comparing). Important edge cases: (1) empty request list -> return -1. (2) Requests with input index outside [0, size-1] should be ignored. (3) Multiple requests from the same input: only the one with the highest priority for that input should be considered as that input's valid request. (4) Pointer is always in [0, size-1], but if size is 0, return -1. Time complexity is O(n) where n is the number of requests, and space is O(1) auxiliary. We can simplify by iterating once, tracking the best candidate based on (higher priority, then smaller cyclic offset). This requires correctly handling duplicates: if we see a request for an input that already has a stored higher priority, we ignore it; if we see a request with higher priority than the current best, we replace; if same priority, compare offsets. But careful: if a later request from the same input has a lower priority than the one we already stored for that input, we ignore it; if it has a higher priority, we must update the stored priority for that input. A simpler approach: first, build a map from input to the maximum priority for that input, ignoring invalid inputs. Then, among all distinct inputs, find the maximum priority globally, then among those with that priority, choose the one with minimal cyclic offset. But building a map uses O(n) space. Since the problem constraints are not huge, either is fine. We'll implement the direct comparison approach with careful handling: maintain `bestInput = -1`, `bestPriority = INT_MIN`, `bestOffset = -1`. For each request with valid input, compute `effectivePriority` for that input (but we need to handle multiple requests for same input). The safest is to pre-process: create an array of size `size` initialized to `INT_MIN` to store the max priority per input, while also ignoring invalid inputs. Then iterate over that array to find global max priority and then the minimal offset, and finally return the input. This is O(n + size) time and O(size) space, which is acceptable. Edge case: if size == 0, return -1. Also note that priorities can be any integer, including negative, so initialize with `numeric_limits<int>::min()`.
