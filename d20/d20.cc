#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <deque>
#include <array>
#include <map>
#include <set>

using namespace std;
using Portals = map<string, array<int, 4>>;
using Point = array<int,4>;
using Portal = pair<string, array<int,2>>;
using Edge = pair<Portal, Portal>;
using Map = array<string, 150>;

pair<bool, Point> isPortal(const Portals &por, int x, int y) {
    for (auto [k, v]: por) {
        bool firstPartEqual = v[0] == x && v[1] == y;
        bool secondPartEqual = v[2] == x && v[3] == y;
        if (firstPartEqual || secondPartEqual) { // if equal, return another port position of portal.
            Point p = firstPartEqual ? Point({v[2], v[3], 0, 0}) : Point({v[0], v[1], 0, 0});
            return {
                true,
                p
            };
        }
    }
    return {
        false,
        Point({0,0,0,0})
    };
}

void traverse(const Map &m, const Portals &portals, Point start, const Point &range) {
    Point goal = portals.at("ZZ");
    deque<pair<Point, int>> queue;
    set<Point> visited;
    visited.insert(start);
    queue.push_front({start, 0});
    int steps = 0;
    while (queue.size() != 0) {
        auto p = queue.back().first;
        auto point = queue.back();
        visited.insert(p);
        queue.pop_back();
        array<Point, 4> neighbors = {{
            {p[0], p[1]-1, 0, 0}, // left
            {p[0], p[1]+1, 0, 0}, // right
            {p[0]-1, p[1], 0, 0}, // up
            {p[0]+1, p[1], 0, 0}  // down
        }};
        for (auto po: neighbors) {
            if (po[0] > range[2] || po[0] < range[0] || po[1] > range[3] || po[1] < range[1]) continue;
            if (!visited.count(po) && m[po[0]].at(po[1]) == '.') { // process '.' only
                auto res = isPortal(portals, po[0], po[1]);
                if (res.first) { // if meets portals, see if it is goal; if so, return, otherwise transport directly.
                    if (po == goal) {
                        cout << "Ans to part1: " << point.second + 1 << endl;
                        return;
                    }
                    visited.insert(po);
                    queue.push_front({res.second, point.second+2});
                    continue;
                }
                if (m[po[0]].at(po[1]) == '.') queue.push_front({po, point.second+1});
            }
        }
    }
}

void solution(Portals p, Point r) {
    // Collect maze data
    ifstream fin("./d20/input.txt");
    string line;
    Map m;
    int i = 0;
    while(getline(fin, line)) {
        m[i++] = line;
    }
    // Traverse the maze from AA to collect edges
    int s = m.size();
    auto it = p.find("AA");
    auto pos = it->second;
    traverse(m, p, pos, r);
}

int main() {
    // Record portals manually
    // <Label String, {x1, y1, x2, y2}>
    Portals portals;
    portals.insert({"AA", {61, 2, 0, 0}});
    portals.insert({"ZZ", {85, 2, 0, 0}});
    portals.insert({"PL", {37, 2, 75, 90}});
    portals.insert({"YP", {43, 2, 92, 55}});
    portals.insert({"EV", {49, 2, 87, 32}});
    portals.insert({"WV", {57, 2, 71, 32}});
    portals.insert({"EL", {71, 2, 39, 32}});
    portals.insert({"UY", {75, 2, 32, 73}});
    portals.insert({"JH", {87, 2, 92, 43}});
    portals.insert({"JG", {2, 41, 85, 90}});
    portals.insert({"LV", {2, 49, 49, 90}});
    portals.insert({"PM", {2, 53, 39, 90}});
    portals.insert({"PI", {2, 65, 59, 32}});
    portals.insert({"VO", {2, 69, 92, 37}});
    portals.insert({"TR", {2, 71, 92, 63}});
    portals.insert({"XC", {2, 79, 55, 32}});
    portals.insert({"BR", {41, 120, 92, 75}});
    portals.insert({"UN", {49, 120, 61, 90}});
    portals.insert({"OD", {57, 120, 32, 59}});
    portals.insert({"OA", {63, 120, 92, 83}});
    portals.insert({"XP", {71, 120, 75, 32}});
    portals.insert({"TH", {83, 120, 47, 32}});
    portals.insert({"OG", {122, 39, 32, 49}});
    portals.insert({"CK", {122, 45, 92, 69}});
    portals.insert({"NB", {122, 53, 32, 71}});
    portals.insert({"XL", {122, 61, 32, 61}});
    portals.insert({"FZ", {122, 65, 32, 39}});
    portals.insert({"SL", {122, 75, 69, 90}});
    portals.insert({"WW", {122, 81, 32, 81}});
    // maze x-y range: 2,2 ~ 122,120
    auto range = Point({2, 2, 122, 120});
    solution(portals, range);
    return 0;
}