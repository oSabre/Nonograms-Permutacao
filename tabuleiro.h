#ifndef TABULEIRO_H
#define TABULEIRO_H


#include <vector>
#include <iostream>
#include <string>
#include <fstream>
using namespace std;

using Restricoes = vector<int>;

struct Nonograma{
    int linhas = 0;
    int colunas = 0;

    vector<Restricoes> col_restr;
    vector<Restricoes> lin_restr;

    vector<vector<int>> grid;
};
Nonograma carregar_nonograma(const string& path);
void imprimir_tabuleiro(const Nonograma& instancia);
void solve_nonogram(Nonograma& instancia);

#endif