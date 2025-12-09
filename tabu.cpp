#include "tabuleiro.h"
#include <iostream>
#include <vector>
#include <numeric>
#include <map>
#include <algorithm>
#include <random>
#include <cmath>
#include <chrono>

using namespace std;

random_device rd;
mt19937 gen(rd());

void perturbar_nonograma(Nonograma& instancia, double taxa_pertubacao){
    int total_celulas = instancia.linhas * instancia.colunas;
    int celulas_para_inverter = (int)ceil(total_celulas*taxa_pertubacao);

    uniform_int_distribution<> distrib_r(0, instancia.linhas - 1);
    uniform_int_distribution<> distrib_c(0, instancia.colunas - 1);

    for(int i = 0; i < celulas_para_inverter; ++i){
        int r = distrib_r(gen);
        int c = distrib_c(gen);

        Movimento mov = {r,c};
        aplicar_movimento(instancia, mov);
    }

}

void aplicar_movimento(Nonograma& instancia, const Movimento& mov){
    instancia.grid[mov.linha][mov.coluna] = 1 - instancia.grid[mov.linha][mov.coluna];
}

Restricoes extrair_blocos(const Nonograma& instancia, bool is_linha, int indice){
    Restricoes blocos;
    int contador = 0;
    int tamanho_max = is_linha ? instancia.colunas : instancia.linhas;

    for(int i = 0; i < tamanho_max; ++i){
        int celula = is_linha ? instancia.grid[indice][i] : instancia.grid[i][indice];
        
        if(celula == 1) {
            contador++;
        }else{
            if(contador > 0){
                blocos.push_back(contador);
            }
            contador = 0;
        }
    }

    if(contador > 0){
        blocos.push_back(contador);
    }
    return blocos;
}

int calcular_custo_linha_coluna(const Restricoes& restr_atuais, const Restricoes& restr_ideais){
    int custo = 0;

    const int PESO_BLOCO = 30;
    const int DIF_TAM = 15;

    if(restr_atuais.size() != restr_ideais.size()){
        custo += abs((int)restr_atuais.size() - (int)restr_ideais.size()) * PESO_BLOCO;
        int menor_tamanho = min(restr_atuais.size(), restr_ideais.size());
        for(size_t i = 0; i < menor_tamanho; ++i){
            custo += abs(restr_atuais[i] - restr_ideais[i]) * DIF_TAM;
        }
    }else{
        for(size_t i = 0; i < restr_atuais.size(); ++i){
            custo += abs(restr_atuais[i] - restr_ideais[i]) * DIF_TAM;
        }
    }
    return custo;
}

int calcular_custo_total(const Nonograma& instancia){
    int custo_total = 0;

    for(int i = 0; i < instancia.linhas; ++i){
        Restricoes blocos_atuais = extrair_blocos(instancia, true, i);
        custo_total += calcular_custo_linha_coluna(blocos_atuais, instancia.lin_restr[i]);
    }

    for(int j = 0; j < instancia.colunas; ++j){
        Restricoes blocos_atuais = extrair_blocos(instancia, false, j);
        custo_total += calcular_custo_linha_coluna(blocos_atuais, instancia.col_restr[j]);
    }

    return custo_total;
}

ResultadoExecucao solve_tabu(Nonograma& instancia){
    auto start = std::chrono::high_resolution_clock::now();
    const int MAX_ITERACOES = 500;
    const int DURACAO_TABU = 50;

    map<Movimento, int> lista_tabu;

    Nonograma melhor_solucao = instancia;
    int melhor_custo = calcular_custo_total(instancia);
    int pior_custo = 0;

    for(int iter = 0; iter < MAX_ITERACOES; ++iter){
        if(melhor_custo == 0){
            //cout << "Solucao otima encontrada na iteracao " << iter << endl;
            break;
        }

        Movimento melhor_mov_vizinho = {-1, -1};
        int melhor_custo_vizinho = -1;
        bool aspiracao = false;

        for(int r = 0; r < instancia.linhas; ++r){
            for(int c = 0; c < instancia.colunas; ++c){
                
                Movimento mov_candidato = {r,c};
                Nonograma solucao_vizinha = instancia;
                aplicar_movimento(solucao_vizinha, mov_candidato);
                int custo_vizinho = calcular_custo_total(solucao_vizinha);

                if(custo_vizinho < melhor_custo){
                    melhor_custo_vizinho = custo_vizinho;
                    melhor_mov_vizinho = mov_candidato;
                    aspiracao = true;
                }else if(lista_tabu.find(mov_candidato) == lista_tabu.end() || lista_tabu[mov_candidato] <= iter){
                    if(melhor_custo_vizinho == -1 || custo_vizinho < melhor_custo_vizinho){
                        melhor_custo_vizinho = custo_vizinho;
                        melhor_mov_vizinho = mov_candidato;
                        aspiracao = false;
                    }
                }

            }
        }

        if(melhor_mov_vizinho.linha != -1){
            aplicar_movimento(instancia, melhor_mov_vizinho);
            int novo_custo = melhor_custo_vizinho;

            if(!aspiracao){
                lista_tabu[melhor_mov_vizinho] = iter + DURACAO_TABU;
            }

            if(novo_custo < melhor_custo){
                melhor_custo = novo_custo;
                melhor_solucao.grid = instancia.grid;
            }

            if(novo_custo > pior_custo){
                pior_custo = novo_custo;
            }

            //cout << "Iteracao " << iter << ": Custo = " << novo_custo << " (Melhor: " << melhor_custo << ")" << endl;

        }else{
            //cout << "Busca estagnada na iteracao " << iter << endl;
            break;
        }
    }
    
    instancia.grid = melhor_solucao.grid;
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end - start;
    //cout << "Busca finalizada. Melhor custo: " << melhor_custo << endl;
    ResultadoExecucao retorno = {duration.count(),melhor_custo,pior_custo};
    return retorno;
}