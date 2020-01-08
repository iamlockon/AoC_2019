/**
 * Give up after 10+ hours....Need to brush up on shortest graph algorithms.
 * https://github.com/bustercopley/advent2019/blob/master/18.cpp
 * 
 */
#include <iostream>
#include <fstream>
#include <map>
#include <set>
#include <string>
#include <vector>

using namespace std;

using field_t = vector<string>;


// Getting the keys and doors, map the character to their positions
map<char, array<int,2>> get_obj(const field_t &field) {
    map<char, array<int,2>> objects;
    int y = 0;
    for (const auto &line: field) {
        int x = 0;
        for (char c: line) {
            if (('@' <= c && c <= 'Z') || ('a' <= c && c <= 'z')) {
                objects.try_emplace(c, array<int, 2>{x, y});
            }
            ++x;
        }
        ++y;
    }
    return objects;
}

// From the given x,y, find all keys reachable and record the shortest distance (BFS)
map<char, int> get_accessible_keys(
    field_t field, const set<char> &keys_taken, int x, int y) {
    map<char, int> keys;
    set<array<int, 2>> access = {{x, y}};
    int distance = 1;
    field[y][x] = '@'; 

    while (!empty(access)) { // repeat until run out of element
        set<array<int,2>> new_access = {};
        for (auto p: access) { // for each element
            array<int, 2> locs[4] = {
                {p[0] - 1, p[1]}, // West
                {p[0] + 1, p[1]}, // East
                {p[0], p[1] - 1}, // North
                {p[0], p[1] + 1}, // South
            };
            for (auto loc: locs) { // for each direction
                char c = field[loc[1]][loc[0]];
                // "c | ' '" will add 32 for those char with '0' in 5th bit counting from LSB.
                // For uppercase letters this means effectively "convert c to lowercase."
                if (('a' <= (c | ' ') && (c | ' ') <= 'z' &&
                    keys_taken.count(c | ' ')) ||
                    c == '.') {
                    field[loc[1]][loc[0]] = '@';
                    new_access.insert({loc[0], loc[1]});
                } else if ('a' <= c && c <= 'z') {
                    keys.try_emplace(c, distance);
                }
            }
        }
        swap(access, new_access);
        ++distance;
    }
    return keys;
}

set<char> string_to_set(const string &s) {
    set<char> result;
    for (auto c: s) {
        result.insert(c);
    }
    return result;
}

string set_to_string(const set<char> &s) {
    string result;
    for (auto c: s) {
        result.push_back(c);
    }
    return result;
}

void part_one(field_t field) {
    auto objects = get_obj(field);
    string all_keys_list;
    {
        set<char> all_keys;
        for (const auto &pair: objects) {
            if ('a' <= pair.first && pair.first <= 'z') {
                all_keys.insert(pair.first);
            }
        }
        all_keys_list = set_to_string(all_keys);
    }
    int key_count = size(all_keys_list);
    auto [x, y] = objects['@'];
    field[y][x] = '.';

    // {[keys_already_taken][current_key], current_distance}
    map<string, int> optimal = {{"@", 0}};
    for (int i = 0; i < key_count; i++) {
        map<string, int> new_optimal;
        for (const auto &[keys_string, current_distance]: optimal) {
            char last_key = keys_string.back();
            auto keys_taken_list = keys_string;
            keys_taken_list.pop_back();

            auto keys_taken = string_to_set(keys_taken_list);
            auto [x,y] = objects[last_key];
            auto keys = get_accessible_keys(field, keys_taken, x, y);
            for (auto [key, distance]: keys) {
                auto ks = keys_taken;
                ks.insert(key);
                auto ks_list = set_to_string(ks);
                ks_list.push_back(key);
                auto &d = new_optimal[ks_list];
                if (!d || d > distance + current_distance) {
                    d = distance + current_distance;
                }
            }
        }
        swap(optimal, new_optimal);
    }
    int best = 1 << 30;
    for (auto [keys_string, distance]: optimal) {
        if (best > distance) {
            best = distance;
        }
    }
    cout << "Answer to part1: " << best << endl;
}

void part_two(field_t field) {
    auto objects = get_obj(field);
    string all_keys_list;
    {
        set<char> all_keys;
        for (const auto &pair: objects) {
            if ('a' <= pair.first && pair.first <= 'z') {
                all_keys.insert(pair.first);
            }
        }
        all_keys_list = set_to_string(all_keys);
    }
    const int key_count = size(all_keys_list);

    auto [x, y] = objects['@'];

    field[y - 1][x - 1] = '.';
    field[y - 1][x] = '#';
    field[y - 1][x + 1] = '.';

    field[y][x - 1] = '#';
    field[y][x] = '#';
    field[y][x + 1] = '#';

    field[y + 1][x - 1] = '.';
    field[y + 1][x] = '#';
    field[y + 1][x + 1] = '.';
    
    array<int,2> robots[4] = {
        {x - 1, y - 1},
        {x + 1, y - 1},
        {x - 1, y + 1},
        {x + 1, y + 1},
    };

    map<string, int> optimal = {{"@@@@", 0}};
    for (int i = 0; i < key_count; i++) {
        map<string, int> new_optimal;
        for (const auto &[keys_string, current_distance]: optimal) {
            auto keys_taken_list = keys_string;
            keys_taken_list.erase(
                begin(keys_taken_list) + i, end(keys_taken_list)
            );
            auto keys_taken = string_to_set(keys_taken_list);

            for (int j = 0; j < 4; j++) {
                string robots_list(
                    begin(keys_string) + i, end(keys_string)
                );
                char last_key = robots_list[j];
                auto [x, y] = objects[last_key];
                if (last_key == '@') {
                    x = robots[j][0];
                    y = robots[j][1];
                }
                auto keys = get_accessible_keys(field, keys_taken, x, y);
                for (auto [key, distance]: keys) {
                    auto ks = keys_taken;
                    ks.insert(key);
                    robots_list[j] = key;
                    auto keys_list = set_to_string(ks);
                    keys_list.append(robots_list);
                    auto &d = new_optimal[keys_list];
                    if (!d || d > distance + current_distance) {
                        d = distance + current_distance;
                    }
                }
            }
        }
        swap(optimal, new_optimal);
    }

    int best = 1 << 30;
    for (auto [keys_string, distance]: optimal) {
        if (best > distance) {
            best = distance;
        }
    }    
    cout << "Ans to part2 is : " << best << endl;
}

istream &read(istream &stream, field_t &field) {
    string line;
    while (getline(stream, line)) {
        if (!empty(line)) {
            field.push_back(std::move(line));
        }
    }
    return stream;
}

field_t read_file(const char *filename) {
    ifstream stream(filename);
    field_t field;
    read(stream, field);
    return field;
}

int main() {
    auto filename = "./d18/input.txt";
    auto field1 = read_file(filename);
    part_one(field1);

    auto field2 = read_file(filename);
    part_two(field2);
    return 0;
}