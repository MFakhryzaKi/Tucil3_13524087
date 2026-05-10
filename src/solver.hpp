#pragma once
#include "io.hpp"

extern int iter_count;

pair<int, string> solveUCS (int heuristik);
pair<int, string> solveGBFS (int heuristik);
pair<int, string> solveAStar (int heuristik);

int getPilihanAlgoritma ();
int getPilihanHeuristik (int pilihanAlgoritma);

pair<int, string> startSolve (int pilihanAlgoritma, int pilihanHeuristik);