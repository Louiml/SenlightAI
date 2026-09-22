// Given an array of `Person` objects where each person has a `name` and `surname`, write a C++ function `std::list<Person> sortPersonsBySurnameThenName(const std::vector<Person>& people)` that returns a new `std::list<Person>` containing all persons from the input vector, sorted primarily by surname in ascending lexicographic order, and secondarily by name if surnames are equal. The function must not modify the input vector, must handle duplicate persons (identical name and surname) by preserving all occurrences (stable sorting is not strictly required but duplicates must appear), and must correctly handle empty input by returning an empty list. Use the `std::string` type for names and surnames, and ensure the sorting comparator is defined exactly as: if surnames differ, compare by surname; otherwise, compare by name, both using standard lexicographic `<` operator on `std::string`.

#include <cassert>
#include <string>
#include <vector>
#include <list>

// Assume the Person class is defined as in the original snippet.
class Person {
public:
    std::string name;
    std::string surname;
    Person() {}
    Person(const std::string& n, const std::string& s) : name(n), surname(s) {}
};

// Include the solution function here (or duplicate it in a header).
// For brevity, we copy it here; in practice, include the header.
std::list<Person> sortPersonsBySurnameThenName(const std::vector<Person>& people) {
    std::list<Person> result(people.begin(), people.end());
    result.sort([](const Person& a, const Person& b) {
        if (a.surname != b.surname) {
            return a.surname < b.surname;
        }
        return a.name < b.name;
    });
    return result;
}

int main() {
    // Test empty input
    std::vector<Person> empty;
    std::list<Person> emptyResult = sortPersonsBySurnameThenName(empty);
    assert(emptyResult.empty());

    // Test single person
    std::vector<Person> single = {Person("Alice", "Smith")};
    std::list<Person> singleResult = sortPersonsBySurnameThenName(single);
    assert(singleResult.size() == 1);
    assert(singleResult.front().surname == "Smith");
    assert(singleResult.front().name == "Alice");

    // Test sorting by surname only
    std::vector<Person> mixed = {
        Person("Bob", "Jones"),
        Person("Alice", "Smith"),
        Person("Charlie", "Brown")
    };
    std::list<Person> sorted = sortPersonsBySurnameThenName(mixed);
    std::vector<std::string> surnames;
    for (const Person& p : sorted) surnames.push_back(p.surname);
    assert((surnames == std::vector<std::string>{"Brown", "Jones", "Smith"}));

    // Test sorting by surname then name (same surname)
    std::vector<Person> sameSurname = {
        Person("Zed", "Doe"),
        Person("Alice", "Doe"),
        Person("Mary", "Doe")
    };
    std::list<Person> sorted2 = sortPersonsBySurnameThenName(sameSurname);
    std::vector<std::string> names;
    for (const Person& p : sorted2) names.push_back(p.name);
    assert((names == std::vector<std::string>{"Alice", "Mary", "Zed"}));

    // Test duplicates – all preserved
    std::vector<Person> withDuplicates = {
        Person("John", "Doe"),
        Person("John", "Doe"),
        Person("Jane", "Doe")
    };
    std::list<Person> sorted3 = sortPersonsBySurnameThenName(withDuplicates);
    assert(sorted3.size() == 3);
    auto it = sorted3.begin();
    assert(it->name == "Jane"); ++it;
    assert(it->name == "John"); ++it;
    assert(it->name == "John");

    // Test input not modified
    std::vector<Person> original = {
        Person("Bob", "Jones"),
        Person("Alice", "Smith")
    };
    sortPersonsBySurnameThenName(original);
    assert(original[0].surname == "Jones");
    assert(original[1].surname == "Smith");

    return 0;
}

#include <string>
#include <list>
#include <vector>

// Returns a list of persons sorted by surname, then by name.
std::list<Person> sortPersonsBySurnameThenName(const std::vector<Person>& people) {
    std::list<Person> result(people.begin(), people.end());
    result.sort([](const Person& a, const Person& b) {
        if (a.surname != b.surname) {
            return a.surname < b.surname;
        }
        return a.name < b.name;
    });
    return result;
}

// The solution approach is straightforward: create a `std::list<Person>` from the input vector, then use the `std::list::sort` member function with a custom comparator that implements the required ordering. The comparator takes two `Person` references (or const references) and returns `true` if the first should come before the second. The logic: if `first.surname != second.surname`, return `first.surname < second.surname`; otherwise, return `first.name < second.name`. This ensures primary sorting by surname and secondary by name. Edge cases include empty input (return empty list), persons with same surname but different names, and completely identical persons (duplicates) — all handled naturally by the comparator because comparison is strict weak ordering (e.g., if both surname and name equal, `false` is returned for both orders, so duplicates remain in the list). Time complexity is O(n log n) where n is the number of persons, as `std::list::sort` uses a merge sort (stable by default in C++ standard library implementations). Space complexity is O(n) for the returned list, plus O(1) auxiliary for sorting (merge sort on list uses no extra heap memory beyond the list itself).
