/*

stub.cpp

Compile: g++ stub.cpp abc.cpp -std=c++11 -o stub -O2
Run as alice: ./stub 0
Run as bob: ./stub 1
Run as circuit: ./stub 2

*/

#include "abc.h"
#include <bits/stdc++.h>
using namespace std;

static constexpr int T = 60;
static constexpr int MAX_N = 700;
static constexpr int MAX_M = 1000;
static constexpr int MAX_LA = 100000;
static constexpr int MAX_LB = 100000;
static constexpr int MAX_L = 20000000;
static constexpr double TL_PER_ALICE = 0.02;
static constexpr double TL_PER_BOB = 0.02;
static constexpr double TL_PER_CIRCUIT = 7;
static constexpr char CIRCUIT_WORKING_CORRECT_OUTPUT[] = "Circuit Output Finished.\n";

// you may find the definitions useful
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

using BS = uint64_t;

static BS operator""_bs(unsigned long long x) {
    return x;
}

struct WAException : public runtime_error {
    WAException(const string& msg) : runtime_error(msg) { }
};

struct TLEException : public runtime_error {
    TLEException(const string& msg) : runtime_error(msg) { }
};

struct FailException : public runtime_error {
    FailException(const string& msg) : runtime_error(msg) { }
};

template <typename F>
static int grader_entry(const F& f) {
    try {
        // ios::sync_with_stdio(false);
        // cin.tie(nullptr);
        stringstream fout;
        f(fout);
        // cerr << "Stub: grader_entry() finished, output:\n" << fout.str() << flush;
        cout << "235fc77fd78da210d6b5a61b92603c4f\n" << fout.str() << flush;
        return 0;
    } catch (WAException& e) {
        cout << "WA " << e.what() << endl;
        return 0;
    } catch (TLEException& e) {
        cout << "TLE " << e.what() << endl;
        return 0;
    } catch (FailException& e) {
        cout << "FAIL " << e.what() << endl;
        return 0;
    } catch (exception& e) {
        cout << "FAIL " << "unexpected exception: " << e.what() << endl;
        return 0;
    } catch (...) {
        cout << "FAIL " << "unknown exception" << endl;
        return 0;
    }
};

static bool isValidName(const string& s) {
    if (s.empty() || s.size() > 4)
        return false;
    for (char c : s)
        if (c < 'a' || c > 'z')
            return false;
    return true;
}

static void readOkOrThrow() {
    string s;
    cin >> s;
    if (s == "235fc77fd78da210d6b5a61b92603c4f")
        return;
    char c; cin.get(c);
    if (c != '\n')
        throw FailException("Expected newline after verdict");
    string msg;
    getline(cin, msg);
    if (s == "WA")
        throw WAException(msg);
    if (s == "TLE")
        throw TLEException(msg);
    if (s == "FAIL")
        throw FailException(msg);
    throw FailException("Unknown verdict: " + s + " " + msg);
}


static int alice() {
    cerr << "Stub: alice()" << endl;
    return grader_entry([&](stringstream& fout) {
        int n; cin >> n;
        cerr << "Stub: alice n=" << n << endl;
        if (n < 0 || n > MAX_N)
            throw FailException("Out of range N=" + to_string(n) + ". Please contact admin.");

        auto names = new char[n][5];
        auto numbers = new unsigned short[n];
        auto outputs = new bool[MAX_LA];

        for (int _ = 0; _ < T; _++) {
            memset(names, 0, n * 5);
            memset(numbers, 0, n * sizeof(unsigned short));
            memset(outputs, 0, MAX_LA);

            set<string> allNames;

            for (int i = 0; i < n; i++) {
                string name; unsigned short number;
                cin >> name >> number;
                // cerr << "Stub: alice read name=" << name << " number=" << number << endl;
                if (!isValidName(name))
                    throw FailException("Invalid name [" + name + "]. Please contact admin.");
                if (allNames.count(name))
                    throw FailException("Duplicate name [" + name + "]. Please contact admin.");
                allNames.insert(name);

                strcpy(names[i], name.c_str());
                numbers[i] = number;
            }

            auto start = clock();
            int la = alice(n, names, numbers, outputs);
            auto end = clock();

            if (la < 0 || la > MAX_LA)
                throw WAException("l_A (return value of alice()) out of range");

            double seconds = (end - start) / (double) CLOCKS_PER_SEC;
            if (seconds > TL_PER_ALICE)
                throw TLEException("alice() exceeds time limit");
            
            for (int i = 0; i < la; i++)
                fout << "01"[outputs[i]];
            fout << '\n';
        }
    });
}

static int bob() {
    cerr << "Stub: bob()" << endl;
    return grader_entry([&](stringstream& fout) {
        int m; cin >> m;
        cerr << "Stub: bob m=" << m << endl;
        if (m < 0 || m > MAX_M)
            throw FailException("Out of range M=" + to_string(m) + ". Please contact admin.");
        
        auto senders = new char[m][5];
        auto recipients = new char[m][5];
        auto outputs = new bool[MAX_LB];

        for (int _ = 0; _ < T; _++) {
            memset(senders, 0, m * 5);
            memset(recipients, 0, m * 5);
            memset(outputs, 0, MAX_LB);

            for (int i = 0; i < m; i++) {
                string sender, recipient;
                cin >> sender >> recipient;
                // cerr << "Stub: bob reads sender=" << sender << " recipient=" << recipient << endl;
                if (!isValidName(sender))
                    throw FailException("Invalid sender [" + sender + "]. Please contact admin.");
                if (!isValidName(recipient))
                    throw FailException("Invalid recipient [" + recipient + "]. Please contact admin.");

                strcpy(senders[i], sender.c_str());
                strcpy(recipients[i], recipient.c_str());
            }

            auto start = clock();
            int lb = bob(m, senders, recipients, outputs);
            auto end = clock();

            if (lb < 0 || lb > MAX_LB)
                throw WAException("l_B (return value of bob()) out of range");
            
            double seconds = (end - start) / (double) CLOCKS_PER_SEC;
            if (seconds > TL_PER_BOB)
                throw TLEException("bob() exceeds time limit");
            
            for (int i = 0; i < lb; i++)
                fout << "01"[outputs[i]];
            fout << '\n';
        }
    });
}

static int circuit() {
    cerr << "Stub: circuit()" << endl;
    return grader_entry([&](stringstream& fout) {
        int la, lb; cin >> la >> lb;
        cerr << "Stub: circuit la=" << la << " lb=" << lb << endl;
        if (la < 0 || la > MAX_LA)
            throw WAException("l_A (return value of alice()) out of range");
        if (lb < 0 || lb > MAX_LB)
            throw WAException("l_B (return value of bob()) out of range");
        
        auto operations = new int[MAX_L];
        auto operands = new int[MAX_L][2];
        auto outputs = new int[MAX_N][16];

        auto start = clock();
        int l = circuit(la, lb, operations, operands, outputs);
        auto end = clock();

        if (l < 0 || l > MAX_L)
            throw WAException("l (return value of circuit()) out of range");
        
        if (l < la + lb)
            throw WAException("Too small l (return value of circuit()), less than la + lb");
        
        double seconds = (end - start) / (double) CLOCKS_PER_SEC;
        if (seconds > TL_PER_CIRCUIT)
            throw TLEException("circuit() exceeds time limit");
        
        for (int i = la + lb; i < l; i++) {
            if (operations[i] < 0 || operations[i] > 15)
                throw WAException("circuit() returns invalid operation (<0 or >15)");
            if (operands[i][0] < 0 || operands[i][0] >= i)
                throw WAException("circuit() returns invalid operand (<0 or >=current label)");
            if (operands[i][1] < 0 || operands[i][1] >= i)
                throw WAException("circuit() returns invalid operand (<0 or >=current label)");
        }
        

        cout << CIRCUIT_WORKING_CORRECT_OUTPUT << flush;

        
        int n; cin >> n;
        if (n < 0 || n > MAX_N)
            throw FailException("Out of range N=" + to_string(n) + ". Please contact admin.");
        
        vector<vector<pair<string, unsigned short>>> alice_input;
        vector<set<string>> allNames(T);
        for (int _ = 0; _ < T; _++) {
            vector<pair<string, unsigned short>> input;
            set<string>& names = allNames[_];
            for (int i = 0; i < n; i++) {
                string name; unsigned short number;
                cin >> name >> number;
                if (!isValidName(name))
                    throw FailException("Invalid name [" + name + "]. Please contact admin.");
                if (names.count(name))
                    throw FailException("Duplicate name [" + name + "]. Please contact admin.");
                names.insert(name);
                input.emplace_back(name, number);
            }
            alice_input.push_back(input);
        }

        int m; cin >> m;
        if (m < 0 || m > MAX_M)
            throw FailException("Out of range M=" + to_string(m) + ". Please contact admin.");
        
        vector<vector<pair<string, string>>> bob_input;
        for (int _ = 0; _ < T; _++) {
            vector<pair<string, string>> input;
            set<string>& names = allNames[_];
            for (int i = 0; i < m; i++) {
                string sender, recipient;
                cin >> sender >> recipient;
                if (!names.count(sender))
                    throw FailException("Unknown sender [" + sender + "]. Please contact admin.");
                if (!names.count(recipient))
                    throw FailException("Unknown recipient [" + recipient + "]. Please contact admin.");
                input.emplace_back(sender, recipient);
            }
            bob_input.push_back(input);
        }

        readOkOrThrow();
        char c; cin.get(c);
        if (c != '\n')
            throw FailException("Expected newline. Please contact admin.");
        vector<string> alice_output;
        for (int _ = 0; _ < T; _++) {
            string s; getline(cin, s);
            // cerr << "Stub: circuit read alice_output=" << s << endl;
            if (s.size() > MAX_LA)
                throw WAException("l_A (return value of alice()) out of range");
            for (char c : s)
                if (c != '0' && c != '1')
                    throw FailException("Invalid Alice output. Please contact admin.");
            if (_ && s.size() != alice_output[0].size())
                throw WAException("l_A (return value of alice()) is not fixed when n is fixed");
            alice_output.push_back(s);
        }

        readOkOrThrow();
        cin.get(c);
        if (c != '\n')
            throw FailException("Expected newline. Please contact admin.");
        vector<string> bob_output;
        for (int _ = 0; _ < T; _++) {
            string s; getline(cin, s);
            // cerr << "Stub: circuit read bob_output=" << s << endl;
            if (s.size() > MAX_LB)
                throw WAException("l_B (return value of bob()) out of range");
            for (char c : s)
                if (c != '0' && c != '1')
                    throw FailException("Invalid Bob output. Please contact admin.");
            if (_ && s.size() != bob_output[0].size())
                throw WAException("l_B (return value of bob()) is not fixed when m is fixed");
            bob_output.push_back(s);
        }
        
        for (int i = la + lb; i < l; i++) {
            if (operations[i] < 0 || operations[i] > 15)
                throw WAException("circuit() returns invalid operation (<0 or >15)");
            if (operands[i][0] < 0 || operands[i][0] >= i)
                throw WAException("circuit() returns invalid operand (<0 or >=current label)");
            if (operands[i][1] < 0 || operands[i][1] >= i)
                throw WAException("circuit() returns invalid operand (<0 or >=current label)");
        }
        for (int i = 0; i < n; i++)
            for (int j = 0; j < 16; j++)
                if (outputs[i][j] < 0 || outputs[i][j] >= l)
                    throw WAException("circuit() returns invalid circuit_output (<0 or >=l)");

        auto values = new BS[l];
        for (int i = 0; i < la; i++) {
            BS bs = 0_bs;
            for (int j = 0; j < T; j++)
                bs |= (BS(alice_output[j][i] - '0') << j);
            values[i] = bs;
        }
        for (int i = 0; i < lb; i++) {
            BS bs = 0_bs;
            for (int j = 0; j < T; j++)
                bs |= (BS(bob_output[j][i] - '0') << j);
            values[la + i] = bs;
        }
        for (int i = la + lb; i < l; i++) {
            BS bs0 = values[operands[i][0]];
            BS bs1 = values[operands[i][1]];
            BS& bs = values[i];
            switch (operations[i]) {
                case OP_ZERO:
                    bs = 0_bs;
                    break;
                case OP_NOR:
                    bs = ~(bs0 | bs1);
                    break;
                case OP_GREATER:
                    bs = bs0 & ~bs1;
                    break;
                case OP_NOT_X1:
                    bs = ~bs1;
                    break;
                case OP_LESS:
                    bs = ~bs0 & bs1;
                    break;
                case OP_NOT_X0:
                    bs = ~bs0;
                    break;
                case OP_XOR:
                    bs = bs0 ^ bs1;
                    break;
                case OP_NAND:
                    bs = ~(bs0 & bs1);
                    break;
                case OP_AND:
                    bs = bs0 & bs1;
                    break;
                case OP_EQUAL:
                    bs = ~(bs0 ^ bs1);
                    break;
                case OP_X0:
                    bs = bs0;
                    break;
                case OP_GEQ:
                    bs = ~(~bs0 & bs1);
                    break;
                case OP_X1:
                    bs = bs1;
                    break;
                case OP_LEQ:
                    bs = ~(bs0 & ~bs1);
                    break;
                case OP_OR:
                    bs = bs0 | bs1;
                    break;
                case OP_ONE:
                    bs = (1_bs << T) - 1_bs;
                    break;
                default:
                    throw WAException("circuit() returns invalid operation (<0 or >15)");
            }
        }

        vector<vector<unsigned short>> circuit_output(T, vector<unsigned short>(n));
        for (int i = 0; i < n; i++)
            for (int j = 0; j < 16; j++)
                for (int k = 0; k < T; k++) {
                    circuit_output[k][i] |= (unsigned short) ((values[outputs[i][j]] >> k) & 1_bs) << j;
                }
        
        vector<vector<unsigned short>> reference_output;
        for (int _ = 0; _ < T; _++) {
            vector<unsigned short> output(n);
            auto& alice = alice_input[_];
            auto& bob = bob_input[_];
            map<string, unsigned short> values;
            map<string, unsigned short> answer;
            for (const auto& p : alice)
                values[p.first] = p.second;
            for (const auto& p : bob)
                answer[p.second] += values[p.first];
            for (int i = 0; i < n; i++)
                output[i] = answer[alice[i].first];
            reference_output.push_back(output);
        }

        auto vector_to_string = [&](const vector<unsigned short>& v) {
            string s;
            for (int i = 0; i < n; i++)
                s += to_string(v[i]) + " ";
            return s;
        };

        for (int _ = 0; _ < T; _++)
            if (circuit_output[_] != reference_output[_])
                throw WAException("Your functions alice(), bob(), circuit() finished successfully, but the final output binary string is incorrect.");
        
        fout << "Correct!\n";
    });
}

int main(int argc, char** argv) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " [0|1|2]" << endl;
        return 0;
    }
    string s; cin >> s;
    if (s != "Hello") {
        if(s == "Goodbye") cerr << "Fail in previous run, exit now" << endl;
        else cout << "FAIL\nDid not receive Hello from manager" << endl;
        return 0;
    }
    int mode = atoi(argv[1]);
    switch (mode) {
        case 0:
            return alice();
            break;
        case 1:
            return bob();
            break;
        case 2:
            return circuit();
            break;
        default:
            cerr << "Usage: " << argv[0] << " [0|1|2]" << endl;
            return 0;
    }
    cin >> s;
    if (s != "Goodbye") {
        cout << "FAIL\nDid not receive Goodbye from manager" << endl;
        return 0;
    }
    return 0;
}
