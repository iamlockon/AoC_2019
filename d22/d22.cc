#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <cmath>

using namespace std;
using Actions = vector<pair<int, int>>;
using Num = long long;
const int CARD_COUNT = 10007;
const Num LARGE_CARD_COUNT = 119315717514047L;
// const Num LARGE_CARD_COUNT = 10;
const Num REPEATS = 101741582076661L;
const Num X = 2020;
const string NEW_STACK_STR = "deal into";
const string INCREMENT_STR = "deal with";

enum ACTION
{
    NEW_STACK = 1,
    CUT = 2,
    INCREMENT = 3
};

void cut(vector<int> &deck, int n)
{
    if (n > 0)
        rotate(deck.begin(), deck.begin() + n, deck.end());
    else
        rotate(deck.begin(), deck.begin() + CARD_COUNT + n, deck.end());
}

vector<int> increm(const vector<int> &deck, int n)
{
    vector<int> res(CARD_COUNT, 0);
    int curr = 0;
    for (int i = 0; i < CARD_COUNT; i++)
    {
        res[curr] = deck[i];
        curr = (curr + n) % CARD_COUNT;
    }
    return res;
}

Num binpowmod(Num a, Num b, Num m)
{
    a %= m;
    Num res = 1;
    while (b > 0)
    {
        if (b & 1)
            res = res * a % m;
        a = a * a % m;
        b >>= 1;
    }
    return res;
}

// Function for extended Euclidean Algorithm
Num gcdExtended(Num a, Num b, Num *x, Num *y);

// Function to find modulo inverse of a
Num modInverse(Num a, Num m)
{
    Num x, y;
    Num g = gcdExtended(a, m, &x, &y);
    if (g != 1)
        cout << "Inverse doesn't exist";
    else
    {
        // m is added to handle negative x
        Num res = (x % m + m) % m;
        return res;
    }
}

// Function for extended Euclidean Algorithm
Num gcdExtended(Num a, Num b, Num *x, Num *y)
{
    // Base Case
    if (a == 0)
    {
        *x = 0, *y = 1;
        return b;
    }

    Num x1, y1; // To store results of recursive call
    Num gcd = gcdExtended(b % a, a, &x1, &y1);

    // Update x and y using results of recursive
    // call
    *x = y1 - (b / a) * x1;
    *y = x1;

    return gcd;
}

void solution(const Actions &actions)
{
    vector<int> deck(CARD_COUNT, 0);
    for (int i = 0; i < CARD_COUNT; i++)
        deck[i] = i;
    for (auto &action : actions)
    {
        switch (action.first)
        {
        case NEW_STACK:
        {
            reverse(deck.begin(), deck.end());
            break;
        }
        case CUT:
        {
            cut(deck, action.second);
            break;
        }
        case INCREMENT:
        {
            deck = increm(deck, action.second);
            break;
        }
        }
    }
    auto it = find(deck.begin(), deck.end(), 2019);
    cout << "Ans to part1 is: " << it - deck.begin() << endl;

    auto func = [&](Num res) {
        for (auto it = actions.rbegin(); it != actions.rend(); it++)
        {
            switch (it->first)
            {
            case NEW_STACK:
            {
                res = (LARGE_CARD_COUNT - 1) - res;
                break;
            }
            case CUT:
            {
                res = (LARGE_CARD_COUNT + it->second + res) % LARGE_CARD_COUNT;
                break;
            }
            case INCREMENT:
            {
                res = modInverse(it->second, LARGE_CARD_COUNT) * res % LARGE_CARD_COUNT;
                break;
            }
            }
            if (res < 0)
            {
                res = (LARGE_CARD_COUNT + res);
            }
        }
        return res < 0 ? (res + LARGE_CARD_COUNT) : res;
    };

    const Num Y = func(X); // Y = AX + B
    const Num Z = func(Y); // Z = AY + B
    cout << "Y,Z = " << Y << ", " << Z << endl;
    const Num A = (Y - Z) * modInverse(X - Y + LARGE_CARD_COUNT, LARGE_CARD_COUNT) % LARGE_CARD_COUNT;
    // const Num A = (Y - Z) / (X - Y);
    const Num B = (Y - A * X) % LARGE_CARD_COUNT;
    // cout << "A,B = " << A << ", " << B << endl;
    const Num ans = (binpowmod(A, REPEATS % (LARGE_CARD_COUNT - 1), LARGE_CARD_COUNT) * X + (binpowmod(A, REPEATS % (LARGE_CARD_COUNT - 1), LARGE_CARD_COUNT) - 1) * modInverse(A - 1, LARGE_CARD_COUNT) * B) % LARGE_CARD_COUNT;
    cout << "Ans to part2 is: " << ans << endl;
}

int main()
{
    // ifstream fin("./d22/case.txt");
    ifstream fin("./d22/input.txt");
    string line;
    Actions actions;
    while (getline(fin, line))
    {
        if (line.substr(0, 9) == NEW_STACK_STR)
        { // deal into new stack
            actions.push_back({NEW_STACK, 0});
        }
        else if (line.substr(0, 9) == INCREMENT_STR)
        { // deal with increment X
            const int num = stoi(line.substr(20));
            actions.push_back({INCREMENT, num});
        }
        else
        { // cut X
            const int num = stoi(line.substr(4));
            actions.push_back({CUT, num});
        }
    }
    solution(actions);
    return 0;
}