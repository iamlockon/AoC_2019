#include <iostream>
#include <string>
using namespace std;

bool isValid(const int i) {
    bool twoAdj = false;
    string s = to_string(i);
    for (int j = 0; j < s.size() - 1; j++) {
        if (s[j] == s[j+1]) twoAdj = true;
        if (s[j] > s[j+1]) return false;
    }
    return twoAdj;
}

bool isValid2(const int i) {
    bool onlyTwoAdj = false;
    string s = to_string(i);
    int j = 0;
    while (j < s.size() - 1) {
        int repeat = 0;
        while (s[j] == s[j+1]) {
            repeat++;
            j++;
        }
        if (repeat == 1) {
            onlyTwoAdj = true;
        }
        if (j+1 < s.size() && s[j] > s[j+1]) return false;
        j++;
    }
    return onlyTwoAdj;
}


void part1(int start, int end) {
    int count = 0;
    for (int i = start; i <= end; i++) {
        if (isValid(i)) count++;
    }
    cout << "Ans to part1 is :" << count << endl;
}

void part2(int start, int end) {
    int count = 0;
    for (int i = start; i <= end; i++) {
        if (isValid2(i)) count++;
    }
    cout << "Ans to part2 is :" << count << endl;
}


int main() {
    int start = 234208;
    int end = 765869;
    part1(start, end);
    part2(start, end);
    return 0;
}