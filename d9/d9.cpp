#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
using namespace std;

enum P_mode
{
    POSITION,
    IMMEDIATE,
    RELATIVE
};

typedef long long myNum;

class IntCode
{
private:
    myNum inst_ptr;
    bool halt;
    vector<myNum> program;
    myNum rel_base;
public:
    IntCode(vector<myNum> vec) : inst_ptr(0), halt(false), lastOut(0), program(vec), rel_base(0){};
    myNum lastOut;
    bool isHalt() {
        return this->halt;
    }

    myNum run(vector<myNum> v, myNum input = 0)
    {
        v.resize(v.size() * 10, 0);
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
                    const myNum operand2 = ((pmodes % 100) / 10 == POSITION) ? v[v[this->inst_ptr + 2]] : (((pmodes % 100) / 10 == IMMEDIATE) ? v[this->inst_ptr + 2]  : v[v[this->inst_ptr + 2] + rel_base]);
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
                    const myNum operand2 = ((pmodes % 100) / 10 == POSITION) ? v[v[this->inst_ptr + 2]] : (((pmodes % 100) / 10 == IMMEDIATE) ? v[this->inst_ptr + 2]  : v[v[this->inst_ptr + 2] + rel_base]);
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
                    cout << operand1 << ", ";
                    this->lastOut = operand1;
                    break;
                }
                case 5:
                { // jump-if-true
                    const myNum pmodes = v[this->inst_ptr] / 100;
                    const myNum operand1 = (pmodes % 10 == POSITION) ? v[v[this->inst_ptr + 1]] : ((pmodes % 10 == IMMEDIATE) ? v[this->inst_ptr + 1] : v[v[this->inst_ptr + 1] + rel_base]);
                    const myNum operand2 = ((pmodes % 100) / 10 == POSITION) ? v[v[this->inst_ptr + 2]] : (((pmodes % 100) / 10 == IMMEDIATE) ? v[this->inst_ptr + 2]  : v[v[this->inst_ptr + 2] + rel_base]);
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
                    const myNum operand2 = ((pmodes % 100) / 10 == POSITION) ? v[v[this->inst_ptr + 2]] : (((pmodes % 100) / 10 == IMMEDIATE) ? v[this->inst_ptr + 2]  : v[v[this->inst_ptr + 2] + rel_base]);
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
                    const myNum operand2 = ((pmodes % 100) / 10 == POSITION) ? v[v[this->inst_ptr + 2]] : (((pmodes % 100) / 10 == IMMEDIATE) ? v[this->inst_ptr + 2]  : v[v[this->inst_ptr + 2] + rel_base]);
                    const myNum operand3 = (pmodes / 100 == POSITION) ? v[this->inst_ptr + 3] : v[this->inst_ptr + 3] + rel_base;
                    v[operand3] = (operand1 < operand2) ? 1 : 0;
                    this->inst_ptr += 4;
                    break;
                }
                case 8:
                { // equal
                    const myNum pmodes = v[this->inst_ptr] / 100;
                    const myNum operand1 = (pmodes % 10 == POSITION) ? v[v[this->inst_ptr + 1]] : ((pmodes % 10 == IMMEDIATE) ? v[this->inst_ptr + 1] : v[v[this->inst_ptr + 1] + rel_base]);
                    const myNum operand2 = ((pmodes % 100) / 10 == POSITION) ? v[v[this->inst_ptr + 2]] : (((pmodes % 100) / 10 == IMMEDIATE) ? v[this->inst_ptr + 2]  : v[v[this->inst_ptr + 2] + rel_base]);
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
        cout << "unexpected instruction pointer out of bound: " << this->inst_ptr << endl;
        return 0;
    }
};

myNum part1(vector<myNum> v) {
    IntCode *comp = new IntCode(v);
    try {
        const myNum res = comp->run(v, 1);
        return res;
    } catch(int e) {
        cout << e << endl;
    }

}

myNum part2(vector<myNum> v) {
    IntCode *comp = new IntCode(v);
    try {
        const myNum res = comp->run(v, 2);
        return res;
    } catch(int e) {
        cout << e << endl;
    }

}

int main()
{
    ifstream fin("./input.txt");
    string s;
    getline(fin, s);
    istringstream ss(s);
    vector<myNum> v;
    while (ss)
    {
        string a;
        if (!getline(ss, a, ','))
            break;
        v.push_back(stoll(a));
    }
    // now we have the program as a vector.
    part1(v);
    part2(v);
    // cout << "Ans to part1 is: " << part1(v) << endl;
    // cout << "Ans to part2 is: " << part2(v) << endl;
    return 0;
}