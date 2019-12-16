#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
using namespace std;

int part1(vector<int> v, int h, int w) {
    //find fewZeroLayer & cal the number of digit 1, 2 & multiply them.
    vector<int> zeros = vector<int>{};
    vector<int> ones = vector<int>{};
    vector<int> twos = vector<int>{};
    const int layer_pix = h * w;
    for (int i = 0; i < v.size(); i++) {
        if (i % layer_pix == 0) {
            zeros.push_back(0);
            ones.push_back(0);
            twos.push_back(0);
        }
        switch(v[i]) {
            case 0: {
                zeros[i / layer_pix]++;
                break;
            }
            case 1: {
                ones[i / layer_pix]++;
                break;
            }
            case 2: {
                twos[i / layer_pix]++;
                break;
            }
            default: {
                break;
            }
        }
    }
    int fewest_z_layer = 0;
    for (int i = 1; i < zeros.size(); i++) {
        fewest_z_layer = zeros[i] < zeros[fewest_z_layer] ? i : fewest_z_layer;
    }
    return ones[fewest_z_layer] * twos[fewest_z_layer];
}

void part2(vector<int> v, int h, int w) {
    int img[h][w] = {};
    bool settled[h][w] = {false};
    const int layer_size = h * w;
    for (int i = 0; i < v.size(); i++) {
        const int layer_idx = i / layer_size;
        const int pos = i - layer_size * layer_idx;
        const int h_pos = pos / w;
        const int w_pos = pos % w;

        if (!settled[h_pos][w_pos]) {
            if (v[i] == 2) continue;
            img[h_pos][w_pos] = v[i];
            settled[h_pos][w_pos] = true;
        }
    }
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            switch (img[i][j]) {
                case 0: {
                    cout << "-";
                    break;
                }
                case 1: {
                    cout << "O";
                    break;
                }
                case 2: {
                    cout << "_";
                }
            }
        }
        cout << endl;
    }
}


int main()
{
    ifstream fin("./input.txt");
    string s;
    getline(fin, s);
    istringstream ss(s);
    vector<int> v;
    while (ss)
    {
        char a;
        if (!ss.get(a))
            break;
        v.push_back(a - '0');
    }
    // now we have the 25x6 image as a vector.
    cout << "Ans to part1 is: " << part1(v, 6, 25) << endl;
    cout << "Ans to part2 is: \n" << endl;
    part2(v, 6, 25);
    return 0;
}