#include "io.hpp"
#include "solver.hpp"

int main () {
    // preprocessing
    readAndVerifyFile();
    papan.findFinishAndAngka();
    papan.initManhattan();
    papan.initEuclidean();
    papan.initAdjacent();

    cout << N << " " << M << endl;
    papan.printPetak(posisiAwal);
    papan.printCost();
    // papan.printTetangga ();

    pilihanAlgoritma = getPilihanAlgoritma ();
    pilihanHeuristik = getPilihanHeuristik (pilihanAlgoritma);

    // process utama
    auto startTime = chrono::high_resolution_clock::now();
    pair<int, string> hasil = startSolve (pilihanAlgoritma, pilihanHeuristik);
    auto endTime = chrono::high_resolution_clock::now();
    auto duration_us = chrono::duration_cast<chrono::microseconds> (endTime - startTime);

    if (hasil.first == -1) {
        cout << "Tidak bisa mencapai goal dari file input" << endl;
    }
    else {
        fillPosisi (hasil);
        printSummaryResult (hasil, duration_us.count(), iter_count);
        showPanel (hasil, duration_us.count(), iter_count);
    }
    
}