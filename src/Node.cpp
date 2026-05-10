#include "Node.hpp"

int jarakManhattan (int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}

double jarakEuclidean (int x1, int y1, int x2, int y2) {
    return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

NodeTetangga::NodeTetangga (int cost, int hcost, Node* n, string dir) : 
    totalCost(cost), heuristikCost(hcost), node(n), jalur(dir) {}

// bool NodeTetangga::operator>(const NodeTetangga& other) const {
//     return this->totalCost + this->heuristikCost > other.totalCost + other.heuristikCost;
// }

Node::Node (int x, int y) : 
    kode('\0'), cost(0), baris(x), kolom(y), isVisited(false), ManhattanKeFinish(-1), EuclideanKeFinish(-1) 
{
    memset (ManhattanKeAngka, -1, sizeof (ManhattanKeAngka));
    memset (EuclideanKeAngka, -1, sizeof (EuclideanKeAngka));
}

void Papan::initialize() {
    koordinatFinish = {-1, -1};
    
    for (int i = 0; i < 10; i++) {
        koordinatAngka[i] = {-1, -1};
    }

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            petak[i][j] = new Node (i, j);
        }
    }
}

void Papan::clear() {
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            delete petak[i][j];
            petak[i][j] = nullptr;
        }
    }
}

void Papan::setVisitedFalse () {
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            petak[i][j]->isVisited = false;
        }
    } 
}

void Papan::printPetak (pair<int, int> posisiPlayer) {
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            if (posisiPlayer == make_pair (i, j)) {
                cout << "Z ";
            }
            else {
                cout << petak[i][j]->kode << ' ';
            }
        }
        cout << endl;
    }
}

void Papan::printCost () {
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            cout << petak[i][j]->cost << ' ';
        }
        cout << endl;
    }
}

void Papan::printTetangga () {
    cout << "\n=== ADJACENT NODES ===" << endl;
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            if (petak[i][j]->isLava() || petak[i][j]->isTembok()) {
                continue;
            }
            
            cout << "Petak (" << i << "," << j << ") [" << petak[i][j]->kode << "]:" << endl;
            
            if (petak[i][j]->adjacent.empty()) {
                cout << "  No adjacent nodes" << endl;
            } else {
                for (int k = 0; k < petak[i][j]->adjacent.size(); k++) {
                    NodeTetangga* tetangga = petak[i][j]->adjacent[k];
                    cout << "  -> (" << tetangga->node->baris << "," << tetangga->node->kolom << ") [" 
                         << tetangga->node->kode << "] | Cost: " << tetangga->totalCost 
                         << " | Direction: " << tetangga->jalur << endl;
                }
            }
        }
    }
    cout << "=====================\n" << endl;
}

void Papan::findFinishAndAngka () {
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            if (petak[i][j]->isFinish()) {
                koordinatFinish = make_pair (i, j);
            }
            else if (petak[i][j]->isAngka()) {
                int temp = petak[i][j]->kode - '0';
                koordinatAngka[temp] = make_pair (i, j);
            }
        }
    }
}

void Papan::initAdjacent () {
    // 4 arah: kiri, kanan, atas, bawah (B, T, U, S)
    int dx[] = {-1, 1, 0, 0};
    int dy[] = {0, 0, -1, 1};
    string arah[4];
    arah[0] = "U";
    arah[1] = "D";
    arah[2] = "L";
    arah[3] = "R";
    
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            // Skip kalo Lava atau Tembok
            if (petak[i][j]->isLava() || petak[i][j]->isTembok()) {
                continue;
            }
            
            // Cek 4 arah
            for (int dir = 0; dir < 4; dir++) {
                int x = i;
                int y = j;
                int lastValidX = i;
                int lastValidY = j;
                int totalCost = 0;
                
                // Gerak satu satu sampe ketemu obstacle atau batas
                while (true) {
                    x += dx[dir];
                    y += dy[dir];
                    
                    // Out of bounds, gunakan posisi terakhir yang valid
                    if (x < 1 || x > n || y < 1 || y > m) {
                        break;
                    }
                    
                    // Lava, jangan masukin ke adjacent
                    if (petak[x][y]->isLava()) {
                        break;
                    }
                    
                    // Tembok, tetangganya itu posisi terakhir yang valid 
                    if (petak[x][y]->isTembok()) {
                        if (lastValidX != i || lastValidY != j) {
                            NodeTetangga *tetangga = new NodeTetangga (totalCost, 0, petak[lastValidX][lastValidY], arah[dir]);
                            petak[i][j]->adjacent.push_back(tetangga);
                        }
                        break;
                    }
                    
                    // Ini adalah petak yang valid lanjutin
                    lastValidX = x;
                    lastValidY = y;
                    totalCost += petak[lastValidX][lastValidY]->cost;
                }
            }
        }
    }
}

void Papan::initManhattan () {
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            petak[i][j]->ManhattanKeFinish = jarakManhattan (i, j, koordinatFinish.first, koordinatFinish.second);
            for (int k = 0; k < 10; k++) {
                if (koordinatAngka[k] != make_pair(-1, -1)) {
                    petak[i][j]->ManhattanKeAngka[k] = jarakManhattan(i, j, koordinatAngka[k].first, koordinatAngka[k].second);
                }
            }
        }
    }
}

void Papan::initEuclidean () {
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            petak[i][j]->EuclideanKeFinish = jarakEuclidean(i, j, koordinatFinish.first, koordinatFinish.second);
            for (int k = 0; k < 10; k++) {
                if (koordinatAngka[k] != make_pair(-1, -1)) {
                    petak[i][j]->EuclideanKeAngka[k] = jarakEuclidean(i, j, koordinatAngka[k].first, koordinatAngka[k].second);
                }
            }
        }
    }
}