#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <map>
#include <set>
#include <deque>
#include <functional>
using namespace std;

enum P_mode
{
    POSITION,
    IMMEDIATE,
    RELATIVE
};

typedef long long myNum;

using list_t = vector<myNum>;
using Map = vector<vector<myNum>>;

struct Point
{
    int type;
    pair<myNum, myNum> position;
};

typedef pair<myNum, myNum> Pos;

class IntCode
{
private:
    myNum inst_ptr;
    bool halt;
    vector<myNum> program;
    myNum rel_base;

public:
    IntCode(vector<myNum> vec) : inst_ptr(0), halt(false), lastOut(0), rel_base(0)
    {
        program = vec;
        program.resize(vec.size() * 10);
    };
    myNum lastOut;
    bool isHalt()
    {
        return this->halt;
    }

    myNum run(myNum input = 0)
    {
        vector<myNum> v = this->program;
        while (this->inst_ptr < v.size())
        {
            const myNum opcode = v[this->inst_ptr] % 100;
            switch (opcode)
            {
            case 1:
            {
                if (this->inst_ptr + 3 >= v.size())
                    return -2;
                const myNum pmodes = (v[this->inst_ptr] / 100);
                const myNum operand1 = (pmodes % 10 == POSITION) ? v[v[this->inst_ptr + 1]] : ((pmodes % 10 == IMMEDIATE) ? v[this->inst_ptr + 1] : v[v[this->inst_ptr + 1] + rel_base]);
                const myNum operand2 = ((pmodes % 100) / 10 == POSITION) ? v[v[this->inst_ptr + 2]] : (((pmodes % 100) / 10 == IMMEDIATE) ? v[this->inst_ptr + 2] : v[v[this->inst_ptr + 2] + rel_base]);
                const myNum operand3 = (pmodes / 100 == POSITION) ? v[this->inst_ptr + 3] : v[this->inst_ptr + 3] + rel_base;
                v[operand3] = operand1 + operand2;
                this->inst_ptr += 4;
                break;
            }
            case 2:
            {
                if (this->inst_ptr + 3 >= v.size())
                    return -2;
                const myNum pmodes = (v[this->inst_ptr] / 100);
                const myNum operand1 = (pmodes % 10 == POSITION) ? v[v[this->inst_ptr + 1]] : ((pmodes % 10 == IMMEDIATE) ? v[this->inst_ptr + 1] : v[v[this->inst_ptr + 1] + rel_base]);
                const myNum operand2 = ((pmodes % 100) / 10 == POSITION) ? v[v[this->inst_ptr + 2]] : (((pmodes % 100) / 10 == IMMEDIATE) ? v[this->inst_ptr + 2] : v[v[this->inst_ptr + 2] + rel_base]);
                const myNum operand3 = (pmodes / 100 == POSITION) ? v[this->inst_ptr + 3] : v[this->inst_ptr + 3] + rel_base;
                v[operand3] = operand1 * operand2;
                this->inst_ptr += 4;
                break;
            }
            case 3:
            { // input opcode
                const myNum pmodes = (v[this->inst_ptr] / 100);
                const myNum operand1 = (pmodes == POSITION) ? v[this->inst_ptr + 1] : v[this->inst_ptr + 1] + rel_base;
                v[operand1] = input;
                this->inst_ptr += 2;
                break;
            }
            case 4:
            { // output opcode
                const myNum pmode = v[this->inst_ptr] / 100;
                const myNum operand1 = (pmode == POSITION) ? v[v[this->inst_ptr + 1]] : ((pmode == IMMEDIATE) ? v[this->inst_ptr + 1] : v[v[this->inst_ptr + 1] + rel_base]);
                this->inst_ptr += 2;
                this->lastOut = operand1;
                this->program = v; // store the program state.
                return operand1;
            }
            case 5:
            { // jump-if-true
                const myNum pmodes = v[this->inst_ptr] / 100;
                const myNum operand1 = (pmodes % 10 == POSITION) ? v[v[this->inst_ptr + 1]] : ((pmodes % 10 == IMMEDIATE) ? v[this->inst_ptr + 1] : v[v[this->inst_ptr + 1] + rel_base]);
                const myNum operand2 = ((pmodes % 100) / 10 == POSITION) ? v[v[this->inst_ptr + 2]] : (((pmodes % 100) / 10 == IMMEDIATE) ? v[this->inst_ptr + 2] : v[v[this->inst_ptr + 2] + rel_base]);
                if (operand1)
                {
                    this->inst_ptr = operand2;
                }
                else
                {
                    this->inst_ptr += 3;
                }
                break;
            }
            case 6:
            { // jump-if-false
                const myNum pmodes = v[this->inst_ptr] / 100;
                const myNum operand1 = (pmodes % 10 == POSITION) ? v[v[this->inst_ptr + 1]] : ((pmodes % 10 == IMMEDIATE) ? v[this->inst_ptr + 1] : v[v[this->inst_ptr + 1] + rel_base]);
                const myNum operand2 = ((pmodes % 100) / 10 == POSITION) ? v[v[this->inst_ptr + 2]] : (((pmodes % 100) / 10 == IMMEDIATE) ? v[this->inst_ptr + 2] : v[v[this->inst_ptr + 2] + rel_base]);
                if (operand1 == 0)
                {
                    this->inst_ptr = operand2;
                }
                else
                {
                    this->inst_ptr += 3;
                }
                break;
            }
            case 7:
            { // less than
                const myNum pmodes = v[this->inst_ptr] / 100;
                const myNum operand1 = (pmodes % 10 == POSITION) ? v[v[this->inst_ptr + 1]] : ((pmodes % 10 == IMMEDIATE) ? v[this->inst_ptr + 1] : v[v[this->inst_ptr + 1] + rel_base]);
                const myNum operand2 = ((pmodes % 100) / 10 == POSITION) ? v[v[this->inst_ptr + 2]] : (((pmodes % 100) / 10 == IMMEDIATE) ? v[this->inst_ptr + 2] : v[v[this->inst_ptr + 2] + rel_base]);
                const myNum operand3 = (pmodes / 100 == POSITION) ? v[this->inst_ptr + 3] : v[this->inst_ptr + 3] + rel_base;
                v[operand3] = (operand1 < operand2) ? 1 : 0;
                this->inst_ptr += 4;
                break;
            }
            case 8:
            { // equal
                const myNum pmodes = v[this->inst_ptr] / 100;
                const myNum operand1 = (pmodes % 10 == POSITION) ? v[v[this->inst_ptr + 1]] : ((pmodes % 10 == IMMEDIATE) ? v[this->inst_ptr + 1] : v[v[this->inst_ptr + 1] + rel_base]);
                const myNum operand2 = ((pmodes % 100) / 10 == POSITION) ? v[v[this->inst_ptr + 2]] : (((pmodes % 100) / 10 == IMMEDIATE) ? v[this->inst_ptr + 2] : v[v[this->inst_ptr + 2] + rel_base]);
                const myNum operand3 = (pmodes / 100 == POSITION) ? v[this->inst_ptr + 3] : v[this->inst_ptr + 3] + rel_base;
                v[operand3] = (operand1 == operand2) ? 1 : 0;
                this->inst_ptr += 4;
                break;
            }
            case 9:
            {
                const myNum pmodes = v[this->inst_ptr] / 100;
                const myNum operand1 = (pmodes % 10 == POSITION) ? v[v[this->inst_ptr + 1]] : ((pmodes % 10 == IMMEDIATE) ? v[this->inst_ptr + 1] : v[v[this->inst_ptr + 1] + rel_base]);
                this->rel_base += operand1;
                this->inst_ptr += 2;
                break;
            }
            case 99:
            {
                this->halt = true;
                return this->lastOut;
            }
            default:
            {
                cout << "unknown opcode..." << endl;
            }
            }
        }
        throw "Error";
        cout << "unexpected instruction pointer out of bound: " << this->inst_ptr << endl;
        return 0;
    }
    myNum seq_run(list_t input)
    {
        vector<myNum> v = this->program;
        while (this->inst_ptr < v.size())
        {
            const myNum opcode = v[this->inst_ptr] % 100;
            switch (opcode)
            {
            case 1:
            {
                if (this->inst_ptr + 3 >= v.size())
                    return -2;
                const myNum pmodes = (v[this->inst_ptr] / 100);
                const myNum operand1 = (pmodes % 10 == POSITION) ? v[v[this->inst_ptr + 1]] : ((pmodes % 10 == IMMEDIATE) ? v[this->inst_ptr + 1] : v[v[this->inst_ptr + 1] + rel_base]);
                const myNum operand2 = ((pmodes % 100) / 10 == POSITION) ? v[v[this->inst_ptr + 2]] : (((pmodes % 100) / 10 == IMMEDIATE) ? v[this->inst_ptr + 2] : v[v[this->inst_ptr + 2] + rel_base]);
                const myNum operand3 = (pmodes / 100 == POSITION) ? v[this->inst_ptr + 3] : v[this->inst_ptr + 3] + rel_base;
                v[operand3] = operand1 + operand2;
                this->inst_ptr += 4;
                break;
            }
            case 2:
            {
                if (this->inst_ptr + 3 >= v.size())
                    return -2;
                const myNum pmodes = (v[this->inst_ptr] / 100);
                const myNum operand1 = (pmodes % 10 == POSITION) ? v[v[this->inst_ptr + 1]] : ((pmodes % 10 == IMMEDIATE) ? v[this->inst_ptr + 1] : v[v[this->inst_ptr + 1] + rel_base]);
                const myNum operand2 = ((pmodes % 100) / 10 == POSITION) ? v[v[this->inst_ptr + 2]] : (((pmodes % 100) / 10 == IMMEDIATE) ? v[this->inst_ptr + 2] : v[v[this->inst_ptr + 2] + rel_base]);
                const myNum operand3 = (pmodes / 100 == POSITION) ? v[this->inst_ptr + 3] : v[this->inst_ptr + 3] + rel_base;
                v[operand3] = operand1 * operand2;
                this->inst_ptr += 4;
                break;
            }
            case 3:
            { // input opcode
                const myNum pmodes = (v[this->inst_ptr] / 100);
                const myNum operand1 = (pmodes == POSITION) ? v[this->inst_ptr + 1] : v[this->inst_ptr + 1] + rel_base;
                if (input.size() == 0) {
                    this->program = v;
                    return 0;
                }
                v[operand1] = input.front();
                input.erase(input.begin());
                this->inst_ptr += 2;
                break;
            }
            case 4:
            { // output opcode
                const myNum pmode = v[this->inst_ptr] / 100;
                const myNum operand1 = (pmode == POSITION) ? v[v[this->inst_ptr + 1]] : ((pmode == IMMEDIATE) ? v[this->inst_ptr + 1] : v[v[this->inst_ptr + 1] + rel_base]);
                this->inst_ptr += 2;
                this->lastOut = operand1;
                this->program = v; // store the program state.
                // return operand1;
                if (operand1 < 256)
                    cout << (char)operand1;
                continue;
            }
            case 5:
            { // jump-if-true
                const myNum pmodes = v[this->inst_ptr] / 100;
                const myNum operand1 = (pmodes % 10 == POSITION) ? v[v[this->inst_ptr + 1]] : ((pmodes % 10 == IMMEDIATE) ? v[this->inst_ptr + 1] : v[v[this->inst_ptr + 1] + rel_base]);
                const myNum operand2 = ((pmodes % 100) / 10 == POSITION) ? v[v[this->inst_ptr + 2]] : (((pmodes % 100) / 10 == IMMEDIATE) ? v[this->inst_ptr + 2] : v[v[this->inst_ptr + 2] + rel_base]);
                if (operand1)
                {
                    this->inst_ptr = operand2;
                }
                else
                {
                    this->inst_ptr += 3;
                }
                break;
            }
            case 6:
            { // jump-if-false
                const myNum pmodes = v[this->inst_ptr] / 100;
                const myNum operand1 = (pmodes % 10 == POSITION) ? v[v[this->inst_ptr + 1]] : ((pmodes % 10 == IMMEDIATE) ? v[this->inst_ptr + 1] : v[v[this->inst_ptr + 1] + rel_base]);
                const myNum operand2 = ((pmodes % 100) / 10 == POSITION) ? v[v[this->inst_ptr + 2]] : (((pmodes % 100) / 10 == IMMEDIATE) ? v[this->inst_ptr + 2] : v[v[this->inst_ptr + 2] + rel_base]);
                if (operand1 == 0)
                {
                    this->inst_ptr = operand2;
                }
                else
                {
                    this->inst_ptr += 3;
                }
                break;
            }
            case 7:
            { // less than
                const myNum pmodes = v[this->inst_ptr] / 100;
                const myNum operand1 = (pmodes % 10 == POSITION) ? v[v[this->inst_ptr + 1]] : ((pmodes % 10 == IMMEDIATE) ? v[this->inst_ptr + 1] : v[v[this->inst_ptr + 1] + rel_base]);
                const myNum operand2 = ((pmodes % 100) / 10 == POSITION) ? v[v[this->inst_ptr + 2]] : (((pmodes % 100) / 10 == IMMEDIATE) ? v[this->inst_ptr + 2] : v[v[this->inst_ptr + 2] + rel_base]);
                const myNum operand3 = (pmodes / 100 == POSITION) ? v[this->inst_ptr + 3] : v[this->inst_ptr + 3] + rel_base;
                v[operand3] = (operand1 < operand2) ? 1 : 0;
                this->inst_ptr += 4;
                break;
            }
            case 8:
            { // equal
                const myNum pmodes = v[this->inst_ptr] / 100;
                const myNum operand1 = (pmodes % 10 == POSITION) ? v[v[this->inst_ptr + 1]] : ((pmodes % 10 == IMMEDIATE) ? v[this->inst_ptr + 1] : v[v[this->inst_ptr + 1] + rel_base]);
                const myNum operand2 = ((pmodes % 100) / 10 == POSITION) ? v[v[this->inst_ptr + 2]] : (((pmodes % 100) / 10 == IMMEDIATE) ? v[this->inst_ptr + 2] : v[v[this->inst_ptr + 2] + rel_base]);
                const myNum operand3 = (pmodes / 100 == POSITION) ? v[this->inst_ptr + 3] : v[this->inst_ptr + 3] + rel_base;
                v[operand3] = (operand1 == operand2) ? 1 : 0;
                this->inst_ptr += 4;
                break;
            }
            case 9:
            {
                const myNum pmodes = v[this->inst_ptr] / 100;
                const myNum operand1 = (pmodes % 10 == POSITION) ? v[v[this->inst_ptr + 1]] : ((pmodes % 10 == IMMEDIATE) ? v[this->inst_ptr + 1] : v[v[this->inst_ptr + 1] + rel_base]);
                this->rel_base += operand1;
                this->inst_ptr += 2;
                break;
            }
            case 99:
            {
                this->halt = true;
                return this->lastOut;
            }
            default:
            {
                cout << "unknown opcode..." << endl;
            }
            }
        }
        throw "Error";
        cout << "unexpected instruction pointer out of bound: " << this->inst_ptr << endl;
        return 0;
    }
};

list_t stringToInput(string s)
{
    list_t res;
    for (const auto &c : s)
    {
        res.push_back((int)c);
    }
    res.push_back(10);
    return res;
}

void solution(const list_t &iv)
{
    {
        IntCode *com = new IntCode(iv);
        cout << "Please, command?" << endl;
        while(!com->isHalt()) {
            string ss;
            cout << endl;
            getline(cin, ss);
            if (ss == "GO") {
                auto trick = vector<string>{
                    "west",
                    "take hologram",
                    "north",
                    "take space heater",
                    "east",
                    "take space law space brochure",
                    "east",
                    "take tambourine",
                    "west",
                    "west",
                    "south",
                    "east",
                    "east",
                    "take festive hat", // CREW QTRS
                    "east",
                    "take food ration",
                    "east",
                    "take spool of cat6",
                    "west",
                    "west",
                    "south",
                    "east",
                    "east",
                    "east",
                    "inv",
                    "drop space heater",
                    "drop hologram",
                    "drop space law space brochure",
                    "drop food ration",
                    "drop tambourine",
                    "drop spool of cat6",
                    "drop festive hat"
                };
                for (const auto &s: trick) com->seq_run(stringToInput(s));
                const array<string,7> elem =  {"space heater", "hologram", "space law space brochure",
                "food ration", "tambourine", "spool of cat6", "festive hat"};
                function<void(int, int, vector<string>&,vector<vector<string>>&)> backtrack = [&](int offset, int take, vector<string> &comb, vector<vector<string>> &res) {
                    if (take == 0) {
                        res.push_back(comb);
                        return;
                    }
                    for (int i = offset; i <= 6; i++) {
                        comb.push_back(elem[i]);
                        backtrack(i+1, take-1, comb, res);
                        comb.pop_back();
                    }
                };

                auto genComb = [&](int i) {
                    vector<vector<string>> res;
                    vector<string> comb;
                    backtrack(0, i, comb, res);
                    return res;
                };
                auto getAllComb = [&](const array<string,7> &elem) {
                    vector<string> res;
                    for (int i = 1; i <= 7; i++) { // from 1 item to 7 items
                        auto r = genComb(i);
                        for (const auto &item: r) {
                            for (const auto &s: item) res.push_back("take "+ s);
                            res.push_back("south"); // test
                            for (const auto &s: item) res.push_back("drop "+ s);
                        }
                    }
                    return res;
                };
                const vector<string> allCombinations = getAllComb(elem);
                for (const auto &s: allCombinations) com->seq_run(stringToInput(s));
                continue;
            }
            com->seq_run(stringToInput(ss));
        }
    }
}

int main()
{
    ifstream fin("./d25/input.txt");
    string ln;
    getline(fin, ln);
    list_t iv;
    stringstream ss(ln);
    string s;
    while (getline(ss, s, ','))
    {
        iv.push_back(stoll(s));
    }
    solution(iv);
    return 0;
}