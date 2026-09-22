// You are designing a rescue algorithm for a river crossing scenario. A group of up to 6 people, consisting of archaeologists and cannibals, must cross a river using a boat that can carry at most 2 people at a time. Each person has an individual crossing time (in minutes). When two people cross together, the boat takes the maximum of their two times. When one person crosses alone, it takes that person's time. At least one person must be in the boat for it to cross, and the boat must always contain at least one of the group members. The goal is to find the minimum total time needed to get all people from the starting side to the destination side. Write a C++ function `int minimumCrossingTime(const std::vector<int>& crossingTimes)` that takes a vector of individual crossing times for all group members (size between 1 and 6 inclusive, times are positive integers) and returns the minimum total crossing time. The members are interchangeable except for their times; there is no distinction between archaeologists and cannibals in this simplified problem. The initial state has all members on the starting side and the boat there; the final state has all members on the destination side and the boat there. You must account for the boat's position and that at least one person must be on the starting side to bring the boat back (except for the final crossing). The function should handle any input size from 1 to 6 and any positive integer times.
// The problem is a classic state-space search. Model each state as a bitmask of which members are on the destination side (0 to 2^n -1, where n ≤ 6), plus a boolean indicating on which side the boat is (0 = starting, 1 = destination). The initial state is (mask=0, boat=0), and the goal is (mask=all bits set, boat=1). From each state, generate all possible moves: either 1 or 2 people cross from the side where the boat currently is to the opposite side. For a crossing, pick the set of people (one or two) from the current side, compute the crossing time as the maximum of their times, and update the mask (flip those bits) and the boat side. Since n ≤ 6, the total states are at most 2^7 = 128 (64 masks × 2 boat positions), so a simple breadth-first search (BFS) over states with Dijkstra-like priority queue (or since times are positive, use a min-heap) works. However, because the graph is small and times are positive integers, we can use Dijkstra's algorithm: priority queue of (total_time, mask, boat), starting from (0,0,0). For each state, for each valid crossing of 1 or 2 people from the current boat side, compute the new mask and new boat side, and relax if the new time is smaller. The answer is the minimum total time to reach state (all bits set, boat=1). Edge cases: n=1: only one person, they cross alone once; answer is their time. n=0 is not allowed by the problem (size between 1 and 6). If the boat is on the starting side and there are no people on that side (mask = all bits), that state is invalid (can't move), but that state would only be the goal if boat=1. If boat is on destination side and mask=0, that state is invalid (nobody on starting side to bring boat back), but such states will never be reached because we only move from valid states. Since n ≤ 6, the number of possible moves per state is at most C(6,1)+C(6,2)=6+15=21, but actually fewer because we can only choose from people on the current boat side. The time complexity is O(2^n * n^2) because for each of ~2^n states, we generate up to O(n^2) transitions, and each transition is O(1) except for the mask operations. Space complexity is O(2^n) for the distance array or O(2^n) for the visited/pq. For n=6, this is trivial.
#include <vector>
#include <queue>
#include <climits>
#include <algorithm>

// Returns the minimum total time to move all members from the starting side
// to the destination side using a boat that carries at most 2 people.
// Each person's crossing time is given in crossingTimes (positive integers).
// n = crossingTimes.size() is between 1 and 6.
int minimumCrossingTime(const std::vector<int>& crossingTimes) {
    int n = crossingTimes.size();
    int fullMask = (1 << n) - 1; // all bits set
    const int INF = INT_MAX;

    // dist[mask][boatSide] where boatSide 0 = starting, 1 = destination
    std::vector<std::vector<int>> dist(1 << n, std::vector<int>(2, INF));
    dist[0][0] = 0;

    // Min-heap: (total_time, mask, boatSide)
    using State = std::tuple<int, int, int>;
    std::priority_queue<State, std::vector<State>, std::greater<State>> pq;
    pq.push({0, 0, 0});

    while (!pq.empty()) {
        auto [time, mask, boat] = pq.top();
        pq.pop();

        if (time != dist[mask][boat]) continue; // stale entry

        // Reached goal?
        if (mask == fullMask && boat == 1) return time;

        // Determine which people are on the side where the boat currently is.
        // If boat == 0 (starting side): people still on starting side are those NOT in mask.
        // If boat == 1 (destination side): people on destination side are those in mask.
        std::vector<int> candidates;
        for (int i = 0; i < n; ++i) {
            if (boat == 0 && !(mask & (1 << i))) {
                candidates.push_back(i);
            } else if (boat == 1 && (mask & (1 << i))) {
                candidates.push_back(i);
            }
        }

        int m = candidates.size();

        // Try all possible crossings of 1 person
        for (int idx = 0; idx < m; ++idx) {
            int person = candidates[idx];
            int travelTime = crossingTimes[person];
            int newMask = mask;
            int newBoat = 1 - boat;
            if (boat == 0) {
                newMask |= (1 << person); // move from starting to destination
            } else {
                newMask &= ~(1 << person); // move from destination to starting
            }
            int newTime = time + travelTime;
            if (newTime < dist[newMask][newBoat]) {
                dist[newMask][newBoat] = newTime;
                pq.push({newTime, newMask, newBoat});
            }
        }

        // Try all possible crossings of 2 people
        for (int a = 0; a < m; ++a) {
            for (int b = a + 1; b < m; ++b) {
                int p1 = candidates[a];
                int p2 = candidates[b];
                int travelTime = std::max(crossingTimes[p1], crossingTimes[p2]);
                int newMask = mask;
                if (boat == 0) {
                    newMask |= (1 << p1);
                    newMask |= (1 << p2);
                } else {
                    newMask &= ~(1 << p1);
                    newMask &= ~(1 << p2);
                }
                int newBoat = 1 - boat;
                int newTime = time + travelTime;
                if (newTime < dist[newMask][newBoat]) {
                    dist[newMask][newBoat] = newTime;
                    pq.push({newTime, newMask, newBoat});
                }
            }
        }
    }

    // Should never reach here because a solution always exists.
    return -1;
}
#include <cassert>
#include <vector>

int main() {
    // Single person: must cross once alone.
    assert(minimumCrossingTime({5}) == 5);

    // Two people: both cross together, time = max.
    assert(minimumCrossingTime({1, 2}) == 2);

    // Three classic: times 1,2,5 -> optimal 14 (1&2 go, 1 returns, 1&5 go)
    assert(minimumCrossingTime({1, 2, 5}) == 8); // Actually optimal is: 1&2 cross (2), 1 returns (1), 1&5 cross (5) = 8? Wait, that's 8, but alternative: 1&5 cross (5), 1 returns (1), 1&2 cross (2) = 8 too. So 8.
    assert(minimumCrossingTime({1, 2, 5}) == 8);

    // Four people: times 1,2,5,10 -> optimal 17 (standard bridge torch problem)
    assert(minimumCrossingTime({1, 2, 5, 10}) == 17);

    // Larger but with equal times: total = (n-1)*t? No, need more careful: for 3 with all 3 same, optimal is 3*3 =9? Let's compute: 3 people all time 3: 1&2 cross (3), 1 returns (3), 1&3 cross (3) = 9. 
    assert(minimumCrossingTime({3, 3, 3}) == 9);

    // Six people all time 1: Optimal: send two (1), one returns (1), send two (1), one returns (1), ... For 6, pattern: 2 cross, 1 returns, 2 cross, 1 returns, 2 cross. That's 3 crossings forward (max) and 2 returns = 1*3 + 1*2 = 5. But is there better? Since all equal, any sequence of moving 6 people with boat capacity 2 requires at least 5 crossings (4 forward? Actually total trips: each forward can carry at most 2, so need at least 3 forward trips. Each forward trip after the first needs a return trip. So total trips = 2*3 -1 = 5. Minimum time = 5*1=5.
    assert(minimumCrossingTime({1, 1, 1, 1, 1, 1}) == 5);

    // Six with one very slow: times {1,1,1,1,1,10}. Optimal: bring fast ones over first? For n=6, typical strategy: use two fastest as shuttles. For times: 1,1,1,1,1,10. Send two slowest? Actually all except 10 are fast. Let's compute: possible optimal: Send (1,10) cross (10), 1 returns (1), send (1,1) cross (1), 1 returns (1), send (1,1) cross (1), 1 returns (1), send (1,1) cross (1) => total 10+1+1+1+1+1+1 = 15? But we have 6 people, need 3 forward trips carrying 2 each, but we have 5 fast ones. Actually better: Use two fastest as shuttles: Let fast1=1, fast2=1. Send slow (10) with fast1 (10), fast1 returns (1), send fast1 with another 1 (1), fast1 returns (1), send fast1 with another 1 (1), fast1 returns (1), send fast1 with last 1 (1) -> total 10+1+1+1+1+1+1=15? That's 7 trips. But we have 6 people: Actually total trips needed at least 2*ceil(6/2)-1 = 2*3-1=5. With fast1 and slow: 1&10 cross (10), 1 returns (1), 1&1 cross (1), 1 returns (1), 1&1 cross (1) -> only 5 people moved? Let's count: 1&10 cross (2 on dest), 1 returns (1 back, so 1 on dest), 1&1 cross (now 3 on dest), 1 returns (2 back, so 2 on dest), 1&1 cross (now 4 on dest). Wait we have 6 people total: two are 10? Actually only one 10. So we have 5 ones and one 10. Final state: all 6 on dest. The sequence: (1,10) go, 1 back, (1,1) go, 1 back, (1,1) go, 1 back, (1,1) go? That would be 4 forward trips? Let's not manually compute; just trust the algorithm. We'll test with a known result: For n=3 with times 1,2,5 we know 8. For n=4 with 1,2,5,10 we know 17. For the six with five 1s and one 10, we can compute: The optimal is 15? Let's quickly reason: Use the fast two (1 and 1) as shuttles. First move the slowest (10) with one fast (1): cross time 10, fast returns (1), now we have 1 person on dest (the other fast? Actually we moved 10 and fast1 to dest, fast1 returns, so 10 is on dest, fast1 back. Then we move fast1 and fast2 to dest (1), fast1 returns (1), then fast1 and fast3 to dest (1), fast1 returns (1), then fast1 and fast4 to dest (1) -> total time = 10+1+1+1+1+1+1? That's 6? Let's count trips: 1) 10+fast1 go (10), fast1 back (1) -> 2 trips, 1 person on dest (10). 2) fast1+fast2 go (1), fast1 back (1) -> now 2 on dest (10 and fast2). 3) fast1+fast3 go (1), fast1 back (1) -> 3 on dest. 4) fast1+fast4 go (1) -> 4 on dest, all done? We have 5 fast ones + 1 slow = 6 total. After step 4, we have moved fast1, fast2, fast3, fast4 and slow to dest? Actually slow is already there, fast1, fast2, fast3, fast4 are there? Let's count: Start: all on start. Step 1: go [10,F1] (dest: 10,F1), return [F1] (dest:10, start:F1,F2,F3,F4,F5). Step 2: go [F1,F2] (dest:10,F1,F2), return [F1] (dest:10,F2, start:F1,F3,F4,F5). Step 3: go [F1,F3] (dest:10,F2,F1,F3), return [F1] (dest:10,F2,F3, start:F1,F4,F5). Step 4: go [F1,F4] (dest:10,F2,F3,F1,F4), return? No, now we have 5 on dest? We have 10,F2,F3,F1,F4 = 5 people on dest, and F5 still on start. Need to bring F5. Go [F1,F5]? But F1 is on dest, not start. So actually we need to have a fast on start to bring back. The pattern is: use F1 as shuttle. Let's write proper sequence for 6 people with times: [1,1,1,1,1,10]. The known optimal for crossing with boat capacity 2 is: 
Trip 1: F1 (1) and F2 (1) cross -> time 1
Trip 2: F1 returns -> time 1
Trip 3: F1 and F3 cross -> time 1
Trip 4: F1 returns -> time 1
Trip 5: F1 and F4 cross -> time 1
Trip 6: F1 returns -> time 1
Trip 7: F1 and F5 cross -> time 1
Trip 8: F1 returns? No, we have 6 people: F1,F2,F3,F4,F5,Slow. That's 6. Trip 1 moves F1,F2. Trip 2 brings F1 back. Trip 3 moves F1,F3. Trip 4 brings F1 back. Trip 5 moves F1,F4. Trip 6 brings F1 back. Trip 7 moves F1,F5. Now we have F2,F3,F4,F5 on dest, F1 and Slow on start? Wait, we never moved Slow. We need to move Slow too. The optimal is to pair Slow with someone early. Actually the standard algorithm for n people with boat capacity 2: Use two fastest as shuttles. The optimal for six with one very slow is: 
1) Fastest and slowest cross (time = slow) -> dest: slow, fastest
2) Fastest returns (time = fastest) -> dest: slow, start: fastest, others
3) Two fastest cross (time = fastest) -> dest: slow, fastest1, fastest2
4) Fastest returns (time = fastest) -> dest: slow, fastest2, start: fastest1, others
5) Two fastest cross (time = fastest) -> dest: slow, fastest2, fastest1, fastest3
6) Fastest returns (time = fastest) -> dest: slow, fastest2, fastest3, start: fastest1, fastest4, fastest5
7) Two fastest cross (time = fastest) -> dest: all? Let's count: After step 7, we have moved fastest1+fastest4? Actually we need to end with all 6. This is getting complicated. For the test, I'll just use a known small case and verify with an alternative method. Since the solution is correct by algorithm, I'll test with n=3 and n=4 where I know results. For n=6 with equal 1s, I know 5. For n=6 with one 100 and five 1s, let me compute via manual reasoning: The optimal is likely 100 + 4*1 + 5*1? Actually consider pattern: Use smallest (1) and second smallest (1) as shuttles. Send the slowest (100) with the smallest (1) first? That would be 100, then smallest returns (1), then send two of the remaining fast (1) cross (1), smallest returns (1), send two more fast cross (1), smallest returns (1), send last fast with the other fast? We have 5 fast + 1 slow = 6. Trip1: (slow, fast1) cross -> 100, fast1 returns ->1 (dest: slow)
Trip2: (fast1, fast2) cross ->1, fast1 returns ->1 (dest: slow, fast2)
Trip3: (fast1, fast3) cross ->1, fast1 returns ->1 (dest: slow, fast2, fast3)
Trip4: (fast1, fast4) cross ->1, fast1 returns ->1 (dest: slow, fast2, fast3, fast4)
Trip5: (fast1, fast5) cross ->1 (dest: all) => total = 100+1+1+1+1+1+1+1+1? That's 9 trips? Count: Trip1 go, Trip1 return, Trip2 go, Trip2 return, Trip3 go, Trip3 return, Trip4 go, Trip4 return, Trip5 go = 9 crossings. Time = 100+1+1+1+1+1+1+1+1? Actually Trip1 go=100, Trip1 return=1, Trip2 go=1, Trip2 return=1, Trip3 go=1, Trip3 return=1, Trip4 go=1, Trip4 return=1, Trip5 go=1 => total = 100+8 = 108. But there's a better way: Send two slow? But we only have one slow. Actually we can send the slow with a fast, then bring the fast back, then send two fast together, etc. The alternative: Use two fastest as shuttles: 
1) F1 & F2 cross (1), F1 returns (1) -> dest: F2
2) Slow & F3 cross (100), F2 returns (1) -> dest: slow, F3, F2? Actually F2 is on dest, bring F2 back? Let's think: Better pattern: 
1) F1 & F2 cross (1), F1 returns (1) -> dest: F2
2) Slow & F3 cross (100), F2 returns (1) -> dest: slow, F3, F2? No, after step 1 dest has F2, start has F1,F3,F4,F5,Slow. Step 2: send Slow and F3 (100), now dest has F2,Slow,F3, start has F1,F4,F5. Then F2 returns (1) -> dest has Slow,F3, start has F1,F2,F4,F5. Step 3: F1 & F4 cross (1), F1 returns (1) -> dest: Slow,F3,F4, start: F1,F2,F5. Step 4: F1 & F5 cross (1) -> dest all? Now dest: Slow,F3,F4,F1,F5? Wait F2 is on start, we need to bring F2. So step 4: F1 & F5 cross (1) -> dest: Slow,F3,F4,F1,F5, start: F2. Then we need to bring F2, but F1 is on dest. So send F1 back? Actually after step 4, F1 is on dest, we need to send someone back to get F2. So step 5: F1 returns (1), step 6: F1 & F2 cross (1). Total trips: 1,2,3,4,5,6 = 6 forward? Actually count sequences: 
1: F1+F2 -> (1)
2: F1 <- (1)
3: Slow+F3 -> (100)
4: F2 <- (1) (F2 from dest to start)
5: F1+F4 -> (1)
6: F1 <- (1)
7: F1+F5 -> (1)
8: F1 <- (1)
9: F1+F2 -> (1)
That's 9 trips again. Time = 1+1+100+1+1+1+1+1+1 = 108. So likely optimal is 108. I'll test that: assert(minimumCrossingTime({1,1,1,1,1,100}) == 108). 

    assert(minimumCrossingTime({1,1,1,1,1,100}) == 108);

    // A case with two slow: {1,1,1,50,50} n=5. Known? Let's not guess; just trust algorithm and test a simple property: result must be positive and <= sum of all times * something. But we'll test with known small ones.

    return 0;
}
