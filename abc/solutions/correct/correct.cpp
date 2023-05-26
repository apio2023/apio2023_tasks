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
    /*if (op == OP_ZERO || op == OP_ONE)
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
    if (ua)
        return computeGate(b, a, dual(op));
    if (ub) {
        int cnt = __builtin_popcount(op);
        if (cnt >= 3)
            return constGate(1);
        if (cnt <= 1)
            return constGate(0);
        return a;
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
    }*/

    /*auto& result = computeGateCache[make_tuple(a, b, op)];
    if (result)
        return result;
    result = make_shared<ComputeGate>(a, b, op);
    return result;*/
    return make_shared<ComputeGate>(a, b, op);
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
static Bit& operator^=(Bit& a, const Bit& b) {
    a = a ^ b;
    return a;
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
    for (int i = 0; i < a.size(); i++)
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
static Bit inputBit(int from, int index) {
    return Bit(inputGate(from, index));
}
static Integer inputInteger(int from, int index, int size) {
    Integer result;
    for (int i = 0; i < size; i++)
        result.push_back(inputBit(from, index + i));
    return result;
}
static Integer zeroInteger(int size) {
    Integer result;
    for (int i = 0; i < size; i++)
        result.push_back(Bit(false));
    return result;
}
static Integer undefinedInteger(int size) {
    Integer result;
    for (int i = 0; i < size; i++)
        result.push_back(Bit());
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
static void condSwap(const Bit& cond, Bit& a, Bit& b) {
    Bit x = a ^ b;
    a ^= (x & cond);
    b ^= (x & cond);
}
static void condSwap(const Bit& cond, Integer& a, Integer& b) {
    assert(a.size() == b.size());
    for (int i = 0; i < a.size(); i++)
        condSwap(cond, a[i], b[i]);
}
static void condAssign(const Bit& cond, Integer& a, const Integer& b) {
    Integer bb = b;
    condSwap(cond, a, bb);
}

struct Element {
    Integer src;    // 19b
    Integer dst;    // 19b
    Integer weight; // 16b
    Bit isEdge;
    // 19+19+16+1=55b
};
static void condSwap(const Bit& cond, Element& a, Element& b) {
    condSwap(cond, a.src, b.src);
    condSwap(cond, a.dst, b.dst);
    condSwap(cond, a.weight, b.weight);
    condSwap(cond, a.isEdge, b.isEdge);
}

static int charsToInt(const char c[5]) {
    int result = 0;
    for (int i = 0; i < 4; i++) {
        if (!c[i])
            break;
        result = result * 26 + (c[i] - 'a') + 1;
    }
    return result;
}

static vector<bool> switchGenerateInv(vector<int> a) {
    // permute `a` to [0, 1, ..., n - 1]
    int n = a.size();
    // cout << "switchGenerateInv " << n << endl;
    if (n <= 1)
        return vector<bool>();
    
    vector<bool> upHold((n + 1) / 2), downHold((n + 1) / 2);
    if (n % 2) {
        // upHold[n / 2 * 2] = true;
        downHold[a[n - 1] >= n / 2 ? a[n - 1] - n / 2 : a[n - 1]] = true;
    }
    
    vector<bool> trace(n / 2), sure(n / 2);

    int cnt = n / 2;
    while (cnt > 0) {
        // cout << cnt << endl;
        bool ok = false;
        for (int i = 0; i < n / 2; i++)
            if (!sure[i]) {
                int col = a[i] >= n / 2 ? a[i] - n / 2 : a[i];
                int ncol = a[i + n / 2] >= n / 2 ? a[i + n / 2] - n / 2 : a[i + n / 2];
                if (upHold[col] || downHold[ncol]) {
                    downHold[col] = 1;
                    upHold[ncol] = 1;
                    trace[i] = 1;
                    swap(a[i], a[i + n / 2]);
                    sure[i] = 1;
                    ok = true;
                    --cnt;
                }
                else if (downHold[col] || upHold[ncol]) {
                    upHold[col] = 1;
                    downHold[ncol] = 1;
                    trace[i] = 0;
                    sure[i] = 1;
                    ok = true;
                    --cnt;
                }
            }
        if (ok)
            continue;
        for (int i = 0; i < n / 2; i++)
            if (!sure[i]) {
                int col = a[i] >= n / 2 ? a[i] - n / 2 : a[i];
                int ncol = a[i + n / 2] >= n / 2 ? a[i + n / 2] - n / 2 : a[i + n / 2];
                upHold[col] = 1;
                downHold[ncol] = 1;
                trace[i] = 0;
                sure[i] = 1;
                --cnt;
                break;
            }
    }
    // cout << cnt << endl;

    vector<int> a1(a.begin(), a.begin() + n / 2);
    vector<int> a2(a.begin() + n / 2, a.end());
    transform(a1.begin(), a1.end(), a1.begin(), [&](int x) { return x >= n / 2 ? x - n / 2 : x; });
    transform(a2.begin(), a2.end(), a2.begin(), [&](int x) { return x >= n / 2 ? x - n / 2 : x; });
    auto trace1 = switchGenerateInv(a1);
    auto trace2 = switchGenerateInv(a2);
    trace.insert(trace.end(), trace1.begin(), trace1.end());
    trace.insert(trace.end(), trace2.begin(), trace2.end());
    vector<bool> trace3(n / 2);
    for (int i = 0; i < n / 2; i++)
        if (a[i] >= n / 2)
            trace3[a[i] - n / 2] = 1;
    trace.insert(trace.end(), trace3.begin(), trace3.end());
    return trace;
}

static vector<bool> switchGenerate(const vector<int>& a) {
    // permute [0, 1, ..., n - 1] to `a`
    int n = a.size();
    vector<int> invA(n);
    for (int i = 0; i < n; i++)
        invA[a[i]] = i;
    return switchGenerateInv(invA);
}


// Alice
int // returns la
alice(
    /*  in */ const int n,
    /*  in */ const char names[][5],
    /*  in */ const unsigned short numbers[],
    /* out */ bool outputs[]
) {
    int l = 0;
    auto addBit = [&](bool bit) {
        outputs[l++] = bit;
    };
    auto addInteger = [&](int integer, int size) {
        for (int i = 0; i < size; i++)
            addBit(integer >> i & 1);
    };

    vector<tuple<int, unsigned short, int>> v;
    for (int i = 0; i < n; i++)
        v.push_back(make_tuple(charsToInt(names[i]), numbers[i], i));
    sort(v.begin(), v.end());

    for (int i = 0; i < n; i++) {
        addInteger(get<0>(v[i]), 19);
        addInteger(get<0>(v[i]), 19);
        addInteger(get<1>(v[i]), 16);
        addBit(false);
    }

    vector<int> p;
    for (int i = 0; i < n; i++)
        p.push_back(get<2>(v[i]));
    // for (int i = 0; i < n; i++)
    //     cout << p[i] << " ";
    // cout << endl;
    auto trace = switchGenerateInv(p);
    for (bool bit : trace)
        addBit(bit);

    return l;
}


// Bob
int // returns lb
bob(
    /*  in */ const int m,
    /*  in */ const char senders[][5],
    /*  in */ const char recipients[][5],
    /* out */ bool outputs[]
) {
    int l = 0;
    auto addBit = [&](bool bit) {
        outputs[l++] = bit;
    };
    auto addInteger = [&](int integer, int size) {
        for (int i = 0; i < size; i++)
            addBit(integer >> i & 1);
    };

    vector<pair<int, int>> v;
    for (int i = 0; i < m; i++)
        v.push_back(make_pair(charsToInt(senders[i]), charsToInt(recipients[i])));
    sort(v.begin(), v.end());

    vector<tuple<int, int, int>> w;
    for (int i = 0; i < m; i++)
        w.emplace_back(v[i].first, v[i].second, i);
    sort(w.begin(), w.end(), [](const tuple<int, int, int>& a, const tuple<int, int, int>& b) {
        return get<1>(a) < get<1>(b);
    });
    vector<int> p;
    for (int i = 0; i < m; i++)
        p.push_back(get<2>(w[i]));
    vector<bool> trace = switchGenerate(p);

    for (int i = 0; i < m; i++) {
        addInteger(v[i].first, 19);
        addInteger(v[i].second, 19);
        addInteger(0, 16);
        addBit(true);
    }

    for (bool bit : trace)
        addBit(bit);
    return l;
}

template <typename F>
static void buildMergeNetworkImpl(const F& yield, int i, int x, int j, int k, int r) {
    int step = r * 2, m = x - i, n = k - j;
    if (m <= 0 || n <= 0)
        return;
    if (m <= r && n <= r) {
        yield(i, j);
        return;
    }
    buildMergeNetworkImpl(yield, i, x, j, k, step);
    buildMergeNetworkImpl(yield, i + r, x, j + r, k, step);
    for (i += r, x -= r; i < x; i += step)
        yield(i, i + r);
    if (i < x + r) {
        yield(i, j);
        j += r;
    }
    for (k -= r; j < k; j += step)
        yield(j, j + r);
}

template <typename F>
static void buildMergeNetworkImpl(const F& yield, int l, int m, int r) {
    auto myYield = [&](int x, int y) {
        if (x >= l && x < r && y >= l && y < r)
            yield(x, y);
    };
    buildMergeNetworkImpl(myYield, l, m, m, r, 1);
}

static vector<pair<int, int>> buildMergeNetwork(int n1, int n2) {
    int mx = max(n1, n2), n = n1 + n2;
    vector<pair<int, int>> ret;
    buildMergeNetworkImpl([&](int x, int y) {
        if (x >= 0 && x < n && y >= 0 && y < n)
            ret.emplace_back(x, y);
    }, n1 - mx, n1, n1 + mx);
    return ret;
}

template <typename Less>
static void sortElements(vector<Element>& elements, Less less) {
    // TODO: optimize
    for (int i = 0; i < elements.size(); i++)
        for (int j = i + 1; j < elements.size(); j++)
            condSwap(less(elements[j], elements[i]), elements[i], elements[j]);
}

template <typename Less>
static vector<tuple<int, int, Bit>> mergeElements(vector<Element>& elements, int mid, Less less) {
    vector<pair<int, int>> network = buildMergeNetwork(mid, elements.size() - mid);
    vector<tuple<int, int, Bit>> ret;
    for (auto& p : network) {
        Bit cond = less(elements[p.second], elements[p.first]);
        condSwap(cond, elements[p.first], elements[p.second]);
        ret.emplace_back(p.first, p.second, cond);
    }
    return ret;
}

static void undoMergeElements(vector<Element>& elements, const vector<tuple<int, int, Bit>>& operations) {
    for (int i = operations.size() - 1; i >= 0; i--) {
        int x, y;
        Bit cond;
        tie(x, y, cond) = operations[i];
        condSwap(cond, elements[x], elements[y]);
    }
}

template <typename F>
static void buildSwitchNetworkImpl(const F& yield, int l, int r) {
    if (r - l <= 1)
        return;
    int m = (l + r) / 2;
    auto preOrPost = [&]() {
        for (int i = l, j = m; i < m; i++, j++)
            yield(i, j);
    };
    preOrPost();
    buildSwitchNetworkImpl(yield, l, m);
    buildSwitchNetworkImpl(yield, m, r);
    preOrPost();
}

static vector<pair<int, int>> buildSwitchNetwork(int n) {
    vector<pair<int, int>> ret;
    buildSwitchNetworkImpl([&](int x, int y) {
        if (x >= 0 && x < n && y >= 0 && y < n)
            ret.emplace_back(x, y);
    }, 0, n);
    return ret;
}

static void permuteElements(vector<Element>& elements, const vector<Bit>& trace) {
    vector<pair<int, int>> network = buildSwitchNetwork(elements.size());
    assert(network.size() == trace.size());
    for (int i = 0; i < network.size(); i++)
        condSwap(trace[i], elements[network[i].first], elements[network[i].second]);
}

static void undoPermuteElements(vector<Element>& elements, const vector<Bit>& trace) {
    vector<pair<int, int>> network = buildSwitchNetwork(elements.size());
    assert(network.size() == trace.size());
    for (int i = network.size() - 1; i >= 0; i--)
        condSwap(trace[i], elements[network[i].first], elements[network[i].second]);
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

    // cout << la << ' ' << lb << endl;
    
    int n = 0;
    while (n * 55 + buildSwitchNetwork(n).size() < la)
        n++;
    int m = 0;
    while (m * 55 + buildSwitchNetwork(m).size() < lb)
        m++;
    // cout << n << ' ' << m << endl;
    assert(m * 55 + buildSwitchNetwork(m).size() == lb);
    int nm = n + m;
    vector<Element> elements;
    for (int i = 0; i < n; i++) {
        Element element;
        element.src = inputInteger(0, i * 55, 19);
        element.dst = inputInteger(0, i * 55 + 19, 19);
        element.weight = inputInteger(0, i * 55 + 38, 16);
        element.isEdge = inputBit(0, i * 55 + 54);
        elements.push_back(element);
    }
    for (int i = 0; i < m; i++) {
        Element element;
        element.src = inputInteger(1, i * 55, 19);
        element.dst = inputInteger(1, i * 55 + 19, 19);
        element.weight = inputInteger(1, i * 55 + 38, 16);
        element.isEdge = inputBit(1, i * 55 + 54);
        elements.push_back(element);
    }
    vector<Bit> perm;
    for (int i = 0; i < lb - m * 55; i++)
        perm.push_back(inputBit(1, m * 55 + i));
        
    vector<Bit> permA;
    for (int i = 0; i < la - n * 55; i++)
        permA.push_back(inputBit(0, n * 55 + i));

    auto trace = mergeElements(elements, n, [](const Element& a, const Element& b) {
        return (a.src < b.src) | ((a.src == b.src) & (a.isEdge < b.isEdge));
    });
    Integer w = undefinedInteger(16);
    for (int i = 0; i < nm; i++) {
        condAssign(~elements[i].isEdge, w, elements[i].weight);
        condAssign(elements[i].isEdge, elements[i].weight, w);
    }
    undoMergeElements(elements, trace);

    vector<Element> tmp;
    for (int i = 0; i < m; i++)
        tmp.push_back(elements[n + i]);
    permuteElements(tmp, perm);
    for (int i = 0; i < m; i++)
        elements[n + i] = tmp[i];
    trace = mergeElements(elements, n, [](const Element& a, const Element& b) {
        return (a.dst < b.dst) | ((a.dst == b.dst) & (a.isEdge > b.isEdge));
    });
    Integer zero = zeroInteger(16), sum = zero;
    for (int i = 0; i < nm; i++) {
        condAssign(elements[i].isEdge, sum, sum + elements[i].weight);
        condAssign(~elements[i].isEdge, elements[i].weight, sum);
        condAssign(~elements[i].isEdge, sum, zero);
    }
    undoMergeElements(elements, trace);

    tmp.clear();
    for (int i = 0; i < n; i++)
        tmp.push_back(elements[i]);
    permuteElements(tmp, permA);
    for (int i = 0; i < n; i++)
        elements[i] = tmp[i];

    for (int i = 0; i < n; i++) {
        vector<int> w = materialize(elements[i].weight);
        memcpy(outputs[i], w.data(), w.size() * sizeof(int));
    }

    return ::l;
}
