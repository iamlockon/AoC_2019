#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <set>
#include <unordered_map>
#include <array>
#include <bitset>
#include <cmath>
#include <iomanip> 

const int GRID_SIZE = 25;
const int SLEN = 5;
const int REP = 200;
using namespace std;

typedef array<array<char, SLEN>,SLEN> Grid;
struct Snapshot {
    bitset<GRID_SIZE> s;
    bool operator< (const Snapshot &rhs) const {
        for (int i = 0; i < GRID_SIZE; i++) {
            if (s[i] < rhs.s[i]) return true;
            else if (s[i] > rhs.s[i]) return false;
            else continue;
        }
    }
};
typedef set<Snapshot> History;



enum TYPE {
    WALL,
    BUG
};

void solution(Grid g) {

    Grid cp = g;
    auto countAdj = [](const Grid &g, int i , int j) {
        int res = 0;
        if (i - 1 > -1 && g[i-1][j] == '#') res++; 
        if (j - 1 > -1 && g[i][j-1] == '#') res++; 
        if (i + 1 < SLEN && g[i+1][j] == '#') res++; 
        if (j + 1 < SLEN && g[i][j+1] == '#') res++; 
        return res;
    };

    auto evolve = [&](Grid &gg) {
        // bug -> empty unless exactly one bug adjecent
        // empty -> bug if one or two bug adjecent
        int willChange[GRID_SIZE] = {};
        for (int i = 0; i < SLEN; i++) {
            for (int j = 0; j < SLEN; j++) {
                const int adj = countAdj(gg, i, j);
                if (gg[i][j] == '#') {
                    if (adj != 1) willChange[i*5 + j] = 1;
                } else {
                    if (adj == 1 || adj == 2) willChange[i*5 + j] = 1;
                }
            }
        }
        // mutate gg
        for (int i = 0; i < SLEN; i++) {
            for (int j = 0; j < SLEN; j++) {
                if (willChange[5*i + j]) gg[i][j] = gg[i][j] == '#' ? '.' : '#';
            }
        }
        return gg;
    };
    auto gridToSnap = [&](const Grid &gg) {
        Snapshot res;
        for (int i = 0; i < GRID_SIZE; i++) {
            if (gg[i / SLEN][i % SLEN] == '#') res.s[GRID_SIZE - i - 1] = 1;
        }
        return res;
    };
    auto calDiversity = [&](const Snapshot &s) {
        double res = 0;
        const int len = s.s.size();
        for (int i = 0; i < len; i++) {
            if (s.s[len - 1 - i]) res += pow(2.0, i);
        }
        return res;
    };

    auto countAdj_r = [](const Grid &g, int i , int j) {
        int res = 0;
        if (i - 1 > -1 && g[i-1][j] == '#') res++; 
        if (j - 1 > -1 && g[i][j-1] == '#') res++; 
        if (i + 1 < SLEN && g[i+1][j] == '#') res++; 
        if (j + 1 < SLEN && g[i][j+1] == '#') res++; 
        return res;
    };

    auto evolve_r = [&](unordered_map<int, Grid> &e) {
        // bug -> empty unless exactly one bug adjecent
        // empty -> bug if one or two bug adjecent
        unordered_map<int, int[]> willChange;
        for (auto &[k, g]: e) {
            willChange.try_emplace(k, array<int,25>{});
            for (int i = 0; i < SLEN; i++) {
                for (int j = 0; j < SLEN; j++) {
                    const int adj = countAdj_r(g, i, j);
                    if (g[i][j] == '#') {
                        if (adj != 1) willChange[k][i*5 + j] = 1;
                    } else {
                        if (adj == 1 || adj == 2) willChange[k][i*5 + j] = 1;
                    }
                }
            }
        }

        // mutate g
        for (auto &[k, g]: e) {
            for (int i = 0; i < SLEN; i++) {
                for (int j = 0; j < SLEN; j++) {
                    if (willChange[k][5*i + j]) g[i][j] = g[i][j] == '#' ? '.' : '#';
                }
            }
        }
    };

    auto recEvolve = [&](unordered_map<int, Grid> e, int rep) {
        for (int i = 0; i < rep; i++) {
            evolve_r(e);
        }
        return e;
    };

    History his;
    Snapshot s = gridToSnap(g);
    his.insert(s);
    {
        double ans;
        while (true) { // evolve until repeat
            const Snapshot ss = gridToSnap(evolve(g));
            if (his.count(ss)) {
                ans = calDiversity(ss);
                break;
            } else {
                his.insert(ss);
            }
        }
        cout << setprecision(15) << "Ans to part1 is : " << ans << endl;
    }

    // init eris (level to Grid)
    unordered_map<int, Grid> eris; 
    eris.insert({0, cp});

    {
        int ans = recEvolve(eris, REP);
        cout << "Ans to part2 is : " << ans << endl;
    }
}

int main() {
    ifstream fin("./d24/input.txt");
    string s;
    Grid g;
    int i = 0;
    while (getline(fin, s)) {
        int j = 0;
        for (auto &c: s) {
            g[i][j++] = c;
        }
        i++;
    }

    solution(g);
    return 0;
}