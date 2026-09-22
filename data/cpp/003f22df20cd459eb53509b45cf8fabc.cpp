Design a C++ state machine dispatcher class that manages a collection of state machines and routes serialized request payloads to the correct handler based on an embedded integer ID. Each payload begins with a 4-byte little-endian integer SMID followed by the actual body data. The dispatcher must support: (1) registering state machines with unique IDs, where duplicates are ignored; (2) executing a single payload by parsing the SMID, extracting the body, and calling the matching state machine's `Execute` method, returning `false` if no matching SMID is found or if no state machines are registered; (3) skipping payloads where SMID is 0 (treated as "no-op") with a return of `true`; and (4) batch execution where multiple `{smid, value}` pairs are provided as a serialized protocol – you may simulate batch parsing using a simple binary format (e.g., a 4-byte count followed by repeated `{4-byte smid, 4-byte value length, value bytes}`). For batch, execute each entry sequentially, and return `false` if any single execution fails. The solution must define a `StateMachine` base class with virtual methods `SMID()` and `Execute(int groupIdx, uint64_t instanceID, const std::string& body, void* ctx)`, and a `SMFac` (State Machine Factory) class that implements `AddSM`, `Execute`, `ExecuteForCheckpoint` (same behavior but calls `ExecuteForCheckpoint` on the state machine), and `PackPaxosValue` which prepends the SMID to a body. Provide a free helper function `RunBatch` that given a vector of `(smid, body)` pairs, constructs the batch binary representation and processes it via the dispatcher. Ensure const-correctness on methods that do not modify state.

#include <cassert>
#include <cstdint>
#include <string>
#include <vector>

// Concrete test state machines.
class TestSM1 : public StateMachine {
public:
    int SMID() const override { return 1; }
    bool Execute(int, uint64_t, const std::string& body, void*) override {
        return body == "alpha";
    }
    bool ExecuteForCheckpoint(int, uint64_t, const std::string& body) override {
        return body == "alpha";
    }
};

class TestSM2 : public StateMachine {
public:
    int SMID() const override { return 2; }
    bool Execute(int, uint64_t, const std::string& body, void*) override {
        return body == "beta";
    }
    bool ExecuteForCheckpoint(int, uint64_t, const std::string& body) override {
        return body == "beta";
    }
};

class FailingSM : public StateMachine {
public:
    int SMID() const override { return 3; }
    bool Execute(int, uint64_t, const std::string&, void*) override {
        return false; // always fails
    }
    bool ExecuteForCheckpoint(int, uint64_t, const std::string&) override {
        return false;
    }
};

int main() {
    SMFac dispatcher(0);
    TestSM1 sm1;
    TestSM2 sm2;
    dispatcher.AddSM(&sm1);
    dispatcher.AddSM(&sm2);

    // Single execution with known SMID 1 and correct body.
    std::string value = "x"; // placeholder, will be replaced
    SMFac::PackPaxosValue(value, 1);
    // Note: PackPaxosValue adds body to the *existing* string, so we must start empty.
    std::string singleValue;
    SMFac::PackPaxosValue(singleValue, 1);
    singleValue.append("alpha"); // body
    // Actually correct order: pack expects body already present. So:
    singleValue = "alpha";
    SMFac::PackPaxosValue(singleValue, 1);
    assert(dispatcher.Execute(100, singleValue) == true);

    // SMID 2 with correct body.
    std::string v2 = "beta";
    SMFac::PackPaxosValue(v2, 2);
    assert(dispatcher.Execute(101, v2) == true);

    // SMID 0 should be skipped (return true) even with garbage body.
    std::string v0;
    SMFac::PackPaxosValue(v0, 0);
    v0.append("anything");
    assert(dispatcher.Execute(102, v0) == true);

    // Unknown SMID 99 should return false.
    std::string v99;
    SMFac::PackPaxosValue(v99, 99);
    v99.append("nobody");
    assert(dispatcher.Execute(103, v99) == false);

    // Malformed payload (less than 4 bytes) returns true.
    assert(dispatcher.Execute(104, "abc") == true);

    // Batch execution with two valid entries.
    std::vector<std::pair<int, std::string>> batch = {{1, "alpha"}, {2, "beta"}};
    assert(RunBatch(dispatcher, 200, batch) == true);

    // Batch with a failing entry should return false.
    FailingSM failSm;
    dispatcher.AddSM(&failSm);
    std::vector<std::pair<int, std::string>> badBatch = {{1, "alpha"}, {3, "whatever"}};
    assert(RunBatch(dispatcher, 201, badBatch) == false);

    // Empty batch returns true.
    std::vector<std::pair<int, std::string>> emptyBatch;
    assert(RunBatch(dispatcher, 202, emptyBatch) == true);

    // Batch with SMID 0 entry (skip) and valid entry.
    std::vector<std::pair<int, std::string>> mixedBatch = {{0, "ignored"}, {1, "alpha"}};
    assert(RunBatch(dispatcher, 203, mixedBatch) == true);

    return 0;
}

#include <cstdint>
#include <cstring>
#include <string>
#include <vector>

// Base class for all state machines.
class StateMachine {
public:
    virtual ~StateMachine() = default;
    virtual int SMID() const = 0;
    virtual bool Execute(int groupIdx, uint64_t instanceID, const std::string& body, void* ctx) = 0;
    virtual bool ExecuteForCheckpoint(int groupIdx, uint64_t instanceID, const std::string& body) = 0;
};

// Dispatcher class that routes serialized payloads to registered state machines.
class SMFac {
public:
    explicit SMFac(int myGroupIdx = 0) : m_groupIdx(myGroupIdx) {}

    // Register a state machine; ignore duplicates.
    void AddSM(StateMachine* sm) {
        for (auto* existing : m_smList) {
            if (existing->SMID() == sm->SMID()) return;
        }
        m_smList.push_back(sm);
    }

    // Execute a single serialized payload.
    bool Execute(uint64_t instanceID, const std::string& paxosValue, void* ctx = nullptr) {
        return ExecuteImpl(m_groupIdx, instanceID, paxosValue, ctx, false);
    }

    // Execute for checkpoint (calls checkpoint version on the state machine).
    bool ExecuteForCheckpoint(uint64_t instanceID, const std::string& paxosValue) {
        return ExecuteImpl(m_groupIdx, instanceID, paxosValue, nullptr, true);
    }

    // Prepend a 4-byte SMID to the given body to form a full Paxos value.
    static void PackPaxosValue(std::string& body, int smid) {
        char prefix[4] = {0, 0, 0, 0};
        if (smid != 0) std::memcpy(prefix, &smid, sizeof(smid));
        body = std::string(prefix, 4) + body;
    }

    // Helper to run batch execution from a vector of (smid, body) pairs.
    // Returns the serialized batch value.
    static std::string BuildBatchValue(const std::vector<std::pair<int, std::string>>& batch) {
        std::string result;
        uint32_t count = static_cast<uint32_t>(batch.size());
        char countBuf[4];
        std::memcpy(countBuf, &count, sizeof(count));
        result.append(countBuf, 4);
        for (const auto& entry : batch) {
            char smidBuf[4];
            int smid = entry.first;
            std::memcpy(smidBuf, &smid, sizeof(smid));
            result.append(smidBuf, 4);
            uint32_t len = static_cast<uint32_t>(entry.second.size());
            char lenBuf[4];
            std::memcpy(lenBuf, &len, sizeof(len));
            result.append(lenBuf, 4);
            result.append(entry.second);
        }
        return result;
    }

    // Execute a batch serialized value.
    bool ExecuteBatch(uint64_t instanceID, const std::string& batchValue, void* ctx = nullptr) {
        const char* data = batchValue.data();
        size_t size = batchValue.size();
        if (size < 4) return false;
        uint32_t count;
        std::memcpy(&count, data, 4);
        size_t offset = 4;
        for (uint32_t i = 0; i < count; ++i) {
            if (offset + 8 > size) return false;
            int smid;
            uint32_t len;
            std::memcpy(&smid, data + offset, 4);
            offset += 4;
            std::memcpy(&len, data + offset, 4);
            offset += 4;
            if (offset + len > size) return false;
            std::string body(data + offset, len);
            offset += len;
            if (smid != 0) {
                if (!DoExecute(m_groupIdx, instanceID, body, smid, ctx, false)) return false;
            }
        }
        return true;
    }

private:
    bool ExecuteImpl(int groupIdx, uint64_t instanceID, const std::string& paxosValue, void* ctx, bool checkpoint) {
        if (paxosValue.size() < 4) return true; // malformed, skip
        int smid = 0;
        std::memcpy(&smid, paxosValue.data(), 4);
        if (smid == 0) return true; // no-op
        std::string body = paxosValue.substr(4);
        if (smid == 0x7FFFFFFF) { // Reserved batch ID (simulated)
            return ExecuteBatch(instanceID, body, ctx);
        }
        return DoExecute(groupIdx, instanceID, body, smid, ctx, checkpoint);
    }

    bool DoExecute(int groupIdx, uint64_t instanceID, const std::string& body, int smid, void* ctx, bool checkpoint) {
        if (m_smList.empty()) return false;
        for (auto* sm : m_smList) {
            if (sm->SMID() == smid) {
                if (checkpoint) return sm->ExecuteForCheckpoint(groupIdx, instanceID, body);
                else return sm->Execute(groupIdx, instanceID, body, ctx);
            }
        }
        return false;
    }

    int m_groupIdx;
    std::vector<StateMachine*> m_smList;
};

// Free helper to run batch directly from a vector of pairs.
// Returns true if all executions succeed.
bool RunBatch(SMFac& dispatcher, uint64_t instanceID, const std::vector<std::pair<int, std::string>>& batch, void* ctx = nullptr) {
    std::string batchValue = SMFac::BuildBatchValue(batch);
    // Simulate batch ID 0x7FFFFFFF being prepended.
    std::string fullValue;
    int batchId = 0x7FFFFFFF;
    char prefix[4];
    std::memcpy(prefix, &batchId, 4);
    fullValue.assign(prefix, 4);
    fullValue.append(batchValue);
    return dispatcher.Execute(instanceID, fullValue, ctx);
}

// The core algorithm involves parsing the leading 4 bytes of a payload to extract the SMID, then using a linear search over a vector of registered state machines to find a matching ID. For batch processing, we first parse a count and then loop over entries, each containing a 4-byte SMID and a 4-byte length plus bytes. The batch function must correctly handle malformed buffers by returning `false` if the buffer is too short for the expected fields. Edge cases include: payload size less than 4 bytes (treat as no-op and return `true`); SMID zero (skip with `true`); no state machines registered (return `false` for non-zero SMID); duplicate SMIDs during registration (ignore the later one). The `ExecuteForCheckpoint` variant mirrors normal execution but calls the checkpoint method on the target state machine. Time complexity is O(N) per single execution for lookup (N is number of registered state machines), and O(B * N) for batch where B is number of batch entries. Space complexity is O(N) for the vector plus O(1) extra for parsing. The `PackPaxosValue` simply concatenates the 4-byte SMID (with zero-padding if SMID is zero) to the body.
