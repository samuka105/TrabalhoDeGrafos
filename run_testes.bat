@echo off
echo Compilando...
g++ -o testes.exe testes.cpp src/grafo_matriz.cpp src/grafo_lista.cpp src/util.cpp -Iinclude -std=c++17 -Wall -Werror
if %errorlevel% neq 0 (
    echo Erro na compilacao!
    pause
    exit
)
echo Executando testes...
testes.exe
echo Verificando vazamentos de memoria...
valgrind --leak-check=full testes.exe
pause