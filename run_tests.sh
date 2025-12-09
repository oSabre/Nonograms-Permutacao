#!/bin/bash

# Pasta contendo os arquivos de problema.
# O caminho deve ser no formato UNIX (usando barras /).
PASTA_PROBLEMAS="./samplestext"

# Número de execuções por problema
NUM_RUNS=10

echo "Iniciando testes de Busca Tabu no MinGW64..."
echo "----------------------------------------------"

# Loop para iterar sobre todos os arquivos na pasta de problemas
for ARQUIVO in "$PASTA_PROBLEMAS"/*
do
    # Verifica se o caminho é um arquivo regular
    if [ -f "$ARQUIVO" ]; then
        echo "Rodando problema: $ARQUIVO ($NUM_RUNS vezes)"
        
        # Loop interno para rodar o mesmo problema N vezes
        for (( i=1; i<=$NUM_RUNS; i++ ))
        do
            # Execução do programa
            # No Msys2/MinGW, você pode chamar o executável diretamente.
            # O '.' é necessário para executar o arquivo do diretório atual.
            ./nonogram.exe "$ARQUIVO"
        done
        echo "--- $ARQUIVO CONCLUÍDO ---"
    fi
done

echo "----------------------------------------------"
echo "Todos os testes finalizados."