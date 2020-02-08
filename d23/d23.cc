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

typedef pair<myNum, myNum> Pos;

const int DEV_NUM = 50;

class IntCode
{
private:
    myNum inst_ptr;
    bool halt;
    bool isInit;
    vector<myNum> program;
    myNum rel_base;

public:
    IntCode(vector<myNum> vec) : inst_ptr(0), halt(false), lastOut(0), rel_base(0)
    {
        program = vec;
        program.resize(vec.size() * 10);
        isInit = true;
    };
    myNum lastOut;
    bool isHalt()
    {
        return this->halt;
    }

    myNum run(myNum addr, array<deque<myNum>,300> &queue)
    {
        vector<myNum> v = this->program;
        myNum currOutAddr;
        myNum ctr = -1;
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
                v[operand1] = isInit ? addr : (queue[addr].size() > 0 ? queue[addr].back() : -1);
                if (isInit) isInit = false;
                if (queue[addr].size() > 0) queue[addr].pop_back();
                this->inst_ptr += 2;
                this->program = v;
                return v[operand1]; // just return after input.
            }
            case 4:
            { // output opcode
                const myNum pmode = v[this->inst_ptr] / 100;
                const myNum operand1 = (pmode == POSITION) ? v[v[this->inst_ptr + 1]] : ((pmode == IMMEDIATE) ? v[this->inst_ptr + 1] : v[v[this->inst_ptr + 1] + rel_base]);
                this->inst_ptr += 2;
                this->lastOut = operand1;
                ctr++;
                if (ctr % 3 == 0) currOutAddr = operand1;
                else queue[currOutAddr].push_front(operand1);
                this->program = v; // store the program state.
                if (ctr % 3 == 2) return currOutAddr; // just return after three outputs
                break;
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
    vector<IntCode*> network(DEV_NUM);
    for (size_t i = 0; i < DEV_NUM; i++) {
        network[i] = new IntCode(iv);
    }
    // init queues for computers
    array<deque<myNum>, 300> queue;

    {
        for (int i = 0; i < DEV_NUM; i++) {
            network[i]->run(i, queue); // init
        }
        myNum ans;
        while (1) {
            for (int i = 0; i < DEV_NUM; i++) {
                if (network[i]->run(i, queue) == 255) {
                    ans = queue[255].front();
                    break;
                }
            }
            if (ans) break;
        }
        cout << "Ans to part1 is: "   << ans << endl;
    }

    { // 
        myNum ans;
        vector<myNum> sentToZero; // store Y value only
        while (1) {
            bool isIdle = true;
            for (int i = 0; i < DEV_NUM; i++) {
                const int res = network[i]->run(i, queue);
                if (res != -1) isIdle = false;
            }
            if (isIdle) {
                queue[0].push_front(queue[255].at(1));
                queue[0].push_front(queue[255].at(0));
                sentToZero.push_back(queue[255].front());
                const int len = sentToZero.size();
                if (sentToZero.back() == sentToZero[len-2]) {
                    ans = sentToZero.back();
                    break;
                }
            }
        }
        cout << "Ans to part2 is: " << ans  << endl;
    }
}

int main()
{
    ifstream fin("./d23/input.txt");
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