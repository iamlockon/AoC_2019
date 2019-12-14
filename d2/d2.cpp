#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

int solution(vector<int> v, int noun, int verb) {
    v[1] = noun;
    v[2] = verb;
    // restore the state...
    int idx = 0;
    while (idx < v.size()) {
        switch (v[idx]) {
            case 1: {
                if (idx+3 >= v.size()) return -2;
                v[v[idx+3]] = v[v[idx+1]] + v[v[idx+2]];
                break;
            }
            case 2: {
                if (idx+3 >= v.size()) return -2;
                v[v[idx+3]] = v[v[idx+1]] * v[v[idx+2]];
                break;
            }
            default:
                if (v[idx] ==  99) {
                    return v[0];
                } else {
                    return -1;
                }
        }
        idx += 4; // update index
    }
    return -1;
}

int main() {
    ifstream fin("d2.txt");
    string s;
    getline(fin, s);
    istringstream ss(s);
    vector<int> v;
    while (ss) {
        string a;
        if (!getline(ss, a, ',')) break;
        v.push_back(stoi(a));
    }

    for (int i = 0; i < 100; i++) {
        for (int j = i; j < 100; j++) {
            if(solution(v, i, j) == 19690720) {
                cout << "Ans is:" << 100 * i + j << endl;
                break;
            }
        }
    }
    
    return 0;
}
