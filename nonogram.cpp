#include "tabuleiro.h"
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <filesystem>

using namespace std;

extern const int webpbn1[];
const std::string NOME_ARQUIVO_CSV = "resultados_busca_tabu.csv";

void exportar_para_csv(double tempo, int melhor_custo, int pior_custo, int diff){
    std::fstream arquivo(NOME_ARQUIVO_CSV, std::ios::out | std::ios::app);

    if (!arquivo.is_open()) {
        std::cerr << "ERRO: Não foi possível abrir ou criar o arquivo CSV: " << NOME_ARQUIVO_CSV << std::endl;
        return;
    }

    arquivo.seekp(0, std::ios::beg);

    arquivo.seekp(0, std::ios::end);
    if (arquivo.tellp() == 0) {
        // Escreve o cabeçalho (nomes das colunas)
        arquivo << "Tempo_Médio,Melhor_Custo,Pior_custo,Diferença\n";
    }

    arquivo << tempo << ","
            << melhor_custo << ","
            << pior_custo << ","
            << diff << "\n";

    arquivo.close();
}

int main(int argc, char const *argv[]){
    const int REINICIOS = 5;
    const double TAXA_PERTUBACAO = 0.50;
    vector<ResultadoExecucao> info;
    int menor_custo = 99999;

    if (argc < 2) {
        cerr << "Uso: " << argv[0] << " <caminho_para_arquivo_do_puzzle>" << endl;
        cerr << "Exemplo: " << argv[0] << " samplestext/5.txt" << endl;
        return 1;
    }

    const string puzzle_file_path = argv[1];

    //Carrega o nonograma
    Nonograma meu_tabuleiro = carregar_nonograma(puzzle_file_path);
    Nonograma best_result = meu_tabuleiro;

    //Tenta carregar
    if (meu_tabuleiro.colunas == 0 || meu_tabuleiro.linhas == 0) {
        cerr << "Falha ao carregar o tabuleiro. Verifique o arquivo e o caminho." << endl;
        return 1;
    }

    //Print tabuleiro
    //cout << "\nTabuleiro Inicial:\n";
    //imprimir_tabuleiro(meu_tabuleiro);

    for(int i = 0; i < REINICIOS-1; ++i){
        ResultadoExecucao info_aux = solve_tabu(meu_tabuleiro);
        info.push_back(info_aux);
        if(info[i].melhor_custo < menor_custo){
            best_result = meu_tabuleiro;
            menor_custo = info[i].melhor_custo;
        }
        perturbar_nonograma(meu_tabuleiro, TAXA_PERTUBACAO);
    }

    ResultadoExecucao info_aux = solve_tabu(meu_tabuleiro);
    info.push_back(info_aux);
    if(info[REINICIOS-1].melhor_custo < menor_custo){
            best_result = meu_tabuleiro;
            menor_custo = info[REINICIOS-1].melhor_custo;
        }
    //imprimir_tabuleiro(best_result);

    int maior_custo_final = 0;
    int menor_custo_final = 99999;
    double acumulador = 0;
    double tempo_medio = 0;
    for(int i = 0; i < info.size(); ++i){
        if(info[i].melhor_custo < menor_custo_final){
            menor_custo_final = info[i].melhor_custo;
        }
        if(info[i].pior_custo > maior_custo_final){
            maior_custo_final = info[i].pior_custo;
        }
        acumulador += info[i].tempo_ms;
    }
    int dif = maior_custo_final - menor_custo_final;
    exportar_para_csv(acumulador/REINICIOS, menor_custo_final, maior_custo_final, dif);

    return 0;
}

