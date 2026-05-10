# Tucil3_13524087
**Nama:** Muhammad Fakhry Zaki  
**NIM:** 13524087

## Pendahuluan
Program ini adalah implementasi tiga algoritma pencarian jalur terbaik (**Uniform Cost Search**, **Greedy Best-First Search**, dan **A\* Search**) untuk memecahkan masalah permainan Ice Sliding. 

Dalam permainan ini, pemain harus menemukan jalur dengan algoritma **UCS**, **GBFS**, atau **A\*** dari posisi awal menuju goal (petak 'O') sambil melewati semua angka (0-9) secara berurutan. Program ini juga menghitung iterasi pencarian dan durasi eksekusi algoritma.

## Deskripsi Program

### Algoritma Pencarian
1. **UCS (Uniform Cost Search)** - Algoritma pencarian yang ekspansi node berdasarkan cost terendah
2. **GBFS (Greedy Best-First Search)** - Algoritma yang menggunakan heuristik (Manhattan atau Euclidean distance)
3. **A\* Search** - Kombinasi dari UCS dan GBFS dengan formula yang menggabungkan cost terendah sekarang dan heuristik

### Contoh Input
```
7 7
XXXXXXX
X0****X
X**X**X
X****OX
X1***LX
XZ**X*X
XXXXXXX
999 999 999 999 999 999 999
999 3   5   2   8   1   999 
999 7   4   999 6   9   999
999 2   8   3   5   4   999 
999 6   1   7   2   999 999
999 9   3   4   999 8   999 
999 999 999 999 999 999 999 
```
### Penjelasan Input
- Baris pertama berisi N dan M yang menyatakan dimensi papan
- Blok baris selanjutnya berisi kode (Z: player, X: tembok, 0-9: angka, L: lava, *: jalan) dari masing-masing petak pada papan
- Blok baris terakhir berisi cost dari masing-masing petak 


## Menjalankan Program

### 1. Kompilasi Program
Pastikan Anda berada di direktori root (Tucil3_13524087), kemudian jalankan:
```bash
g++ -o .\bin\main .\src\main.cpp .\src\Node.cpp .\src\io.cpp .\src\solver.cpp
```

### 2. Jalankan Program
```bash
.\bin\main.exe
```

### 3. Menggunakan Program
1. Program akan menampilkan dimensi papan dan konfigurasi awal
2. Pilih algoritma yang ingin digunakan (1: UCS, 2: GBFS, 3: A*)
3. Jika memilih GBFS atau A*, pilih heuristik (1: Manhattan, 2: Euclidean)
4. Program akan menampilkan hasil dengan format:
   - Algoritma yang digunakan
   - Heuristik yang digunakan (jika ada)
   - Solusi (urutan gerakan: R/L/U/D)
   - Cost total
   - Waktu eksekusi
   - Jumlah iterasi
5. Pilih menu untuk melihat hasil lengkap, playback mode, atau keluar
6. Opsi untuk menyimpan output dan data iterasi ke file

## Output Program

### Output Terminal
Menampilkan:
- Initial state (papan dengan posisi Z)
- Setiap step dengan papan yang diupdate
- Summary hasil (algoritma, heuristik, cost, waktu, iterasi)

### File Output
1. **Output Result** - Berisi board state di setiap step dan info lengkap solusi
2. **Output Iterasi** - Berisi semua node yang dikunjungi selama proses pencarian
