#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <set>
#include <map>
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

    auto countAdj_r = [](const Grid &g, const map<int, Grid> &e, int l, int i , int j) {
        // rule: out of bound -> check in -1 level
        //       encounter center -> check in +1 level
        int res = 0;
        // check same level tiles
        if (i > 0 && g[i-1][j] == '#') res++; 
        if (j > 0 && g[i][j-1] == '#') res++; 
        if (i + 1 < SLEN && g[i+1][j] == '#') res++; 
        if (j + 1 < SLEN && g[i][j+1] == '#') res++; 
        // check different level tiles

        // outer level
        if (e.find(l-1) != e.end()) {
            const auto &outGrid = e.at(l-1);
            if (i == 0 && outGrid[1][2] == '#') res++;
            if (i == SLEN-1 && outGrid[3][2] == '#') res++;
            if (j == 0 && outGrid[2][1] == '#') res++;
            if (j == SLEN-1 && outGrid[2][3] == '#') res++;
        }

        // inner level
        if (e.find(l+1) != e.end()) {
            const auto &inGrid = e.at(l+1);
            if (i + 1 == 2 && j == 2) {
                for (int m = 0; m < SLEN; m++) {
                    if (inGrid[0][m] == '#') res++;
                }
            } 
            if (i - 1 == 2 && j == 2) {
                for (int m = 0; m < SLEN; m++) {
                    if (inGrid[SLEN-1][m] == '#') res++;
                }
            }
            if (i == 2 && j + 1 == 2) {
                for (int m = 0; m < SLEN; m++) {
                    if (inGrid[m][0] == '#') res++;
                }
            }
            if (i == 2 && j - 1 == 2) {
                for (int m = 0; m < SLEN; m++) {
                    if (inGrid[m][SLEN-1] == '#') res++;
                }
            }
        }
        
        return res;
    };
    auto createEmptyGrid = []() {
        Grid d;
        for (auto &a: d) {
            for (auto &c: a) {
                c = '.';
            }
        }
        return d;
    };

    auto alterBoundary = [&](map<int,Grid> &e) {
        const int i = e.rbegin()->first;
        const int o = e.begin()->first;
        // check outward
        auto &out = e.begin()->second;
        int ctru = 0, ctrd = 0;
        for (int m = 0; m < SLEN; m++) { // up and down
            if (out[0][m] == '#') ctru++;
            if (out[SLEN-1][m] == '#') ctrd++;
        } 
        int ctrl = 0, ctrr = 0;
        for (int m = 0; m < SLEN; m++) { // left and right
            if (out[m][0] == '#') ctrl++;
            if (out[m][SLEN-1] == '#') ctrr++;
        } 
        if (ctru == 1 || ctru == 2 || ctrd == 1 || ctrd == 2
            || ctrl == 1 || ctrl == 2 || ctrr == 1 || ctrr == 2) {
            e.insert({o-1, createEmptyGrid()});
        }  
        // check inward
        auto &in = e.end()->second;
        if (in[SLEN / 2 - 1][SLEN / 2] == '#' || in[SLEN / 2 + 1][SLEN / 2] == '#'
           || in[SLEN / 2][SLEN / 2 - 1] == '#' || in[SLEN / 2][SLEN / 2 + 1] == '#') {
            e.insert({i+1, createEmptyGrid()});
           }
    };

    auto evolve_r = [&](map<int, Grid> &e) {
        // bug -> empty unless exactly one bug adjecent
        // empty -> bug if one or two bug adjecent
        map<int, array<int, 25>> willChange;
        alterBoundary(e); // increase level if conditions meet
        for (auto &[k, g]: e) {
            willChange.insert({k, array<int,25>{}});
            for (int i = 0; i < SLEN; i++) {
                for (int j = 0; j < SLEN; j++) {
                    // skip center
                    if (i == SLEN / 2 && j == SLEN / 2) continue;
                    const int adj = countAdj_r(g, e, k, i, j);
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
                    // skip center
                    if (i == SLEN / 2 && j == SLEN / 2) continue;
                    if (willChange[k][5*i + j]) g[i][j] = g[i][j] == '#' ? '.' : '#';
                }
            }
        }
    };

    auto show = [&](const map<int, Grid> &e) {
        for (const auto &[k, g]: e) {
            cout << "Level " << k << ":" << endl;
            for (const auto &m: g) {
                for (const auto &c: m) {
                    cout << c;
                }
                cout << endl;
            }
            cout << endl;
        }
        cout << "========================" << endl;
    };

    auto recEvolve = [&](map<int, Grid> e, int rep) {
        for (int i = 0; i < rep; i++) {
            evolve_r(e);
            // show(e);
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
    map<int, Grid> eris; 
    eris.insert({0, cp});

    auto countBugs = [&](const map<int, Grid> &e) {
        int res = 0;
        for (const auto &[k,g]: e) {
            for (const auto &m: g) {
                for (const auto &c: m) {
                    if (c == '#') res++;
                }
            }
        }
        return res;
    };

    {
        int ans;
        auto last = recEvolve(eris, REP);
        // show(last);
        ans = countBugs(last);
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