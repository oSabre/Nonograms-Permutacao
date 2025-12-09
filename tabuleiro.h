#ifndef TABULEIRO_H
#define TABULEIRO_H


#include <vector>
#include <iostream>
#include <string>
#include <fstream>
using namespace std;

using Restricoes = vector<int>;

struct Movimento{
    int linha;
    int coluna;

    bool operator<(const Movimento& outro) const{
        if (linha != outro.linha) return linha < outro.linha;
        return coluna < outro.coluna;
    }

    bool operator==(const Movimento& outro) const{
        return linha == outro.linha && coluna == outro.coluna;
    }
};

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

Restricoes extrair_blocos( const Nonograma& instancia, bool is_linha, int indice);
int calcular_custo_linha_coluna(const Restricoes& restr_atuais, const Restricoes& restr_ideais);
int calcular_custo_total(const Nonograma& instancia);
int solve_tabu(Nonograma& instancia);

void aplicar_movimento(Nonograma& instancia, const Movimento& mov);
void perturbar_nonograma(Nonograma& instancia, double taxa_pertubacao);

#endif