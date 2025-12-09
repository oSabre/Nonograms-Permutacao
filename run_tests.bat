@echo off
SETLOCAL

:: Pasta contendo os arquivos de problema
SET PASTA_PROBLEMAS=samplestext

:: Número de execuções por problema
SET NUM_RUNS=10

ECHO Iniciando testes de Busca Tabu...
ECHO -----------------------------------

:: Loop para iterar sobre todos os arquivos na pasta de problemas
:: O /R percorre subpastas, mas o %%f na pasta especificada funciona para listar arquivos.
FOR %%f IN ("%PASTA_PROBLEMAS%\*") DO (
    ECHO Rodando problema: "%%f" (%NUM_RUNS% vezes)
    
    :: Loop interno para rodar o mesmo problema N vezes
    FOR /L %%i IN (1, 1, %NUM_RUNS%) DO (
        :: Execução do programa, passando o nome do arquivo entre aspas.
        nonogram.exe "%%f"
    )
    ECHO --- "%%f" CONCLUÍDO ---
)

ECHO -----------------------------------
ECHO Todos os testes finalizados.
ENDLOCAL
PAUSE