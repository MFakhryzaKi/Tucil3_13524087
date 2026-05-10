#include "io.hpp"

Papan papan;
int N, M;
pair<int, int> posisiAwal;
pair<int, int> posisiSekarang;
int angkaTerbesar = -1;
int angkaDahDilewati = -1;
vector<pair<int, int>> posisiSemua;
int pilihanAlgoritma = 0;
int pilihanHeuristik = 0;
vector<NodeTetangga*> listIterasi;

bool adaZ, adaO;

void printError (string s) {
    cout << "Isi file tidak valid: " << s << endl; 
}

bool isInteger (string s) {
    try {
        size_t pos;
        stoi (s, &pos);
        return pos == s.length();
    }
    catch (...) {
        return false;
    }
}

char isPetaValid (string s) {
    for (char c : s) {
        char lower_c = tolower(c);
        if ((lower_c >= '0' && lower_c <= '9') || lower_c == 'x' || lower_c == '*' || lower_c == 'o' || lower_c == 'z' || lower_c == 'l') {
            continue;  
        }
        return c;  
    }
    return '\0';  
}

vector<string> parse(string s) {
    stringstream ss(s);
    string token;
    vector <string> res;

    while (ss >> token) {
        res.push_back(token);
    }

    return res;
}


pair<bool, int> verifyFile (ifstream &fileInput) {
    
    if (!fileInput.eof()) {
        // baris pertama
        string s;
        vector<string> v;

        getline (fileInput, s);
        v = parse (s);

        if (v.size() != 2) {
            printError ("input baris pertama tidak sama dengan 2");
            return {false, 1};
        }

        if (!isInteger(v[0]) || !isInteger(v[1])) {
            printError ("input baris pertama bukan integer");
            return {false, 1};
        }

        N = stoi (v[0]);
        M = stoi (v[1]);

        if (M > 1000 || N > 1000) {
            printError ("ukuran papan tidak boleh melebihi 1000");
            return {false, 1};
        }

        papan.n = N;
        papan.m = M;
        papan.initialize();

        // peta petak
        for (int i = 1; i <= N; i++) {
            if (fileInput.eof()) {
                printError ("baris input file tidak cukup");
                return {false, i + 1};
            }

            getline (fileInput, s);

            if (s.length() != M) {
                printError ("lebar papan tidak sama dengan yang disebutkan (" + to_string(M) + ")");
                return {false, i + 1};
            }

            char c = isPetaValid (s);
            if (c != '\0') {
                printError ("papan mengandung karakter yang tidak valid (" + string(1, c) + ")");
                return {false, i + 1};
            }

            for (int j = 0; j < s.size(); j++) {
                if (s[j] >= '0' && s[j] <= '9') {
                    angkaTerbesar = max (angkaTerbesar, int(s[j] - '0'));
                }

                if (s[j] == 'z' || s[j] == 'Z') {
                    adaZ = true;
                    posisiAwal = {i, j+1};
                    posisiSekarang = {i, j+1};
                    papan.petak[i][j+1]->kode = '*';
                }
                else {
                    if (s[j] == 'o' || s[j] == 'O') {
                        adaO = true;
                    }
                    papan.petak[i][j+1]->kode = s[j];
                }
            }
        }

        // cost tiap petak
        for (int i = 1; i <= N; i++) {
            if (fileInput.eof()) {
                printError ("baris input file tidak cukup");
                return {false, N + i + 1};
            }

            getline (fileInput, s);
            v = parse (s);

            if (v.size() != M) {
                printError ("ukuran petak yang diberi cost tidak sama dengan yang disebutkan  (" + to_string(M) + ")");
                return {false, N + i + 1};
            }
            
            for (int j = 0; j < v.size(); j++) {
                if (!isInteger(v[j])) {
                    printError ("ada cost yang bukan integer");
                    return {false, N + i + 1};
                }
            }

            vector<int> temp;
            for (int j = 0; j < v.size(); j++) {
                temp.push_back (stoi(v[j]));
            }

            for (int j = 0; j < temp.size(); j++) {
                papan.petak[i][j+1]->cost = temp[j];
            }
        }

        if (!fileInput.eof()) {
            printError ("isi file melebihi yang diperlukan");
            return {false, 2*N + 1};
        }
        else {
            return {true, 2*N + 1};
        }

    }
    else {
        printError ("file kosong");
        return {false, 0};
    }


}

void readAndVerifyFile () {
    cout << "SELAMAT DATANG DI GAME ICE SLIDING" << endl;
    while (true) {
        papan.clear();
        adaZ = false;
        adaO = false;

        cout << "Masukkan file input: ";
        string s;
        getline(cin, s);

        ifstream stream1 ("test/input/" + s);
        ifstream stream2 ("test/input/" + s + ".txt");

        if (!stream1.is_open() && !stream2.is_open()) {
            cout << "File dengan nama " << s << " tidak ditemukan!\n";
            continue;
        }
        else {
            ifstream fileInput;
            if (stream1.is_open()) {
                fileInput.open("test/input/" + s);
            }
            else {
                fileInput.open("test/input/" + s + ".txt");
            }

            pair<bool, int> valid = verifyFile(fileInput);
            if (!valid.first) {
                cout << "Error line: " << valid.second << endl; 
                continue;
            }
            else {
                if (adaO && adaZ) {
                    return;
                }
                else {
                    if (!adaO) {
                        printError ("Tidak ada petak finish!");
                    }
                    else {
                        printError ("Tidak ada petak start!");
                    }
                    continue;

                }
            }
        }
    }

}

void fillPosisi (pair<int, string> hasil) {
    posisiSemua.push_back (posisiAwal);
    pair<int, int> currentPos = posisiAwal;
    for (int i = 0; i < hasil.second.size(); i++) {
        char direction = hasil.second[i];
        string dirStr(1, direction);
        
        // Cari adjacent node dengan direction yang sesuai
        Node* currentNode = papan.petak[currentPos.first][currentPos.second];
        pair<int, int> nextPos = currentPos;
        
        for (auto tetangga : currentNode->adjacent) {
            if (tetangga->jalur == dirStr) {
                nextPos = {tetangga->node->baris, tetangga->node->kolom};
                break;
            }
        }
        
        currentPos = nextPos;
        posisiSemua.push_back (currentPos);
        
    }
}

void printSummaryResult (pair<int, string> hasil, int waktu, int iter) {
    string namaAlgoritma = "";
    if (pilihanAlgoritma == 1) {
        namaAlgoritma = "UCS";
    }
    else if (pilihanAlgoritma == 2) {
        namaAlgoritma = "GBFS";
    }
    else if (pilihanAlgoritma == 3) {
        namaAlgoritma = "A*";
    }
    
    string namaHeuristik = "-";
    if (pilihanHeuristik == 1) {
        namaHeuristik = "Manhattan distance";
    }
    else if (pilihanHeuristik == 2) {
        namaHeuristik = "Euclidean distance";
    }
    
    cout << "Algoritma : " << namaAlgoritma << endl;
    cout << "Heuristik : " << namaHeuristik << endl;
    cout << "Solusi Yang Ditemukan : " << hasil.second << endl;
    cout << "Cost dari Solusi : " << hasil.first << endl;
    cout << "Waktu Eksekusi : " << waktu << " microseconds" << endl;
    cout << "Jumlah Iterasi : " << iter << endl;
    cout << endl;
}

void printResult (pair<int, string> hasil, int waktu, int iter) {
    printSummaryResult (hasil, waktu, iter);

    cout << "Initial" << endl;
    papan.printPetak (posisiAwal);
    cout << endl;

    for (int i = 1; i < posisiSemua.size(); i++) {
        cout << "Step " << (i) << " : " << hasil.second[i] << endl;
        papan.printPetak (posisiSemua[i]);
        cout << endl;
    }

}

void showPanel (pair<int, string> hasil, int waktu, int iter) {
    while (true) {
        cout << "Apakah kamu ingin: " << endl;
        cout << "1. Print Hasil Keseluruhan" << endl;
        cout << "2. Playback" << endl;
        cout << "3. Quit" << endl;

        string s;
        getline (cin, s);
        if (s != "1" && s != "2" && s != "3") {
            cout << "Masukan harus merupakan angka 1-3" << endl;
            continue;
        }
        else {
            if (s == "1") {
                printResult (hasil, waktu, iter);
            }
            else if (s == "2") {
                showplaybackPanel (hasil);
            }
            else {
                break;
            }
        }
    }

    while (true) {
        cout << "Apakah kamu ingin menyimpan output ke file? (y/n): ";
        string s;
        getline (cin, s);

        if (s == "y") {
            saveOutput (hasil, waktu, iter);
            break;
        }
        else if (s == "n") {
            break;
        }
        else {
            cout << "Pilihan hanya y/n" << endl;
        }
    }

    while (true) {
        cout << "Apakah kamu ingin menyimpan iterasi ke file? (y/n): ";
        string s;
        getline (cin, s);

        if (s == "y") {
            saveIter (hasil, waktu, iter);
            break;
        }
        else if (s == "n") {
            break;
        }
        else {
            cout << "Pilihan hanya y/n" << endl;
        }
    }
}

void showplaybackPanel (pair<int, string> hasil) {
    int index = 0;
    while (true) {
        cout << "Mau mulai dari step ke berapa?(0 - " << hasil.second.size() << ")" << endl;
        string s;
        getline (cin, s);

        if (!isInteger(s)) {
            cout << "Input harus berupa integer!" << endl;
            continue;
        }
        
        int step = stoi(s);
        
        if (step < 0 || step > hasil.second.size()) {
            cout << "Input harus berada dalam range 0 - " << hasil.second.size() << endl;
            continue;
        }

        index = step;
        
        break;
    }

    while (true) {
        if (index != 0) {
            cout << "Step " << (index) << " : " << hasil.second[index-1] << endl;
        }
        else {
            cout << "Step 0" << endl;
        }
        papan.printPetak (posisiSemua[index]);
        cout << endl;

        while (true) {
            cout << "(n : next, b : back, q : quit)" << endl;
            string s;
            getline (cin, s);

            if (s != "n" && s != "b" && s != "q") {
                cout << "Input tidak valid" << endl;
                continue;
            }
            else {
                if (s == "n") {
                    if (index == hasil.second.size()) {
                        cout << "Sudah di step terakhir, tidak bisa maju lagi" << endl;
                        continue;
                    }
                    else {
                        index++;
                        break;
                    }
                }
                else if (s == "b") {
                    if (index == 0) {
                        cout << "Sudah di step pertama, tidak bisa mundur lagi" << endl;
                        continue;
                    }
                    else {
                        index--;
                        break;
                    }
                }
                else {
                    return;
                }
            }
        }

    }
}

void saveOutput (pair<int, string> hasil, int waktu, int iter) {
    string filename;
    while (true) {
        cout << "Masukkan nama file untuk output hasil: ";
        getline (cin, filename);
        
        // Tambahkan .txt kalo ga ada
        if (filename.length() < 4 || filename.substr(filename.length() - 4) != ".txt") {
            filename += ".txt";
        }
        break;
    }
    
    ofstream outputFile("test/output/" + filename);
    
    if (!outputFile.is_open()) {
        cout << "Gagal membuka file untuk ditulis!" << endl;
        return;
    }
    
    // Tentukan nama algoritma dan heuristik
    string namaAlgoritma = "";
    if (pilihanAlgoritma == 1) {
        namaAlgoritma = "UCS";
    }
    else if (pilihanAlgoritma == 2) {
        namaAlgoritma = "GBFS";
    }
    else if (pilihanAlgoritma == 3) {
        namaAlgoritma = "A*";
    }
    
    string namaHeuristik = "-";
    if (pilihanHeuristik == 1) {
        namaHeuristik = "Manhattan distance";
    }
    else if (pilihanHeuristik == 2) {
        namaHeuristik = "Euclidean distance";
    }
    
    // Tulis summary
    outputFile << "Algoritma : " << namaAlgoritma << endl;
    outputFile << "Heuristik : " << namaHeuristik << endl;
    outputFile << "Solusi Yang Ditemukan : " << hasil.second << endl;
    outputFile << "Cost dari Solusi : " << hasil.first << endl;
    outputFile << "Waktu Eksekusi : " << waktu << " microseconds" << endl;
    outputFile << "Jumlah Iterasi : " << iter << endl;
    outputFile << endl;
    
    // Tulis initial state
    outputFile << "Initial" << endl;
    for (int i = 1; i <= papan.n; i++) {
        for (int j = 1; j <= papan.m; j++) {
            if (posisiAwal == make_pair(i, j)) {
                outputFile << "Z ";
            }
            else {
                outputFile << papan.petak[i][j]->kode << ' ';
            }
        }
        outputFile << endl;
    }
    outputFile << endl;
    
    // Tulis setiap step
    for (int i = 1; i < posisiSemua.size(); i++) {
        outputFile << "Step " << (i) << " : " << hasil.second[i-1] << endl;
        for (int row = 1; row <= papan.n; row++) {
            for (int col = 1; col <= papan.m; col++) {
                if (posisiSemua[i] == make_pair(row, col)) {
                    outputFile << "Z ";
                }
                else {
                    outputFile << papan.petak[row][col]->kode << ' ';
                }
            }
            outputFile << endl;
        }
        outputFile << endl;
    }
    
    outputFile.close();
    cout << "Output berhasil disimpan ke test/output/" << filename << endl;
}

void saveIter (pair<int, string> hasil, int waktu, int iter) {
    string filename;
    while (true) {
        cout << "Masukkan nama file untuk output hasil iterasi: ";
        getline (cin, filename);
        
        // Tambahkan .txt kalo ga ada
        if (filename.length() < 4 || filename.substr(filename.length() - 4) != ".txt") {
            filename += ".txt";
        }
        break;
    }
    
    ofstream outputFile("test/output/" + filename);
    
    if (!outputFile.is_open()) {
        cout << "Gagal membuka file untuk ditulis!" << endl;
        return;
    }
    
    string namaAlgoritma = "";
    if (pilihanAlgoritma == 1) {
        namaAlgoritma = "UCS";
    }
    else if (pilihanAlgoritma == 2) {
        namaAlgoritma = "GBFS";
    }
    else if (pilihanAlgoritma == 3) {
        namaAlgoritma = "A*";
    }
    
    string namaHeuristik = "-";
    if (pilihanHeuristik == 1) {
        namaHeuristik = "Manhattan distance";
    }
    else if (pilihanHeuristik == 2) {
        namaHeuristik = "Euclidean distance";
    }
    
    // Tulis summary
    outputFile << "Algoritma : " << namaAlgoritma << endl;
    outputFile << "Heuristik : " << namaHeuristik << endl;
    outputFile << "Solusi Yang Ditemukan : " << hasil.second << endl;
    outputFile << "Cost dari Solusi : " << hasil.first << endl;
    outputFile << "Waktu Eksekusi : " << waktu << " microseconds" << endl;
    outputFile << "Jumlah Iterasi : " << iter << endl;
    outputFile << endl;

    // Tulis setiap iterasi (posisi node yang dikunjungi)
    outputFile << "Urutan Iterasi:" << endl;
    for (int i = 0; i < listIterasi.size(); i++) {
        outputFile << "Iter " << (i + 1) << " : (" << listIterasi[i]->node->baris << ", " << listIterasi[i]->node->kolom << ")" << endl;
    }
    
    outputFile.close();
    cout << "Iterasi berhasil disimpan ke test/output/" << filename << endl;
}
