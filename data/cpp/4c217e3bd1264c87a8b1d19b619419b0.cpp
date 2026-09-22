Write a C++ class named `FSA` that models a deterministic finite automaton with string-named states and string-valued transition inputs. The class must support: default construction; construction from a start state and an accept state (both stored as states); construction from an input stream that first reads a line containing the number of states, then that many state names (one per line), then a line with the start state, then a line with the accept state, then a line with the number of transitions, then that many lines each containing `from_state input to_state` (space-separated); adding a state (throwing `std::domain_error` if it already exists); adding a transition (throwing `std::domain_error` if the source state does not exist); checking if a state exists; returning a string representation of all transitions from a given state (throwing `std::domain_error` if the state doesn't exist); a `next(state, input)` method that returns the target state for a transition (throwing `std::domain_error` if the state doesn't exist, or if no transition matches that input, or if the input symbol is not recognized—the set of recognized inputs is exactly those that appear in at least one transition from that state); a `run(input_string)` method that processes the input string character by character (each character converted to a string of length 1) starting from the start state, returning true if the final state is the accept state, and throwing `std::domain_error` if an intermediate state has no transition for the current character; and an overloaded `operator<<` that prints the automaton in a readable format (e.g., each state followed by its transitions, and marking the start and accept states). The `operator>>` for input stream construction must handle malformed input gracefully by throwing `std::domain_error` if the stream is in a bad state or if any expected token is missing. Also provide a method `transitions_to_string(state)` that returns a string listing all transitions from that state, each formatted as `from_input_to` separated by spaces (no trailing space), throwing `std::domain_error` if the state is not found. Ensure all member functions are `const` where appropriate and that `run` does not modify the automaton.
The core data structure is a `std::map<std::string, std::map<std::string, std::string>>` mapping a source state to a map of input symbol to target state. This naturally handles the deterministic constraint (one target per input per state). Also maintain a `std::map<std::string, bool>` or a simple set of state names for `exists_state`. For the `next` method, look up the state, then the input; if either missing, throw `domain_error`. The `run` method iterates over each character of the input string, converting to a one-character string, calls `next`, and updates current state; if `next` throws, propagate it; at the end compare the current state to the accept state. The constructor from stream must first clear any existing data, then read the count of states, read that many state names, read start and accept states, add all states (including start and accept) and set them, then read transition count and add each transition (using `add_state` for source and target as needed, but note the problem says adding a state throws if duplicate—so we must be careful: the stream format may list states that are also sources/targets; we can call `add_state` but catch duplicates? Better approach: first add all declared states, then start/accept are from those, then for each transition, ensure both source and target exist (by adding them with a custom internal method that doesn't throw on duplicates, or by checking existence first). The problem statement's code snippet shows that in the stream constructor, it likely calls `add_state` for each declared state and then adds transitions, but the given tests may include transitions that reference states not in the declared list; to be safe, we can allow adding a transition to a non-existent state by automatically adding it (but the specification for `add_transition` requires source to exist; however for stream construction we can add states on the fly). To match the provided test cases, we'll follow the exact behavior: the constructor from stream should read the number of states, add each as declared, then read start/accept and ensure they exist (they should be among declared), then read transition count and for each transition line, call `add_state` on the from and to (catching duplicate if already exists? Actually the snippet in the case 5 test just reads and prints; we need to ensure the stream constructor works for those files). Since the problem is to create an independent task, we'll define a consistent behavior: the stream constructor may add any state encountered in transitions if not already present, but if a state name appears twice in the declared list, that's an error. Edge cases: empty input string for `run`—if start state equals accept state, return true, else false (no transitions needed). If transition has same source and target, it's fine. For `transitions_to_string`, iterate over the inner map and format `from+input+to` with no spaces. Time complexity: `exists_state` is O(log S), `add_state` O(log S), `add_transition` O(log S + log T) where T is number of inputs from that state, `next` O(log S + log T), `run` O(L * (log S + log T)) where L is length of input string, `operator<<` O(S + total transitions). Space complexity O(S + total transitions).
#include <map>
#include <string>
#include <sstream>
#include <stdexcept>
#include <ostream>
#include <iostream>

class FSA {
private:
    std::map<std::string, std::map<std::string, std::string>> transitions_;
    std::map<std::string, bool> states_;
    std::string start_state_;
    std::string accept_state_;

public:
    FSA() = default;

    FSA(const std::string& start, const std::string& accept)
        : start_state_(start), accept_state_(accept) {
        add_state(start);
        add_state(accept);
    }

    FSA(std::istream& is) {
        int num_states;
        if (!(is >> num_states)) {
            throw std::domain_error("Invalid state count");
        }
        std::string state;
        for (int i = 0; i < num_states; ++i) {
            if (!(is >> state)) {
                throw std::domain_error("Missing state name");
            }
            add_state(state);
        }
        if (!(is >> start_state_)) {
            throw std::domain_error("Missing start state");
        }
        if (!(is >> accept_state_)) {
            throw std::domain_error("Missing accept state");
        }
        if (!exists_state(start_state_) || !exists_state(accept_state_)) {
            throw std::domain_error("Start or accept state not in state list");
        }
        int num_transitions;
        if (!(is >> num_transitions)) {
            throw std::domain_error("Invalid transition count");
        }
        std::string from, input, to;
        for (int i = 0; i < num_transitions; ++i) {
            if (!(is >> from >> input >> to)) {
                throw std::domain_error("Missing transition data");
            }
            // Add states on the fly if needed
            if (!exists_state(from)) add_state(from);
            if (!exists_state(to)) add_state(to);
            add_transition(from, input, to);
        }
    }

    void add_state(const std::string& state) {
        if (exists_state(state)) {
            throw std::domain_error("State already exists: " + state);
        }
        states_[state] = true;
    }

    void add_transition(const std::string& from, const std::string& input, const std::string& to) {
        if (!exists_state(from)) {
            throw std::domain_error("From state does not exist: " + from);
        }
        if (!exists_state(to)) {
            throw std::domain_error("To state does not exist: " + to);
        }
        transitions_[from][input] = to;
    }

    bool exists_state(const std::string& state) const {
        return states_.find(state) != states_.end();
    }

    std::string transitions_to_string(const std::string& state) const {
        auto it = transitions_.find(state);
        if (it == transitions_.end()) {
            throw std::domain_error("State not found: " + state);
        }
        std::ostringstream oss;
        bool first = true;
        for (const auto& pair : it->second) {
            if (!first) oss << " ";
            oss << state << pair.first << pair.second;
            first = false;
        }
        return oss.str();
    }

    std::string next(const std::string& state, const std::string& input) const {
        if (!exists_state(state)) {
            throw std::domain_error("State does not exist: " + state);
        }
        auto state_it = transitions_.find(state);
        if (state_it == transitions_.end()) {
            throw std::domain_error("No transition from state: " + state);
        }
        auto input_it = state_it->second.find(input);
        if (input_it == state_it->second.end()) {
            throw std::domain_error("No transition for input '" + input + "' from state " + state);
        }
        return input_it->second;
    }

    bool run(const std::string& input_str) const {
        std::string current = start_state_;
        for (char c : input_str) {
            current = next(current, std::string(1, c));
        }
        return current == accept_state_;
    }

    friend std::ostream& operator<<(std::ostream& os, const FSA& fsa) {
        for (const auto& state_pair : fsa.states_) {
            const std::string& state = state_pair.first;
            os << (state == fsa.start_state_ ? ">" : "");
            os << (state == fsa.accept_state_ ? "*" : "");
            os << state << ": ";
            auto it = fsa.transitions_.find(state);
            if (it != fsa.transitions_.end()) {
                bool first = true;
                for (const auto& trans : it->second) {
                    if (!first) os << ", ";
                    os << trans.first << "->" << trans.second;
                    first = false;
                }
            }
            os << "\n";
        }
        return os;
    }
};
#include <cassert>
#include <sstream>
#include <string>

// Note: The FSA class is assumed to be defined above (in the solution section).
// This main function tests the solution function directly.

int main() {
    // Test 1: Default construction and add/check states
    FSA fsa1;
    fsa1.add_state("A");
    assert(fsa1.exists_state("A"));
    assert(!fsa1.exists_state("B"));
    bool threw = false;
    try { fsa1.add_state("A"); } catch (const std::domain_error&) { threw = true; }
    assert(threw);

    // Test 2: Construction from start and accept
    FSA fsa2("S", "F");
    assert(fsa2.exists_state("S"));
    assert(fsa2.exists_state("F"));
    fsa2.add_transition("S", "a", "F");
    assert(fsa2.next("S", "a") == "F");
    assert(fsa2.run("a") == true);
    assert(fsa2.run("") == false);

    // Test 3: transitions_to_string
    FSA fsa3("q0", "q1");
    fsa3.add_transition("q0", "0", "q1");
    fsa3.add_transition("q0", "1", "q0");
    fsa3.add_transition("q1", "0", "q0");
    std::string result = fsa3.transitions_to_string("q0");
    assert(result == "q00q1 q11q0" || result == "q00q1 q11q0"); // no spaces before/after

    // Test 4: Stream construction from a sample file content
    std::istringstream input(
        "3\nA\nB\nC\nA\nC\n2\nA 0 B\nB 1 C\n"
    );
    FSA fsa4(input);
    assert(fsa4.exists_state("A") && fsa4.exists_state("B") && fsa4.exists_state("C"));
    assert(fsa4.run("01") == true);
    assert(fsa4.run("00") == false);
    assert(fsa4.run("") == false);

    // Test 5: next throws on missing state or missing input
    FSA fsa5("X", "Y");
    fsa5.add_transition("X", "a", "Y");
    threw = false;
    try { fsa5.next("Z", "a"); } catch (const std::domain_error&) { threw = true; }
    assert(threw);
    threw = false;
    try { fsa5.next("X", "b"); } catch (const std::domain_error&) { threw = true; }
    assert(threw);

    // Test 6: run with invalid character throws
    FSA fsa6("p", "q");
    fsa6.add_transition("p", "0", "q");
    threw = false;
    try { fsa6.run("0x"); } catch (const std::domain_error&) { threw = true; }
    assert(threw);

    // Test 7: operator<< produces non-empty output
    FSA fsa7("s", "t");
    fsa7.add_transition("s", "1", "t");
    std::ostringstream oss;
    oss << fsa7;
    assert(!oss.str().empty());

    // Test 8: Multiple transitions from same state with different inputs
    FSA fsa8("start", "end");
    fsa8.add_transition("start", "0", "mid");
    fsa8.add_transition("mid", "1", "end");
    fsa8.add_transition("start", "1", "end");
    assert(fsa8.run("01") == true);
    assert(fsa8.run("1") == true);
    assert(fsa8.run("0") == false);

    // Test 9: Self-loop
    FSA fsa9("s", "s");
    fsa9.add_transition("s", "a", "s");
    assert(fsa9.run("aaa") == true);
    assert(fsa9.run("") == true);

    // Test 10: Stream constructor throws on malformed input
    std::istringstream bad("1\nA\nB\nC\n0\n");
    threw = false;
    try { FSA fsa_bad(bad); } catch (const std::domain_error&) { threw = true; }
    assert(threw);

    return 0;
}
