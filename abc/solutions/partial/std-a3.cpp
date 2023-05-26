#include "abc.h"
#include <bits/stdc++.h>
using namespace std;

const int OP_ZERO    = 0;  // f(OP_ZERO,    x0, x1) = 0
const int OP_NOR     = 1;  // f(OP_NOR,     x0, x1) = !(x0 || x1)
const int OP_GREATER = 2;  // f(OP_GREATER, x0, x1) = (x0 > x1)
const int OP_NOT_X1  = 3;  // f(OP_NOT_X1,  x0, x1) = !x1
const int OP_LESS    = 4;  // f(OP_LESS,    x0, x1) = (x0 < x1)
const int OP_NOT_X0  = 5;  // f(OP_NOT_X0,  x0, x1) = !x0
const int OP_XOR     = 6;  // f(OP_XOR,     x0, x1) = (x0 ^ x1)
const int OP_NAND    = 7;  // f(OP_NAND,    x0, x1) = !(x0 && x1)
const int OP_AND     = 8;  // f(OP_AND,     x0, x1) = (x0 && x1)
const int OP_EQUAL   = 9;  // f(OP_EQUAL,   x0, x1) = (x0 == x1)
const int OP_X0      = 10; // f(OP_X0,      x0, x1) = x0
const int OP_GEQ     = 11; // f(OP_GEQ,     x0, x1) = (x0 >= x1)
const int OP_X1      = 12; // f(OP_X1,      x0, x1) = x1
const int OP_LEQ     = 13; // f(OP_LEQ,     x0, x1) = (x0 <= x1)
const int OP_OR      = 14; // f(OP_OR,      x0, x1) = (x0 || x1)
const int OP_ONE     = 15; // f(OP_ONE,     x0, x1) = 1

static int l, la, lb;
static int* operations;
static int (*operands)[2];
static int (*outputs)[16];

int makeGate(int op, int x0, int x1) {
    operations[l] = op;
    operands[l][0] = x0;
    operands[l][1] = x1;
    return l++;
}

struct Gate;
using GatePtr = shared_ptr<Gate>;

struct Gate {
    int id;
    Gate() : id(-1) { }
    virtual ~Gate() { }
    virtual int materialize() = 0;
};

struct InputGate : Gate {
    int from, index;
    InputGate(int from, int index) : from(from), index(index) { }
    int materialize() override {
        if (id != -1)
            return id;
        return id = from * la + index;
    }
};

struct ComputeGate : Gate {
    GatePtr inputs[2];
    int op;
    ComputeGate(const GatePtr& a, const GatePtr& b, int op) : op(op) {
        inputs[0] = a;
        inputs[1] = b;
    }
    int materialize() override {
        if (id != -1)
            return id;
        return id = makeGate(op, inputs[0]->materialize(), inputs[1]->materialize());
    }
};

struct NotGate : Gate {
    GatePtr input;
    NotGate(const GatePtr& input) : input(input) { }
    int materialize() override {
        if (id != -1)
            return id;
        return id = makeGate(OP_NOT_X0, input->materialize(), 0);
    }
};

struct ConstGate : Gate {
    bool value;
    ConstGate(bool value) : value(value) { }
    int materialize() override {
        if (id != -1)
            return id;
        return id = makeGate(value * OP_ONE, 0, 0);
    }
};

struct UndefinedGate : Gate {
    int materialize() override {
        if (id != -1)
            return id;
        return id = makeGate(OP_ZERO, 0, 0);
    }
};

static int dual(int op) {
    return op & 9 | (op & 2) << 1 | (op & 4) >> 1;
}

static GatePtr undefinedGate() {
    static GatePtr undefined = make_shared<UndefinedGate>();
    return undefined;
}

static GatePtr constGate(bool value) {
    static GatePtr consts[2] = { make_shared<ConstGate>(false), make_shared<ConstGate>(true) };
    return consts[value];
}

static GatePtr notGate(const GatePtr& input) {
    if (auto c = dynamic_cast<ConstGate*>(input.get()))
        return constGate(!c->value);
    if (auto u = dynamic_cast<UndefinedGate*>(input.get()))
        return undefinedGate();
    if (auto n = dynamic_cast<NotGate*>(input.get()))
        return n->input;
    return make_shared<NotGate>(input);
}

static map<tuple<GatePtr, GatePtr, int>, GatePtr> computeGateCache;
static GatePtr computeGate(const GatePtr& a, const GatePtr& b, int op) {
    if (op == OP_ZERO || op == OP_ONE)
        return constGate(op == OP_ONE);
    if (op == OP_X0)
        return a;
    if (op == OP_X1)
        return b;
    if (op == OP_NOT_X0)
        return notGate(a);
    if (op == OP_NOT_X1)
        return notGate(b);
    
    if (NotGate* na = dynamic_cast<NotGate*>(a.get()))
        return computeGate(b, a, dual(op));
    if (NotGate* nb = dynamic_cast<NotGate*>(b.get()))
        return computeGate(a, nb->input, op >> 2 | (op & 3) << 2);

    UndefinedGate* ua = dynamic_cast<UndefinedGate*>(a.get()), *ub = dynamic_cast<UndefinedGate*>(b.get());
    if (ua && ub)
        return undefinedGate();
    if (ua || ub) {
        if (op == OP_XOR || op == OP_EQUAL)
            return undefinedGate();
        return constGate(__builtin_popcount(op) >> 1);
    }

    ConstGate* ca = dynamic_cast<ConstGate*>(a.get()), *cb = dynamic_cast<ConstGate*>(b.get());
    if (ca && cb)
        return constGate(op >> (ca->value | cb->value << 1) & 1);
    if (ca)
        return computeGate(b, a, dual(op));
    if (cb) {
        int x = op >> (cb->value << 1) & 3;
        if (x == 1)
            return notGate(a);
        if (x == 2)
            return a;
        return constGate(x == 3);
    }

    auto& result = computeGateCache[make_tuple(a, b, op)];
    if (result)
        return result;
    result = make_shared<ComputeGate>(a, b, op);
    return result;
}

static GatePtr inputGate(int from, int index) {
    return make_shared<InputGate>(from, index);
}

struct Bit {
    GatePtr gate;
    Bit() : gate(undefinedGate()) { }
    explicit Bit(bool value) : gate(constGate(value)) { }
    Bit(const GatePtr& gate) : gate(gate) { }
};
static Bit operator&(const Bit& a, const Bit& b) {
    return computeGate(a.gate, b.gate, OP_AND);
}
static Bit operator|(const Bit& a, const Bit& b) {
    return computeGate(a.gate, b.gate, OP_OR);
}
static Bit operator^(const Bit& a, const Bit& b) {
    return computeGate(a.gate, b.gate, OP_XOR);
}
static Bit operator~(const Bit& a) {
    return notGate(a.gate);
}
static Bit operator==(const Bit& a, const Bit& b) {
    return computeGate(a.gate, b.gate, OP_EQUAL);
}
static Bit operator!=(const Bit& a, const Bit& b) {
    return computeGate(a.gate, b.gate, OP_XOR);
}
static Bit operator>=(const Bit& a, const Bit& b) {
    return computeGate(a.gate, b.gate, OP_GEQ);
}
static Bit operator<=(const Bit& a, const Bit& b) {
    return computeGate(a.gate, b.gate, OP_LEQ);
}
static Bit operator>(const Bit& a, const Bit& b) {
    return computeGate(a.gate, b.gate, OP_GREATER);
}
static Bit operator<(const Bit& a, const Bit& b) {
    return computeGate(a.gate, b.gate, OP_LESS);
}

struct Integer : vector<Bit> {
    using vector<Bit>::vector;
};
static Integer operator&(const Integer& a, const Integer& b) {
    assert(a.size() == b.size());
    Integer result;
    for (int i = 0; i < a.size(); i++)
        result.push_back(a[i] & b[i]);
    return result;
}
static Integer operator|(const Integer& a, const Integer& b) {
    assert(a.size() == b.size());
    Integer result;
    for (int i = 0; i < a.size(); i++)
        result.push_back(a[i] | b[i]);
    return result;
}
static Integer operator^(const Integer& a, const Integer& b) {
    assert(a.size() == b.size());
    Integer result;
    for (int i = 0; i < a.size(); i++)
        result.push_back(a[i] ^ b[i]);
    return result;
}
static Integer operator~(const Integer& a) {
    Integer result;
    for (int i = 0; i < a.size(); i++)
        result.push_back(~a[i]);
    return result;
}
static Integer operator+(const Integer& a, const Integer& b) {
    assert(a.size() == b.size());
    Integer result;
    Bit carry(false);
    for (int i = 0; i < a.size(); i++) {
        Bit x = a[i] ^ b[i];
        result.push_back(x ^ carry);
        carry = (a[i] & b[i]) | (x & carry);
    }
    return result;
}
static Integer& operator+=(Integer& a, const Integer& b) {
    return a = a + b;
}
static Bit operator==(const Integer& a, const Integer& b) {
    assert(a.size() == b.size());
    Bit result(true);
    for (int i = 0; i < a.size(); i++)
        result = result & (a[i] == b[i]);
    return result;
}
static Bit operator!=(const Integer& a, const Integer& b) {
    return ~(a == b);
}
static Bit operator<(const Integer& a, const Integer& b) {
    assert(a.size() == b.size());
    Bit result(false);
    for (int i = a.size() - 1; i >= 0; i--)
        result = (a[i] < b[i]) | ((a[i] == b[i]) & result);
    return result;
}
static Bit operator>(const Integer& a, const Integer& b) {
    return b < a;
}
static Bit operator<=(const Integer& a, const Integer& b) {
    return ~(a > b);
}
static Bit operator>=(const Integer& a, const Integer& b) {
    return ~(a < b);
}
static Integer inputInteger(int from, int index, int size) {
    Integer result;
    for (int i = 0; i < size; i++)
        result.push_back(inputGate(from, index + i));
    return result;
}
static Integer zeroInteger(int size) {
    Integer result;
    for (int i = 0; i < size; i++)
        result.push_back(Bit(false));
    return result;
}
static int materialize(const Bit& bit) {
    return bit.gate->materialize();
}
static vector<int> materialize(const Integer& integer) {
    vector<int> result;
    for (int i = 0; i < integer.size(); i++)
        result.push_back(materialize(integer[i]));
    return result;
}


// Alice
int // returns la
alice(
    /*  in */ const int n,
    /*  in */ const char names[][5],
    /*  in */ const unsigned short numbers[],
    /* out */ bool outputs[]
) {
    for (int i = 0; i < 16; i++)
        outputs[i] = numbers[0] >> i & 1;
    return 16;
}


// Bob
int // returns lb
bob(
    /*  in */ const int m,
    /*  in */ const char senders[][5],
    /*  in */ const char recipients[][5],
    /* out */ bool outputs[]
) {
    return m;
}


// Circuit
int // returns l
circuit(
    /*  in */ const int la,
    /*  in */ const int lb,
    /* out */ int operations[],
    /* out */ int operands[][2],
    /* out */ int outputs[][16]
) {
    ::l = la + lb;
    ::la = la;
    ::lb = lb;
    ::operations = operations;
    ::operands = operands;
    ::outputs = outputs;
    
    Integer a = inputInteger(0, 0, 16);
    Integer sum = zeroInteger(16);
    for (int i = 0; i < lb; i++)
        sum += a;
    
    vector<int> result = materialize(sum);
    memcpy(outputs[0], result.data(), 16 * sizeof(int));

    return ::l;
}
