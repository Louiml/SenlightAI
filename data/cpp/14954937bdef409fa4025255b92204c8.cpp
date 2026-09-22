/*
Design a C++ class hierarchy to represent sparse gradients as tree-like structures, where each node is one of four types: `NullGradient` (represents zero), `ScalarGradient` (holds a `double` value), `VectorGradient` (holds a `std::vector<Gradient*>` indexed by integer keys), and `MapGradient` (holds a `std::map<std::string, Gradient*>` indexed by string keys). The hierarchy must support dynamic mutation from `NullGradient` nodes: when an operation is performed on a null leaf, it should transform into a concrete node (e.g., assigning a value via `+=` or indexing into it via `operator[]`), with the parent container updating its pointer accordingly. Implement a free function `double l2norm(const Gradient& g)` that returns the Euclidean norm (L2 norm) of the gradient, calculated by taking the square root of the sum of squares of all scalar leaves reachable through the tree. For `NullGradient`, the norm is `0`; for `ScalarGradient`, it is the absolute value of its scalar; for `VectorGradient` and `MapGradient`, it is the square root of the sum of squares of all children. The solution must include the full class definitions with constructors, destructors (deep-deleting children), virtual methods for `square()`, `operator+=`, `operator-=`, `operator[]`, `operator*=`/`operator/=`, `clone()`, `operator!`, `max()`, `min()`, and `print()`, plus the free function. Use `std::map` and `std::vector`; ensure proper parent tracking so that a `NullGradient` can replace itself in its parent when mutated, and handle `VectorGradient::operator[]` growing the vector with nulls. Provide the free function as the primary task, with the class hierarchy as supporting infrastructure.
*/

#include <cmath>
#include <map>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

class Gradient {
public:
    virtual ~Gradient() = 0;
    virtual Gradient& operator +=(Gradient& rhs) = 0;
    virtual Gradient& operator -=(Gradient& rhs) = 0;
    virtual Gradient& operator +=(double rhs) = 0;
    virtual Gradient& operator -=(double rhs) = 0;
    virtual Gradient& operator *=(double rhs) = 0;
    virtual Gradient& operator /=(double rhs) = 0;
    virtual Gradient& operator [](const std::string& key) = 0;
    virtual Gradient& operator [](int key) = 0;
    virtual Gradient& operator -() const = 0;
    virtual bool operator !() const = 0;
    virtual double square() const = 0;
    virtual double max() const = 0;
    virtual double min() const = 0;
    virtual void print(std::ostream& os) const = 0;
    virtual Gradient* clone() const = 0;
    virtual void set_parent(Gradient* parent) = 0;
    virtual Gradient*& reference(Gradient* g) = 0;
    Gradient& reset(Gradient* self, Gradient* replacement) {
        reference(self) = replacement;
        delete self;
        return *replacement;
    }
};

Gradient::~Gradient() {}

class ScalarGradient;
class VectorGradient;
class MapGradient;
class NullGradient;

class ScalarGradient : public Gradient {
public:
    double scalar;
    ScalarGradient(double value) : scalar(value) {}
    ~ScalarGradient() override {}
    Gradient& operator +=(Gradient& rhs) override {
        if (auto* s = dynamic_cast<ScalarGradient*>(&rhs)) {
            scalar += s->scalar;
            return *this;
        }
        if (dynamic_cast<NullGradient*>(&rhs)) return *this;
        throw std::runtime_error("Incompatible types");
    }
    Gradient& operator -=(Gradient& rhs) override {
        if (auto* s = dynamic_cast<ScalarGradient*>(&rhs)) {
            scalar -= s->scalar;
            return *this;
        }
        if (dynamic_cast<NullGradient*>(&rhs)) return *this;
        throw std::runtime_error("Incompatible types");
    }
    Gradient& operator +=(double rhs) override { scalar += rhs; return *this; }
    Gradient& operator -=(double rhs) override { scalar -= rhs; return *this; }
    Gradient& operator *=(double rhs) override { scalar *= rhs; return *this; }
    Gradient& operator /=(double rhs) override { scalar /= rhs; return *this; }
    Gradient& operator [](const std::string&) override { throw std::runtime_error("Invalid index"); }
    Gradient& operator [](int) override { throw std::runtime_error("Invalid index"); }
    Gradient& operator -() const override { return *new ScalarGradient(-scalar); }
    bool operator !() const override { return std::abs(scalar) < 1e-10; }
    double square() const override { return scalar * scalar; }
    double max() const override { return scalar; }
    double min() const override { return scalar; }
    void print(std::ostream& os) const override { os << scalar; }
    Gradient* clone() const override { return new ScalarGradient(scalar); }
    void set_parent(Gradient*) override {}
    Gradient*& reference(Gradient*) override { throw std::runtime_error("No parent"); }
};

class NullGradient : public Gradient {
public:
    Gradient* parent;
    NullGradient(Gradient* parent) : parent(parent) {}
    ~NullGradient() override {}
    Gradient& operator +=(Gradient& rhs) override {
        if (dynamic_cast<NullGradient*>(&rhs)) return *this;
        return parent->reset(this, rhs.clone());
    }
    Gradient& operator -=(Gradient& rhs) override {
        if (dynamic_cast<NullGradient*>(&rhs)) return *this;
        return parent->reset(this, &-rhs);
    }
    Gradient& operator +=(double rhs) override {
        return parent->reset(this, new ScalarGradient(rhs));
    }
    Gradient& operator -=(double rhs) override {
        return parent->reset(this, new ScalarGradient(-rhs));
    }
    Gradient& operator *=(double) override { return *this; }
    Gradient& operator /=(double) override { return *this; }
    Gradient& operator [](const std::string& key) override {
        std::map<std::string, Gradient*> dict{{key, this}};
        auto* p = parent;
        p->reference(this) = new MapGradient(dict);
        return *this;
    }
    Gradient& operator [](int key) override {
        std::vector<Gradient*> vec(key + 1);
        for (int i = 0; i < key; ++i) vec[i] = new NullGradient(nullptr);
        vec[key] = this;
        auto* p = parent;
        p->reference(this) = new VectorGradient(vec);
        return *this;
    }
    Gradient& operator -() const override { return *new NullGradient(nullptr); }
    bool operator !() const override { return true; }
    double square() const override { return 0; }
    double max() const override { return 0; }
    double min() const override { return 0; }
    void print(std::ostream& os) const override { os << "null"; }
    Gradient* clone() const override { return new NullGradient(parent); }
    void set_parent(Gradient* p) override { parent = p; }
    Gradient*& reference(Gradient*) override { throw std::runtime_error("Null has no children"); }
};

class VectorGradient : public Gradient {
public:
    std::vector<Gradient*> vec;
    VectorGradient() = default;
    VectorGradient(std::vector<Gradient*>& dict) : vec(dict) {
        for (auto* g : dict) g->set_parent(this);
    }
    ~VectorGradient() override {
        for (auto* g : vec) delete g;
    }
    Gradient& operator +=(Gradient& rhs) override {
        if (auto* v = dynamic_cast<VectorGradient*>(&rhs)) {
            for (size_t i = 0; i < v->vec.size(); ++i) {
                (*this)[static_cast<int>(i)] += *v->vec[i];
            }
            return *this;
        }
        if (dynamic_cast<NullGradient*>(&rhs)) return *this;
        throw std::runtime_error("Incompatible types");
    }
    Gradient& operator -=(Gradient& rhs) override {
        if (auto* v = dynamic_cast<VectorGradient*>(&rhs)) {
            for (size_t i = 0; i < v->vec.size(); ++i) {
                (*this)[static_cast<int>(i)] -= *v->vec[i];
            }
            return *this;
        }
        if (dynamic_cast<NullGradient*>(&rhs)) return *this;
        throw std::runtime_error("Incompatible types");
    }
    Gradient& operator +=(double) override { throw std::runtime_error("Invalid"); }
    Gradient& operator -=(double) override { throw std::runtime_error("Invalid"); }
    Gradient& operator *=(double rhs) override {
        for (auto* g : vec) *g *= rhs;
        return *this;
    }
    Gradient& operator /=(double rhs) override {
        for (auto* g : vec) *g /= rhs;
        return *this;
    }
    Gradient& operator [](const std::string&) override { throw std::runtime_error("Invalid index"); }
    Gradient& operator [](int key) override {
        if (key >= static_cast<int>(vec.size())) {
            int start = vec.size();
            vec.resize(key + 1);
            for (int i = start; i <= key; ++i) vec[i] = new NullGradient(this);
        }
        return *vec[key];
    }
    Gradient& operator -() const override {
        std::vector<Gradient*> result(vec.size());
        for (size_t i = 0; i < vec.size(); ++i) result[i] = &-*vec[i];
        return *new VectorGradient(result);
    }
    bool operator !() const override {
        for (auto* g : vec) if (!*g) continue; else return false;
        return true;
    }
    double square() const override {
        double sum = 0;
        for (auto* g : vec) sum += g->square();
        return sum;
    }
    double max() const override {
        double max = -1e308;
        for (auto* g : vec) {
            double m = g->max();
            if (m > max) max = m;
        }
        return max;
    }
    double min() const override {
        double min = 1e308;
        for (auto* g : vec) {
            double m = g->min();
            if (m < min) min = m;
        }
        return min;
    }
    void print(std::ostream& os) const override {
        os << "[";
        for (size_t i = 0; i < vec.size(); ++i) {
            if (i > 0) os << ", ";
            vec[i]->print(os);
        }
        os << "]";
    }
    Gradient* clone() const override {
        std::vector<Gradient*> result(vec.size());
        for (size_t i = 0; i < vec.size(); ++i) result[i] = vec[i]->clone();
        return new VectorGradient(result);
    }
    void set_parent(Gradient*) override {}
    Gradient*& reference(Gradient* g) override {
        for (auto& v : vec) if (v == g) return v;
        throw std::runtime_error("Not found");
    }
};

class MapGradient : public Gradient {
public:
    std::map<std::string, Gradient*> map;
    MapGradient() = default;
    MapGradient(std::map<std::string, Gradient*>& dict) : map(dict) {
        for (auto& pair : dict) pair.second->set_parent(this);
    }
    ~MapGradient() override {
        for (auto& pair : map) delete pair.second;
    }
    Gradient& operator +=(Gradient& rhs) override {
        if (auto* m = dynamic_cast<MapGradient*>(&rhs)) {
            for (auto& pair : m->map) {
                (*this)[pair.first] += *pair.second;
            }
            return *this;
        }
        if (dynamic_cast<NullGradient*>(&rhs)) return *this;
        throw std::runtime_error("Incompatible types");
    }
    Gradient& operator -=(Gradient& rhs) override {
        if (auto* m = dynamic_cast<MapGradient*>(&rhs)) {
            for (auto& pair : m->map) {
                (*this)[pair.first] -= *pair.second;
            }
            return *this;
        }
        if (dynamic_cast<NullGradient*>(&rhs)) return *this;
        throw std::runtime_error("Incompatible types");
    }
    Gradient& operator +=(double) override { throw std::runtime_error("Invalid"); }
    Gradient& operator -=(double) override { throw std::runtime_error("Invalid"); }
    Gradient& operator *=(double rhs) override {
        for (auto& pair : map) *pair.second *= rhs;
        return *this;
    }
    Gradient& operator /=(double rhs) override {
        for (auto& pair : map) *pair.second /= rhs;
        return *this;
    }
    Gradient& operator [](const std::string& key) override {
        if (map.count(key)) return *map[key];
        auto* node = new NullGradient(this);
        map[key] = node;
        return *node;
    }
    Gradient& operator [](int) override { throw std::runtime_error("Invalid index"); }
    Gradient& operator -() const override {
        std::map<std::string, Gradient*> result;
        for (auto& pair : map) result[pair.first] = &-*pair.second;
        return *new MapGradient(result);
    }
    bool operator !() const override {
        for (auto& pair : map) if (!*pair.second) continue; else return false;
        return true;
    }
    double square() const override {
        double sum = 0;
        for (auto& pair : map) sum += pair.second->square();
        return sum;
    }
    double max() const override {
        double max = -1e308;
        for (auto& pair : map) {
            double m = pair.second->max();
            if (m > max) max = m;
        }
        return max;
    }
    double min() const override {
        double min = 1e308;
        for (auto& pair : map) {
            double m = pair.second->min();
            if (m < min) min = m;
        }
        return min;
    }
    void print(std::ostream& os) const override {
        os << "{";
        bool first = true;
        for (auto& pair : map) {
            if (!first) os << ", ";
            first = false;
            os << pair.first << " : ";
            pair.second->print(os);
        }
        os << "}";
    }
    Gradient* clone() const override {
        std::map<std::string, Gradient*> result;
        for (auto& pair : map) result[pair.first] = pair.second->clone();
        return new MapGradient(result);
    }
    void set_parent(Gradient*) override {}
    Gradient*& reference(Gradient* g) override {
        for (auto& pair : map) if (pair.second == g) return pair.second;
        throw std::runtime_error("Not found");
    }
};

std::ostream& operator <<(std::ostream& os, const Gradient& g) {
    g.print(os);
    return os;
}

// Free function: compute the L2 norm of a gradient.
double l2norm(const Gradient& g) {
    return std::sqrt(g.square());
}

#include <cassert>
#include <cmath>

int main() {
    // Scalar
    ScalarGradient s(3.0);
    assert(std::abs(l2norm(s) - 3.0) < 1e-9);
    ScalarGradient s2(-4.0);
    assert(std::abs(l2norm(s2) - 4.0) < 1e-9);

    // Null
    NullGradient n(nullptr);
    assert(l2norm(n) == 0.0);

    // Vector with nulls and scalars
    std::vector<Gradient*> vec1;
    vec1.push_back(new ScalarGradient(3.0));
    vec1.push_back(new NullGradient(nullptr)); // replaced later
    VectorGradient v(vec1);
    assert(std::abs(l2norm(v) - 3.0) < 1e-9);

    // Mutate null to scalar via +=
    v[1] += 4.0;
    assert(std::abs(l2norm(v) - 5.0) < 1e-9);

    // Map with nested vector
    std::map<std::string, Gradient*> map1;
    std::vector<Gradient*> innerVec;
    innerVec.push_back(new ScalarGradient(1.0));
    innerVec.push_back(new ScalarGradient(2.0));
    map1["a"] = new VectorGradient(innerVec);
    map1["b"] = new ScalarGradient(2.0);
    MapGradient m(map1);
    assert(std::abs(l2norm(m) - 3.0) < 1e-9); // sqrt(1+4+4=9) = 3

    // Empty structures
    VectorGradient emptyVec;
    assert(l2norm(emptyVec) == 0.0);
    MapGradient emptyMap;
    assert(l2norm(emptyMap) == 0.0);

    // After scaling
    s *= 2.0; // s becomes 6
    assert(std::abs(l2norm(s) - 6.0) < 1e-9);

    // Deep clone and compute
    Gradient* clone = m.clone();
    assert(std::abs(l2norm(*clone) - 3.0) < 1e-9);
    delete clone;

    return 0;
}

// The core algorithm for computing the L2 norm is a recursive traversal: for each node, if it is a `ScalarGradient`, return `scalar * scalar`; if `NullGradient`, return `0`; otherwise, for `VectorGradient` and `MapGradient`, iterate over all child pointers and recursively sum their `square()` results, then take the square root at the top level. Key edge cases: empty vector/map returns `0`; negative scalars are squared so they contribute positively; deeply nested structures are handled recursively; `NullGradient` children contribute zero. The class hierarchy requires careful memory management: destructors delete all child pointers, `clone()` performs deep copies, and `operator+=`/`operator-=` on `NullGradient` calls `parent->reset(this, replacement)` to update the parent’s pointer and delete the old null. `VectorGradient::operator[]` must resize the vector and fill new slots with `NullGradient` having `this` as parent; `MapGradient::operator[]` creates a `NullGradient` for missing keys. The `reference(Gradient*)` virtual method returns a reference to the pointer in the parent, allowing `reset` to update it. Time complexity for `l2norm` is O(N) where N is the total number of nodes in the tree, as each node is visited once; space complexity is O(D) for the recursion stack where D is the tree depth, but for a typical sparse gradient the depth is small.
