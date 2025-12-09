#!/bin/bash

PASTA_PROBLEMAS="./samplestext"
NUM_RUNS=10
MAX_PARALELO=4 # Defina o número de núcleos que você deseja usar

# Variável para o nome do executável
EXEC_CMD="./nonogram.exe"

echo "Iniciando testes em paralelo com log em tempo real..."
echo "----------------------------------------------------"

# --- 1. GERAÇÃO DA LISTA DE COMANDOS (Encapsulando ECHO) ---
lista_comandos=$(
    for ARQUIVO in "$PASTA_PROBLEMAS"/*; do
        if [ -f "$ARQUIVO" ]; then
            for (( i=1; i<=$NUM_RUNS; i++ )); do
                # A string de log foi modificada para evitar ( )
                echo "echo \"[RUN $i] - ARQUIVO: $(basename "$ARQUIVO") Iteração $i\" && ${EXEC_CMD} \"$ARQUIVO\""
            done
        fi
    done
)

echo "Total de $(echo "$lista_comandos" | wc -l) execuções preparadas."
echo "Executando em paralelo (Máximo: $MAX_PARALELO processos)..."
echo "----------------------------------------------------"

# --- 2. EXECUÇÃO PARALELA COM XARGS ---
# -I {}: O comando gerado acima é substituído por {}
# bash -c {}: Executa a string gerada (que contém o ECHO e a chamada ao nonogram.exe)
# O uso de '&&' no comando encapsulado garante que nonogram.exe só rode se o echo for bem-sucedido.
echo "$lista_comandos" | xargs -P $MAX_PARALELO -I {} bash -c {}

echo "----------------------------------------------------"
echo "Todos os testes em paralelo finalizados com xargs."