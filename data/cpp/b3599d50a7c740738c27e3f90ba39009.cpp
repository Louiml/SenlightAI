// Write a C++ function that processes a sequence of bus route descriptions. Each description begins with an integer `k` (the number of stops), followed by `k` distinct stop names (strings, no spaces within a name). The function must maintain a registry that maps each unique set of stops (in any order) to a unique bus number. Bus numbers are assigned sequentially starting from 1. For each new description, if the exact same set of stops has already been seen, print `"Already exists for <bus_number>"` where `<bus_number>` is the previously assigned number. Otherwise, assign the next available bus number, print `"New bus <bus_number>"`, and increment the counter. The function should take a reference to the registry map and a reference to the current next bus number, and read all input from `std::cin`. The input format is: first an integer `Q` (number of queries), then `Q` descriptions as described. The function should output results in the same order as the input queries. Assume valid input: `k >= 1`, stop names are non-empty, distinct within a description, and composed of letters/digits (no whitespace). The total number of distinct stop sets across all queries is at most 50,000, and the total number of stop names across all queries is at most 500,000.
The core idea is to use a `std::map` with keys as `std::set<std::string>` and values as integers. Each query reads `k`, then inserts all stop names into a temporary set. The map automatically treats sets with the same elements as equal regardless of insertion order, because `std::set` orders its elements. We then check if the set already exists in the map using `.count()`. If it does, output the stored bus number. If not, assign the current `n`, store it, output `"New bus "` with that number, and increment `n`. Edge cases include duplicate queries with the same stops (must output "Already exists" every time) and queries with different orders of stops (must be treated as the same because `set` normalizes order). The time complexity per query is `O(k log k)` for inserting into the set (each insertion is `O(log k)`) and `O(log m)` for the map lookup where `m` is the number of unique sets. Total time `O(Q * k_avg log k_avg + Q log m)`. Space complexity is `O(total number of unique stop names across all queries)` because each set stores only distinct names, and the map stores one copy of each unique set (though `set` may store duplicates across different sets, the total memory is bounded by total input names).
#include <iostream>
#include <map>
#include <set>
#include <string>

// Process bus route queries from stdin. Maintains a registry mapping each
// unique set of stops to a sequentially assigned bus number.
// mp: registry map (set of stops -> bus number)
// n: next available bus number (starts at 1)
void processBusQueries(std::map<std::set<std::string>, int>& mp, int& n) {
    int Q;
    std::cin >> Q;
    for (int i = 0; i < Q; ++i) {
        int k;
        std::cin >> k;
        std::set<std::string> stops;
        for (int j = 0; j < k; ++j) {
            std::string stop;
            std::cin >> stop;
            stops.insert(stop);
        }
        auto it = mp.find(stops);
        if (it != mp.end()) {
            std::cout << "Already exists for " << it->second << "\n";
        } else {
            mp[stops] = n;
            std::cout << "New bus " << n << "\n";
            ++n;
        }
    }
}
#include <cassert>
#include <sstream>
#include <iostream>
#include <map>
#include <set>
#include <string>

// Include the solution function declaration or implementation here.
// For testing, we re-implement the function inline (in practice it would be in a header).
void processBusQueries(std::map<std::set<std::string>, int>& mp, int& n) {
    int Q;
    std::cin >> Q;
    for (int i = 0; i < Q; ++i) {
        int k;
        std::cin >> k;
        std::set<std::string> stops;
        for (int j = 0; j < k; ++j) {
            std::string stop;
            std::cin >> stop;
            stops.insert(stop);
        }
        auto it = mp.find(stops);
        if (it != mp.end()) {
            std::cout << "Already exists for " << it->second << "\n";
        } else {
            mp[stops] = n;
            std::cout << "New bus " << n << "\n";
            ++n;
        }
    }
}

int main() {
    // Test 1: From provided example
    {
        std::string input = "5\n2 Marushkino Kokoshkino\n1 Kokoshkino\n2 Marushkino Kokoshkino\n2 Kokoshkino Marushkino\n2 Kokoshkino Kokoshkino\n";
        std::istringstream iss(input);
        std::streambuf* old_buf = std::cin.rdbuf(iss.rdbuf());
        std::ostringstream oss;
        std::streambuf* old_out = std::cout.rdbuf(oss.rdbuf());
        std::map<std::set<std::string>, int> mp;
        int n = 1;
        processBusQueries(mp, n);
        std::cin.rdbuf(old_buf);
        std::cout.rdbuf(old_out);
        assert(oss.str() == "New bus 1\nNew bus 2\nAlready exists for 1\nAlready exists for 1\nNew bus 3\n");
        assert(n == 4);
    }

    // Test 2: Empty set? Not allowed by spec (k>=1), but we can test duplicate with same order
    {
        std::string input = "3\n2 A B\n2 A B\n2 B A\n";
        std::istringstream iss(input);
        std::streambuf* old_buf = std::cin.rdbuf(iss.rdbuf());
        std::ostringstream oss;
        std::streambuf* old_out = std::cout.rdbuf(oss.rdbuf());
        std::map<std::set<std::string>, int> mp;
        int n = 1;
        processBusQueries(mp, n);
        std::cin.rdbuf(old_buf);
        std::cout.rdbuf(old_out);
        assert(oss.str() == "New bus 1\nAlready exists for 1\nAlready exists for 1\n");
        assert(n == 2);
    }

    // Test 3: Single stop repeated, and a different stop
    {
        std::string input = "3\n1 X\n1 Y\n1 X\n";
        std::istringstream iss(input);
        std::streambuf* old_buf = std::cin.rdbuf(iss.rdbuf());
        std::ostringstream oss;
        std::streambuf* old_out = std::cout.rdbuf(oss.rdbuf());
        std::map<std::set<std::string>, int> mp;
        int n = 1;
        processBusQueries(mp, n);
        std::cin.rdbuf(old_buf);
        std::cout.rdbuf(old_out);
        assert(oss.str() == "New bus 1\nNew bus 2\nAlready exists for 1\n");
        assert(n == 3);
    }

    // Test 4: Large set (10 stops) and duplicate
    {
        std::string input = "2\n3 a b c\n3 c b a\n";
        std::istringstream iss(input);
        std::streambuf* old_buf = std::cin.rdbuf(iss.rdbuf());
        std::ostringstream oss;
        std::streambuf* old_out = std::cout.rdbuf(oss.rdbuf());
        std::map<std::set<std::string>, int> mp;
        int n = 1;
        processBusQueries(mp, n);
        std::cin.rdbuf(old_buf);
        std::cout.rdbuf(old_out);
        assert(oss.str() == "New bus 1\nAlready exists for 1\n");
        assert(n == 2);
    }

    // Test 5: Two different sets with one common stop
    {
        std::string input = "2\n2 A B\n2 A C\n";
        std::istringstream iss(input);
        std::streambuf* old_buf = std::cin.rdbuf(iss.rdbuf());
        std::ostringstream oss;
        std::streambuf* old_out = std::cout.rdbuf(oss.rdbuf());
        std::map<std::set<std::string>, int> mp;
        int n = 1;
        processBusQueries(mp, n);
        std::cin.rdbuf(old_buf);
        std::cout.rdbuf(old_out);
        assert(oss.str() == "New bus 1\nNew bus 2\n");
        assert(n == 3);
    }

    return 0;
}
