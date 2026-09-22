Write a C++ function that simulates the core synchronization and initialization logic from the given LIO localization frontend workflow. The function should take two queues of timestamped sensor data (LiDAR cloud timestamps and GNSS pose timestamps) as input, along with a flag indicating whether localization has already been initialized. The function must process the data according to the following rules: if not initialized, it synchronizes the front elements of both queues—if the LiDAR timestamp is more than 0.05 seconds earlier than the GNSS timestamp, discard the LiDAR front element; if more than 0.05 seconds later, discard the GNSS front element; otherwise, both front elements are consumed and the function returns the synchronized pair and sets initialized to true. If already initialized, it simply consumes the front LiDAR element (clearing the GNSS queue) and returns that LiDAR element as synchronized without requiring a GNSS match. The function should return a structured result containing whether synchronization succeeded, the consumed LiDAR timestamp (if any), and whether initialization completed. Handle edge cases where queues may be empty at the start or become empty during processing.
// The solution simulates a data synchronization loop that repeatedly checks the front elements of two queues (cloud timestamps and GNSS timestamps) until a valid synchronized pair is found or a queue becomes empty. The algorithm mirrors the original `ValidData()` logic but encapsulated as a standalone function. The main steps: (1) If already initialized, pop the front of the cloud queue and return success with that timestamp, clearing the GNSS queue. (2) If not initialized and either queue is empty, return failure. (3) Otherwise, compare the absolute difference between the front timestamps. If the cloud timestamp is at least 0.05 seconds earlier (i.e., cloud_time - gnss_time < -0.05), discard cloud front and continue. If the cloud timestamp is at least 0.05 seconds later (diff > 0.05), discard GNSS front and continue. If within the 0.05-second window, both are consumed, and the function returns success with the cloud timestamp and marks initialized. The loop continues until either synchronization succeeds or a queue empties. Edge cases include empty queues at the start, queues that become empty after discarding elements, and timestamps exactly at the boundary (e.g., diff = -0.05 is considered valid since the condition is strictly less than -0.05). Time complexity is O(k) where k is the number of discarded elements until a match or exhaustion, and space is O(1) beyond the input queues.
#include <queue>
#include <utility>

// Result of synchronization attempt
struct SyncResult {
    bool success;        // true if a synchronized data point was obtained
    bool initialized;    // true if initialization completed (first sync)
    double timestamp;    // the LiDAR timestamp that was consumed (if success)
};

// Synchronize LiDAR and GNSS data queues according to the given logic.
// If already initialized, consume the front LiDAR timestamp and clear GNSS queue.
// Otherwise, discard mismatched timestamps until within 0.05 seconds or queues empty.
SyncResult synchronizeData(
    std::queue<double>& cloud_timestamps,
    std::queue<double>& gnss_timestamps,
    bool has_inited
) {
    // If already initialized, just take the next cloud timestamp
    if (has_inited) {
        if (cloud_timestamps.empty()) {
            return {false, true, 0.0};
        }
        double time = cloud_timestamps.front();
        cloud_timestamps.pop();
        while (!gnss_timestamps.empty()) {
            gnss_timestamps.pop();
        }
        return {true, true, time};
    }
    
    // Not initialized: synchronize until within 0.05s or a queue empties
    while (!cloud_timestamps.empty() && !gnss_timestamps.empty()) {
        double cloud_time = cloud_timestamps.front();
        double gnss_time = gnss_timestamps.front();
        double diff = cloud_time - gnss_time;
        
        if (diff < -0.05) {
            // Cloud is too early, discard it
            cloud_timestamps.pop();
        } else if (diff > 0.05) {
            // Cloud is too late, discard GNSS
            gnss_timestamps.pop();
        } else {
            // Within acceptable window, consume both and initialize
            cloud_timestamps.pop();
            gnss_timestamps.pop();
            return {true, true, cloud_time};
        }
    }
    
    // No valid synchronization possible
    return {false, false, 0.0};
}
#include <cassert>
#include <queue>

int main() {
    // Test 1: Already initialized, takes front cloud and clears GNSS
    {
        std::queue<double> clouds;
        std::queue<double> gnss;
        clouds.push(1.0);
        clouds.push(2.0);
        gnss.push(100.0);
        auto result = synchronizeData(clouds, gnss, true);
        assert(result.success == true);
        assert(result.initialized == true);
        assert(result.timestamp == 1.0);
        assert(clouds.size() == 1);
        assert(gnss.empty());
    }
    
    // Test 2: Not initialized, perfect match within window
    {
        std::queue<double> clouds;
        std::queue<double> gnss;
        clouds.push(10.0);
        gnss.push(10.0);
        auto result = synchronizeData(clouds, gnss, false);
        assert(result.success == true);
        assert(result.initialized == true);
        assert(result.timestamp == 10.0);
        assert(clouds.empty());
        assert(gnss.empty());
    }
    
    // Test 3: Not initialized, cloud too early, discard cloud then match
    {
        std::queue<double> clouds;
        std::queue<double> gnss;
        clouds.push(1.0);
        clouds.push(10.0);
        gnss.push(10.0);
        auto result = synchronizeData(clouds, gnss, false);
        assert(result.success == true);
        assert(result.initialized == true);
        assert(result.timestamp == 10.0);
        assert(clouds.empty());
        assert(gnss.empty());
    }
    
    // Test 4: Not initialized, cloud too late, discard GNSS then match
    {
        std::queue<double> clouds;
        std::queue<double> gnss;
        clouds.push(10.0);
        gnss.push(1.0);
        gnss.push(10.0);
        auto result = synchronizeData(clouds, gnss, false);
        assert(result.success == true);
        assert(result.initialized == true);
        assert(result.timestamp == 10.0);
        assert(clouds.empty());
        assert(gnss.empty());
    }
    
    // Test 5: Not initialized, all clouds too early, queue empties
    {
        std::queue<double> clouds;
        std::queue<double> gnss;
        clouds.push(1.0);
        clouds.push(2.0);
        gnss.push(100.0);
        auto result = synchronizeData(clouds, gnss, false);
        assert(result.success == false);
        assert(result.initialized == false);
        assert(clouds.empty());
        assert(!gnss.empty());
    }
    
    // Test 6: Not initialized, empty cloud queue
    {
        std::queue<double> clouds;
        std::queue<double> gnss;
        gnss.push(5.0);
        auto result = synchronizeData(clouds, gnss, false);
        assert(result.success == false);
        assert(result.initialized == false);
    }
    
    // Test 7: Boundary condition, diff exactly -0.05 is valid
    {
        std::queue<double> clouds;
        std::queue<double> gnss;
        clouds.push(10.0);
        gnss.push(10.05);
        auto result = synchronizeData(clouds, gnss, false);
        assert(result.success == true);
        assert(result.timestamp == 10.0);
    }
    
    // Test 8: Boundary condition, diff exactly 0.05 is valid
    {
        std::queue<double> clouds;
        std::queue<double> gnss;
        clouds.push(10.05);
        gnss.push(10.0);
        auto result = synchronizeData(clouds, gnss, false);
        assert(result.success == true);
        assert(result.timestamp == 10.05);
    }
    
    return 0;
}
