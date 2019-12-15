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
    IMMEDIATE
};

typedef vector<vector<int>> myVec;

int intCode(vector<int> v, int input, int phase)
{
    int idx = 0;
    vector<int> res;
    while (idx < v.size())
    {
        const int opcode = v[idx] % 100;
        switch (opcode)
        {
        case 1:
        {
            if (idx + 3 >= v.size())
                return -2;
            const int pmodes = (v[idx] / 100);
            const int operand1 = (pmodes % 10 == POSITION) ? v[v[idx + 1]] : v[idx + 1];
            const int operand2 = ((pmodes % 100) / 10 == POSITION) ? v[v[idx + 2]] : v[idx + 2];
            v[v[idx + 3]] = operand1 + operand2;
            idx += 4;
            break;
        }
        case 2:
        {
            if (idx + 3 >= v.size())
                return -2;
            const int pmodes = (v[idx] / 100);
            const int operand1 = (pmodes % 10 == POSITION) ? v[v[idx + 1]] : v[idx + 1];
            const int operand2 = ((pmodes % 100) / 10 == POSITION) ? v[v[idx + 2]] : v[idx + 2];
            v[v[idx + 3]] = operand1 * operand2;
            idx += 4;
            break;
        }
        case 3:
        { // input opcode
            v[v[idx + 1]] = idx == 0 ? phase : input;
            idx += 2;
            break;
        }
        case 4:
        { // output opcode
            const int pmode = v[idx] / 100;
            const int operand1 = pmode == POSITION ? v[v[idx + 1]] : v[idx + 1];
            res.push_back(operand1);
            idx += 2;
            break;
        }
        case 5:
        { // jump-if-true
            const int pmods = v[idx] / 100;
            const int operand1 = (pmods % 10 == POSITION) ? v[v[idx + 1]] : v[idx + 1];
            const int operand2 = ((pmods % 100) / 10 == POSITION) ? v[v[idx + 2]] : v[idx + 2];
            if (operand1)
            {
                idx = operand2;
            }
            else
            {
                idx += 3;
            }
            break;
        }
        case 6:
        { // jump-if-false
            const int pmods = v[idx] / 100;
            const int operand1 = (pmods % 10 == POSITION) ? v[v[idx + 1]] : v[idx + 1];
            const int operand2 = ((pmods % 100) / 10 == POSITION) ? v[v[idx + 2]] : v[idx + 2];
            if (operand1 == 0)
            {
                idx = operand2;
            }
            else
            {
                idx += 3;
            }
            break;
        }
        case 7:
        { // less than
            const int pmods = v[idx] / 100;
            const int operand1 = (pmods % 10 == POSITION) ? v[v[idx + 1]] : v[idx + 1];
            const int operand2 = ((pmods % 100) / 10 == POSITION) ? v[v[idx + 2]] : v[idx + 2];
            v[v[idx + 3]] = (operand1 < operand2) ? 1 : 0;
            idx += 4;
            break;
        }
        case 8:
        { // equal
            const int pmods = v[idx] / 100;
            const int operand1 = (pmods % 10 == POSITION) ? v[v[idx + 1]] : v[idx + 1];
            const int operand2 = ((pmods % 100) / 10 == POSITION) ? v[v[idx + 2]] : v[idx + 2];
            v[v[idx + 3]] = (operand1 == operand2) ? 1 : 0;
            idx += 4;
            break;
        }
        case 99:
        {
            return res[res.size() - 1];
        }
        default:
        {
            return -1;
        }
        }
    }
    return 0;
}

class IntCode
{
private:
    int inst_ptr;
    int phase;
    bool init, halt;
    vector<int> program;
public:
    IntCode(int p, vector<int> vec) : phase(p), inst_ptr(0), init(false), halt(false), lastOut(0), program(vec){};
    int lastOut;
    bool isHalt() {
        return this->halt;
    }
    int run(int input)
    {
        vector<int> v = this->program;
        while (this->inst_ptr < v.size())
        {
            const int opcode = v[this->inst_ptr] % 100;
            switch (opcode)
            {
                case 1:
                {
                    if (this->inst_ptr + 3 >= v.size())
                        return -2;
                    const int pmodes = (v[this->inst_ptr] / 100);
                    const int operand1 = (pmodes % 10 == POSITION) ? v[v[this->inst_ptr + 1]] : v[this->inst_ptr + 1];
                    const int operand2 = ((pmodes % 100) / 10 == POSITION) ? v[v[this->inst_ptr + 2]] : v[this->inst_ptr + 2];
                    v[v[this->inst_ptr + 3]] = operand1 + operand2;
                    this->inst_ptr += 4;
                    break;
                }
                case 2:
                {
                    if (this->inst_ptr + 3 >= v.size())
                        return -2;
                    const int pmodes = (v[this->inst_ptr] / 100);
                    const int operand1 = (pmodes % 10 == POSITION) ? v[v[this->inst_ptr + 1]] : v[this->inst_ptr + 1];
                    const int operand2 = ((pmodes % 100) / 10 == POSITION) ? v[v[this->inst_ptr + 2]] : v[this->inst_ptr + 2];
                    v[v[this->inst_ptr + 3]] = operand1 * operand2;
                    this->inst_ptr += 4;
                    break;
                }
                case 3:
                { // input opcode
                    v[v[this->inst_ptr + 1]] = init == false ? this->phase : input;
                    if (init == false) init = true;
                    this->inst_ptr += 2;
                    break;
                }
                case 4:
                { // output opcode
                    const int pmode = v[this->inst_ptr] / 100;
                    const int operand1 = pmode == POSITION ? v[v[this->inst_ptr + 1]] : v[this->inst_ptr + 1];
                    this->inst_ptr += 2;
                    this->lastOut = operand1;
                    this->program = v; // store the program status.
                    return operand1;
                }
                case 5:
                { // jump-if-true
                    const int pmods = v[this->inst_ptr] / 100;
                    const int operand1 = (pmods % 10 == POSITION) ? v[v[this->inst_ptr + 1]] : v[this->inst_ptr + 1];
                    const int operand2 = ((pmods % 100) / 10 == POSITION) ? v[v[this->inst_ptr + 2]] : v[this->inst_ptr + 2];
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
                    const int pmods = v[this->inst_ptr] / 100;
                    const int operand1 = (pmods % 10 == POSITION) ? v[v[this->inst_ptr + 1]] : v[this->inst_ptr + 1];
                    const int operand2 = ((pmods % 100) / 10 == POSITION) ? v[v[this->inst_ptr + 2]] : v[this->inst_ptr + 2];
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
                    const int pmods = v[this->inst_ptr] / 100;
                    const int operand1 = (pmods % 10 == POSITION) ? v[v[this->inst_ptr + 1]] : v[this->inst_ptr + 1];
                    const int operand2 = ((pmods % 100) / 10 == POSITION) ? v[v[this->inst_ptr + 2]] : v[this->inst_ptr + 2];
                    v[v[this->inst_ptr + 3]] = (operand1 < operand2) ? 1 : 0;
                    this->inst_ptr += 4;
                    break;
                }
                case 8:
                { // equal
                    const int pmods = v[this->inst_ptr] / 100;
                    const int operand1 = (pmods % 10 == POSITION) ? v[v[this->inst_ptr + 1]] : v[this->inst_ptr + 1];
                    const int operand2 = ((pmods % 100) / 10 == POSITION) ? v[v[this->inst_ptr + 2]] : v[this->inst_ptr + 2];
                    v[v[this->inst_ptr + 3]] = (operand1 == operand2) ? 1 : 0;
                    this->inst_ptr += 4;
                    break;
                }
                case 99:
                {
                    cout << "halt.." << endl;
                    this->halt = true;
                    return this->lastOut;
                }
                default:
                {
                    cout << "unknown opcode..." << endl;
                    return -1;
                }
            }
        }
        cout << "unexpected instruction pointer out of bound: " << this->inst_ptr << endl;
        return 0;
    }
};


bool taken[5] = {};
int v[5] = {};
myVec set = myVec{};

void backtrack(int start, int end)
{
    if (start == end)
    {
        vector<int> vec = vector<int>{};
        for (auto i : v)
        {
            vec.push_back(i);
        }
        set.push_back(vec);
        return;
    }
    for (int i = 0; i < 5; i++)
    {
        if (!taken[i])
        {
            taken[i] = true;
            v[start] = i;
            backtrack(start + 1, end);
            taken[i] = false;
        }
    }
}

int part1(vector<int> v)
{
    // generate permutations (can also use std::next_permutation..)
    backtrack(0, 5);
    // now we have all permutations in set.
    int res = INT32_MIN;
    for (auto vect : set)
    {
        const int out1 = intCode(v, 0, vect[0]);
        const int out2 = intCode(v, out1, vect[1]);
        const int out3 = intCode(v, out2, vect[2]);
        const int out4 = intCode(v, out3, vect[3]);
        const int out5 = intCode(v, out4, vect[4]);
        res = std::max(res, out5);
        // cout << res << ":" << out5 << endl;
    }
    return res;
}

int part2(vector<int> v)
{
    backtrack(0, 5);
    // use previous permutations, but offset +5
    int res = INT32_MIN;
    // vector<vector<int>> set = vector<vector<int>>{{4,3,2,1,0}};
    for (auto vect: set) {
        IntCode *amp1 = new IntCode(vect[0]+5, v);
        IntCode *amp2 = new IntCode(vect[1]+5, v);
        IntCode *amp3 = new IntCode(vect[2]+5, v);
        IntCode *amp4 = new IntCode(vect[3]+5, v);
        IntCode *amp5 = new IntCode(vect[4]+5, v);

        int inp = 0; // initial input value
        do {
            int o1 = amp1->run(inp);
            int o2 = amp2->run(o1);
            int o3 = amp3->run(o2);
            int o4 = amp4->run(o3);
            int o5 = amp5->run(o4);
            if (amp5->isHalt()) break;
            inp = o5;
        } while (1);
    
        res = std::max(res, amp5->lastOut);
    }
    return res;
}

int main()
{
    ifstream fin("./d7/input.txt");
    string s;
    getline(fin, s);
    istringstream ss(s);
    vector<int> v;
    while (ss)
    {
        string a;
        if (!getline(ss, a, ','))
            break;
        v.push_back(stoi(a));
    }
    // now we have the program as a vector.
    cout << "Ans to part1 is: " << part1(v) << endl;
    cout << "Ans to part2 is: " << part2(v) << endl;
    return 0;
}