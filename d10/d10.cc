#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <map>
using namespace std;

typedef vector<pair<int,int>> PosVec;
typedef pair<int,int> Point;

int gcd(int a, int b) {
    if (a == 0 || b == 0) 
        return a ? a : b;
    int temp;
    while (b != 0) {
        temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

bool isDirect(vector<string> v, Point i, Point j) {
    const int delta_x = j.first - i.first;
    const int delta_y = j.second - i.second;
    const int dividor = gcd(abs(delta_x), abs(delta_y));
    const int unit_x = delta_x / dividor;
    const int unit_y = delta_y / dividor;
    int curr_x = i.first, curr_y = i.second, end_x = j.first, end_y = j.second;
    while (curr_x != end_x || curr_y != end_y) {
        curr_x += unit_x;
        curr_y += unit_y;
        if (curr_x == end_x && curr_y == end_y) return true;
        if (v[curr_y][curr_x] == '#') return false;
    }
    return true;
}

Point part1(vector<string> v, PosVec pos) {
    // for every astroid, find paired astroid in direct line of sight.
    vector<int> detected = vector<int>(pos.size(), 0);
    for (int i = 0; i < pos.size() - 1; i++) {
        for (int j = i + 1; j < pos.size(); j++) {
            if (isDirect(v, pos[i], pos[j])) {
                detected[i]++;
                detected[j]++;
            }
        }
    }
    int maxIdx = 0;
    for (int i = 0; i < detected.size(); i++) {
        maxIdx = detected[maxIdx] < detected[i] ? i : maxIdx;
    }

    cout << detected[maxIdx] << endl;
    return pos[maxIdx];
}

int part2(vector<string> v, PosVec pos, Point p) {
    // first need to store the angle(0~359) relative to station and their distance to it.
    map<
    vector<Point> res; // store vaporized Point in order
    vector<bool> vaporized = vector<bool>(pos.size(), false);
    // Goal: every point is vaporized except itself.
    while (res.size() < pos.size() - 1) {

    }
}

int main() {
    ifstream fin("./d10/input.txt");
    string s;
    vector<string> map;
    while (getline(fin, s)) {
        map.push_back(s);
    }
    // get all astroid coordinates from map
    PosVec position;
    for (int i = 0; i < map.size(); i++) {
        for (int j = 0; j < map[0].size(); j++) {
            if (map[i][j] == '#') position.push_back({j, i});
        }
    }

    const Point point = part1(map, position);
    part2(map, position, point);
    return 0;

}