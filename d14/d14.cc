/**
 * Inspired by https://github.com/AKQuaternion/AdventOfCode2019/blob/master/day14.cpp
 *
 */

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>
#include <set>
#include <deque>
using namespace std;

using Number = unsigned long long;

struct Ingredient {
    string name;
    Number amount;
};

struct Reaction {
    Number outAmount;
    vector<Ingredient> ingredients;
};

typedef map<string, Reaction> Reactions;

void readInput(ifstream &fin, Reactions &reactions) {
    string s;
    stringstream ss;
    while (getline(fin, s)) {
        Reaction r;
        ss.str("");
        ss.clear();
        ss << s;
        bool done = false;
        Ingredient i{"", 0};
        while (!done) {
            ss >> i.amount >> i.name;
            if (i.name.back() == ',') {
                i.name.pop_back();
            }
            else {
                done = true;
            }
            r.ingredients.push_back(i);
        }
        string arrow;
        ss >> arrow >> i.amount >> i.name;
        reactions[i.name] = {i.amount, r.ingredients};
    }
}

void topoHelper(const string &name, const Reactions &reactions, set<string> &visited, deque<string> &order) {
    if (name == "ORE") return;
    visited.insert(name);
    for (const auto &ingredient: reactions.at(name).ingredients) {
        const string _name = ingredient.name;
        if (!visited.count(_name)) {
            topoHelper(_name, reactions, visited, order);
        }
    }
    order.push_front(name);
}

void topoSort(const string &name, const Reactions &reactions, deque<string> &order) {
    set<string> visited;
    topoHelper(name, reactions, visited, order);
}

void part12(const Reactions &reactions) {
    // Get topological order array
    deque<string> topoOrder;
    topoSort("FUEL", reactions, topoOrder);
    // reduce reactions to ore needed.

    auto makeFuel = [&](Number needed) {
        map<string, Number> amountNeeded{{"FUEL", needed}};
        for (const auto &chemical: topoOrder) {
            const auto &ingredients = reactions.at(chemical).ingredients;
            for (const auto &reactant: ingredients) {
                const Number timesToRun = amountNeeded[chemical] % reactions.at(chemical).outAmount > 0 ?
                 amountNeeded[chemical] / reactions.at(chemical).outAmount + 1 : amountNeeded[chemical] / reactions.at(chemical).outAmount;
                amountNeeded[reactant.name] += timesToRun * reactant.amount;
            }
        }
        return amountNeeded["ORE"];
    };
    cout << makeFuel(1) << endl;

    const Number TARGET = 1000000000000ull;
    //binary search
    Number lower = 0ull, upper = TARGET;
    while (lower < upper) {
        const Number mid = lower + (upper - lower) / 2;
        const Number ores = makeFuel(mid);
        if (ores < TARGET) {
            lower = mid + 1;
        }
        else {
            upper = mid - 1;
        }
    } 
    cout << lower << endl;
}


int main() {
    ifstream fin("./d14/input.txt");
    Reactions reactions;
    readInput(fin, reactions);
    part12(reactions);
    return 0;
}