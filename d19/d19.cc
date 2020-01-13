#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <array>
using namespace std;


enum P_mode
{
    POSITION,
    IMMEDIATE,
    RELATIVE
};

typedef long long myNum;
struct Point {
    int type;
    pair<myNum,myNum> position;
};

class IntCode
{
private:
    myNum inst_ptr;
    bool halt;
    vector<myNum> program;
    myNum rel_base;
public:
    IntCode(vector<myNum> vec) : inst_ptr(0), halt(false), lastOut(0), rel_base(0){
        program = vec;
        program.resize(vec.size() * 10);
    };
    myNum lastOut;
    bool isHalt() {
        return this->halt;
    }

    myNum run(myNum x, myNum y)
    {
        vector<myNum> v = this->program;
        inst_ptr = 0;
        bool first = true;
        rel_base = 0;
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
                    v[operand1] = first ? x : y;
                    this->inst_ptr += 2;
                    first = false;
                    break;
                }
                case 4:
                { // output opcode
                    const myNum pmode = v[this->inst_ptr] / 100;
                    const myNum operand1 = (pmode == POSITION) ? v[v[this->inst_ptr + 1]] : ((pmode == IMMEDIATE) ? v[this->inst_ptr + 1] : v[v[this->inst_ptr + 1] + rel_base]);
                    this->inst_ptr += 2;
                    this->lastOut = operand1;
                    // this->program = v; // store the program state.
                    return operand1;
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
        throw 'Error';
        cout << "unexpected instruction pointer out of bound: " << this->inst_ptr << endl;
        return 0;
    }
};

void part1(vector<myNum> program) {
    IntCode *comp = new IntCode(program);
    array<array<int,50>, 50> map;
    myNum count = 0;
    for (int i = 0; i < 50; i++) {
        for (int j = 0; j < 50; j++) {
            map[i][j] = comp->run(j, i);
            if (map[i][j] == 1) count++;
        }
    }
    cout << "Ans to part1: " << count << endl;
    for (auto i: map) {
        for (auto j: i) {
            cout << j;
        }
        cout << endl;
    }
}

void part2(vector<myNum> program) {
    IntCode *comp = new IntCode(program);
    array<int,2> curr = {3, 5};
    while (comp->run(curr[0]+1, curr[1])) {
        curr[0]++;
    };
    auto checkCoverage = [&](array<int,2> edge) {
        if (edge[0] - 99 < 0) return false;
        return comp->run(edge[0]-99, edge[1]+99) == 1;
    };

    while(!checkCoverage(curr)) {
        // Increment y
        curr[1]++;
        // Chances that it's 0
        if (comp->run(curr[0], curr[1]) == 0) {
            array<int,2> leftright = {curr[0]-1, curr[0]+1};
            while (!comp->run(leftright[0], curr[1]) &&
                    !comp->run(leftright[1], curr[1])) {
                leftright[0]--;leftright[1]++;
            }
            curr[0] = comp->run(leftright[0], curr[1]) ? 
                leftright[0] : leftright[1];
        }
        // Find right boundary in this row
        while (comp->run(curr[0]+1, curr[1])) {
            curr[0]++;
        }
    };
    bool resu = checkCoverage(curr);
    cout << "Ans to part 2: "<< (curr[0]-99) * 10000 + curr[1] << endl;
}

int main() {
    ifstream fin("./d19/input.txt");
    string line;
    getline(fin, line);
    stringstream ss(line);
    string s;
    vector<myNum> prog;
    while (getline(ss, s, ',')) {
        prog.push_back(stoi(s));
    }
    part1(prog);
    part2(prog);
}