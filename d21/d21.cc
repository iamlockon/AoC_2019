#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <map>
#include <set>
#include <deque>
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
    return res;
}

void solution(const list_t &iv)
{
    // {
    //     IntCode *com = new IntCode(iv);
    //     string s = "NOT A J\nNOT C T\nAND D T\nOR T J\nWALK\n";
    //     list_t input = stringToInput(s);
    //     while (!com->isHalt())
    //     {
    //         cout << (char)com->seq_run(input);
    //     }

    //     cout << "Ans to part1 is: " << com->lastOut << endl;
    // }

    { // See: https://work.njae.me.uk/2020/01/07/advent-of-code-2019-day-21/
        IntCode *com2 = new IntCode(iv);
        string s = "NOT J J\nOR H T\nOR E T\nAND T J\nAND A T\n" \
                    "AND B T\nAND C T\nNOT T T\nAND T J\nAND D J\nRUN\n";
        list_t input = stringToInput(s);
        while (!com2->isHalt())
        {
            cout << (char)com2->seq_run(input);
        }
        cout << "Ans to part2 is: " <<  com2->lastOut << endl;
    }
}

int main()
{
    ifstream fin("./d21/input.txt");
    string ln;
    getline(fin, ln);
    list_t iv;
    stringstream ss(ln);
    string s;
    while (getline(ss, s, ','))
    {
        iv.push_back(stoi(s));
    }
    solution(iv);
    return 0;
}