#include "tabuleiro.h"
#include <iostream>
#include <vector>
#include <numeric>

using namespace std;

Nonograma carregar_nonograma(const string& path){
    Nonograma instancia;
    ifstream file(path);

    if (!file.is_open()) {
        cerr << "ERRO: Não foi possível abrir o arquivo : " << path << endl;
        return instancia; 
    }

    if (!(file >> instancia.colunas >> instancia.linhas)) {
        cerr << "ERRO: Falha ao ler dimensoes do arquivo: " << path << endl;
        return instancia;
    }

    instancia.col_restr.resize(instancia.colunas);
    instancia.lin_restr.resize(instancia.linhas);

    instancia.grid.assign(instancia.linhas, vector<int>(instancia.colunas, 1));

    for (int c = 0; c < instancia.colunas; ++c) {
        int num_restr;
        
        if (!(file >> num_restr)) {
             cerr << "ERRO: Dados insuficientes ao ler contagem para Coluna " << c << " em " << path << endl;
             return instancia;
        }
        instancia.col_restr[c].reserve(num_restr);

        for (int i = 0; i < num_restr; i++) {
            int pista;
            if (!(file >> pista)) {
                 cerr << "ERRO: Pistas insuficientes para Coluna " << c << " em " << path << endl;
                 return instancia;
            }
            instancia.col_restr[c].push_back(pista);
        }
    }

    for (int r = 0; r < instancia.linhas; ++r) {
        int num_restr;

        if (!(file >> num_restr)) {
            cerr << "ERRO: Dados insuficientes ao ler contagem para Linha " << r << " em " << path << endl;
            return instancia;
        }
        instancia.lin_restr[r].reserve(num_restr);

        for (int i = 0; i < num_restr; ++i) {
            int pista;
            if (!(file >> pista)) {
                 cerr << "ERRO: Pistas insuficientes para Linha " << r << " em " << path << endl;
                 return instancia;
            }
            instancia.lin_restr[r].push_back(pista);
        }
    }

    cout << "Nonograma " << instancia.colunas << "x" << instancia.linhas << " carregado com sucesso!" << endl;
    return instancia;
}

void imprimir_tabuleiro(const Nonograma& instancia){
    cout << "\n--- ESTADO ATUAL DO TABULEIRO (" << instancia.linhas << "x" << instancia.colunas << ") ---\n";

    size_t max_col_clues = 0;
    for (const auto& clues : instancia.col_restr) {
        if (clues.size() > max_col_clues) {
            max_col_clues = clues.size();
        }
    }

    size_t max_row_clues = 0;
    for (const auto& clues : instancia.lin_restr) {
        if (clues.size() > max_row_clues) {
            max_row_clues = clues.size();
        }
    }

    size_t clue_width = max_row_clues * 3; 

    for (size_t i = 0; i < max_col_clues; ++i) {

        cout << string(clue_width, ' ');
        
        for (const auto& col_clues : instancia.col_restr) {

            if (i < col_clues.size()) {
                cout << " " << col_clues[i] << " ";
            } else {
                cout << "   ";
            }
        }
        cout << endl;
    }
    
    cout << string(clue_width, ' ') << string(instancia.colunas * 3, '-') << endl;

    for (int i = 0; i < instancia.linhas; ++i) {
        const auto& row_clues = instancia.lin_restr[i];
        
        string row_clue_string = "";
        for (int clue : row_clues) {
            row_clue_string += to_string(clue) + " ";
        }
        
        cout << row_clue_string;
        cout << string(clue_width - row_clue_string.length(), ' ');
        
        for (int j = 0; j < instancia.colunas; ++j) {
            int cell = instancia.grid[i][j];
            
            if (cell == 1) {
                cout << " # "; // Célula Preta/Preenchida
            } else if (cell == -1) {
                cout << " . "; // Célula Branca/Vazia
            } else {
                cout << " ? "; // Célula Desconhecida (0)
            }
        }
        cout << endl;
    }
    cout << "------------------------------------------\n";
}