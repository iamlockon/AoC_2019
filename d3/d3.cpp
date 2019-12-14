#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>
#include <unordered_map>
using namespace std;

typedef pair<int, int> pairs;

int part1() {
    ifstream fin("input.txt");
    string p1, p2;
    getline(fin, p1);
    getline(fin, p2);
    istringstream ss1(p1);
    istringstream ss2(p2);

    vector<string> v1, v2;
    while (ss1) {
        string a;
        if (!getline(ss1, a, ',')) break;
        v1.push_back(a);
    }
    while (ss2) {
        string a;
        if (!getline(ss2, a, ',')) break;
        v2.push_back(a);
    }
    
    set<pairs> mset;
    // store path 1 to set
    int x = 0, y = 0;
    for (int i = 0; i < v1.size(); i++) {
        char direction = v1[i][0];
        int step = stoi(v1[i].substr(1));
        switch (direction) {
            case 'R': {
                while (step--) mset.insert(make_pair(++x, y));
                break;
            }
            case 'L': {
                while (step--) mset.insert(make_pair(--x, y));
                break;
            }
            case 'D': {
                while (step--) mset.insert(make_pair(x, --y));
                break;
            }
            case 'U': {
                while (step--) mset.insert(make_pair(x, ++y));
                break;
            }
        }
    }
    // check duplicates and calculate min distance
    int res = INT32_MAX;
    x = 0, y = 0;
    for (int i = 0; i < v2.size(); i++) {
        char direction = v2[i][0];
        int step = stoi(v2[i].substr(1));
        set<pairs>::iterator it;
        switch (direction) {
            case 'R': {
                while (step--) {
                    it = mset.find(make_pair(++x, y));
                    if (it != mset.end() && res > abs(x) + abs(y)) res = abs(x) + abs(y); 
                }
                break;{}
            }
            case 'L': {
                while (step--) {
                    it = mset.find(make_pair(--x, y));
                    if (it != mset.end() && res > abs(x) + abs(y)) res = abs(x) + abs(y); 
                }
                break;
            }
            case 'D': {
                while (step--) {
                    it = mset.find(make_pair(x, --y));
                    if (it != mset.end() && res > abs(x) + abs(y)) res = abs(x) + abs(y); 
                }
                break;
            }
            case 'U': {
                while (step--) {
                    it = mset.find(make_pair(x, ++y));
                    if (it != mset.end() && res > abs(x) + abs(y)) res = abs(x) + abs(y); 
                }
                break;
            }
        }
    }
    cout << "Ans to part1 is: " << res << endl;
    return 0;
}

int part2() {
    ifstream fin("input.txt");
    string p1, p2;
    getline(fin, p1);
    getline(fin, p2);
    istringstream ss1(p1);
    istringstream ss2(p2);

    vector<string> v1, v2;
    while (ss1) {
        string a;
        if (!getline(ss1, a, ',')) break;
        v1.push_back(a);
    }
    while (ss2) {
        string a;
        if (!getline(ss2, a, ',')) break;
        v2.push_back(a);
    }
    
    set<string> mset;
    unordered_map<string, int> mmap;
    // store path 1 to set
    int x = 0, y = 0;
    for (int i = 0; i < v1.size(); i++) {
        set<string>::iterator it;
        char direction = v1[i][0];
        int step = stoi(v1[i].substr(1));
        switch (direction) {
            case 'R': {
                while (step--) mset.insert(to_string(++x) + " " + to_string(y));
                break;
            }
            case 'L': {
                while (step--) mset.insert(to_string(--x) + " " + to_string(y));
                break;
            }
            case 'D': {
                while (step--) mset.insert(to_string(x) + " " + to_string(--y));
                break;
            }
            case 'U': {
                while (step--) mset.insert(to_string(x) + " " + to_string(++y));
                break;
            }
        }
    }
    // check duplicates and map duplicates string to steps of 2
    int stepsFor2 = 0;
    x = 0, y = 0;
    for (int i = 0; i < v2.size(); i++) {
        char direction = v2[i][0];
        int step = stoi(v2[i].substr(1));
        set<string>::iterator it;
        switch (direction) {
            case 'R': {
                while (step--) {
                    stepsFor2++;
                    it = mset.find(to_string(++x) + " " + to_string(y));
                    if (it != mset.end()) {
                        mmap.insert(make_pair(*it, stepsFor2));
                    } 
                }
                break;
            }
            case 'L': {
                while (step--) {
                    stepsFor2++;
                    it = mset.find(to_string(--x) + " " + to_string(y));
                    if (it != mset.end()) {
                        mmap.insert(make_pair(*it, stepsFor2));
                    } 
                }
                break;
            }
            case 'D': {
                while (step--) {
                    stepsFor2++;
                    it = mset.find(to_string(x) + " " + to_string(--y));
                    if (it != mset.end()) {
                        mmap.insert(make_pair(*it, stepsFor2));
                    } 
                }
                break;
            }
            case 'U': {
                while (step--) {
                    stepsFor2++;
                    it = mset.find(to_string(x) + " " + to_string(++y));
                    if (it != mset.end()) {
                        mmap.insert(make_pair(*it, stepsFor2));
                    } 
                }
                break;
            }
        }
    }

    // accumulate mapping-to values for path 1
    int stepsFor1 = 0;
    x = 0, y = 0;
    int min = INT32_MAX;
    for (int i = 0; i < v1.size(); i++) {
        char direction = v1[i][0];
        int step = stoi(v1[i].substr(1));
        unordered_map<string, int>::iterator it;
        switch (direction) {
            case 'R': {
                while (step--) {
                    stepsFor1++;
                    it = mmap.find(to_string(++x) + " " + to_string(y));
                    if (it != mmap.end()) {
                        min = std::min(it->second + stepsFor1, min);
                    } 
                }
                break;
            }
            case 'L': {
                while (step--) {
                    stepsFor1++;
                    it = mmap.find(to_string(--x) + " " + to_string(y));
                    if (it != mmap.end()) {
                        min = std::min(it->second + stepsFor1, min);
                    } 
                }
                break;
            }
            case 'D': {
                while (step--) {
                    stepsFor1++;
                    it = mmap.find(to_string(x) + " " + to_string(--y));
                    if (it != mmap.end()) {
                        min = std::min(it->second + stepsFor1, min);
                    } 
                }
                break;
            }
            case 'U': {
                while (step--) {
                    stepsFor1++;
                    it = mmap.find(to_string(x) + " " + to_string(++y));
                    if (it != mmap.end()) {
                        min = std::min(it->second + stepsFor1, min);
                    } 
                }
                break;
            }
        }
    }
    cout << "Ans to part2 is: " << min << endl; 
    return 0;
}

int main() {
    part1();
    part2();
    return 0;
}
