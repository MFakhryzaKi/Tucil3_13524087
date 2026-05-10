#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <math.h>
#include <chrono>
#include "Node.hpp"

using namespace std;

extern int N, M;
extern Papan papan;
extern pair<int, int> posisiAwal;
extern pair<int, int> posisiSekarang;
extern int angkaTerbesar;
extern int angkaDahDilewati;
extern vector<pair<int, int>> posisiSemua;
extern int pilihanAlgoritma;
extern int pilihanHeuristik;
extern vector<NodeTetangga*> listIterasi;

void printError (string s);
bool isInteger (string s);
char isPetaValid (string s);
vector<string> parse(string s);
pair<bool, int> verifyFile (ifstream &fileInput);
void readAndVerifyFile ();
void fillPosisi (pair<int, string> hasil);
void printSummaryResult (pair<int, string> hasil, int waktu, int iter);
void printResult (pair<int, string> hasil, int waktu, int iter);
void showPanel (pair<int, string> hasil, int waktu, int iter);
void showplaybackPanel (pair<int, string> hasil);
void saveOutput (pair<int, string> hasil, int waktu, int iter);
void saveIter (pair<int, string> hasil, int waktu, int iter);