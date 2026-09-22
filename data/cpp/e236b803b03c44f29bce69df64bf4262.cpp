// Given a sorted vector of timestamps (strictly increasing) and a vector of "controller" data blocks, where each block contains a `timeStamp_` vector and corresponding `biasArray_` and `gainArray_` vectors of equal non-zero length (all vectors in all blocks are non-empty), plus two vectors of event times (`eventTimes` and `controllerEventTimes` of equal length, both sorted strictly increasing, with all values lying within the range of timestamps across all blocks), write a C++ function `spreadEventControllers` that adjusts the controller data so that the event times are moved from their original positions (given by `controllerEventTimes`) to new positions (given by `eventTimes`). The adjustment follows this rule: for each pair (newEvent, oldEvent) at the same index, identify the indices (block index and position within block) where these event times fall in the `timeStamp_` vectors using binary search (lower_bound). If either index is "undefined" (meaning the event is exactly at the final timestamp of the last non-empty block, or outside valid range), skip that pair. Otherwise, if the new event index is not the very first timestamp of the first non-empty block, compute an interpolation factor `alpha` from the timestamps surrounding the new event (previous timestamp and the new event's lower_bound timestamp). This alpha is used to create a corrected feedforward value `uffCorrected` and feedback gain `kCorrected` by linear interpolation between the values at those two surrounding timestamps. Then, depending on whether the new event is earlier or later than the old event, set a contiguous range of entries (from the new event index to the old event index, inclusive, across blocks that may span multiple blocks) in `biasArray_` and `gainArray_` to either `uffCorrected`/`kCorrected` (when new is earlier and we start at the new event) or to the value at the old event's following timestamp (when new is earlier) or the value at the old event's timestamp (when new is later). Also, when new is earlier, set the timestamp at the new event index to the new event time; when new is later, set the timestamp at the previous index (before the new event) to the new event time. The function must modify the provided vector of controller blocks in place and handle skipped empty blocks (but here all blocks are non-empty, so treat all blocks as valid). The function signature is `void spreadEventControllers(const std::vector<double>& eventTimes, const std::vector<double>& controllerEventTimes, std::vector<ControllerBlock>& controllers)` where `ControllerBlock` is a struct with `std::vector<double> timeStamp_`, `biasArray_`, `gainArray_`. The input guarantees that no event time equals the final timestamp of the last block (so "undefined" only occurs if a search fails, but you can assume it never happens under the given constraints; still, guard against it). Assume all timestamps are strictly increasing within each block and across blocks (i.e., the concatenation is sorted). The function should perform the described adjustment exactly.

// The core algorithm processes each event pair sequentially. For each pair, we locate the indices using a two-level binary search: for a given event time, we iterate over blocks from the current search starting block (to be efficient we can start from the previous event's block for the next event, but simpler is to just search all blocks from the beginning each time). For each block, we use `std::lower_bound` to find the first timestamp >= eventTime. If found, that gives the block index and the position within the block; otherwise move to next block. Since the concatenated timestamps are sorted, the first block containing a lower_bound is the correct one. Because we assume all event times are within the range, the search always succeeds. We also track the first and last non-empty block indices (here all blocks are non-empty, so first=0 and last = size-1). For each event pair, if the found index is (first block, position 0) OR the other index is (first block, position 0), we skip (per original code). Also if either index is undefined (which shouldn't happen) we skip. Otherwise, we compute the previous index (the timestamp immediately before the event's lower_bound index; if the lower_bound is at position 0 of a block, the previous index is the last position of the previous block, unless it's the very first block, in which case we don't have a previous and we already skipped). Then we compute alpha as (eventTime - timeStamp[prevIndex]) / (timeStamp[index] - timeStamp[prevIndex]). Since timestamps are distinct, denominator > 0. Then `uffCorrected` and `kCorrected` are the linear interpolation between the bias/gain at prevIndex and at index. Then we determine start and final indices for overwriting. If new event index is <= old event index (using lexicographic order: block index first, then position), then start = new index, final = old index, and we will overwrite from start to final inclusive (note: original code uses finalIndex = controlerEventTimeIndex and then in the loop for i == finalIndex it uses finalItr = finalIndex.second, not +1; but the original code has a suspicious `+1` in reading the spread values but then sets finalItr to finalIndex.second, so we follow that pattern). The spread value is taken from the old event index's following entry (i.e., at controlerEventTimeIndex.second + 1) if that position exists; under the given constraints we assume it does (the old event is not the very last timestamp). Also set the timestamp at the new event index to the new event time. If new index > old index, then start = old index, final = previous index before new event, and the spread value is taken from the old event index itself (biasArray_[oldIndex.second] and gainArray_[oldIndex.second]). Also set the timestamp at the previous index (before new event) to the new event time. Then we loop over all blocks from start.first to final.first, and within each block from the appropriate start position to end position (inclusive) and assign the biasArray_ and gainArray_ entries to the computed spread values. The loop must handle the case where start and final are in the same block or different blocks. Complexity: For each of the m event pairs, we do a search that could scan up to B blocks, each lower_bound O(log n) where n is the average block size, so O(B log n) per event. The overwriting loop could span many blocks and positions, worst-case O(total number of entries) per event, so overall O(m * totalEntries) in the worst case. With typical small m and many blocks, it's acceptable. Space complexity is O(1) extra besides the input.

#include <vector>
#include <algorithm>
#include <cstddef>

// A controller block: timestamps and associated feedforward bias and feedback gain.
struct ControllerBlock {
    std::vector<double> timeStamp_;
    std::vector<double> biasArray_;  // feedforward part
    std::vector<double> gainArray_;  // feedback part
};

// Helper: find the (blockIndex, position) of the first timestamp >= value.
// Returns false if not found in any block.
bool findLowerBound(const std::vector<ControllerBlock>& controllers,
                    double value,
                    std::pair<std::size_t, std::size_t>& result) {
    for (std::size_t p = 0; p < controllers.size(); ++p) {
        const auto& timestamps = controllers[p].timeStamp_;
        if (timestamps.empty()) continue;
        auto it = std::lower_bound(timestamps.begin(), timestamps.end(), value);
        if (it != timestamps.end()) {
            result = {p, static_cast<std::size_t>(it - timestamps.begin())};
            return true;
        }
    }
    return false;
}

// Helper: return the index immediately before the given index (in the concatenated timestamps).
// Assumes there is a previous index (i.e., not the very first timestamp).
std::pair<std::size_t, std::size_t> previousIndex(
        std::pair<std::size_t, std::size_t> idx,
        const std::vector<ControllerBlock>& controllers) {
    if (idx.second > 0) {
        return {idx.first, idx.second - 1};
    } else {
        // must be a block with index > 0 because there is a previous timestamp
        return {idx.first - 1, controllers[idx.first - 1].timeStamp_.size() - 1};
    }
}

// Lexicographic comparison: by block index, then by position.
bool isSmallerOrEqual(const std::pair<std::size_t, std::size_t>& a,
                      const std::pair<std::size_t, std::size_t>& b) {
    if (a.first < b.first) return true;
    if (a.first > b.first) return false;
    return a.second <= b.second;
}

// Adjust the controller blocks so that event times occur at the new positions.
void spreadEventControllers(const std::vector<double>& eventTimes,
                            const std::vector<double>& controllerEventTimes,
                            std::vector<ControllerBlock>& controllers) {
    const std::size_t numEvents = eventTimes.size();
    if (controllerEventTimes.size() != numEvents) {
        return; // invalid input; the task guarantees equality, but guard.
    }
    if (controllers.empty()) return;

    // Assume all blocks are non-empty per task description.
    const std::size_t firstBlock = 0;
    const std::size_t lastBlock = controllers.size() - 1;

    // Process each event pair.
    for (std::size_t j = 0; j < numEvents; ++j) {
        double newTime = eventTimes[j];
        double oldTime = controllerEventTimes[j];

        // Find indices for new and old event times.
        std::pair<std::size_t, std::size_t> newIdx, oldIdx;
        if (!findLowerBound(controllers, newTime, newIdx) ||
            !findLowerBound(controllers, oldTime, oldIdx)) {
            continue; // undefined index, skip per original logic.
        }

        // Skip events at the very start of the first controller.
        if (newIdx == std::make_pair(firstBlock, std::size_t(0)) ||
            oldIdx == std::make_pair(firstBlock, std::size_t(0))) {
            continue;
        }

        // Compute previous index before the new event.
        auto newPrev = previousIndex(newIdx, controllers);

        // Interpolation factor alpha between newPrev and newIdx.
        double tPrev = controllers[newPrev.first].timeStamp_[newPrev.second];
        double tCurr = controllers[newIdx.first].timeStamp_[newIdx.second];
        double alpha = (newTime - tPrev) / (tCurr - tPrev);

        double uffCorrected = (1.0 - alpha) * controllers[newPrev.first].biasArray_[newPrev.second] +
                              alpha * controllers[newIdx.first].biasArray_[newIdx.second];
        double kCorrected = (1.0 - alpha) * controllers[newPrev.first].gainArray_[newPrev.second] +
                            alpha * controllers[newIdx.first].gainArray_[newIdx.second];

        std::pair<std::size_t, std::size_t> startIdx, finalIdx;
        double uffSpread, kSpread;

        if (isSmallerOrEqual(newIdx, oldIdx)) {
            // New event is earlier or at same position as old event.
            startIdx = newIdx;
            finalIdx = oldIdx;

            // Spread value comes from the entry right after the old event.
            // Task guarantees this position exists (old event is not the last timestamp).
            uffSpread = controllers[oldIdx.first].biasArray_[oldIdx.second + 1];
            kSpread = controllers[oldIdx.first].gainArray_[oldIdx.second + 1];

            // Update timestamp at the new event index.
            controllers[newIdx.first].timeStamp_[newIdx.second] = newTime;
        } else {
            // New event is later than old event.
            startIdx = oldIdx;
            finalIdx = newPrev;

            // Spread value comes from the old event's entry itself.
            uffSpread = controllers[oldIdx.first].biasArray_[oldIdx.second];
            kSpread = controllers[oldIdx.first].gainArray_[oldIdx.second];

            // Update timestamp at the previous index (before the new event).
            controllers[newPrev.first].timeStamp_[newPrev.second] = newTime;
        }

        // Overwrite the range from startIdx to finalIdx inclusive.
        for (std::size_t i = startIdx.first; i <= finalIdx.first; ++i) {
            std::size_t beginPos = (i == startIdx.first) ? startIdx.second : 0;
            std::size_t endPos = (i == finalIdx.first) ? finalIdx.second : 
                                                         controllers[i].timeStamp_.size() - 1;
            for (std::size_t k = beginPos; k <= endPos; ++k) {
                controllers[i].biasArray_[k] = uffSpread;
                controllers[i].gainArray_[k] = kSpread;
            }
        }
    }
}

#include <cassert>
#include <cmath>

// The ControllerBlock struct and spreadEventControllers function are assumed to be defined above.
// (Include the definition from the Solution section here for the test.)

int main() {
    // Test 1: Single block, move an event later.
    {
        std::vector<ControllerBlock> controllers;
        controllers.push_back({ {0.0, 1.0, 2.0, 3.0}, {0.0, 10.0, 20.0, 30.0}, {0.0, 0.1, 0.2, 0.3} });
        std::vector<double> eventTimes = {2.5};
        std::vector<double> controllerEventTimes = {2.0};
        spreadEventControllers(eventTimes, controllerEventTimes, controllers);
        // After adjustment, timestamps: 0,1,2.5,3 ; biases: 0,10,15,15? Wait compute manually.
        // newIdx for 2.5 is position 2 (timestamp 2.0? actually lower_bound of 2.5 in {0,1,2,3} gives index 3? No, 2.5 >2 and <3, lower_bound returns index 3 (value 3). So newIdx=(0,3), newPrev=(0,2). alpha=(2.5-2)/(3-2)=0.5. uffCorrected=0.5*20+0.5*30=25. kCorrected=0.25. Since newIdx (pos3) > oldIdx (pos2), start=oldIdx=(0,2), final=newPrev=(0,2). spread from oldIdx itself: uffSpread=20, kSpread=0.2. Overwrite pos2 to 20,0.2. Set timestamp at newPrev (pos2) to 2.5. So timestamps become {0,1,2.5,3}, biases {0,10,20,30}, gains {0,0.1,0.2,0.3}. Assert.
        assert(controllers[0].timeStamp_ == std::vector<double>({0.0, 1.0, 2.5, 3.0}));
        assert(controllers[0].biasArray_ == std::vector<double>({0.0, 10.0, 20.0, 30.0}));
        assert(controllers[0].gainArray_ == std::vector<double>({0.0, 0.1, 0.2, 0.3}));
    }

    // Test 2: Single block, move an event earlier.
    {
        std::vector<ControllerBlock> controllers;
        controllers.push_back({ {0.0, 1.0, 2.0, 3.0}, {0.0, 10.0, 20.0, 30.0}, {0.0, 0.1, 0.2, 0.3} });
        std::vector<double> eventTimes = {1.5};
        std::vector<double> controllerEventTimes = {2.0};
        spreadEventControllers(eventTimes, controllerEventTimes, controllers);
        // newIdx for 1.5 is pos2 (timestamp 2.0). newPrev=(0,1). alpha=(1.5-1)/(2-1)=0.5 -> uffCorrected=15, kCorrected=0.15.
        // newIdx (pos2) <= oldIdx (pos2) -> start=(0,2), final=(0,2). spread from oldIdx.second+1 = pos3 -> uffSpread=30,k=0.3.
        // Overwrite pos2 with 30,0.3. Set timestamp at newIdx (pos2) to 1.5. timestamps: {0,1,1.5,3} biases {0,10,30,30}.
        assert(controllers[0].timeStamp_[2] == 1.5);
        assert(controllers[0].biasArray_[2] == 30.0);
        assert(controllers[0].gainArray_[2] == 0.3);
    }

    // Test 3: Two blocks, event moves from block 1 to block 0.
    {
        std::vector<ControllerBlock> controllers;
        controllers.push_back({ {0.0, 1.0}, {0.0, 10.0}, {0.0, 0.1} });
        controllers.push_back({ {2.0, 3.0, 4.0}, {20.0, 30.0, 40.0}, {0.2, 0.3, 0.4} });
        std::vector<double> eventTimes = {1.5};
        std::vector<double> controllerEventTimes = {2.0};
        spreadEventControllers(eventTimes, controllerEventTimes, controllers);
        // newIdx for 1.5: in block0, lower_bound of 1.5 in {0,1} returns end (not found), move to block1, lower_bound of 1.5 in {2,3,4} gives pos0. So newIdx=(1,0). newPrev=(0,1). alpha=(1.5-1)/(2-1)=0.5 -> uffCorrected=15, kCorrected=0.15.
        // oldIdx for 2.0 is (1,0). newIdx == oldIdx, so start=(1,0), final=(1,0). spread from oldIdx.second+1 = pos1 in block1: uffSpread=30,k=0.3. Overwrite (1,0) with 30,0.3. Set timestamp at newIdx (1,0) to 1.5.
        // Also previous index (0,1) timestamp? Actually in else branch (since newIdx <= oldIdx true) we set timestamp at newIdx, not prev. So block1 timestamp[0] becomes 1.5. Block0 unchanged.
        assert(controllers[1].timeStamp_[0] == 1.5);
        assert(controllers[1].biasArray_[0] == 30.0);
        assert(controllers[1].gainArray_[0] == 0.3);
        // Check block0 unchanged.
        assert(controllers[0].biasArray_[1] == 10.0);
    }

    // Test 4: Two blocks, event moves from block 0 to block 1.
    {
        std::vector<ControllerBlock> controllers;
        controllers.push_back({ {0.0, 1.0}, {0.0, 10.0}, {0.0, 0.1} });
        controllers.push_back({ {2.0, 3.0, 4.0}, {20.0, 30.0, 40.0}, {0.2, 0.3, 0.4} });
        std::vector<double> eventTimes = {2.5};
        std::vector<double> controllerEventTimes = {1.0};
        spreadEventControllers(eventTimes, controllerEventTimes, controllers);
        // newIdx for 2.5: in block1, lower_bound gives pos1 (timestamp 3.0). newPrev=(1,0) timestamp 2.0. alpha=(2.5-2)/(3-2)=0.5 -> uffCorrected=25, k=0.25.
        // oldIdx for 1.0 is (0,1). newIdx (1,1) > oldIdx (0,1). So start=oldIdx=(0,1), final=newPrev=(1,0). spread from oldIdx itself: uffSpread=10, k=0.1. Set timestamp at newPrev (1,0) to 2.5.
        // Overwrite from block0 pos1 to block1 pos0 inclusive: block0 pos1, block1 pos0.
        assert(controllers[0].biasArray_[1] == 10.0);
        assert(controllers[1].biasArray_[0] == 10.0);
        assert(controllers[1].timeStamp_[0] == 2.5);
        // Block1 pos1 unchanged? Actually not overwritten.
        assert(controllers[1].biasArray_[1] == 30.0);
    }

    // Test 5: Event at exactly a timestamp, no interpolation change? The algorithm still works.
    {
        std::vector<ControllerBlock> controllers;
        controllers.push_back({ {0.0, 1.0, 2.0}, {0.0, 10.0, 20.0}, {0.0, 0.1, 0.2} });
        std::vector<double> eventTimes = {1.0};
        std::vector<double> controllerEventTimes = {1.0};
        spreadEventControllers(eventTimes, controllerEventTimes, controllers);
        // newIdx=(0,1), oldIdx=(0,1), newPrev=(0,0), alpha=(1-0)/(1-0)=1 -> uffCorrected=10, k=0.1.
        // newIdx <= oldIdx, start=final=(0,1), spread from oldIdx.second+1=2 -> uffSpread=20,k=0.2.
        // Overwrite pos1 with 20,0.2. timestamp at pos1 set to 1.0 (unchanged).
        assert(controllers[0].biasArray_[1] == 20.0);
        assert(controllers[0].gainArray_[1] == 0.2);
        assert(controllers[0].timeStamp_[1] == 1.0);
    }

    // Test 6: Multiple events in sequence (two events in same block).
    {
        std::vector<ControllerBlock> controllers;
        controllers.push_back({ {0.0, 1.0, 2.0, 3.0, 4.0}, {0.0, 10.0, 20.0, 30.0, 40.0}, {0.0, 0.1, 0.2, 0.3, 0.4} });
        std::vector<double> eventTimes = {1.5, 3.5};
        std::vector<double> controllerEventTimes = {1.0, 3.0};
        spreadEventControllers(eventTimes, controllerEventTimes, controllers);
        // Process first pair: new 1.5, old 1.0. newIdx for 1.5 -> pos2 (timestamp 2.0), newPrev pos1 (timestamp 1.0). alpha=(1.5-1)/(2-1)=0.5 -> uff=15,k=0.15. oldIdx pos1. newIdx>=oldIdx -> start=oldIdx pos1, final=newPrev pos1, spread from oldIdx itself (bias at pos1=10,k=0.1). Overwrite pos1 with 10,0.1. Set timestamp at pos1 to 1.5. So timestamps become {0,1.5,2,3,4}. biases still {0,10,20,30,40}? Actually overwrote pos1 with 10, unchanged. 
        // Second pair: new 3.5, old 3.0. newIdx for 3.5 -> pos4 (timestamp 4.0), newPrev pos3 (timestamp 3.0). alpha=(3.5-3)/(4-3)=0.5 -> uff=35,k=0.35. oldIdx pos3. newIdx>=oldIdx -> start=oldIdx pos3, final=newPrev pos3, spread from oldIdx itself (bias at pos3=30,k=0.3). Overwrite pos3 with 30,0.3. Set timestamp at pos3 to 3.5.
        // Check final timestamps: {0,1.5,2,3.5,4}, biases {0,10,20,30,40}.
        assert(controllers[0].timeStamp_[1] == 1.5);
        assert(controllers[0].timeStamp_[3] == 3.5);
        assert(controllers[0].biasArray_[2] == 20.0);
        assert(controllers[0].gainArray_[3] == 0.3);
    }

    return 0;
}
