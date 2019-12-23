#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>
using namespace std;

typedef vector<vector<string>> Reactions;

void reduce(map<string, int> &ingre, const vector<string> formula, const string name) {
    const int unit_output = stoi(formula[formula.size()-2]);
    const int needed = ingre[name];
    const int multiples = needed % unit_output > 0 ? needed / unit_output + 1 : needed / unit_output;
    for (int i = 0; i < formula.size() - 2; i += 2) {
        const int coefficient = stoi(formula[i]) * multiples;
        ingre[formula[i+1]] += coefficient;
    }
    const int left_output = needed - unit_output * multiples;
    if (left_output) ingre[name] -= left_output;
}

void reduceInput(const vector<string> formula, vector<string> &input, int idx) {
    const int unit_output = stoi(formula[formula.size()-2]);
    const int needed = stoi(input[idx]);
    const int multiples = needed % unit_output > 0 ? needed / unit_output + 1 : needed / unit_output;
    for (int i = 0; i < formula.size() - 2; i += 2) {
        const int coefficient = stoi(formula[i]) * multiples;
        input.push_back(to_string(coefficient));
        input.push_back(formula[i+1]);
    }
    const int left_output = unit_output * multiples - needed;
    if (left_output) {
        input.push_back(to_string(-left_output));
        input.push_back(input[idx+1]);
    }
    input.erase(input.begin() + idx, input.begin() + idx + 2);
}

int inverseOutput(const vector<string> formula, vector<string> &input, const int amount, int idx) {
    const int unit_output = stoi(formula[formula.size()-2]);
    const int left = amount;
    const int multiples = left / unit_output;
    for (int i = 0; i < formula.size() - 2; i += 2) {
        const int coefficient = stoi(formula[i]) * multiples;
        input.push_back(to_string(-coefficient));
        input.push_back(formula[i+1]);
    }
    const int left_output = left - unit_output * multiples;
    if (left_output) {
        input.push_back(to_string(-left_output));
        input.push_back(input[idx+1]);
    }
    input.erase(input.begin() + idx, input.begin() + idx + 2);
    return left_output;
}

void part1(const Reactions &r) {
    // get map of "output to reaction vector idx"
    map<string, int> outPutToIdx;
    for (int i = 0; i < r.size(); i++) {
        const string output = r[i][r[i].size()-1];
        outPutToIdx.insert({output, i});
    }
    // find ore-equivalent output names.
    map<string, bool> equalOre;
    for (auto &re: r) {
        if (re.size() == 4 && re[1] == "ORE") { // since only reactions look like "N ORE => M A" (length 4) is true
            equalOre.insert({re[re.size()-1], true});
        } else {
            equalOre.insert({re[re.size()-1], false});
        }
    }    

    vector<string> input = r[outPutToIdx["FUEL"]];
    input.pop_back();
    input.pop_back();
    map<string, int> excessiveOutput;
    int idx = 0;
    while (idx+1 < input.size()) {
        if(equalOre[input[idx+1]]) idx += 2;
        else { // ex: 35 FEW
            const vector<string> formula = r[outPutToIdx[input[idx+1]]];
            if (stoi(input[idx]) < 0) {
                if (excessiveOutput.find(input[idx+1]) != excessiveOutput.end()) excessiveOutput[input[idx+1]] += stoi(input[idx]);
                else excessiveOutput[input[idx+1]] = stoi(input[idx]);
                const int unit_out = stoi(formula[formula.size() - 2]);
                if (unit_out + excessiveOutput[input[idx+1]] > 0) {
                    idx += 2;
                    continue;
                }
                const int amount = abs(excessiveOutput[input[idx+1]]);
                const string key = input[idx+1];
                const int left = inverseOutput(formula, input, amount, idx);
                excessiveOutput[key] = -left;
                continue;
            }
            reduceInput(formula, input, idx);
        }
    }

    // reduce input to ORE unit.
    map<string, int> ingredients;
    for (int i = 0; i < input.size(); i += 2) {
        if (ingredients.find(input[i+1]) != ingredients.end()) ingredients[input[i+1]] += stoi(input[i]);
        else ingredients[input[i+1]] = stoi(input[i]);
    }
    
    int res = 0;
    for (auto &ingredient: ingredients) {
        if (ingredient.second <= 0) continue;
        const vector<string> formula = r[outPutToIdx[ingredient.first]];
        const int unit_output = stoi(formula[formula.size()-2]);
        const int unit_input = stoi(formula[0]);
        const int multiples = ingredient.second % unit_output > 0 ?
             ingredient.second / unit_output + 1 : ingredient.second / unit_output;
        const int oreNeeded = unit_input * multiples;
        res += oreNeeded;
    }
    cout << res << endl;
}

int main() {
    ifstream fin("./d14/case.txt");
    string line;
    Reactions react;
    stringstream ss;
    while (getline(fin, line)) {
        ss << line;
        vector<string> v;
        while(ss) {
            string s;
            ss >> s;
            if (s == "=>") continue;
            if (s.size() > 0) {
                if (s.find(',') != string::npos) s = s.substr(0, s.find(',')); 
                v.push_back(s);
            }
        }
        react.push_back(v);
        // clean-up
        ss.str("");
        ss.clear();
    }
    part1(react);

    return 0;
}