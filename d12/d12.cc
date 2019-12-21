#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <set>

using namespace std;

void part1(vector<vector<int>> pos, vector<vector<int>> vel, int steps) {
    while (steps--) {
        // apply gravity for each pair / axis
        for (int i = 0; i < pos.size()-1; i++) {
            for (int j = i+1; j < pos.size(); j++) {
                for (int axis = 0; axis < 3; axis++) {
                    if (pos[i][axis] > pos[j][axis]) {
                        vel[i][axis]--;
                        vel[j][axis]++;
                    } else if (pos[i][axis] < pos[j][axis]) {
                        vel[i][axis]++;
                        vel[j][axis]--;
                    }
                }
            }
        }
        // apply velocity
        for (int i = 0; i < pos.size(); i++) {
            for (int axis = 0; axis < 3; axis++) {
                pos[i][axis] += vel[i][axis];
            }
        }
    }
    vector<int> ener = vector<int>(pos.size(), 0);
    // calculate energy
    for (int i = 0; i < pos.size(); i++) {
        int pot = 0, kin = 0;
        for (int axis = 0; axis < 3; axis++) {
            pot += abs(pos[i][axis]);
            kin += abs(vel[i][axis]);
        }
        ener[i] = pot * kin;
    }
    int total = 0;
    for (auto e: ener) {
        total += e;
    }
    cout << total << endl;
}

string calHash(const vector<vector<int>> &pos, const vector<vector<int>> &vel, const int axis) {
    string s = "";
    for (int i = 0; i < pos.size(); i++) {
        s += to_string(pos[i][axis]) + "|" + to_string(vel[i][axis]) + "|";
    }
    return s;
}

long long GCD(long long a, long long b) {
    if (a == 0 || b == 0) 
        return a ? a : b;
    long long temp;
    while (b != 0) {
        temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

long long LCM(long long a, long long b) {
    return  (a * b) / GCD(a, b);
}


void part2(vector<vector<int>> pos, vector<vector<int>> vel) {
    // use LCM to get the answer.
    string initXHash = calHash(pos, vel, 0);
    string initYHash = calHash(pos, vel, 1);
    string initZHash = calHash(pos, vel, 2);

    long long x_repeat = 0, y_repeat = 0, z_repeat = 0;    
    long long step = 0;
    while (x_repeat == 0|| y_repeat == 0 || z_repeat == 0) {
        // apply gravity for each pair / axis
        step++;
        for (int i = 0; i < pos.size()-1; i++) {
            for (int j = i+1; j < pos.size(); j++) {
                for (int axis = 0; axis < 3; axis++) {
                    if (pos[i][axis] > pos[j][axis]) {
                        vel[i][axis]--;
                        vel[j][axis]++;
                    } else if (pos[i][axis] < pos[j][axis]) {
                        vel[i][axis]++;
                        vel[j][axis]--;
                    }
                }
            }
        }
        // apply velocity
        for (int i = 0; i < pos.size(); i++) {
            for (int axis = 0; axis < 3; axis++) {
                pos[i][axis] += vel[i][axis];
            }
        }

        // see if repeat happens for each axis
        if (x_repeat == 0 && calHash(pos, vel, 0) == initXHash) x_repeat = step;
        if (y_repeat == 0 && calHash(pos, vel, 1) == initYHash) y_repeat = step;
        if (z_repeat == 0 && calHash(pos, vel, 2) == initZHash) z_repeat = step;
    }
    cout << LCM(LCM(x_repeat, y_repeat), z_repeat) << endl;
}
//2028 4702 983
int main() {
    ifstream fin("./d12/input.txt");
    string line = "";
    vector<vector<int>> position;
    while (getline(fin, line)) {
        auto found_x = line.find("x=");
        auto x_comma = line.find(",", found_x);
        int x = stoi(line.substr(found_x+2, x_comma - found_x - 2));
        auto found_y = line.find("y=");
        auto y_comma = line.find(",", found_y);
        int y = stoi(line.substr(found_y+2, y_comma - found_y - 2));
        auto found_z = line.find("z=");
        auto z_comma = line.find(",", found_z);
        int z = stoi(line.substr(found_z+2, z_comma - found_z - 2));
        int temp[] = {x, y, z};
        vector<int> tempv(temp, temp + sizeof(temp) / sizeof(int));
        position.push_back(tempv);
    }
    // now we have position vector, construct velocity vector.
    vector<vector<int>> velocity(position.size(), vector<int>(3, 0));
    part1(position, velocity, 1000);
    part2(position, velocity);
    return 0;
}