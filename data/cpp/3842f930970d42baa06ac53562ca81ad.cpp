// Given a tree represented by nodes of three types — parameter nodes (constant values), variable nodes (independent inputs), and unary/binary operation nodes (functions like addition, multiplication, sine, etc.) — write a standalone C++ function that, given a root node and a list of variable-node pointers, computes the gradient (vector of partial derivatives) of the expression tree with respect to those variables using reverse-mode automatic differentiation. The function should return the function value and fill a provided `std::vector<double>` with the gradient components in the same order as the input variable list. The implementation must be self-contained, not rely on any external autodiff library, and correctly handle expressions that are constant or involve variables not present in the tree. The nodes are assumed to be defined in a simple struct hierarchy with virtual methods for evaluation and backpropagation, and the function must manage its own tape/stack for reverse traversal.

#include <cassert>
#include <cmath>
#include <vector>

int main() {
    // Test 1: f = x + y, x=2, y=3
    VarNode x(2.0), y(3.0);
    BinaryNode add(OpCode::Add, &x, &y);
    std::vector<VarNode*> vars1 = {&x, &y};
    std::vector<double> grad1;
    double val1 = computeGradient(&add, vars1, grad1);
    assert(std::fabs(val1 - 5.0) < 1e-12);
    assert(std::fabs(grad1[0] - 1.0) < 1e-12);
    assert(std::fabs(grad1[1] - 1.0) < 1e-12);

    // Test 2: f = x * y, x=4, y=0.5
    VarNode x2(4.0), y2(0.5);
    BinaryNode mul(OpCode::Mul, &x2, &y2);
    std::vector<VarNode*> vars2 = {&x2, &y2};
    std::vector<double> grad2;
    double val2 = computeGradient(&mul, vars2, grad2);
    assert(std::fabs(val2 - 2.0) < 1e-12);
    assert(std::fabs(grad2[0] - 0.5) < 1e-12);
    assert(std::fabs(grad2[1] - 4.0) < 1e-12);

    // Test 3: f = sin(x) * exp(y), x=0, y=0 => f=0, grad = [exp(0)*cos(0)=1, sin(0)*exp(0)=0]
    VarNode x3(0.0), y3(0.0);
    UnaryNode sin_node(OpCode::Sin, &x3);
    UnaryNode exp_node(OpCode::Exp, &y3);
    BinaryNode mul3(OpCode::Mul, &sin_node, &exp_node);
    std::vector<VarNode*> vars3 = {&x3, &y3};
    std::vector<double> grad3;
    double val3 = computeGradient(&mul3, vars3, grad3);
    assert(std::fabs(val3 - 0.0) < 1e-12);
    assert(std::fabs(grad3[0] - 1.0) < 1e-12);
    assert(std::fabs(grad3[1] - 0.0) < 1e-12);

    // Test 4: f = x / (y + 2), x=6, y=1 => f=2, grad = [1/(y+2)=1/3, -x/(y+2)^2 = -6/9 = -2/3]
    VarNode x4(6.0), y4(1.0);
    ConstNode two(2.0);
    BinaryNode add4(OpCode::Add, &y4, &two);
    BinaryNode div4(OpCode::Div, &x4, &add4);
    std::vector<VarNode*> vars4 = {&x4, &y4};
    std::vector<double> grad4;
    double val4 = computeGradient(&div4, vars4, grad4);
    assert(std::fabs(val4 - 2.0) < 1e-12);
    assert(std::fabs(grad4[0] - (1.0/3.0)) < 1e-12);
    assert(std::fabs(grad4[1] - (-2.0/3.0)) < 1e-12);

    // Test 5: variable not in tree yields zero gradient
    VarNode x5(1.0);
    VarNode y5(10.0); // not used
    UnaryNode cos_node(OpCode::Cos, &x5);
    std::vector<VarNode*> vars5 = {&x5, &y5};
    std::vector<double> grad5;
    double val5 = computeGradient(&cos_node, vars5, grad5);
    assert(std::fabs(val5 - std::cos(1.0)) < 1e-12);
    assert(std::fabs(grad5[0] + std::sin(1.0)) < 1e-12); // dcos/dx = -sin(x)
    assert(std::fabs(grad5[1] - 0.0) < 1e-12);

    return 0;
}

#include <vector>
#include <cmath>
#include <stack>
#include <memory>
#include <stdexcept>

// Forward declarations
enum class OpCode { Add, Sub, Mul, Div, Sin, Cos, Exp, Log, Neg };

struct Node {
    virtual double evaluate() = 0;
    virtual void backprop(double adjoint) = 0;
    virtual ~Node() {}
};

struct ConstNode : Node {
    double value;
    explicit ConstNode(double v) : value(v) {}
    double evaluate() override { return value; }
    void backprop(double) override {} // no children
};

struct VarNode : Node {
    double value;
    double adjoint = 0.0;
    explicit VarNode(double v) : value(v) {}
    double evaluate() override { return value; }
    void backprop(double adj) override { adjoint += adj; }
};

struct UnaryNode : Node {
    OpCode op;
    Node* child;
    double child_value;
    UnaryNode(OpCode o, Node* c) : op(o), child(c) {}
    double evaluate() override {
        child_value = child->evaluate();
        switch (op) {
            case OpCode::Neg: return -child_value;
            case OpCode::Sin: return std::sin(child_value);
            case OpCode::Cos: return std::cos(child_value);
            case OpCode::Exp: return std::exp(child_value);
            case OpCode::Log: return std::log(child_value);
            default: throw std::runtime_error("Invalid unary op");
        }
    }
    void backprop(double adj) override {
        double local_derivative;
        switch (op) {
            case OpCode::Neg: local_derivative = -1.0; break;
            case OpCode::Sin: local_derivative = std::cos(child_value); break;
            case OpCode::Cos: local_derivative = -std::sin(child_value); break;
            case OpCode::Exp: local_derivative = std::exp(child_value); break;
            case OpCode::Log: local_derivative = 1.0 / child_value; break;
            default: throw std::runtime_error("Invalid unary op");
        }
        child->backprop(adj * local_derivative);
    }
};

struct BinaryNode : Node {
    OpCode op;
    Node* left;
    Node* right;
    double left_value, right_value;
    BinaryNode(OpCode o, Node* l, Node* r) : op(o), left(l), right(r) {}
    double evaluate() override {
        left_value = left->evaluate();
        right_value = right->evaluate();
        switch (op) {
            case OpCode::Add: return left_value + right_value;
            case OpCode::Sub: return left_value - right_value;
            case OpCode::Mul: return left_value * right_value;
            case OpCode::Div: return left_value / right_value;
            default: throw std::runtime_error("Invalid binary op");
        }
    }
    void backprop(double adj) override {
        double dl = 0.0, dr = 0.0;
        switch (op) {
            case OpCode::Add: dl = 1.0; dr = 1.0; break;
            case OpCode::Sub: dl = 1.0; dr = -1.0; break;
            case OpCode::Mul: dl = right_value; dr = left_value; break;
            case OpCode::Div: dl = 1.0 / right_value; dr = -left_value / (right_value * right_value); break;
        }
        left->backprop(adj * dl);
        right->backprop(adj * dr);
    }
};

// The main gradient computation function
double computeGradient(Node* root, const std::vector<VarNode*>& variables, std::vector<double>& gradient) {
    // Reset adjoints of all variable nodes
    for (VarNode* v : variables) {
        v->adjoint = 0.0;
    }

    // Forward pass: evaluate the root, which recursively evaluates all children
    double value = root->evaluate();

    // Backward pass: start with adjoint 1.0 at the root
    root->backprop(1.0);

    // Extract gradient values in the order given by the variable list
    gradient.clear();
    for (VarNode* v : variables) {
        gradient.push_back(v->adjoint);
    }

    return value;
}

// Reverse-mode automatic differentiation computes both the function value and all partial derivatives in one forward pass and one backward pass. The forward pass evaluates each node’s value and stores it on a stack (or in the node). During the backward pass, we propagate the derivative of the output with respect to each node’s value backward from the root to the leaves. For a node with output `z` and children `x` and `y`, the chain rule states: `dz/dx = dz/dz * dz/dx` and similarly for `y`. We accumulate adjoints (derivatives of the final output with respect to each node’s value) starting with the root’s adjoint set to 1. For each operation, we compute local partial derivatives (e.g., for addition: both 1; for multiplication: `y` and `x`; for sine: `cos(x)`) and add them to the children’s adjoints. After processing all nodes in topological order (post-order), each variable node’s adjoint equals the desired gradient component. Edge cases: variables not present in the tree will have adjoint 0; constant nodes and unused subexpressions must be handled without errors; division by zero or domain errors in elementary functions are not the focus here, but robust code should avoid them. Time complexity is O(N) for N nodes, and auxiliary space is O(N) for the stack/topological order.
