#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <set>
using namespace std;

enum P_mode
{
    POSITION,
    IMMEDIATE,
    RELATIVE
};

typedef long long myNum;
typedef pair<int,int> Point;
typedef vector<Point> Path;
typedef map<Point, int> Map;
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
                    this->lastOut = operand1;
                    this->program = v; // store the program state.
                    return operand1;
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

Point calNewPosition(Point curr, int &facing, const int &turnTo) {
    if (turnTo == 0) { // left
        switch (facing) {
            case 0: {
                facing = 3;
                return make_pair(curr.first - 1, curr.second);
            }
            case 1: {
                facing = 0;
                return make_pair(curr.first, curr.second + 1);
            }
            case 2: {
                facing = 1;
                return make_pair(curr.first + 1, curr.second);
            }
            case 3: {
                facing = 2;
                return make_pair(curr.first, curr.second - 1);
            }
        }
    } else { // right
        switch (facing) {
            case 0: {
                facing = 1;
                return make_pair(curr.first + 1, curr.second);
            }
            case 1: {
                facing = 2;
                return make_pair(curr.first, curr.second - 1);
            }
            case 2: {
                facing = 3;
                return make_pair(curr.first - 1, curr.second);
            }
            case 3: {
                facing = 0;
                return make_pair(curr.first, curr.second + 1);
            }
        }
    }
}

myNum part1(vector<myNum> v) {
    // let starting point be (0, 0), using cartesian coordinates
    Path p; // store traversed points in order.
    Map m; // store current color of traversed points.
    m.insert(make_pair(make_pair(0,0), 0));
    IntCode *com = new IntCode(v);
    Point currentPos = make_pair(0,0);
    int f = 0; // ^: 0, >: 1, v: 2, <: 3
    int &currentFacing = f;
    while(!com->isHalt()) {
        p.push_back(currentPos);
        const int paintTo = com->run(m[currentPos]); // 0 : black 1: white
        const int turnTo = com->run(m[currentPos]); // 0 : left 1: right
        m[currentPos] = paintTo;
        currentPos = calNewPosition(currentPos, currentFacing, turnTo); // currentFacing will be changed.
    }
    set<Point> mySet(p.begin(), p.end());
    cout << mySet.size() << endl;
    return mySet.size();
}

myNum part2(vector<myNum> v) {
    // let starting point be (0, 0), using cartesian coordinates
    Path p; // store traversed points in order.
    Map m; // store current color of traversed points.
    m.insert(make_pair(make_pair(0,0), 1));
    IntCode *com = new IntCode(v);
    Point currentPos = make_pair(0,0);
    int f = 0; // ^: 0, >: 1, v: 2, <: 3
    int &currentFacing = f;
    while(!com->isHalt()) {
        p.push_back(currentPos);
        const int paintTo = com->run(m[currentPos]); // 0 : black 1: white
        const int turnTo = com->run(m[currentPos]); // 0 : left 1: right
        m[currentPos] = paintTo;
        currentPos = calNewPosition(currentPos, currentFacing, turnTo); // currentFacing will be changed.
    }
    int minX = INT32_MAX, minY = INT32_MAX, maxX = INT32_MIN, maxY = INT32_MIN;
    for (auto &p: m) {
        Point po = p.first;
        minX = std::min(minX, po.first);
        maxX = std::max(maxX, po.first);
        minY = std::min(minY, po.second);
        maxY = std::max(maxY, po.second);
    }
    cout << minX << "~" << maxX << ", " << minY << "~" << maxY << endl;
    char imag[50][10];
    for (int i = 0; i < 50; i++) {
        fill_n(imag[i], 10, '.');
    }
    imag[0][5] = '#';
    for (auto &p: m) {
        Point po = p.first;
        int color = p.second;
        imag[po.first][po.second + 5] = color == 0 ? '.' : '#';
    }
    for (int i = 0; i < 50; i++) {
        for (int j = 0; j < 10; j++) {
            cout << imag[i][j];
        }
        cout << endl;
    }
}

int main()
{
    ifstream fin("./d11/input.txt");
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