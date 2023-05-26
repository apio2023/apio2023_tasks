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


// Alice
int // returns la
alice(
    /*  in */ const int n,
    /*  in */ const char names[][5],
    /*  in */ const unsigned short numbers[],
    /* out */ bool outputs[]
) {
    // Ouput the number
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
    // Output m zeros
    return m;
}

static int l;
static int* operations;
static int (*operands)[2];
static int (*outputs)[16];

int makeGate(int op, int x0, int x1) {
    operations[l] = op;
    operands[l][0] = x0;
    operands[l][1] = x1;
    return l++;
}
int makeZero() {
    return makeGate(OP_ZERO, 0, 0);
}
vector<int> makeZeroes(int n) {
    vector<int> res;
    for (int i = 0; i < n; i++)
        res.push_back(makeZero());
    return res;
}

vector<int> add(const vector<int>& a, const vector<int>& b) {
    vector<int> res;
    int carry = makeZero();
    for (int i = 0; i < a.size(); i++) {
        int sum = makeGate(OP_XOR, a[i], b[i]);
        sum = makeGate(OP_XOR, sum, carry);
        res.push_back(sum);
        int ocarry = carry;
        carry = makeGate(OP_AND, a[i], b[i]);
        carry = makeGate(OP_OR, carry, makeGate(OP_AND, a[i], ocarry));
        carry = makeGate(OP_OR, carry, makeGate(OP_AND, b[i], ocarry));
    }
    return res;
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
    l = la + lb;
    ::operations = operations;
    ::operands = operands;
    ::outputs = outputs;
    vector<int> number;
    for (int i = 0; i < 16; i++)
        number.push_back(i);
    
    vector<int> ans = makeZeroes(16);
    for (int i = 0; i < lb; i++)
        ans = add(ans, number);
    
    for (int i = 0; i < 16; i++)
        outputs[0][i] = ans[i];

    return l;
}
