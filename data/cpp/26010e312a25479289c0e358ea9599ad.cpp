/*
Design and implement a C++ class `DynamicRanking` that maintains a collection of (node ID, value) pairs and supports the following operations: insert or update a node's value (if the node already exists, its value is replaced), erase a node by ID, query whether a node exists, retrieve a node's current value (returning a specified default if absent), return the node with the smallest value (and the largest value), and print all node-value pairs in order of node ID. The class must be based on the design pattern shown in the provided snippet: store a `std::map<int, std::multiset<std::pair<double,int>>::iterator>` for O(1)-amortized lookup of a node's iterator in a `std::multiset<std::pair<double,int>>` that keeps values sorted by (value, node ID) for efficient min/max retrieval. The task requires implementing a free function `processRankingQueries` that reads a sequence of commands from standard input (each command is one of: `INSERT id value`, `ERASE id`, `QUERY id`, `MIN`, `MAX`, `PRINT`, `EXIT`), performs the operations using your class, and outputs the results: for `QUERY` print the value or `-1` if absent; for `MIN` and `MAX` print the node ID or `-1` if empty; for `PRINT` print each node ID and value separated by a tab, one per line. Commands are case-sensitive. The function should output nothing for `INSERT`, `ERASE`, and `EXIT`. The input may contain any integers for IDs and double values, including negatives, and there may be multiple `INSERT` commands for the same ID (the latest value replaces the old). The function must be robust to commands that attempt to erase or query non-existent nodes (ignore or return default as specified). The input ends with `EXIT`; no output is expected after that. The function signature is `void processRankingQueries(std::istream& in, std::ostream& out)`. Your implementation must provide the class and the free function, but no `main` function.
*/
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <sstream>

class DynamicRanking {
public:
    DynamicRanking() = default;
    ~DynamicRanking() = default;

    // Returns true if node 'id' is present.
    bool contains(int id) const {
        return data.find(id) != data.end();
    }

    // Inserts or updates the value for node 'id'.
    void insertValue(int id, double value) {
        erase(id);
        auto it = sortedPairs.insert({value, id});
        data[id] = it;
    }

    // Removes node 'id' if present; returns true if removed.
    bool erase(int id) {
        auto it = data.find(id);
        if (it == data.end()) return false;
        sortedPairs.erase(it->second);
        data.erase(it);
        return true;
    }

    // Returns the value for node 'id', or 'defaultValue' if absent.
    double getValue(int id, double defaultValue = -1.0) const {
        auto it = data.find(id);
        if (it == data.end()) return defaultValue;
        return it->second->first;
    }

    // Returns the node with smallest value; returns -1 if empty.
    int getMinNode() const {
        if (sortedPairs.empty()) return -1;
        return sortedPairs.begin()->second;
    }

    // Returns the node with largest value; returns -1 if empty.
    int getMaxNode() const {
        if (sortedPairs.empty()) return -1;
        auto it = sortedPairs.end();
        --it;
        return it->second;
    }

    // Prints all (id, value) pairs sorted by id, one per line.
    void printNodes(std::ostream& out) const {
        for (const auto& entry : data) {
            out << entry.first << "\t" << entry.second->first << "\n";
        }
    }

    int size() const { return data.size(); }

private:
    // Maps node id to its iterator in the multiset.
    std::map<int, std::multiset<std::pair<double, int>>::iterator> data;
    // Sorted by (value, id) for min/max.
    std::multiset<std::pair<double, int>> sortedPairs;
};

// Processes a sequence of commands from input and writes results to output.
void processRankingQueries(std::istream& in, std::ostream& out) {
    DynamicRanking ranking;
    std::string command;
    while (in >> command) {
        if (command == "INSERT") {
            int id;
            double value;
            in >> id >> value;
            ranking.insertValue(id, value);
        } else if (command == "ERASE") {
            int id;
            in >> id;
            ranking.erase(id);
        } else if (command == "QUERY") {
            int id;
            in >> id;
            out << ranking.getValue(id) << "\n";
        } else if (command == "MIN") {
            out << ranking.getMinNode() << "\n";
        } else if (command == "MAX") {
            out << ranking.getMaxNode() << "\n";
        } else if (command == "PRINT") {
            ranking.printNodes(out);
        } else if (command == "EXIT") {
            break;
        }
    }
}
#include <sstream>
#include <cassert>

void testDynamicRanking() {
    std::string input = "INSERT 2 14\n"
                        "INSERT 7 24\n"
                        "INSERT 10 -1.2\n"
                        "INSERT 11 12.8\n"
                        "QUERY 10\n"
                        "MIN\n"
                        "MAX\n"
                        "ERASE 11\n"
                        "QUERY 11\n"
                        "PRINT\n"
                        "EXIT\n";

    std::istringstream in(input);
    std::ostringstream out;
    processRankingQueries(in, out);

    std::string expected = "-1.2\n"
                           "10\n"
                           "7\n"
                           "-1\n"
                           "2\t14\n"
                           "7\t24\n"
                           "10\t-1.2\n";

    assert(out.str() == expected);

    // Test update behavior: same node gets new value.
    std::string input2 = "INSERT 1 5\nINSERT 1 3\nQUERY 1\nMIN\nEXIT\n";
    std::istringstream in2(input2);
    std::ostringstream out2;
    processRankingQueries(in2, out2);
    assert(out2.str() == "3\n1\n");

    // Test empty and negative values.
    std::string input3 = "MIN\nMAX\nINSERT -5 -2.5\nMIN\nMAX\nEXIT\n";
    std::istringstream in3(input3);
    std::ostringstream out3;
    processRankingQueries(in3, out3);
    assert(out3.str() == "-1\n-1\n-5\n-5\n");

    // Test ties in values.
    std::string input4 = "INSERT 1 10\nINSERT 2 10\nMIN\nMAX\nERASE 1\nMIN\nEXIT\n";
    std::istringstream in4(input4);
    std::ostringstream out4;
    processRankingQueries(in4, out4);
    assert(out4.str() == "1\n2\n2\n");
}

int main() {
    testDynamicRanking();
    return 0;
}
// The core idea is to maintain two synchronized data structures: a `std::map<int, std::multiset<std::pair<double,int>>::iterator>` for O(log n) access to a node's position in the multiset, and a `std::multiset<std::pair<double,int>>` that orders by (value, node ID). For insertion/update: first erase the node if it exists (using the map to find and remove from the multiset), then insert the new pair into the multiset and store the iterator in the map. This gives O(log n) per insert/update and O(log n) erase. `MIN` and `MAX` are O(1) by checking the multiset's begin() and rbegin() (or end-1). `QUERY` is O(log n) via map lookup and then reading the value from the iterator. `PRINT` is O(n log n) if we iterate the map (since map is sorted by ID, we have to read values from the multiset iterator, but that's fine). Edge cases: empty structures – MIN/MAX return -1, QUERY returns -1; duplicate values are handled by the multiset because the pair includes the node ID as tie-breaker; updating an existing node must remove the old pair completely before inserting the new one. Time complexity per command: INSERT, ERASE, QUERY are O(log n); MIN, MAX are O(1); PRINT is O(n). Space is O(n) for the two structures. The free function parses strings from the input stream, dispatches commands, and writes results to the output stream with proper formatting.
