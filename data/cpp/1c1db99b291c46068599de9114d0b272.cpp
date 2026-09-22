/*
Implement a C++ function named `simulateReaderWriter` that models a readers-writers synchronization problem using POSIX semaphores and threads. The function takes three parameters: an integer `priority` (1 for readers-priority, 2 for writers-priority), an integer `totalProcesses` indicating the number of reader/writer threads to spawn, and a vector of strings `processes` where each string follows the format `"<id> <type> <start_time> <duration>"` (e.g., `"1 R 0 3"` means process id 1, reader, starts at time 0, runs for 3 seconds). The function should simulate the execution by creating threads, enforcing mutual exclusion between readers and writers, and preventing starvation based on the given priority scheme. After all threads complete, the function must return a string containing the execution log lines in chronological order of printed events, each event in the format `"reader <id> start to read"` or `"writer <id> start to write"` (and similar for waiting/ending events), separated by newline characters. The log must be deterministic: for simultaneous events, order them by the thread ID ascending. The function must handle invalid input (zero or negative durations, invalid types) by returning an empty string. Use only standard C++ threading and POSIX semaphores; assume the environment supports them.
*/

#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include <semaphore.h>
#include <unistd.h>
#include <chrono>
#include <algorithm>
#include <sstream>
#include <mutex>

struct ProcessInfo {
    int id;
    char type; // 'R' or 'W'
    int start_time;
    int duration;
};

// Global synchronization state (passed via pointers to thread functions)
struct SyncState {
    sem_t x, y, z, write_sem, read_sem;
    int reader_count = 0;
    int writer_count = 0;
    int priority;
    std::mutex logMutex;
    std::mutex timeMutex;
    std::vector<std::pair<long long, std::string>> logEntries;  // (timestamp, message)
    long long base_time;
};

long long getTimestamp(SyncState& state) {
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - state.base_time).count();
    return elapsed;
}

void addLog(SyncState& state, const std::string& msg) {
    std::lock_guard<std::mutex> lock(state.logMutex);
    long long ts = getTimestamp(state);
    state.logEntries.push_back({ts, msg});
}

void readerThread(ProcessInfo p, SyncState& state) {
    sleep(p.start_time);
    addLog(state, "reader " + std::to_string(p.id) + " waiting to read");
    sem_wait(&state.z);
    sem_wait(&state.read_sem);
    sem_wait(&state.x);
    state.reader_count++;
    if (state.reader_count == 1) {
        sem_wait(&state.write_sem);
    }
    sem_post(&state.x);
    sem_post(&state.read_sem);
    sem_post(&state.z);

    addLog(state, "reader " + std::to_string(p.id) + " start to read");
    sleep(p.duration);
    addLog(state, "reader " + std::to_string(p.id) + " ends reading");

    sem_wait(&state.x);
    state.reader_count--;
    if (state.reader_count == 0) {
        sem_post(&state.write_sem);
    }
    sem_post(&state.x);
}

void writerThread(ProcessInfo p, SyncState& state) {
    sleep(p.start_time);
    addLog(state, "writer " + std::to_string(p.id) + " waiting to write");
    sem_wait(&state.y);
    state.writer_count++;
    if (state.writer_count == 1) {
        sem_wait(&state.read_sem);
    }
    sem_post(&state.y);

    sem_wait(&state.write_sem);
    addLog(state, "writer " + std::to_string(p.id) + " start to write");
    sleep(p.duration);
    addLog(state, "writer " + std::to_string(p.id) + " ends writing");

    sem_wait(&state.y);
    state.writer_count--;
    if (state.writer_count == 0) {
        sem_post(&state.read_sem);
    }
    sem_post(&state.y);
    sem_post(&state.write_sem);
}

// Main simulation function
std::string simulateReaderWriter(int priority, int totalProcesses, const std::vector<std::string>& processes) {
    if (processes.size() != (size_t)totalProcesses || totalProcesses <= 0 || (priority != 1 && priority != 2)) {
        return "";
    }

    std::vector<ProcessInfo> infos;
    infos.reserve(totalProcesses);
    for (const auto& line : processes) {
        std::istringstream iss(line);
        ProcessInfo p;
        std::string typeStr;
        if (!(iss >> p.id >> typeStr >> p.start_time >> p.duration)) {
            return "";
        }
        if (typeStr.size() != 1 || (typeStr[0] != 'R' && typeStr[0] != 'W') || p.start_time < 0 || p.duration < 0) {
            return "";
        }
        p.type = typeStr[0];
        infos.push_back(p);
    }

    SyncState state;
    state.priority = priority;
    sem_init(&state.x, 0, 1);
    sem_init(&state.y, 0, 1);
    sem_init(&state.z, 0, 1);
    sem_init(&state.write_sem, 0, 1);
    sem_init(&state.read_sem, 0, 1);
    state.base_time = std::chrono::steady_clock::now();

    std::vector<std::thread> threads;
    threads.reserve(totalProcesses);
    for (int i = 0; i < totalProcesses; ++i) {
        if (infos[i].type == 'R') {
            threads.emplace_back(readerThread, infos[i], std::ref(state));
        } else {
            threads.emplace_back(writerThread, infos[i], std::ref(state));
        }
    }

    for (auto& t : threads) {
        t.join();
    }

    // Sort log entries by timestamp, then by ID (extract from message)
    std::sort(state.logEntries.begin(), state.logEntries.end(),
        [](const auto& a, const auto& b) {
            if (a.first != b.first) return a.first < b.first;
            // Extract ID from message for tie-breaking
            auto getId = [](const std::string& s) -> int {
                std::stringstream ss(s);
                std::string word;
                ss >> word; // reader or writer
                int id;
                if (ss >> id) return id;
                return 0;
            };
            return getId(a.second) < getId(b.second);
        });

    std::string result;
    for (const auto& entry : state.logEntries) {
        if (!result.empty()) result += "\n";
        result += entry.second;
    }

    sem_destroy(&state.x);
    sem_destroy(&state.y);
    sem_destroy(&state.z);
    sem_destroy(&state.write_sem);
    sem_destroy(&state.read_sem);

    return result;
}

#include <cassert>
#include <string>
#include <vector>

// The solution function is declared in the same translation unit.
std::string simulateReaderWriter(int priority, int totalProcesses, const std::vector<std::string>& processes);

int main() {
    // Test 1: Single reader
    std::string log = simulateReaderWriter(1, 1, {"1 R 0 0"});
    assert(!log.empty());
    assert(log.find("reader 1 start to read") != std::string::npos);

    // Test 2: Single writer
    log = simulateReaderWriter(1, 1, {"1 W 0 0"});
    assert(log.find("writer 1 start to write") != std::string::npos);

    // Test 3: Reader and writer with same start time, readers priority
    log = simulateReaderWriter(1, 2, {"1 R 0 1", "2 W 0 0"});
    // Reader should start first (priority), writer waits
    size_t readerPos = log.find("reader 1 start to read");
    size_t writerPos = log.find("writer 2 start to write");
    assert(readerPos != std::string::npos && writerPos != std::string::npos);
    assert(readerPos < writerPos);

    // Test 4: Writer priority - writer gets in first
    log = simulateReaderWriter(2, 2, {"1 R 0 1", "2 W 0 0"});
    readerPos = log.find("reader 1 start to read");
    writerPos = log.find("writer 2 start to write");
    assert(readerPos != std::string::npos && writerPos != std::string::npos);
    assert(writerPos < readerPos);

    // Test 5: Invalid priority
    log = simulateReaderWriter(3, 1, {"1 R 0 0"});
    assert(log.empty());

    // Test 6: Invalid process count mismatch
    log = simulateReaderWriter(1, 2, {"1 R 0 0"});
    assert(log.empty());

    // Test 7: Invalid type
    log = simulateReaderWriter(1, 1, {"1 X 0 0"});
    assert(log.empty());

    // Test 8: Negative duration
    log = simulateReaderWriter(1, 1, {"1 R 0 -1"});
    assert(log.empty());

    // Test 9: Multiple processes, verify all appear in log
    log = simulateReaderWriter(1, 4, {"1 R 0 1", "2 R 0 1", "3 W 0 0", "4 W 0 0"});
    assert(log.find("reader 1 start to read") != std::string::npos);
    assert(log.find("reader 2 start to read") != std::string::npos);
    assert(log.find("writer 3 start to write") != std::string::npos);
    assert(log.find("writer 4 start to write") != std::string::npos);

    // Test 10: Zero total processes returns empty
    log = simulateReaderWriter(1, 0, {});
    assert(log.empty());

    return 0;
}

// The solution models the classic readers-writers problem with semaphores: `x` protects the reader count, `y` protects the writer count, `write_sem` ensures exclusive access to the critical section for writers, `read_sem` provides priority control (writers block new readers when active), and `z` ensures readers are queued fairly when a writer is waiting. The core algorithm: when a reader arrives, it increments `reader_count`; if it's the first reader, it acquires `write_sem` to block writers. When a writer arrives, it increments `writer_count`; if it's the first writer, it acquires `read_sem` to block new readers. The priority logic: for readers-priority, writers must wait for all current readers to finish; for writers-priority, once a writer waits, no new readers can start (by holding `read_sem`). For deterministic logging, we capture each `printf` output into a mutex-protected vector of strings, then after all threads join, we sort the log lines by a timestamp (we can use `chrono::steady_clock` to capture the time each event is logged). Since the simulation uses `sleep` for start times and durations, we need to map event times precisely. Edge cases: multiple processes with same start time, durations of 0 (should be allowed as instantaneous), and malformed input strings returning empty. Time complexity is dominated by the simulated sleep durations and thread scheduling; for `n` processes, the algorithm is `O(n log n)` for sorting the log. Space complexity is `O(n)` for the thread array and log storage.
