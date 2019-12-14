#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
using namespace std;

enum P_mode{
    POSITION,
    IMMEDIATE
};

int part1(vector<int> v, int id) {
    int idx = 0, res = INT32_MAX;
    unordered_map<int, int> output_map; // idx map to output code
    while (idx < v.size()) {
        const int opcode = v[idx] % 100;
        // cout << "idx: " << idx << ", opcode: " << opcode << endl;
        switch (opcode) {
            case 1: {
                if (idx+3 >= v.size()) return -2;
                const int pmodes = (v[idx] / 100);
                const int operand1 = (pmodes % 10 == POSITION) ? v[v[idx+1]] : v[idx+1];
                const int operand2 = ((pmodes % 100) / 10 == POSITION) ? v[v[idx+2]] : v[idx+2]; 
                v[v[idx+3]] = operand1 + operand2;
                idx += 4;
                break;
            }
            case 2: {
                if (idx+3 >= v.size()) return -2;
                const int pmodes = (v[idx] / 100);
                const int operand1 = (pmodes % 10 == POSITION) ? v[v[idx+1]] : v[idx+1];
                const int operand2 = ((pmodes % 100) / 10 == POSITION) ? v[v[idx+2]] : v[idx+2]; 
                v[v[idx+3]] = operand1 * operand2;
                idx += 4;
                break;
            }
            case 3: { // input opcode
                v[v[idx+1]] = id;
                idx += 2;
                break;
            }
            case 4: { // output opcode
                const int pmode = v[idx] / 100;
                const int operand1 = pmode == POSITION ? v[v[idx+1]] : v[idx+1];
                output_map.insert(make_pair(idx, operand1));
                idx += 2;
                break;
            }
            case 99: {         
                for (auto i : output_map) {
                    // if (i.second != 0) {
                    //     cout << "Test at " << i.first << " failed with output code :" << i.second << endl;
                    // }
                    // cout << "(" << i.first << ", " << i.second << " )" << endl;
                }
                
                return output_map[idx-2];
            }
        }
    }
    return res;
}

int part2 (vector<int> v, int id) {
    int idx = 0;
    vector<int> res;
    while (idx < v.size()) {
        const int opcode = v[idx] % 100;
        switch (opcode) {
            case 1: {
                if (idx+3 >= v.size()) return -2;
                const int pmodes = (v[idx] / 100);
                const int operand1 = (pmodes % 10 == POSITION) ? v[v[idx+1]] : v[idx+1];
                const int operand2 = ((pmodes % 100) / 10 == POSITION) ? v[v[idx+2]] : v[idx+2]; 
                v[v[idx+3]] = operand1 + operand2;
                idx += 4;
                break;
            }
            case 2: {
                if (idx+3 >= v.size()) return -2;
                const int pmodes = (v[idx] / 100);
                const int operand1 = (pmodes % 10 == POSITION) ? v[v[idx+1]] : v[idx+1];
                const int operand2 = ((pmodes % 100) / 10 == POSITION) ? v[v[idx+2]] : v[idx+2]; 
                v[v[idx+3]] = operand1 * operand2;
                idx += 4;
                break;
            }
            case 3: { // input opcode
                v[v[idx+1]] = id;
                idx += 2;
                break;
            }
            case 4: { // output opcode
                const int pmode = v[idx] / 100;
                const int operand1 = pmode == POSITION ? v[v[idx+1]] : v[idx+1];
                res.push_back(operand1);
                idx += 2;
                break;
            }
            case 5: { // jump-if-true
                const int pmods = v[idx] / 100;
                const int operand1 = (pmods % 10 == POSITION) ? v[v[idx+1]] : v[idx+1];
                const int operand2 = ((pmods % 100) / 10 == POSITION) ? v[v[idx+2]] : v[idx+2]; 
                if (operand1) {
                    idx = operand2;
                }
                else {
                    idx += 3;
                }
                break;
            }
            case 6: { // jump-if-false
                const int pmods = v[idx] / 100;
                const int operand1 = (pmods % 10 == POSITION) ? v[v[idx+1]] : v[idx+1];
                const int operand2 = ((pmods % 100) / 10 == POSITION) ? v[v[idx+2]] : v[idx+2]; 
                if (operand1 == 0) {
                    idx = operand2;
                }
                else {
                    idx += 3;
                }
                break;
            }
            case 7: { // less than
                const int pmods = v[idx] / 100;
                const int operand1 = (pmods % 10 == POSITION) ? v[v[idx+1]] : v[idx+1];
                const int operand2 = ((pmods % 100) / 10 == POSITION) ? v[v[idx+2]] : v[idx+2];
                v[v[idx+3]] = (operand1 < operand2) ? 1 : 0;
                idx += 4;
                break;
            }
            case 8: { // equal
                const int pmods = v[idx] / 100;
                const int operand1 = (pmods % 10 == POSITION) ? v[v[idx+1]] : v[idx+1];
                const int operand2 = ((pmods % 100) / 10 == POSITION) ? v[v[idx+2]] : v[idx+2];
                v[v[idx+3]] = (operand1 == operand2) ? 1 : 0;
                idx += 4;
                break;
            }
            case 99: {               
                return res[res.size()-1];
            }
            default: {
                return -1;
            }
        }
    }
    return 0;
}

int main() {
    ifstream fin("input.txt");
    string s;
    getline(fin, s);
    istringstream ss(s);
    vector<int> v;
    while (ss) {
        string a;
        if (!getline(ss, a, ',')) break;
        v.push_back(stoi(a));
    }
    // now we have the program as a vector.
    //cout << "Ans to part1 is :" << part1(v, 1) << endl;
    cout << "Ans of part2 is: " << part2(v, 5) << endl;
    return 0;
}
