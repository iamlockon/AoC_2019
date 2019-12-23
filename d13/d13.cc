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

enum TileType {
    EMPTY,
    WALL,
    BLOCK,
    HORIZONTAL_PADDLE,
    BALL
};

typedef long long myNum;
typedef pair<int,int> Point;
typedef vector<vector<Point>> GameObject;

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

myNum part1(vector<myNum> v) {
    IntCode *com = new IntCode(v);
    GameObject go = GameObject(5, vector<Point>());
    while (!com->isHalt()) {
        const int x = com->run();
        const int y = com->run();
        const int type = com->run();
        go[type].push_back(make_pair(x,y));
    }
    cout << go[BLOCK].size() << endl;
}

void renderScreen(vector<vector<string>> scr, myNum bx, myNum by, myNum px, myNum py, int x_size, int y_size) {
    scr[by][bx] = "O";
    scr[py][px] = "==";
    for (int i = 0; i < y_size; i++) {
        for (int j = 0; j < x_size; j++) {
            cout << scr[i][j];
        }
        cout << endl;
    }
    cout << endl << endl << endl << endl;
}

myNum part2(vector<myNum> v) {
    v[0] = 2; // free coins
    IntCode *com = new IntCode(v);
    int x_size = 50, y_size = 22;
    vector<vector<string>> screen = vector<vector<string>>(y_size, vector<string>(x_size, "-"));
    myNum score = 0, paddle_x = 0, paddle_y = 0, ball_x = 0, ball_y = 0;
    while (!com->isHalt()) {
        myNum input = paddle_x > ball_x ? -1 : (paddle_x < ball_x ? +1 : 0);
        const myNum x = com->run(input);
        const myNum y = com->run(input);
        const myNum type = com->run(input);
        if (x == -1 && y == 0) score = type;
        switch (type) {
            case BALL: {
                ball_x = x;
                ball_y = y;
                // renderScreen(screen, ball_x, ball_y, paddle_x, paddle_y, x_size, y_size);
                break;
            }
            case HORIZONTAL_PADDLE: {
                paddle_x = x;
                paddle_y = y;
                // renderScreen(screen, ball_x, ball_y, paddle_x, paddle_y, x_size, y_size);
                break;
            }
        }
        // cout << ball_x << "," << ball_y << "," << paddle_x << "," << paddle_y << endl;
        
    }
    cout << score << endl;
    
}

int main()
{
    ifstream fin("./d13/input.txt");
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