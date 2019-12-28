#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
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

enum STATUS_CODE {
    WALL,
    SPACE,
    OXYGEN
};
enum Direction {
    NORTH = 1,
    SOUTH = 2,
    WEST = 3,
    EAST = 4,
};

typedef long long myNum;
struct Point {
    int type;
    pair<myNum,myNum> position;
};

typedef pair<myNum, myNum> Pos;

struct Node {
    Pos p;
    Node *predecessor;
    int layer;
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
        throw 'Error';
        cout << "unexpected instruction pointer out of bound: " << this->inst_ptr << endl;
        return 0;
    }
};

int reverseInput(int input) {
    switch (input)
    {
    case 1:
        return 2;
        break;
    case 2:
        return 1;
        break;
    case 3:
        return 4;
        break;
    case 4:
        return 3;
        break;
    }
}

Pos calNewPos(int input, Pos lastPoint) {
    Pos p;
    switch (input)
    {
    case 1:
        p.first = lastPoint.first;
        p.second = lastPoint.second + 1;
        break;
    case 2:
        p.first = lastPoint.first;
        p.second = lastPoint.second - 1;
        break;
    case 3:
        p.first = lastPoint.first - 1;
        p.second = lastPoint.second;
        break;
    case 4:
        p.first = lastPoint.first + 1;
        p.second = lastPoint.second;
        break;
    }
    return p;
}

void backtrack(IntCode *comp, map<Pos, int> &map, Pos cur, int type, set<Pos> &visited, int steps, int &done) {
    if (done != -1) return; // if done, just return
    if (type == OXYGEN) { // if found oxygen, record and return
        map[cur] = OXYGEN;
        done = steps;
        return;
    }
    if (!visited.count(cur)) { // else if not visited, try run IntCode
        map[cur] = type;
        visited.insert(cur);

        for (int i = 1; i < 5; i++) {
            const int type = comp->run(i);
            map[calNewPos(i, cur)] = type;
            if(type != WALL) {
                backtrack(comp, map, calNewPos(i, cur), type, visited, steps + 1, done);
                comp->run(reverseInput(i));
            }
        }
    }

}


void solution(vector<myNum> vec) {
    set<Pos> visited;
    map<Pos, int> map;
    IntCode *computer = new IntCode(vec);
    //backtrack
    Pos p{ make_pair(0,0) };
    int steps = 0;
    int done = -1;
    backtrack(computer, map, p, SPACE, visited, steps, done);
    cout << "Ans to part1: " << done << endl;

    // get oxygen coordinates and collect graph data
    vector<vector<char>> graph = vector<vector<char>>(50, vector<char>(50));
    Pos oxy;
    for (auto p: map) {
        if (p.second == OXYGEN) {
            oxy.first = p.first.first;
            oxy.second = p.first.second;
            graph[oxy.second + 20][oxy.first + 30] = 'O';
            continue;
        }
        int vx = p.first.first + 30, vy = p.first.second + 20;
        graph[vy][vx] = p.second == WALL ? 'X' : ' ';
    }
    for (auto i: graph) {
        for (auto j: i) {
            if(j == 'X' || j == ' ' || j == 'O' ) cout << j;
            else cout << '_';
        }
        cout << endl;
    }
    // use map to calculate.
    // bfs
    deque<Node> deque;
    visited.clear();
    Node n = { oxy, nullptr, 0};
    deque.push_front(n);
    int mins = 0;
    while (deque.size() != 0) {
        Node *node = new Node(deque.back());
        Pos *p = &(node->p);
        deque.pop_back();
        visited.insert(*p);
        for (int i = 1; i < 5; i++) {
            Pos po = calNewPos(i, *p);
            if(map[po] != WALL && !visited.count(po)) {
                Node m = { po, node, node->layer+1 };
                deque.push_front(m);   
            }
        }
        mins = std::max(mins, node->layer);
    }
    cout << "Ans to part2: " << mins << endl;
}

int main()
{
    ifstream fin("./d15/input.txt");
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
    solution(v);
    return 0;
}