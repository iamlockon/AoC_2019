#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <memory>
using namespace std;

class Node {
    Node *parent;
public:
    string name;
    Node(string n): name(n), parent(nullptr) {};
    void setParent(Node *n) {
        this->parent = n;
    }
    Node *getParent() {
        return this->parent;
    }
};

class OrbitTree {
    private:
        vector<Node*> nodes;
        unordered_map<string, Node*> nodeMap;
    public:
    OrbitTree() {
        nodes = vector<Node*>{};
        nodeMap = unordered_map<string, Node*>{};
    }
    bool hasNode(string name) {
        return !!nodeMap[name];
    }
    void addNode(Node *n) {
        nodes.push_back(n);
        nodeMap[n->name] = n;
    }

    Node *getNode(string name) {
        return nodeMap[name];
    }
    int getOrbitSum() {
        // for all nodes, calculate accumulative heights.
        int res = 0;
        for (auto n : nodes) {
            Node *node = n;
            while(node->getParent()) {
                node = node->getParent();
                res++;
            }
        }
        return res;
    }
    int getMinTransfer(string a, string b) {
        Node *na = this->getNode(a)->getParent();
        Node *nb = this->getNode(b)->getParent();
        // assume both nodes exist.
        // traverse over to root for both nodes and record transfers,
        // find the first common node,
        // add the two transfer counts.
        unordered_map<string, int> txMap = unordered_map<string, int>{};
        int tx_a = 0;
        while (na->getParent()) {
            txMap[na->name] = tx_a++;
            na = na->getParent();
        }
        int tx_b = 0;
        while (nb->getParent()) {
            if (txMap[nb->name]) {
                return txMap[nb->name] + tx_b;
            }
            tx_b++;
            nb = nb->getParent();
        }
        return -1; // did not find common node.
    }
};

int part1(OrbitTree *tree) {
    return tree->getOrbitSum();
}

int part2(OrbitTree *tree, string a, string b) {
    return tree->getMinTransfer(a, b);
}

int main() {
    ifstream fin;
    fin.open("./d6/input.txt");
    if(!fin) {
        cout << "File I/O failed" << endl;
        return 1;
    }
    OrbitTree *tree = new OrbitTree;
    string s;
    while(getline(fin, s)) {
        size_t idx = s.find(')');
        string a = s.substr(0, idx);
        string b = s.substr(idx+1);
        if (!tree->hasNode(a)) {
            Node *na = new Node(a);
            tree->addNode(na);
        }
        if (!tree->hasNode(b)) {
            Node *nb = new Node(b);
            tree->addNode(nb);
        }
        tree->getNode(b)->setParent(tree->getNode(a));
    }
    // now we get full tree.
    cout << "Ans to part1 is: " << part1(tree) << endl;
    cout << "Ans to part2 is: " << part2(tree, "YOU", "SAN") << endl;
    return 0;
}
