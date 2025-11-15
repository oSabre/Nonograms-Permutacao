#include "tabuleiro.h"
#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <cmath>
#include <chrono>

using namespace std;
using namespace std::chrono;

bool permutacao_consistente(const vector<int>& cells, int start, int tamanho_bloco) {
    int tamanho_linha = cells.size();

    for (int k = start; k < start + tamanho_bloco; ++k) {
        if (cells[k] == -1) {
            return false;
        }
    }

    if (start > 0 && cells[start - 1] == 1) {
        return false;
    }

    int pos_separador = start + tamanho_bloco;
    if (pos_separador < tamanho_linha && cells[pos_separador] == 1) {
        return false;
    }
    
    return true;
}

void gerar_permutacao(
    int indice_bloco, 
    int posicao_atual, 
    vector<int> current_cells, 
    const Restricoes& restr, 
    vector<int>& contagem_preto, 
    vector<int>& contagem_branco, 
    int& total_permutacoes_validas) 
{
    int tamanho_linha = current_cells.size();

    while (posicao_atual < tamanho_linha && current_cells[posicao_atual] == -1) {
        posicao_atual++;
    }

    if (indice_bloco == restr.size()) {
        for (int k = posicao_atual; k < tamanho_linha; ++k) {
            if (current_cells[k] == 1) { 
                return; 
            }
        }
        
        total_permutacoes_validas++;
        for (int k = 0; k < tamanho_linha; ++k) {
            if (current_cells[k] == 1) {
                contagem_preto[k]++;
            } else {
                contagem_branco[k]++;
            }
        }
        return;
    }

    int tamanho_bloco = restr[indice_bloco];

    int tam_blocos_restantes = 0;
    for (size_t j = indice_bloco + 1; j < restr.size(); ++j) {
        tam_blocos_restantes += restr[j];
    }
    int espaco_req_rest = tam_blocos_restantes + (restr.size() - 1 - indice_bloco);
    
    int max_start = tamanho_linha - espaco_req_rest - tamanho_bloco;
    
    if (max_start < posicao_atual) {
        return; 
    }

    for (int start = posicao_atual; start <= max_start; ++start) {
        if (start > posicao_atual && current_cells[start - 1] == 1) {
            break; 
        }
        if (permutacao_consistente(current_cells, start, tamanho_bloco)) {
            vector<int> next_cells = current_cells;
            for (int k = start; k < start + tamanho_bloco; ++k) {
                next_cells[k] = 1; 
            }
            int pos_separador = start + tamanho_bloco;
            if (pos_separador < tamanho_linha) {
                next_cells[pos_separador] = -1; 
            }
            gerar_permutacao(
                indice_bloco + 1, 
                pos_separador + 1,
                next_cells, 
                restr, 
                contagem_preto, 
                contagem_branco, 
                total_permutacoes_validas
            );
        }
    }
}

bool aplicar_permutacoes(vector<int>& cells, const Restricoes& restr) {
    if (restr.empty()) {
        return true;
    }

    int tamanho_linha = cells.size();
    vector<int> contagem_preto(tamanho_linha, 0);
    vector<int> contagem_branco(tamanho_linha, 0);
    int total_permutacoes_validas = 0;

    gerar_permutacao(
        0, 
        0, 
        cells,
        restr, 
        contagem_preto, 
        contagem_branco, 
        total_permutacoes_validas
    );

    if (total_permutacoes_validas == 0) {
        return false; 
    }
    
    bool mudou = false;

    if (total_permutacoes_validas > 0) {
        for (int k = 0; k < tamanho_linha; ++k) {
            if (cells[k] == 0) {
                if (contagem_preto[k] == total_permutacoes_validas) {
                    cells[k] = 1; 
                    mudou = true;
                }
                else if (contagem_branco[k] == total_permutacoes_validas) {
                    cells[k] = -1; 
                    mudou = true;
                }
            }
        }
    }

    return true; 
}

bool is_valid(int r, int c, int linhas, int colunas) {
    return r >= 0 && r < linhas && c >= 0 && c < colunas;
}

bool encontrar_proxima_celula_desconhecida(const Nonograma& instancia, int& r, int& c) {
    for (r = 0; r < instancia.linhas; ++r) {
        for (c = 0; c < instancia.colunas; ++c) {
            if (instancia.grid[r][c] == 0) { // '?' ou 0
                return true;
            }
        }
    }
    return false;
}

bool linha_e_coluna_resolvida(const Nonograma& instancia) {
    for (int r = 0; r < instancia.linhas; ++r) {
        for (int c = 0; c < instancia.colunas; ++c) {
            if (instancia.grid[r][c] == 0) {
                return false;
            }
        }
    }
    return true;
}

bool propagar_e_verificar(Nonograma& instancia) {
    bool changed = true;
    int iterations = 0;
    
    while (changed && iterations < 50) { 
        changed = false;
        iterations++;

        for (int i = 0; i < instancia.linhas; ++i) {
            if (!aplicar_permutacoes(instancia.grid[i], instancia.lin_restr[i])) {
                return false;
            }
        }

        for (int j = 0; j < instancia.colunas; ++j) {
            vector<int> col;
            for (int i = 0; i < instancia.linhas; ++i) col.push_back(instancia.grid[i][j]);

            if (!aplicar_permutacoes(col, instancia.col_restr[j])) {
                return false;
            }

            for (int i = 0; i < instancia.linhas; ++i) {
                if (instancia.grid[i][j] != col[i]) {
                    instancia.grid[i][j] = col[i];
                    changed = true;
                }
            }
        }

        bool linha_mudou = false;
        for (int i = 0; i < instancia.linhas; ++i) {
             vector<int> linha_anterior = instancia.grid[i];
             if (!aplicar_permutacoes(instancia.grid[i], instancia.lin_restr[i])) return false;
             if (linha_anterior != instancia.grid[i]) changed = true;
        }

    }

    return true; 
}

bool backtrack(Nonograma& instancia) {
    int r, c;

    if (!encontrar_proxima_celula_desconhecida(instancia, r, c)) {
        return true; 
    }

    Nonograma copia_preto = instancia; 
    copia_preto.grid[r][c] = 1;
    
    if (propagar_e_verificar(copia_preto)) {
        if (backtrack(copia_preto)) {
            instancia = copia_preto; 
            return true;
        }
    }

    Nonograma copia_branco = instancia;
    copia_branco.grid[r][c] = -1;

    if (propagar_e_verificar(copia_branco)) {
        if (backtrack(copia_branco)) {
            instancia = copia_branco; 
            return true;
        }
    }

    return false; 
}



void solve_nonogram(Nonograma& instancia) {
    cout << "--- [FASE 1] Aplicando Permutacao Deterministica... ---\n";
    
    // Crônometro
    auto start_total = high_resolution_clock::now();

    int celulas_iniciais_desconhecidas = 0;
    for (int i = 0; i < instancia.linhas; ++i) {
        for (int j = 0; j < instancia.colunas; ++j) {
            if (instancia.grid[i][j] == 0) {
                celulas_iniciais_desconhecidas++;
            }
        }
    }

    // 1. Fase Deterministica 
    auto start_heuristica = high_resolution_clock::now();

    bool changed = true;
    int iterations = 0;
    while (changed && iterations < 100) { 
        changed = false;
        iterations++;
        
        for (int i = 0; i < instancia.linhas; ++i) {
            if (aplicar_permutacoes(instancia.grid[i], instancia.lin_restr[i])) changed = true;
        }
        for (int j = 0; j < instancia.colunas; ++j) {
            vector<int> col; 
            if (aplicar_permutacoes(col, instancia.col_restr[j])) {
                changed = true;
            }
        }
    }

    auto end_heuristica = high_resolution_clock::now();

    int celulas_apos_heuristica = 0;
    for (int i = 0; i < instancia.linhas; ++i) {
        for (int j = 0; j < instancia.colunas; ++j) {
            if (instancia.grid[i][j] == 0) {
                celulas_apos_heuristica++;
            }
        }
    }
    int celulas_resolvidas_heur = celulas_iniciais_desconhecidas - celulas_apos_heuristica;

    cout << "--- [FASE 2] Iniciando Backtracking... ---\n";
    
    // 2. Fase de Backtracking (Se ainda houver células '0')
    if (backtrack(instancia)) {
        cout << "SUCESSO: O Nonograma foi resolvido por Backtracking!\n";
    } else {
        cout << "FALHA: O Backtracking nao encontrou solucao.\n";
    }

    auto end_total = high_resolution_clock::now();
    duration<double> tempo_total = duration_cast<duration<double>>(end_total - start_total);

    // Imprime o resultado final
    imprimir_tabuleiro(instancia);

    duration<double> tempo_heuristica = duration_cast<duration<double>>(end_heuristica - start_heuristica);
    
    cout << "--- [RESULTADO HEURÍSTICA] ---\n";
    cout << "  Tempo de Execucao (Fase 1): " << tempo_heuristica.count() << " segundos\n";
    cout << "  Celulas Resolvidas: " << celulas_resolvidas_heur << " / " << celulas_iniciais_desconhecidas << " ("
         << (double)celulas_resolvidas_heur * 100.0 / celulas_iniciais_desconhecidas << "%)\n";

    cout << "--- [RESULTADO FINAL] ---\n";
    cout << "  Tempo Total de Execucao: " << tempo_total.count() << " segundos\n";
}