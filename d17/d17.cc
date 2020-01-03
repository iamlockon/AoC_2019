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

enum LOCA {
    NL = 10,
    SCAF = 35,
    SPACE = 46,
    BOT = 94
};

typedef long long myNum;

using list_t = vector<myNum>;
using Map = vector<vector<myNum>>;

struct Point {
    int type;
    pair<myNum,myNum> position;
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
                    if (operand1 < 256) cout << (char)operand1;
                    continue;
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
        throw "Error";
        cout << "unexpected instruction pointer out of bound: " << this->inst_ptr << endl;
        return 0;
    }
};

Map render(IntCode *com) {
    Map m;
    list_t v;
    while (!com->isHalt()) {
        const myNum i = com->run();
        if (i == NL && v.size() > 0) {
            m.push_back(v);
            v.clear();
        }
        else v.push_back(i);
        cout << (char)i;
    }
    return m;
}

list_t _gen_seq(Map map, pair<int, int> start) {
    enum { UP, DOWN, LEFT, RIGHT };
    list_t lt;
    auto _go = [&](pair<int,int> curr, list_t &seq, int direction) {
        seq.push_back(direction == LEFT ? (int)'L' : (int)'R');
        seq.push_back(0);
        int delta_x = 0, delta_y = 0;
        while (map[curr.first][curr.second] == SCAF || map[curr.first][curr.second] == BOT) {
            switch (direction) {
                case LEFT: {
                    delta_x = 0;
                    delta_y = -1;
                    break;
                }
                case RIGHT: {
                    delta_x = 0;
                    delta_y = +1;
                    break;
                }
                case UP: {
                    delta_x = -1;
                    delta_y = 0;
                    break;
                }
                case DOWN: {
                    delta_x = +1;
                    delta_y = 0;
                    break;
                }
            }
            do {
                curr.first += delta_x;
                curr.second += delta_y;
                if (seq.back() < 9)seq.back()++;
                else {
                    seq.push_back(0);
                    seq.back()++;
                }
                if (curr.first+ delta_x >= map.size() || curr.first+delta_x < 0 
                    || curr.second+ delta_y >= map[0].size() || curr.second+delta_y < 0
                ) break;
            } while (map[curr.first + delta_x][curr.second + delta_y] == SCAF);

            switch (direction) {
                case LEFT: {
                    if (curr.first+1 < map.size() && map[curr.first+1][curr.second] == SCAF) {
                        direction = DOWN;
                        seq.push_back((int)'L');
                        seq.push_back(0);
                    } else if (curr.first >= 0 && map[curr.first-1][curr.second] == SCAF) {
                        direction = UP;
                        seq.push_back((int)'R');
                        seq.push_back(0);
                    } else {
                        return;
                    }
                    break;
                }
                case RIGHT: {
                    if (curr.first-1 >= 0 && map[curr.first-1][curr.second] == SCAF) {
                        direction = UP;
                        seq.push_back((int)'L');
                        seq.push_back(0);
                    } else if (curr.first+1 < map.size() && map[curr.first+1][curr.second] == SCAF) {
                        direction = DOWN;
                        seq.push_back((int)'R');
                        seq.push_back(0);
                    } else {
                        return;
                    }
                    break;
                }
                case UP: {
                    if (curr.second-1 >= 0 && map[curr.first][curr.second-1] == SCAF) {
                        direction = LEFT;
                        seq.push_back((int)'L');
                        seq.push_back(0);
                    } else if (curr.second+1 < map[0].size() && map[curr.first][curr.second+1] == SCAF) {
                        direction = RIGHT;
                        seq.push_back((int)'R');
                        seq.push_back(0);
                    } else {
                        return;
                    }
                    break;
                }
                case DOWN: {
                    if (curr.second+1 < map[0].size() && map[curr.first][curr.second+1] == SCAF) {
                        direction = RIGHT;
                        seq.push_back((int)'L');
                        seq.push_back(0);
                    } else if (curr.second-1 >= 0 && map[curr.first][curr.second-1] == SCAF) {
                        direction = LEFT;
                        seq.push_back((int)'R');
                        seq.push_back(0);
                    } else {
                        return;
                    }
                    break;
                }
            }

        }
    };
    // terminating on non-SCAF left and right (with respect to the bot's direction)
    int x = start.first, y = start.second;
    int direction = UP;
    if (map[x+1][y] == SCAF) {
        direction = DOWN;
    } else if (map[x][y-1] == SCAF) {
        direction = LEFT;
    } else if (map[x][y+1] == SCAF) {
        direction = RIGHT;
    }
    _go(start, lt, direction);
    return lt;
}

// reference: https://github.com/SinisterMJ/AdventOfCode/blob/master/AllDays/Day_17.hpp
map<char, string> find_patterns(list_t inpt) {

    auto CheckFuncLen = [](string s) {
        return s.size() + (s.size() - 1) <= 20;
    };

    string input;

    auto CheckPatterns = [&input](map<char, string> dict, string &patt) {
        int offset = 0;
        while (offset < input.size()) {
            string sA = input.substr(offset, dict['A'].size());
            string sB = input.substr(offset, dict['B'].size());
            string sC = input.substr(offset, dict['C'].size());
            
            if (sA == dict['A']) {
                offset += sA.size();
                patt += "A";
                continue;
            }
            if (sB == dict['B']) {
                offset += sB.size();
                patt += "B";
                continue;
            }
            if (sC == dict['C']) {
                offset += sC.size();
                patt += "C";
                continue;
            }
            return false;
        }
        return true;
    };

    for (auto i: inpt) {
        input += (char)i;
    }
    // input : R2R5L3L3R3.....
    map<char, string> m;
    m['A'] = "";
    m['B'] = "";
    m['C'] = "";
    for (int lA = 10; lA > 0; lA--) {
        int offsetA = 0;
        m['A'] = input.substr(offsetA, lA);
        if (!CheckFuncLen(m['A'])) continue;
        for (int lB = 10; lB > 0; lB--) {
            int offsetB = lA;
            while (input.substr(offsetB, lA) == m['A']) offsetB += lA;
            m['B'] = input.substr(offsetB, lB);
            if (!CheckFuncLen(m['B'])) continue;
            for (int lC = 10; lC > 0; lC--) {
                int offsetC = offsetB + lB;
                while (true) {
                    if (input.substr(offsetC, lA) == m['A']) {
                        offsetC += lA;
                        continue;
                    }
                    if (input.substr(offsetC, lB) == m['B']) {
                        offsetC += lB;
                        continue;
                    }
                    break;
                }
                m['C'] = input.substr(offsetC, lC);
                if (!CheckFuncLen(m['C'])) continue;
                string patt;
                if (CheckPatterns(m, patt)) {
                    m['M'] = patt;
                    return m;
                }
            }
        }
    }
}

void solution(const list_t &iv) {
    IntCode *com = new IntCode(iv);
    Map m = render(com);
    for (auto row: m) {
        if (row.size() ==0 ) {
            
        }
    }
    vector<pair<int,int>> itsec;
    pair<int,int> start;
    for (int i = 1; i < m.size() - 1; i++) {
        for (int j = 1; j < m[0].size() - 1; j++) {
            if (m[i][j] == BOT) start = make_pair(i, j);
            const int k = 35;
            if (k == m[i][j] && k == m[i-1][j] && k == m[i+1][j] && k == m[i][j-1] && k == m[i][j+1]) itsec.push_back({i,j});
        }
    }
    int res = 0;
    for (auto i: itsec) {
        res += i.first * i.second;
    }
    cout << "Ans to part1: "<< res << endl;

    // generate sequence.
    list_t seq = _gen_seq(m, start);

    for (auto s: seq) {
        if (s == (int)'L' || s == (int)'R') cout << (char)s << ",";
        else cout << s << ",";
    }
    cout << endl;
    auto patt_dict = find_patterns(seq);
    
    list_t main, l_A, l_B, l_C;
    for (auto l: patt_dict) {
        switch (l.first) {
            case 'M': {
                for (auto c: l.second) {
                    main.push_back((int)c);
                    main.push_back((int)',');
                }
                main.pop_back();
                main.push_back(10);
                break;
            }
            case 'A': {
                for (auto c: l.second) {
                    if (c == 'R' || c == 'L') {
                        l_A.push_back((int)c);
                        l_A.push_back((int)',');
                    }
                    else {
                        string s = to_string((int)c);
                        for (auto ss: s) {
                            l_A.push_back((int)ss);
                            l_A.push_back((int)',');
                        }
                    }
                }
                l_A.pop_back();
                l_A.push_back(10);
                break;
            }
            case 'B': {
                for (auto c: l.second) {
                    if (c == 'R' || c == 'L') {
                        l_B.push_back((int)c);
                        l_B.push_back((int)',');
                    }
                    else {
                        string s = to_string((int)c);
                        for (auto ss: s) {
                            l_B.push_back((int)ss);
                            l_B.push_back((int)',');
                        }
                    }
                }
                l_B.pop_back();
                l_B.push_back(10);
                break;
            }
            case 'C': {
                for (auto c: l.second) {
                    if (c == 'R' || c == 'L') {
                        l_C.push_back((int)c);
                        l_C.push_back((int)',');
                    }
                    else {
                        string s = to_string((int)c);
                        for (auto ss: s) {
                            l_C.push_back((int)ss);
                            l_C.push_back((int)',');
                        }
                    }
                }
                l_C.pop_back();
                l_C.push_back(10);
                break;
            }
        }
    }

    list_t niv = iv;
    niv[0] = 2;
    IntCode *com2 = new IntCode(niv);
    list_t all_input(main.size()+l_A.size()+l_B.size()+l_C.size());
    auto aib = all_input.begin();
    aib = copy_n(main.begin(), main.size(), aib);
    aib = copy_n(l_A.begin(), l_A.size(), aib);
    aib = copy_n(l_B.begin(), l_B.size(), aib);
    aib = copy_n(l_C.begin(), l_C.size(), aib);
    all_input.push_back((int)'y');
    all_input.push_back(10);
    auto re = com2->seq_run(all_input);

    cout << "Ans to part2 is: " << re << endl;
}


int main() {
    ifstream fin("./d17/input.txt");
    string ln;
    getline(fin, ln);
    list_t iv;
    stringstream ss(ln);
    string s;
    while (getline(ss, s, ',')) {
        iv.push_back(stoi(s));
    }
    solution(iv);
    return 0;
}