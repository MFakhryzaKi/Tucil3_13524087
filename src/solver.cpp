#include "solver.hpp"

int iter_count = 0;

int telusuri (Node* awal, Node* akhir, string arah, int dicariSekarang) { 
    // 0 ga aman, 1 aman, 2 ngelewatin yang dicari

    int dbaris = 0, dkolom = 0;
    
    if (arah == "U") {
        dbaris = -1;
    }
    else if (arah == "D") {
        dbaris = 1;
    }
    else if (arah == "R") {
        dkolom = 1;
    }
    else if (arah == "L") {
        dkolom = -1;
    }
    
    int barisSekarang = awal->baris + dbaris;
    int kolomSekarang = awal->kolom + dkolom;
    Node* current = papan.petak[barisSekarang][kolomSekarang];
    
    // Telusuri sampai ketemu obstacle atau tujuan
    while (true) {
        if (current->isAngka()) {
            int angka = current->kode - '0';
            if (angka > dicariSekarang) {
                return 0; // ngelewatin angka yang belum bisa dikunjungi gaboleh
            }
            else if (angka == dicariSekarang) {
                return 2;
            }
        }

        if (current->baris == akhir->baris && current->kolom == akhir->kolom) {
            break;
        }
        
        // Lanjut bergerak ke arah tertentu
        barisSekarang += dbaris;
        kolomSekarang += dkolom;
        current = papan.petak[barisSekarang][kolomSekarang];
    }

    return 1; // ga ngelewatin apa2 jadi aman
}

pair<int, string> solveUCS (int heuristik) {
    // datengin ke semua angka dulu
    int hargaTotal = 0;
    string urutanGerakan = "";

    for (int i = 0; i <= angkaTerbesar; i++) {
        papan.setVisitedFalse();
        int hargaSekarang = 99999;
        string gerakanSekarang = "";
        priority_queue<NodeTetangga*, vector<NodeTetangga*>, CompareByCostAndHeuristik> pq;
        
        // Buat node baru dari posisi sekarang dengan visited = false
        NodeTetangga* nodeStart = new NodeTetangga (0, 0, papan.petak[posisiSekarang.first][posisiSekarang.second], "");
        pq.push (nodeStart);
        bool bisa = false;

        while (!pq.empty()) {
            NodeTetangga* curNode = pq.top();
            curNode->node->isVisited = true;
            pq.pop();

            if (curNode->totalCost >= hargaSekarang) {
                continue;
            }

            iter_count++;
            listIterasi.push_back (curNode);
            for (auto node : curNode->node->adjacent) {
                if (!node->node->isVisited) {
                    // Gunakan telusuri untuk validasi path dari curNode ke adjacent node
                    int validasi = telusuri(curNode->node, node->node, node->jalur, i);
                    
                    if (validasi == 0) {
                        // Path tidak aman (ada angka yang lebih besar dari i)
                        continue;
                    }
                    else if (validasi == 2) {
                        // Melewati angka i - ini adalah solusi yang kita cari!
                        if (node->totalCost + curNode->totalCost < hargaSekarang) {
                            hargaSekarang = node->totalCost + curNode->totalCost;
                            gerakanSekarang = curNode->jalur + node->jalur;
                        }
                        bisa = true;
                    }
                    else if (validasi == 1) {
                        // Path aman, lanjut BFS
                        NodeTetangga* newTetangga = new NodeTetangga (node->totalCost + curNode->totalCost, 0, node->node, curNode->jalur + node->jalur);
                        pq.push (newTetangga);
                    }
                }
            }
        }

        if (bisa) {
            // Update angka yang sudah dilewati dan posisi sekarang
            angkaDahDilewati = i;
            hargaTotal += hargaSekarang;
            urutanGerakan += gerakanSekarang;
            
            // Update posisi
            pair<int, int> pos = posisiSekarang;
            for (char dir : gerakanSekarang) {
                for (auto adj : papan.petak[pos.first][pos.second]->adjacent) {
                    if (adj->jalur[0] == dir) {
                        pos = {adj->node->baris, adj->node->kolom};
                        break;
                    }
                }
            }
            posisiSekarang = pos;
            
            continue;
        }
        else {
            cout << "TIDAK BISA MENCAPAI ANGKA " << i << endl;
            return {-1, ""}; 
        }
    }

    // ke finish
    papan.setVisitedFalse();
    int hargaSekarang = 99999;
    string gerakanSekarang = "";
    priority_queue<NodeTetangga*, vector<NodeTetangga*>, CompareByCostAndHeuristik> pq;
    
    NodeTetangga* nodeStart = new NodeTetangga (0, 0, papan.petak[posisiSekarang.first][posisiSekarang.second], "");
    pq.push (nodeStart);
    bool bisa = false;

    while (!pq.empty()) {
        NodeTetangga* curNode = pq.top();
        curNode->node->isVisited = true;
        pq.pop();

        if (curNode->totalCost >= hargaSekarang) {
            continue;
        }

        iter_count++;
        listIterasi.push_back (curNode);
        if (curNode->node->isFinish()) { // berenti di sini
            if (curNode->totalCost < hargaSekarang) {
                hargaSekarang = curNode->totalCost;
                gerakanSekarang = curNode->jalur;
            }
            bisa = true;
            posisiSekarang = {curNode->node->baris, curNode->node->kolom};
            continue;
        }

        for (auto node : curNode->node->adjacent) {
            if (!node->node->isVisited) {                
                // Path aman (validasi 1 atau 2), lanjut BFS
                NodeTetangga* newTetangga = new NodeTetangga (node->totalCost + curNode->totalCost, 0, node->node, curNode->jalur + node->jalur);
                pq.push (newTetangga);
            }
        }
    }

    if (bisa) {
        hargaTotal += hargaSekarang;
        urutanGerakan += gerakanSekarang;
        return {hargaTotal, urutanGerakan};
    }
    else {
        cout << "GAGAL: Tidak bisa mencapai goal" << endl;
        return {-1, ""};
    }
}

pair<int, string> solveGBFS (int heuristik) {
    bool manhattan = heuristik == 1;
    bool euclidean = heuristik == 2;

    int hargaTotal = 0;
    string urutanGerakan = "";

    for (int i = 0; i <= angkaTerbesar; i++) {
        papan.setVisitedFalse();
        int hargaSekarang = 99999;
        int hargaH = 99999;
        string gerakanSekarang = "";
        priority_queue<NodeTetangga*, vector<NodeTetangga*>, CompareByHeuristic> pq;

        
        // Buat node baru dari posisi sekarang dengan visited = false
        NodeTetangga* nodeStart = new NodeTetangga (0, 0, papan.petak[posisiSekarang.first][posisiSekarang.second], "");
        pq.push (nodeStart);
        bool bisa = false;

        while (!pq.empty()) {
            NodeTetangga* curNode = pq.top();
            curNode->node->isVisited = true;
            pq.pop();

            if (bisa) {
                break;
            }

            iter_count++;
            listIterasi.push_back (curNode);
            for (auto node : curNode->node->adjacent) {
                if (!node->node->isVisited) {
                    // Gunakan telusuri untuk validasi path dari curNode ke adjacent node
                    int validasi = telusuri(curNode->node, node->node, node->jalur, i);
                    int costHeuristik = 
                    manhattan*jarakManhattan (node->node->baris, node->node->kolom, papan.koordinatAngka[i].first, papan.koordinatAngka[i].second) + 
                    euclidean*jarakEuclidean (node->node->baris, node->node->kolom, papan.koordinatAngka[i].first, papan.koordinatAngka[i].second);
                    
                    if (validasi == 0) {
                        // Path tidak aman (ada angka yang lebih besar dari i)
                        continue;
                    }
                    else if (validasi == 2) {
                        hargaSekarang = node->totalCost + curNode->totalCost;
                        hargaH = costHeuristik;
                        gerakanSekarang = curNode->jalur + node->jalur;
                        bisa = true;
                    }
                    else if (validasi == 1) {
                        // Path aman, lanjut BFS
                        NodeTetangga* newTetangga = new NodeTetangga (node->totalCost + curNode->totalCost, costHeuristik, node->node, curNode->jalur + node->jalur);
                        pq.push (newTetangga);
                    }
                }
            }
        }

        if (bisa) {
            // Update angka yang sudah dilewati dan posisi sekarang
            angkaDahDilewati = i;
            hargaTotal += hargaSekarang;
            urutanGerakan += gerakanSekarang;
            
            // Update posisi ke endpoint dari gerakanSekarang terakhir
            pair<int, int> pos = posisiSekarang;
            for (char dir : gerakanSekarang) {
                for (auto adj : papan.petak[pos.first][pos.second]->adjacent) {
                    if (adj->jalur[0] == dir) {
                        pos = {adj->node->baris, adj->node->kolom};
                        break;
                    }
                }
            }
            posisiSekarang = pos;
            
            continue;
        }
        else {
            cout << "TIDAK BISA MENCAPAI ANGKA " << i << endl;
            return {-1, ""};
        }
    }

    // ke finish
    papan.setVisitedFalse();
    int hargaSekarang = 99999;
    string gerakanSekarang = "";
    priority_queue<NodeTetangga*, vector<NodeTetangga*>, CompareByHeuristic> pq;
    
    NodeTetangga* nodeStart = new NodeTetangga (0, 0, papan.petak[posisiSekarang.first][posisiSekarang.second], "");
    pq.push (nodeStart);
    bool bisa = false;

    while (!pq.empty()) {
        NodeTetangga* curNode = pq.top();
        curNode->node->isVisited = true;
        pq.pop();

        iter_count++;
        listIterasi.push_back (curNode);
        if (curNode->node->isFinish()) { // berenti di sini
            hargaSekarang = curNode->totalCost;
            gerakanSekarang = curNode->jalur;
            bisa = true;
            posisiSekarang = {curNode->node->baris, curNode->node->kolom};
            break;
        }

        for (auto node : curNode->node->adjacent) {
            if (!node->node->isVisited) {
                int costHeuristik = 
                manhattan*jarakManhattan (node->node->baris, node->node->kolom, papan.koordinatFinish.first, papan.koordinatFinish.second) + 
                euclidean*jarakEuclidean (node->node->baris, node->node->kolom, papan.koordinatFinish.first, papan.koordinatFinish.second);
                
                // Path aman (validasi 1 atau 2), lanjut BFS
                NodeTetangga* newTetangga = new NodeTetangga (node->totalCost + curNode->totalCost, costHeuristik, node->node, curNode->jalur + node->jalur);
                pq.push (newTetangga);
            }
        }
    }

    if (bisa) {
        hargaTotal += hargaSekarang;
        urutanGerakan += gerakanSekarang;
        return {hargaTotal, urutanGerakan};
    }
    else {
        cout << "GAGAL: Tidak bisa mencapai goal" << endl;
        return {-1, ""};
    }


}


pair<int, string> solveAStar (int heuristik) {
    bool manhattan = heuristik == 1;
    bool euclidean = heuristik == 2;

    int hargaTotal = 0;
    string urutanGerakan = "";

    for (int i = 0; i <= angkaTerbesar; i++) {
        papan.setVisitedFalse();
        int hargaSekarang = 99999;
        int hargaH = 99999;
        string gerakanSekarang = "";
        priority_queue<NodeTetangga*, vector<NodeTetangga*>, CompareByCostAndHeuristik> pq;
        
        // Buat node baru dari posisi sekarang dengan visited = false
        NodeTetangga* nodeStart = new NodeTetangga (0, 0, papan.petak[posisiSekarang.first][posisiSekarang.second], "");
        pq.push (nodeStart);
        bool bisa = false;

        while (!pq.empty()) {
            NodeTetangga* curNode = pq.top();
            curNode->node->isVisited = true;
            pq.pop();

            if (curNode->totalCost + curNode->heuristikCost >= hargaSekarang + hargaH) {
                continue;
            }

            iter_count++;
            listIterasi.push_back (curNode);
            for (auto node : curNode->node->adjacent) {
                if (!node->node->isVisited) {
                    // Gunakan telusuri untuk validasi path dari curNode ke adjacent node
                    int validasi = telusuri(curNode->node, node->node, node->jalur, i);
                    int costHeuristik = 
                    manhattan*jarakManhattan (node->node->baris, node->node->kolom, papan.koordinatAngka[i].first, papan.koordinatAngka[i].second) + 
                    euclidean*jarakEuclidean (node->node->baris, node->node->kolom, papan.koordinatAngka[i].first, papan.koordinatAngka[i].second);
                    
                    if (validasi == 0) {
                        // Path tidak aman (ada angka yang lebih besar dari i)
                        continue;
                    }
                    else if (validasi == 2) {
                        // Melewati angka i - ini adalah solusi yang kita cari!
                        if (node->totalCost + curNode->totalCost < hargaSekarang) {
                            hargaSekarang = node->totalCost + curNode->totalCost;
                            hargaH = costHeuristik;
                            gerakanSekarang = curNode->jalur + node->jalur;
                        }
                        bisa = true;
                    }
                    else if (validasi == 1) {
                        // Path aman, lanjut BFS
                        NodeTetangga* newTetangga = new NodeTetangga (node->totalCost + curNode->totalCost, costHeuristik, node->node, curNode->jalur + node->jalur);
                        pq.push (newTetangga);
                    }
                }
            }
        }

        if (bisa) {
            // Update angka yang sudah dilewati dan posisi sekarang
            angkaDahDilewati = i;
            hargaTotal += hargaSekarang;
            urutanGerakan += gerakanSekarang;
            
            // Update posisi ke endpoint dari gerakanSekarang terakhir
            pair<int, int> pos = posisiSekarang;
            for (char dir : gerakanSekarang) {
                for (auto adj : papan.petak[pos.first][pos.second]->adjacent) {
                    if (adj->jalur[0] == dir) {
                        pos = {adj->node->baris, adj->node->kolom};
                        break;
                    }
                }
            }
            posisiSekarang = pos;
            
            continue;
        }
        else {
            cout << "TIDAK BISA MENCAPAI ANGKA " << i << endl;
            return {-1, ""};
        }
    }

    // ke finish
    papan.setVisitedFalse();
    int hargaSekarang = 99999;
    string gerakanSekarang = "";
    priority_queue<NodeTetangga*, vector<NodeTetangga*>, CompareByCostAndHeuristik> pq;
    
    NodeTetangga* nodeStart = new NodeTetangga (0, 0, papan.petak[posisiSekarang.first][posisiSekarang.second], "");
    pq.push (nodeStart);
    bool bisa = false;

    while (!pq.empty()) {
        NodeTetangga* curNode = pq.top();
        curNode->node->isVisited = true;
        pq.pop();

        cout << curNode->node->baris << ' ' << curNode->node->kolom << endl;

        if (curNode->totalCost + curNode->heuristikCost >= hargaSekarang) {
            continue;
        }

        iter_count++;
        listIterasi.push_back (curNode);
        if (curNode->node->isFinish()) { // berenti di sini
            if (curNode->totalCost < hargaSekarang) {
                hargaSekarang = curNode->totalCost;
                gerakanSekarang = curNode->jalur;
            }
            bisa = true;
            posisiSekarang = {curNode->node->baris, curNode->node->kolom};
            continue;
        }

        for (auto node : curNode->node->adjacent) {
            if (!node->node->isVisited) {
                int costHeuristik = 
                manhattan*jarakManhattan (node->node->baris, node->node->kolom, papan.koordinatFinish.first, papan.koordinatFinish.second) + 
                euclidean*jarakEuclidean (node->node->baris, node->node->kolom, papan.koordinatFinish.first, papan.koordinatFinish.second);
                
                // Path aman (validasi 1 atau 2), lanjut BFS
                NodeTetangga* newTetangga = new NodeTetangga (node->totalCost + curNode->totalCost, costHeuristik, node->node, curNode->jalur + node->jalur);
                pq.push (newTetangga);
            }
        }
    }

    if (bisa) {
        hargaTotal += hargaSekarang;
        urutanGerakan += gerakanSekarang;
        return {hargaTotal, urutanGerakan};
    }
    else {
        cout << "GAGAL: Tidak bisa mencapai goal" << endl;
        return {-1, ""};
    }


}

int getPilihanAlgoritma () {
    while (true) {
        cout << "Algoritma apa yang ingin digunakan? (1-3)" << endl;
        cout << "1. UCS" << endl;
        cout << "2. GBFS" << endl;
        cout << "3. A*" << endl;

        string input;
        getline (cin, input);

        if (input != "1" && input != "2" && input != "3") {
            cout << "Input tidak valid! Masukkan angka antara 1-3" << endl;
        }
        else {
            return stoi(input);
        }
    }
}

int getPilihanHeuristik (int pilihanAlgoritma) {
    if (pilihanAlgoritma == 2 || pilihanAlgoritma == 3) {
        while (true) {
            cout << "Heuristik apa yang ingin digunakan? (1-3)" << endl;
            cout << "1. Manhattan distance" << endl;
            cout << "2. Euclidean distance" << endl;

            string input;
            getline (cin, input);

            if (input != "1" && input != "2") {
                cout << "Input tidak valid! Masukkan angka antara 1-2" << endl;
            }
            else {
                return stoi(input);
            }
        }
    }
    else {
        return 0;
    }
}

pair<int, string> startSolve (int pilihanAlgoritma, int pilihanHeuristik) {
    if (pilihanAlgoritma == 1) {
        return solveUCS (pilihanHeuristik);
    }
    else if (pilihanAlgoritma == 2) {
        return solveGBFS (pilihanHeuristik);
    }
    else if (pilihanAlgoritma == 3) {
        return solveAStar (pilihanHeuristik);
    }
}