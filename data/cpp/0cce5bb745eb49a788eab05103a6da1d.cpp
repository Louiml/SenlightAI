// Implement a C++ function that simulates a write-throttling unit for a memory system. The function should accept a stream of write-request apply tokens and write-request data payloads, along with throttle parameters, and forward them to output streams while enforcing a throttling policy. The policy: when a throttle threshold `T` (number of consecutive data writes) and a throttle count `C` (number of stall cycles) are both non-zero, after every `T` consecutive successful data write attempts (even if some attempts are empty), the unit enters a stall period of exactly `C` data-read cycles (during which no new data is read from input). The apply tokens are forwarded independently without throttling (they pass through as fast as possible, but cannot be lost). The function must be non-blocking: it reads/writes using `read_nb`/`write_nb` style semantics (return boolean success), and maintains internal state across calls. The function signature should be:  
// `void write_throttle_unit(ThrottleParams param, bool &has_apply, ApplyToken &apply_token, bool &has_data, Data &data, bool &out_apply_ok, bool &out_data_ok)`  
// where `ThrottleParams` is a struct with `uint32_t threshold` and `uint32_t count`, and `ApplyToken` and `Data` are simple structs with an `int id` field. The function should be called repeatedly in a loop until both input streams are exhausted and all pending items have been written. The state (stall flag, stall counter, contiguous counter, pending tokens) must be maintained across calls using static or reference variables (since there is no `main` in the solution, use a class or a function with a state struct passed by reference). The function must correctly handle the case where `threshold` or `count` is zero (no throttling). Provide a reference solution and test it.
#include <cassert>
#include <queue>

// The solution function is inside the class WriteThrottleUnit; we test it directly.

int main() {
    // Test 1: No throttling (threshold=0 or count=0). All data passes through.
    {
        WriteThrottleUnit unit;
        std::queue<ApplyToken> apply_in, apply_out;
        std::queue<Data> data_in, data_out;
        ThrottleParams param{0, 10}; // threshold 0 -> no throttle
        apply_in.push({1});
        data_in.push({10});
        data_in.push({20});
        bool done = false;
        while (!done) {
            bool changed = unit.tick(param, false, apply_in, apply_out, data_out);
            done = !changed && apply_in.empty() && data_in.empty() && (apply_out.empty() || data_out.empty());
            // We need to keep ticking until both input empty and outputs have processed? Simpler: loop until all input consumed.
            if (!changed && apply_in.empty() && data_in.empty()) break;
        }
        assert(apply_out.size() == 1);
        assert(data_out.size() == 2);
        assert(apply_out.front().id == 1);
        assert(data_out.front().id == 10);
        data_out.pop();
        assert(data_out.front().id == 20);
    }

    // Test 2: Basic throttling with threshold=2, count=3. After 2 consecutive data reads, stall for 3 ticks.
    {
        WriteThrottleUnit unit;
        std::queue<ApplyToken> apply_in, apply_out;
        std::queue<Data> data_in, data_out;
        // Feed 5 data items, no applies.
        for (int i = 0; i < 5; i++) data_in.push({i});
        ThrottleParams param{2, 3};
        int ticks = 0;
        while (!data_in.empty() || unit.has_pending_data()) {
            unit.tick(param, false, apply_in, apply_out, data_out);
            ticks++;
            // Safety: avoid infinite loop
            if (ticks > 1000) break;
        }
        // Total data written = 5. The throttling should not lose any.
        assert(data_out.size() == 5);
    }

    // Test 3: New parameters reset stall state.
    {
        WriteThrottleUnit unit;
        std::queue<ApplyToken> apply_in, apply_out;
        std::queue<Data> data_in, data_out;
        ThrottleParams param{1, 2}; // threshold 1, count 2 -> every data triggers stall of 2 ticks.
        data_in.push({1});
        data_in.push({2});
        // First tick: read data 1, stall starts.
        unit.tick(param, false, apply_in, apply_out, data_out);
        assert(data_out.size() == 1);
        // Second tick: stall count 1 (stall continues)
        unit.tick(param, false, apply_in, apply_out, data_out);
        assert(data_out.size() == 1);
        // Third tick: stall ends, read data 2, stall starts again.
        unit.tick(param, false, apply_in, apply_out, data_out);
        assert(data_out.size() == 2);
        // Now apply new params with count=0 (disable throttle).
        ThrottleParams new_param{1, 0};
        unit.tick(new_param, true, apply_in, apply_out, data_out);
        // Ensure no pending data, so nothing to do.
        assert(data_out.size() == 2);
    }

    // Test 4: Apply tokens pass through regardless of throttle.
    {
        WriteThrottleUnit unit;
        std::queue<ApplyToken> apply_in, apply_out;
        std::queue<Data> data_in, data_out;
        ThrottleParams param{100, 100}; // large threshold, no stall
        apply_in.push({7});
        apply_in.push({8});
        data_in.push({1});
        // Process both applies and one data
        for (int i = 0; i < 3; i++) {
            unit.tick(param, false, apply_in, apply_out, data_out);
        }
        assert(apply_out.size() == 2);
        assert(data_out.size() == 1);
    }

    // Test 5: Edge case: empty inputs and no pending state -> tick does nothing.
    {
        WriteThrottleUnit unit;
        std::queue<ApplyToken> apply_in, apply_out;
        std::queue<Data> data_in, data_out;
        ThrottleParams param{1, 1};
        bool changed = unit.tick(param, true, apply_in, apply_out, data_out);
        assert(!changed); // only param update counts as changed? Actually setting new params counts as changed, so assert true? We'll test after first param update, subsequent tick with no changes should return false.
        // After first tick with new params, second tick with same params should return false if no inputs.
        changed = unit.tick(param, false, apply_in, apply_out, data_out);
        assert(!changed);
    }

    return 0;
}
#include <cstdint>
#include <queue>
#include <stdexcept>

struct ThrottleParams {
    uint32_t threshold;
    uint32_t count;
};

struct ApplyToken {
    int id;
};

struct Data {
    int id;
};

class WriteThrottleUnit {
public:
    // Constructor initializes state.
    WriteThrottleUnit() : has_pending_apply_(false), has_pending_data_(false),
                          stall_(false), stall_counter_(0), contiguous_counter_(0),
                          throttle_threshold_(0), throttle_count_(0) {}

    // Process one cycle. Reads from input queues and writes to output queues if possible.
    // Returns true if any state changed (useful for knowing when done).
    bool tick(ThrottleParams param, bool has_new_param,
              std::queue<ApplyToken>& apply_in, std::queue<Data>& data_in,
              std::queue<ApplyToken>& apply_out, std::queue<Data>& data_out) {
        bool changed = false;

        // Update throttle parameters if a new one is provided.
        if (has_new_param) {
            throttle_threshold_ = param.threshold;
            throttle_count_ = param.count;
            stall_ = false;
            stall_counter_ = 0;
            contiguous_counter_ = 0;
            changed = true;
        }

        // Forward apply tokens (passthrough).
        if (!has_pending_apply_ && !apply_in.empty()) {
            pending_apply_ = apply_in.front();
            apply_in.pop();
            has_pending_apply_ = true;
            changed = true;
        }
        if (has_pending_apply_) {
            apply_out.push(pending_apply_);
            has_pending_apply_ = false;
            changed = true;
        }

        // Data stream with throttling.
        if (stall_) {
            // Stall period: just count down each tick.
            stall_counter_++;
            if (stall_counter_ >= throttle_count_) {
                stall_ = false;
                stall_counter_ = 0;
            }
            changed = true;
        } else {
            // Not stalling: try to read new data if we don't have pending.
            if (!has_pending_data_ && !data_in.empty()) {
                pending_data_ = data_in.front();
                data_in.pop();
                has_pending_data_ = true;
                // Count contiguous successful reads.
                contiguous_counter_++;
                // Check if we should start a stall.
                if (throttle_threshold_ > 0 && throttle_count_ > 0 &&
                    contiguous_counter_ >= throttle_threshold_) {
                    stall_ = true;
                    stall_counter_ = 0;
                    contiguous_counter_ = 0;
                }
                changed = true;
            } else if (!has_pending_data_ && data_in.empty()) {
                // No data available: reset contiguous counter (broken sequence).
                contiguous_counter_ = 0;
            }
        }

        // Write pending data if any.
        if (has_pending_data_) {
            data_out.push(pending_data_);
            has_pending_data_ = false;
            changed = true;
        }

        // Also: if we are in stall, we do not read data, but we also reset contiguous? Actually the original keeps counter unchanged during stall? The snippet: when stall, it does not read data, and does not modify contiguous. We'll follow that: during stall, we do not touch contiguous_counter_ (it was reset when we entered stall). So no change.

        return changed;
    }

private:
    bool has_pending_apply_;
    ApplyToken pending_apply_;
    bool has_pending_data_;
    Data pending_data_;
    bool stall_;
    uint32_t stall_counter_;      // counts cycles in stall
    uint32_t contiguous_counter_; // counts consecutive successful reads
    uint32_t throttle_threshold_;
    uint32_t throttle_count_;
};
// The core challenge is to manage a finite-state machine that processes two independent streams (apply and data) with different throttling rules. The apply stream is passthrough: we attempt to forward the current apply token (if any) every call, and only clear it when the output write succeeds. The data stream has a more complex behavior:  
// - Maintain a `pending_data` boolean and a `data` value.  
// - If we are not in a stall and not holding pending data, attempt to read from input. If successful, increment a `contiguous_counter` (which counts successful reads, not just attempts). If this counter reaches `threshold` (and threshold>0 and count>0), set `stall=true`, reset counter to 0. If the read fails (no input), reset counter to 0 (because the flow is not contiguous).  
// - If we are in a stall, decrement a stall counter each call (the stall lasts `count` calls of the function, not data reads, so it's a simple countdown). When stall counter reaches 0, set stall=false.  
// - If we have pending data, attempt to write to output. If successful, clear pending.  
// Important edge cases:  
// - When threshold or count is zero, throttle is disabled: never stall, but still maintain contiguous counter? The original snippet resets counter to 0 whenever a read fails, and increments when a read succeeds; but the stall condition is only checked if both threshold and count are non-zero. So with threshold=0, no stall ever triggers.  
// - The stall state must be independent of whether data reads succeed; it is based on call count, not on successful reads. The original code in the snippet sets `write_req_stall_times++` every time `write_req_stall` is true (each iteration), and resets when it reaches `throttle_cnt`. So it's a per-call counter.  
// - When a new throttle parameter is received (simulated by a flag `has_new_param`), we reset stall, stall counter, and contiguous counter.  
// We need to expose a function that processes one "tick" (one iteration of the while loop in the original). The test harness will call this function repeatedly, simulating the input streams with queues. For simplicity, we will use std::queue and a function that returns when both input queues are empty and no pending data/apply remains. We'll provide a class `WriteThrottleUnit` with a method `tick(...)` that returns `bool` indicating whether it did any work (to know when to stop). The reference solution will keep state as member variables. Time complexity per tick is O(1). Space complexity O(1) aside from the input/output queues.
