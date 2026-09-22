// Design a C++ function that models a hybrid inheritance hierarchy for a virtual classroom system. The hierarchy combines hierarchical inheritance (a single base class `Person` with two derived classes `Teacher` and `Student`) and multiple inheritance (a class `TeachingAssistant` that inherits from both `Teacher` and `Student`). The `Person` base class stores a name (as a `std::string`) and provides a virtual method `getRole()` that returns a string identifying the role (e.g., "Person", "Teacher", "Student", "TeachingAssistant"). Each derived class overrides `getRole()`. To avoid the diamond problem, use virtual inheritance for `Teacher` and `Student` when inheriting from `Person`. The `TeachingAssistant` class must define its own `getRole()` and also provide a method `getPrimaryRole()` that explicitly calls one of the base class implementations (e.g., `Teacher::getRole()`) to simulate resolving ambiguity. Write a free function `createAndSummarize()` that takes no arguments, creates a `TeachingAssistant` object with a given name (e.g., "Alex"), and returns a string containing the name, role, and primary role in the format `"Name: <name>, Role: <role>, Primary Role: <primary>"`. The function must be `const`-correct (all methods that do not modify state should be `const`). The input is fixed, so no user input is needed; the function simply demonstrates the hierarchy behavior.
The core problem is demonstrating hybrid inheritance (hierarchical + multiple) with virtual inheritance to resolve the diamond issue. The design uses a `Person` base class with a virtual method `getRole()`. `Teacher` and `Student` inherit from `Person` using `virtual public` to ensure only one copy of `Person` exists in any further derived class. `TeachingAssistant` inherits from both `Teacher` and `Student`; because of virtual inheritance, there is no ambiguity in accessing `Person` members, but `getRole()` is overridden in each intermediate class, so calling `getRole()` on a `TeachingAssistant` object would be ambiguous. Therefore, `TeachingAssistant` must override `getRole()` itself (to return "TeachingAssistant"). For `getPrimaryRole()`, we explicitly call `Teacher::getRole()` to mimic resolving the ambiguity (as in the provided example using `bike::display_vehicle()`). The main algorithm is straightforward: construct the object, call the relevant methods (which are all `const` because they only return strings), and concatenate the results. Edge cases: ensure virtual inheritance prevents multiple copies of `Person` (so no ambiguity in accessing the name), and ensure all methods are correctly marked `const`. Time complexity is O(1) for all operations, and space complexity is O(1) auxiliary (ignoring the strings returned). The solution must be a free function that creates the object and returns the summary string.
#include <string>
#include <iostream>

class Person {
public:
    Person(const std::string& name) : name_(name) {}
    virtual ~Person() = default;
    virtual std::string getRole() const { return "Person"; }
    std::string getName() const { return name_; }

protected:
    std::string name_;
};

class Teacher : virtual public Person {
public:
    Teacher(const std::string& name) : Person(name) {}
    std::string getRole() const override { return "Teacher"; }
};

class Student : virtual public Person {
public:
    Student(const std::string& name) : Person(name) {}
    std::string getRole() const override { return "Student"; }
};

class TeachingAssistant : public Teacher, public Student {
public:
    TeachingAssistant(const std::string& name)
        : Person(name), Teacher(name), Student(name) {}
    std::string getRole() const override { return "TeachingAssistant"; }
    std::string getPrimaryRole() const { return Teacher::getRole(); }
};

// Build a summary string for a TeachingAssistant object named "Alex".
std::string createAndSummarize() {
    TeachingAssistant ta("Alex");
    return "Name: " + ta.getName() + ", Role: " + ta.getRole()
         + ", Primary Role: " + ta.getPrimaryRole();
}
#include <cassert>
#include <string>

int main() {
    // Direct verification of the hierarchy behavior.
    TeachingAssistant ta("Sam");
    assert(ta.getRole() == "TeachingAssistant");
    assert(ta.getPrimaryRole() == "Teacher");
    assert(ta.getName() == "Sam");

    // Verify the summary string format.
    assert(createAndSummarize() == "Name: Alex, Role: TeachingAssistant, Primary Role: Teacher");

    // Verify the full hierarchy (virtual inheritance ensures one Person).
    Person* p = &ta;
    assert(p->getRole() == "TeachingAssistant");

    // Verify that Teacher/Student roles are correct via base class pointers.
    Teacher* t = &ta;
    Student* s = &ta;
    assert(t->getRole() == "TeachingAssistant");
    assert(s->getRole() == "TeachingAssistant");

    // Test with a different name (using a local helper in test only).
    TeachingAssistant ta2("Jordan");
    assert(ta2.getName() == "Jordan");
}
