// Design a C++ class `NamedDateHeap` that manages a collection of records, each containing a 10-character date string in the format `DDMMYYYY` and a name string. The class must support two modes of heap ordering: by name (ascending lexicographic order, where the smallest name is at the top of the max-heap when using an inverted comparison) and by date (ascending chronological order from earliest to latest). Implement a dynamic array-based heap with insert, extract-top, and rebuild operations. Specifically, write a free function `processNamedDates` that reads commands from standard input (as in the snippet) and processes them, returning a string containing all output produced by the commands (printed entries, array prints) in order, with each printed entry on its own line (no extra blank lines except after array prints as per original behavior). The function should handle commands: `+ n` (insert n records read from stdin, each record first 10 characters date then name, space-separated), `- m` (extract and print the top m records in the current mode), `p` (print all current records in heap array order), `r` (switch mode: from name-based to date-based or vice versa, rebuilding the heap accordingly), and `q` (quit). Input ends at end-of-file. Use `std::string` for names, assume valid input, and have a maximum heap capacity of 10000 records.
// The core challenge is implementing two different heap orderings on the same set of records. A max-heap is maintained, but the "largest" element depends on the current mode. In name mode, we use a comparator that treats smaller names as `true` when comparing child to parent during heapify-up (so the smallest name bubbles to the root, effectively a min-heap on names, but the extract operation calls it "ExtractMax" as in the snippet – we'll treat the root as the top element). In date mode, we compare dates chronologically so that the earliest date is at the root. The date string is `DDMMYYYY`, so we can compare by scanning characters: first years (positions 6-9), then months (positions 2-3), then days (positions 0-1). For insertion, we add the new record at the end and bubble up while the child is "better" than its parent. For extraction, we swap the root with the last element, reduce size, and heapify down. When switching mode, we rebuild the entire heap using the new comparator from the current array. Edge cases: empty heap extraction (should not happen per problem). Complexity: insertion O(log n), extraction O(log n), rebuild O(n). Space O(n). The function reads from stdin using `cin` and writes to a `std::ostringstream` to collect output, then returns the accumulated string.
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <stdexcept>

class NamedDateHeap {
private:
    struct Record {
        std::string date; // 10 chars: DDMMYYYY
        std::string name;
    };
    std::vector<Record> heap;
    bool nameMode; // true if ordering by name, false by date

    static int parent(int i) { return (i - 1) / 2; }
    static int left(int i) { return 2 * i + 1; }
    static int right(int i) { return 2 * i + 2; }

    // Return true if a should be placed above b in the heap (i.e., a is "larger")
    bool better(const Record& a, const Record& b) const {
        if (nameMode) {
            // For name mode, we want the smallest name at top (min-heap on names)
            return a.name < b.name;
        } else {
            // For date mode, compare years, months, days
            // date[6..9] = year, date[2..3] = month, date[0..1] = day
            for (int i = 6; i <= 9; ++i) {
                if (a.date[i] != b.date[i]) return a.date[i] < b.date[i];
            }
            for (int i = 2; i <= 3; ++i) {
                if (a.date[i] != b.date[i]) return a.date[i] < b.date[i];
            }
            for (int i = 0; i <= 1; ++i) {
                if (a.date[i] != b.date[i]) return a.date[i] < b.date[i];
            }
            return false; // equal dates
        }
    }

    void heapifyUp(int i) {
        while (i > 0 && better(heap[i], heap[parent(i)])) {
            std::swap(heap[i], heap[parent(i)]);
            i = parent(i);
        }
    }

    void heapifyDown(int i) {
        int n = static_cast<int>(heap.size());
        while (true) {
            int l = left(i);
            int r = right(i);
            int best = i;
            if (l < n && better(heap[l], heap[best])) best = l;
            if (r < n && better(heap[r], heap[best])) best = r;
            if (best == i) break;
            std::swap(heap[i], heap[best]);
            i = best;
        }
    }

public:
    NamedDateHeap() : nameMode(true) {}

    void setMode(bool mode) {
        if (nameMode == mode) return;
        nameMode = mode;
        // Rebuild heap
        for (int i = static_cast<int>(heap.size()) / 2 - 1; i >= 0; --i) {
            heapifyDown(i);
        }
    }

    void insert(const Record& rec) {
        heap.push_back(rec);
        heapifyUp(static_cast<int>(heap.size()) - 1);
    }

    Record extractTop() {
        if (heap.empty()) throw std::runtime_error("extract from empty heap");
        Record top = heap[0];
        if (heap.size() == 1) {
            heap.pop_back();
        } else {
            heap[0] = heap.back();
            heap.pop_back();
            heapifyDown(0);
        }
        return top;
    }

    void printAll(std::ostream& out) const {
        for (const auto& rec : heap) {
            out << rec.date << " " << rec.name << "\n";
        }
        out << "\n"; // extra blank line after print
    }

    bool empty() const { return heap.empty(); }
};

// Process commands from stdin, return accumulated output
std::string processNamedDates() {
    std::ostringstream output;
    NamedDateHeap heap;
    char command;
    while (std::cin >> command) {
        if (command == 'q') break;
        else if (command == '+') {
            int n;
            std::cin >> n;
            for (int i = 0; i < n; ++i) {
                NamedDateHeap::Record rec;
                std::cin >> rec.date >> rec.name;
                heap.insert(rec);
            }
        } else if (command == '-') {
            int m;
            std::cin >> m;
            for (int i = 0; i < m; ++i) {
                if (heap.empty()) break;
                auto top = heap.extractTop();
                output << top.date << " " << top.name << "\n";
            }
        } else if (command == 'p') {
            heap.printAll(output);
        } else if (command == 'r') {
            // Toggle mode
            bool current = heap.getMode(); // we need a getter
            heap.setMode(!current);
        }
    }
    return output.str();
}

But the above solution has a missing getter and access issue. Let me provide a corrected, self-contained version that satisfies the task (only a free function, no main). I'll include a method to toggle mode inside the class and adjust accordingly.

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <stdexcept>

class NamedDateHeap {
private:
    struct Record {
        std::string date;
        std::string name;
    };
    std::vector<Record> heap;
    bool nameMode;

    static int parent(int i) { return (i - 1) / 2; }
    static int left(int i) { return 2 * i + 1; }
    static int right(int i) { return 2 * i + 2; }

    bool better(const Record& a, const Record& b) const {
        if (nameMode) {
            return a.name < b.name;
        } else {
            for (int i = 6; i <= 9; ++i) {
                if (a.date[i] != b.date[i]) return a.date[i] < b.date[i];
            }
            for (int i = 2; i <= 3; ++i) {
                if (a.date[i] != b.date[i]) return a.date[i] < b.date[i];
            }
            for (int i = 0; i <= 1; ++i) {
                if (a.date[i] != b.date[i]) return a.date[i] < b.date[i];
            }
            return false;
        }
    }

    void heapifyUp(int i) {
        while (i > 0 && better(heap[i], heap[parent(i)])) {
            std::swap(heap[i], heap[parent(i)]);
            i = parent(i);
        }
    }

    void heapifyDown(int i) {
        int n = static_cast<int>(heap.size());
        while (true) {
            int l = left(i);
            int r = right(i);
            int best = i;
            if (l < n && better(heap[l], heap[best])) best = l;
            if (r < n && better(heap[r], heap[best])) best = r;
            if (best == i) break;
            std::swap(heap[i], heap[best]);
            i = best;
        }
    }

public:
    NamedDateHeap() : nameMode(true) {}

    void toggleMode() {
        nameMode = !nameMode;
        for (int i = static_cast<int>(heap.size()) / 2 - 1; i >= 0; --i) {
            heapifyDown(i);
        }
    }

    void insert(const std::string& date, const std::string& name) {
        heap.push_back({date, name});
        heapifyUp(static_cast<int>(heap.size()) - 1);
    }

    std::pair<std::string, std::string> extractTop() {
        if (heap.empty()) throw std::runtime_error("empty");
        Record top = heap[0];
        if (heap.size() == 1) {
            heap.pop_back();
        } else {
            heap[0] = heap.back();
            heap.pop_back();
            heapifyDown(0);
        }
        return {top.date, top.name};
    }

    void printAll(std::ostream& out) const {
        for (const auto& rec : heap) {
            out << rec.date << " " << rec.name << "\n";
        }
        out << "\n";
    }
};

// Main processing function
std::string processNamedDates() {
    std::ostringstream output;
    NamedDateHeap heap;
    char command;
    while (std::cin >> command) {
        if (command == 'q') break;
        else if (command == '+') {
            int n;
            std::cin >> n;
            for (int i = 0; i < n; ++i) {
                std::string date, name;
                std::cin >> date >> name;
                heap.insert(date, name);
            }
        } else if (command == '-') {
            int m;
            std::cin >> m;
            for (int i = 0; i < m; ++i) {
                if (heap.empty()) break;
                auto [date, name] = heap.extractTop();
                output << date << " " << name << "\n";
            }
        } else if (command == 'p') {
            heap.printAll(output);
        } else if (command == 'r') {
            heap.toggleMode();
        }
    }
    return output.str();
}
#include <cassert>
#include <sstream>
#include <iostream>

// The solution function is declared externally (assume it's included from above)

int main() {
    // Test 1: Insert two, extract in name mode
    {
        std::istringstream input("+ 2\n01012020 Alice\n02022021 Bob\n- 1\nq\n");
        std::streambuf* old = std::cin.rdbuf(input.rdbuf());
        std::string result = processNamedDates();
        std::cin.rdbuf(old);
        assert(result == "01012020 Alice\n");
    }
    // Test 2: Switch to date mode and extract earliest
    {
        std::istringstream input("+ 2\n01012020 Alice\n02022021 Bob\nr\n- 1\nq\n");
        std::streambuf* old = std::cin.rdbuf(input.rdbuf());
        std::string result = processNamedDates();
        std::cin.rdbuf(old);
        assert(result == "01012020 Alice\n");
    }
    // Test 3: Extract twice in name mode (should get smallest name first)
    {
        std::istringstream input("+ 2\n01012020 Bob\n02022021 Alice\n- 2\nq\n");
        std::streambuf* old = std::cin.rdbuf(input.rdbuf());
        std::string result = processNamedDates();
        std::cin.rdbuf(old);
        assert(result == "02022021 Alice\n01012020 Bob\n");
    }
    // Test 4: Print after inserts
    {
        std::istringstream input("+ 2\n01012020 Bob\n02022021 Alice\np\nq\n");
        std::streambuf* old = std::cin.rdbuf(input.rdbuf());
        std::string result = processNamedDates();
        std::cin.rdbuf(old);
        // The exact array order may depend on heap structure, but must contain both records and a blank line
        assert(result.find("02022021 Alice") != std::string::npos);
        assert(result.find("01012020 Bob") != std::string::npos);
        assert(result.back() == '\n');
        // Ensure there is a double newline at the end (blank line)
        assert(result.length() >= 2 && result[result.length()-1] == '\n' && result[result.length()-2] == '\n');
    }
    // Test 5: Toggle twice should restore order
    {
        std::istringstream input("+ 1\n01012020 Alice\nr\nr\n- 1\nq\n");
        std::streambuf* old = std::cin.rdbuf(input.rdbuf());
        std::string result = processNamedDates();
        std::cin.rdbuf(old);
        assert(result == "01012020 Alice\n");
    }
    // Test 6: Empty extraction should not output anything
    {
        std::istringstream input("- 1\nq\n");
        std::streambuf* old = std::cin.rdbuf(input.rdbuf());
        std::string result = processNamedDates();
        std::cin.rdbuf(old);
        assert(result.empty());
    }
    // Test 7: Multiple records, date comparison across years
    {
        std::istringstream input("+ 2\n31122020 Eve\n01012021 Adam\nr\n- 1\nq\n");
        std::streambuf* old = std::cin.rdbuf(input.rdbuf());
        std::string result = processNamedDates();
        std::cin.rdbuf(old);
        assert(result == "31122020 Eve\n");
    }
    // Test 8: Multiple records, date comparison same year different month
    {
        std::istringstream input("+ 2\n15052020 Bob\n15062020 Alice\nr\n- 1\nq\n");
        std::streambuf* old = std::cin.rdbuf(input.rdbuf());
        std::string result = processNamedDates();
        std::cin.rdbuf(old);
        assert(result == "15052020 Bob\n");
    }
    // Test 9: Same date, name mode picks smaller name
    {
        std::istringstream input("+ 2\n01012020 Bob\n01012020 Alice\n- 1\nq\n");
        std::streambuf* old = std::cin.rdbuf(input.rdbuf());
        std::string result = processNamedDates();
        std::cin.rdbuf(old);
        assert(result == "01012020 Alice\n");
    }
    // Test 10: Larger capacity test
    {
        std::istringstream input("+ 3\n01012020 A\n02012020 B\n03012020 C\nr\np\nq\n");
        std::streambuf* old = std::cin.rdbuf(input.rdbuf());
        std::string result = processNamedDates();
        std::cin.rdbuf(old);
        assert(result.find("01012020 A") != std::string::npos);
        assert(result.find("02012020 B") != std::string::npos);
        assert(result.find("03012020 C") != std::string::npos);
    }
    std::cout << "All tests passed!" << std::endl;
    return 0;
}
