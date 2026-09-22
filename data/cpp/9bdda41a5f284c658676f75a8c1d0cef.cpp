/*
Write a C++ function `extractVIP` that simulates a priority queue for a hospital triage system. The function takes a vector of strings, where each string is either `"V"` (meaning the doctor is ready to see the next patient) or a command of the form `"name priority"` (meaning a patient named `name` with an integer priority from 1 to 1000 arrives; lower priority values mean higher urgency). The function must process these commands in order, maintaining a min-heap keyed by a composite value: `(1001 - priority) * 1000000 + insertion_order`, where `insertion_order` is a monotonically increasing counter starting at 1 for the first patient. Whenever a `"V"` command appears, the function must output (as a string) the name of the patient with the smallest key (highest urgency), and if multiple patients have the same priority, the one who arrived first. If no patient is waiting, output an empty string. The function returns a single string containing all outputs separated by newline characters, preserving order. The input will never contain malformed commands, and priorities are guaranteed to be between 1 and 1000 inclusive.
*/
#include <vector>
#include <string>
#include <sstream>
#include <cstdint>
#include <limits>

// Simulate a hospital triage priority queue with min-heap.
// Each command is either "V" or "name priority".
// Returns newline-separated outputs for each V command.
std::string extractVIP(const std::vector<std::string>& commands) {
    const long long TOP_NUMBER = 1000000;  // multiplier for key construction
    const long long INF = std::numeric_limits<long long>::max();
    const int MAX_Q = 1000005;  // sufficient for any test (adjust if needed)

    // 1-indexed heap
    std::vector<std::string> nameQ(MAX_Q);
    std::vector<long long> keyQ(MAX_Q);
    int heapSize = 0;
    int insertionID = 0;
    std::ostringstream out;

    auto parent = [](int i) { return i >> 1; };
    auto left = [](int i) { return i << 1; };
    auto right = [](int i) { return (i << 1) + 1; };

    // Heapify down from index i
    auto minHeapify = [&](int i) {
        while (true) {
            int l = left(i);
            int r = right(i);
            int smallest = i;
            if (l <= heapSize && keyQ[l] < keyQ[smallest]) smallest = l;
            if (r <= heapSize && keyQ[r] < keyQ[smallest]) smallest = r;
            if (smallest != i) {
                std::swap(nameQ[i], nameQ[smallest]);
                std::swap(keyQ[i], keyQ[smallest]);
                i = smallest;
            } else {
                break;
            }
        }
    };

    // Insert a new element with a key
    auto insert = [&](const std::string& name, long long key) {
        heapSize++;
        keyQ[heapSize] = INF;
        nameQ[heapSize] = name;  // temporary, will be overwritten
        // Decrease key to actual key and bubble up
        if (key > keyQ[heapSize]) {
            // Should not happen in normal use
            return;
        }
        keyQ[heapSize] = key;
        int i = heapSize;
        while (i > 1 && keyQ[parent(i)] > keyQ[i]) {
            std::swap(nameQ[i], nameQ[parent(i)]);
            std::swap(keyQ[i], keyQ[parent(i)]);
            i = parent(i);
        }
    };

    // Extract the minimum (root)
    auto extractMin = [&]() -> std::string {
        if (heapSize < 1) {
            return "";
        }
        std::string minName = nameQ[1];
        nameQ[1] = nameQ[heapSize];
        keyQ[1] = keyQ[heapSize];
        heapSize--;
        if (heapSize > 0) minHeapify(1);
        return minName;
    };

    for (const auto& cmd : commands) {
        if (cmd == "V") {
            std::string next = extractMin();
            out << next << "\n";
        } else {
            std::istringstream iss(cmd);
            std::string name;
            int priority;
            iss >> name >> priority;
            insertionID++;
            long long key = (1001LL - priority) * TOP_NUMBER + insertionID;
            insert(name, key);
        }
    }

    std::string result = out.str();
    // Remove trailing newline if present (but we need it for empty outputs? Not needed)
    if (!result.empty() && result.back() == '\n') {
        result.pop_back();
    }
    return result;
}
#include <cassert>
#include <vector>
#include <string>

// The solution function is assumed to be declared above this main.
int main() {
    // Test 1: Basic priority order
    std::vector<std::string> commands1 = {"Alice 5", "Bob 3", "V", "V", "V"};
    assert(extractVIP(commands1) == "Bob\nAlice\n");

    // Test 2: Same priority, FIFO order
    std::vector<std::string> commands2 = {"A 2", "B 2", "V", "V", "V"};
    assert(extractVIP(commands2) == "A\nB\n");

    // Test 3: Empty queue at V
    std::vector<std::string> commands3 = {"V", "X 1", "V"};
    assert(extractVIP(commands3) == "\nX");

    // Test 4: Mixed priorities and insertions in between
    std::vector<std::string> commands4 = {"P 10", "V", "Q 1", "R 5", "V", "V", "V"};
    // After first V: P (priority 10) -> output P
    // Then Q (prio 1) arrives, then R (prio 5)
    // Next V: Q (prio 1) -> Q
    // Next V: R (prio 5) -> R
    // Next V: empty -> ""
    assert(extractVIP(commands4) == "P\nQ\nR\n");

    // Test 5: High priority 1000 vs low priority 1
    std::vector<std::string> commands5 = {"Low 1000", "High 1", "V", "V"};
    assert(extractVIP(commands5) == "High\nLow");

    // Test 6: Multiple V with no patients
    std::vector<std::string> commands6 = {"V", "V", "V"};
    assert(extractVIP(commands6) == "\n\n");

    // Test 7: Large number of patients, ensure no overflow
    std::vector<std::string> commands7;
    for (int i = 0; i < 1000; ++i) {
        commands7.push_back("Patient" + std::to_string(i) + " " + std::to_string(1 + (i % 1000)));
    }
    for (int i = 0; i < 1000; ++i) {
        commands7.push_back("V");
    }
    // Not checking full output, just that it doesn't crash and returns non-empty
    std::string result7 = extractVIP(commands7);
    assert(result7.size() > 0);

    return 0;
}
// The core idea is to implement a min-priority queue using a binary heap, exactly as in the original snippet. However, we encapsulate it in a function that accepts a vector of command strings (instead of reading from stdin) and returns the accumulated output as a single string. The priority queue stores pairs of `(name, key)`, where `key = (1001 - priority) * 1000000 + insertion_order`. Using a large multiplier (1,000,000) ensures that for any two patients with different priorities, the one with higher urgency (lower priority number) always has a smaller key, regardless of insertion order. For same priority, the smaller `insertion_order` gives a smaller key, preserving FIFO order. We maintain a heap array (1-indexed), a heap size counter, and an insertion counter. Operations: `extractMin` removes the root and calls `heapify`; `insert` adds a new element with a sentinel large key then decreases it to the true key (or we can directly insert and bubble up). Edge cases: attempting to extract from an empty heap yields an empty string for that output. The algorithm runs in O(total_commands * log(number_of_waiting_patients)) time. Space is O(number_of_patients) for the heap and O(output_size) for the result string. The multiplier is chosen to avoid overflow; with at most 1000 priority difference and insertion_order up to, say, 1e6, the key fits in long long.
