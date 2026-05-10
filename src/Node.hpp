#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <queue>
#include <string>
#include <cstring>
#include <math.h>

using namespace std;

int jarakManhattan (int x1, int y1, int x2, int y2);
double jarakEuclidean (int x1, int y1, int x2, int y2);

class Node;

class NodeTetangga {
public:
    int totalCost;
    int heuristikCost;
    Node* node;
    string jalur; 
    NodeTetangga (int cost, int hcost, Node* n, string dir);
    
    // bool operator>(const NodeTetangga& other) const; // ini yang buat A* dan UCS
};

// ini khusus buat yang GBFS 
struct CompareByHeuristic {
    bool operator()(const NodeTetangga* a, const NodeTetangga* b) const {
        return a->heuristikCost > b->heuristikCost;
    }
};

// ini buat A* dan UCS
struct CompareByCostAndHeuristik {
    bool operator()(const NodeTetangga* n1, const NodeTetangga* n2) {
        return n1->totalCost + n1->heuristikCost > n2->totalCost + n2->heuristikCost; 
    }
};


class Node {
public:
    char kode;
    int cost;
    int baris, kolom;
    bool isVisited;
    vector<NodeTetangga*> adjacent;
    
    int ManhattanKeFinish;
    double EuclideanKeFinish;
    int ManhattanKeAngka[10];
    double EuclideanKeAngka[10];

    Node (int x, int y);

    bool isPath () { return kode == '*'; }
    bool isTembok () { return kode == 'X' || kode == 'x'; }
    bool isAngka () { return kode >= '0' && kode <= '9'; }
    bool isStart () { return kode == 'Z' || kode == 'z'; }
    bool isFinish () { return kode == 'O' || kode == 'o'; }
    bool isLava () { return kode == 'L' || kode == 'l'; }
};

class Papan {
public:
    int n, m;
    Node* petak[1000][1000];
    pair<int, int> koordinatFinish;
    pair<int, int> koordinatAngka[10];

    Papan () {}

    void initialize();

    void clear();

    void setVisitedFalse ();

    void printPetak (pair<int, int> posisiPlayer);

    void printCost ();

    void printTetangga ();

    void findFinishAndAngka ();

    void initAdjacent ();

    void initManhattan ();

    void initEuclidean ();
};