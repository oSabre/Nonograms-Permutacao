#include "tabuleiro.h"
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <filesystem>

using namespace std;

extern const int webpbn1[];

int main(int argc, char const *argv[]){
    const string puzzle_file_path = "samplestext/3.txt"; 

    //Carrega o nonograma
    Nonograma meu_tabuleiro = carregar_nonograma(puzzle_file_path);

    //Tenta carregar
    if (meu_tabuleiro.colunas == 0 || meu_tabuleiro.linhas == 0) {
        cerr << "Falha ao carregar o tabuleiro. Verifique o arquivo e o caminho." << endl;
        return 1;
    }

    //Print tabuleiro
    cout << "\nTabuleiro Inicial:\n";
    imprimir_tabuleiro(meu_tabuleiro);
    solve_nonogram(meu_tabuleiro);


    return 0;
}

